#include <string>

const char configDefault[] = R"(
########################################################

# Rainbomizer for IV/EFLC
# Copyright (C) 2019 - Parik

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.	If not, see <https://www.gnu.org/licenses/>.
#
#######################################################

# General Configuration

Enabled	  = true
DisableTugas = false

#######################################################
# Traffic Randomizer - Randomizes cars that spawn in traffic

[TrafficRandomizer]

Enabled = true

# This will make a specific vehicle spawn in traffic
ForcedVehicleEnabled = false
ForcedVehicleName = "admiral"

# Change these options to disable certain types of vehicles from spawning in traffic
EnableBoats = true
EnableCars = true
EnableHelis = true
EnableBikes = true

#######################################################
# Colour Randomizer - Randomizes the colours of various in-game elements, including cars, the HUD, and the markers. 
[ColourRandomizer]

Enabled = true
RainbowHueCycle = false # (All randomized elements will have transition between rainbow colours)
RandomizeHUD = true # Randomizes HUD elements and markers
RandomizeCarCols = true # Randomizes Car Colours

#######################################################
# Cheat Randomizer - Randomizes which cheat activates when you enter a cheat using a phone.
[CheatRandomizer]

Enabled = true

#######################################################
# Weapon Randomizer - Randomizes the weapons that are dropped/wielded by the player/enemies.
[WeaponRandomizer]

Enabled = true

#######################################################
# Parked Car Randomizer - Randomizes cars which are found parked around the map including airplanes at airports with entirely random spawns.
[ParkedCarRandomizer]

Enabled = true

#######################################################
# Mission Randomizer: Randomizes which mission is started by each mission marker
[MissionRandomizer]

Enabled = true

# Makes every misssion a specific mission. (Take mission number from main.scm)
ForcedMissionEnabled = false
ForcedMissionID = "roman1"

# https://en.wikipedia.org/wiki/Random_seed - useful for races
# If ForceSeed is set to false, the seed will not take effect on save files
Seed = ""
ForceSeed = false # Force the seed on existing save files

#######################################################
# Cutscene Randomizer - Randomizes the models used in motion-captured cutscenes
[CutsceneRandomizer]

Enabled = true

#######################################################
# Sounds Randomizer - Randomizes various elements of the game audio.
[SoundsRandomizer]

Enabled = true
RandomizeVoiceLines = true # dialogue spoken in missions
RandomizeGameSfx = true # Engine sounds, weapon sounds, etc.
RandomizeGameMusic = true # Intro, radio, interior music, other music.

#######################################################
# Script Vehicle Randomizer - Randomizes the vehicles you are given in missions.
[ScriptVehicleRandomizer]

Enabled = true
)";
