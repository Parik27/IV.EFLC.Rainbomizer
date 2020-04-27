#include "CPed.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"

void (__thiscall *CPedWeapons__GiveWeapon) (CPedWeapons *, int, int, char, char,
                                            bool);

/*******************************************************/
void
CPedWeapons::GiveWeapon (int weapon, int ammo, char param4, char param5,
                         bool shown)
{
    CPedWeapons__GiveWeapon (this, weapon, ammo, param4, param5, shown);
}

/*******************************************************/
void
CPedWeapons::InitialisePatternsCE ()
{
    ConvertCall (hook::get_pattern (
                     "53 55 56 8b f1 8b 4c ? ? 57 51 e8 ? ? ? ?"),
                 CPedWeapons__GiveWeapon);
}


/*******************************************************/
void
CPedWeapons::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("e8 ? ? ? ? 8b ? 24 1c 8b ? 04 83 c4 04",
                                    -11),
                 CPedWeapons__GiveWeapon);
}

/*******************************************************/
CPedWeapons& CPed::m_pWeapons()
{
    return *(CPedWeapons *) (((char *) this) + CPed::m_pWeaponsOffset);
}

/*******************************************************/
void
CPed::InitialisePatternsCE()
{
    CPed::m_pWeaponsOffset
        = *hook::get_pattern<int> ("6a 00 6a 00 6a 32 6a 07 8d", 10);
}

/*******************************************************/
void
CPed::InitialisePatterns ()
{
    CPed::m_pWeaponsOffset
        = *hook::get_pattern<int> ("6a 00 6a 00 6a 01 6a 03 8d", 10);
}

int CPed::m_pWeaponsOffset = 688;
