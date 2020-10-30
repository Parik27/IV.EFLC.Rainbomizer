![Build](https://github.com/Parik27/Rainbomizer/workflows/Build/badge.svg)
[![Discord](https://img.shields.io/discord/681996979974570066.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/BYVBQw7)

# Rainbomizer: IV/EFLC 🌈

<a href="https://media.discordapp.net/attachments/681997133351747585/682871480048025644/unknown.png"> <img src="https://media.discordapp.net/attachments/681997133351747585/682871480048025644/unknown.png?width=912&height=513"></a>

A modification for Grand Theft Auto IV and Episodes from Liberty City that randomizes various aspects of the game from vehicles and sounds to missions and weapons.
<hr>
<p align="center">
  <a href="http://rainbomizer.com/"><strong>homepage</strong></a> • 
  <a href="https://github.com/Parik27/IV.EFLC.Rainbomizer/releases"><strong>download</strong></a> • 
  <a href="https://github.com/Parik27/IV.EFLC.Rainbomizer/issues"><strong>issues</strong></a> • 
  <a href="#installation"><strong>installation</strong></a> • 
  <a href="#features"><strong>features</strong></a> • 
  <a href="#contribute"><strong>contribute</strong></a> 
</p>
<hr>

<span id="features"></span>
## Features

- :helicopter: **Traffic Randomizer** - Randomizes cars that spawn in traffic. 
- :car: **Parked Car Randomizer** - Randomizes cars parked around the map.
- :blue_car: **Mission Vehicle Randomizer** - Randomizes vehicles you are given in missions, making sure that the vehicle you're given is usable for that mission.
- :rainbow: **Colour Randomizer** - Randomizes colours of various in-game elements, including cars, the HUD and more. 
- :page_with_curl: **Mission Randomizer** - Randomizes order of missions in-game. Upon completion of a mission, the game progresses as if you completed the original mission.
- :gun: **Weapon Randomizer** - Randomizes weapons given to the player and enemies.
- :sound: **Sounds Randomizer** - Randomizes dialogues, sound effects and music played in-game. The intensity of this randomizer can be modified in the configuration file. 
- :movie_camera: **Cutscene Randomizer** - Randomizes models used in motion-captured cutscenes.
- :iphone: **Cheat Randomizer** - Randomizes which cheats activates when you enter a cheat.
- :bird: **Object Randomizer** - Randomizes random vehicles into random objects.

<span id="installation"></span>
## Installation

Rainbomizer requires an **ASI-Loader** to function. It works with **all** versions of the game, but only the latest (Steam), 1.0.4.0, 1.1.2.0, 1.0.7.0, and 1.0.8.0 versions have been tested.

1. Install **one** of the listed ASI Loaders: 
    - [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/download/v4.52/Ultimate-ASI-Loader.zip) by [ThirteenAG](https://github.com/ThirteenAG) is a popular option since it supports CrashDumps, which will be necessary if you want to report a crash.
    - [xliveless](https://gtaforums.com/topic/388658-relgtaiv-xliveless) (1.0.8.0 and below only) - required for versions before Complete Edition. xliveless removes requirement for GFWL and also includes an ASI-Loader

2. Download Rainbomizer from [Releases](https://github.com/Parik27/IV.EFLC.Rainbomizer/releases) and **extract the archive** to the root directory of GTA IV. 
  
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
    
## Configuration

If you wish to change any of the mod's functionalities, for example to disable a specific randomizer, a configuration file (`config.toml`) has been provided with the mod. The config file is located in the `rainbomizer` folder in the game's root directory.

The default configuration file is in the main repository, `config.toml`, and is automatically created by the mod if it doesn't exist in the rainbomizer folder.

The configuration file is in the TOML format. It is similar in structure to INI's, but with a defined and better structure.

To configure a specific randomizer, search for its name in the configuration file. You can set the "Enabled" key to 'true' or 'false' to enable or disable it respectively.

More information about how to configure specific randomizers is provided in the default config file.

## Credits 🌈

#### Lead Developers

- [Parik](https://github.com/Parik27) - Creation and implementation of the ASI.

#### Major Contributors

- [123robot](https://www.twitch.tv/123robot) - Creating external files, gathering data, providing ideas, lots of testing, and general support throughout development.
- [Fryterp23](https://www.twitch.tv/fryterp23) - Creating episodes external files, lots of testing, and providing general support and ideas throughout development.

#### Additional Contributors

- [hoxi](https://www.twitch.tv/hoxi___) - Beta testing and help with patterns early in development.
- [Powdinet](https://www.twitch.tv/powdinet) - Help with implementation of the Mission Randomizer.

#### Beta Testers
- [SpeedyFolf](https://www.twitch.tv/speedyfolf)
- [Hugo_One](https://www.twitch.tv/hugo_one)

<span id="contribute"></span>
## Contribution

You can contribute to the project by reporting any crashes/issues you encounter in the [Issues](https://github.com/Parik27/Rainbomizer/issues) section of the repository.

Pull requests are welcome, but are required to follow the current style used in the project.

You can also keep up with or contribute to development as it happens by [joining the official Rainbomizer Discord!](https://discord.gg/BYVBQw7) You can give #suggestions, ask for #support, or view current development in #iv-eflc.

The lib folder in the repository's base is intended for reuse in other projects, and should not contain any Rainbomizer specific code.
