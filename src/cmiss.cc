#include "Utils.hh"
#include "injector/utility.hpp"
#include <cstring>
#include <CScrVM.hh>
#include <gtaThread.hh>
#include <unordered_map>
#include <random>
#include <ctime>
#include <eStats.hh>
#include <CMaths.hh>
#include "logger.hh"
#include "common.hh"

int (__thiscall *scrThread__ParseOriginal) (scrThread *  scr,
                                            unsigned int param_2);

struct MissionInfo
{
    int     strand;
    Vector3 endPos;
    int     citiesUnlocked;
    int     citiesUnlockedStart;
    bool    phoneMission;
};

std::unordered_map<std::string, MissionInfo> mMissionInfos{};

struct MissionStrandInfo
{
    int __unk00[6];
    int m_nLastMissionPassed;
    int __unk28[77];
};

static_assert (sizeof (MissionStrandInfo) == 4 * 84,
               "MissionStrandInfo is wrong size");

struct PreviousChange
{
    uint32_t hash = -1;
    uint32_t offset;
    uint8_t originalValue;
    uint8_t newValue;
};

class MissionRandomizer
{
    static std::unordered_map<std::string, std::string> mMissionMaps;
    static MissionInfo                                  mRandomizedMission;
    static MissionInfo                                  mOriginalMission;
    static int                                          mStoredIslandsUnlocked;
    static uint32_t                                     mMissionHash;
    static bool                                         mbFading;
    static int                                          miFadeTimer;
    static PreviousChange                               mPreviousChange;
    static const char *                                 mForcedMission;

    /*******************************************************/
    static std::string
    GetNewMission (std::string old)
    {
        std::transform (old.begin (), old.end (), old.begin (),
                        [] (unsigned char c) { return std::tolower (c); });

        // return "PXDFcut";
        if (mMissionMaps.count (old))
            return mMissionMaps[old]; // mForcedMission; //mMissionMaps[old];
        return old;
    }

    // Macro so I don't have to repeat similar code multiple times
#define ADJUST_SCRIPT_NAME(name, native)                                         \
    static void name (NativeData *data)                                          \
    {                                                                            \
        std::string newThread      = GetNewMission (data->Params[0].cstr_param); \
        data->Params[0].cstr_param = newThread.data ();                          \
                                                                                 \
        CNativeManager::CallOriginalNative (native, data);                       \
    }

    ADJUST_SCRIPT_NAME (HasScriptLoaded, "HAS_SCRIPT_LOADED")
    ADJUST_SCRIPT_NAME (RequestScript, "REQUEST_SCRIPT")
    ADJUST_SCRIPT_NAME (MarkScriptAsNoLongerNeeded,
                        "MARK_SCRIPT_AS_NO_LONGER_NEEDED")

#undef ADJUST_SCRIPT_NAME

    /*******************************************************/
    // This function adjusts the strand the mission will increment after a
    // mission passes. It changes the ipush1 instruction in the mission pass
    // strand code function.
    /*******************************************************/
    static void
    AdjustMissionPassCode (scrProgram *program)
    {
        auto pattern = hook::make_range_pattern (
            (uintptr_t) program->m_pCodeBlock,
            (uintptr_t) program->m_pCodeBlock + program->m_nCodeSize,
            "2f 03 06 00 ? 3b 32 3b 31 36 31 37 31 38 31 43 ?");

        pattern.for_each_result ([] (hook::pattern_match match) {
            *match.get<char> (4) = mOriginalMission.strand + 96;
        });
    }

    /*******************************************************/
    // This function adjusts the strand for the mission fail process code. If
    // this is not corrected, mission replay will not be activated. Further,
    // this code also adjusts the mission passes for missions like Uncle Vlad,
    // which deactivate a strand when you pass it. If this fucntion detects such
    // a code, the code will put a "return 0x0" instruction in-place of the
    // ipush1 instruction to prevent any stack corruptions.
    /*******************************************************/
    static void
    AdjustMissionFailCode (scrProgram *program)
    {
        auto pattern
            = hook::make_range_pattern ((uintptr_t) program->m_pCodeBlock,
                                        (uintptr_t) program->m_pCodeBlock
                                            + program->m_nCodeSize,
                                        "2f 00 03 00 ? 38 32 38 31 22");

        if (mPreviousChange.hash == program->m_dwHash
            && program->m_pCodeBlock[mPreviousChange.offset]
                   == mPreviousChange.newValue)
            return;

        pattern.for_each_result ([&program] (hook::pattern_match match) {
            uint8_t &strand = *match.get<uint8_t> (4);

            // ipush1 <strand>
            if (strand - 96 == mRandomizedMission.strand)
                {
                    strand = mOriginalMission.strand + 96;

                    if(strand - 96 != mRandomizedMission.strand)
                        {
                            mPreviousChange.hash = program->m_dwHash;
                            mPreviousChange.offset = &strand - program->m_pCodeBlock;
                            mPreviousChange.newValue = strand;
                        }
                    
                    return;
                }

            // ret 0x0
            *match.get<uint8_t> (4)  = 0x30;
            *match.get<uint16_t> (5) = 0;
        });
    }

