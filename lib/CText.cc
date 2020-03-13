#include "CText.hh"
#include "Utils.hh"

void (__thiscall *CText__LoadAdditionalText) (CText *, const char *, int);
int (__thiscall *fiFile__readData7e9166) (void *, short *, int);


/*******************************************************/
void
CText::LoadAdditionalText (const char *gxt, int slot)
{
    CText__LoadAdditionalText (this, gxt, slot);
}

#ifndef NO_WSTR_DETECT_HOOK
/*******************************************************/
int __fastcall CText::EncodingDetectHook (void *thisThingy, void *edx,
                                          short *out, int size)
{
    int ret = fiFile__readData7e9166(thisThingy, out, size);

    m_nEncoding = *out;
    return ret;
}
#endif

/*******************************************************/
void
CText::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("85 db 56 57 8b bc", -28),
                 CText__LoadAdditionalText);

    g_Text = *hook::get_pattern<CText *> (
        "5e 6a 01 b9 ? ? ? ? e8 ? ? ? ? b0 01 c3 ", 4);

    m_nTotalAdditionalTextSlots = *hook::get_pattern<uint8_t> (
        "83 ff ? 7d ? 80 bc ? ? ? ? ? 00 ", 2);

#ifndef NO_WSTR_DETECT_HOOK
    RegisterHook ("8d ? ? ? ? 8b ce e8 ? ? ? ? bf 04 00 00 00", 7,
                  fiFile__readData7e9166, EncodingDetectHook);
#endif
}

#define GTEXT_MEMBER(type, name, offset)                                       \
    type &CText::name ()                                                       \
    {                                                                          \
        return *((type *) &reinterpret_cast<char *> (this)[offset]);           \
    }
#define GTEXT_MEMBER_ARR(type, name, offset)                                   \
    type CText::name ()                                                       \
    {                                                                          \
        return ((type) & reinterpret_cast<char *> (this)[offset]);             \
    }

#define N this->m_nTotalAdditionalTextSlots

GTEXT_MEMBER (CText_Data<CGxtEntry>, m_aTextDataKeys, 0);
GTEXT_MEMBER (CText_Data<char>, m_aTextDataStrings, 8);
GTEXT_MEMBER_ARR (CText_Data<CGxtEntry> *, m_aAdditionalTextDataKeys, 16);
GTEXT_MEMBER_ARR (CText_Data<char> *, m_aAdditionalTextDataStrings, 16 + 8 * N);
GTEXT_MEMBER_ARR (bool *, m_abAdditionalTextLoaded, 16 + 16 * N);
GTEXT_MEMBER_ARR (bool *, m_abStreamingAdditionalText, 16 + 16 * N + N);
GTEXT_MEMBER_ARR (CText__data2 *, field_A0, 16 + 16 * N + 2 * N);
GTEXT_MEMBER_ARR (CText__string *, m_szAdditionalTextNames,
                  16 + 16 * N + 2 * N + 16 * N);
GTEXT_MEMBER (int, m_nKeyGxtTable, 16 + 16 * N + 2 * N + 16 * N + 64 * N);
GTEXT_MEMBER (unsigned char, m_cTextType,
              20 + 16 * N + 2 * N + 16 * N + 64 * N);
GTEXT_MEMBER (bool, m_bAdditionalTextLoaded,
              21 + 16 * N + 2 * N + 16 * N + 64 * N);
GTEXT_MEMBER (unsigned char, field_166, 22 + 16 * N + 2 * N + 16 * N + 64 * N);
GTEXT_MEMBER (unsigned char, field_167, 23 + 16 * N + 2 * N + 16 * N + 64 * N);
GTEXT_MEMBER (CText_Data<CText__data2>, m_paGxtTables,
              24 + 16 * N + 2 * N + 16 * N + 64 * N);

#undef N
#undef GTEXT_MEMBER

CText *CText::g_Text = nullptr;
unsigned char CText::m_nTotalAdditionalTextSlots = 8;
int CText::m_nEncoding = 8;
