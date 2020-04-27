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
#include "CTheScripts.hh"
#include "CStreaming.hh"
#include "CCrypto.hh"
#include "common.hh"
#include "config.hh"
#include "logger.hh"
#include <CPlayer.hh>
#include <random>

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
    static std::vector<int>                mValidWeapons;
    static std::discrete_distribution<int> mDistribution;

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
    static void
    InitialiseWeaponWeights ()
    {
        FILE *file
            = Rainbomizer::Common::GetRainbomizerDataFile ("WeaponWeights.txt");
        std::unordered_map<uint32_t, double> probabilities;

        char line[256] = {0};

        double sum_probability     = 0.0;
        int    total_probabilities = 0;

        if (file)
            {
                while (fgets (line, 256, file))
                    {
                        if (line[0] == '#' || strlen (line) < 5)
                            continue;

                        char   model[128]  = {0};
                        double probability = 0.0;
                        if (sscanf (line, "%s = %lf", model, &probability) == 2)
                            {
                                probabilities[CCrypto::HashStringLowercase (
                                    model)]
                                    = probability;
                                sum_probability += probability;
                                total_probabilities++;
                            }
                    }
            }

        double mean_probability
            = total_probabilities ? sum_probability / total_probabilities : 1;

        std::vector<double> weights;
        for (auto i : mValidWeapons)
            {
                auto info = CWeaponInfo::GetInfoFromIndex (i);
                if (probabilities.count (info->m_nWeaponModelHash))
                    weights.push_back (probabilities[info->m_nWeaponModelHash]);
                else
                    weights.push_back (mean_probability);
            }

        mDistribution
            = std::discrete_distribution<int>{weights.begin (), weights.end ()};
    }

    /*******************************************************/
    static void
    InitialiseWeaponsArray ()
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

        InitialiseWeaponWeights ();
        Rainbomizer::Logger::LogMessage ("Initialised %d valid weapons",
                                         mValidWeapons.size ());
    }

    /*******************************************************/
    static int
    GetNewWeaponForWeapon (int weapon, bool player, std::mt19937 &engine)
    {
        if (CTheScripts::m_pRunningThread () && player
            && CTheScripts::m_pRunningThread ()->m_szProgramName
                   == std::string ("yusuf3")
            && CWeaponInfo::GetInfoFromIndex (weapon)->m_nWeaponModelHash
                   == CCrypto::HashStringLowercase ("w_e2_dsr1"))
            return weapon;

        return mValidWeapons[mDistribution (engine)];
    }
    
    /*******************************************************/
    static int
    GetNewWeaponForWeapon (int weapon, CPedWeapons *weapons)
    {
        thread_local static std::mt19937 engine{(unsigned int) time (NULL)};
        bool isPlayer = &CPlayer::FindPlayerPed ()->m_pWeapons () == weapons;

        // Don't randomize the weapon if it's the player's weapon and it wasn't
        // a thread that gave it to the player
        if (!CTheScripts::m_pRunningThread () && isPlayer)
            return weapon;

        if (std::find (mValidWeapons.begin (), mValidWeapons.end (), weapon)
            != mValidWeapons.end ())
            return GetNewWeaponForWeapon(weapon, isPlayer, engine); //

        return weapon;
    }

    /*******************************************************/
    static void __fastcall RandomizeWeapon (CPedWeapons *weapons, void *edx,
                                            int weapon, int ammo, char param4,
                                            char param5, bool shown)
    {
        mHooka98500.restore ();

        auto newWeapon = GetNewWeaponForWeapon (weapon, weapons);
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

        Rainbomizer::Common::AddEpisodeChangeCallback ([] (int) {
            mValidWeapons.clear ();
            InitialiseWeaponsArray ();
        });

        Rainbomizer::Logger::LogMessage ("Initialised WeaponRandomizer");
    }
};

injector::scoped_jmp WeaponRandomizer::mHooka98500{};
std::vector<int>     WeaponRandomizer::mValidWeapons;
std::discrete_distribution<int> WeaponRandomizer::mDistribution;

WeaponRandomizer _weap;
