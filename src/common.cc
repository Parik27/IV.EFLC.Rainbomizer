#include "Utils.hh"
#include "common.hh"
#include "CScrVM.hh"
#include "CModelInfoStore.hh"
#include "CModelInfo.hh"

CPed *(*FindPlayerPed_48e602) (int);

namespace Rainbomizer {

/*******************************************************/
CPed *
Common::HandleEpisodicChanges (int num)
{
    int episode;

    CNativeManager::CallNativeRet (&episode, "GET_CURRENT_EPISODE");

    if (episode != mPreviousEpisode)
        {
            for (auto i : GetCallbacks())
                i (episode);
        }
    mPreviousEpisode = episode;

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
    GetCallbacks().push_back(callback);
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

    mIndicesInitialised = true;
}

/*******************************************************/
std::vector<int> &
Common::GetVehicleIndices ()
{
    if(!mIndicesInitialised)
        InitialiseIndices();
    
    return mVehicleIndices;
}

/*******************************************************/
std::vector<std::function<void (int)>> &
Common::GetCallbacks ()
{
    static std::vector<std::function<void(int)>> mCallbacks;
    return mCallbacks;
}
    
/*******************************************************/
std::vector<int> &
Common::GetPedIndices ()
{
    if(!mIndicesInitialised)
        InitialiseIndices();
    
    return mPedIndices;
}

bool             Common::mIndicesInitialised = false;
std::vector<int> Common::mVehicleIndices;
std::vector<int> Common::mPedIndices;
int              Common::mPreviousEpisode = -1;

Common           _utils;
}; // namespace Rainbomizer
