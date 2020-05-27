#pragma once

#include <cstdint>

template <typename T> class atHashPair
{
public:
    uint32_t hash;
    T        data;
};

template <typename T> class audHashPair
{
public:
    T        data;
    uint32_t hash;
};

template <typename T = void *> class atMap
{
    T *      m_aData;
    uint16_t m_nCapacity;
    uint16_t m_nSize;
    uint32_t field_0x8;
};

static_assert (sizeof (atMap<>) == 12);
static_assert (sizeof (audHashPair<uint32_t>) == 8);
