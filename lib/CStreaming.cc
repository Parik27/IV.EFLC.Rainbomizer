#include "Patterns/Patterns.hh"
#include "CStreaming.hh"
#include "Utils.hh"
#include "CCrypto.hh"
#include <CPools.hh>

void (*CStreaming__RequestResource) (int index, int fileType, int flags);
int (*CStreaming__GetStreamingFlags) (int index, int fileType);
void (*CStreaming__LoadAllObjects) (int bPriorityOnly);
bool (*CStreaming__HasResourceLoaded) (int index, int fileType);
void (*CStreaming__MarkResourceAsNoLongerNeeded) (int index, int fileType);
CBaseModelInfo *(*CStreaming__GetModelAndIndexFromHash) (uint32_t, int *);
CBaseModelInfo *(*CStreaming__GetModelFromHash) (uint32_t);
int *CStreaming__g_pFileTypeWdrIndex;
void (__stdcall *CStreaming__FreeModel) (int);

/*******************************************************/
void
InitialiseGetModelFunctions ()
{
    auto pattern = hook::pattern ("8b 44 ? ? 89 44 ? ? 66 a1 ? ? ? ?");
    ConvertCall (pattern.get (ByVersion (0, 1)).get<void> (),
                 CStreaming__GetModelAndIndexFromHash);
    ConvertCall (pattern.get (ByVersion (1, 0)).get<void> (),
                 CStreaming__GetModelFromHash);
}

/*******************************************************/
void
CStreaming::InitialisePatternsCE ()
{
    ConvertCall (
        SearchBack (
            "6b c0 64 8b ? ? ? ? ? 03 44 ? ? b9 ? ? ? ? 50 e8 ? ? ? ? c3",
            "8b 44 24 08", 0x10),
        CStreaming__RequestResource);

    ConvertCall (SearchBack ("8d 0c 40 a1 ? ? ? ? 0f b7 44 c8 0e c3 ",
                             "8b 44 24 08", 0x30),
                 CStreaming__GetStreamingFlags);

    ConvertCall (hook::get_pattern (
                     "8b 0d ? ? ? ? ff 74 ? ? 8b 01 ff 50 18 c3 "),
                 CStreaming__LoadAllObjects);

    ConvertCall (SearchBack ("80 e1 03 33 c0 80 f9 01 0f 94", "8b 44 24 08",
                             0x30),
                 CStreaming__HasResourceLoaded);

    ConvertCall (
        SearchBack (
            "6b c0 64 6a 02 8b ? ? ? ? ? 03 44 ? ? b9 ? ? ? ? 50 e8 ? ? ? ? ",
            "8b 44 24 08", 0x30),
        CStreaming__MarkResourceAsNoLongerNeeded);

    ReadCall (hook::get_pattern ("3b c7 74 ? 50 8b cb e8 ? ? ? ? ", 7),
              CStreaming__FreeModel);

    InitialiseGetModelFunctions (); // Same patterns for CE

    ms_instance = *hook::get_pattern<CStreaming *> (
        "b9 ? ? ? ? e8 ? ? ? ? 85 c0 0f 88 ? ? ? ? ", 1);

    CStreaming__g_pFileTypeWdrIndex = *hook::get_pattern<int *> (
        "ff 35 ? ? ? ? b9 ? ? ? ? e8 ? ? ? ? 50 e8 ? ? ? ? 83 c4 08 ", 2);
}

