#include "CScrVM.hh"
#include "CCrypto.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "injector/hooking.hpp"
#include "CVehicleFactory.hh"
#include "CStreaming.hh"
#include "config.hh"
#include "logger.hh"
#include "common.hh"
#include "CModelInfoStore.hh"
#include <chrono>
#include <unordered_map>

int (*scanf_aca2e7) (char *, char *, ...);

struct Vehicle
{
    std::string name;
    std::string type;
};

class ScriptVehicleRandomizer
{
    static std::unordered_map<int, int> mSeatsCache;
    
    /*******************************************************/
    static uint32_t
    GetVehicleForModel (uint32_t hash)
    {
        auto indices = Rainbomizer::Common::GetVehicleIndices();
        auto model   = CModelInfoStore::GetModelInfoPointer<CVehicleModelInfo> (
            indices[RandomInt (indices.size () - 1)]);

        return model->m_nModelHash;
    }

    /*******************************************************/
    static void
    CreateCarNativeHook (NativeData *data)
    {

        static int number = 0;
        // Vehicle    car             = mVehicles[RandomInt(mVehicles.size () -
        // 1)];
        uint32_t originalHash = data->Params[0].uint_param;

        data->Params[0].uint_param = GetVehicleForModel (
            originalHash); // CCrypto::HashString (car.name.c_str ());

        // This does stuff related to unloading and loading the old/new vehicles
        // CNativeManager::CallNative ("MARK_MODEL_AS_NO_LONGER_NEEDED",
        // data->Params[0].uint_param);

        // TODO: Replace this with random loaded vehicle once the exe is more
        // reverse engineered.
        // Fallback for when the model couldn't get loaded (Streaming limits)
        if (!CStreaming::AttemptToLoadModel (data->Params[0].uint_param))
            data->Params[0].uint_param = originalHash;

        CNativeManager::CallOriginalNative ("CREATE_CAR", data);
    }

    /*******************************************************/
    static void
    InitialiseCarSeatsCache (int)
    {
        auto indices = Rainbomizer::Common::GetVehicleIndices ();
        auto timestamp = clock();
        
        for (auto i : indices)
            {
                auto model
                    = CModelInfoStore::GetModelInfoPointer<CVehicleModelInfo> (
                        i);

                if (CStreaming::AttemptToLoadModel (model->m_nModelHash))
                    mSeatsCache[i]
                        = CModelInfoStore::GetMaximumNumberOfPassengers (i);

                CStreaming::FreeWdrModel (i);
                puts("Freed");
            }

        Rainbomizer::Logger::LogMessage (
            "Initialised Seat Count cache in %.2f seconds",
            1.0f * (clock () - timestamp) / CLOCKS_PER_SEC);
    }
    
public:
    /*******************************************************/
    ScriptVehicleRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().scriptVehicle.enabled)
            return;

        InitialiseAllComponents ();

        Rainbomizer::Common::AddEpisodeChangeCallback(InitialiseCarSeatsCache);

        CNativeManager::OverwriteNative ("CREATE_CAR", CreateCarNativeHook);
        Rainbomizer::Logger::LogMessage ("Initialised ScriptVehicleRandomizer");
    }
};

std::unordered_map<int, int> ScriptVehicleRandomizer::mSeatsCache{};
ScriptVehicleRandomizer _scr;
