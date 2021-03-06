#include "CModelInfoStore.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include <CCrypto.hh>

int (*CModelInfoStore__GetMaximumNumberOfPassengers) (int model);
int *CModelInfoStore__nTotalModelPointers;

/*******************************************************/
uint32_t
CModelInfoStore::FindVehicleModelWithGameName (uint32_t hash)
{
    for (uint32_t i = 0; i < m_nTotalModelPointers (); i++)
        if (m_aModelPointers[i]->GetType () == MODEL_INFO_VEHICLE)
            if (CCrypto::atStringHash (
                    GetModelInfoPointer<CVehicleModelInfo> (i)->m_szGameName)
                == hash)
                return i;
    return -1;
}

/*******************************************************/
uint32_t
CModelInfoStore::FindVehicleModelWithGameName (const char *name)
{
    return FindVehicleModelWithGameName (CCrypto::atStringHash (name));
}

/*******************************************************/
void
CModelInfoStore::InitialisePatternsCE ()
{
    CModelInfoStore__nTotalModelPointers = *hook::get_pattern<int *> (
        "89 30 8b 0d ? ? ? ? 89 48 04 ff 05 ? ? ? ? 8b c7 ", 4);

    m_aModelPointers = *hook::get_pattern<CBaseModelInfo **> (
        "83 c4 04 89 3c ? ? ? ? ? 8b 77 3c 6a 10", 6);

    // e8 ? ? ? ? ff 35 ? ? ? ? 8d ? ? 28 e8 ? ? ? ?
    CModelLookup__superlod = reinterpret_cast<CModelLookup *> (
        *hook::get_pattern<char *> (
            "e8 ? ? ? ? ff 35 ? ? ? ? 8d ? ? 28 e8 ? ? ? ?", 7)
        - 4);

    ConvertCall (hook::get_pattern (
                     "8b 44 ? ? 33 c9 8b 04 ? ? ? ? ? ba 01 00 00 00 "),
                 CModelInfoStore__GetMaximumNumberOfPassengers);
}

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

int &
CModelInfoStore::m_nTotalModelPointers ()
{
    return *CModelInfoStore__nTotalModelPointers;
}

CBaseModelInfo **CModelInfoStore::m_aModelPointers = nullptr;
CModelLookup *   CModelInfoStore::CModelLookup__superlod;
