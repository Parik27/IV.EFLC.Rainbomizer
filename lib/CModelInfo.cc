#include "CModelInfo.hh"

/*******************************************************/
eModelInfoType
CBaseModelInfo::GetType ()
{
    return this->__vmt->GetType (this);
}

/*******************************************************/
void
CBaseModelInfo::Release ()
{
    return this->__vmt->Release (this);
}
