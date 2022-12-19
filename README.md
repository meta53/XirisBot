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
MQ2Nav is a game-changing quality of life improvement. Instead of having your toons `/stick` to you, you can have them follow you using MQ2Nav that *automatically generates paths* to the target (you) or you could tell your entire raid to navigate to a target or predefined (by you) waypoint. [MQ2Nav on Github](https://github.com/brainiac/MQ2Nav/wiki) The wiki link will give detail usage. Xiris bot has several built in commands that require MQ2Nav to function ex: `/rs FollowMe /type|NAV`
The navigation paths are determined by the plugin parsing a pre-generated zone mesh file. These navmesh files are found in the */Macroquest2/MQ2Nav* folder. They do not come by default with the compile, it is best to grab a premade package of files from [MQ2Mesh](https://mqmesh.com/) grab each package from the expansions listed (you do not need anything past OOW at this point) and extract the zip files to the MQ2Nav folder.
###### MQ2Medley
MQ2Medley replaces MQ2Twist for bard songs. This is because medley allows for queing new songs without restarting the twist, which is invaluable for self buffs, casting debuffs, casting nukes, etc. This will require setting up 'medleys' in your characters macroquest.ini file found in the root of your macroquest folder. 
An example would be for the bard Achlys, */Macroquest2/EQTitan_Achlys.ini* [Wiki Guide](https://www.redguides.com/wiki/MQ2Medley)
```
[MQ2Medley]
Delay=3
Quiet=1
Debug=0

[MQ2Medley-tank]
song1=Niv's Harmonic^18^1
song2=Guardian Rhythms^18^1
song3=Verse of Vesagran^18^1
song4=War March of Muram^18^1
song5=Psalm of Mystic Shielding^18^1 
```

#### Everquest Build
While Xiris bot would work with UF, the MQ2 Plugin requirements mean that only the ROF2 build of Everquest will work properly without a huge degredation of service and code changes.

### Loading
#### bot_loader.mac
This macro will load the class specific macro. Think of this as a bootstrapper, or loader. The best way to accomplish loading the macro on all toons is to create a single hotkey on your tank wit the line: ```/dgza /mac bot_loader ${Me.Name} Xiria Xirea 85```  
The arguments in that command can be described as
1. `/dgza` this is a MQ2Dannet command that *everyone in zone do this command*
2. `/mac bot_loader` load this macro
3. `${Me.Name}` This toon is MT - note this could be a hard set name, however, in a hotkey this will resolve to whomever clicked the hotkey. Allows for easy copying of UIs and hotkeys between your tanks and not having to edit every hotkey.
4. `Xiria Xirea` These toons are ST and TT
5. `85` This is the default healpoint for the MT
#### MQ2Autologin
This build recommends using an MQ2Autologin script to load your raid. To create the batch file, open up notepad (or notepad++, which is recommended) and for each of your characters you'd be loading in:
`ECHO Loading 1 [Xiris] `
`START "" "C:\Program Files (x86)\EQ\EQ_INI\RoF2_1.lnk" patchme /login:myaccountname TIMEOUT /T 10 /NOBREAK`
You should end up with 54 entries in the file, which you should then save as RAID.bat or some other name you'd remember.
Additionally, if you note, the path does not link directly to the eqgame.exe but instead a shortcut link. This just makes it easier to change the path to the .exe file without editing the batch file later. Additionally, you should also create a shortcut link *to every character you have* while it is not required at this point, this will allow you to load a single toon easily.

#### CPU Affinity
To achieve the best performance, you will have to run a PowerShell script (triggerable via batch file) to set the affinity of every eqgame.exe instance to a core. See `eqaff.bat` and `_AFFINITY.bat` inside the distribution zip file.

### INI Files & Class Macros
XirisBot individual character properties are defined in a INI file, with the naming convention `./Macros/xiris_class_ini/BOT_ClassShortName_CharName.ini` or for example `BOT_BER_Ophidia.ini` In each of these INI files there are many sections, which tell XirisBot how the character should behave such as, disciplines, AA, nukes, debuffs, etc. See any of the ini files for examples. Most classes have their own class macros, except for pure Endurance based melee (MNK, ROG, BER) which share bot_melee.mac. Please note that **I personally don't usually run a macro on the MT**
#### Bard
**bot_brd.mac** Melee class, with support for nuking, dotting, debuffing, and singing.
#### Cleric
**bot_clr.mac** Priest class, with support for healing, buffing, debuffing, nuking, curing, and healing.
#### Druid
**bot_dru.mac** Priest class, with support for healing, buffing, debuffing, nuking, dotting, curing, and healing. 
#### Enchanter
**bot_enc.mac** Caster class, with support for debuffing, buffing, nuking, dotting, and charming.
#### Magician
**bot_mag.mac** Caster class, with support for pets, buffing, debuffing, and nuking.
#### Necromancer
**bot_nec.mac** Caster class, with support for pets, buffing, debuffing, nuking, and dotting.
#### Paladin
**bot_pal.mac** Tank class, with support for buffing, stunning, healing, and offtanking.
#### Ranger
**bot_rng.mac** Melee class, with support for healing, buffing, debuffing, nuking, and healing.
#### Shadowknight
**bot_shd.mac** Tank class, with support for nuking, lifetapping, debuffing, dotting, and offtanking [recommend as MT during trash clearing].
#### Shaman
**bot_shm.mac** Priest class, with support for healing, buffing, debuffing, nuking, dotting, curing, and healing. 
#### Warrior
**bot_war.mac** Tank class, offtanking.
#### Wizard
**bot_wiz.mac** Caster class, with support for nuking, quick nuking, ae nuking
#### Endurance Melee (Monk, Rogue, Berserker)
**bot_melee.mac** Standard melee class macro

## Commands
While there are *many* commands built into XirisBot, only a few are generally needed to start. It is recommended that you create hotkeys for these however it is comfortable to use, primarily on your MT. Note that these examples use `/rs` which is raidsay, you could also do this with `/dgt` which tells all toons online the same thing.

### Attack
Hotkey that is used on the MT to call assist on the MT's current target, and has the MT attack the target as well. 
`/multiline ; /rs KillMob ${Target.ID} "${Target.Name}" ${Time.Time24} ; /killthis`

### Buffing
Hotkey or command that is used to tell all available buffers to walk through the raid buffing each group (and single target buff defined memembers with special buffs)
`/rs doraidbuff ALL`

### Debuffing
Hotkey or command that is used to tell all available debuffers to debuff the current target. Note: usually debuffers will also debuff on the Attack call, but sometimes you want to predebuff a target (if its rooted like in Tacvi) or force them to re-debuff all at the same time.
`/rs DebuffTarget ${Target.ID}`

### Curing
Curing becomes *vitally* important in Txevu+ As such there are several commands to force curing. Note: for the majority of the time, all toons will request the appropriate cure from their assigned curers. However, there are times when you want to force it.
1. Cure All Raid Groups `/dgt cureGroupPoison 45` this example tells all curers to look at their groups and to attempt to cure at least 45 poisons counters. Note that the curer will inspect the curee before casting and if its not required will skip that curee. The valid commands here are `cureGroupPoison` `cureGroupDisease` `cureGroupCurse`
2. AutoCure the MT `/dt MyCleric autoCureMTOn` this tells a toon named MyCleric to watch the MT and cure anything that lands on her that is cureable. Note since we can set different MTs per cleric (by telling a cleric or whomever `/dt changeMT newMT`) you can set multiple clerics to autocure multiple 'MTs'

### Healing
Healing is usually handled automatically. 
Specifically healers, in the default healmode (2, this is settable in the ini file) will attempt to heal their MT, themselves, and then their group, in that order. You can set different MTs for various healers and they will watch that MT and that MT only. MTs do not need to be in the group to be healed.
### Looting
Looting is fairly complex, and is based on  xiris_loot.ini (found in the /xiris_common subfolder in your macro folder)
I have an Excel spreadsheet that generates the ini file for myself through Excel macros, but you could do so manually as well. Loot is not auto added to the ini file - an entry must be created.
`/dt Looter looton ALL` will tell looter, Looter to loot ALL items as defined as lootable in the ini file
`/dt Looter looton QUEST` will tell looter, Looter to loot only QUEST items as defined as lootable in the ini file

Loot INI file layout, arguments seperated by pipes
*Loot1=Minor Muramite Rune|QUEST|*|*|!|Aergia|F*
- Item Name 
- Type (VENDOR|TRADE|QUEST) Currently only QUEST is handled differently, the rest is combined under the ALL flag.
- Count (* or integer)
- Class (* or CSV ClassShortName BER,CLR etc)
- Not ToonName or ! for any toon
- Only Tooname or * for everyone
- Beep T or F if the looter should emit a beep if item is found

### Movement
Movement can be achieved either using MQ2Nav or MQ2MoveUtils(stick) with MQ2Nav far preferable. Note in some cases Nav will fail if the toon is on an unnavigable surface (stuck near wall, etc) and you may have to either manually move them or tell them to stick to you.
`/rs FollowMe /type|NAV` or `/rs FollowMe /type|STICK`

Additionally you can tell toons to navigate to waypoints which you had already defined (the command `/nav recordwaypoint WaypointName`)
`/rs NavToWP WaypointName`

### Offtanking
To tell a toon to offtank (and currently only plate classes listen for this) the command is
`/dt ToonName offtankon` and the offtank will attempt to offtank any aggrod mob that isn't currently hitting a tank. However due to lag or other issues sometimes this is not 100% reliable. It works mostly though. `/dt ToonName offtankoff` will cancel offtanking

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