#include <CCarGenerator.hh>
#include "Utils.hh"
#include "common.hh"
#include "config.hh"
#include "logger.hh"

bool (__thiscall *CCarGenerator__Processa90d71) (CCarGenerator *cgen,
                                                 char param_2, char param_3);

class ParkedCarRandomizer
{
    /*******************************************************/
    static bool __fastcall RandomizeCarGenerator (CCarGenerator *cgen,
                                                  void *edx, char param_2,
                                                  char param_3)
    {
        auto original_value = cgen->m_nModelId;

        auto &vehicles = Rainbomizer::Common::GetVehicleIndices ();
        if (cgen->m_nModelId != -1)
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
        RegisterHook ("51 6a 00 8b ce e8 ? ? ? ? eb ? 80 7c ? ? 00", 5,
                      CCarGenerator__Processa90d71, RandomizeCarGenerator);

        Rainbomizer::Logger::LogMessage ("Initialised ParkedCarRandomizer");
    }
} _pcar;