/*******************************************************/
void
CStreaming::InitialisePatterns ()
{
    ConvertCall (SearchBack ("03 44 ? ? 51 50 b9 ? ? ? ? e8 ? ? ? ?",
                             "8b 44 24 08", 0x30),
                 CStreaming__RequestResource);

    ConvertCall (SearchBack ("03 44 ? ? 8d 0c 40 0f b7 44 ca 0e c3",
                             "8b 44 24 08", 0x30),
                 CStreaming__GetStreamingFlags);

    ConvertCall (SearchBack ("8b 40 18 52 ff d0 c3", "8b 0d", 18),
                 CStreaming__LoadAllObjects);

    ConvertCall (SearchBack ("24 03 2c 01 f6 d8 1b c0 83 c0 01 c3",
                             "8b 44 24 08", 0x30),
                 CStreaming__HasResourceLoaded);

    ConvertCall (SearchBack ("6a 02 51 b9 ? ? ? ? e8 ? ? ? ? c3", "8b 44 24 08",
                             0x30),
                 CStreaming__MarkResourceAsNoLongerNeeded);

    ReadCall (hook::get_pattern ("85 c0 7c ? 8b 4c ? 1c 50 e8", 9),
              CStreaming__FreeModel);

    InitialiseGetModelFunctions ();

    ms_instance = *hook::get_pattern<CStreaming *> (
        "85 c0 0f 8c ? ? ? ? b9 ? ? ? ? e8 ? ? ? ?", 9);

    CStreaming__g_pFileTypeWdrIndex
        = *hook::get_pattern<int *> ("8b f8 e9 ? ? ? ? a1 ? ? ? ?", 8);
}

/*******************************************************/
void
CStreaming::RequestResource (int index, int fileType, int flags)
{
    CStreaming__RequestResource (index, fileType, flags);
}

/*******************************************************/
int
CStreaming::GetStreamingFlags (int index, int fileType)
{
    return CStreaming__GetStreamingFlags (index, fileType);
}

/*******************************************************/
void
CStreaming::LoadAllObjects (int bPriorityOnly)
{
    CStreaming__LoadAllObjects (bPriorityOnly);
}

/*******************************************************/
bool
CStreaming::HasResourceLoaded (int index, int fileType)
{
    return CStreaming__HasResourceLoaded (index, fileType);
}

/*******************************************************/
void
CStreaming::MarkResourceAsNoLongerNeeded (int index, int fileType)
{
    return CStreaming__MarkResourceAsNoLongerNeeded (index, fileType);
}

/*******************************************************/
CBaseModelInfo *
CStreaming::GetModelAndIndexFromHash (unsigned int hash, int *indexOut)
{
    return CStreaming__GetModelAndIndexFromHash (hash, indexOut);
}

/*******************************************************/
CBaseModelInfo *
CStreaming::GetModelFromHashHash (unsigned int hash)
{
    return CStreaming__GetModelFromHash (hash);
}

/*******************************************************/
bool
CStreaming::AttemptToLoadModel (const std::string &modelName, int numTries,
                                bool vehicle)
{
    uint32_t hash = CCrypto::atStringHash (modelName.c_str ());
    return CStreaming::AttemptToLoadModel (hash, numTries, vehicle);
}

/*******************************************************/
bool
CStreaming::AttemptToLoadModel (uint32_t hash, int numTries, bool vehicle)
{
    int index = -1;
    CStreaming::GetModelAndIndexFromHash (hash, &index);

    // Make sure there aren't already too many vehicles loaded
    if (CPools::g_pVehicleStructPool ()->m_nCount
            > CPools::g_pVehicleStructPool ()->m_nMaxElements - 5
        && vehicle)
        return false;

    while (index > -1 && numTries--)
        {
            CStreaming::RequestResource (index, g_pFileTypeWdrIndex (), 0);
            CStreaming::LoadAllObjects (0);
            if (CStreaming::HasResourceLoaded (index, g_pFileTypeWdrIndex ()))
                return true;
        }
    return false;
}

/*******************************************************/
void
CStreaming::FreeWdrModel (int index)
{
    CStreaming__FreeModel (index);
}

/*******************************************************/
int &
CStreaming::g_pFileTypeWdrIndex ()
{
    return *CStreaming__g_pFileTypeWdrIndex;
}
CStreaming *CStreaming::ms_instance = nullptr;
