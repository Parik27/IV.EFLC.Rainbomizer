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

int (__thiscall *scrThread__ParseOriginal) (scrThread* scr, unsigned int param_2);

struct MissionInfo
{
    int strand;
    Vector3 endPos;
    int citiesUnlocked;
    int citiesUnlockedStart;
    bool phoneMission;
};

std::unordered_map<std::string, MissionInfo> mMissionInfos{};

#ifdef HARDCODED_STRANDS
std::unordered_map<std::string, int> mMissionStrands = {
    {"bell2", 19},     {"bell5", 19},     {"bell6", 19},     {"bell7", 19},
    {"bernie1", 18},   {"bernie2", 18},   {"bernie3", 18},   {"brucie1", 8},
    {"brucie2", 8},    {"brucie3a", 8},   {"brucie3b", 8},   {"brucie4", 8},
    {"cia1", 12},      {"cia2", 12},      {"cia3", 12},      {"cia4", 12},
    {"derrick1", 16},  {"derrick2", 16},  {"derrick3", 16},  {"dwayne1", 7},
    {"dwayne3", 7},    {"elizabeta1", 6}, {"elizabeta2", 6}, {"elizabeta3", 6},
    {"elizabeta4", 6}, {"faustin1", 4},   {"faustin2", 4},   {"faustin3", 4},
    {"faustin4", 4},   {"faustin5", 4},   {"faustin6", 4},   {"faustin7", 4},
    {"finale1a", 0},   {"finale1b", 0},   {"finale1c", 0},   {"finale1d", 0},
    {"finale2", 0},    {"francis1", 11},  {"francis2a", 11}, {"francis2b", 11},
    {"francis3", 11},  {"francis4", 11},  {"francis5", 11},  {"francis6", 11},
    {"gambetti1", 21}, {"gambetti2", 21}, {"gambetti3", 21}, {"gerry1", 15},
    {"gerry2", 15},    {"gerry3a", 15},   {"gerry3b", 15},   {"gerry3c", 15},
    {"gerry4", 15},    {"gerry5", 15},    {"jacob1", 3},     {"jacob2", 3},
    {"jimmy1", 20},    {"jimmy2", 20},    {"jimmy3", 20},    {"jimmy4", 20},
    {"jimmy4a", 20},   {"manny1", 5},     {"manny2", 5},     {"manny3", 5},
    {"michelle1", 2},  {"packie1", 13},   {"packie2", 13},   {"packie3", 13},
    {"playboy2", 9},   {"playboy3", 9},   {"playboy4", 9},   {"pxdfcut", 9},
    {"ray1", 14},      {"ray2", 14},      {"ray3", 14},      {"ray4", 14},
    {"ray5", 14},      {"ray6", 14},      {"roman1", 0},     {"roman2", 0},
    {"roman3", 0},     {"roman4", 0},     {"roman5", 0},     {"roman6", 0},
    {"roman7", 0},     {"roman9", 0},     {"roman11", 0},    {"roman12", 0},
    {"roman13", 0},    {"roman14", 0},    {"vlad1", 1},      {"vlad2", 1},
    {"vlad3", 1},      {"vlad4", 1}
};
#endif

struct MissionStrandInfo
{
    int __unk00[6];
    int m_nLastMissionPassed;
    int __unk28[77];
};

static_assert (sizeof (MissionStrandInfo) == 4 * 84,
               "MissionStrandInfo is wrong size");

class MissionRandomizer
{
    static std::unordered_map<std::string, std::string> mMissionMaps;
    static MissionInfo                                  mRandomizedMission;
    static MissionInfo                                  mOriginalMission;
    static int                                          mStoredIslandsUnlocked;
    static uint32_t                                     mMissionHash;
    static bool                                         mbFading;
    static int                                          miFadeTimer;

