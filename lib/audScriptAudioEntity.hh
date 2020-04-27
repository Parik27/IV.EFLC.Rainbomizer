#include <cstdint>
#pragma once

#pragma pack(push, 1)
class CSound
{
public:
    uint32_t __pad00[2];
    uint32_t hash;
    uint16_t __padC;
};
#pragma pack(pop)

#pragma pack(push, 1)
class CSoundBank
{
public:
    CSound * sounds;
    uint32_t hash;
    uint16_t size;
};
#pragma pack(pop)

class audScriptAudioEntity
{
public:
    void AddLineToConversation (int index, int param_3, char *identifier,
                                char *subtitle, int param_6, int param_7);

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();

    static CSoundBank *&g_aSoundMetadata();
    static int &        g_nSoundMetadataLen();
};
