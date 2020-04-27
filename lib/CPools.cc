#include "CPools.hh"
#include "Utils.hh"

CPool<CVehicleStruct> **g_pVehicleStructPool;

/*******************************************************/
CPool<CVehicleStruct> *&
CPools::g_pVehicleStructPool ()
{
    return *::g_pVehicleStructPool;
}

/*******************************************************/
void
CPools::InitialisePatterns ()
{
    ::g_pVehicleStructPool = *hook::get_pattern<CPool<CVehicleStruct> **> (
        "68 0c 02 00 00 68 ? ? ? ? 6a ? 8b c8", 20);
}
