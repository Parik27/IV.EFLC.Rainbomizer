#pragma once
#include "CMaths.hh"
#include <cstdint>

typedef enum eVehicleType
{
    VEHICLE_TYPE_BIKE  = 1,
    VEHICLE_TYPE_BOAT  = 2,
    VEHICLE_TYPE_CAR   = 0,
    VEHICLE_TYPE_HELI  = 4,
    VEHICLE_TYPE_PLANE = 5,
    VEHICLE_TYPE_TRAIN = 3
} eVehicleType;

enum eModelInfoType : unsigned char
{
    MODEL_INFO_BASE    = 1,
    MODEL_INFO_MLO     = 2,
    MODEL_INFO_PED     = 6,
    MODEL_INFO_TIME    = 3,
    MODEL_INFO_VEHICLE = 5,
    MODEL_INFO_WEAPON  = 4
};

struct CBaseModelInfo;

struct CBaseModelInfo__vftable
{
    void *deconstructor;
    void *Initialise;
    void *m8;
    eModelInfoType (__thiscall *GetType) (CBaseModelInfo *);
    void *m10;
    void *m14;
    void *m18;
    void *m1c;
    void *m20;
    void *m24;
    void *m28;
    void (__thiscall *Release) (CBaseModelInfo *);
    void *m30;
    void *m34;
    void *m38;
};

struct CVehicleModelInfo_obj2
{
    uint32_t field_0;
    uint32_t field_4;
    uint32_t field_8;
};

struct CVirtualBase
{
    CBaseModelInfo__vftable *__vmt;
};

struct CBaseModelInfo : CVirtualBase
{ /* size = 0x60 */
    uint32_t field_4;
    uint32_t field_8;
    uint32_t field_C;
    uint32_t field_10; /* float */
    uint32_t field_14; /* float */
    uint32_t field_18; /* float */
    uint32_t field_1C;
    Vector3  m_vecMinBounds;
    uint32_t m_fDrawDistance;
    Vector3  m_vecMaxBounds;
    uint32_t m_nModelHash;
    uint32_t m_nBaseFlags;
    uint32_t m_dwRefCount;
    short    m_wTxd;
    uint16_t field_4A;
    uint16_t field_4C;
    uint16_t field_4E;
    uint16_t field_50;
    uint16_t field_52;
    uint16_t field_54;
    uint16_t m_nAnims;
    uint16_t m_wBlendshapeId;
    uint8_t  field_5A;
    uint8_t  field_5B;
    uint32_t field_5C;

    eModelInfoType GetType ();
    void           Release ();
};

struct CVehicleModelInfo_obj1
{
    Vector3  field_0;
    uint32_t field_C;
    uint8_t  field_10;
    uint8_t  field_11;
    uint8_t  field_0x12;
    uint8_t  field_0x13;
    uint32_t field_14;
    uint32_t field_18;
    uint32_t field_1C;
    uint32_t field_20;
    uint32_t field_24;
    uint32_t field_28;
    uint32_t field_2C;
};

