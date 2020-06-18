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

    ::g_pPedsPool = *hook::get_pattern<atPool<CPed> **> (
        "8b 0d ? ? ? ? 53 56 57 ff 74 ? ? e8 ? ? ? ? 8b 4c ? 1c ", 2);
}

/*******************************************************/
void
CPools::InitialisePatterns ()
{
    ::g_pVehicleStructPool
        = *hook::get_pattern<atPool<CVehicleStruct> **> ("68 0c 02 00 00 68",
                                                         20);

    ::g_pPedsPool = *hook::get_pattern<atPool<CPed> **> (
        "8b 44 ? ? 8b 0d ? ? ? ? 53 55 56 57 50 ", 6);
}
