#include "CVehicleFactory.hh"
#include "Patterns/Patterns.hh"

/*******************************************************/
CVehicle *
CVehicleFactory::CreateCar (int index, int unk0, int unk2, int unk3)
{
    return this->vtable->CreateCar (this, index, unk0, unk2, unk3);
}

void
CVehicleFactory::InitialisePatternsCE ()
{
    hook::pattern pattern (
        "8b f0 85 f6 74 ? 8b ce e8 ? ? ? ? c7 06 ? ? ? ? 89 35 ? ? ? ? eb");

    g_pVehicleFactory = *pattern.get (1).get<CVehicleFactory **> (15);
}

void
CVehicleFactory::InitialisePatterns ()
{
    hook::pattern pattern ("8b f0 3b f3 74 ? 8b ce e8 ? ? ? ? c7 06 ? ? "
                           "? ? 89 35 ? ? ? ? eb ?");

    g_pVehicleFactory = *pattern.get (1).get<CVehicleFactory **> (28);
}

CVehicleFactory **CVehicleFactory::g_pVehicleFactory = nullptr;
