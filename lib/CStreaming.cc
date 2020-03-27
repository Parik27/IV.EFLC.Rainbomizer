#include "Patterns/Patterns.hh"
#include "CStreaming.hh"
#include "Utils.hh"
#include "CCrypto.hh"
#include "../src/logger.hh"

void (*CStreaming__RequestResource) (int index, int fileType, int flags);
int (*CStreaming__GetStreamingFlags) (int index, int fileType);
void (*CStreaming__LoadAllObjects) (int bPriorityOnly);
bool (*CStreaming__HasResourceLoaded) (int index, int fileType);
void (*CStreaming__MarkResourceAsNoLongerNeeded) (int index, int fileType);
CBaseModelInfo *(*CStreaming__GetModelAndIndexFromHash) (uint32_t, int *);
CBaseModelInfo *(*CStreaming__GetModelFromHash) (uint32_t);
int *CStreaming__g_pFileTypeWdrIndex;
void (*CStreaming__FreeModel) (int);

/*******************************************************/
void
InitialiseGetModelFunctions ()
{
    auto pattern = hook::pattern ("8b 44 ? ? 89 44 ? ? 66 a1 ? ? ? ?");
    ConvertCall (pattern.get (0).get<void> (),
                 CStreaming__GetModelAndIndexFromHash);
    ConvertCall (pattern.get (1).get<void> (), CStreaming__GetModelFromHash);
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
CStreaming::AttemptToLoadModel (const std::string &modelName, int numTries)
{
    uint32_t hash = CCrypto::HashString (modelName.c_str ());
    return CStreaming::AttemptToLoadModel (hash, numTries);
}

/*******************************************************/
bool
CStreaming::AttemptToLoadModel (uint32_t hash, int numTries)
{
    int index = -1;
    CStreaming::GetModelAndIndexFromHash (hash, &index);
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
CStreaming::FreeWdrModel(int index)
{
    CStreaming__FreeModel(index);
}

/*******************************************************/
int &
CStreaming::g_pFileTypeWdrIndex ()
{
    return *CStreaming__g_pFileTypeWdrIndex;
}
CStreaming *CStreaming::ms_instance = nullptr;
