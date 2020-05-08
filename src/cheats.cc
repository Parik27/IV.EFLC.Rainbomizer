#include "CTheScripts.hh"
#include "Utils.hh"
#include "config.hh"
#include "logger.hh"
#include <vector>
#include <array>

class CheatRandomizer
{
    /*******************************************************/
    static void
    ActivateCheatHook (NativeData *data)
    {
        // Just in case someone in the future/past is crazy enough
        // to add custom implementation of cheats
        if (data->Params[0].int_param < 17)
            data->Params[0].int_param = RandomInt (16);

        CNativeManager::CallOriginalNative ("ACTIVATE_CHEAT", data);
    }

public:
    /*******************************************************/
    CheatRandomizer ()
    {
        if (!ConfigManager::GetConfigs ().cheat.enabled)
            return;

        InitialiseAllComponents ();

        CNativeManager::OverwriteNative ("ACTIVATE_CHEAT", ActivateCheatHook);
        Rainbomizer::Logger::LogMessage ("Initialised CheatsRandomizer");
    }
} _cheats;
