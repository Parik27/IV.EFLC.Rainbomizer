#pragma once
#include "CMaths.hh"

typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;

typedef long long    longlong;
typedef unsigned long long    qword;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef struct CWeaponModelInfo CWeaponModelInfo, *PCWeaponModelInfo;

typedef struct CBaseModelInfo CBaseModelInfo, *PCBaseModelInfo;

typedef struct CVirtualBase CVirtualBase, *PCVirtualBase;

typedef struct Vector3 Vector3, *PVector3;

struct CVirtualBase {
    dword __vmt;
};

struct CBaseModelInfo { /* size = 0x60 */
    struct CVirtualBase __parent;
    dword field_4;
    dword field_8;
    dword field_C;
    dword field_10; /* float */
    dword field_14; /* float */
    dword field_18; /* float */
    dword field_1C;
    struct Vector3 field_20;
    dword m_fDrawDistance;
    struct Vector3 field_30;
    dword field_3C;
    dword m_nBaseFlags;
    dword m_dwRefCount;
    short m_wTxd;
    word field_4A;
    word field_4C;
    word field_4E;
    word field_50;
    word field_52;
    word field_54;
    word m_nAnims;
    word m_wBlendshapeId;
    byte field_5A;
    byte field_5B;
    dword field_5C;
};

struct CWeaponModelInfo {
    struct CBaseModelInfo __parent;
    dword field_60;
    dword field_64;
    dword field_68;
    dword field_6C;
    dword field_70;
    dword field_74;
    dword field_78;
    dword field_7C;
};

