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
CPedWeapons::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("e8 ? ? ? ? 8b ? 24 1c 8b ? 04 83 c4 04",
                                    -11),
                 CPedWeapons__GiveWeapon);
}
