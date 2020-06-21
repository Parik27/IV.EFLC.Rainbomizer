#pragma once

#include <CEntity.hh>
#include <cstddef>
#include "CMaths.hh"
#include <cstdint>

enum ePedBone : uint32_t
{
    BONE_ROOT               = 0,
    BONE_PELVIS             = 0x1A1,
    BONE_SPINE              = 0x4B2,
    BONE_SPINE1             = 0x4B3,
    BONE_SPINE2             = 0x36A0,
    BONE_SPINE3             = 0x36A1,
    BONE_NECK               = 0x4B4,
    BONE_HEAD               = 0x4B5,
    BONE_NECKROLL           = 0x37A0,
    BONE_R_CLAVICLE         = 0x4C7,
    BONE_R_UPPERARM         = 0x4C8,
    BONE_R_FOREARM          = 0x4C9,
    BONE_R_HAND             = 0x4D0,
    BONE_R_FINGER0          = 0x35B0,
    BONE_R_FINGER01         = 0x35B1,
    BONE_R_FINGER02         = 0x35B2,
    BONE_R_FINGER1          = 0x35B3,
    BONE_R_FINGER11         = 0x35B4,
    BONE_R_FINGER12         = 0x35B5,
    BONE_R_FINGER2          = 0x35B6,
    BONE_R_FINGER21         = 0x35B7,
    BONE_R_FINGER22         = 0x35B8,
    BONE_R_FINGER3          = 0x35B9,
    BONE_R_FINGER31         = 0x35C0,
    BONE_R_FINGER32         = 0x35C1,
    BONE_R_FINGER4          = 0x35C2,
    BONE_R_FINGER41         = 0x35C3,
    BONE_R_FINGER42         = 0x35C4,
    BONE_R_FORETWIST        = 0x39A1,
    BONE_R_FORETWIST1       = 0x39A2,
    BONE_R_UPPPERARMROLL    = 0x39A0,
    BONE_R_ARMROLL          = 0x3E01,
    BONE_L_CLAVICLE         = 0x4C0,
    BONE_L_UPPERARM         = 0x4C1,
    BONE_L_FOREARM          = 0x4C2,
    BONE_L_HAND             = 0x4C3,
    BONE_L_FINGER0          = 0x35D0,
    BONE_L_FINGER01         = 0x35D1,
    BONE_L_FINGER02         = 0x35D2,
    BONE_L_FINGER1          = 0x35D3,
    BONE_L_FINGER11         = 0x35D4,
    BONE_L_FINGER12         = 0x35D5,
    BONE_L_FINGER2          = 0x35D6,
    BONE_L_FINGER21         = 0x35D7,
    BONE_L_FINGER22         = 0x35D8,
    BONE_L_FINGER3          = 0x35D9,
    BONE_L_FINGER31         = 0x35E0,
    BONE_L_FINGER32         = 0x35E1,
    BONE_L_FINGER4          = 0x35E2,
    BONE_L_FINGER41         = 0x35E3,
    BONE_L_FINGER42         = 0x35E4,
    BONE_L_FORETWIST        = 0x38A1,
    BONE_L_FORETWIST1       = 0x38A2,
    BONE_L_UPPPERARMROLL    = 0x38A0,
    BONE_L_ARMROLL          = 0x3DF1,
    BONE_L_THIGH            = 0x1A2,
    BONE_L_CALF             = 0x1A3,
    BONE_L_FOOT             = 0x1A4,
    BONE_L_TOE              = 0x1A5,
    BONE_L_CALFROLL         = 0x38B0,
    BONE_R_THIGH            = 0x1A7,
    BONE_R_CALF             = 0x1A8,
    BONE_R_FOOT             = 0x1A9,
    BONE_R_TOE              = 0x4B0,
    BONE_R_CALFROLL         = 0x39B0,
    BONE_L_BROWAJNT         = 0x78F7,
    BONE_L_BROWBJNT         = 0x78F8,
    BONE_L_LIDJNT           = 0x78F9,
    BONE_C_LOWLIDJNT        = 0x78FA,
    BONE_L_CHEEKJNT         = 0x78FB,
    BONE_L_EYEJNT           = 0x78FC,
    BONE_L_CORNERAJNT       = 0x7903,
    BONE_L_CORNERBJNT       = 0x79F4,
    BONE_L_JAWAJNT          = 0x7904,
    BONE_L_JAWBJNT          = 0x7905,
    BONE_L_LIPUPAJNT        = 0x7906,
    BONE_R_BROWAJNT         = 0x7907,
    BONE_R_BROWBJNT         = 0x7908,
    BONE_R_LIDJNT           = 0x7909,
    BONE_R_CHEEKJNT         = 0x790A,
    BONE_R_EYEJNT           = 0x790B,
    BONE_R_CORNERAJNT       = 0x790C,
    BONE_R_CORNERBJNT       = 0x79F3,
    BONE_R_JAWAJNT          = 0x7913,
    BONE_R_JAWBJNT          = 0x7914,
    BONE_R_LIPUPAJNT        = 0x7915,
    BONE_C_JAWAJNT          = 0x7916,
    BONE_FB_C_BROW          = 0x7F94,
    BONE_FB_C_JAW           = 0x7F9B,
    BONE_FB_L_LIPLOWER      = 0x7FA7,
    BONE_FB_R_LIPLOWER      = 0x7FA6,
    BONE_FB_L_BROW          = 0x7F9A,
    BONE_FB_L_CRN_MOUTH     = 0x7FA5,
    BONE_FB_L_EYEBALL       = 0x7F98,
    BONE_FB_L_EYELID        = 0x7F99,
    BONE_FB_L_LIPUPPER      = 0x7F9D,
    BONE_FB_R_BROW          = 0x7F95,
    BONE_FB_R_CRN_MOUTH     = 0x7FA4,
    BONE_FB_R_EYEBALL       = 0x7F97,
    BONE_FB_R_EYELID        = 0x7F96,
    BONE_FB_R_LIPUPPER      = 0x7F9C,
    BONE_FB_L_BROWBJNT      = 0x0A917,
    BONE_FB_L_BROWAJNT      = 0x0A905,
    BONE_FB_C_FOREHEAD      = 0x0A8F6,
    BONE_FB_L_EYEJNT        = 0x0A8F7,
    BONE_FB_L_UPPCHEEKJNT   = 0x0A8F8,
    BONE_FB_L_CORNERLIPJNT  = 0x0A8F9,
    BONE_FB_L_LOCHEEKJNT    = 0x0A8FA,
    BONE_FB_L_UPPLIPJNT     = 0x0A8FB,
    BONE_FB_L_UPPLIDJNT     = 0x0A8FC,
    BONE_FB_L_LOLIDJNT      = 0x0A903,
    BONE_FB_R_BROWAJNT      = 0x0A904,
    BONE_FB_R_EYEJNT        = 0x0A916,
    BONE_FB_R_UPPLIDJNT     = 0x0A906,
    BONE_FB_R_LOLIDJNT      = 0x0A907,
    BONE_FB_R_BROWBJNT      = 0x0A908,
    BONE_FB_R_UPPCHEEKJNT   = 0x0A909,
    BONE_FB_R_UPPLIPJNT     = 0x0A90A,
    BONE_FB_R_CORNERLIPJNT  = 0x0A90B,
    BONE_FB_R_LOCHEEKJNT    = 0x0A90C,
    BONE_FB_C_JAWJNT        = 0x0A913,
    BONE_FB_R_LOLIPJNT      = 0x0A914,
    BONE_FB_L_LOLIPJNT      = 0x0A915,
    BONE_FB_C_TONGUE_A_JNT  = 0x0A8F5,
    BONE_FB_C_TONGUE_B_JNT  = 0x0A8F4,
    BONE_POINTFB_C_JAW      = 0x0C944,
    BONE_POINTFB_R_LIPLOWER = 0x0C947,
    BONE_POINTFB_L_LIPLOWER = 0x0C948,
    BONE_POINTFB_L_LIPUPPER = 0x0C946,
    BONE_POINTFB_R_LIPUPPER = 0x0C945,
    BONE_EXTRA1             = 0x0B064,
    BONE_EXTRA2             = 0x0B065,
    BONE_EXTRA3             = 0x0B066,
    BONE_UNKNOWN            = 0x0FFFFFFFF
};

class CPedInventory
{
public:
    void GiveWeapon (int weapon, int ammo, char param4, char param5,
                     bool shown);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};

class CPed : public CEntity
{
    static int m_pWeaponsOffset;

public:
    CPedInventory &m_pWeapons ();

    Matrix34 *GetBoneMatrix (int bone);

    // Helper function to get the bone index
    short GetBoneIndexFromId (ePedBone boneId);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();
};
