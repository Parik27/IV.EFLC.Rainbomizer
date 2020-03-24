#pragma once

struct CGxtEntry
{
    unsigned int m_nOffset;
    unsigned int m_nHash;
};

template <typename T> struct CText_Data
{
    T *          m_aData;
    unsigned int m_nSize;
};

struct CText__data2
{
    unsigned int m_nHash;
    unsigned int field_4;
    unsigned int field_8;
    int          field_C;
};

struct CText__string
{
    char m_szName[8];
};

class CText
{
public:
    /* This below shows the structure of g_Text for GTAIV.exe:1040.
       It changed in later versions to have more than 8 additional text
       banks available. If you want to access these variables, use the
       getter/setters

       CText_Data<CGxtEntry>    m_aTextDataKeys;
       CText_Data<char>         m_aTextDataStrings;
       CText_Data<CGxtEntry>    m_aAdditionalTextDataKeys[8];
       CText_Data<char>         m_aAdditionalTextDataStrings[8];
       bool                     m_abAdditionalTextLoaded[8];
       bool                     m_abStreamingAdditionalText[8];
       CText__data2             field_A0[8];
       char                     m_szAdditionalTextNames[8][8];
       int                      m_nKeyGxtTable;
       unsigned char            m_cTextType;
       bool                     m_bAdditionalTextLoaded;
       unsigned char            field_166;
       unsigned char            field_167;
       CText_Data<CText__data2> m_paGxtTables;

    */

    static unsigned char m_nTotalAdditionalTextSlots;

#ifndef NO_WSTR_DETECT_HOOK
    static int __fastcall EncodingDetectHook (void *thisThingy, void *edx,
                                              short *out, int size);

    static int m_nEncoding;
#endif

    CText_Data<CGxtEntry> &   m_aTextDataKeys ();
    CText_Data<char> &        m_aTextDataStrings ();
    CText_Data<CGxtEntry> *   m_aAdditionalTextDataKeys ();
    CText_Data<char> *        m_aAdditionalTextDataStrings ();
    bool *                    m_abAdditionalTextLoaded ();
    bool *                    m_abStreamingAdditionalText ();
    CText__data2 *            field_A0 ();
    CText__string *           m_szAdditionalTextNames ();
    int &                     m_nKeyGxtTable ();
    unsigned char &           m_cTextType ();
    bool &                    m_bAdditionalTextLoaded ();
    unsigned char &           field_166 ();
    unsigned char &           field_167 ();
    CText_Data<CText__data2> &m_paGxtTables ();

    static CText *g_Text;

    void LoadAdditionalText (const char *gxt, int slot);

    static void InitialisePatterns ();
};
