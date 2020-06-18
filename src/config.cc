#include <string>
#define CPPTOML_NO_RTTI

#include "config.hh"
#include "cpptoml/cpptoml.h"
#include <cstdlib>
#include "logger.hh"
#include "common.hh"
#include "configDefault.hh"
#include <sstream>

ConfigManager *ConfigManager::mInstance = nullptr;

#define CONFIG(table, member, key, type)                                       \
    this->member = table->get_as<type> (key).value_or (this->member);

/*******************************************************/
void
BaseConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    CONFIG (table, enabled, "Enabled", bool);
}

/*******************************************************/
void
GeneralConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    CONFIG (table, disableTugas, "DisableTugas", bool);
}

/*******************************************************/
void
MissionConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    CONFIG (table, enabled, "Enabled", bool);
    CONFIG (table, forcedMissionEnabled, "ForcedMissionEnabled", bool);
    CONFIG (table, forcedMissionID, "ForcedMissionID", std::string);
    CONFIG (table, forceSeed, "ForceSeed", bool);

    std::string seed = table->get_as<std::string> ("Seed").value_or ("");

    if (seed != "")
        this->seed = std::hash<std::string>{}(seed);
}

/*******************************************************/
void
TrafficConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    BaseConfig::Read (table);
    CONFIG (table, forcedVehicleEnabled, "ForcedVehicleEnabled", bool);
    CONFIG (table, forcedVehicleName, "ForcedVehicleName", std::string);
    CONFIG (table, enableBoats, "EnableBoats", bool);
    CONFIG (table, enableHelicopters, "EnableHelis", bool);
    CONFIG (table, enableRandomPeds, "EnableRandomPedsInCars", bool);
    CONFIG (table, enableCars, "EnableCars", bool);
    CONFIG (table, enableBikes, "EnableBikes", bool);
    CONFIG (table, defaultModel, "DefaultModel", int);
}

/*******************************************************/
void
ColourConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    BaseConfig::Read (table);
    CONFIG (table, hueCycle, "RainbowHueCycle", bool);
    CONFIG (table, hud, "RandomizeHUD", bool);
    CONFIG (table, carcols, "RandomizeCarCols", bool);
}

/*******************************************************/
void
WeaponStatsConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    CONFIG (table, enabled, "Enabled", bool);
}

/*******************************************************/
void
SoundsConfig::Read (std::shared_ptr<cpptoml::table> table)
{
    if (!table)
        return;

    CONFIG (table, enabled, "Enabled", bool);
    CONFIG (table, RandomizeVoiceLines, "RandomizeVoiceLines", bool);
    CONFIG (table, RandomizeGameSFX, "RandomizeGameSfx", bool);
    CONFIG (table, RandomizeGameMusic, "RandomizeGameMusic", bool);
}

/*******************************************************/
bool
DoesFileExist (const std::string &file)
{
    FILE *f = Rainbomizer::Common::GetRainbomizerFile (file, "r");
    if (f)
        {
            fclose (f);
            return true;
        }
    return false;
}

/*******************************************************/
void
ConfigManager::WriteDefaultConfig (const std::string &file)
{
    FILE *f = Rainbomizer::Common::GetRainbomizerFile (file.c_str (), "wb");
    if (f)
        {
            fwrite (configDefault, sizeof (configDefault) - 1, 1, f);
            fclose (f);
        }
}

/*******************************************************/
std::shared_ptr<cpptoml::table>
ConfigManager::ParseDefaultConfig ()
{
    // Read the default config file
    auto stream = std::istringstream (configDefault);

    cpptoml::parser p{stream};
    return p.parse ();
}

/*******************************************************/
ConfigManager::ConfigManager (const std::string &file)
{
    std::shared_ptr<cpptoml::table> config;
    config = ParseDefaultConfig ();
    try
        {
            config = cpptoml::parse_file (
                Rainbomizer::Common::GetRainbomizerFileName (file));
        }
    catch (const std::exception &e)
        {
            Rainbomizer::Logger::LogMessage (e.what ());

            if (!DoesFileExist (file))
                WriteDefaultConfig (file);
        }

    mConfigs.general.Read (config);
    mConfigs.traffic.Read (config->get_table ("TrafficRandomizer"));
    mConfigs.colours.Read (config->get_table ("ColourRandomizer"));
    mConfigs.cheat.Read (config->get_table ("CheatRandomizer"));
    mConfigs.weapon.Read (config->get_table ("WeaponRandomizer"));
    mConfigs.parkedCar.Read (config->get_table ("ParkedCarRandomizer"));
    mConfigs.sounds.Read (config->get_table ("SoundsRandomizer"));
    mConfigs.scriptVehicle.Read (config->get_table ("ScriptVehicleRandomizer"));
    mConfigs.missions.Read (config->get_table ("MissionRandomizer"));
    mConfigs.cutscenes.Read (config->get_table ("CutsceneRandomizer"));
    mConfigs.weaponStats.Read (config->get_table ("WeaponStatsRandomizer"));
}

/*******************************************************/
void
ConfigManager::DestroyInstance ()
{
    if (ConfigManager::mInstance)
        delete ConfigManager::mInstance;
}

/*******************************************************/
ConfigManager *
ConfigManager::GetInstance ()
{
    if (!ConfigManager::mInstance)
        {
            ConfigManager::mInstance = new ConfigManager ("config.toml");
            atexit (&ConfigManager::DestroyInstance);
        }
    return ConfigManager::mInstance;
}

/*******************************************************/
const Configs &
ConfigManager::GetConfigs ()
{
    return GetInstance ()->mConfigs;
}
