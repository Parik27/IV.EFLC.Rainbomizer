#include "CTheScripts.hh"
#include "Utils.hh"
#include "injector/hooking.hpp"
#include "audScriptAudioEntity.hh"
#include <unordered_map>
#include "CText.hh"
#include "common.hh"
#include "CCrypto.hh"
#include <array>
#include "config.hh"
#include "logger.hh"
#include "audEngine.hh"
#include "CModelInfoStore.hh"

// All the audio gxt tables
const std::array<std::vector<std::string>, 3> gxtTables
    = {{// Grand Theft Auto IV
        {"B1AUD",   "B2AUD",   "B3AUD",   "B5AUD",   "B6AUD",   "BC1AUD",
         "BC2AUD",  "BC3AUD",  "BR1AUD",  "BR2AUD",  "BR3AUD",  "BR4AUD",
         "BR4BAUD", "C1AUD",   "C2AUD",   "C3AUD",   "C4AUD",   "DF1AUD",
         "DF3AUD",  "DFBKAUD", "DM1AUD",  "DM2AUD",  "DM3AUD",  "E1AUD",
         "E2AUD",   "E3AUD",   "E4AUD",   "F1AUD",   "F2AUD",   "F3AUD",
         "F4AUD",   "F5AUD",   "F6AUD",   "F7AUD",   "FCB4AUD", "FCDAUD",
         "FCJAAUD", "FCJSAUD", "FCPAAUD", "FCPSAUD", "FFD2AUD", "FFD3AUD",
         "FFDAUD",  "FM1AUD",  "FM2AUD",  "FM3AUD",  "FM4AUD",  "FM5AUD",
         "FM6AUD",  "G1AUD",   "G2AUD",   "G3AUD",   "GCA1AUD", "GCA2AUD",
         "GCA3AUD", "GCC1AUD", "GCC2AUD", "GCC3AUD", "GCK1AUD", "GCK2AUD",
         "GCK3AUD", "GCM1AUD", "GCM2AUD", "GCM3AUD", "GM1AUD",  "GM2AUD",
         "GM3AUD",  "GM3CAUD", "GM4AUD",  "GM5AUD",  "HT1AUD",  "J1AUD",
         "J1BAUD",  "J2AUD",   "J3AUD",   "J4AUD",   "LJ1AUD",  "LJ2AUD",
         "LJP3AUD", "M1AUD",   "M2AUD",   "M3AUD",   "MF10AUD", "MF11AUD",
         "MF12AUD", "MF13AUD", "MF14AUD", "MF1AUD",  "MF2AUD",  "MF3AUD",
         "MF4AUD",  "MF5AUD",  "MF6AUD",  "MF7AUD",  "MF8AUD",  "MF9AUD",
         "MI1AUD",  "MPKPAUD", "PIBAUD",  "PICAUD",  "PICLAUD", "PIEAUD",
         "PIGAUD",  "PIHAUD",  "PIIAUD",  "PIJAUD",  "PIMAAUD", "PIMAUD",
         "PIP2AUD", "PIPAUD",  "PIRAUD",  "PIS2AUD", "PISAUD",  "PIVAUD",
         "PM1AUD",  "PM2AUD",  "PM3AUD",  "PM3BAUD", "PMLAUD",  "PX2AUD",
         "PX3AUD",  "PXDFAUD", "R11AUD",  "R12AUD",  "R13AUD",  "R13BAUD",
         "R14AUD",  "R1AUD",   "R2AUD",   "R3AUD",   "R4AUD",   "R5AUD",
         "R6AUD",   "R7AUD",   "R9AUD",   "RB1AUD",  "RB2AUD",  "RB3AUD",
         "RB4AUD",  "RB5AUD",  "RB6AUD",  "RP10AUD", "Rp8AUD",  "V1AUD",
         "V2AUD",   "V3AUD",   "V4AUD",   "TWAT",    "BRP5AU",  "FCB2AU",
         "FCB2aAU", "FCB3AU",  "FCBCPAU", "FCBDAU",  "FCBPCAU", "FCD2AU",
         "FCD3AU",  "FCDDAU",  "FCJC1AU", "FCJC2AU", "FCJCPAU", "FCJDCAU",
         "FCJPCAU", "FCPC1AU", "FCPC2AU", "FCPCPAU", "FCPDCAU", "FCPPCAU",
         "FCR11AU", "FCR12AU", "FCR21AU", "FCR22AU", "FCRC1AU", "FCRC2AU",
         "FCRDCAU", "FIN1AAU", "FIN1BAU", "FIN1CAU", "FIN1DAU", "FIN1EAU",
         "GCAC1AU", "GCAC2AU", "GCADCAU", "GCCC1AU", "GCCC2AU", "GCCDCAU",
         "GCKC1AU", "GCKC2AU", "GCKDCAU", "GCKK1AU", "GCKK2AU", "GCKK3AU",
         "GCMC1AU", "GCMDCAU", "MPKP2AU", "MPKPbAU", "SHOW1AU", "SHOW2AU",
         "SHOW3AU", "SHOW4AU", "SHOW5AU", "SHOW6AU", "SHOW7AU", "SHOW8AU",
         "FIN1CBA", "FIN1DBA", "PIMA2AD"},

        // The Lost and Damned
        {"DAVE1AU", "E12WA",   "E13WA",   "E1A1AUD", "E1A2AUD", "E1B1AUD",
         "E1B2AUD", "E1B3AUD", "E1B4AUD", "E1B5AUD", "E1B6AUD", "E1BBAU",
         "E1BEA",   "E1DRA",   "E1E1AUD", "E1E2AUD", "E1E3AUD", "E1FCC2A",
         "E1FCC3A", "E1FCC4A", "E1FCCA",  "E1FCJ2A", "E1FCJ3A", "E1FCJ4A",
         "E1FCJAU", "E1FCT2A", "E1FCT3A", "E1FCT4A", "E1FCTA",  "E1J1AUD",
         "E1J2AUD", "E1J3AUD", "E1J4AUD", "E1J5AUD", "E1J6AUD", "E1MF2AU",
         "E1MF3AU", "E1MFAU",  "E1R1AUD", "E1R2AUD", "E1R3AUD", "E1S1AUD",
         "E1S2AUD", "E1S3AUD", "E1S3p1A", "E1S3p2A", "E1S3p3A", "E1S3p4A",
         "E1S3p5A", "E1S4AUD", "GWAUD",   "MALC1AU", "MALC2AU", "RACESAU",
         "RPi1AUD", "RCLnch"},

        // The Ballad of Gay Tony
        {
            "E2BCAUD", "E2BCBAU", "E2BG1AU", "E2BG1BA", "E2BG1CA", "E2BG2AU",
            "E2BG2BA", "E2BG2CA", "E2BG3AU", "E2BR1AU", "E2BR1BA", "E2BR2AU",
            "E2BR3AU", "E2BR3BA", "E2CLAUD", "E2CLJA",  "E2DWAUD", "E2DWBA",
            "E2DWCA",  "E2F1AUD", "E2F1BAU", "E2F2AUD", "E2F2BAU", "E2FAAU",
            "E2FAHA",  "E2FC2AU", "E2FCAUD", "E2FHAU",  "E2MF1AU", "E2MF2AU",
            "E2MUM1A", "E2T10AU", "E2T11AU", "E2T1AUD", "E2T2AUD", "E2T2BAU",
            "E2T3AUD", "E2T4AUD", "E2T4BAU", "E2T5AUD", "E2T5BAU", "E2T6AUD",
            "E2T7AUD", "E2T7BA",  "E2T8AUD", "E2T9AUD", "E2T9BA",  "E2Tm1AU",
            "E2Tm2AU", "E2Tm3AU", "E2Tm4AU", "E2Tm5AU", "E2Tm6AU", "E2Tm7AU",
            "E2Tm8AU", "E2Y1Aud", "E2Y2AUD", "E2Y3AUD", "E2Y4AUD", "FAHBA",
            "GIRL1AU", "GIRL2AU", "ROC1AUD", "ROM1AUD", "ROCCO1",
        }}};

