#pragma once

#include <vector>
#include <functional>
#include <cstdio>

class CPed;

namespace Rainbomizer {
class Common
{
    static bool mIndicesInitialised;
    
    static std::vector<int> mVehicleIndices;
    static std::vector<int> mPedIndices;

    static CPed* HandleEpisodicChanges(int);
    static void InitialiseIndices();
    
    static int mPreviousEpisode;

    static std::vector<std::function<void (int)>> &GetCallbacks ();
    
public:

    Common();

    static std::string GetEpisodeNameFromID(int id);
    
    static std::vector<int>& GetVehicleIndices();
    static std::vector<int>& GetPedIndices();

    static int GetStoredEpisodeNumber() { return mPreviousEpisode; };
    
    static void AddEpisodeChangeCallback(std::function<void(int)> callback);

    static std::string GetRainbomizerFileName (const std::string &name,
                                               const std::string  &subdirs = "");
    
    static FILE *      GetRainbomizerFile (const std::string &name,
                                           const std::string &mode,
                                           const std::string &subdirs = "");

    // episodic means whether the file is different for each episode
    static FILE *      GetRainbomizerDataFile (const std::string &name,
                                               const std::string &mode = "r",
                                               bool episodic = true);
};
}; // namespace Rainbomizer