    /*******************************************************/
    static void
    AdjustScriptCodeForNewStrand (const std::string &newThread,
                                  const std::string &originalThread)
    {
        scrProgram *program = CScrVM::GetScrProgram (newThread);

        // Set the strand and mission hash for future reference
        mOriginalMission   = mMissionInfos[originalThread];
        mRandomizedMission = mMissionInfos[newThread];
        mMissionHash       = program->m_dwHash;

        HandleMissionStart ();

        AdjustMissionPassCode (program);
        AdjustMissionFailCode (program);
    }

    /*******************************************************/
    static void
    StartNewScript (NativeData *data)
    {
        bool adjustScript = false;

        std::string originalThread = data->Params[0].cstr_param;
        std::transform (originalThread.begin (), originalThread.end (),
                        originalThread.begin (),
                        [] (unsigned char c) { return std::tolower (c); });

        std::string newThread = GetNewMission (originalThread);

        if (mMissionMaps.count (originalThread))
            {
                data->Params[0].cstr_param = newThread.data ();
                adjustScript               = true;
                data->Params[1].int_param
                    = std::max (8192, data->Params[1].int_param);
            }

        CNativeManager::CallOriginalNative ("START_NEW_SCRIPT", data);

        // This is after calling the native because the native does some things
        // with the active scripts
        if (adjustScript)
            AdjustScriptCodeForNewStrand (newThread, originalThread);
    }

    /*******************************************************/
    static int
    GetPlayerId ()
    {
        int playerId = 0;

        CNativeManager::CallNativeRet (&playerId, "GET_PLAYER_ID");

        return playerId;
    }

    /*******************************************************/
    static int
    GetPlayerChar ()
    {
        int player = -1;
        CNativeManager::CallNative ("GET_PLAYER_CHAR", GetPlayerId (), &player);

        return player;
    }

    /*******************************************************/
    static void
    HandleMissionStart ()
    {

        CNativeManager::CallNativeRet (&mStoredIslandsUnlocked, "GET_INT_STAT",
                                       STAT_CITIES_UNLOCKED);

        CNativeManager::CallNative ("SET_INT_STAT", STAT_CITIES_UNLOCKED,
                                    mRandomizedMission.citiesUnlocked);

        if (mOriginalMission.phoneMission && !mRandomizedMission.phoneMission)
            CNativeManager::CallNative ("DO_SCREEN_FADE_OUT", 1000);

        else if (mRandomizedMission.phoneMission
                 && !mOriginalMission.phoneMission)
            {
                CNativeManager::CallNative ("DO_SCREEN_FADE_IN", 1000);
                CNativeManager::CallNative ("SET_PLAYER_CONTROL",
                                            GetPlayerId (), 1);
            }

        CScrVM::m_pGlobals[101] = std::max (CScrVM::m_pGlobals[101], 1);
    }

    /*******************************************************/
    static void
    HandleMissionPass ()
    {
        CNativeManager::CallNative ("DO_SCREEN_FADE_OUT", 1000);
        mbFading    = true;
        miFadeTimer = time (NULL);
    }

    /*******************************************************/
    static void
    HandleMissionEnd ()
    {
        int newCitiesUnlocked = mStoredIslandsUnlocked;

        if (mOriginalMission.citiesUnlockedStart != -1)
            newCitiesUnlocked = mOriginalMission.citiesUnlockedStart;

        CNativeManager::CallNative ("SET_INT_STAT", STAT_CITIES_UNLOCKED,
                                    newCitiesUnlocked);
    }

    /*******************************************************/
    // Waits for the game to fade out completely after a mission pass. This is
    // intended to stop the script execution whilst the game fades to prevent
    // any wanted level issues and make the game think that you ended the
    // mission at the end position
    /*******************************************************/
    static bool
    HandleFadingCode (scrThread *scr)
    {
        if (!mbFading)
            return false;

        int faded = false;
        CNativeManager::CallNativeRet (&faded, "IS_SCREEN_FADED_OUT");

        if (!faded && time (NULL) - miFadeTimer < 11)
            return true;

        CNativeManager::CallNative ("ALLOW_GAME_TO_PAUSE_FOR_STREAMING", true);

        CNativeManager::CallNative ("LOAD_SCENE", mOriginalMission.endPos.x,
                                    mOriginalMission.endPos.y,
                                    mOriginalMission.endPos.z);

        CNativeManager::CallNative ("SET_CHAR_COORDINATES_NO_OFFSET",
                                    GetPlayerChar (), mOriginalMission.endPos.x,
                                    mOriginalMission.endPos.y,
                                    mOriginalMission.endPos.z);

        CNativeManager::CallNative ("DO_SCREEN_FADE_IN", 1000);

        mbFading = false;
        return false;
    }

