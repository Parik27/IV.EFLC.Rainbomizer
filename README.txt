Rainbomizer: IV/EFLC

A modification for Grand Theft Auto IV and Episodes from Liberty City that randomizes various aspects of the game from vehicles and sounds to missions and weapons.

Features

- Traffic Randomizer - Randomizes cars that spawn in traffic. 
- Parked Car Randomizer - Randomizes cars parked around the map.
- Mission Vehicle Randomizer - Randomizes vehicles you are given in missions, making sure that the vehicle you're given is usable for that mission.
- Colour Randomizer - Randomizes colours of various in-game elements, including cars, the HUD and more. 
- Mission Randomizer - Randomizes order of missions in-game. Upon completion of a mission, the game progresses as if you completed the original mission.
- Weapon Randomizer - Randomizes weapons given to the player and enemies.
- Sounds Randomizer - Randomizes dialogues, sound effects and music played in-game. The intensity of this randomizer can be modified in the configuration file. 
- Cutscene Randomizer - Randomizes models used in motion-captured cutscenes.
- Cheat Randomizer - Randomizes which cheats activates when you enter a cheat.
- Object Randomizer - Randomizes random vehicles into random objects.

Installation

Rainbomizer requires an ASI-Loader to function. It works with all versions of the game, but only the latest (Steam), 1.0.4.0, 1.1.2.0, 1.0.7.0, and 1.0.8.0 versions have been tested.

1. Install one of the listed ASI Loaders: 
    - Ultimate ASI Loader by ThirteenAG is a popular option since it supports CrashDumps, which will be necessary if you want to report a crash.
    - xliveless (1.0.8.0 and below only) - required for versions before Complete Edition. xliveless removes requirement for GFWL and also includes an ASI-Loader

2. Extract the archive to the root directory of GTA IV. 
  
   The directory structure should look something like this:
    ```
      GTAIV.exe
      IV.Rainbomizer.asi
      rainbomizer/
          config.toml
          data/
              ...
      dinput8.dll (or xlive.dll for 1.0.8.0 and below)
    ```
    
Configuration

If you wish to change any of the mod's functionalities, for example to disable a specific randomizer, a configuration file (`config.toml`) has been provided with the mod. The config file is located in the `rainbomizer` folder in the game's root directory.

The default configuration file is in the main repository, `config.toml`, and is automatically created by the mod if it doesn't exist in the rainbomizer folder.

The configuration file is in the TOML format. It is similar in structure to INI's, but with a defined and better structure.

To configure a specific randomizer, search for its name in the configuration file. You can set the "Enabled" key to 'true' or 'false' to enable or disable it respectively.

More information about how to configure specific randomizers is provided in the default config file.

Credits 🌈

#### Lead Developers

- Parik - Creation and implementation of the ASI.

#### Major Contributors

- 123robot - Creating external files, gathering data, providing ideas, lots of testing, and general support throughout development.
- Fryterp23 - Creating episodes external files, lots of testing, and providing general support and ideas throughout development.

#### Additional Contributors

- hoxi - Beta testing and help with patterns early in development.
- Powdinet - Help with implementation of the Mission Randomizer.

#### Beta Testers
- SpeedyFolf
- Hugo_One
