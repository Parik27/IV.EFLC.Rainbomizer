#include "CTheScripts.hh"
#include "CCrypto.hh"
#include <stdexcept>
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "injector/injector.hpp"
#include "NativeTranslationTable.hh"

int **                     CTheScripts__m_pGlobals;
unsigned int *             CTheScripts__m_nMaximumScriptPrograms;
unsigned int *             CTheScripts__m_nNumScriptPrograms;
atHashPair<scrProgram *> **CTheScripts__m_aScriptPrograms;

std::vector<std::pair<int, void (*) (NativeData *)>>
    *CNativeManager::m_aNewNatives = nullptr;
std::unordered_map<int, void (*) (NativeData *)>
                 CNativeManager::m_aOriginalNatives;
NativeHashPair **CTheScripts::m_aNatives;
unsigned int *   CTheScripts::m_nMaximumNatives;
bool             CTheScripts::m_bNeedTranslation = false;
GtaThread **     CTheScripts__m_pRunningThread   = nullptr;

/*******************************************************/
int *&
CTheScripts::m_pGlobals ()
{
    return *CTheScripts__m_pGlobals;
}

/*******************************************************/
unsigned int &
CTheScripts::m_nNumScriptPrograms ()
{
    return *CTheScripts__m_nNumScriptPrograms;
}

/*******************************************************/
unsigned int &
CTheScripts::m_nMaxScriptPrograms ()
{
    return *CTheScripts__m_nMaximumScriptPrograms;
}

/*******************************************************/
atHashPair<scrProgram *> *&
CTheScripts::m_aScriptPrograms ()
{
    return *CTheScripts__m_aScriptPrograms;
}

/*******************************************************/
void
CNativeManager::OverwriteNative (const std::string &native,
                                 void (*func) (NativeData *))
{
    uint32_t hash = CCrypto::atStringHash (native.c_str ());
    if (m_aNewNatives)
        {
            m_aNewNatives->push_back (std::make_pair (hash, func));
        }
}

int (*CTheScripts__CreateScriptStore7ffa35) ();

/*******************************************************/
void
CNativeManager::NativeHook ()
{
    if (CTheScripts::GetNativeIndex (0x8b3fed78) == -1
        && CTheScripts::GetNativeIndex (
               GetNativeTranslationTable ().at (0x8b3fed78))
               != -1)
        CTheScripts::m_bNeedTranslation = true;

    for (auto i : *m_aNewNatives)
        {
            auto &native = (*CTheScripts::m_aNatives)
                [CTheScripts::GetNativeIndexTranslated (i.first)];
            m_aOriginalNatives[i.first] = native.data;
            native.data                 = i.second;
        }

    if (m_aNewNatives)
        delete m_aNewNatives;
    m_aNewNatives = nullptr;

    CTheScripts__CreateScriptStore7ffa35 ();
}

/*******************************************************/
int
CTheScripts::GetNativeIndex (uint32_t hash)
{
    for (int i = 0; i < *m_nMaximumNatives; i++)
        {
            if ((*m_aNatives)[i].hash == hash)
                return i;
        }
    return -1;
}

/*******************************************************/
scrProgram *
CTheScripts::GetScrProgram (uint32_t hash)
{
    for (int i = 0; i < m_nMaxScriptPrograms (); i++)
        {
            if (m_aScriptPrograms ()[i].hash == hash)
                return m_aScriptPrograms ()[i].data;
        }
    return nullptr;
}

/*******************************************************/
scrProgram *
CTheScripts::GetScrProgram (const std::string &name)
{
    uint32_t hash = CCrypto::atStringHash (name.c_str ());
    return GetScrProgram (hash);
}

/*******************************************************/
int
CTheScripts::GetNativeIndex (const std::string &name)
{
    uint32_t hash = CCrypto::atStringHash (name.c_str ());
    return GetNativeIndex (hash);
}

/*******************************************************/
int
CTheScripts::GetNativeIndexTranslated (const std::string &name)
{
    uint32_t hash = CCrypto::atStringHash (name.c_str ());
    return GetNativeIndexTranslated (hash);
}

/*******************************************************/
int
CTheScripts::GetNativeIndexTranslated (uint32_t hash)
{
    if (m_bNeedTranslation)
        hash = GetNativeTranslationTable ().at (hash);

    return GetNativeIndex (hash);
}

/*******************************************************/
void
CNativeManager::InitialiseCE ()
{
    char *addr
        = hook::get_pattern<char> ("8b 0d ? ? ? ? 8b 01 ff 50 04 e8 ? ? ? ? 68",
                                   11);
    ReadCall (addr, CTheScripts__CreateScriptStore7ffa35);

    if (m_aNewNatives == nullptr)
        m_aNewNatives
            = new std::vector<std::pair<int, void (*) (NativeData *)>>;

    injector::MakeCALL (addr, NativeHook);
}