struct CVehicleModelInfo : CBaseModelInfo
{
    char                   m_szGameName[9];
    uint8_t                field_0x69;
    uint8_t                field_0x6a;
    uint8_t                field_0x6b;
    eVehicleType           m_nType;
    eVehicleType           m_nClassId;
    float                  m_fWheelRadiusA2;
    float                  m_fWheelRadiusB1; /* Created by retype action */
    float                  m_fWheelRadiusA1;
    float                  m_fWheelRadiusB2; /* Created by retype action */
    uint32_t               m_nLodMult;
    uint32_t               m_fSwankness; /* float */
    uint32_t               m_dwHandlingIndex;
    uint8_t                field_0x90;
    uint8_t                field_0x91;
    uint8_t                field_0x92;
    uint8_t                field_0x93;
    uint32_t               flags;
    int                    field_0x98;
    float                  m_fFrq;
    int                    m_nMaxNum; /* Created by retype action */
    int                    m_ndefDirtLevel;
    uint32_t               m_nLastRemoved;
    uint32_t               m_nPedGroup;
    uint32_t               m_nSpawnCount;
    uint32_t               field_B4; /* float */
    uint32_t               field_B8; /* float */
    int                    unkfloat_related;
    short                  m_nAnim2;
    uint8_t                field_0xc2;
    uint8_t                field_0xc3;
    uint32_t               field_C4;
    uint32_t               field_C8;
    uint32_t               field_CC;
    uint8_t                m_aColours[4][25];
    uint32_t               m_nTotalColours;
    uint32_t               m_nCurrentColourId;
    uint32_t               field_13C;
    uint32_t               field_140;
    uint32_t               field_144;
    uint32_t               field_148;
    uint8_t                field_0x14c;
    uint8_t                field_0x14d;
    uint8_t                field_0x14e;
    uint8_t                field_0x14f;
    CVehicleModelInfo_obj1 field_150;
    uint8_t                field_0x180;
    uint8_t                field_0x181;
    uint8_t                field_0x182;
    uint8_t                field_0x183;
    uint8_t                field_0x184;
    uint8_t                field_0x185;
    uint8_t                field_0x186;
    uint8_t                field_0x187;
    uint8_t                field_0x188;
    uint8_t                field_0x189;
    uint8_t                field_0x18a;
    uint8_t                field_0x18b;
    uint8_t                field_0x18c;
    uint8_t                field_0x18d;
    uint8_t                field_0x18e;
    uint8_t                field_0x18f;
    uint8_t                field_0x190;
    uint8_t                field_0x191;
    uint8_t                field_0x192;
    uint8_t                field_0x193;
    uint8_t                field_0x194;
    uint8_t                field_0x195;
    uint8_t                field_0x196;
    uint8_t                field_0x197;
    uint8_t                field_0x198;
    uint8_t                field_0x199;
    uint8_t                field_0x19a;
    uint8_t                field_0x19b;
    uint8_t                field_0x19c;
    uint8_t                field_0x19d;
    uint8_t                field_0x19e;
    uint8_t                field_0x19f;
    uint8_t                field_0x1a0;
    uint8_t                field_0x1a1;
    uint8_t                field_0x1a2;
    uint8_t                field_0x1a3;
    uint8_t                field_0x1a4;
    uint8_t                field_0x1a5;
    uint8_t                field_0x1a6;
    uint8_t                field_0x1a7;
    uint8_t                field_0x1a8;
    uint8_t                field_0x1a9;
    uint8_t                field_0x1aa;
    uint8_t                field_0x1ab;
    uint8_t                field_0x1ac;
    uint8_t                field_0x1ad;
    uint8_t                field_0x1ae;
    uint8_t                field_0x1af;
    uint8_t                field_0x1b0;
    uint8_t                field_0x1b1;
    uint8_t                field_0x1b2;
    uint8_t                field_0x1b3;
    uint8_t                field_0x1b4;
    uint8_t                field_0x1b5;
    uint8_t                field_0x1b6;
    uint8_t                field_0x1b7;
    uint8_t                field_0x1b8;
    uint8_t                field_0x1b9;
    uint8_t                field_0x1ba;
    uint8_t                field_0x1bb;
    uint8_t                field_0x1bc;
    uint8_t                field_0x1bd;
    uint8_t                field_0x1be;
    uint8_t                field_0x1bf;
    uint8_t                field_0x1c0;
    uint8_t                field_0x1c1;
    uint8_t                field_0x1c2;
    uint8_t                field_0x1c3;
    uint8_t                field_0x1c4;
    uint8_t                field_0x1c5;
    uint8_t                field_0x1c6;
    uint8_t                field_0x1c7;
    uint8_t                field_0x1c8;
    uint8_t                field_0x1c9;
    uint8_t                field_0x1ca;
    uint8_t                field_0x1cb;
    uint8_t                field_0x1cc;
    uint8_t                field_0x1cd;
    uint8_t                field_0x1ce;
    uint8_t                field_0x1cf;
    uint8_t                field_0x1d0;
    uint8_t                field_0x1d1;
    uint8_t                field_0x1d2;
    uint8_t                field_0x1d3;
    uint8_t                field_0x1d4;
    uint8_t                field_0x1d5;
    uint8_t                field_0x1d6;
    uint8_t                field_0x1d7;
    uint8_t                field_0x1d8;
    uint8_t                field_0x1d9;
    uint8_t                field_0x1da;
    uint8_t                field_0x1db;
    uint8_t                field_0x1dc;
    uint8_t                field_0x1dd;
    uint8_t                field_0x1de;
    uint8_t                field_0x1df;
    uint8_t                field_0x1e0;
    uint8_t                field_0x1e1;
    uint8_t                field_0x1e2;
    uint8_t                field_0x1e3;
    uint8_t                field_0x1e4;
    uint8_t                field_0x1e5;
    uint8_t                field_0x1e6;
    uint8_t                field_0x1e7;
    uint8_t                field_0x1e8;
    uint8_t                field_0x1e9;
    uint8_t                field_0x1ea;
    uint8_t                field_0x1eb;
    uint8_t                field_0x1ec;
    uint8_t                field_0x1ed;
    uint8_t                field_0x1ee;
    uint8_t                field_0x1ef;
    uint8_t                field_0x1f0;
    uint8_t                field_0x1f1;
    uint8_t                field_0x1f2;
    uint8_t                field_0x1f3;
    uint8_t                field_0x1f4;
    uint8_t                field_0x1f5;
    uint8_t                field_0x1f6;
    uint8_t                field_0x1f7;
    uint8_t                field_0x1f8;
    uint8_t                field_0x1f9;
    uint8_t                field_0x1fa;
    uint8_t                field_0x1fb;
    uint8_t                field_0x1fc;
    uint8_t                field_0x1fd;
    uint8_t                field_0x1fe;
    uint8_t                field_0x1ff;
    uint8_t                field_0x200;
    uint8_t                field_0x201;
    uint8_t                field_0x202;
    uint8_t                field_0x203;
    uint8_t                field_0x204;
    uint8_t                field_0x205;
    uint8_t                field_0x206;
    uint8_t                field_0x207;
    uint8_t                field_0x208;
    uint8_t                field_0x209;
    uint8_t                field_0x20a;
    uint8_t                field_0x20b;
    uint8_t                field_0x20c;
    uint8_t                field_0x20d;
    uint8_t                field_0x20e;
    uint8_t                field_0x20f;
    uint8_t                field_0x210;
    uint8_t                field_0x211;
    uint8_t                field_0x212;
    uint8_t                field_0x213;
    uint8_t                field_0x214;
    uint8_t                field_0x215;
    uint8_t                field_0x216;
    uint8_t                field_0x217;
    uint8_t                field_0x218;
    uint8_t                field_0x219;
    uint8_t                field_0x21a;
    uint8_t                field_0x21b;
    uint8_t                field_0x21c;
    uint8_t                field_0x21d;
    uint8_t                field_0x21e;
    uint8_t                field_0x21f;
    uint8_t                field_0x220;
    uint8_t                field_0x221;
    uint8_t                field_0x222;
    uint8_t                field_0x223;
    uint8_t                field_0x224;
    uint8_t                field_0x225;
    uint8_t                field_0x226;
    uint8_t                field_0x227;
    uint8_t                field_0x228;
    uint8_t                field_0x229;
    uint8_t                field_0x22a;
    uint8_t                field_0x22b;
    uint8_t                field_0x22c;
    uint8_t                field_0x22d;
    uint8_t                field_0x22e;
    uint8_t                field_0x22f;
    uint8_t                field_0x230;
    uint8_t                field_0x231;
    uint8_t                field_0x232;
    uint8_t                field_0x233;
    uint8_t                field_0x234;
    uint8_t                field_0x235;
    uint8_t                field_0x236;
    uint8_t                field_0x237;
    uint8_t                field_0x238;
    uint8_t                field_0x239;
    uint8_t                field_0x23a;
    uint8_t                field_0x23b;
    uint8_t                field_0x23c;
    uint8_t                field_0x23d;
    uint8_t                field_0x23e;
    uint8_t                field_0x23f;
    uint8_t                field_0x240;
    uint8_t                field_0x241;
    uint8_t                field_0x242;
    uint8_t                field_0x243;
    uint8_t                field_0x244;
    uint8_t                field_0x245;
    uint8_t                field_0x246;
    uint8_t                field_0x247;
    uint8_t                field_0x248;
    uint8_t                field_0x249;
    uint8_t                field_0x24a;
    uint8_t                field_0x24b;
    uint8_t                field_0x24c;
    uint8_t                field_0x24d;
    uint8_t                field_0x24e;
    uint8_t                field_0x24f;
    uint8_t                field_0x250;
    uint8_t                field_0x251;
    uint8_t                field_0x252;
    uint8_t                field_0x253;
    uint8_t                field_0x254;
    uint8_t                field_0x255;
    uint8_t                field_0x256;
    uint8_t                field_0x257;
    uint8_t                field_0x258;
    uint8_t                field_0x259;
    uint8_t                field_0x25a;
    uint8_t                field_0x25b;
    uint8_t                field_0x25c;
    uint8_t                field_0x25d;
    uint8_t                field_0x25e;
    uint8_t                field_0x25f;
    uint8_t                field_0x260;
    uint8_t                field_0x261;
    uint8_t                field_0x262;
    uint8_t                field_0x263;
    uint8_t                field_0x264;
    uint8_t                field_0x265;
    uint8_t                field_0x266;
    uint8_t                field_0x267;
    uint8_t                field_0x268;
    uint8_t                field_0x269;
    uint8_t                field_0x26a;
    uint8_t                field_0x26b;
    uint8_t                field_0x26c;
    uint8_t                field_0x26d;
    uint8_t                field_0x26e;
    uint8_t                field_0x26f;
    uint8_t                field_0x270;
    uint8_t                field_0x271;
    uint8_t                field_0x272;
    uint8_t                field_0x273;
    uint8_t                field_0x274;
    uint8_t                field_0x275;
    uint8_t                field_0x276;
    uint8_t                field_0x277;
    uint8_t                field_0x278;
    uint8_t                field_0x279;
    uint8_t                field_0x27a;
    uint8_t                field_0x27b;
    uint8_t                field_0x27c;
    uint8_t                field_0x27d;
    uint8_t                field_0x27e;
    uint8_t                field_0x27f;
    uint8_t                field_0x280;
    uint8_t                field_0x281;
    uint8_t                field_0x282;
    uint8_t                field_0x283;
    uint8_t                field_0x284;
    uint8_t                field_0x285;
    uint8_t                field_0x286;
    uint8_t                field_0x287;
    uint8_t                field_0x288;
    uint8_t                field_0x289;
    uint8_t                field_0x28a;
    uint8_t                field_0x28b;
    uint8_t                field_0x28c;
    uint8_t                field_0x28d;
    uint8_t                field_0x28e;
    uint8_t                field_0x28f;
    uint8_t                field_0x290;
    uint8_t                field_0x291;
    uint8_t                field_0x292;
    uint8_t                field_0x293;
    uint8_t                field_0x294;
    uint8_t                field_0x295;
    uint8_t                field_0x296;
    uint8_t                field_0x297;
    uint8_t                field_0x298;
    uint8_t                field_0x299;
    uint8_t                field_0x29a;
    uint8_t                field_0x29b;
    uint8_t                field_0x29c;
    uint8_t                field_0x29d;
    uint8_t                field_0x29e;
    uint8_t                field_0x29f;
    uint8_t                field_0x2a0;
    uint8_t                field_0x2a1;
    uint8_t                field_0x2a2;
    uint8_t                field_0x2a3;
    uint8_t                field_0x2a4;
    uint8_t                field_0x2a5;
    uint8_t                field_0x2a6;
    uint8_t                field_0x2a7;
    uint8_t                field_0x2a8;
    uint8_t                field_0x2a9;
    uint8_t                field_0x2aa;
    uint8_t                field_0x2ab;
    uint8_t                field_0x2ac;
    uint8_t                field_0x2ad;
    uint8_t                field_0x2ae;
    uint8_t                field_0x2af;
    uint8_t                field_0x2b0;
    uint8_t                field_0x2b1;
    uint8_t                field_0x2b2;
    uint8_t                field_0x2b3;
    uint8_t                field_0x2b4;
    uint8_t                field_0x2b5;
    uint8_t                field_0x2b6;
    uint8_t                field_0x2b7;
    uint8_t                field_0x2b8;
    uint8_t                field_0x2b9;
    uint8_t                field_0x2ba;
    uint8_t                field_0x2bb;
    uint8_t                field_0x2bc;
    uint8_t                field_0x2bd;
    uint8_t                field_0x2be;
    uint8_t                field_0x2bf;
    uint8_t                field_0x2c0;
    uint8_t                field_0x2c1;
    uint8_t                field_0x2c2;
    uint8_t                field_0x2c3;
    uint8_t                field_0x2c4;
    uint8_t                field_0x2c5;
    uint8_t                field_0x2c6;
    uint8_t                field_0x2c7;
    uint8_t                field_0x2c8;
    uint8_t                field_0x2c9;
    uint8_t                field_0x2ca;
    uint8_t                field_0x2cb;
    uint8_t                field_0x2cc;
    uint8_t                field_0x2cd;
    uint8_t                field_0x2ce;
    uint8_t                field_0x2cf;
    uint8_t                field_0x2d0;
    uint8_t                field_0x2d1;
    uint8_t                field_0x2d2;
    uint8_t                field_0x2d3;
    uint8_t                field_0x2d4;
    uint8_t                field_0x2d5;
    uint8_t                field_0x2d6;
    uint8_t                field_0x2d7;
    uint8_t                field_0x2d8;
    uint8_t                field_0x2d9;
    uint8_t                field_0x2da;
    uint8_t                field_0x2db;
    uint8_t                field_0x2dc;
    uint8_t                field_0x2dd;
    uint8_t                field_0x2de;
    uint8_t                field_0x2df;
    uint8_t                field_0x2e0;
    uint8_t                field_0x2e1;
    uint8_t                field_0x2e2;
    uint8_t                field_0x2e3;
    uint8_t                field_0x2e4;
    uint8_t                field_0x2e5;
    uint8_t                field_0x2e6;
    uint8_t                field_0x2e7;
    uint8_t                field_0x2e8;
    uint8_t                field_0x2e9;
    uint8_t                field_0x2ea;
    uint8_t                field_0x2eb;
    uint8_t                field_0x2ec;
    uint8_t                field_0x2ed;
    uint8_t                field_0x2ee;
    uint8_t                field_0x2ef;
    uint8_t                field_0x2f0;
    uint8_t                field_0x2f1;
    uint8_t                field_0x2f2;
    uint8_t                field_0x2f3;
    uint8_t                field_0x2f4;
    uint8_t                field_0x2f5;
    uint8_t                field_0x2f6;
    uint8_t                field_0x2f7;
    uint8_t                field_0x2f8;
    uint8_t                field_0x2f9;
    uint8_t                field_0x2fa;
    uint8_t                field_0x2fb;
    uint8_t                field_0x2fc;
    uint8_t                field_0x2fd;
    uint8_t                field_0x2fe;
    uint8_t                field_0x2ff;
    uint8_t                field_0x300;
    uint8_t                field_0x301;
    uint8_t                field_0x302;
    uint8_t                field_0x303;
    uint8_t                field_0x304;
    uint8_t                field_0x305;
    uint8_t                field_0x306;
    uint8_t                field_0x307;
    uint8_t                field_0x308;
    uint8_t                field_0x309;
    uint8_t                field_0x30a;
    uint8_t                field_0x30b;
    uint8_t                field_0x30c;
    uint8_t                field_0x30d;
    uint8_t                field_0x30e;
    uint8_t                field_0x30f;
    uint8_t                field_0x310;
    uint8_t                field_0x311;
    uint8_t                field_0x312;
    uint8_t                field_0x313;
    uint8_t                field_0x314;
    uint8_t                field_0x315;
    uint8_t                field_0x316;
    uint8_t                field_0x317;
    uint8_t                field_0x318;
    uint8_t                field_0x319;
    uint8_t                field_0x31a;
    uint8_t                field_0x31b;
    uint8_t                field_0x31c;
    uint8_t                field_0x31d;
    uint8_t                field_0x31e;
    uint8_t                field_0x31f;
    uint8_t                field_0x320;
    uint8_t                field_0x321;
    uint8_t                field_0x322;
    uint8_t                field_0x323;
    uint8_t                field_0x324;
    uint8_t                field_0x325;
    uint8_t                field_0x326;
    uint8_t                field_0x327;
    uint8_t                field_0x328;
    uint8_t                field_0x329;
    uint8_t                field_0x32a;
    uint8_t                field_0x32b;
    uint8_t                field_0x32c;
    uint8_t                field_0x32d;
    uint8_t                field_0x32e;
    uint8_t                field_0x32f;
    uint32_t               field_330;
    float                  field_334; /* float */
    uint32_t               field_338; /* float */
    uint32_t               field_33C; /* float */
    uint32_t               field_340; /* float */
    uint32_t               field_344; /* float */
    uint32_t               field_348; /* float */
    uint32_t               field_34C;
    CVehicleModelInfo_obj2 field_350;
    uint8_t                field_0x35c;
    uint8_t                field_0x35d;
    uint8_t                field_0x35e;
    uint8_t                field_0x35f;
    uint8_t                field_0x360;
    uint8_t                field_0x361;
    uint8_t                field_0x362;
    uint8_t                field_0x363;
    uint8_t                field_0x364;
    uint8_t                field_0x365;
    uint8_t                field_0x366;
    uint8_t                field_0x367;
    uint8_t                field_0x368;
    uint8_t                field_0x369;
    uint8_t                field_0x36a;
    uint8_t                field_0x36b;
    uint8_t                field_0x36c;
    uint8_t                field_0x36d;
    uint8_t                field_0x36e;
    uint8_t                field_0x36f;
    uint8_t                field_0x370;
    uint8_t                field_0x371;
    uint8_t                field_0x372;
    uint8_t                field_0x373;
    uint8_t                field_0x374;
    uint8_t                field_0x375;
    uint8_t                field_0x376;
    uint8_t                field_0x377;
    uint8_t                field_0x378;
    uint8_t                field_0x379;
    uint8_t                field_0x37a;
    uint8_t                field_0x37b;
    uint8_t                field_0x37c;
    uint8_t                field_0x37d;
    uint8_t                field_0x37e;
    uint8_t                field_0x37f;
    uint8_t                field_0x380;
    uint8_t                field_0x381;
    uint8_t                field_0x382;
    uint8_t                field_0x383;
    uint8_t                field_0x384;
    uint8_t                field_0x385;
    uint8_t                field_0x386;
    uint8_t                field_0x387;
    uint8_t                field_0x388;
    uint8_t                field_0x389;
    uint8_t                field_0x38a;
    uint8_t                field_0x38b;
    uint8_t                field_0x38c;
    uint8_t                field_0x38d;
    uint8_t                field_0x38e;
    uint8_t                field_0x38f;
    uint8_t                field_0x390;
    uint8_t                field_0x391;
    uint8_t                field_0x392;
    uint8_t                field_0x393;
    uint8_t                field_0x394;
    uint8_t                field_0x395;
    uint8_t                field_0x396;
    uint8_t                field_0x397;
    uint8_t                field_0x398;
    uint8_t                field_0x399;
    uint8_t                field_0x39a;
    uint8_t                field_0x39b;
    uint8_t                field_0x39c;
    uint8_t                field_0x39d;
    uint8_t                field_0x39e;
    uint8_t                field_0x39f;
    uint8_t                field_0x3a0;
    uint8_t                field_0x3a1;
    uint8_t                field_0x3a2;
    uint8_t                field_0x3a3;
    uint8_t                field_0x3a4;
    uint8_t                field_0x3a5;
    uint8_t                field_0x3a6;
    uint8_t                field_0x3a7;
    uint8_t                field_0x3a8;
    uint8_t                field_0x3a9;
    uint8_t                field_0x3aa;
    uint8_t                field_0x3ab;
    uint8_t                field_0x3ac;
    uint8_t                field_0x3ad;
    uint8_t                field_0x3ae;
    uint8_t                field_0x3af;
    uint8_t                field_0x3b0;
    uint8_t                field_0x3b1;
    uint8_t                field_0x3b2;
    uint8_t                field_0x3b3;
    uint8_t                field_0x3b4;
    uint8_t                field_0x3b5;
    uint8_t                field_0x3b6;
    uint8_t                field_0x3b7;
    uint8_t                field_0x3b8;
    uint8_t                field_0x3b9;
    uint8_t                field_0x3ba;
    uint8_t                field_0x3bb;
    uint32_t               field_3BC;
    uint8_t                field_3C0;
    uint8_t                field_0x3c1;
    uint8_t                field_0x3c2;
    uint8_t                field_0x3c3;
    uint16_t               field_3C4;
    uint8_t                field_3C6;
    uint8_t                field_3C7;
};

