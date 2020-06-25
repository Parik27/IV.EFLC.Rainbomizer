#include "CTheScripts.hh"
#include "CCrypto.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "injector/hooking.hpp"
#include "CVehicleFactory.hh"
#include "CStreaming.hh"
#include "config.hh"
#include "logger.hh"
#include "common.hh"
#include "CModelInfoStore.hh"
#include <chrono>
#include <unordered_map>
#include <gtaThread.hh>
#include <array>
#include <bitset>

int (*scanf_aca2e7) (char *, char *, ...);

struct VehiclePattern
{
    std::string thread;
    uint32_t    vehicleHash;

    std::vector<int> validIndices;

    enum FLAGS
    {
        GUNS         = 1,
        WASHABLE     = 2,
        START_ROTORS = 4,
        NO_ROTORS    = 8
    };
    uint8_t flags;

    std::vector<bool> moveConditions;
    Vector3           moveCoords;
};

class ScriptVehicleRandomizer
{
    static std::unordered_map<int, int> mSeatsCache;
    static std::vector<VehiclePattern>  mVehiclePatterns;

    /*******************************************************/
    static uint32_t
    GetVehicleForPattern (const VehiclePattern &pattern)
    {
        if (pattern.validIndices.size () > 0)
            return pattern
                .validIndices[RandomInt (pattern.validIndices.size () - 1)];
        return -1;
    }

    /*******************************************************/
    static uint32_t
    GetVehicleWithSeatCheck (uint32_t hash)
    {
        auto indices = Rainbomizer::Common::GetVehicleIndices ();
        std::vector<uint32_t> validVehicles;

        int index;
        CStreaming::GetModelAndIndexFromHash (hash, &index);

        int seats = mSeatsCache.at (index);

        for (auto i : indices)
            {
                if (mSeatsCache.at (i) >= seats)
                    validVehicles.push_back (
                        CModelInfoStore::m_aModelPointers[i]->m_nModelHash);
            }

        return validVehicles[RandomInt (validVehicles.size () - 1)];
    }

    /*******************************************************/
    static uint32_t
    GetVehicleForModel (uint32_t hash, uint8_t &flagsOut, Vector3 &move)
    {
        for (const auto &i : mVehiclePatterns)
            {
                if (i.vehicleHash != hash)
                    continue;

                if (i.thread
                    != CTheScripts::m_pRunningThread ()->m_szProgramName)
                    continue;

                int vehicle = GetVehicleForPattern (i);
                if (vehicle == -1)
                    {
                        Rainbomizer::Logger::LogMessage (
                            "No matching vehicle found for pattern: %d",
                            &i - &mVehiclePatterns[0]);
                        continue;
                    }

                auto model = static_cast<CVehicleModelInfo *> (
                    CModelInfoStore::m_aModelPointers[vehicle]);

                flagsOut = i.flags;

                if (i.moveConditions[model->m_nType])
                    move = i.moveCoords;

                return model->m_nModelHash;
            }

        return GetVehicleWithSeatCheck (hash);
    }

    /*******************************************************/
    static void
    HandleCreateCarFlags (uint8_t flags, uint32_t handle)
    {
        /* This flag removes both the rotors (tail and main) for a helicopter.
           These are components (extras) 1 and 3 (Tested by Fry) */

        if (flags & VehiclePattern::NO_ROTORS)
            for (auto i : {1, 3})
                CNativeManager::CallNative ("TURN_OFF_VEHICLE_EXTRA", handle, i,
                                            true);

        /* This flag starts the rotors immediately after a helicopter is
         * spawned. */

        if (flags & VehiclePattern::START_ROTORS)
            CNativeManager::CallNative ("SET_HELI_BLADES_FULL_SPEED", handle);
    }