bool (__thiscall *FUN_00787340__8f6ae9) (audScriptAudioEntity *, char *, char *,
                                         char *);
bool (__stdcall *audScriptAudioEntity__IsSFXSound8f6959) (char *);
void (__thiscall *FUN_0091cac0__8f8a86) (audScriptAudioEntity *, char *, char *,
                                         char *, float, int, char);

audSoundMetadata *(__thiscall *MetadataGetAtOffset_77752f) (
    audConfigMetadata<audSoundMetadata> *, uint32_t);

audSoundMetadata *(__thiscall *MetadataGetByHash_77753d) (
    audConfigMetadata<audSoundMetadata> *, uint32_t);

struct SoundPair
{
    uint32_t    bankHash = -1;
    std::string sound    = "";
    std::string subtitle = "";
};

class SoundsRandomizer
{
    static injector::scoped_jmp                             mHook8f5b00;
    static std::unordered_map<uint32_t, std::string>        mTexts;
    static std::array<std::pair<uint32_t, std::string>, 50> mBankAudioPairs;
    static std::vector<SoundPair>                           mSounds;
    static bool                                             mAdjustNextBankHash;
    static int                                              mCorrectedBankHash;

    /*******************************************************/
    static void __fastcall RandomizeConversationLine (
        audScriptAudioEntity *entity, void *edx, int index, int param_3,
        char *identifier, char *subtitle, int param_6, int param_7)
    {
        mHook8f5b00.restore ();

        if (mSounds.size () > 0)
            {
                auto &sound = mSounds[RandomInt (mSounds.size () - 1)];

                identifier             = sound.sound.data ();
                subtitle               = sound.subtitle.data ();
                mBankAudioPairs[index] = {sound.bankHash, sound.sound};
            }

        entity->AddLineToConversation (index, param_3, identifier, subtitle,
                                       param_6, param_7);

        InitialiseAddLineToConversationHook ();
    }

