#include "CEntity.hh"
#include "Utils.hh"

void* (__thiscall *CEntity__GetAnimFrag) (CEntity*);

/*******************************************************/
void *
CEntity::GetAnimFrag ()
{
    return CEntity__GetAnimFrag (this);
}

/*******************************************************/
void
CEntity::InitialisePatternsCE ()
{
    ReadCall (hook::get_pattern ("8b cb ff 74 ? 38 55 e8 ? ? ? ? 50 8b ce", 7),
              CEntity__GetAnimFrag);
}

/*******************************************************/
void
CEntity::InitialisePatterns ()
{
    ReadCall (hook::get_pattern ("55 8b ce e8 ? ? ? ? 50 8b cf e8", 3),
              CEntity__GetAnimFrag);
}