    /*******************************************************/
    static void
    CreateCarNativeHook (NativeData *data)
    {

        static int number       = 0;
        uint32_t   originalHash = data->Params[0].uint_param;
        uint8_t    flags        = 0;
        Vector3    moveCoords   = {0, 0, 0};

        data->Params[0].uint_param
            = GetVehicleForModel (originalHash, flags, moveCoords);

        // Move the vehicle to an alternative coordinate
        data->Params[1].float_param += moveCoords.x;
        data->Params[2].float_param += moveCoords.y;
        data->Params[3].float_param += moveCoords.z;

        // This does stuff related to unloading and loading the old/new vehicles
        // CNativeManager::CallNative ("MARK_MODEL_AS_NO_LONGER_NEEDED",
        // data->Params[0].uint_param);

        // TODO: Replace this with random loaded vehicle once the exe is more
        // reverse engineered.
        // Fallback for when the model couldn't get loaded (Streaming limits)
        if (!CStreaming::AttemptToLoadModel (data->Params[0].uint_param, 1,
                                             true))
            {
                Rainbomizer::Logger::LogMessage (
                    "Too many vehicles loaded, returning the original vehicle");

                data->Params[0].uint_param = originalHash;
            }

        CNativeManager::CallOriginalNative ("CREATE_CAR", data);
        HandleCreateCarFlags (flags, *data->GetParam<uint32_t *> (4));
    }

    /*******************************************************/
    static void
    InitialiseCarSeatsCache (int)
    {
        const auto &indices   = Rainbomizer::Common::GetVehicleIndices ();
        auto        timestamp = clock ();

        for (const auto &i : indices)
            {
                const auto model
                    = CModelInfoStore::GetModelInfoPointer<CVehicleModelInfo> (
                        i);

                if (CStreaming::AttemptToLoadModel (model->m_nModelHash))
                    mSeatsCache[i]
                        = CModelInfoStore::GetMaximumNumberOfPassengers (i) + 1;
                else
                    Rainbomizer::Logger::LogMessage (
                        "Failed to initialise seat count for vehicle: %s",
                        model->m_szGameName);

                CStreaming::FreeWdrModel (i);
            }

        Rainbomizer::Logger::LogMessage (
            "Initialised Seat Count (%d/%d) cache in %.2f seconds",
            mSeatsCache.size (), indices.size (),
            1.0f * (clock () - timestamp) / CLOCKS_PER_SEC);
    }

    /*******************************************************/
    static uint8_t
    ReadFlags (char *flags)
    {
        uint8_t     flags_out = 0;
        const char *flag      = strtok (flags, " +");

        while (flag != NULL)
            {
                if (std::string (flags) == "guns")
                    flags_out |= VehiclePattern::FLAGS::GUNS;
                else if (std::string (flags) == "washable")
                    flags_out |= VehiclePattern::FLAGS::WASHABLE;
                else if (std::string (flags) == "no_rotors")
                    flags_out |= VehiclePattern::FLAGS::NO_ROTORS;
                else if (std::string (flags) == "start_rotors")
                    flags_out |= VehiclePattern::FLAGS::START_ROTORS;

                flag = strtok (NULL, "+");
            }

        return flags_out;
    }

    /*******************************************************/
    static bool
    IsVehicleWashable (CVehicleModelInfo *model)
    {
        if (model->m_nType != VEHICLE_TYPE_CAR)
            return false;

        // Source: sub_8708 (carwash.sc)
        static constexpr std::array<uint32_t, 51> mNotAllowed{
            0x45d56ada, 0x7a61b330, 0x32b91ae8, 0x92e56a2c, 0x898eccea,
            0xd577c962, 0xc6c3242d, 0xc6c3242d, 0x73920f8e, 0x73920f8e,
            0x50b0215a, 0x58e49664, 0x35ed670b, 0x47b9138a, 0x21eee87d,
            0x809aa4cb, 0x9f05f101, 0x68e27cb6, 0x63ffe6ec, 0x6827cf72,
            0x2fbc4d30, 0x17df5ec2, 0xc9e8ff76, 0xc9e8ff76, 0xbe6ff06a,
            0x9d0450ca, 0x1517d4d9, 0x8b13f083, 0x5d0aac8f, 0x432ea949,
            0x31f0b376, 0x3d961290, 0x432ea949, 0x79fbb0c5, 0x33581161,
            0x22c16a2f, 0x8de2a8b,  0x71ef6313, 0xeb221fc2, 0xe2e7d4ab,
            0x8eb78f5a, 0xcd935ef9, 0x63ffe6ec, 0x72435a19, 0x3f724e66,
            0xafbb2ca4, 0xf8de29a8, 0xcfb3870c, 0xc1ce1183, 0x2ef89e46,
            0x9229e4eb};

        return std::find (mNotAllowed.begin (), mNotAllowed.end (),
                          model->m_nModelHash)
               == mNotAllowed.end ();
    }