    /*******************************************************/
    static void
    InitialiseAddLineToConversationHook ()
    {
        static void *addr = hook::get_pattern (
            ByVersion ("8b 44 ? ? 6b c0 70 56 8d 34 08 8b 4c",
                       "8b 44 ? ? 56 8b 74 ? ? 6b f6 70 03 f1"));

        mHook8f5b00.make_jmp (addr, RandomizeConversationLine);
    }

    /*******************************************************/
    static void
    AppendTextData (CText_Data<CGxtEntry> &keys, CText_Data<char> &data)
    {
        for (int i = 0; i < keys.m_nSize; i++)
            {
                uint32_t hash   = keys.m_aData[i].m_nHash;
                uint32_t offset = keys.m_aData[i].m_nOffset;

                if (CText::m_nEncoding == 8)
                    mTexts[hash] = std::string (data.m_aData + offset);
                else
                    {
                        std::string string = "";
                        for (char *str = data.m_aData + offset; *str; str += 2)
                            string.push_back (*str);

                        mTexts[hash] = std::string (string);
                    }
            }
    }

    /*******************************************************/
    static void
    InitialiseTextDataForEpisode (int episode)
    {
        AppendTextData (CText::g_Text->m_aTextDataKeys (),
                        CText::g_Text->m_aTextDataStrings ());

        for (auto i : gxtTables[episode])
            {
                CText::g_Text->LoadAdditionalText (i.c_str (), 1);
                if (!CText::g_Text->m_abAdditionalTextLoaded ()[1])
                    {
                        Rainbomizer::Logger::LogMessage ("Failed to load: %s",
                                                         i.c_str ());
                        continue;
                    }

                AppendTextData (
                    CText::g_Text->m_aAdditionalTextDataKeys ()[1],
                    CText::g_Text->m_aAdditionalTextDataStrings ()[1]);
            }
    }

    /*******************************************************/
    static bool
    DoesTextLabelExist (const std::string &label)
    {
        return mTexts.count (CCrypto::atStringHash (label.c_str ()));
    }

    /*******************************************************/
    static std::string
    GetTextLabel (const std::string &label)
    {
        uint32_t hash = CCrypto::atStringHash (label.c_str ());
        if (mTexts.count (hash))
            return mTexts[hash];
        return "";
    }

