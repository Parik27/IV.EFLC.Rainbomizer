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
#include <CCarGrp.hh>
#include <algorithm>
#include <CCrypto.hh>

int (*GetPedModelForVehicle_ab333d) (int);
CVehicleModelInfo *(*CModelInfoStore__AllocateVehicleModel9adc6d) (char *);

class TrafficRandomizer
{

    /*******************************************************/
    inline static bool
    IsCarPartOfGroupExtra (uint32_t model, ePopcycleGrps group)
    {
        if (group != POPCYCLE_GROUP_COPS)
            return false;

        switch (CModelInfoStore::m_aModelPointers[model]->m_nModelHash)
            {
            case "polmav"_joaat:
            case "annihilator"_joaat: return true;
            }

        return false;
    }

    /*******************************************************/
    inline static void
    RemoveGroupFromCarsSet (std::set<int> &cars, ePopcycleGrps group)
    {
        for (auto it = cars.begin (); it != cars.end ();)
            {
                if (CCarGrp::IsCarPartOfGroup (*it, group)
                    || IsCarPartOfGroupExtra (*it, group))
                    it = cars.erase (it);
                else
                    ++it;
            }
    }

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

        // To prevent an insane amount of cop cars spawning if you have a wanted
        // level.
        if (wanted)
            RemoveGroupFromCarsSet (cars, POPCYCLE_GROUP_COPS);

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
        void *addr = hook::get_pattern (ByVersion (
            "81 ec 04 01 00 00 e8 ? ? ? ? 85 c0 7f ?",
            "81 ec 08 01 00 00 a1 ? ? ? ? 33 c4 89 84 ? ? ? ? ? 56 8b b4"));
        injector::MakeJMP (addr, GetRandomCarToGenerate);
    }

    /*******************************************************/
    static int
    GetPedModelForVehicle (int id)
    {
        // Similar to GetRandomCarToGenerate
        int ped = GetPedModelForVehicle_ab333d (id);
        if (ped != -1)
            return ped;

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

        RegisterHook (ByVersion ("74 ? 50 8b cb e8 ? ? ? ? 8b f8 85 ff",
                                 "74 ? 51 8b cb e8 ? ? ? ? 8b f8 85 ff"),
                      5, RandomizeCarToLoad);

        if (ConfigManager::GetConfigs ().traffic.enableRandomPeds)
            {
                RegisterHook (
                    ByVersion ("eb ? 0f bf 47 2e 50 e8 ? ? ? ? 83 c4 04 8b d8 ",
                               "0f bf 47 2e 50 e8 ? ? ? ? 83 c4 04 8b f0 "),
                    ByVersion (7, 5), GetPedModelForVehicle_ab333d,
                    GetPedModelForVehicle);
            }

        Rainbomizer::Logger::LogMessage ("Initialised TrafficRandomizer");
    }
};

TrafficRandomizer _traf;
