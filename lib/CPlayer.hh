#pragma once

#include <CPed.hh>

class CPlayer
{
public:
    // 8b 44 ?? ?? 85 c0 75 ?? a1 ?? ?? ?? ?? 83 f8 ff 74 ??
    static CPed *FindPlayerPed (CPlayer *player = nullptr);

    static CEntity *FindPlayerEntity (CPlayer *player = nullptr);

    static void InitialisePatterns ();
    static void InitialisePatternsCE ();
};