/*******************************************************/
void
CNativeManager::Initialise ()
{
    char *addr
        = hook::get_pattern<char> ("ff d6 8b 0d ? ? ? ? 8b 11 8b 42 04", 15);
    ReadCall (addr, CTheScripts__CreateScriptStore7ffa35);

    if (m_aNewNatives == nullptr)
        m_aNewNatives
            = new std::vector<std::pair<int, void (*) (NativeData *)>>;

    injector::MakeCALL (addr, NativeHook);
}

/*******************************************************/
void
InitialiseScriptProgramPatternsCE ()
{
    CTheScripts__m_aScriptPrograms
        = *hook::get_pattern<atHashPair<scrProgram *> **> (
            "a1 ? ? ? ? 5f c7 44 d0 04 00 00 00 0", 1);

    CTheScripts__m_nNumScriptPrograms = *hook::get_pattern<unsigned int *> (
        "a1 ? ? ? ? 5f c7 44 d0 04 00 00 00 0", 16);

    CTheScripts__m_nMaximumScriptPrograms = *hook::get_pattern<unsigned int *> (
        "53 8b 1d ? ? ? ? 56 8b 74 ? ? 33 d2  ", 3);
}

/*******************************************************/
void
InitialiseScriptProgramPatterns ()
{
    CTheScripts__m_aScriptPrograms
        = *hook::get_pattern<atHashPair<scrProgram *> **> (
            "a1 ? ? ? ? 5f c7 44 d0 04 00 00 00 0", 1);

    CTheScripts__m_nNumScriptPrograms = *hook::get_pattern<unsigned int *> (
        "a1 ? ? ? ? 5f c7 44 d0 04 00 00 00 0", 16);

    CTheScripts__m_nMaximumScriptPrograms = *hook::get_pattern<unsigned int *> (
        "8b 5c ? ? 56 57 8b 3d ? ? ? ? 33 d2 8b c3 ", 8);
}

/*******************************************************/
void
CTheScripts::InitialisePatternsCE ()
{
    uint32_t addr = injector::GetBranchDestination (
                        hook::get_pattern ("c1 e7 08 0b f8 57 e8", 6))
                        .as_int ();
    m_aNatives        = injector::ReadMemory<NativeHashPair **> (addr + 31);
    m_nMaximumNatives = injector::ReadMemory<unsigned int *> (addr + 3);

    InitialiseScriptProgramPatternsCE ();

    // 8b 48 08 8b 15 ?? ?? ?? ?? a1 ?? ?? ?? ??
    // 8b 57 18 8b 40 08 89 44 ? ? a1 ? ? ? ?

    CTheScripts__m_pGlobals
        = *hook::get_pattern<int **> ("8b 57 18 8b 40 08 89 44 ? ? a1", 11);

    CTheScripts__m_pRunningThread
        = *hook::get_pattern<GtaThread **> ("83 f8 03 0f 84 ? ? ? ? 8b 35", 11);
}

/*******************************************************/
void
CTheScripts::InitialisePatterns ()
{
    uint32_t addr = injector::GetBranchDestination (
                        hook::get_pattern ("0b c1 c1 e0 08 0b c2 8b f0", 9))
                        .as_int ();
    m_aNatives        = injector::ReadMemory<NativeHashPair **> (addr + 24);
    m_nMaximumNatives = injector::ReadMemory<unsigned int *> (addr + 3);

    InitialiseScriptProgramPatterns ();

    // 8b 48 08 8b 15 ?? ?? ?? ?? a1 ?? ?? ?? ??
    CTheScripts__m_pGlobals
        = *hook::get_pattern<int **> ("8b 48 08 8b 15 ? ? ? ? a1 ? ? ? ?", 5);

    CTheScripts__m_pRunningThread
        = *hook::get_pattern<GtaThread **> ("83 f8 03 0f 84 ? ? ? ? a1", 10);
}

/*******************************************************/
GtaThread *&
CTheScripts::m_pRunningThread ()
{
    return *CTheScripts__m_pRunningThread;
}

/*******************************************************/
void
CNativeManager::CallOriginalNative (const std::string &name, NativeData *data)
{
    uint32_t hash = CCrypto::atStringHash (name.c_str ());
    m_aOriginalNatives[hash](data);
}

/*******************************************************/
void
CNativeManager::CallNativeRaw (const std::string &name, NativeData *data)
{
    uint32_t hash = CCrypto::atStringHash (name.c_str ());
    auto &   native
        = (*CTheScripts::m_aNatives)[CTheScripts::GetNativeIndexTranslated (
            hash)];

    native.data (data);
}