    /*******************************************************/
    static void
    ProcessAudioBank (std::unordered_map<uint32_t, SoundPair> &pairs,
                      const std::string &                      bankSL)
    {
        if (!DoesTextLabelExist (bankSL))
            return;

        std::string bank
            = bankSL.substr (0, bankSL.length () - 2); // bankSL -> bank

        SoundPair pair;
        if ((pair.sound = GetTextLabel (bank + "A"), pair.sound != ""))
            {
                pair.subtitle                                      = bank;
                pairs[CCrypto::atStringHash (pair.sound.c_str ())] = pair;
            }
        else
            {
                for (int i = 1; i < 50; i++)
                    {
                        SoundPair pair;
                        pair.subtitle = bank + "_" + std::to_string (i);
                        if ((pair.sound = GetTextLabel (pair.subtitle + "A"),
                             pair.sound == ""))
                            break;

                        pairs[CCrypto::atStringHash (pair.sound.c_str ())]
                            = pair;
                    }
            }
    }

    /*******************************************************/
    static void
    FindBankSlotsForSoundPairs (
        std::unordered_map<uint32_t, SoundPair> &soundPairs)
    {
        int total = 0;
        for (int i = 0; i < audScriptAudioEntity::g_nSoundMetadataLen (); i++)
            {
                auto metadata = audScriptAudioEntity::g_aSoundMetadata ()[i];
                for (int j = 0; j < metadata.size; j++)
                    {
                        uint32_t hash = metadata.sounds[j].hash;
                        if (soundPairs.count (hash))
                            {
                                soundPairs[hash].bankHash = metadata.hash;
                                total++;
                            }
                    }
            }

        Rainbomizer::Logger::LogMessage ("Total Bank Slots resolved: %d\n",
                                         total);
    }

    /*******************************************************/
    static void
    InitialiseAudioDataForEpisode (int episode)
    {
        std::unordered_map<uint32_t, SoundPair> soundPairs;
        mSounds.clear ();
        char  line[128] = {0};
        FILE *audioFile
            = Rainbomizer::Common::GetRainbomizerDataFile ("AudioBanks.txt");

        if (!audioFile)
            return;

        while (fgets (line, 128, audioFile))
            {
                if (strlen (line) > 2 && line[0] != '#')
                    {
                        line[strcspn (line, "\n")] = 0;
                        ProcessAudioBank (soundPairs, line);
                    }
            }

        Rainbomizer::Logger::LogMessage ("Number of voice lines: %d",
                                         soundPairs.size ());
        FindBankSlotsForSoundPairs (soundPairs);

        std::for_each (soundPairs.begin (), soundPairs.end (),
                       [] (auto value) { mSounds.push_back (value.second); });
    }

    /*******************************************************/
    static void
    InitialiseRandomizationForEpisode (int episode)
    {
        InitialiseTextDataForEpisode (episode);
        InitialiseAudioDataForEpisode (episode);
    }

    /*******************************************************/
    static uint32_t
    CorrectBankHash (const char *str)
    {
        if (mCorrectedBankHash != -1)
            return mCorrectedBankHash;

        return CCrypto::atStringHash (str);
    }

    /*******************************************************/
    static uint32_t
    CorrectBankHashChecked (const char *str)
    {
        if (mCorrectedBankHash != -1 && mAdjustNextBankHash)
            {
                mAdjustNextBankHash = false;
                return mCorrectedBankHash;
            }

        return CCrypto::atStringHash (str);
    }

    /*******************************************************/
    static bool __fastcall SetPlaceholderBankName (audScriptAudioEntity *ent,
                                                   void *edx, char *bank,
                                                   char *sound, char *param_4)
    {
        mAdjustNextBankHash = true;
        return FUN_00787340__8f6ae9 (ent, bank, sound, param_4);
    }

    /*******************************************************/
    static void
    SetNextBankAudioHash (std::string str)
    {
        for (auto i : mBankAudioPairs)
            {
                if (i.second == str)
                    {
                        mCorrectedBankHash = i.first;
                        break;
                    }
            }
    }