    /*******************************************************/
    static void
    HandleMissionState (scrThread *scr)
    {
        if (mMissionHash == -1 || scr->m_context.dwScriptHash != mMissionHash)
            return;

        auto strands = reinterpret_cast<MissionStrandInfo *> (
            &CScrVM::m_pGlobals[10982]);

        // Thread ended
        if (scr->m_context.eThreadState == 2
            || strands[mOriginalMission.strand].m_nLastMissionPassed != -1)
            {
                // And the mission was passed
                if (strands[mOriginalMission.strand].m_nLastMissionPassed != -1)
                    HandleMissionPass ();

                HandleMissionEnd ();
                mMissionHash = -1;
            }
    }

    /*******************************************************/
    static int __fastcall ParseThreadHook (scrThread *scr, void *edx,
                                           unsigned int param_2)
    {
        if (HandleFadingCode (scr))
            return 0;

        int ret = scrThread__ParseOriginal (scr, param_2);
        return (HandleMissionState (scr), ret);
    }

    /*******************************************************/
    void
    InitialiseVFTableHooks ()
    {
        scrThread__vftable *gtaThread_vftable
            = *hook::get_pattern<scrThread__vftable *> (
                "c7 86 a8 00 00 00 00 00 00 00 8b c6 5e c3", -9);

        scrThread__vftable *scrThread_vftable
            = *hook::get_pattern<scrThread__vftable *> (
                "68 04 20 00 00 c7 06 ? ? ? ? 89 6e 58", 7);

        scrThread__ParseOriginal = gtaThread_vftable->ParseThread;

        ConvertCall (ParseThreadHook, gtaThread_vftable->ParseThread);
        ConvertCall (ParseThreadHook, scrThread_vftable->ParseThread);
    }

    /*******************************************************/
    static void
    InitialiseMissionsMap (unsigned int seed)
    {
        // Inefficient but whatever
        std::vector<std::string> missions;
        std::vector<std::string> order;
        std::mt19937             engine{seed};

        for (const auto &s : mMissionInfos)
            missions.push_back (s.first);

        order = missions;
        std::shuffle (order.begin (), order.end (), engine);

        for (int i = 0; i < missions.size (); i++)
            mMissionMaps[missions[i]] = order[i];
    }

    /*******************************************************/
    static bool
    ReadMissionsFile (const std::string &name)
    {
        FILE *file = Rainbomizer::Common::GetRainbomizerDataFile (name);
        mMissionInfos.clear();
        
        if (!file)
            return false;

        char line[2048] = {0};
        while (fgets (line, 2048, file))
            {
                if (line[0] == '#' || strlen (line) < 10)
                    continue;

                char    thread[32] = {0};
                Vector3 pos;
                int     strand;
                int     citiesUnlocked;
                char    phoneMission        = 'N';
                int     citiesUnlockedStart = -1;

                sscanf (line, "%s %*s %d %f %f %f %d %d %c", thread, &strand,
                        &pos.x, &pos.y, &pos.z, &citiesUnlocked,
                        &citiesUnlockedStart, &phoneMission);

                mMissionInfos[thread]
                    = {strand, pos, citiesUnlocked, citiesUnlockedStart,
                       phoneMission == 'Y'};
            }

        return true;
    }

public:
    /*******************************************************/
    MissionRandomizer ()
    {
        InitialiseAllComponents ();

        Rainbomizer::Common::AddEpisodeChangeCallback ([] (int) {
            ReadMissionsFile ("Missions.txt");
            InitialiseMissionsMap (time (NULL));
        });

        InitialiseVFTableHooks ();

        CNativeManager::OverwriteNative ("HAS_SCRIPT_LOADED", HasScriptLoaded);
        CNativeManager::OverwriteNative ("REQUEST_SCRIPT", RequestScript);
        CNativeManager::OverwriteNative ("MARK_SCRIPT_AS_NO_LONGER_NEEDED",
                                         MarkScriptAsNoLongerNeeded);
        CNativeManager::OverwriteNative ("START_NEW_SCRIPT", StartNewScript);
    }
};

std::unordered_map<std::string, std::string> MissionRandomizer::mMissionMaps{};
MissionInfo    MissionRandomizer::mRandomizedMission;
MissionInfo    MissionRandomizer::mOriginalMission;
uint32_t       MissionRandomizer::mMissionHash           = -1;
int            MissionRandomizer::mStoredIslandsUnlocked = -1;
bool           MissionRandomizer::mbFading               = false;
int            MissionRandomizer::miFadeTimer            = 0;
const char *   MissionRandomizer::mForcedMission         = "roman14";
PreviousChange MissionRandomizer::mPreviousChange;

MissionRandomizer _missions;
