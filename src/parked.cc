#include <CCarGenerator.hh>
#include "Utils.hh"
#include "common.hh"
#include "config.hh"
#include "logger.hh"
#include "CModelInfoStore.hh"

bool (__thiscall *CCarGenerator__Processa90d71) (CCarGenerator *cgen,
                                                 char param_2, char param_3);

class ParkedCarRandomizer
{
    /*******************************************************/
    static bool CanRandomizeCarGenerator (CCarGenerator* cgen)
    {
        // Stevie's Car Thefts fix
        if (!cgen->m_nFlags2.isMission)
            return true;

        // None of the Stevie's Cars are helis/boats, so we can safely randomize
        // those
        return CModelInfoStore::GetModelInfoPointer<CVehicleModelInfo>
            (cgen->m_nModelId)->m_nType != VEHICLE_TYPE_CAR;
    }

    /*******************************************************/
    static bool __fastcall RandomizeCarGenerator (CCarGenerator *cgen,
                                                  void *edx, char param_2,
                                                  char param_3)
    {
        auto original_value = cgen->m_nModelId;

        const auto &vehicles = Rainbomizer::Common::GetVehicleIndices ();
        if (cgen->m_nModelId != -1 && CanRandomizeCarGenerator (cgen))
            cgen->m_nModelId = vehicles[RandomInt (vehicles.size () - 1)];

        bool ret = CCarGenerator__Processa90d71 (cgen, param_2, param_3);
        cgen->m_nModelId = original_value;

        return ret;
    }

public:
    /*******************************************************/
    ParkedCarRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().parkedCar.enabled)
            return;

        InitialiseAllComponents ();
        RegisterHook (
            ByVersion ("51 6a 00 8b ce e8 ? ? ? ? eb ? 80 7c ? ? 00",
                       "ff 74 ? ? 8b ce 6a 00 e8 ? ? ? ? eb ? 80 7c ? ? 00"),
            ByVersion (5, 8), CCarGenerator__Processa90d71,
            RandomizeCarGenerator);

        Rainbomizer::Logger::LogMessage ("Initialised ParkedCarRandomizer");
    }
} _pcar;