    /*******************************************************/
    static std::string
    GetNewMission (std::string old)
    {
        std::transform (old.begin (), old.end (), old.begin (),
                        [] (unsigned char c) { return std::tolower (c); });

        // return "PXDFcut";
        if (mMissionMaps.count (old))
            return mMissionMaps[old];
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
    static void
    AdjustScriptCodeForNewStrand (const std::string &newThread,
                                  const std::string &originalThread)
    {
        scrProgram *program = CScrVM::GetScrProgram (newThread);
        auto        pattern = hook::make_range_pattern (
            (uintptr_t) program->m_pCodeBlock,
            (uintptr_t) program->m_pCodeBlock + program->m_nCodeSize,
            "2f 03 06 00 ? 3b 32 3b 31 36 31 37 31 38 31 43 ?");

        // Set the strand and mission hash for future reference
        mOriginalMission      = mMissionInfos[originalThread];
        mRandomizedMission    = mMissionInfos[newThread];
        mMissionHash = program->m_dwHash;

        CNativeManager::CallNativeRet (&mStoredIslandsUnlocked, "GET_INT_STAT",
                                       STAT_CITIES_UNLOCKED);

        printf ("Overwriting Mission Strand for %s (%d) to %s (%d): %d "
                "matches\n",
                newThread.c_str (), mRandomizedMission.strand,
                originalThread.c_str (), mOriginalMission.strand,
                pattern.size ());

        HandleMissionStart();
        
        pattern.for_each_result ([] (hook::pattern_match match) {
            *match.get<char> (4) = mOriginalMission.strand + 96;
        });
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
            }

        CNativeManager::CallOriginalNative ("START_NEW_SCRIPT", data);

        // This is after calling the native because the native does some things
        // with the active scripts
        if (adjustScript)
            AdjustScriptCodeForNewStrand (newThread, originalThread);
    }

    /*******************************************************/
    static void HandleMissionStart()
    {
        CNativeManager::CallNative("SET_INT_STAT", STAT_CITIES_UNLOCKED,
                                   mRandomizedMission.citiesUnlocked);

        if(mOriginalMission.phoneMission)
            CNativeManager::CallNative("DO_SCREEN_FADE_OUT", 1000);
        
        else if(mRandomizedMission.phoneMission)
            CNativeManager::CallNative("DO_SCREEN_FADE_IN", 1000);
    }

    /*******************************************************/
    static int GetPlayerChar()
    {
        int player = -1;
        int playerId = 0;
        
        CNativeManager::CallNativeRet(&playerId, "GET_PLAYER_ID");
        CNativeManager::CallNative ("GET_PLAYER_CHAR", playerId, &player);

        return player;
    }

    /*******************************************************/
    static void
    HandleMissionPass ()
    {
        CNativeManager::CallNative("DO_SCREEN_FADE_OUT", 1000);
        mbFading = true;
        miFadeTimer = time(NULL);
    }

    /*******************************************************/
    static void HandleMissionEnd()
    {
        int newCitiesUnlocked = mStoredIslandsUnlocked;
        if(mOriginalMission.citiesUnlockedStart != -1)
            newCitiesUnlocked = mOriginalMission.citiesUnlockedStart;

        CNativeManager::CallNative ("SET_INT_STAT", STAT_CITIES_UNLOCKED,
                                    newCitiesUnlocked);
    }
    