    /*******************************************************/
    static bool __stdcall SetNextBankAudioHash1 (char *soundName)
    {
        mCorrectedBankHash = -1;
        SetNextBankAudioHash (soundName);
        return audScriptAudioEntity__IsSFXSound8f6959 (soundName);
    }

    /*******************************************************/
    static char *__stdcall CorrectSubtitles (char *txt)
    {
        std::string str;
        try
            {
                str = mTexts.at (CCrypto::atStringHash (txt));
            }
        catch (const std::out_of_range &e)
            {
                str = "";
            }

        static std::unique_ptr<char[]> ptr;
        if (CText::m_nEncoding == 8)
            {
                ptr = std::make_unique<char[]> (str.length () + 1);
                for (int i = 0; i < str.length (); i++)
                    ptr.get ()[i] = str[i];
                ptr.get ()[str.length ()] = 0;

                return ptr.get ();
            }

        // Conversion to widestring
        ptr = std::make_unique<char[]> (str.length () * 2 + 2);

        for (int i = 0; i <= str.length (); i++)
            {
                if (i != str.length ())
                    ptr.get ()[i * 2] = str[i];
                else
                    ptr.get ()[i * 2] = 0;

                ptr.get ()[(i * 2) + 1] = 0;
            }

        return ptr.get ();
    }

    /*******************************************************/
    static void __fastcall SetNextBankAudioHash2 (audScriptAudioEntity *ent,
                                                  void *edx, char *bank,
                                                  char *sound, char *variation,
                                                  float param_5, int param_6,
                                                  char param_7)
    {
        SetNextBankAudioHash (sound);
        FUN_0091cac0__8f8a86 (ent, bank, sound, variation, param_5, param_6,
                              param_7);
    }

    /*******************************************************/
    static bool __fastcall CorrectSubtitleVariationLabelCheck (CText *text,
                                                               void * edx,
                                                               char * label)
    {
        return DoesTextLabelExist (label);
    }

    void
    InitialiseVoiceLineRandomizerPatches ()
    {
        if (!ConfigManager::GetConfigs ().sounds.RandomizeVoiceLines)
            return;

        InitialiseAddLineToConversationHook ();

        RegisterHook (ByVersion ("8d bd f0 15 00 00 57 8b ce e8",
                                 "8d 83 f0 15 00 00 50 8b ce e8"),
                      9, audScriptAudioEntity__IsSFXSound8f6959,
                      SetNextBankAudioHash1);

        RegisterHook (
            ByVersion ("b9 ? ? ? ? e8 ? ? ? ? 80 bd d2 2b 00 00 00",
                       "b9 ? ? ? ? 50 e8 ? ? ? ? 80 bb d2 2b 00 00 00 "),
            ByVersion (5, 6), CorrectSubtitles);

        RegisterHook (ByVersion ("50 89 44 ? ? e8 ? ? ? ? 83 c4 08 50",
                                 "50 6a 00 51 89 4c ? ? e8 ? ? ? ? 83 c4 08 "),
                      ByVersion (5, 8), CorrectBankHash);

        RegisterHook (ByVersion ("8b 4c ? ? 51 52 6a 00 50 e8",
                                 "8b f1 ff 74 ? ? 6a 00 50 e8"),
                      9, CorrectBankHash);

        RegisterHook (ByVersion ("57 52 c6 86 09 02 00 00 00 e8",
                                 "ff 74 ? 48 c6 87 09 02 00 00 00 e8 "),
                      ByVersion (9, 11), CorrectBankHash);

        RegisterHook (ByVersion ("57 50 6a 00 52 e8 ? ? ? ? 83 c4 08",
                                 "56 50 6a 00 ff 74 ? 54 e8 "),
                      ByVersion (5, 8), CorrectBankHash);

        RegisterHook (
            ByVersion ("50 8d ? ? ? ? ? 89 7c ? ? e8 ? ? ? ? eb ?",
                       "05 38 2e 00 00 03 c3 50 e8 ? ? ? ? 8b 54 ? ? eb ?"),
            ByVersion (11, 8), FUN_0091cac0__8f8a86, SetNextBankAudioHash2);

        RegisterHook (
            ByVersion ("f3 a4 50 b9 ? ? ? ? e8 ? ? ? ? 84 c0",
                       "f3 a4 50 b9 ? ? ? ? e8 ? ? ? ? b9 ? ? ? ? 84 c0 "),
            8, CorrectSubtitleVariationLabelCheck);
    }

