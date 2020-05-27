#pragma once
#include <cstdint>

class CCrypto
{
public:
    static uint32_t atStringHash (const char *string);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};

constexpr std::uint32_t
atStringHash (char const *s, size_t len)
{
    size_t   i    = 0;
    uint32_t hash = 0;
    while (i != len)
        {
            hash += s[i++];
            hash += hash << 10;
            hash ^= hash >> 6;
        }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

constexpr std::uint32_t operator"" _joaat (char const *s, size_t len)
{
    return atStringHash (s, len);
}
