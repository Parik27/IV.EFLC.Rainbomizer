#include <cstdint>
#include "CARGB.hh"

#pragma once

class CHud
{
public:
    // aColours[66]
    static CARGB *aColours;

    // aColourNameHashes
    static uint32_t *aColourNameHashes;

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};
