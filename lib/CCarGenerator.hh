#pragma once

#include "CMaths.hh"
#include <cstdint>

struct CCarGenerator
{
    Vector3  m_vecPos;
    float    m_vec2MinX;
    float    m_vec2MinY;
    float    m_vec2MaxX;
    float    m_vec2MaxY;
    int16_t  m_nModelId;
    short    m_nPoolIndex;
    uint16_t field_20;
    uint8_t  m_aColours[4];
    uint8_t  m_bAlarm;
    uint8_t  m_bLocked;
    uint8_t  m_nType;
    uint8_t  m_nFlags1;
    uint8_t  m_nFlags2;
    uint8_t  m_bEnabled;
};

static_assert (sizeof (CCarGenerator) == 44, "CCarGenerator invalid size");
