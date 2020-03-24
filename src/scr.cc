#include "CScrVM.hh"
#include "CCrypto.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "injector/hooking.hpp"
#include "CVehicleFactory.hh"
#include "CStreaming.hh"
#include "config.hh"
#include "logger.hh"

int (*scanf_aca2e7) (char *, char *, ...);

struct Vehicle
{
    std::string name;
    std::string type;
};

class ScriptVehicleRandomizer
{

    static std::vector<Vehicle>  mVehicles;
    static std::vector<uint32_t> mBoats;
    static std::vector<uint32_t> mHelis;
    static injector::scoped_jmp  mHookCreateCar;

    /*******************************************************/
    static uint32_t
    GetVehicleForModel (uint32_t hash)
    {
        for (auto i : mBoats)
            if (i == hash)
                return mBoats[RandomInt (mBoats.size () - 1)];
        for (auto i : mHelis)
            if (i == hash)
                return mHelis[RandomInt (mHelis.size () - 1)];

        return CCrypto::HashString (
            mVehicles[RandomInt (mVehicles.size () - 1)].name.c_str ());
    }

    /*******************************************************/
    static void
    CreateCarNativeHook (NativeData *data)
    {

        static int number = 0;
        // Vehicle    car             = mVehicles[RandomInt(mVehicles.size () -
        // 1)];
        uint32_t originalHash = data->Params[0].uint_param;

        data->Params[0].uint_param = GetVehicleForModel (
            originalHash); // CCrypto::HashString (car.name.c_str ());

        // This does stuff related to unloading and loading the old/new vehicles
        // CNativeManager::CallNative ("MARK_MODEL_AS_NO_LONGER_NEEDED",
        // data->Params[0].uint_param);

        // TODO: Replace this with random loaded vehicle once the exe is more
        // reverse engineered.
        // Fallback for when the model couldn't get loaded (Streaming limits)
        if (!CStreaming::AttemptToLoadModel (data->Params[0].uint_param))
            data->Params[0].uint_param = originalHash;

        CNativeManager::CallOriginalNative ("CREATE_CAR", data);
    }

    /*******************************************************/
    static int
    CarListHook (char *src, char *format, const char *model, const char *txd,
                 const char *type, const char *handling, const char *gameName,
                 const char *anims, const char *anims2, int *frq, int *maxNum,
                 float *f1, float *f2, float *f3, int *i1, float *f4, int *i2,
                 const char *flags)
    {
        int count = scanf_aca2e7 (src, format, model, txd, type, handling,
                                  gameName, anims, anims2, frq, maxNum, f1, f2,
                                  f3, i1, f4, i2, flags);
        mVehicles.push_back ({model, type});
        if (std::string (type) == "boat")
            mBoats.push_back (CCrypto::HashStringLowercase (model));
        else if (std::string (type) == "heli")
            mHelis.push_back (CCrypto::HashStringLowercase (model));

        return count;
    }

    /*******************************************************/
    static CVehicle *__fastcall CreateVehicleHook (CVehicleFactory *factory,
                                                   void *edx, int index,
                                                   int param3, int param4,
                                                   int param5)
    {
        mHookCreateCar.restore ();

        CVehicle *veh = factory->CreateCar (index, param3, param4, param5);

        InitialiseCreateVehicleHook ();

        return veh;
    }

    /*******************************************************/
    static void
    InitialiseCreateVehicleHook ()
    {
        static void *addr
            = hook::get_pattern ("55 57 8b 7c ? ? 8b 04 ? ? ? ? ? 85 c0 8b e9");

        mHookCreateCar.make_jmp (addr, CreateVehicleHook);
    }

public:
    /*******************************************************/
    ScriptVehicleRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().scriptVehicle.enabled)
            return;

        InitialiseAllComponents ();

        RegisterHook ("8d 84 ? e4 00 00 00 50 68 ? ? ? ? 56 e8", 14,
                      scanf_aca2e7, CarListHook);

        InitialiseCreateVehicleHook ();

        CNativeManager::OverwriteNative ("CREATE_CAR", CreateCarNativeHook);
        Rainbomizer::Logger::LogMessage ("Initialised ScriptVehicleRandomizer");
    }
};

std::vector<Vehicle>  ScriptVehicleRandomizer::mVehicles;
std::vector<uint32_t> ScriptVehicleRandomizer::mHelis;
std::vector<uint32_t> ScriptVehicleRandomizer::mBoats;
injector::scoped_jmp  ScriptVehicleRandomizer::mHookCreateCar{};

ScriptVehicleRandomizer _scr;