    /*******************************************************/
    static bool
    DoesVehicleHaveGuns (CVehicleModelInfo *model)
    {
        constexpr std::array mGunVehicles{"apc"_joaat, "buzzard"_joaat,
                                          "annihilator"_joaat};

        return std::find (mGunVehicles.begin (), mGunVehicles.end (),
                          model->m_nModelHash)
               != mGunVehicles.end ();
    }

    /*******************************************************/
    static std::vector<int>
    GenerateValidIndicesForPattern (uint32_t hash, int seats, bool cars,
                                    bool bikes, bool helis, bool boats,
                                    bool trains, uint8_t flags)
    {
        std::vector<int> out;
        {
            int index = 0;
            CStreaming::GetModelAndIndexFromHash (hash, &index);
            if (seats == -1)
                seats = mSeatsCache[index];

            out.push_back (index);
        }

        auto indices = Rainbomizer::Common::GetVehicleIndices ();
        for (auto i : indices)
            {
                auto model
                    = CModelInfoStore::GetModelInfoPointer<CVehicleModelInfo> (
                        i);

                if (model->m_nType == VEHICLE_TYPE_CAR && !cars)
                    continue;
                if (model->m_nType == VEHICLE_TYPE_BIKE && !bikes)
                    continue;
                if (model->m_nType == VEHICLE_TYPE_BOAT && !boats)
                    continue;
                if (model->m_nType == VEHICLE_TYPE_TRAIN && !trains)
                    continue;
                if (model->m_nType == VEHICLE_TYPE_HELI && !helis)
                    continue;
                if (flags & VehiclePattern::FLAGS::GUNS
                    && !DoesVehicleHaveGuns (model))
                    continue;
                if (flags & VehiclePattern::FLAGS::WASHABLE
                    && !IsVehicleWashable (model))
                    continue;
                if (mSeatsCache.at (i) < seats)
                    continue;
                if (model->m_nModelHash == hash)
                    continue;

                out.push_back (i);
            }
        return out;
    }

    /*******************************************************/
    static void
    ReadScriptPatterns (FILE *file)
    {
        if (!file)
            return;

        char line[2048] = {0};
        while (fgets (line, 2048, file))
            {
                if (line[0] == '#' || strlen (line) < 10)
                    continue;

                char    threadName[64]  = {0};
                char    vehicleName[64] = {0};
                int     seats           = 0;
                char    cars            = 'N';
                char    bikes           = 'N';
                char    helis           = 'N';
                char    boats           = 'N';
                char    trains          = 'N';
                char    flags[256]      = {0};
                Vector3 move            = {0, 0, 0};

                sscanf (line, "%s %s %d %c %c %c %c %c %s %f %f %f", threadName,
                        vehicleName, &seats, &cars, &bikes, &helis, &boats,
                        &trains, flags, &move.x, &move.y, &move.z);

                auto hash     = CCrypto::atStringHash (vehicleName);
                auto flagsInt = ReadFlags (flags);

                mVehiclePatterns.push_back (
                    {threadName,
                     hash,
                     GenerateValidIndicesForPattern (hash, seats, cars != 'N',
                                                     bikes != 'N', helis != 'N',
                                                     boats != 'N',
                                                     trains != 'N', flagsInt),
                     flagsInt,

                     {cars == 'C', bikes == 'C', boats == 'C', trains == 'C',
                      helis == 'C', false},

                     move});
            }
    }

    /*******************************************************/
    static void
    InitialiseScriptPatterns (int episode)
    {
        InitialiseCarSeatsCache (episode);
        ReadScriptPatterns (Rainbomizer::Common::GetRainbomizerDataFile (
            "VehiclePatterns.txt"));
    }

public:
    /*******************************************************/
    ScriptVehicleRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().scriptVehicle.enabled)
            return;

        InitialiseAllComponents ();

        Rainbomizer::Common::AddEpisodeChangeCallback (
            InitialiseScriptPatterns);

        CNativeManager::OverwriteNative ("CREATE_CAR", CreateCarNativeHook);
        Rainbomizer::Logger::LogMessage ("Initialised ScriptVehicleRandomizer");
    }
};

std::unordered_map<int, int> ScriptVehicleRandomizer::mSeatsCache{};
std::vector<VehiclePattern>  ScriptVehicleRandomizer::mVehiclePatterns;

ScriptVehicleRandomizer _scr;
