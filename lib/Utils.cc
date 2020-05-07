#include <random>
#include <ctime>

#include "CCrypto.hh"
#include "Patterns/Patterns.hh"
#include "CHud.hh"
#include "CPed.hh"
#include "CTheScripts.hh"
#include "CStreaming.hh"
#include "CTimer.hh"
#include "CVehicleFactory.hh"
#include "CWeaponInfo.hh"
//#include "CWeaponModelInfo.h"
#include "CModelInfoStore.hh"
#include "CText.hh"
#include "audScriptAudioEntity.hh"
#include "CPlayer.hh"
#include "CPools.hh"

/*******************************************************/
std::mt19937 &
RandEngine ()
{
    thread_local static std::mt19937 engine{(unsigned int) time (NULL)};

    return engine;
}

/*******************************************************/
int
RandomInt (int min, int max)
{
    std::uniform_int_distribution<int> dist{min, max};
    return dist (RandEngine ());
}

/*******************************************************/
int
RandomInt (int max)
{
    return RandomInt (0, max);
}

/*******************************************************/
bool
IsCompleteEdition ()
{
    // There might be a better way to do this, but this works for now :P
    static bool ce
        = hook::pattern ("83 f8 03 0f 84 ? ? ? ? a1").size () == 0
          && hook::pattern ("83 f8 03 0f 84 ? ? ? ? 8b 35").size () != 0;

    return ce;
}

/*******************************************************/
void
InitialiseAllComponents ()
{

    static bool initialised = false;

    if (initialised)
        return;

    if (IsCompleteEdition ())
        {
            CPedWeapons::InitialisePatternsCE ();
            CWeaponInfo::InitialisePatternsCE ();
            CStreaming::InitialisePatternsCE ();
            CTheScripts::InitialisePatternsCE ();
            CNativeManager::InitialiseCE ();
            CVehicleFactory::InitialisePatternsCE ();
            CHud::InitialisePatternsCE ();
            CModelInfoStore::InitialisePatternsCE ();
            audScriptAudioEntity::InitialisePatternsCE ();
            CText::InitialisePatternsCE ();
            CPlayer::InitialisePatternsCE ();
            CPools::InitialisePatternsCE ();
            CPed::InitialisePatternsCE ();
        }
    else
        {
            CPedWeapons::InitialisePatterns ();
            CWeaponInfo::InitialisePatterns ();
            CStreaming::InitialisePatterns ();
            CTheScripts::InitialisePatterns ();
            CNativeManager::Initialise ();
            CVehicleFactory::InitialisePatterns ();
            CHud::InitialisePatterns ();
            CModelInfoStore::InitialisePatterns ();
            audScriptAudioEntity::InitialisePatterns ();
            CText::InitialisePatterns ();
            CPlayer::InitialisePatterns ();
            CPools::InitialisePatterns ();
            CPed::InitialisePatterns ();
        }
    initialised = true;
}
