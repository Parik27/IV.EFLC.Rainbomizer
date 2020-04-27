#include "CPlayer.hh"
#include "Utils.hh"

CEntity* (*CPlayer__FindPlayerEntity)(CPlayer*);
CPed* (*CPlayer__FindPlayerPed)(CPlayer*);

/*******************************************************/
void CPlayer::InitialisePatterns()
{
    ConvertCall (hook::get_pattern (
                     "8b 44 ? ? 85 c0 75 ? a1 ? ? ? ? 83 f8 ff 74"),
                 CPlayer__FindPlayerPed);

    ConvertCall (hook::get_pattern (
                     "8b 44 ? ? 85 c0 75 ? a1 ? ? ? ? 83 f8 ff 75"),
                 CPlayer__FindPlayerEntity);
}

/*******************************************************/
CPed* CPlayer::FindPlayerPed(CPlayer* player)
{
  return CPlayer__FindPlayerPed(player);
}

/*******************************************************/
CEntity* CPlayer::FindPlayerEntity(CPlayer* player)
{
  return CPlayer__FindPlayerEntity(player);
}
