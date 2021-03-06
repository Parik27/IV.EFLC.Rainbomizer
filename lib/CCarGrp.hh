#pragma once

#include <cstdint>

// Autogenerated by Ghidra - too lazy to rearrarange sorry :P
enum ePopcycleGrps : uint32_t
{
    POPCYCLE_GROUP_BOHEMIAN              = 9,
    POPCYCLE_GROUP_GANG_BIKER_1          = 31,
    POPCYCLE_GROUP_GANG_BIKER_2          = 32,
    POPCYCLE_GROUP_NETWORK               = 43,
    POPCYCLE_GROUP_TRENDY                = 10,
    POPCYCLE_GROUP_GANG_KOREAN           = 39,
    POPCYCLE_GROUP_ITALIAN               = 12,
    POPCYCLE_GROUP_BRONX                 = 1,
    POPCYCLE_GROUP_SHOPPING_AVERAGE      = 17,
    POPCYCLE_GROUP_GANG_ALBANIAN         = 30,
    POPCYCLE_GROUP_BLACK_POOR            = 7,
    POPCYCLE_GROUP_GANG_IRISH            = 36,
    POPCYCLE_GROUP_COPS                  = 42,
    POPCYCLE_GROUP_PARK                  = 22,
    POPCYCLE_GROUP_GANG_CHINESE_JAPANESE = 40,
    POPCYCLE_GROUP_GANG_AFRICAN_AMERICAN = 38,
    POPCYCLE_GROUP_SOUTH_AMERICAN        = 16,
    POPCYCLE_GROUP_TAXI                  = 24,
    POPCYCLE_GROUP_GANG_ITALIAN          = 33,
    POPCYCLE_GROUP_PROSTITUTES_PIMPS     = 28,
    POPCYCLE_GROUP_UPTOWN                = 3,
    POPCYCLE_GROUP_GANG_RUSSIAN_2        = 35,
    POPCYCLE_GROUP_AIRPORT_WORKERS       = 26,
    POPCYCLE_GROUP_GOLF                  = 23,
    POPCYCLE_GROUP_QUEENS                = 4,
    POPCYCLE_GROUP_ONLY_IN_NATIVE_ZONE   = 46,
    POPCYCLE_CAR_GROUP_BOATS             = 44,
    POPCYCLE_GROUP_MIDTOWN               = 6,
    POPCYCLE_GROUP_FASHION               = 8,
    POPCYCLE_GROUP_DOCKS                 = 18,
    POPCYCLE_GROUP_GANG_RUSSIAN          = 34,
    POPCYCLE_GROUP_SECURITY              = 27,
    POPCYCLE_CAR_GROUP_BORING_SALOONS    = 45,
    POPCYCLE_GROUP_GANG_PUERTO_RICAN     = 41,
    POPCYCLE_GROUP_FACTORY               = 21,
    POPCYCLE_GROUP_GANG_JAMAICAN         = 37,
    POPCYCLE_GROUP_SHOPPING_RICH         = 14,
    POPCYCLE_GROUP_BUSINESS_HIGH         = 19,
    POPCYCLE_GROUP_RUSSIAN               = 11,
    POPCYCLE_GROUP_BUSINESS_LOW          = 20,
    POPCYCLE_GROUP_BUMS                  = 29,
    POPCYCLE_GROUP_CHINESE_JAPANESE      = 15,
    POPCYCLE_GROUP_JERSEY                = 2,
    POPCYCLE_GROUP_HARLEM                = 0,
    POPCYCLE_GROUP_BROOKLYN              = 5,
    POPCYCLE_GROUP_BUILDING_SITE         = 25,
    POPCYCLE_GROUP_EASTERN_EUROPEAN      = 13
};

class CCarGrp
{
public:
    static bool IsCarPartOfGroup (uint32_t modelId, ePopcycleGrps group);

    static void InitialisePatterns ();
    static void InitialisePatternsCE ();
};
