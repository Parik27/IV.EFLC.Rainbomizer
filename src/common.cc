#include "Utils.hh"
#include "common.hh"
#include "CScrVM.hh"
#include "CModelInfoStore.hh"
#include "CModelInfo.hh"
#include <filesystem>
#include "logger.hh"

CPed *(*FindPlayerPed_48e602) (int);

namespace Rainbomizer {

/*******************************************************/
std::string
Common::GetEpisodeNameFromID (int id)
{
    switch (id)
        {
        case 0: return "IV";
        case 1: return "TLAD";
        case 2: return "TBOGT";
        default: return std::to_string (id);
        }
}

/*******************************************************/
CPed *
Common::HandleEpisodicChanges (int num)
{
    int episode;

    CNativeManager::CallNativeRet (&episode, "GET_CURRENT_EPISODE");

    if (episode != mPreviousEpisode)
        {
            Logger::LogMessage ("Detected episode switch: %d to %d",
                                mPreviousEpisode, episode);
            
            mPreviousEpisode = episode;
            
            for (auto i : GetCallbacks ())
                i (episode);
        }
    

    return FindPlayerPed_48e602 (num);
}

/*******************************************************/
Common::Common ()
{
    InitialiseAllComponents ();
    RegisterHook ("e8 ? ? ? ? 8b f0 83 c4 04 85 f6 74 ? 8b 06 ", 0,
                  FindPlayerPed_48e602, Common::HandleEpisodicChanges);

    // Add a callback to reset the indices
    this->AddEpisodeChangeCallback ([&] (int) {
        this->mPedIndices.clear ();
        this->mVehicleIndices.clear ();

        this->mIndicesInitialised = false;
    });
}

/*******************************************************/
void
Common::AddEpisodeChangeCallback (std::function<void (int)> callback)
{
    GetCallbacks ().push_back (callback);
}

/*******************************************************/
void
Common::InitialiseIndices ()
{
    for (size_t i = 0; i < CModelInfoStore::m_nTotalModelPointers; i++)
        {
            auto info = CModelInfoStore::m_aModelPointers[i];
            auto type = info->__vmt->GetType ();

            switch (type)
                {
                    case MODEL_INFO_PED: {

                        // Can't spawn more than 1 players, so we don't add
                        // them to the array
                        auto ped = reinterpret_cast<CPedModelInfo *> (info);

                        if (!ped->m_nPlayerPedType)
                            mPedIndices.push_back (i);

                        break;
                    }

                    case MODEL_INFO_VEHICLE: {

                        // Trains crash the game when spawned in traffic, so
                        // skip them
                        auto veh = reinterpret_cast<CVehicleModelInfo *> (info);

                        if (veh->m_nType != VEHICLE_TYPE_TRAIN)
                            mVehicleIndices.push_back (i);

                        break;
                    }

                default: break;
                }
        }

    Logger::LogMessage ("Initialised model indices");
    Logger::LogMessage ("Ped models: %d, Vehicle models: %d",
                        mPedIndices.size (), mVehicleIndices.size ());

    mIndicesInitialised = true;
}

/*******************************************************/
std::vector<int> &
Common::GetVehicleIndices ()
{
    if (!mIndicesInitialised)
        InitialiseIndices ();

    return mVehicleIndices;
}

/*******************************************************/
std::vector<std::function<void (int)>> &
Common::GetCallbacks ()
{
    static std::vector<std::function<void (int)>> mCallbacks;
    return mCallbacks;
}

struct GamePathA
{
    char _path[160 + 1];
    char _temp_path[160 + 1];

    GamePathA ()
    {
      _path[0]  = '\0';
        HMODULE h = NULL;
        h         = GetModuleHandleA (NULL);
        GetModuleFileNameA (h, _path, MAX_PATH);
        char *bslp = strrchr (_path, '\\');
        char *fslp = strrchr (_path, '/');
        char *slp  = (std::max) (bslp, fslp);
        if (slp)
            slp[1] = '\0';
    }
};

/*******************************************************/
char *
GetGameDirRelativePathA (const char *subpath)
{
    static GamePathA gamePath;
    strcpy (gamePath._temp_path, gamePath._path);
    strcat (gamePath._temp_path, subpath);
    return gamePath._temp_path;
}

/*******************************************************/
std::string
Common::GetRainbomizerFileName (const std::string &name,
                                const std::string &subdirs)
{
    std::string baseDir = GetGameDirRelativePathA (("rainbomizer/" + subdirs).c_str());
    std::filesystem::create_directories (baseDir);
    return baseDir + name;
}

/*******************************************************/
FILE *
Common::GetRainbomizerFile (const std::string &name, const std::string &mode,
                            const std::string &subdirs)
{
    return fopen (GetRainbomizerFileName (name, subdirs).c_str (),
                  mode.c_str ());
}

/*******************************************************/
FILE *
Common::GetRainbomizerDataFile (const std::string &name,
                                const std::string &mode, bool episodic)
{
    std::string dirs
        = episodic ? GetEpisodeNameFromID (GetStoredEpisodeNumber ()) : "";

    FILE *f = GetRainbomizerFile (name, mode, "data/" + dirs + "/");
    if(!f)
        Logger::LogMessage ("Failed to read Rainbomizer data file: %s/data/%s",
                            dirs.c_str(), name.c_str());

    return f;
}

/*******************************************************/
std::vector<int> &
Common::GetPedIndices ()
{
    if (!mIndicesInitialised)
        InitialiseIndices ();

    return mPedIndices;
}
    
bool             Common::mIndicesInitialised = false;
std::vector<int> Common::mVehicleIndices;
std::vector<int> Common::mPedIndices;
int              Common::mPreviousEpisode = -1;

Common _utils;
}; // namespace Rainbomizer
