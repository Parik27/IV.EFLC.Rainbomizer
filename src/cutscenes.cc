#include "Utils.hh"
#include <vector>
#include <algorithm>
#include "common.hh"
#include "config.hh"
#include "logger.hh"
#include "CPed.hh"
#include "CDrawCommands.hh"
#include "CModelInfoStore.hh"
#include <CTheScripts.hh>
#include <array>

int (*scanf__8665d0) (char *str, char *format, ...);
void (__thiscall *DrawCutscenePed_0b99b41)(CPed*, void*, void*);

class CutsceneRandomizer
{
    static std::vector<std::vector<std::string>> mModels;

    /*******************************************************/
    static int
    HookedAddCutsceneModel (char *str, char *format, int *id, char *name,
                            char *param5, char *param6, char *param7,
                            int *param8)
    {
        int ret = scanf__8665d0 (str, format, id, name, param5, param6, param7,
                                 param8);
        strcpy (name, GetRandomModel (name).c_str ());

        return ret;
    }

    /*******************************************************/
    static std::string
    GetRandomModel (std::string model)
    {
        std::transform (std::begin (model), std::end (model), model.begin (),
                        [] (unsigned char c) { return std::tolower (c); });

        for (auto i : mModels)
            {
                if (std::find (std::begin (i), std::end (i), model)
                    != std::end (i))
                    {
                        auto replaced = i[RandomInt (i.size () - 1)];
                        return replaced;
                    }
            }

        return model;
    }

    /*******************************************************/
    static bool
    InitialiseModelData ()
    {
        FILE *modelFile = Rainbomizer::Common::GetRainbomizerDataFile (
            "CutsceneModels.txt");
        mModels.clear ();

        if (!modelFile)
            return false;

        char line[512] = {0};
        mModels.push_back ({});
        while (fgets (line, 512, modelFile))
            {
                if (strlen (line) < 2)
                    {
                        mModels.push_back (std::vector<std::string> ());
                        continue;
                    }
                line[strcspn (line, "\n")] = 0;
                if (std::string(line) == "cs_stubbs" && !ConfigManager::GetConfigs().cutscenes.enableNSFW)
                    continue;
                mModels.back ().push_back (line);
            }

        return true;
    }

    /*******************************************************/
    static void __fastcall FixSuperLodFings (CPed *ped, void *, void *param_2,
                                             void *param_3)
    {
        if (ped->m_wModelIndex
            != CModelInfoStore::CModelLookup__superlod->m_nIndex)
            return DrawCutscenePed_0b99b41 (ped, param_2, param_3);

        (new CDrawSLODPedDC (ped->GetAnimFrag (), param_2, ped->m_bAlpha,
                             ped->m_bBrightness, param_3))
            ->Render ();
    }

    /*******************************************************/
    static void
    RandomizeFullCutscene (NativeData *data)
    {
        constexpr static std::array sCutscenes
            = {"B_1",     "B_2",     "B_5",     "B_6",     "BC_1",   "BC_2",
               "BC_3",    "IMBC2",   "BR_1",    "BR_2",    "BR_3",   "BR_4",
               "SHOW_1",  "SHOW_2",  "SHOW_3",  "SHOW_4",  "SHOW_5", "SHOW_6",
               "SHOW_7",  "SHOW_8",  "N1_A1",   "N1_A2",   "N2",     "N3",
               "N1",      "CM1",     "CM2",     "CM3",     "CM5",    "CM6",
               "CM7",     "DM_1",    "DM_3",    "DF_2",    "DF_3",   "EM_1",
               "EM_2",    "EM_5",    "EM_6",    "EM_7",    "EM_4",   "IMEM7",
               "IMEM6",   "IMEM1",   "EM_4B",   "EM_3",    "FAU1_B", "FAU3_A1",
               "FAU3_A2", "IMFAU6",  "FAU5_A",  "FAU1_A",  "FAU2_A", "FAU4_A",
               "FAU6_A",  "FAU7_A",  "IMFAU2",  "IMFAU4",  "FAU3_A", "FIN_1",
               "FIN_1AA", "FIN_1AB", "FIN_1BA", "FIN_1BB", "FM_1",   "FM_2",
               "FM_3",    "FM_4",    "FM_5",    "FM6_A",   "FM6_B",  "G_1",
               "G_2",     "G_3",     "GM_3F",   "GM_3D",   "GM_4",   "GM_5",
               "IMGER3",  "IMGER4B", "IMGER4A", "GM_1",    "GM_2",   "IMGER4",
               "IMGM5",   "INTROM",  "J_1",     "J_2",     "J_3",    "J_4",
               "LILJ1_A", "LILJ2_A", "MICH_A",  "IMBHST",  "IMPM1",  "PM_1",
               "PM_3",    "PM_2",    "IMPX4",   "PX_1",    "PX_2",   "PXDF",
               "PX_1_A2", "PX_1_A1", "RPMAR1",  "RPBAD1",  "RPBRI1", "RPBRI2",
               "RPBRI3",  "RPED1",   "RPED2",   "RPEXG1",  "RPGHN1", "RPHO1",
               "RPILY1",  "RPIVN1",  "RPJEF1",  "RPJEF2",  "RPJEF3", "RPMAR2",
               "RPMEL1",  "RPMOBD1", "RPPAT1",  "RPPAT2",  "RPSAR1", "RPSAR2",
               "IMRAY1",  "IMRAY2",  "RB_1",    "RB_2",    "RB_3",   "RB_4",
               "RB_4B",   "RB_5",    "RB_6",    "IMROM4",  "R_9",    "ROM1_B",
               "ROM2_A",  "ROM3_A",  "ROM5_A",  "ROM6_A",  "ROM8_A", "ROM8_B",
               "ROM8_C",  "RP_11",   "RP_11B",  "RP_12",   "RP_13",  "RP_14",
               "WEDINT",  "VLA1_A",  "VLA2_A",  "VLA3_A",  "VLA4_A"};

        data->Params[0].cstr_param
            = (char *) sCutscenes[RandomInt (sCutscenes.size () - 1)];

        CNativeManager::CallOriginalNative ("START_CUTSCENE_NOW", data);
    }

public:
    /*******************************************************/
    CutsceneRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().cutscenes.enabled)
            return;

        InitialiseAllComponents ();
        Rainbomizer::Common::AddEpisodeChangeCallback (
            [] (int) { InitialiseModelData (); });
        RegisterHook (ByVersion ("e8 ? ? ? ? 8b 4c ? 3c 83 c4 1c",
                                 "e8 ? ? ? ? 8b 74 ? ? 83 c4 1c "),
                      0, scanf__8665d0, HookedAddCutsceneModel);

        // c2 10 00 53 8b 5c ? ? 57 8b 7c ? ? 57
        RegisterHook (
            ByVersion ("c2 10 00 53 8b 5c ? ? 57 8b 7c ? ? 57 ",
                       "c2 10 00 ff 74 ? ? ff 74 ? ? e8 ? ? ? ? ff 74"),
            ByVersion (15, 11), DrawCutscenePed_0b99b41, FixSuperLodFings);

        CNativeManager::OverwriteNative("START_CUTSCENE_NOW", RandomizeFullCutscene);
        
        Rainbomizer::Logger::LogMessage ("Initialised CutsceneRandomizer");
    }
};

std::vector<std::vector<std::string>> CutsceneRandomizer::mModels{};

CutsceneRandomizer _cuts;
