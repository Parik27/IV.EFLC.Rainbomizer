#pragma once

#include <cstdint>
#include <CMaths.hh>

/* Structure correct for at least 1.0.4.0 */
class CEntity
{
public:
    void *            __vmt;
    uint32_t          field_4;
    uint32_t          field_8;
    uint32_t          field_C;
    Vector3           m_vecPos;
    uint32_t          m_fHeading;
    Matrix34 *        m_pCoords;
    uint32_t          field_24;
    uint32_t          field_28;
    uint16_t          field_2C;
    short             m_wModelIndex;
    uint32_t          m_pReference;
    uint32_t          field_34;
    struct phInstGta *m_pPhysicsInst;
    uint32_t          field_3C;
    uint8_t           field_40;
    uint8_t           field_41;
    uint16_t          field_42;
    char *            m_szModelName;
    int               m_hInterior;
    uint32_t          field_4C;
    uint32_t          field_50;
    uint32_t          field_54;
    uint8_t           field_58;
    uint8_t           field_0x59;
    uint8_t           field_0x5a;
    uint8_t           m_bBrightness;
    uint16_t          field_5C;
    uint16_t          field_5E;
    uint8_t           field_60;
    uint8_t           field_61;
    uint8_t           field_62;
    uint8_t           m_bAlpha;
    uint32_t          field_64;
    uint32_t          field_68;
    uint32_t          field_6C;
    uint32_t          field_70;

    // Not confirmed, most likely incorrect
    void* GetAnimFrag ();
    
    static void InitialisePatterns ();
    static void InitialisePatternsCE ();
};
