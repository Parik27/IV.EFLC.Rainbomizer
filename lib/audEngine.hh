#pragma once

#include <cstdint>
#include "atPrimitives.hh"

enum eGameMetadataType : uint8_t
{
    GM_AMBIENT_ZONE_LIST              = 21,
    GM_AMBIENT_ZONE                   = 19,
    GM_COLLISION                      = 1,
    GM_CLOTHING                       = 15,
    GM_DOOR                           = 25,
    GM_CUTSCENE                       = 23,
    GM_AMBIENT_EMITTER                = 2,
    GM_AMBIENT_EMITTER_LIST           = 17,
    GM_AMBIENT_EMITTER_ENTITY         = 3,
    GM_FOOTSTEPS                      = 9,
    GM_INTERIOR                       = 24,
    GM_MELEE_COMBAT                   = 5,
    GM_CRIME                          = 14,
    GM_RADIO_STATION_TRACK_CATEGORY   = 12,
    GM_RADIO_STATION                  = 11,
    GM_RADIO_STATION_CATEGORY_WEIGHTS = 13,
    GM_PED                            = 16,
    GM_TRAIN_STATION                  = 22,
    GM_VEHICLE                        = 0,
    GM_WEAPON                         = 8,
    GM_RADIO_STATION_LIST             = 10,
    GM_SOUND_RULES                    = 20
};

enum eSoundType : uint8_t
{
    audLoopingSound               = 1,
    audEnvelopeSound              = 2,
    audTwinLoopSound              = 3,
    audSpeechSound                = 4,
    audOnStopSound                = 5,
    audWrapperSound               = 6,
    audSequentialSound            = 7,
    audStreamingSound             = 8,
    audRetriggeredOverlappedSound = 9,
    audCrossfadeSound             = 10,
    audCollapsingStereoSound      = 11,
    audSimpleSound                = 12,
    audMultitrackSound            = 13,
    audRandomizedSound            = 14,
    audSwitchSound                = 16,
    audVariableCurveSound         = 17,
    audVariablePrintValueSound    = 18,
    audAssertSound                = 19,
    audVariableSetTimeSound       = 20,
    audVariableBlockSound         = 21,
    audIfSound                    = 22,
    audForLoopSound               = 23,
    audMathOperationSound         = 24

};

#pragma pack(push, 1)
struct audBaseGameMetadata
{
    eGameMetadataType Type;
    uint32_t          NameOffset;
    uint32_t          Flags;
    uint8_t           __field08;
};

struct audSoundMetadata
{
    eSoundType Type;
    uint32_t   NameOffset;
    uint32_t   __unk05;
    int16_t    __unk09;
    uint32_t   Flags;
};

template <typename T> struct audConfigMetadata
{
    T *m_pData;

    uint32_t m_pArchiveNamesRaw2;
    uint32_t m_nSize;
    int      m_nDataOffset;

    atMap<>  m_pArchiveNamesMap;
    uint32_t m_nNumArchives;
    uint32_t m_nArchiveNamesRawLen;
    int *    m_pArchiveNamesRaw;
    void *   m_pArchiveNames;

    uint32_t m_nObjectNamesLen;
    uint32_t field_0x30;
    char *   m_aszObjects;

    int m_nSuffix;

    uint32_t               m_nNumObjectHeaders;
    audHashPair<uint32_t> *m_apObjectHeaders;

    char field_0x44;

    T *
    GetByHash (uint32_t hash)
    {
        for (size_t i = 0; i < m_nNumObjectHeaders; i++)
            {
                if (m_apObjectHeaders[i].hash == hash)
                    return reinterpret_cast<T *> (
                        reinterpret_cast<char *> (m_pData)
                        + m_apObjectHeaders[i].data);
            }

        return nullptr;
    }

    template <typename F>
    void
    for_each (F functor)
    {
        for (size_t i = 0; i < m_nNumObjectHeaders; i++)
            {
                if (m_apObjectHeaders[i].hash == 0xcdcdcdcd
                    || m_apObjectHeaders[i].hash == -1
                    || m_apObjectHeaders[i].data == 0xcdcdcdcd
                    || m_apObjectHeaders[i].hash == 0)
                    continue;

                functor (reinterpret_cast<T *> (
                             reinterpret_cast<char *> (m_pData)
                             + m_apObjectHeaders[i].data),
                         m_apObjectHeaders[i].hash);
            }
    }
};
#pragma pack(pop)

class audEngine
{
    static int mGameMetadataOffset;

public:
    static audEngine *                      sm_Instance;
    audConfigMetadata<audBaseGameMetadata> *GetGameMetadata ();

    audBaseGameMetadata *GetGameMetadataItem (const char *name);

    static int
    GetGameMetadataOffset ()
    {
        return mGameMetadataOffset;
    }

    static void InitialisePatterns ();
    static void InitialisePatternsCE ();
};
