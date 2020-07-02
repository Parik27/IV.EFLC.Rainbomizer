#pragma once

#include <CMaths.hh>
#include <CARGB.hh>
#include <cstdint>
#include <cstring>

struct CScriptText_Str
{
    uint32_t m_nWidescreenFormat;
    Vector2  m_vecScale;
    void *   m_pViewport;
    uint32_t field_0x10;
    CARGB    m_nTextColour;
    bool     m_bAlignLeft;
    bool     m_bAlignRight;
    bool     m_bAlignCentre;
    bool     m_bMonospaced;
    bool     field_0x1c;
    bool     m_bHasBackground;
    uint8_t  field_0x1e;
    uint8_t  field_0x1f;
    Vector2  field_0x20;
    float    m_fLineHeight;
    uint32_t field_0x2c;
    CARGB    m_nBackgroundColour;
    CARGB    m_nStrokeColour;
    uint8_t  field_0x38;
    uint8_t  field_0x39;
    bool     m_bDrawBeforeFade;
    uint8_t  field_0x3b;
    uint8_t  m_ubFont;
    uint8_t  field_0x3d;
    uint8_t  field_0x3e;
    uint8_t  field_0x3f;
    Vector2  m_vecPos;
    char     m_szGxtEntry[16];
    uint32_t field_0x58;
    uint32_t field_0x5c;
    uint32_t m_aNumbers[3];
    float    m_fFloat;
    float    m_fFloatPrecision;
    char *   m_paLiteralStrings[2];
};

static_assert (sizeof (CScriptText_Str) == 124, "Invalid CScriptText_Str size");

/*******************************************************/
/* Internal representation of text created with DISPLAY_TEXT and stuff */
/* Note: You can edit certain things you can't with natives (like the colours,
 * scale, etc.).  */
/*******************************************************/
class CScriptText
{
    // static CScriptText_Str **m_apScriptTexts;
    static uint32_t *m_pTotalScriptTexts;

public:
    static CScriptText_Str *m_aScriptTexts;
    static void **          m_pScript2dViewport;

    inline static uint32_t &
    m_nTotalScriptTexts ()
    {
        return *m_pTotalScriptTexts;
    }

    inline static CScriptText_Str *
    NewText (const char *gxt, void *viewportPtr = nullptr)
    {
        if (!viewportPtr)
            viewportPtr = *m_pScript2dViewport;

        strcpy (m_aScriptTexts[m_nTotalScriptTexts ()].m_szGxtEntry, gxt);
        m_aScriptTexts[m_nTotalScriptTexts ()].m_pViewport = viewportPtr;
        return &m_aScriptTexts[m_nTotalScriptTexts ()++];
    }

    static void InitialisePatterns ();
    static void InitialisePatternsCE ();
};
