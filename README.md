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
DanNet was created by Dannuic, one of the main contributers to MQ2 and MQNext. This is a replacement for EQBC that provides direct access to any toon from any other toon via queries and observers. The documentation is found at [MQ2Dan on Github](https://github.com/dannuic/MQ2Dan) While it is not required that you know the details of this plugin, it will be helpful to learn the command syntax since it totally replaces EQBC - specifically the 1:1 replacements for common commands in EQBC. Forexample if in EQBC you'd use `/bcaa` instead you'd use `/dga` and instead of `/bct` you'd use `/dt`  Once you get used to it, it works the same. *IMPORTANT* By Default, MQ2Dannet uses fully qualified names as chatsender. ie: EQTitan.Xiris, to preserve backwards compatibility in EVENTs, run `/dnet fullnames off` which will make it so chatsender only sends the character name. Note: this command is now always run in the xbot_initialize (xiris_common.inc) on load.
###### MQ2Debuffs
Required for curing of group/raid. Probably also used with E3. Added it here for clarity.
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
###### MQ2Melee
Standard plugin generally. Used to handle the auto skills portion of meleeing, such as backstab/frenzy/kick etc. Note: if there is a desire, I can eliminate this and run it entirely within the macro. 
###### MQ2WorstHurt
Plugin available in modern compiles that replacing for loops looking at group, and xtarget members hp points. See https://www.redguides.com/wiki/MQ2WorstHurt
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
While there are *many* commands built into XirisBot, only a few are generally needed to start. It is recommended that you create hotkeys for these however it is comfortable to use, primarily on your MT. Note that these examples use `/rs` which is raidsay, you could also do this with `/dgt` which tells all toons online the same thing. A full command list will follow under the detailed library descriptions.

### Attack
Hotkey that is used on the MT to call assist on the MT's current target, and has the MT attack the target as well. 
`/multiline ; /rs KillMob ${Target.ID} "${Target.Name}" ${Time.Time24} ; /attack on`

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
XirisBot shares libraries for all class variants. These files are found in the xiris_common sub folder.

### xiris_buffing.inc
#### Overview
XirisBot buffing handlers. The library reads the buff settings in each character ini file to determine what buffs the character can cast (if any). There are several classes of buffs:
1. OutOfCombat (OOC) Buffs - These are buffs such as Symbol/Haste/etc which you would generally cast out of combat
2. SingleTarget (ST) Buffs  - These are buffs that effect a single target, like Panoply of Vie, or Guard of Earth
3. Combat (CMBT) Buffs - These are buffs that are (auto) cast during a fight, eg: Spirit of Jaguar
#### Requirements
mq2cast, mq2dannet
#### Events
1. `/dgt DoRaidBuffs *ALL*` Will have all buffers walk through the raid and cast all designated buffs (OOC and ST).
2. `/dgt DoCharBuffs *CharName*` Will have all buffers target the specified character, and cast all buffs (note: most buffs are group buffs)
3. `/dgt RemoveBuff *BuffName*` Will find and remove that buff from all characters
4. `/dgt RemoveAllBuffs` Will remove all buffs from all characters


### xiris_casting.inc
#### Overview
XirisBot casting handler. The library reads the casting (NUKE, AE NUKE, QUICK NUKE, DOT, FAST DOT, and STUN) settings in each character ini file to determine what the character can cast (if any)
#### Requirements
mq2cast, mq2dannet
#### Events
1. `/dgt SetResistTypes *fire|poison|magic|etc*` Sets the resist types that will be cast. Can be used to limit to only say... fire and magic on Ture (`/dgt setresisttypes fire|magic`)
2. `/dgt SetUseFastOnly *on|off` Turning this on will have wizards (and other classes if they have any defined QNUKE) only use the QNUKE entries instead of normal NUKE

### xiris_charm.inc
#### Overview
XiristBot charm handler. The library reads the charming (CHARM) settings in the Bard, Necromancer, and Enchanter ini files. Note this is generally untested as there is very little need to charm things (other than OMM)
#### Requirements
mq2cast, mq2dannet
#### Events
1. `/dt charname CharmNPCByID *NPCID*` Tells toon charname to charm the npc with the id given
2. `/dt charname CharmNPCByName *NPCName*` Tells toon charname to charm the npc with the name given
3. `/dt charmOn` Allows charming, must be called before telling a toon to charm with the above commands
4. `/dt charmOff` Disables charming.

### xiris_common.inc
#### Overview
XirisBot common handlers. Contains the links to all other includes. Has the common functions for all the class macros, and serves to kick off all initializations.
#### Requirements
mq2cast, mq2dannet, mq2exchange, mq2melee
#### Events
1. `/dgt KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}` Directs everyone to kill your target
2. `/dgt BackOff` Directs everyone to reset melee, stop casting, etc.
3. `/dgt ChangeMT *MTNAME*` Changes the current Main Tank  (MT) to the new MT via name
4. `/dgt ChangeSA *SANAME*` Changes the current Secondary Assist (SA) to the new SA via name
5. `/dgt ChangeAP *AP*` Changes the autoassist point to int AP
6. `/dgt DoStaunchRecovery` Tells everyone to hit Staunch Recovery AA
7. `/dgt DoIntensity` Tells everyone to hit Intensity of the Resolute AA
8. `/dgt DoServants` Tells everyone to hit Steadfast Servant AA
9. `/dgt DoInfusion` Tells everyone to hit Infusion of the Faithful AA
10. `/dgt SetupRaid` A mostly internal or event used event. Describes the raid mode currerntly running, defaults to.. DEFAULT. Calls SetupRaid which pulls from xiris_common INI. Sets lists of group leaders, and DI available clerics
12. `/dgt RefreshXTarget` Used to manually force raid to refresh their XTarget list.
### xiris_curing.inc
#### Overview
XirisBot curing handlers. This library has several events that can be fired via hotkey, or auto cures driven by the curing section in the toon ini files. Note that this file is responsible for a toons self-need-cure checks - something to be familiar with.
#### Requirements
mq2cast, mq2dannet, mq2debuffs
#### Events
1. `/dt CurerName AutoCureMTOn` `/dt CurerName AutoCureMTOff` Toggle to tell specific character to auto cure the MT (clerics mostly)
2. `/dgt cureGroupPoison 45` Tell the curers in raid to walk through their groups and cure 45 poison counters on whomever has poison counters (will skip toons with no counters)
3. `/dgt cureGroupCurse 45` Tell the curers in raid to walk through their groups and cure 45 curse counters on whomever has curse counters (will skip toons with no counters)
4. `/dgt cureGroupDisease 45` Tell the curers in raid to walk through their groups and cure 45 disease counters on whomever has disease counters (will skip toons with no counters)
5. `/dt CurerName cureMe cureType` Asks a curer to cure requester by type (poison, disease, curse) - curer will determine how many counters.
### xiris_debuffing.inc
#### Overview
XirisBot debuffing handlers. This library has several events that can be fired via hotkey. Much of the debuffing is automatic and is controlled by character ini file.
#### Requirements
mq2cast, mq2dannet, mq2debuffs
#### Events
1. `/dgt DebuffTarget ${Target.ID}` Will force raid debuffers to cycle through debuffs on target. 
### xiris_debuffing.inc
#### Overview
XirisBot debuffing handlers. This library has several events that can be fired via hotkey. Much of the debuffing is automatic and is controlled by character ini file.
#### Requirements
mq2cast, mq2dannet, mq2debuffs
#### Events
1. `/dgt DebuffTarget ${Target.ID}` Will force raid debuffers to cycle through debuffs on target. 
### xiris_events.inc
#### Overview
XirisBot event handler(s). This library has several sub libraries. These contain all the handlers for specific raid/mob events, such as MPG trials, or Anguish events. Each of those files is specific to the raid, and should be familiarized before attempting those raids.
#### Requirements
*all*
#### Events
Many events, all raid specific.
### xiris_healing.inc
#### Overview
XirisBot healing handler. This library is a core library for priest classes.
#### Requirements
mq2cast, mq2dannet
#### Events
1. `/dgt GroupHeal` Tells every capable class to fire a group heal
2. `/dgt InterruptON` `/dgt InterruptOFF` Toggles the ability to interrupt heals if no longer necessary (used usually in conjunction with efficiency mode)
3. `/dgt ChangeHP 85` Changes the MT heal point to provided integer
4. `/dgt ChangeHPTank 85` Changes the MT heal point to provided integer
5. `/dgt ChangeHPSelf 85` Changes the Self heal point to provided integer
6. `/dgt ChangeHpGroup 85` Changes the Group heal point to provided integer
7. `/dgt HealMode EFFICIENT|DEFAULT` Changes the heal mode (and spells cast) to either default or efficient mode. Efficient recommended for most events if you have many clerics.
8. `/dt Healer FocusHEALMT_ON|FocusHealMT_OFF` Toggles the healer to only heal self and MT
9. `/dt Healer HealType 0|1|2|3` Changes the heal type to following:
    1. /if (${int_healMode}==0) /echo \a-wChanging healing \aoTYPE \a-wto \ag0-Self (only)
    2. /if (${int_healMode}==1) /echo \a-wChanging healing \aoTYPE \a-wto \ag1-MT (and self)
    3. /if (${int_healMode}==2) /echo \a-wChanging healing \aoTYPE \a-wto \ag2-Group (and MT, includes self)
    4. /if (${int_healMode}==3) /echo \a-wChanging healing \aoTYPE \a-wto \ag3-GroupOnly (includes self)
10. `/dgt FireTotem X` where X is the totem group. (Cleric, Druid, Shaman) defined by char ini. Will cause toons in group X (integer) to place their heal totems
##### CH Events
1. `/dgt CHStart 1 ${Me.Name} Xiria 30 cleric1,cleric2,cleric3,cleric4` Will start a ch chain with the 4 clerics listed. 3 second delay. Chain key of 1, meaning that chain is index 1, so multiple chains can be run. `CHStart ChainIndex HealTarget BackupTarget Delay [CSV Clerics]`
2. `/dgt CHStop 1` Will stop the 1 index CHChain
3. `/dgt CHPause 1` CHChain #1 will pause until resumed
4. `/dgt CHResume 1` CHChain #1 will resume 
5. `/dgt CHSwitch 1 Xiria` CHChain #1 will switch to healing Xiria
