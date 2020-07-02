#include "Utils.hh"
#include "CScriptText.hh"
#include <injector/injector.hpp>

/*******************************************************/
void
CScriptText::InitialisePatterns ()
{
    m_pTotalScriptTexts = *hook::get_pattern<uint32_t*> (
        "39 2d ? ? ? ? 0f 86 ? ? ? ? 56 57 be ? ? ? ?", 2);

    m_aScriptTexts = reinterpret_cast<CScriptText_Str *> (
        *hook::get_pattern<uint8_t*> (
            "39 2d ? ? ? ? 0f 86 ? ? ? ? 56 57 be ? ? ? ?", 15)
        - 58);

    m_pScript2dViewport = *hook::get_pattern<void **> ("6b c0 7c 8b 0d", 5);
}

/*******************************************************/
void
CScriptText::InitialisePatternsCE ()
{
    m_pTotalScriptTexts = *hook::get_pattern<uint32_t*> (
        "8b 0d ? ? ? ? 33 ff 85 c9 0f 84 ? ? ? ? be ? ? ? ?", 2);
    
    m_aScriptTexts = reinterpret_cast<CScriptText_Str *> (
        *hook::get_pattern<uint8_t*> (
            "39 2d ? ? ? ? 0f 86 ? ? ? ? 56 57 be ? ? ? ?", 15)
        - 58);

    m_pScript2dViewport = *hook::get_pattern<void **> (
        "69 c9 7c a1", 4);
}

// CScriptText_Str **CScriptText::m_apScriptTexts;
CScriptText_Str *CScriptText::m_aScriptTexts;
uint32_t *       CScriptText::m_pTotalScriptTexts;
void**       CScriptText::m_pScript2dViewport;
