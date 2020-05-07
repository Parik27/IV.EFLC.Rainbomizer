#pragma once

#include <CEntity.hh>
#include <cstddef>

class CPedWeapons
{
public:
    void GiveWeapon (int weapon, int ammo, char param4, char param5,
                     bool shown);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};

class CPed
{
    static int m_pWeaponsOffset;

public:
    char         __unk00[672] = {0};
    CPedWeapons &m_pWeapons ();

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};