    /*******************************************************/
    /* Sfx Randomizer Patches                              */
    /*******************************************************/
    static __fastcall audBaseGameMetadata *
    RandomizeGameMetadataRequestByName (audEngine *engine, void *edx,
                                        const char *name)
    {
        return RandomizeGameMetadataRequestByHash (engine, edx,
                                                   CCrypto::atStringHash (
                                                       name));
    }

    /*******************************************************/
    static __fastcall audBaseGameMetadata *
    RandomizeGameMetadataRequestByHash (audEngine *engine, void *edx,
                                        uint32_t hash)
    {
        auto metadata = audEngine::sm_Instance->GetGameMetadata ();

        // Types that will be randomized by this function
        std::array randomizedTypes{GM_CRIME,        GM_WEAPON,   GM_DOOR,
                                   GM_COLLISION,    GM_CLOTHING, GM_FOOTSTEPS,
                                   GM_MELEE_COMBAT, GM_PED,      GM_VEHICLE};

        auto object = metadata->GetByHash (hash);
        if (!object
            || std::find (std::begin (randomizedTypes),
                          std::end (randomizedTypes), object->Type)
                   == std::end (randomizedTypes))
            return object;

        // Special case required for them because the size of the vehicle
        // metadata structure differs for helicopters and boats.
        if (object->Type == GM_VEHICLE)
            {
                CVehicleModelInfo *originalInfo
                    = CModelInfoStore::GetModelInfoPointer<CVehicleModelInfo> (
                        CModelInfoStore::FindVehicleModelWithGameName (hash));

                // Get a new vehicle of the same type
                std::vector<CVehicleModelInfo *> validVehicles;
                for (auto i : Rainbomizer::Common::GetVehicleIndices ())
                    {
                        auto info = CModelInfoStore::GetModelInfoPointer<
                            CVehicleModelInfo> (i);

                        switch (originalInfo->m_nType)
                            {
                            case VEHICLE_TYPE_CAR:
                                case VEHICLE_TYPE_BIKE: {
                                    if (info->m_nType == VEHICLE_TYPE_CAR
                                        || info->m_nType == VEHICLE_TYPE_BIKE)
                                        validVehicles.push_back (info);

                                    break;
                                }

                            default:
                                if (info->m_nType == originalInfo->m_nType)
                                    validVehicles.push_back (info);
                            }
                    }

                return metadata->GetByHash (CCrypto::atStringHash (
                    validVehicles[RandomInt (validVehicles.size () - 1)]
                        ->m_szGameName));
            }

        std::vector<audBaseGameMetadata *> objects;

        metadata->for_each ([&] (audBaseGameMetadata *obj, uint32_t hash) {
            if (obj->Type == object->Type)
                objects.push_back (obj);
        });

        return objects[RandomInt (objects.size () - 1)];
    }

    /*******************************************************/
    /* Music Randomizer Patches                            */
    /*******************************************************/
    static bool
    ShouldMetadataBeRandomized (audSoundMetadata* metadata, uint32_t hash)
    {
        // Randomize streaming sounds only (that's all the music)
        if (metadata->Type != audStreamingSound)
            return false;

        // Don't randomize cutscenes
        if (metadata->GetCategoryHash() == "cutscenes"_joaat)
            return false;

        // To prevent softlocks whilst dancing
        switch (hash)
            {
            case "dancing_hercules_mix"_joaat:
            case "dancing_maisonette_mix"_joaat:
            case "dancing_bahamamammas_mix"_joaat:
                return false;
            }

        return true;
    }

