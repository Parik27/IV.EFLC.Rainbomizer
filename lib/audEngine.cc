#include "audEngine.hh"
#include <injector/injector.hpp>
#include <Utils.hh>

audBaseGameMetadata *(__thiscall *audEngine__GetGameMetadataItem) (
    audEngine *, const char *);

/*******************************************************/
audConfigMetadata<audBaseGameMetadata> *
audEngine::GetGameMetadata ()
{
    return (injector::raw_ptr (this) + mGameMetadataOffset)
        .get_raw<audConfigMetadata<audBaseGameMetadata>> ();
}

/*******************************************************/
void
audEngine::InitialisePatternsCE ()
{
    InitialisePatterns ();
}

/*******************************************************/
audBaseGameMetadata *
audEngine::GetGameMetadataItem (const char *name)
{
    return audEngine__GetGameMetadataItem (this, name);
}

/*******************************************************/
void
audEngine::InitialisePatterns ()
{
    injector::memory_pointer_raw addr
        = hook::get_pattern ("53 55 8b e9 68 ? ? ? ? b9 ? ? ? ? e8");

    ReadCall (addr + 14, audEngine__GetGameMetadataItem);
    sm_Instance = *(addr + 10).get_raw<audEngine *> ();

    mGameMetadataOffset = injector::ReadMemory<uint32_t> (
        injector::raw_ptr (audEngine__GetGameMetadataItem) + 2);
}

audEngine *audEngine::sm_Instance         = nullptr;
int        audEngine::mGameMetadataOffset = 0x2c8;
