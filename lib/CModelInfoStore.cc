#include "CModelInfoStore.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"

int (*CModelInfoStore__GetMaximumNumberOfPassengers) (int model);
int *CModelInfoStore__nTotalModelPointers;

/*******************************************************/
void
CModelInfoStore::InitialisePatterns ()
{
    // 89 50 04 83 05 ?? ?? ?? ?? 01 5f
    CModelInfoStore__nTotalModelPointers
        = *hook::get_pattern<int *> ("89 50 04 83 05 ? ? ? ? 01 5f", 5);

    m_aModelPointers = *hook::get_pattern<CBaseModelInfo **> (
        "83 c4 04 89 34 ? ? ? ? ? 8b 7e 3c", 6);

    CModelLookup__superlod = reinterpret_cast<CModelLookup *> (
        *hook::get_pattern<char *> (
            "52 8d 8c ? ? ? ? ? e8 ? ? ? ? a1 ? ? ? ? 50 8d 8c", 14)
        - 4);

    ConvertCall (hook::get_pattern (
                     "8b 4c ? ? 8b 0c ? ? ? ? ? 8b 89 cc 00 00 00 83 ca ff"),
                 CModelInfoStore__GetMaximumNumberOfPassengers);
}

/*******************************************************/
int
CModelInfoStore::GetMaximumNumberOfPassengers (int index)
{
    return CModelInfoStore__GetMaximumNumberOfPassengers (index);
}

int &CModelInfoStore::m_nTotalModelPointers()
{
    return *CModelInfoStore__nTotalModelPointers;
}

CBaseModelInfo **CModelInfoStore::m_aModelPointers = nullptr;
CModelLookup *   CModelInfoStore::CModelLookup__superlod;
