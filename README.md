# MacrosV2
Xiris Bot for use on EQEMU servers (EQTITAN primarily)
---
## Setup
### Requirements

#### Macroquest2
Xiris bot recommends the ROF2 EMU build of MQ2 from MMOBugs. As of Spring 2022 it was decided that this would be free, and no longer require a mmobugs.com subcscription. To run the build you will need to go to [MMOBugs](https://www.mmobugs.com/) and create an account. Once the account is created, go to the forums, and find the EverQuest Server Emulator section. In this section there is a thread: [MMOBugs MQ2 Installation for Emulator Clients - Download Links and Instructions](https://www.mmobugs.com/forums/index.php?threads/mmobugs-mq2-installation-for-emulator-clients-download-links-and-instructions.29682/) At the top of the thread you will see download links, and the first 1) is the ROF2 build with the lastest MQ2 codebase. Grab the setup version, and install wherever you want.

##### Plugins
XirisBot requires several plugins that are not standard to the E3 builds you are used to. These are more modern plugins than what were generally avaialable. These are powerful and important plugins that will greatly enhance the experience once you become used to them
###### MQ2Dannet
DanNet was created by Dannuic, one of the main contributers to MQ2 and MQNext. This is a replacement for EQBC that provides direct access to any toon from any other toon via queries and observers. The documentation is found at [MQ2Dan on Github](https://github.com/dannuic/MQ2Dan) While it is not required that you know the details of this plugin, it will be helpful to learn the command syntax since it totally replaces EQBC - specifically the 1:1 replacements for common commands in EQBC. Forexample if in EQBC you'd use `/bcaa` instead you'd use `/dga` and instead of `/bct` you'd use `/dt`  Once you get used to it, it works the same.
###### MQ2Nav
MQ2Nav is a game-changing quality of life improvement. Instead of having your toons `/stick` to you, you can have them follow you using MQ2Nav that *automatically generates paths* to the target (you) or you could tell your entire raid to navigate to a target or predefined (by you) waypoint. [MQ2Nav on Github](https://github.com/brainiac/MQ2Nav/wiki) The wiki link will give detail usage. Xiris bot has several built in commands that require MQ2Nav to function `
#### Everquest Build

### Loading
#### bot_loader.mac
#### MQ2Autologin
#### CPU Affinity
### INI Descriptions
#### Bard
#### Cleric
#### Druid
#### Enchanter
#### Magician
#### Necromancer
#### Paladin
#### Ranger
#### Shaman
#### Warrior
#### Wizard
#### Endurance Melee (Monk, Rogue, Berserker)


## Commands
### Attack
### Buffing
### Debuffing
### Curing
### Healing
### Looting
### Movement
### Offtanking

## Detailed Library Descriptions
### Overview

### xiris_buffing.inc
#### Requirements
#### Events
#### Use Cases

### xiris_casting.inc
#### Requirements
#### Events
#### Use Cases
