# Valkyrie

Project: Valkyrie is a LAN multiplayer third-person shooter built with Unreal Engine 5.7.

The game currently has two multiplayer maps: a Shooting Range and a 3v3 Team Deathmatch map.

## Features

### Character

- Third-person camera, movement, character rotation, locomotion, jumping, and in-air animation states.
- Upper-body Aim Offset follows the player's view pitch.
- Reusable Health, Weapon, and Interaction components.

### Gameplay Ability System

- Character-owned Ability System Component.
- GAS abilities for Aim, Fire, Reload, Switch Weapon, Dash, and Throw Grenade.
- Ability Tasks manage montages, delays, Root Motion, and input release.

### Weapons And Combat Feedback

- Primary rifle and secondary pistol with hitscan firing, ammunition, damage, fire-rate, and reload mechanics.
- Movement, Aim, and sustained fire affect weapon and crosshair spread.
- Synchronized muzzle flashes, fire audio, and bullet-trail VFX.
- Hits trigger blood mist, Hit React montages, attacker Hit Markers and audio, and directional damage indicators.

### Game Modes And UI

- Shooting Range with damageable targets that reset automatically.
- 3v3 Team Deathmatch with team assignment, spawning, death, respawning, scoring, a Tab scoreboard, and match completion.
- HUD displays health, current weapon name, ammunition, reload state, dynamic crosshair, and team scores.

### Multiplayer Flow

- Main menu, map selection, and pre-match lobby (player list, automatic team balancing, and host-controlled match start).


## Third-Party Assets

Valkyrie uses the following assets for models, animations, audio, VFX, and UI.

- Unreal Engine Third Person Template by Epic Games: Manny character, animations, and environment assets.
- [Weapons FREE - Low Poly 3D Models Pack](https://www.fab.com/listings/9939041c-a505-47f5-a636-ee51381787ea) by ithappy: weapon models. Licensed under the [Fab Standard License](https://www.fab.com/eula).
- [Free Weapon Sound Effects](https://www.fab.com/listings/1697af22-7e2a-410e-b8c0-88239216520d) by SoundLab1: weapon audio. Licensed under the [Fab Standard License](https://www.fab.com/eula).
- [Easy Crosshair - Free Pack](https://www.fab.com/listings/de40757a-69f1-4d2d-87e7-141d3bfca085) by Dodge Theory: crosshair and Hit Marker textures. Licensed under the [Fab Standard License](https://www.fab.com/eula).
- [Free Muzzle Flash Texture Pack](https://www.fab.com/listings/d5afc31a-5c7a-4b37-90e4-f4881e4940d6) by kolpakov vfx: muzzle flash materials and textures. Licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/).
- [Low Poly Shooter Pack v6.0](https://www.fab.com/listings/90ba076a-dc9a-4782-9ac8-dc2ed4f06405) by Infima Games: weapon models, character and weapon animations, VFX, and reference material. Licensed under the [Fab Standard License](https://www.fab.com/eula).

Third-party Fab content under `Content/Packs` is excluded from this repository and must be obtained separately under its respective license.
