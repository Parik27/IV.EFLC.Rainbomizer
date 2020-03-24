#include <random>
#include <ctime>

#include "CCrypto.hh"
#include "CHud.hh"
#include "CPed.hh"
#include "CScrVM.hh"
#include "CStreaming.hh"
#include "CTimer.hh"
#include "CVehicleFactory.hh"
#include "CWeaponInfo.hh"
//#include "CWeaponModelInfo.h"
#include "CModelInfoStore.hh"
#include "CText.hh"
#include "audScriptAudioEntity.hh"

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
void
InitialiseAllComponents ()
{

    static bool initialised = false;

    if (initialised)
        return;

    CPedWeapons::InitialisePatterns ();
    CWeaponInfo::InitialisePatterns ();
    CStreaming::InitialisePatterns ();
    CCrypto::InitialisePatterns ();
    CScrVM::InitialisePatterns ();
    CNativeManager::Initialise ();
    CVehicleFactory::InitialisePatterns ();
    CTimer::InitialisePatterns ();
    CHud::InitialisePatterns ();
    CModelInfoStore::InitialisePatterns ();
    audScriptAudioEntity::InitialisePatterns ();
    CText::InitialisePatterns ();

    initialised = true;
}