    /*******************************************************/
    static int __fastcall ParseThreadHook (scrThread *scr, void *edx,
                                           unsigned int param_2)
    {
        if(mbFading)
            {
                bool faded = false;
                if ((CNativeManager::CallNativeRet (&faded,
                                                    "IS_SCREEN_FADED_OUT"),
                     faded)
                    || time(NULL) - miFadeTimer > 10)
                    {
                        CNativeManager::CallNative (
                            "ALLOW_GAME_TO_PAUSE_FOR_STREAMING", true);
                        CNativeManager::CallNative (
                            "SET_CHAR_COORDINATES_NO_OFFSET", GetPlayerChar (),
                            mOriginalMission.endPos.x,
                            mOriginalMission.endPos.y,
                            mOriginalMission.endPos.z);

                        CNativeManager::CallNative ("LOAD_SCENE",
                                                    mOriginalMission.endPos.x,
                                                    mOriginalMission.endPos.y,
                                                    mOriginalMission.endPos.z);

                        CNativeManager::CallNative ("DO_SCREEN_FADE_IN", 1000);
                        mbFading = false;
                    }
                else
                    return 0;
            }

        int ret = scrThread__ParseOriginal (scr, param_2);
        
        if (mMissionHash != -1 && scr->m_context.dwScriptHash == mMissionHash)
            {
                auto strands = reinterpret_cast<MissionStrandInfo *> (
                            &CScrVM::m_pGlobals[10982]);
                
                // Thread ended
                if (scr->m_context.eThreadState == 2 ||
                    strands[mOriginalMission.strand].m_nLastMissionPassed != -1)
                    {
                        // And the mission was passed
                        if(strands[mOriginalMission.strand].m_nLastMissionPassed != -1)
                            HandleMissionPass();

                        HandleMissionEnd();
                        mMissionHash = -1;
                    }
            }

        return ret;
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

        ConvertCall(ParseThreadHook, gtaThread_vftable->ParseThread);
        ConvertCall(ParseThreadHook, scrThread_vftable->ParseThread);
    }

    /*******************************************************/
    void InitialiseMissionsMap(unsigned int seed)
    {
        // Inefficient but whatever
        std::vector<std::string> missions;
        std::vector<std::string> order;
        std::mt19937 engine{seed};
        
        for (const auto &s : mMissionInfos)
            missions.push_back(s.first);

        printf ("Total defined missions: %d\n", missions.size ());

        order = missions;
        std::shuffle (order.begin (), order.end (), engine);

        for (int i = 0; i < missions.size (); i++)
            mMissionMaps[missions[i]] = order[i];
    }

    /*******************************************************/
    bool
    ReadMissionsFile (const std::string &name)
    {
        FILE *file = fopen (name.c_str (), "r");
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
                char    phoneMission;
                int     citiesUnlockedStart;

                sscanf (line, "%s %*s %d %f %f %f %d %d %c", thread, &strand,
                        &pos.x, &pos.y, &pos.z, &citiesUnlocked,
                        &citiesUnlockedStart, &phoneMission);

                printf ("%s: %d %f %f %f %d\n", thread, strand, pos.x, pos.y,
                        pos.z, citiesUnlocked);

                mMissionInfos[thread] = {strand, pos, citiesUnlocked,
                                         citiesUnlockedStart, phoneMission == 'Y'};
            }

        return true;
    }

public:
    
    /*******************************************************/
    MissionRandomizer ()
    {
        InitialiseAllComponents();
        if(!ReadMissionsFile("missions.txt"))
            return;
        InitialiseMissionsMap(time(NULL));
        InitialiseVFTableHooks();
        
        CNativeManager::OverwriteNative("HAS_SCRIPT_LOADED", HasScriptLoaded);
        CNativeManager::OverwriteNative("REQUEST_SCRIPT", RequestScript);
        CNativeManager::OverwriteNative ("MARK_SCRIPT_AS_NO_LONGER_NEEDED",
                                         MarkScriptAsNoLongerNeeded);
        CNativeManager::OverwriteNative ("START_NEW_SCRIPT", StartNewScript);
    }
};

std::unordered_map<std::string, std::string> MissionRandomizer::mMissionMaps{};
MissionInfo MissionRandomizer::mRandomizedMission;
MissionInfo MissionRandomizer::mOriginalMission;
uint32_t MissionRandomizer::mMissionHash = -1;
int MissionRandomizer::mStoredIslandsUnlocked = -1;
bool MissionRandomizer::mbFading = false;
int MissionRandomizer::miFadeTimer = 0;
MissionRandomizer _missions;
