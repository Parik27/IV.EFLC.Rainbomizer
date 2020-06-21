#include "Utils.hh"
#include <vector>
#include <algorithm>
#include "common.hh"
#include "config.hh"
#include "logger.hh"
#include "CPed.hh"
#include "CDrawCommands.hh"
#include "CModelInfoStore.hh"

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

        Rainbomizer::Logger::LogMessage ("Initialised CutsceneRandomizer");
    }
};

std::vector<std::vector<std::string>> CutsceneRandomizer::mModels{};

CutsceneRandomizer _cuts;
