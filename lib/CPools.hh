#pragma once

template <typename T> class CPool
{
public:
    T *   m_pObjects;
    bool *m_pMask;
    int   m_nMaxElements;
    int   m_nSize;
    int   m_nFirstFree;
    int   m_nCount;
    bool  unk_24;
};

struct CVehicleStruct;

class CPools
{
public:
    static CPool<CVehicleStruct>*& g_pVehicleStructPool();

    static void InitialisePatterns();
};
