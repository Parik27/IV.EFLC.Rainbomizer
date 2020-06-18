#include "CPed.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "injector/calling.hpp"

void (__thiscall *CPedWeapons__GiveWeapon) (CPedInventory *, int, int, char,
                                            char, bool);
Matrix34 *(__thiscall *CPed__GetBoneMatrix) (CPed *, int);
short (__stdcall *crSkeleton__ConvertBoneIdToIndex) (void *, ePedBone);

/*******************************************************/
void
CPedInventory::GiveWeapon (int weapon, int ammo, char param4, char param5,
                           bool shown)
{
    CPedWeapons__GiveWeapon (this, weapon, ammo, param4, param5, shown);
}

/*******************************************************/
void
CPedInventory::InitialisePatternsCE ()
{
    ConvertCall (hook::get_pattern (
                     "53 55 56 8b f1 8b 4c ? ? 57 51 e8 ? ? ? ?"),
                 CPedWeapons__GiveWeapon);
}

/*******************************************************/
void
CPedInventory::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("e8 ? ? ? ? 8b ? 24 1c 8b ? 04 83 c4 04",
                                    -11),
                 CPedWeapons__GiveWeapon);
}

/*******************************************************/
short
CPed::GetBoneIndexFromId (ePedBone boneId)
{
    // Gets skeleton function thingy or something, copied from game code.
    injector::memory_pointer_raw skel = nullptr;
    skel = injector::thiscall<void *(CPed *)>::vtbl<40> (this);
    if (!skel)
        skel = *injector::ReadMemory<void **> (this + 0x100);
    else
        skel = injector::thiscall<void *(void *)>::vtbl<56> (
            skel.get_raw<void *> ());

    return crSkeleton__ConvertBoneIdToIndex (*(skel + 4).get_raw<void **> (),
                                             boneId);
}

/*******************************************************/
Matrix34 *
CPed::GetBoneMatrix (int bone)
{
    return CPed__GetBoneMatrix (this, bone);
}

/*******************************************************/
CPedInventory &
CPed::m_pWeapons ()
{
    return *(CPedInventory *) (((char *) this) + CPed::m_pWeaponsOffset);
}

/*******************************************************/
void
CPed::InitialisePatternsCE ()
{
    CPed::m_pWeaponsOffset
        = *hook::get_pattern<int> ("6a 00 6a 00 6a 32 6a 07 8d", 10);

    ReadCall (hook::get_pattern ("83 c4 08 8b cf 50 e8 ? ? ? ? 8b 54 ? 0c ", 6),
              CPed__GetBoneMatrix);

    ReadCall (
        hook::get_pattern ("56 ff 74 ? 10 ff 70 04 e8 ? ? ? ? 83 c4 08 ", 8),
        crSkeleton__ConvertBoneIdToIndex);
}

/*******************************************************/
void
CPed::InitialisePatterns ()
{
    CPed::m_pWeaponsOffset
        = *hook::get_pattern<int> ("6a 00 6a 00 6a 01 6a 03 8d", 10);

    ConvertCall (SearchBack ("85 c9 0f 85 ? ? ? ? 0f 57 c0 f3 0f 10 0d",
                             "56 8b f1 8b 06 8b 90 a0 00 00 00 ", 60),
                 CPed__GetBoneMatrix);

    ConvertCall (hook::get_pattern ("51 8b 4c ? ? 8b 41 1c d1 e8 a8 01 "),
                 crSkeleton__ConvertBoneIdToIndex);
}

int CPed::m_pWeaponsOffset = 688;