struct CPedModelInfo : CBaseModelInfo
{ /* size = 0x160 */
    uint32_t field_60;
    uint32_t field_64;
    uint32_t field_68;
    uint32_t field_6C;
    uint32_t field_70;
    uint32_t field_74;
    uint32_t field_78;
    uint32_t field_7C;
    uint32_t m_nGestureGroup;
    uint32_t m_nFacialGroup;
    int      m_nVisemeGroup;
    bool     m_nPlayerPedType; /* Created by retype action */
    uint8_t  field_0x8d;
    uint8_t  field_0x8e;
    uint8_t  field_0x8f;
    uint32_t m_nPedType;
    uint32_t field_94;
    uint16_t field_98;
    uint16_t m_nFlags;
    int      field_0x9c;
    uint32_t m_nLastLoaded;
    uint32_t field_A4;
    uint32_t field_A8;
    uint32_t m_nUses;
    uint8_t  m_bRadio2;
    uint8_t  m_bRadio;
    uint8_t  field_B2;
    uint8_t  field_0xb3;
    uint16_t field_B4;
    uint16_t field_B6;
    uint16_t field_B8;
    uint16_t field_BA;
    uint32_t m_nFirstVoiceHash;
    uint8_t  field_C0;
    uint8_t  field_0xc1;
    uint8_t  field_0xc2;
    uint8_t  field_0xc3;
    uint8_t  field_0xc4;
    uint8_t  field_0xc5;
    uint8_t  field_0xc6;
    uint8_t  field_0xc7;
    uint8_t  field_0xc8;
    uint8_t  field_0xc9;
    uint8_t  field_0xca;
    uint8_t  field_0xcb;
    uint8_t  field_0xcc;
    uint8_t  field_0xcd;
    uint8_t  field_0xce;
    uint8_t  field_0xcf;
    uint8_t  field_0xd0;
    uint8_t  field_0xd1;
    uint8_t  field_0xd2;
    uint8_t  field_0xd3;
    uint8_t  field_0xd4;
    uint8_t  field_0xd5;
    uint8_t  field_0xd6;
    uint8_t  field_0xd7;
    uint8_t  field_0xd8;
    uint8_t  field_0xd9;
    uint8_t  field_0xda;
    uint8_t  field_0xdb;
    uint8_t  field_0xdc;
    uint8_t  field_0xdd;
    uint8_t  field_0xde;
    uint8_t  field_0xdf;
    uint8_t  field_0xe0;
    uint8_t  field_0xe1;
    uint8_t  field_0xe2;
    uint8_t  field_0xe3;
    uint8_t  field_0xe4;
    uint8_t  field_0xe5;
    uint8_t  field_0xe6;
    uint8_t  field_0xe7;
    uint32_t field_E8;
    uint8_t  m_nbAge;
    uint8_t  m_nbSexiness;
    uint8_t  m_nbBravery;
    uint8_t  m_nbAgility;
    uint8_t  m_e1stLanguage;
    uint8_t  m_e2ndLanguage;
    uint8_t  field_0xf2;
    uint8_t  field_0xf3;
    uint32_t m_fAttack;                   /* float */
    uint32_t m_fDefend;                   /* float */
    uint32_t m_fMeleeMartialLevelPercent; /* float */
    uint32_t field_100;                   /* animation inidces or pointers */
    uint8_t  field_0x104;
    uint8_t  field_0x105;
    uint8_t  field_0x106;
    uint8_t  field_0x107;
    uint8_t  field_0x108;
    uint8_t  field_0x109;
    uint8_t  field_0x10a;
    uint8_t  field_0x10b;
    uint8_t  field_0x10c;
    uint8_t  field_0x10d;
    uint8_t  field_0x10e;
    uint8_t  field_0x10f;
    uint8_t  field_0x110;
    uint8_t  field_0x111;
    uint8_t  field_0x112;
    uint8_t  field_0x113;
    uint8_t  field_0x114;
    uint8_t  field_0x115;
    uint8_t  field_0x116;
    uint8_t  field_0x117;
    uint8_t  field_0x118;
    uint8_t  field_0x119;
    uint8_t  field_0x11a;
    uint8_t  field_0x11b;
    uint32_t field_11C; /* animation index or pointer */
    uint32_t m_dwPersonalityFlagsL;
    uint32_t m_dwPersonalityFlagsH;
    uint32_t field_128;
    uint32_t field_12C;
    uint32_t field_130;
    uint8_t  field_134;
    uint8_t  field_0x135;
    uint8_t  field_0x136;
    uint8_t  field_0x137;
    uint8_t  field_0x138;
    uint8_t  field_0x139;
    uint8_t  field_0x13a;
    uint8_t  field_0x13b;
    uint8_t  field_0x13c;
    uint8_t  field_0x13d;
    uint8_t  field_0x13e;
    uint8_t  field_0x13f;
    uint8_t  field_0x140;
    uint8_t  field_0x141;
    uint8_t  field_0x142;
    uint8_t  field_0x143;
    uint8_t  field_0x144;
    uint8_t  field_0x145;
    uint8_t  field_0x146;
    uint8_t  field_0x147;
    uint8_t  field_0x148;
    uint8_t  field_0x149;
    uint8_t  field_0x14a;
    uint8_t  field_0x14b;
    uint8_t  field_0x14c;
    uint8_t  field_0x14d;
    uint8_t  field_0x14e;
    uint8_t  field_0x14f;
    uint8_t  field_0x150;
    uint8_t  field_0x151;
    uint8_t  field_0x152;
    uint8_t  field_0x153;
    uint8_t  field_0x154;
    uint8_t  field_0x155;
    uint8_t  field_0x156;
    uint8_t  field_0x157;
    uint8_t  field_158;
    uint8_t  field_159;
    uint8_t  field_0x15a;
    uint8_t  field_0x15b;
    uint32_t field_15C;
};
