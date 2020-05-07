#include <cstdio>
#include "CTheScripts.hh"
#include "Utils.hh"
#include "CStreaming.hh"
#include "CModelInfo.hh"
#include "CModelInfoStore.hh"
#include <set>
#include "common.hh"
#include "config.hh"
#include "logger.hh"

int (*GetPedModelForVehicle_ab333d) (int);
CVehicleModelInfo *(*CModelInfoStore__AllocateVehicleModel9adc6d) (char *);

class TrafficRandomizer
{

    /*******************************************************/
    static int
    GetRandomCarToGenerate (bool *copCar, int param2, bool wanted,
                            bool smallworker, bool param5)
    {
        std::set<int> cars;
        auto          groups = CStreaming::ms_instance;

        // We combine all four groups into one set (since the first two can
        // overlap)

        groups->m_aAppropriateCars.for_each (
            [&cars] (int val) { cars.insert (val); });
        groups->m_aInappropriateCars.for_each (
            [&cars] (int val) { cars.insert (val); });
        groups->m_aBoats.for_each ([&cars] (int val) { cars.insert (val); });

        groups->m_aSpecialCars.for_each ([&cars] (int val) {
            CVehicleModelInfo *info = reinterpret_cast<CVehicleModelInfo *> (
                CModelInfoStore::m_aModelPointers[val]);

            // Exempt train since they crash the game when spawned
            if (info->m_nType != eVehicleType::VEHICLE_TYPE_TRAIN)
                cars.insert (val);
        });

        if (cars.size () < 1)
            return -1;

        auto it = cars.begin ();
        std::advance (it, RandomInt (cars.size () - 1));

        // Vehicle should always be loaded if they're in the loaded groups, but
        // just in case
        if (CStreaming::HasResourceLoaded (*it,
                                           CStreaming::g_pFileTypeWdrIndex ()))
            return *it;

        return -1;
    }

    /*******************************************************/
    static int __fastcall RandomizeCarToLoad (CStreaming *groups, void *edx,
                                              int group)
    {
        auto &vehicles = Rainbomizer::Common::GetVehicleIndices ();
        for (int i = 0; i < 16; i++)
            {
                int vehicle = vehicles[RandomInt (vehicles.size () - 1)];
                if (CStreaming::HasResourceLoaded (
                        vehicle, CStreaming::g_pFileTypeWdrIndex ()))
                    continue;

                return vehicle;
            }
        return -1;
    }

    /*******************************************************/
    void
    InitialiseSpawnHook ()
    {
        void *addr = hook::get_pattern (VersionedData (
            "81 ec 04 01 00 00 e8 ? ? ? ? 85 c0 7f ?",
            "81 ec 08 01 00 00 a1 ? ? ? ? 33 c4 89 84 ? ? ? ? ? 56 8b b4"));
        injector::MakeJMP (addr, GetRandomCarToGenerate);
    }

    /*******************************************************/
    static int
    GetPedModelForVehicle (int id)
    {
        // Similar to GetRandomCarToGenerate

        std::set<int> peds;
        auto          groups = CStreaming::ms_instance;

        groups->m_aAppropriatePeds.for_each (
            [&peds] (int val) { peds.insert (val); });
        groups->m_aInappropriatePeds.for_each (
            [&peds] (int val) { peds.insert (val); });
        groups->m_aSpecialPeds.for_each ([&peds] (int val) {
            if (val != CModelInfoStore::CModelLookup__superlod->m_nIndex)
                peds.insert (val);
        });

        if (peds.size () < 1)
            return -1;

        auto it = peds.begin ();
        std::advance (it, RandomInt (peds.size () - 1));

        if (CStreaming::HasResourceLoaded (*it,
                                           CStreaming::g_pFileTypeWdrIndex ()))
            return *it;

        return -1;
    }

public:
    /*******************************************************/
    TrafficRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().traffic.enabled)
            return;

        InitialiseAllComponents ();
        InitialiseSpawnHook ();

        RegisterHook (VersionedData ("74 ? 50 8b cb e8 ? ? ? ? 8b f8 85 ff",
                                     "74 ? 51 8b cb e8 ? ? ? ? 8b f8 85 ff"),
                      5, RandomizeCarToLoad);

        RegisterHook (
            VersionedData ("eb ? 0f bf 47 2e 50 e8 ? ? ? ? 83 c4 04 8b d8 ",
                           "0f bf 47 2e 50 e8 ? ? ? ? 83 c4 04 8b f0 "),
            VersionedData (7, 5), GetPedModelForVehicle_ab333d,
            GetPedModelForVehicle);

        Rainbomizer::Logger::LogMessage ("Initialised TrafficRandomizer");
    }
};

TrafficRandomizer _traf;
