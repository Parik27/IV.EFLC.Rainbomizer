#include "Utils.hh"
#include <vector>
#include <algorithm>

int (*scanf__8665d0) (char *str, char *format, ...);

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
        strcpy (name, GetRandomModel(name).c_str());

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
    static bool InitialiseModelData()
    {
        FILE* modelFile = fopen("Cutscene_Models.txt", "r");
        if(!modelFile)
            return false;

        char line[512] = {0};
        mModels.push_back({});
        while(fgets (line ,512, modelFile))
            {
                if(strlen(line) < 2)
                    {
                        mModels.push_back(std::vector<std::string>());
                        continue;
                    }
                line[strcspn (line, "\n")] = 0;
                mModels.back ().push_back (line);
            }

        return true;
    }

public:
    /*******************************************************/
    CutsceneRandomizer ()
    {
        InitialiseAllComponents ();
        if (InitialiseModelData ())
            RegisterHook ("e8 ? ? ? ? 8b 4c ? 3c 83 c4 1c", 0, scanf__8665d0,
                          HookedAddCutsceneModel);
    }
};

std::vector<std::vector<std::string>> CutsceneRandomizer::mModels{};

CutsceneRandomizer _cuts;
