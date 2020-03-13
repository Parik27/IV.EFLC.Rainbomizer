#pragma once
#include <cstdint>

class CCrypto
{    
public:
    static uint32_t HashString (const char *string, int seed = 0);
    static uint32_t HashStringLowercase (const char *string);
    
    static void InitialisePatterns();
};
