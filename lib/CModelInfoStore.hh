#pragma once

#include "CModelInfo.hh"

struct CModelLookup
{
    char *        m_szName;
    int           m_nIndex;
    CModelLookup *m_pNext;
};

class CModelInfoStore
{
public:
    static CModelLookup *CModelLookup__superlod;

    static CBaseModelInfo **m_aModelPointers;

    static int &m_nTotalModelPointers ();

    static int GetMaximumNumberOfPassengers (int index);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();

    static uint32_t FindVehicleModelWithGameName (uint32_t hash);
    static uint32_t FindVehicleModelWithGameName (const char *name);

    template <typename T>
    static T *
    GetModelInfoPointer (int index)
    {
        return (T *) m_aModelPointers[index];
    }
};
