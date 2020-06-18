#include "audEngine.hh"
#include <injector/injector.hpp>
#include <Utils.hh>

audBaseGameMetadata *(__thiscall *audEngine__GetGameMetadataItem) (
    audEngine *, const char *);

/*******************************************************/
uint32_t
audSoundMetadata::GetCategoryHash ()
{
    if(!this->m_nFlags.bHasCategoryHash)
        return -1;

    injector::memory_pointer_raw ptr = this;
    ptr = ptr + 15;

    if(m_nFlags.bField0) ptr = ptr + 2;
    if(m_nFlags.bField1) ptr = ptr + 2;
    if(m_nFlags.bField2) ptr = ptr + 2;
    if(m_nFlags.bField3) ptr = ptr + 2;
    if(m_nFlags.bField4) ptr = ptr + 2;
    if(m_nFlags.bField5) ptr = ptr + 2;
    if(m_nFlags.bField6) ptr = ptr + 2;
    if(m_nFlags.bField7) ptr = ptr + 2;
    if(m_nFlags.bField8) ptr = ptr + 4;
    if(m_nFlags.bField9) ptr = ptr + 4;
    if(m_nFlags.bField10) ptr = ptr + 2;
    if(m_nFlags.bField11) ptr = ptr + 2;
    if(m_nFlags.bField12) ptr = ptr + 2;

    return *ptr.get<uint32_t>();
}

/*******************************************************/
audMetadataMgr<audBaseGameMetadata> *
audEngine::GetGameMetadataMgr ()
{
    return (injector::raw_ptr (this) + mGameMetadataMgrOffset)
        .get_raw<audMetadataMgr<audBaseGameMetadata>> ();
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

    mGameMetadataMgrOffset = injector::ReadMemory<uint32_t> (
        injector::raw_ptr (audEngine__GetGameMetadataItem) + 2);
}

audEngine *audEngine::sm_Instance         = nullptr;
int        audEngine::mGameMetadataMgrOffset = 0x2c8;
