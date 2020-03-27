#include "CHud.hh"
#include "injector/hooking.hpp"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include <algorithm>
#include <utility>
#include <ctime>
#include <cmath>
#include "CPed.hh"
#include <vector>
#include "CWeaponInfo.hh"
#include "CScrVM.hh"
#include "CStreaming.hh"
#include "CCrypto.hh"
#include "common.hh"
#include "config.hh"
#include "logger.hh"

bool
IsModelPartOfGroup (uint32_t hash, const std::vector<std::string> &mList)
{
    for (auto i : mList)
        {
            if (CCrypto::HashString (i.c_str ()) == hash)
                return true;
        }
    return false;
}

class WeaponRandomizer
{
    static injector::scoped_jmp mHooka98500;
    static std::vector<int>     mValidWeapons;

    /*******************************************************/
    static void
    RandomizeWeaponStat (int index)
    {
        auto info = CWeaponInfo::GetInfoFromIndex (index);

        auto randomInfo = [] {
            return CWeaponInfo::GetInfoFromIndex (
                mValidWeapons[RandomInt (mValidWeapons.size () - 1)]);
        };

        std::swap (randomInfo ()->clipSize, info->clipSize);
        // std::swap (randomInfo ()->fireType, info->fireType);
        std::swap (randomInfo ()->damageType, info->damageType);
        std::swap (randomInfo ()->targetRange, info->targetRange);
        std::swap (randomInfo ()->weaponRange, info->weaponRange);
        std::swap (randomInfo ()->statType, info->statType);
        // std::swap (randomInfo ()->flags, info->flags);
        // std::swap (randomInfo ()->m_nWeaponModelHash,
        // info->m_nWeaponModelHash); std::swap (randomInfo ()->animGroup,
        // info->animGroup); std::swap (randomInfo ()->animFireRate,
        // info->animFireRate); std::swap (randomInfo ()->animBlindFireRate,
        // info->animBlindFireRate);
        std::swap (randomInfo ()->rumbleDuration, info->rumbleDuration);
        std::swap (randomInfo ()->rumbleIntensity, info->rumbleIntensity);
        std::swap (randomInfo ()->regenTime, info->regenTime);
        std::swap (randomInfo ()->ammoonstreet, info->ammoonstreet);
        std::swap (randomInfo ()->base, info->base);
        std::swap (randomInfo ()->fps, info->fps);
        std::swap (randomInfo ()->clipSize, info->clipSize);
        std::swap (randomInfo ()->ammoMax, info->ammoMax);
        std::swap (randomInfo ()->timeBetweenShots, info->timeBetweenShots);
        std::swap (randomInfo ()->force, info->force);
        std::swap (randomInfo ()->time, info->time);
        std::swap (randomInfo ()->fastTime, info->fastTime);
        std::swap (randomInfo ()->crouchTime, info->crouchTime);
        // std::swap (randomInfo ()->animMeleeGroup1, info->animMeleeGroup1);
        // std::swap (randomInfo ()->animMeleeGroup2, info->animMeleeGroup2);
        std::swap (randomInfo ()->muzzleFx, info->muzzleFx);
        std::swap (randomInfo ()->shellFx, info->shellFx);
        std::swap (randomInfo ()->trailFx, info->trailFx);
    }

    /*******************************************************/
    static void InitialiseWeaponsArray()
    {
        mValidWeapons.clear ();

        // These models are exempt from randomization
        const std::vector<std::string> mExceptions
            = {"cj_rpg_rocket", "w_e2_grenade", "W_e2_rocket", "w_e1_grenade"};

        const int TOTAL_WEAPONS = 0x3c;
        for (int i = 0; i < TOTAL_WEAPONS; i++)
            {
                auto info = CWeaponInfo::GetInfoFromIndex (i);
                int  hash = info->m_nWeaponModelHash;

                if (hash && !IsModelPartOfGroup (hash, mExceptions))
                    mValidWeapons.push_back (i);
            }

        Rainbomizer::Logger::LogMessage ("Initialised %d valid weapons\n",
                                         mValidWeapons.size ());
    }
    
    /*******************************************************/
    static void __fastcall RandomizeWeapon (CPedWeapons *weapons, void *edx,
                                            int weapon, int ammo, char param4,
                                            char param5, bool shown)
    {
        mHooka98500.restore ();

        if (mValidWeapons.size () == 0)
            InitialiseWeaponsArray();

        auto newWeapon = mValidWeapons[RandomInt (mValidWeapons.size () - 1)];
        if (ConfigManager::GetConfigs ().weaponStats.enabled)
            RandomizeWeaponStat (newWeapon);

        weapons->GiveWeapon (newWeapon, ammo, param4, param5, shown);

        InitialiseRandomWeaponsHook ();
    }

    /*******************************************************/
    static void
    InitialiseRandomWeaponsHook ()
    {
        static void *addr
            = hook::get_pattern ("e8 ? ? ? ? 8b ? 24 1c 8b ? 04 83 c4 04", -11);

        mHooka98500.make_jmp (addr, WeaponRandomizer::RandomizeWeapon);
    }

public:
    /*******************************************************/
    WeaponRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().weapon.enabled)
            return;

        InitialiseAllComponents ();
        InitialiseRandomWeaponsHook ();

        Rainbomizer::Common::AddEpisodeChangeCallback (
            [] (int) { mValidWeapons.clear (); });

        Rainbomizer::Logger::LogMessage ("Initialised WeaponRandomizer");
    }
};

injector::scoped_jmp WeaponRandomizer::mHooka98500{};
std::vector<int>     WeaponRandomizer::mValidWeapons;

WeaponRandomizer _weap;
