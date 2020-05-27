#include "CPools.hh"
#include "Utils.hh"

atPool<CVehicleStruct> **g_pVehicleStructPool;
atPool<CPed> **          g_pPedsPool;

/*******************************************************/
atPool<CVehicleStruct> *&
CPools::g_pVehicleStructPool ()
{
    return *::g_pVehicleStructPool;
}

/*******************************************************/
atPool<CPed> *&
CPools::g_pPedsPool ()
{
    return *::g_pPedsPool;
}

/*******************************************************/
void
CPools::InitialisePatternsCE ()
{
    ::g_pVehicleStructPool
        = *hook::get_pattern<atPool<CVehicleStruct> **> ("68 0c 02 00 00 68",
                                                         20);

#warning "Peds pool not converted for CE"
}

/*******************************************************/
void
CPools::InitialisePatterns ()
{
    ::g_pVehicleStructPool
        = *hook::get_pattern<atPool<CVehicleStruct> **> ("68 0c 02 00 00 68",
                                                         20);

    ::g_pPedsPool = *hook::get_pattern<atPool<CPed> **> (
        "80 7c ? ? 00 0f 84 ? ? ? ? a1 ? ? ? ? 8b ? ? ", 12);
}
