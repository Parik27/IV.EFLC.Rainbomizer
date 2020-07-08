#pragma once

#include <string>
#include <memory>
#include <vector>

namespace cpptoml {
class table;
} // namespace cpptoml

/*******************************************************/
struct BaseConfig
{
    bool enabled = true;

    void Read (std::shared_ptr<cpptoml::table> table);
};

/*******************************************************/
struct GeneralConfig : public BaseConfig
{
    bool disableTugas = false;

    void Read (std::shared_ptr<cpptoml::table> table);
};

/*******************************************************/
struct TrafficConfig : public BaseConfig
{

    bool        forcedVehicleEnabled = false;
    std::string forcedVehicleName    = "";

    bool enableBoats       = true;
    bool enableHelicopters = true;
    bool enableCars        = true;
    bool enableBikes       = true;

    bool enableRandomPeds = false;

    int defaultModel = 0;

    void Read (std::shared_ptr<cpptoml::table> table);
};

/*******************************************************/
struct ColourConfig : public BaseConfig
{
    bool hueCycle = true;
    bool carcols  = true;
    bool hud      = false;

    void Read (std::shared_ptr<cpptoml::table> table);
};

/*******************************************************/
struct CheatConfig : public BaseConfig
{
};

/*******************************************************/
struct WeaponConfig : public BaseConfig
{
};

/*******************************************************/
struct ParkedCarConfig : public BaseConfig
{
};

/*******************************************************/
struct SoundsConfig : public BaseConfig
{
    bool RandomizeVoiceLines = true;
    bool RandomizeGameSFX    = true;
    bool RandomizeGameMusic  = true;

    void Read (std::shared_ptr<cpptoml::table> table);
};

/*******************************************************/
struct ScriptVehicleConfig : public BaseConfig
{
};

/*******************************************************/
struct MissionConfig : public BaseConfig
{
    bool        enabled              = false;
    bool        forcedMissionEnabled = false;
    std::string forcedMissionID      = "";
    int         seed                 = -1;
    bool        forceSeed            = false;

    void Read (std::shared_ptr<cpptoml::table> table);
};

/*******************************************************/
struct CutsceneConfig : public BaseConfig
{
};

/******************************************************/
struct WeaponStatsConfig : public BaseConfig
{
    bool enabled = false;
    void Read (std::shared_ptr<cpptoml::table> table);
};

struct ObjectsConfig : public BaseConfig
{
};

/*******************************************************/
struct Configs
{
    GeneralConfig       general;
    TrafficConfig       traffic;
    ColourConfig        colours;
    CheatConfig         cheat;
    WeaponConfig        weapon;
    ParkedCarConfig     parkedCar;
    SoundsConfig        sounds;
    ScriptVehicleConfig scriptVehicle;
    MissionConfig       missions;
    CutsceneConfig      cutscenes;
    WeaponStatsConfig   weaponStats;
    ObjectsConfig       objects;
};

/*******************************************************/
class ConfigManager
{
    static ConfigManager *mInstance;

    Configs mConfigs;

    ConfigManager (){};
    static void DestroyInstance ();

    void WriteDefaultConfig (const std::string &file);
    std::shared_ptr<cpptoml::table> ParseDefaultConfig ();

public:
    /// Returns the static instance for ConfigManager.
    static ConfigManager *GetInstance ();

    /// Initialises
    ConfigManager (const std::string &file = "config.toml");

    /// Gets the config
    static const Configs &GetConfigs ();
};