    /*******************************************************/
    static audSoundMetadata *
    GetRandomizedGameMusic (audConfigMetadata<audSoundMetadata> *mgr,
                            audSoundMetadata *metadata, uint32_t orHash)
    {
        std::vector<audSoundMetadata *> validDatas;

        if (!metadata || !ShouldMetadataBeRandomized (metadata, orHash))
            return metadata;

        mgr->for_each ([&] (audSoundMetadata *obj, uint32_t hash) {
            if (obj->Type == metadata->Type)
                validDatas.push_back (obj);
        });

        return validDatas[RandomInt (validDatas.size () - 1)];
    }

    /*******************************************************/
    static __fastcall audSoundMetadata *
    GetRandomizedGameMusicByOffset (audConfigMetadata<audSoundMetadata> *mgr,
                                    void *edx, uint32_t offset)
    {
        auto metadata = MetadataGetAtOffset_77752f (mgr, offset);
        return GetRandomizedGameMusic (mgr, metadata, offset);
    }

    /*******************************************************/
    static __fastcall audSoundMetadata *
    GetRandomizedGameMusicByHash (audConfigMetadata<audSoundMetadata> *mgr,
                                  void *edx, uint32_t hash)
    {
        auto metadata = MetadataGetByHash_77753d (mgr, hash);
        return GetRandomizedGameMusic (mgr, metadata, hash);
    }

    /*******************************************************/
    void
    InitialiseMusicRandomizerPatches ()
    {
        if (!ConfigManager::GetConfigs ().sounds.RandomizeGameMusic)
            return;

        RegisterHook (ByVersion ("74 ? 8b 44 ? ? 50 e8 ? ? ? ? eb",
                                 "8b f1 74 ? e8 ? ? ? ? eb ? e8"),
                      ByVersion (7, 4), MetadataGetAtOffset_77752f,
                      GetRandomizedGameMusicByOffset);

        RegisterHook (ByVersion ("74 ? 8b 44 ? ? 50 e8 ? ? ? ? eb",
                                 "8b f1 74 ? e8 ? ? ? ? eb ? e8"),
                      ByVersion (21, 11), MetadataGetByHash_77753d,
                      GetRandomizedGameMusicByHash);
    }

    /*******************************************************/
    void
    InitialiseSfxRandomizerPatches ()
    {
        if (!ConfigManager::GetConfigs ().sounds.RandomizeGameSFX)
            return;

        void *   ItemByNameAddr;
        uint32_t GameMetaOffset = audEngine::GetGameMetadataOffset ();

        ReadCall (hook::get_pattern ("53 55 8b e9 68 ? ? ? ? b9 ? ? ? ? e8",
                                     14),
                  ItemByNameAddr);

        hook::pattern ("81 c1 ? ? ? ? e9")
            .for_each_result ([&] (hook::pattern_match m) {
                if (*m.get<uint32_t> (2) == GameMetaOffset)
                    {
                        if (m.get<void> () == ItemByNameAddr)
                            injector::MakeJMP (
                                m.get<void> (),
                                RandomizeGameMetadataRequestByName);
                        else
                            injector::MakeJMP (
                                m.get<void> (),
                                RandomizeGameMetadataRequestByHash);
                    }
            });
    }

public:
    /*******************************************************/
    SoundsRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().sounds.enabled)
            return;

        InitialiseAllComponents ();
        Rainbomizer::Common::AddEpisodeChangeCallback (
            InitialiseRandomizationForEpisode);

        InitialiseVoiceLineRandomizerPatches ();
        InitialiseSfxRandomizerPatches ();
        InitialiseMusicRandomizerPatches ();

        Rainbomizer::Logger::LogMessage ("Initialised SoundsRandomizer");
    }
};

injector::scoped_jmp                      SoundsRandomizer::mHook8f5b00{};
std::unordered_map<uint32_t, std::string> SoundsRandomizer::mTexts;
std::array<std::pair<uint32_t, std::string>, 50>
                       SoundsRandomizer::mBankAudioPairs{};
int                    SoundsRandomizer::mCorrectedBankHash  = -1;
bool                   SoundsRandomizer::mAdjustNextBankHash = false;
std::vector<SoundPair> SoundsRandomizer::mSounds;

SoundsRandomizer _sounds;
