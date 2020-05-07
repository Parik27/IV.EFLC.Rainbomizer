#pragma once
#include <cstdint>

class CCrypto
{
public:
    static uint32_t HashStringLowercase (const char *string);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};
