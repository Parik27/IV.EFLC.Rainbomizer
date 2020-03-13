#include "CScrVM.hh"
#include "CCrypto.hh"
#include <stdexcept>
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "injector/injector.hpp"


std::vector<std::pair<int, void (*) (NativeData *)>>
                 *CNativeManager::m_aNewNatives = nullptr;
std::unordered_map<int, void (*) (NativeData *)>
                 CNativeManager::m_aOriginalNatives;
NativeHashPair **CScrVM::m_aNatives;
unsigned int *   CScrVM::m_nMaximumNatives;
bool CScrVM::m_bNeedTranslation = false;

/* Because Rockstar are so kind and helpful :) */
std::unordered_map<uint32_t, uint32_t> *mHashTranslationTables;

/*******************************************************/
void
CNativeManager::OverwriteNative (const std::string &native,
                                 void (*func) (NativeData *))
{
    uint32_t hash = CCrypto::HashStringLowercase (native.c_str ());
    if(m_aNewNatives)
        {
            m_aNewNatives->push_back (std::make_pair (hash, func));
        }
}

int (*CScrVM__CreateScriptStore7ffa35) ();

/*******************************************************/
void
CNativeManager::NativeHook ()
{
    if (CScrVM::GetNativeIndex (0x8b3fed78) == -1 &&
        CScrVM::GetNativeIndex(mHashTranslationTables->at(0x8b3fed78)) != -1)
        CScrVM::m_bNeedTranslation = true;
 
    puts("Jace me, Jace me!");
    for (auto i : *m_aNewNatives)
        {
            auto& native = (*CScrVM::m_aNatives)[CScrVM::GetNativeIndexTranslated (i.first)];
            m_aOriginalNatives[i.first] = native.data;
            native.data  = i.second;
        }

    if(m_aNewNatives)
        delete m_aNewNatives;
    m_aNewNatives = nullptr;
    
    CScrVM__CreateScriptStore7ffa35 ();
}

/*******************************************************/
int
CScrVM::GetNativeIndex (uint32_t hash)
{
    for (int i = 0; i < *m_nMaximumNatives; i++)
        {
            if ((*m_aNatives)[i].hash == hash)
                return i;
        }
    return -1;
}

/*******************************************************/
int
CScrVM::GetNativeIndex (const std::string &name)
{    
    uint32_t hash = CCrypto::HashStringLowercase (name.c_str ());
    return GetNativeIndex(hash);
}

/*******************************************************/
int
CScrVM::GetNativeIndexTranslated(const std::string &name)
{
    uint32_t hash = CCrypto::HashStringLowercase (name.c_str ());
    return GetNativeIndexTranslated(hash);
}

/*******************************************************/
int
CScrVM::GetNativeIndexTranslated(uint32_t hash)
{
    if (m_bNeedTranslation)
        hash = mHashTranslationTables->at (hash);
    
    return GetNativeIndex(hash);
}

/*******************************************************/
void
CNativeManager::Initialise ()
{
    char *addr
        = hook::get_pattern<char> ("ff d6 8b 0d ? ? ? ? 8b 11 8b 42 04", 15);
    ReadCall (addr, CScrVM__CreateScriptStore7ffa35);

    if(m_aNewNatives == nullptr)
        m_aNewNatives
            = new std::vector<std::pair<int, void (*) (NativeData *)>>;

 
    injector::MakeCALL (addr, NativeHook);
}

/*******************************************************/
void
CScrVM::InitialisePatterns ()
{
    uint32_t addr = injector::GetBranchDestination (
                        hook::get_pattern ("0b c1 c1 e0 08 0b c2 8b f0", 9))
                        .as_int ();
    m_aNatives        = injector::ReadMemory<NativeHashPair **> (addr + 24);
    m_nMaximumNatives = injector::ReadMemory<unsigned int *> (addr + 3);

    mHashTranslationTables = new std::unordered_map<uint32_t, uint32_t>{
        {0x8b3fed78, 0x2F1D6843}, {0xfff1b500, 0x502B5185},
        {0xc9dbda90, 0x4BF36A32}, {0x62bfdb37, 0x4E61480A},
        {0xe1fe3fdf, 0x0FA0E33},  {0x5aa0d18e, 0x69E742FC},
        {0xece0b08, 0x7D7619D2}, {0xe26b2666, 0x6FFE0DFD},
        {0x944e9033, 0x9E405DB}, {0x5d67f751, 0x2A171915},
        {0x3f166d0e, 0x4E2260B9}};
}

/*******************************************************/
void
CNativeManager::CallOriginalNative(const std::string &name, NativeData* data)
{
    uint32_t hash = CCrypto::HashStringLowercase (name.c_str ());
    m_aOriginalNatives[hash](data);
}

/*******************************************************/
void
CNativeManager::CallNativeRaw(const std::string &name, NativeData* data)
{
    uint32_t hash = CCrypto::HashStringLowercase (name.c_str ());
    auto& native = (*CScrVM::m_aNatives)[CScrVM::GetNativeIndexTranslated (hash)];

    native.data(data);
}
