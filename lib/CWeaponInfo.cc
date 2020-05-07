#include "CWeaponInfo.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"

// 8b 44 24 04 83 f8 3c 7d ?
CWeaponInfo *(*CWeaponInfo__GetInfoFromIndex) (int);

/*******************************************************/
CWeaponInfo *
CWeaponInfo::GetInfoFromIndex (int index)
{
    return CWeaponInfo__GetInfoFromIndex (index);
}

/*******************************************************/
void
CWeaponInfo::InitialisePatternsCE ()
{
    CWeaponInfo::InitialisePatterns ();
}

/*******************************************************/
void
CWeaponInfo::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("8b 44 24 04 83 f8 3c 7d ?"),
                 CWeaponInfo__GetInfoFromIndex);
}
