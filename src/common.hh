#pragma once

#include <vector>
#include <functional>

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
    
    static std::vector<int>& GetVehicleIndices();
    static std::vector<int>& GetPedIndices();

    static int GetStoredEpisodeNumber();
    
    static void AddEpisodeChangeCallback(std::function<void(int)> callback);
    
};
}; // namespace Rainbomizer
