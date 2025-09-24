|**
----------------------------
xiris_common.inc
----------------------------
Common subs used in the majority of xiris's bots.
Looting, Leashing, AA Purchasing are common threads.
Many of the events are from PeteSampras's excellent macs
which have formed the basis of a lot of the bots

______________________________________________
REVISION HISTORY
	10.12.10	xiris	INITIAL REVISION
	10.25.10	xiris	Modified: var names rectified
	10.27.10	xiris	Added Immunity Checks
	11.01.10	xiris	Reverted to using spell_routines.inc
						Modified the cast calls to use this.
	11.15.10	xiris	Modified some of the variable names
	01.26.11	xiris	Updated invisoff event to click off if sent /bc invisOff
						Fixed loot check/npc check inconsistency			
	12.04.15	xiris 	Modified how the initialization of parameters works
						Added various events & handlers
	01.14.17	xiris 	Added new event handlers (includes)		
	08.02.18	xiris	Included xiris_spell_routines and changed engagement events
	09.17.18    xiris   Included ALL xiris bot libraries here instead of in individual macros
	02.27.21	xiris	Added a method to reset XTarget windows due to bug on EMU where they fill up with corpses
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only
	08.05.22	xiris 	Added support for DI checking
	10.01.22	xiris	Fixed the default tank settings on no param passing
	10.14.22	xiris	Fixed Burn with item. Had logic error, would never click item.
	12.27.22	xiris	Moved Burn logic to xiris_burn.inc since its... verbose.
	12.29.22	xiris 	Removed Bufflines init/file due to it not being used for the foreseeable future.
	12.31.22	xiris	Moved Clicky/AA/Burns to their own file
	01.05.23	xiris	Changed logic in chkTargetStatus method to be more streamlined
	06.21.25	xiris	Added Melee Override Event (to turn on melee for any class)
______________________________________________

**|

#define INI_CHAR xiris_class_ini/BOT_${Me.Class.ShortName}_${Me.CleanName}.ini
#define INI_RAID xiris_common/xiris_common.ini

#include xiris_common/xiris_dannet.inc
#include xiris_common/xiris_buffing.inc
#include xiris_common/xiris_burn.inc
#include xiris_common/xiris_casting.inc
#include xiris_common/xiris_charm.inc
#include xiris_common/xiris_curing.inc
#include xiris_common/xiris_debuffing.inc
#include xiris_common/xiris_events.inc 
#include xiris_common/xiris_exclude.inc
#include xiris_common/xiris_healing.inc
#include xiris_common/xiris_looting.inc
#include xiris_common/xiris_melee.inc
#include xiris_common/xiris_offtank.inc
#include xiris_common/xiris_pets.inc
#include xiris_common/xiris_movement.inc

#include xiris_common/xiris_spell_routines.inc

|COMMON
#EVENT KillMob 				"[#1#] KillMob #2#"
#EVENT KillMob 				"<#1#> KillMob #2#"
#EVENT KillMob				"#1# tells the group#*#'KillMob #2#'"
#EVENT KillMob				"#1# tells you#*#'KillMob #2#'"
#EVENT KillMob				"#1# tells the raid#*#'KillMob #2#'"
#EVENT KillMob				"#1# tell your raid#*#'KillMob #2#'"
#EVENT KillMob				"#1# tell your party, #*#'KillMob #2#'"
#EVENT KillMob 				"[ #1# #*#]#*#KillMob #2#"

#EVENT Zoned				"You have entered#*#"
#EVENT Zoned				"LOADING, PLEASE WAIT..."
#EVENT EndMac				"#*#end macro#*#"

#EVENT BackOff 				"[#1#] BackOff #2#"
#EVENT BackOff 				"<#1#> BackOff #2#"
#EVENT BackOff				"#1# tells the group#*#'BackOff#*#'"
#EVENT BackOff				"#1# tells you#*#'BackOff#*#'"
#EVENT BackOff				"#1# tells the raid#*#'BackOff#*#'"
#EVENT BackOff				"#1# tell your raid#*#'BackOff#*#'"
#EVENT BackOff 				"[ #1# #*#]#*#BackOff#*#"

#EVENT QuitExpedition       "#*#DZQUIT#*#"

#EVENT ChangeMT 			"[#1#] ChangeMT #2#"
#EVENT ChangeMT 			"<#1#> ChangeMT #2#"
#EVENT ChangeMT				"#1# tells the group#*#'ChangeMT #2#'"
#EVENT ChangeMT				"#1# tells you#*#'ChangeMT #2#'"
#EVENT ChangeMT				"#1# tells the raid#*#'ChangeMT #2#'"
#EVENT ChangeMT				"#1# tell your raid#*#'ChangeMT #2#'"
#EVENT ChangeMT 			"[ #1# #*#]#*#ChangeMT #2#"

#EVENT ChangeSA 			"[#1#] ChangeSA #2#"
#EVENT ChangeSA 			"<#1#> changeSA #2#"
#EVENT ChangeSA				"#1# tells the group#*#'ChangeSA #2#'"
#EVENT ChangeSA				"#1# tells you#*#'ChangeSA #2#'"
#EVENT ChangeSA				"#1# tells the raid#*#'ChangeSA #2#'"
#EVENT ChangeSA				"#1# tell your raid#*#'ChangeSA #2#'"
#EVENT ChangeSA 			"[ #1# #*#]#*#ChangeSA #2#"

#EVENT ChangeAP 			"[#1#] ChangeAP #2#"
#EVENT ChangeAP 			"<#1#> ChangeAP #2#"
#EVENT ChangeAP				"#1# tells the group#*#'ChangeAP #2#'"
#EVENT ChangeAP				"#1# tells you#*#'ChangeAP #2#'"
#EVENT ChangeAP				"#1# tells the raid#*#'ChangeAP #2#'"
#EVENT ChangeAP				"#1# tell your raid#*#'ChangeAP #2#'"
#EVENT ChangeAP 			"[ #1# #*#]#*#ChangeAP #2#"

| These could be in BURN but leaving them here
#EVENT UseStaunchRecovery   "#*#DoStaunchRecovery#*#"
#EVENT UseIntensity			"#*#DoIntensity#*#"
#EVENT UseServants			"#*#DoServants#*#"
#Event UseInfusion			"#*#DoInfusion#*#"

|MISC events
#EVENT CheckNaked			"#*#CheckNaked#*#"

#EVENT RageOff				"#*#is no longer enraged#*#"
#EVENT RageOn				"#*#has become ENRAGED#*#"
#EVENT ImDead          		"#*#You have been slain by#*#"
#EVENT ImDead          		"Returning to home point, please wait..."
#EVENT ImDead          		"#*#Returning to Bind Location#*#"
#EVENT RemoveRingset		"#1#RemoveRingset#*#"
#EVENT LoadRingset 			"#1#LoadRingset#*#"
#EVENT LoadFaceSlot			"#1#LoadFaceSlot#*#"
#EVENT LoadWeaponset		"#1#LoadWeaponset #2#"
#EVENT SaveJewelry			"#1#SaveJewelry#*#"
#EVENT SaveRingset      	"#1#SaveRingset#*#"
#EVENT SaveFaceSlot      	"#1#SaveFaceSlot#*#"
#EVENT SaveWeaponset  		"#1#SaveWeaponset #2#"
#EVENT SaveCurrentWeaponset "#1#SaveCurrentWeaponset #2#"
#EVENT InvisOFF				"#1#InvisOff#*#"
#EVENT SkillCheck           "#1#SkillCheck #2#"

#EVENT FixCorpses			"#*#fixCorpses#*#"

#EVENT SetupRaid 	"[#1#] SetupRaid #2#"
#EVENT SetupRaid 	"<#1#> SetupRaid #2#"
#EVENT SetupRaid	"#1# tells the group#*#'SetupRaid #2#'"
#EVENT SetupRaid	"#1# tells you#*#'SetupRaid #2#'"
#EVENT SetupRaid	"#1# tells the raid#*#'SetupRaid #2#'"
#EVENT SetupRaid	"#1# tell your raid#*#'SetupRaid #2#'"
#EVENT SetupRaid 	"[ #1# #*#]#*#SetupRaid #2#"

#EVENT RefreshXTarget		"#1# tells the group#*#'RefreshXTarget#*#'"
#EVENT RefreshXTarget		"#1# tells you#*#'RefreshXTarget#*#'"
#EVENT RefreshXTarget		"#1# tells the raid#*#'RefreshXTarget#*#'"
#EVENT RefreshXTarget		"#1# tell your raid#*#'RefreshXTarget#*#'"
#EVENT RefreshXTarget		"#*#RefreshXTarget#*#"

#EVENT MELEE_OVERRIDE 	"#1# tells the group#*#'MeleeOverride #2#'"
#EVENT MELEE_OVERRIDE	"#1# tells you#*#'MeleeOverride #2#'"
#EVENT MELEE_OVERRIDE	"#1# tells the raid#*#'MeleeOverride #2#'"
#EVENT MELEE_OVERRIDE	"[ #1# #*#]#*#MeleeOverride #2#"

|FORCE NAMED
#EVENT ForceNamed     		"#1# tells the group#*#'ForceNamed #*#'"
#EVENT ForceNamed			"#1# tells you#*#'ForceNamed #*#'"
#EVENT ForceNamed			"#1# tells the raid#*#ForceNamed #*#"
#EVENT ForceNamed 			"[ #1# #*#]#*#ForceNamed #2*#"

Sub InitEvents_Common
	/call RegisterBackgroundEvents 	Events_Common 17
	/call RegisterRaidEvents 		Events_Raid 9
	/call RegisterRapidEvents 		Events_Rapid 2
/return

Sub Events_Raid 
	/doevents RageOn
	/doevents RageOff
	/doevents BackOff
	/doevents ChangeMT
	/doevents ChangeAP
	/doevents UseStaunchRecovery
	/doevents UseIntensity
	/doevents UseServants
	/doevents UseInfusion
	/doevents ForceNamed
/return

Sub Events_Common
	| Updating the Environment Cache
	/call UpdateEnvironmentCache

	| Generic Events
	/doevents Zoned	
	/doevents CheckNaked

	| Inventory Save Events
	/doevents SaveJewelry
	/doevents SaveFaceSlot
	/doevents SaveRingset
	/doevents SaveEarrings
	/doevents SaveWeaponset
	/doevents SaveCurrentWeaponset
	
	| Iventory Remove Events
	/doevents RemoveRingset

	| Inventory Load Events
	/doevents LoadFaceSlot
	/doevents LoadRingset
	/doevents LoadWeaponset

	/doevents SaveRingset
	/doevents SetupRaid
	/doevents RefreshXTarget
	/doevents FixCorpses
	/doevents MELEE_OVERRIDE
/return

Sub Events_Rapid
	/doevents KillMob 
	/doevents BackOff
/return 


|**
 ----------------------------
 INITIALIZATION Sub
 ----------------------------
 xbot_initialize has all the common variables
 used in the bot set. Unioned with all the modern macros
 ----------------------------
**|   

Sub xbot_initialize(string _tank1Name, string _tank2Name, string _tank3Name, int _healPoint, string _assistArgs) 
	/echo \aoxbot_initialize 1:${_tank1Name} 2:${_tank2Name} 3:${_tank3Name} 4:${_healPoint} 5:${_assistArgs}

	|-- [DEFAULT_SETTINGS] : Communications & INI Declarations
	/declare 	cmdChannel 				string	outer	dgt
	/declare    announceChannel			string  outer   rsay
	/declare 	mq2meleeEnabled			bool    outer   FALSE
	/call LoadPlugin MQ2Melee ${mq2meleeEnabled}
	/declare 	int_macroMaximumResponseDist	int outer 1000
	/dnet fullnames off

	|-- [DEFAULT_SETTINGS] : Class Archetype (used in case switching)
	/declare 	str_classArchetype		string	outer	UNKNOWN
	/call 	 	getClassArchetype 		${Me.Class.ShortName}
	/varset 	str_classArchetype		${Macro.Return}
	/declare 	MeleeClasses 			string	outer	BRD,BER,BST,MNK,PAL,RNG,ROG,SHD,WAR
	/declare 	PriestClasses 			string	outer	CLR,DRU,SHM
	/declare 	CasterClasses 			string	outer	ENC,MAG,NEC,WIZ
	/declare 	RangedClasses			string	outer	RNG
	/declare 	str_raidGroupMode		string  outer   DEFAULT
	/declare 	bln_meleeOverride		bool    outer   FALSE
	/declare 	RaidGroupLeaders[9] 	string  outer 	NULL 
	/declare 	RaidDIClerics[6] 		string  outer 	NULL

	/call SetupRaid ${str_raidGroupMode}

	|-- SQLite Testing
	| character database
	/declare _pathDB					string outer C:\Program Files (x86)\EQ\ROF2 MQNext\macros\xiris_common\sql\XirisBot.db

	/declare timer_di_request timer outer 30s
	/declare i int local 0
	/for i 1 to ${RaidDIClerics.Size}
		/declare timer_di_${i}		timer outer 0
	/next i
	
	|-- [DEFAULT_SETTINGS] :Tank Settings
	/declare	bln_tanking				bool	outer	FALSE
	/declare 	bln_taunting			bool 	outer	FALSE
	/if (${Me.Combat}) /attack off
	/if (${Stick.Active}) /stick off

	/declare    bln_battleCleric		bool    outer   FALSE
	/if (${_tank1Name.Equal[${Me.Name}]}) 	/varset bln_tanking TRUE
	/if (${_tank1Name.Equal[${Me.Name}]}) 	/varset bln_tanking TRUE

	|-- Set the default tanks
	/declare	str_defTank1			string	outer	${Ini[INI_CHAR,DEFAULT_SETTINGS,defaultTank1,Xiris]}
	/declare	str_defTank2			string	outer	${Ini[INI_CHAR,DEFAULT_SETTINGS,defaultTank2,Xiria]}
	/declare	str_defTank3			string	outer	${Ini[INI_CHAR,DEFAULT_SETTINGS,defaultTank3,Xirea]}
	/declare 	str_tankName1			string	outer 	${str_defTank1}
	/declare	str_tankName2			string	outer 	${str_defTank2}
	/declare 	str_tankName3			string  outer	${str_defTank3}

	|-- Insert passed in values for tanks if we have the parameters passed in
	/if (${_tank1Name.Length} > 3 )  /varset str_tankName1 ${_tank1Name}
	/if (${_tank2Name.Length} > 3 )  /varset str_tankName2 ${_tank2Name}
	/if (${_tank3Name.Length} > 3 )  /varset str_tankName3 ${_tank3Name}

	|-- ID and Name here keep track of *current* tank/second tank. They are swapped as needed.
	/declare	int_MTID           		int		outer	${Spawn[${str_tankName1}].ID}
	/declare 	str_MTName				string  outer  	${str_tankName1}	
	/call chkMTGrouped ${int_MTID}
	/declare 	bln_MTGrouped			bool  	outer   ${Macro.Return}
	/echo \ar chkMTGrouped: \ao ${bln_MTGrouped}
	/declare 	int_STID				int		outer	${Spawn[${str_tankName2}].ID}
	/declare 	str_STName				string  outer 	${str_tankName2}
	/declare	int_TTID				int	 	outer 	${Spawn[${str_tankName3}].ID}
	/declare 	str_TTName				string	outer 	${str_tankName3}

	/echo \aw Tanks \ayMT:\ag${str_MTName} \ayST:\ag${str_STName} \ayTT:\ag${str_TTName}
	| Create a string of group member names for SELECT statements
	/declare	str_groupMembers		string  outer  	${Me.Name} 
	/declare  	str_groupClerics		string  outer   NULL
	/declare 	int_groupClerics 		int 	outer  	1
	/call createGroupList
	/echo \aw Group:\ag${str_groupMembers}

	|-- [DEFAULT_SETTINGS] :Auto Assist Settings
	/declare 	bln_autoAssist			bool	outer 	FALSE
	/declare	int_defAssistAt			int		outer   ${Ini[INI_CHAR,DEFAULT_SETTINGS,assistAt,98]}
	/declare 	int_assistAt			int		outer   ${int_defAssistAt}
	/if (${_assistArgs.Length}) {
		/varset bln_autoAssist ${_assistArgs.Token[1,,]}
		/varset int_assistAt ${_assistArgs.Token[2,,]}
	} 
	/echo \ayAuto Assist settings: \awAuto?\ag${bln_autoAssist} \awAt?\ag${int_assistAt} \awArgs:\ag${_assistArgs}

	|-- [DEFAULT_SETTINGS] : Miscellaneous Defaults
	/declare	str_spellSet			string	outer	${Ini[INI_CHAR,DEFAULT_SETTINGS,str_spellSet,default]}
	/declare	bln_engaged				bool	outer	FALSE
	/declare	bln_enraged				bool	outer	FALSE
	/declare    bln_pulling		    	bool    outer   FALSE
	/declare 	int_targetID			int		outer 	0
	/declare    str_targetName			string  outer   NULL
	/declare 	aggroAnim 				string 	outer 	|5|8|12|17|18|42|44|80|106|129|144|
	/declare    bln_naked               bool	outer   FALSE
	/declare 	bln_ducking				bool	outer	FALSE
	/declare	timer_naked 			timer   outer	0
	/declare 	timer_hott				timer   outer   20s
	/declare 	timer_xtargetRefresh	timer	outer 	60s
	/declare 	bln_forceNamed 			bool	outer	FALSE
	| strFaceItem storage (for OMM event have to swap masks)
	/declare 	strFaceItem				string  outer   ${InvSlot[3].Item}

	|-- [DEFAULT_SETTINGS] : Meditation & Utilities
	/declare int_sitRadius   		int     outer ${Ini[INI_CHAR,DEFAULT_SETTINGS,sitRadius,50]}
	/declare int_medPercent       	int     outer ${Ini[INI_CHAR,DEFAULT_SETTINGS,medPercent,100]}
	/declare timer_medDelay			timer	outer ${Ini[INI_CHAR,DEFAULT_SETTINGS,medDelay,45s]}
	/declare timer_sitDelay			timer	outer ${Ini[INI_CHAR,DEFAULT_SETTINGS,sitDelay,10s]}
	/declare timer_check_utilities	timer   outer 0

	|-- [DEFAULT_SETTINGS] : Camp
	/declare bln_useCAMP			bool	outer 	FALSE
	/declare int_campX 				int 	outer 	${Me.X}
	/declare int_campY 				int 	outer 	${Me.Y}
	/declare int_campZ 				int 	outer 	${Me.Z}
	/declare int_campRadius			int		outer	85			
	/declare str_startZone			string	outer ${cache_zoneShort}
	
	|--include the custom event initializations & other things
	/declare bln_outside			bool	outer	TRUE
	/declare bln_slowUse			bool	outer	FALSE
	/declare bln_slowed				bool	outer	FALSE
	/declare int_slowResisted		int		outer	0
	/declare int_slowMaxTries		int		outer	3

	|-------------------------------------------------
	| Environment cache (update this once per tick)
	|-------------------------------------------------
	/declare timer_envCache          timer   outer 0
	/declare cache_zoneShort         string  outer
	/declare cache_boundZone         string  outer
	/declare cache_npcR200           int     outer
	/declare cache_npcR400           int     outer
	/declare cache_hasXTarget        bool    outer
	/declare cache_targetIsCorpse    bool    outer
	/declare cache_targetID          int     outer
	/declare cache_targetType        string  outer
	/declare cache_targetName        string  outer
	/declare cache_now               string  outer
	
	:init_naked_wait
	/call chkNaked 
	/if (${bln_naked}) {
		/echo \arI am naked, waiting 30s and rechecking before I initialize!
		/call LootSelf
		/call chkNaked 
		/if (${bln_naked}) {
			/delay 1m
			/goto :init_naked_wait
		}
	}

	|-- init common events
	/call xbot_initialize_events
	/call InitEvents_Common

	|-- init other libraries
	/call xbot_initialize_dannet
	/call xbot_initialize_spellRoutines
	/call xbot_initialize_buffing
	/call xbot_initialize_burns
	/call xbot_initialize_casting
	/call xbot_initialize_charming
	/call xbot_initialize_curing
	/call xbot_initialize_debuffing
	/call xbot_initialize_exclusions
	/call xbot_initialize_healing ${_healPoint}
	/call xbot_initialize_looting
	/call xbot_initialize_melee
	/call xbot_initialize_offtanking
	/call xbot_initialize_pets
	/call xbot_initialize_movement

	|-- report that the common things are initialized
	/echo \agXBOT_INITIALIZE COMPLETE
/return

Sub UpdateEnvironmentCache
    /if (${timer_envCache}) /return
	|/echo \agUpdating Environment Cache

    /varset cache_zoneShort      ${cache_zoneShort}
    /varset cache_boundZone      ${cache_boundZone}
    /varset cache_npcR200        ${SpawnCount[npc radius 200]}
    /varset cache_npcR400        ${cache_npcR400}
    /varset cache_hasXTarget     ${cache_hasXTarget}
    /varset cache_now            ${Time.Time24}

    /if (${Target.ID}) {
        /varset cache_targetID       ${cache_targetID}
        /varset cache_targetType     ${cache_targetType}
        /varset cache_targetName     ${cache_targetName}
        /varset cache_targetIsCorpse ${cache_targetType.Equal[corpse]}
    } else {
        /varset cache_targetID       0
        /varset cache_targetType     
        /varset cache_targetName     
        /varset cache_targetIsCorpse FALSE
    }

    | ~0.1s at ~100 ticks/sec; tune as needed
    /varset timer_envCache 10
/return


Sub createGroupList
	/declare    g  int local 0
	/declare 	sz int local 0
	/varcalc 	sz ${Group.GroupSize}-1
	/for g 1 to ${sz}
		/varset str_groupMembers ${str_groupMembers},${Group.Member[${g}].Name}
	/next g
	/call updateClericCount
/return

Sub updateClericCount
	/declare    g  int local 0
	/declare 	sz int local 0
	/varcalc 	sz ${Group.GroupSize}-1
	/varset 	str_groupClerics NULL
	/varset 	int_groupClerics 0

	/for g 0 to ${sz}
		/if (${Group.Member[${g}].Class.ShortName.Equal[CLR]} && ${Group.Member[${g}].Present}) {
			/if (${str_groupClerics.Equal[NULL]}) {
				/varset str_groupClerics ${Group.Member[${g}].Name}
			} else {
				/varset str_groupClerics ${str_groupClerics},${Group.Member[${g}].Name}
			}
			
			/varcalc int_groupClerics ${int_groupClerics}+1
		}
	/next g
	/echo \ar str_groupClerics: ${str_groupClerics} int_groupClerics: ${int_groupClerics}
	
/return 


|**
 ----------------------------
 Fail States
 ----------------------------
 Check things that should be/should not be
 ----------------------------
**|

Sub Event_QuitExpedition
	|echo yeah not going to do that here.
/return  

|-- No Need to check fail states on EMU!
Sub chkFailStates
	|--/call chkGM
	|--/call chkZone
/return

Sub chkGM
	/if (${Spawn.Equal[GM]}) /end
/return



|**
 ----------------------------
 Mana/HP/Endurance checking routine
 ----------------------------
 Based on class. ie: CLR/WIZ/MAG/NEC check for mana - others check for endurance
 uses int_sitRadius, etc
 ----------------------------
**|
Sub chkSit
	/if (${Me.Moving} || ${Navigation.Active}) /return
	|-- Mana Check
	/if (${str_classArchetype.Equal[CASTER]} ||  ${str_classArchetype.Equal[PRIEST]}) {
		| Sit my ass down
		/if ( ${Me.PctMana}<=${int_medPercent} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sitRadius} zradius 55]} == 0)) {
			|/echo I should be sitting my ass down for MANA Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit
		} 
		
	}
	|-- Endurance Check
	/if (${str_classArchetype.Equal[MELEE_END]} ||  ${str_classArchetype.Equal[TANK]}) {
		/if ( ${Me.PctEndurance}<=${int_medPercent} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sitRadius} zradius 55]} == 0)) {
			|/echo I should be sitting my ass down for Endurance Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit 
		} 
	}
/return


|--Engage Target EVENT, calls EngageTarget
Sub EVENT_KillMob(_line, _sender, _eventParams)
	/if (${_eventParams.Arg[1, ].Equal[NULL]}) /return
	/call EngageTarget ${_eventParams.Arg[1, ]}
/return

|-- Engage Target Sub, handles class specific modifications (minor)
Sub EngageTarget(int _targetID)
	/if (!${_targetID}) /return
	| Don't switch targets if you are the MT on AMV or OMM, or it will suck for you
	
	/if (${Me.ID} == ${int_MTID} && (${bln_amvEngaged} || ${bln_OMMEngaged})) /return
	/if (${bln_following}) /call StopFollowing
	/echo \awEngaging Target \ag${Spawn[id ${_targetID}].Name} \ay(${_targetID}) \ao${Time.Time24} \ay${MacroQuest.Running}
	
	| Set Target Flags
	/varset int_targetID ${_targetID}
	/varset str_targetName ${Spawn[id ${_targetID}].Name}
	/varset bln_engaged TRUE

	/if (${Select[${Me.Class.ShortName},CLR,DRU]} || (${Me.Class.ShortName.Equal[CLR]} && !${bln_battleCleric})) /return

	
	
	| Target NPC
	/call TrueTarget ${_targetID}
	/if (${bln_meleeOverride} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) /call EngageTargetMelee

	| Sicc Pet
	/if ((${Select[${Me.Class.ShortName},MAG,BST,NEC,ENC,SHM]}) && ${Pet.ID}) /call chkPetAttack
	
	| Priests don't need to do anything else
	| Stand Up
	/if ((${Me.Feigning} && (${Me.PctHPs} > 50))) /stand
	| Reset recast timers (nukes, dots, etc)
	/if (${str_classArchetype.NotEqual[MELEE_END]} || ${Me.Class.ShortName.NotEqual[WAR]}) /call ResetCastTimers
	
	| Start Swinging

	| Stop STICK if you are MT, cause it's annoying
	/if (${Me.ID} == ${int_MTID} || ${bln_tanking}) /squelch /stick off
/return

|**
 ----------------------------
 Target Checking
 ----------------------------
 Determines if the main assist has a new target, and whether we can attack
 ----------------------------
**|

Sub chkTargetStatus
	/declare reason string local TARGET_VALID
	/if (!${bln_engaged}) /goto :return

	| Target doesnt exist
	/if (!${SpawnCount[id ${int_targetID}]}) {
		/varset reason TARGET_DOESNT_EXIST 
		/goto :disengage
	}

	| No target, or I am at my bound zone, if engaged, kill
	/if (${int_targetID} == 0 || !${cache_targetID} || ${cache_zoneShort.Equal[${cache_boundZone}]}) {
		/varset reason NOTARGET_or_ATBIND
		/goto :disengage
	}
	
	:chkTargetValid
	| Assumes bln_engaged == true
	| Check for target being corpse or non-existant
	/if ((!${SpawnCount[npc id ${int_targetID}]} || ${Spawn[id ${int_targetID}].Type.Equal[corpse]} || ${Spawn[id ${int_targetID}].Name.Find[s_corpse]})) {
		/echo \awNo Targets with ID(\ay${int_targetID}\aw) or is a CORPSE \aoDisengaging!
		/varset reason TARGET_IS_CORPSE
		/goto :disengage
	} 
	
	| Check to see if my target is NPC and there is no NPCs around
	/if (${cache_targetID} && ${cache_targetID} == ${int_MTID} && (!${SpawnCount[npc id ${int_targetID} radius 200]} || ${cache_zoneShort.Equal[${cache_boundZone}]})) {
		/echo \awTarget is MT, and No NPC in radius 200 (or at bind) \aoDisengaging!
		/varset reason TARGETMT_or_ATBIND
		/goto :disengage
	}

	| Check to see if there are any NPCs on XTarget and Radius
	/if ((${cache_hasXTarget}==0 && !${cache_npcR200})) {
		/echo \awNo Targets within 200, or on XTarget \aoDisengaging!
		/varset reason NOTARGETSWITHIN200_AND_NOXTARGET
		/goto :disengage
	} 

	| If there is something on XTarget, and its over 400 away, ignore it.
	/if (${cache_hasXTarget} && (!${cache_npcR400}  || ${cache_zoneShort.Equal[${cache_boundZone}]})) {
		/echo \awNo Targets within 400, even if they are on XTarget \aoDisengaging!
		/varset reason NOTARGETWITHIN400_AND_HASXTARGET
		/goto :disengage
	}

	| Priests can skip next check and just go to check hott aggro
	/if (${Me.Class.ShortName.Equal[SHM]} && ${Me.Class.ShortName.Equal[CLR]} && ${Me.Class.ShortName.Equal[DRU]}) /goto :chkTargetTarget

	| I am engaged, check we are targetting correct NPC (not if we are tanking becausse that is annoying)
	/if ((${cache_targetID} != ${int_targetID}) && (${Spawn[id ${int_targetID}].PctHPs} >= 0 && !${Spawn[id ${int_targetID}].Name.Find[s_corpse]}) && !${bln_tanking} && !${bln_activeCharming}) {
		| I am not targetting the correct NPC
		/echo \aoCurrent Target \arNOT \ao${Spawn[id ${int_targetID}].Name}(${int_targetID})\aw, re-targetting
		/call TrueTarget ${int_targetID}
	} else /if ((${cache_targetID} != ${int_targetID}) && (${Target.PctHPs} >= 0 && !${Spawn[id ${int_targetID}].Name.Find[s_corpse]}) && ${bln_tanking}) {
		/varset int_targetID ${cache_targetID}
	}
	
	:chkTargetTarget
	/call TargetNamedCheck ${int_targetID}
    /declare isNamed bool local ${Macro.Return}
	/if (${cache_targetType.Equal[NPC]} && ${Me.TargetOfTarget.CleanName.Equal[${Me.CleanName}]} && !${bln_tanking} && ${isNamed}) {
		| Priest/Caster (with some sort of a aggro dump ability)
		/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) {
			| Wizard
			/if (${Me.Class.ShortName.Equal[WIZ]}) {
				/if (${Me.AltAbilityReady[Mind Crash]})   	/call MQ2Cast "Mind Crash" alt
				/if (${Me.SpellReady[${str_concSpell}]})	/call MQ2Cast "${str_concSpell}" ${str_concGem}
			} else /if (${Me.Class.ShortName.Equal[NEC]}) {
				/if (${Me.AltAbilityReady[Death Peace]})   	/call MQ2Cast "Death Peace" alt
				/if (!${Me.Standing}) /stand
			} else /if (${Me.Class.ShortName.Equal[ENC]}) {
				/if (${Me.AltAbilityReady[Soothing Words]}) /call MQ2Cast "Soothing Words" alt	
			} else /if (${Me.Class.ShortName.Equal[MAG]}) {

			} else /if (${Me.Class.ShortName.Equal[CLR]}) {
				/if (${Me.AltAbilityReady[Sanctuary]}) 		/call MQ2Cast "Sanctuary" alt	
			} else /if (${Me.Class.ShortName.Equal[SHM]}) {

			} else /if (${Me.Class.ShortName.Equal[DRU]}) { 

			}
		} else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
			/declare use_riposte bool local FALSE
			/if (${cache_zoneShort.Equal[tacvi]} && (${Target.Name.Find[Pirik]} || ${Target.Name.Find[Yihst]} || ${Target.Name.Find[Tunat]})) {
				/varset use_riposte TRUE
			} else /if (${cache_zoneShort.Equal[anguish]} && (${Target.Name.Find[Keldovan]} || ${Target.Name.Find[Ture]} || ${Target.Name.Find[Vangl]} || ${Target.Name.Find[Zun]} || ${Target.Name.Find[Hanvar]} || ${Target.Name.Find[Overlord]})) {
				/varset use_riposte TRUE
			} else /if (${cache_zoneShort.Find[chambers]} &&  ${Target.Name.Find[Master]}) {
				/varset use_riposte TRUE
			} 
			/if (${Me.Class.ShortName.Equal[SHD]}) {
				/if (${Me.AltAbilityReady[Death Peace]})   	/call MQ2Cast "Death Peace" alt
				/if (!${Me.Standing}) /stand
			} else /if (${Me.Class.ShortName.Equal[BER]}) { 
				/if (${use_riposte}) /call FireRiposteDisc
				/if (${Me.CombatAbilityReady[Baffling Strike]} && ${FindItemCount[Axe of the Destroyer]}>0) /doability "Baffling Strike"
				/if (${Me.CombatAbilityReady[Confusing Strike]} && ${FindItemCount[Rage Axe]}>0) 			/doability "Confusing Strike"
				/if (${Me.CombatAbilityReady[Distracting Strike]} && ${FindItemCount[Bonesplicer Axe]}>0) 	/doability "Distracting Strike"
			} else /if (${Me.Class.ShortName.Equal[ROG]}) { 
				/if (${use_riposte}) /call FireRiposteDisc
			} else /if (${Me.Class.ShortName.Equal[RNG]}) { 
				/if (${Me.SpellReady[${str_concSpell}]})	/call MQ2Cast "${str_concSpell}" ${str_concGem}
				/if (${use_riposte}) /call FireRiposteDisc	
			} else /if (${Me.Class.ShortName.Equal[MNK]}) { 
				/if (${Me.AbilityReady[Feign Death]}) /doability "Feign Death"
				/if (${use_riposte}) /call FireRiposteDisc	
				/if (!${Me.Standing}) /stand
			}
		}
	}
	/goto :return

	:disengage
		/echo \aoChkTargetStatus \awTriggering Disengage: \ag${reason}
		/goto :return
	:retarget
	
	:return
	/if (${reason.NotEqual[TARGET_VALID]}) /call Disengage
/return


|-------------------------------------------------------------------------------- 
|SUB: Event_BackOff!
|--------------------------------------------------------------------------------

Sub Event_BackOff
	/echo \ao Event Back Off Called
	/call Disengage
/return

Sub Disengage 
	/echo \awDisengaging \ao${Time.Time24}
	|-- global reset
	/varset bln_engaged 		FALSE
	/varset int_targetID		0
	/varset str_targetName		NULL
	/varset str_debuffedTargets NULL
	/varset bln_forceNamed		FALSE

	/call RunRapidEvents
	| Priests have nothing left to do here;
	/if (${str_classArchetype.Equal[PRIEST]}) /return
	/if (${Stick.Active}) /stick off

	|-- class archetype resets
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/echo \ag Disengage: Attack Off
		/squelch /attack off
		/stick off
	}
	/if (${Pet.ID}) /pet back off
/return


Sub chkMobDead
	/if ((${cache_npcR200} == 0 || ${cache_hasXTarget} == 0) && ${bln_engaged} ) {
		/echo chkMobDead triggered XTarget:${cache_hasXTarget} || SpawnCount:!${SpawnCount[npc id ${int_targetID} radius 250]}  && engaged:${bln_engaged}
		/call Disengage
	}
/return

Sub TrueTarget(int _targetID)
	/if (${SpawnCount[id ${_targetID}]} && ${cache_targetID} != ${_targetID} ) {
		/declare retryTimer timer local 30
		:Target_Loop
		/squelch /target id ${_targetID}
		/delay 2 ${cache_targetID} == ${_targetID}
		
		/if (${cache_targetID} != ${_targetID} && ${SpawnCount[id ${_targetID}]}) {
			/if (${retryTimer}) {
				/goto :Target_Loop
			} else {
				/echo Failed to target [${Spawn[id ${_targetID}].CleanName} - ${_targetID}]
			}
		}
	}
/return


|**
 ----------------------------
 Assist Checking
 ----------------------------
 Determines if the main assist is dead, if so, set new MA
 ----------------------------
**|
Sub chkMTAlive(bool _setAsTarget)
	|--MT is a live and ID is correct
	/if (${SpawnCount[PC ${str_MTName}]} && (${int_MTID} == ${Spawn[PC ${str_MTName}].ID})) /goto :return 

	|--MT, ST, and TT are not in zone or dead!
	/if (!${SpawnCount[PC ${str_MTName}]} && !${SpawnCount[PC ${str_STName}]} && !${SpawnCount[PC ${str_TTName}]}) {
		/if (${Me.Casting.ID}) /call Interrupt
		/echo \arALL TANKS ARE DEAD!
		/call getMTbyPrecedence
		/varset int_MTID ${Spawn[PC ${Macro.Return}].ID}
		/varset str_MTName ${Spawn[PC ${Macro.Return}].Name}
		/goto :hasChanged
	}

	|--MT is alive but ID is wrong; 
	/if (${SpawnCount[PC ${str_MTName}]} && (${int_MTID} != ${Spawn[PC ${str_MTName}].ID})) {
		/echo \ay MT ${str_MTName} is alive, and my MTID ${int_MTID} is wrong
		/varset int_MTID ${Spawn[${str_MTName}].ID}
		/goto :hasChanged
	}

	|--ST is still alive; 
	/if (${SpawnCount[PC ${str_STName}]} && (${int_MTID} != ${Spawn[PC ${str_STName}].ID})) {
		/if (${Me.Casting.ID}) /call Interrupt
		/echo \ao ST  is still alive, switching!
		/varset str_MTName ${str_STName}
		/varset str_STName ${str_TTName}
		/varset int_MTID ${Spawn[${str_STName}].ID}
		/goto :hasChanged
	}
	
	|--TT is still alive; 
	/if (${SpawnCount[PC ${str_TTName}]} && (${int_MTID} != ${Spawn[PC ${str_TTName}].ID})) {
		/if (${Me.Casting.ID}) /call Interrupt
		/echo \ao TT is still alive, switching!
		/varset str_MTName ${str_TTName}
		/varset int_MTID ${Spawn[${str_TTName}].ID}
		/goto :hasChanged
	}
	:hasChanged
	/if (${Me.Name.Equal[${str_MTName}]}) {
		/varset bln_tanking TRUE
		/varset bln_tanking TRUE
		/varset bln_taunting TRUE
	}
	
	|-- If set as target, target the new MT immediately
	/if (${_setAsTarget} && (${cache_targetID} != ${int_MTID})) {
		/if (${Me.Casting.ID}) /call Interrupt
		/call TrueTarget ${int_MTID}
	}
:return	
/return

Sub chkMTGrouped(int _mtid)
	/declare g 			int 	local 0
	/declare isGrouped 	bool 	local FALSE

	/for g 0 to 5
		/if (${Group.Member[${g}].Present} && ${Group.Member[${g}].ID} == ${_mtid}) /return TRUE 
	/next g
/return FALSE




|**
 ----------------------------
 Class Archetype
 ----------------------------
 Based on class. ie: CLR/DRU/SHM = PRIEST, NEC/WIZ/MAG/ENC = CASTER
 ----------------------------
**|
Sub getClassArchetype(string _shortName)
	| PRIEST CLASS ENUMERATION
	/if (${Select[${_shortName},CLR,DRU,SHM]}) {
		/return PRIEST
	}
	
	| CASTER CLASS ENUMERATION
	/if (${Select[${_shortName},NEC,WIZ,MAG,ENC]}) {
		/return CASTER
	}

	| MELEE DPS CLASS ENUMERATION - Endurance Focus
	/if (${Select[${_shortName},ROG,BER,MNK]}) {
		/return MELEE_END
	}
	
	| MELEE DPS CLASS ENUMERATION - Mana Focus
	/if (${Select[${_shortName},BST,RNG,BRD]}) {
		/return MELEE_MANA
	}	
	
	| TANK CLASS ENUMERATION
	/if (${Select[${_shortName},WAR,SHD,PAL]}) {
		/return TANK
	}		
/return

Sub getClassBaseType(string _shortName)
	/if (${Select[${_shortName},CLR,DRU,SHM]}) {
		/return PRIEST
	}
	
	| CASTER CLASS ENUMERATION
	/if (${Select[${_shortName},NEC,WIZ,MAG,ENC]}) {
		/return CASTER
	}

	| MELEE DPS CLASS ENUMERATION - Endurance Focus
	/if (${Select[${_shortName},ROG,BER,MNK]}) {
		/return MELEE
	}
	
	| MELEE DPS CLASS ENUMERATION - Mana Focus
	/if (${Select[${_shortName},BST,RNG,BRD]}) {
		/return MELEE
	}	
	
	| TANK CLASS ENUMERATION
	/if (${Select[${_shortName},WAR,SHD,PAL]}) {
		/return MELEE
	}	

/return 



Sub Event_SKILLCHECK(_line, _sender, _eventParams) 	
	/echo ${_sender} checking skill archetype:${_eventParams.Arg[1,|]} | skill:${_eventParams.Arg[2,|]}
	/echo str_classArchetype: ${str_classArchetype}
	|/if (${str_classArchetype.Equal[${_eventParams.Arg[1,|]}]})  {
		/echo found ${Me.Skill[${_eventParams.Arg[1,|]}]}
		/docommand /${announceChannel} ${_eventParams.Arg[1,|]}:${Me.Skill[${_eventParams.Arg[1,|]}]}
	|}
/return



|**
 ------------------------
 EVENTS
	Event Handlers defined at the top of this file
	
 ------------------------
**|

|-------------------------------------------------------------------------------- 
|SUB: Event_SetupRaid
|	  Sets the raid group leaders based on mode. Also, calls this on init with default settings
|     Also sets the list of available Divine INtervention Clerics

|--------------------------------------------------------------------------------
Sub Event_SetupRaid(_line, _sender, _eventParams)
	/call SetupRaid ${_eventParams.Arg[1,|]}
/return 


Sub SetupRaid(string _mode)
	|-- Pull from the common ini settings file to grab the group leaders/di clerics etc
	/echo \ay Setting raid group mode to ${_mode}

	/varset str_raidGroupMode ${_mode}

	

	/deletevar RaidGroupLeaders 
	/deletevar RaidDIClerics
	
	|[RaidBuilder]
	|keys=DEFAULT|TRIAD|JELVAN|THIRTYSIX
	/declare keys string local ${Ini[INI_RAID,RaidBuilder,keys]}
	:fail_noRaid
	/if (!${keys.Find[${_mode}]}) {
		/echo \awSetting Raid Group Mode \arERROR: 
		/echo \aw   Unable to find \ay${_mode} in \ao${keys}
		/declare RaidGroupLeaders[1] string outer NULL
		/declare RaidDIClerics[1] string outer NULL
		/goto :return
	}

	/declare settingsGroup	string local RAID_${_mode}
	/declare raidCount		int	local ${Ini[INI_RAID,${settingsGroup},RaidCount]}
	/declare groupCount     int local
	/declare groupCountCSV	string local ${Ini[INI_RAID,${settingsGroup},GroupCount]}

	/declare leaderList  	string local
	/declare leaderIndex  	int local 0

	/declare DICount		int local
	/declare DIList			string local

	/declare myGroupNum		int local 0

	/declare i int local
	/declare j int local

	| Find which Group Set I am in 
	/for i 1 to ${raidCount}
		/varset leaderList ${Ini[INI_RAID,${settingsGroup},Raid_${i}_Leaders]}
		
		/if (${leaderList.Find[${Raid.Leader}]}) {
			/varset leaderIndex ${i}
			/varset groupCount ${groupCountCSV.Arg[${i},,]}
			| Set the group leaders
			/declare RaidGroupLeaders[${groupCount}] string outer NULL
			/varset leaderList ${Ini[INI_RAID,${settingsGroup},Raid_${i}_Leaders]}
			/echo \awFound my membership in Raid \ao${i}
			/echo \aySetting Raid Group mode to \ao${_mode}
			/for j 1 to ${groupCount}
				/varset	RaidGroupLeaders[${j}]  ${leaderList.Arg[${j},|]}
				/echo \ag Raid Group Leader \aw[\ag${j}\aw] \ay${RaidGroupLeaders[${j}]}
				/if (${Group.Leader.Name.Equal[${RaidGroupLeaders[${j}]}]}) /varset myGroupNum ${j}
			/next j

			| Set the Divine Intervention Information
			/varset DICount ${Ini[INI_RAID,${settingsGroup},Raid_${i}_DI_Count]}
			/varset DIList	${Ini[INI_RAID,${settingsGroup},Raid_${i}_DI_Clerics]}
			/declare RaidDIClerics[${DICount}] string outer NULL
			/echo \aySetting Raid Divine Intervention mode to \ao${_mode}
			/for j 1 to ${DICount}
				/varset	RaidDIClerics[${j}]  ${DIList.Arg[${j},|]}
				/echo \ag Divine Intervention \aw[\ag${j}\aw] \ay${RaidDIClerics[${j}]}
			/next j
			/goto :return
		}

	/next i
	:fail_noGroup
		/echo \awSetting Raid Group Mode \arERROR
		/echo \aw   Unable to find our raid leader in the leader list for the \ao${_mode} \awraid type.
		/declare RaidGroupLeaders[1] string outer NULL
		/declare RaidDIClerics[1] string outer NULL
		/goto :return
	:return

/return


|-------------------------------------------------------------------------------- 
|SUB: Event_InvisOff
|--------------------------------------------------------------------------------
Sub Event_InvisOFF 
	/call InvisOff
/return 

Sub InvisOff
	/if (${Me.Buff[Group Perfected Invisibility].ID}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[Group Perfected Invisibility].ID}-1].Int} leftmouseup
	/if (${Me.Buff[Gather Shadows].ID}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[Gather Shadows].ID}-1].Int} leftmouseup
	/if (${Me.Buff[Invisibility].ID}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[Invisibility].ID}-1].Int} leftmouseup
/return 

|-------------------------------------------------------------------------------- 
|SUB: Event_EndMac
|--------------------------------------------------------------------------------
Sub Event_EndMac
	/dgt Someone told me to end the macro it! ${Time}
	/delay 1s
	/gate
	/delay 2m
	/camp desktop
	/end
/return




| AA Special Events
Sub EVENT_UseStaunchRecovery
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${cache_zoneShort.Equal[${cache_boundZone}]}) /return

	/if ((${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[TANK]}) && ${Me.PctEndurance} < 20) {
		/casting "Staunch Recovery" alt 
	} else /if (${Me.Class.ShortName.Equal[PAL]} && ${Me.PctMana} < 20) {
		/casting "Staunch Recovery" alt
	} else /if (${Me.Class.ShortName.Equal[BRD]} && ${Me.PctMana} < 20) { 
		/call MQ2Medley ALT "Staunch Recovery" ${int_targetID} FALSE
	} else /if ((${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) && ${Me.PctMana} < 20) { 
		/casting "Staunch Recovery" alt 
	} 
/return

Sub EVENT_UseIntensity
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${cache_zoneShort.Equal[${cache_boundZone}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley ALT "Intensity of the Resolute" ${int_targetID} TRUE
	} else {
		/casting "Intensity of the Resolute" alt
	}
/return

Sub EVENT_UseServants
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${cache_zoneShort.Equal[${cache_boundZone}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley ALT "Steadfast Servant" ${int_targetID} FALSE
	} else {
		/casting "Steadfast Servant" alt
	}
/return

Sub EVENT_UseInfusion
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${cache_zoneShort.Equal[${cache_boundZone}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley ALT "Infusion of the Faithful" ${int_targetID} FALSE
	} else {
		/casting "Infusion of the Faithful" alt
	}
/return


|-------------------------------------------------------------------------------- 
|SUB: Event_ENRAGE HANDLERS for pets (and melee if they aren't usign mq2melee, ala specialization)
|--------------------------------------------------------------------------------


Sub Event_RageOn(string line)
	/if (!${mq2meleeEnabled} && !${Stick.Behind} && (${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${cache_targetName}]}) && (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]})) {
		/attack off
	}
	/if (((${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${cache_targetName}]}) && ${Pet.ID}) && !${Me.PetBuff[Master's Aura Effect]}) {
		/echo Enrage detected! Backing pet off.
		/pet back off
		/varset bln_enraged TRUE
	}
/return

Sub Event_RageOff(string line)
	/if (!${mq2meleeEnabled} && (${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${cache_targetName}]}) && (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) && ${cache_targetID}) {
		/echo  \awRageOff :tryKill \ag Attacking 
		/attack on
	}
	/if ((${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${cache_targetName}]}) && ${Pet.ID}) {
		/pet attack
		/varset bln_enraged FALSE
	}
/return


|-------------------------------------------------------------------------------- 
|-- DUCKING EVENT
|-- Used for events like MPG/Demi:Hatchet
|-------------------------------------------------------------------------------- 
Sub EVENT_DUCKON
	/echo ducking!
	/varset bln_ducking TRUE
	/duck
	:ducking
	/if (!${Me.Ducking}) /keypress x
	/call RunRaidEvents
	/if (${bln_ducking}) {
		/delay 1s
		/goto :ducking
	}
/return

Sub EVENT_DUCKOFF
	/echo end ducking!
	/if (${Me.Ducking}) /keypress x
	/varset bln_ducking FALSE
/return



|-------------------------------------------------------------------------------- 
|SUB: Event_ZONE HANDLERS
|--------------------------------------------------------------------------------
Sub Event_Zoned
	/echo \awEvent_Zoned ${Time.Time24}
	/call setExclusions
	/if (${MacroQuest.GameState.Equal[CHARSELECT]}) /end
/return



Sub doRefreshAlerts
  /declare i int local
  /declare NPCAlertList int local ${Math.Calc[${list_ignore.Count[|]}-1].Int}
  /squelch /alert clear 1
    /if (${NPCAlertList}>0) {
  	/for i 1 to ${NPCAlertList}
  		/squelch /alert add 1 ${list_ignore.Arg[${i},|]}
	/next i
  }
/return




|-------------------------------------------------------------------------------- 
|EVENTS: MISCELLANEOUS
| Note, the immunity calls send the immune announcemnt 
|       to the channel, so all bots can pick it up.
|--------------------------------------------------------------------------------

|**
----------------------------
Set the MT to a specific NAME
----------------------------  
**|

Sub getMTbyPrecedence
	/echo getMTbyPrecedence

	/declare newTank	string	local
	/declare newID 		int 	local

	|-- Start with the defined str_MTName,...3
	/if (${SpawnCount[pc ${str_MTName}]} == 1) {
		/varset newTank ${str_MTName}
		/goto :return
	} else /if (${SpawnCount[pc ${str_STName}]} == 1) {
		/varset newTank ${str_STName}
		/goto :return
	}  else /if (${SpawnCount[pc ${str_TTName}]} == 1) {
		/varset newTank ${str_TTName}
		/goto :return
	}


	| No tank is still alive, lets do by class precedence
	| Try to determine the next valid tank (likely a DPS or myself)
	/if (${NearestSpawn[PC WAR].Name.NotEqual[NULL]}) {
		/varset newTank ${NearestSpawn[PC WAR].Name}
	} else /if (${NearestSpawn[PC SK].Name.NotEqual[NULL]}) {
		/varset newTank ${NearestSpawn[PC SHD].Name}
	} else /if (${NearestSpawn[PC PAL].Name.NotEqual[NULL]}) {
		/varset newTank ${NearestSpawn[PC PAL].Name}
	} else /if (${NearestSpawn[PC MNK].Name.NotEqual[NULL]}) {
		/varset newTank ${NearestSpawn[PC MNK].Name}
	} else {
		/echo NO Suitable Replacement MT's met, set self to MT
		/varset newTank ${Me.Name}
	}

	:return
	/varset newID ${Spawn[PC ${newTank}].ID}
	| set globals
	/varset int_MTID ${newID}
	/varset str_MTName ${newTank}
	/echo End getMTbyPrecedence: ${newTank}(${newID});
/return ${newID}


Sub EVENT_ChangeMT(_line, _sender, _eventParams)
	/echo Changing the MT
	/echo _sender: ${_sender}
	/echo _eventParams:${_eventParams}
	/declare tankName string local ${_eventParams.Arg[1, ]}
	/call setMT ${tankName}

	/echo MTID: ${int_MTID} MTName: ${name}
	/if (${str_classArchetype.Equal[PRIEST]}) /dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${healPoint}
	/if (${Me.Class.ShortName.Equal[CLR]}) /call TrueTarget ${int_MTID}
	/if (${Me.Name.Equal[${tankName}]}) {
		/varset bln_tanking 	TRUE
		/varset bln_taunting 	TRUE
	} else {
		/varset bln_tanking 	FALSE
		/varset bln_taunting 	FALSE
	}
/return

Sub setMT(string _tankName)
	/echo \agSetMT(${_tankName}):: ${Spawn[${_tankName}].ID}
	/varset str_MTName ${_tankName}
	/varset int_MTID ${Spawn[${_tankName}].ID}
/return

Sub EVENT_ChangeSA(_line, _sender, _eventParams)
	/echo SA Change called! Changing to: ${_eventParams.Arg[1, ]}
	/echo _sender: ${_sender}
	/echo _eventParams:${_eventParams}
	/declare SA string local ${_eventParams.Arg[1, ]}
	/varset	int_STID ${Spawn[${SA}].ID}
/return


Sub EVENT_ChangeAP(_line, _sender, _eventParams)
	/if (${str_classArchetype.NotEqual[PRIEST]}) {
		/echo _sender: ${_sender}
		/echo _eventParams:${_eventParams}
		/echo AP (AssistPoint) Change called! Changing to: ${chatTe_eventParamsxt.Arg[1, ]}
		/declare AP int local ${_eventParams.Arg[1, ]}
		/varset	int_assistAt ${AP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_assistAt}
	}
/return





Sub LoadPlugin(string _pluginName, bool _load) 
	/if (${_load}) {
		/if (!${Plugin[_pluginName].Name.Length}) {
			/docommand /${cmdChannel} //plugin ${_pluginName}
			/delay 5s
		}
	} else {
		/if (${Plugin[_pluginName].Name.Length}) {
			/docommand /${cmdChannel} //plugin ${_pluginName} unload
		}
	}
/return



Sub chkAutoAssist
	/if (!${bln_autoAssist}) /return
	/declare tgtID int local
	/dquery ${str_MTName} -q "cache_targetID"
	/delay 20 ${DanNet.Query.Received}
	/varset tgtID ${DanNet.Q}
	/call TrueTarget ${tgtID}
	/call EngageTarget ${tgtID}
/return


| --------------------------------------------------------------------------------------|
| -- Shrinks your group if you have one of several shrink items 					   -|
| --------------------------------------------------------------------------------------|


SUB EVENT_shrinkGroup

	| ---------- Declare your shrink item.  If you have one that isn't in this list, simply add another elseif statement at the bottom
	/declare shrink_ItemName string local

	/if (${FindItem[Bracelet of the Shadow Hive].ID}) {
		/varset shrink_ItemName Bracelet of the Shadow Hive
	} else /if (${FindItem[Cobalt Bracer].ID}) {
		/varset shrink_ItemName Cobalt Bracer
	}

	| ---------- You don't have a shrink item, exit
	/if (!${shrink_ItemName.Length}) /return

	| ---------- Cycle through group and shrink them
	/declare i int local

	/for i 0 to ${Group}

		:recheckGroupMember
		/if (${Spawn[${Group.Member[${i}]}].Height} > 2.40) {

			| ---------- Equip the item if not already done so
			/if (!${Defined[originalItem]}) {
				/declare originalItem string local ${InvSlot[${FindItem[${shrink_ItemName}].WornSlot[1].Name}].Item.Name}
				/exchange "${shrink_ItemName}" ${FindItem[${shrink_ItemName}].WornSlot[1].Name}
			}
			
			/call e3_Cast ${Group.Member[${i}].ID} "${shrink_ItemName}"
			/delay 10s !${Me.Casting}
			/goto :recheckGroupMember
		}

	/next i

	| ---------- Swap back to the original item if you used it
	/if (${Defined[originalItem]}) {
		/delay 5
		/exchange "${originalItem}" ${FindItem[${shrink_ItemName}].WornSlot[1].Name}
	}
/return
|-------------------------------------------------------------|
|- Reliably determine if target is Named				     -|
|-------------------------------------------------------------|
|- @param TargetID: The ID of the spawn you wish to check on
|-------------------------------------------------------------|
Sub EVENT_ForceNamed(_line, _chatSender, _eventParams)
	/if (${_eventParams.Equal[TRUE]}) {
		/echo \aoForcing named only mode:\ag${_eventParams}
		/varset bln_forceNamed TRUE
	} else {
		/echo \aoForcing named only mode:\ag${_eventParams}
		/varset bln_forceNamed FALSE
	}
/return


Sub TargetNamedCheck(_id)
	/if (${bln_forceNamed}) /return TRUE
	/if ((${Spawn[npc id ${_id}].Named} || ${Spawn[npc id ${_id}].Name.Find[#]}) && ${Target.Level} >= 75) /return TRUE
	/if (${cache_zoneShort.Equal[tacvi]} && ${Target.Level} >= 70) /return TRUE
	/if (${cache_zoneShort.Equal[chambersa]} || ${cache_zoneShort.Equal[chambersb]} || ${cache_zoneShort.Equal[chambersc]} || ${cache_zoneShort.Equal[chambersd]} || ${cache_zoneShort.Equal[chamberse]}  && ${Target.Level} >= 70) /return TRUE
	/if (${cache_zoneShort.Equal[anguish]}  && ${Target.Level} >= 75) /return TRUE
	/if (${cache_zoneShort.Equal[arena]}) /return TRUE
	/if (${cache_zoneShort.Equal[thundercrest]}) /return TRUE
/return FALSE
	

| --------------------------------------------------------------------------------------------------------|
| -- Determines if a bot is dead by the number of items of gear they have on 							 -|
| -- 5 or more empty spots is "Dead"																	 -|
| --------------------------------------------------------------------------------------------------------|

Sub chkDead
	/declare i int local
	/declare emptySlotCount int local 0

	/for i 1 to 20
	 	/if (!${InvSlot[${i}].Item.Name.Length}) {
	 		/varset emptySlotCount ${Math.Calc[${emptySlotCount} + 1]}
	 	}
	/next i

	/if (${emptySlotCount} > 5) {
		/return DEAD
	} else {
		/return NOTDEAD
	}

/return


|-- CHECK NAKED
Sub Event_CheckNaked
	/call chkNaked
/return

Sub chkNaked
	/if (!${Me.Inventory[2].ID} && !${timer_naked}) {
		/docommand /${announceChannel} I am NAKED. 
		/varset bln_naked TRUE
		/varset timer_naked 30s
	} else {
		/varset bln_naked FALSE
	}
/return

Sub Event_FixCorpses
	/hidecorpse all
	/delay 2
	/hidecorpse none
	/delay 2
	/hidecorpse looted
/return


SUB verifyEvent(sring _chatSender, string _eventParams)
	/declare userValidated 	bool local FALSE
	/declare includeBot 	bool local TRUE
	/declare inZone 		bool local FALSE
	/declare inRange 		bool local FALSE
	/declare inAssistRange 	bool local FALSE
	/declare sender         string local ${_chatSender}

	| Set sender by parsing the _chatSender
	/if (${_chatSender.Left[1].Compare[ ]}<0) /varset sender ${_chatSender.Right[-2].Left[-1]}
	/if (${_chatSender.Equal[You]}) /varset sender ${Me.CleanName}
	/if (${_chatSender.Equal[MQ2]}) /varset sender ${Me.CleanName}

	| Note if this is sent by dannet as a /dgt (instead of box chat /bc)
	| Then the sender will be like xiris (all)
 	/if (${_chatSender.Find[(]})  {
		/varset sender ${_chatSender.Arg[1,(]}
	}

	| -Validate _chatSender.
	/varset userValidated TRUE
	
	| -Check inZone
	/if (${SpawnCount[pc =${sender}]}) /varset inZone TRUE
	
	| -Check inRange
	/if (${SpawnCount[pc =${sender} radius ${int_macroMaximumResponseDist}]}) /varset inRange TRUE
	|/echo \awVerifyEvent Sender:\ag${sender} \awParams: \ag${_eventParams}

	| -Check for /only|
	/if (${_eventParams.Find[/only|]}) {
		/varset includeBot FALSE
		/call argueString only| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot TRUE
	}
		
	| -Check for /exclude|
	/if (${_eventParams.Find[/exclude|]}) {
		/call argueString exclude| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot FALSE
	}
	
	| -Check for /not|
	/if (${_eventParams.Find[/not|]}) {
		/call argueString not| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot FALSE
	}
	
	| -Check for /include|
	/if (${_eventParams.Find[/include|]}) {
		/call argueString include| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot TRUE
	}
	/echo \ayReturning Sender:\ag${sender} \awValid?\ag${userValidated} \awIncluded?\ag${includeBot} \awInZone?\ag${inZone} \awInRange?\ag${inRange}
/return ${sender}_${userValidated}_${includeBot}_${inZone}_${inRange}

| See if the bot is in the return from the arguestring parser
SUB check_selectedBots(_selectedBots, _chatSender)
	/declare botSelected bool local FALSE
	/if (${_selectedBots.Find[ ]}) /varset _selectedBots ${_selectedBots.Replace[ ,,]}
	/if (${Select[priests,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${PriestClasses}]} || ${Select[healers,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${PriestClasses}]} || ${Select[ranged,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${RangedClasses}]} || ${Select[casters,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${CasterClasses}]} || ${Select[melee,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${MeleeClasses}]} || ${Select[me,${_selectedBots}]} && ${_chatSender.Equal[${Me.CleanName}]} || ${Select[${Me.Class},${_selectedBots}]} || ${Select[${Me.Class.ShortName},${_selectedBots}]} || ${Select[${Me.CleanName},${_selectedBots}]} || ${Select[all,${_selectedBots}]} || ${Select[group,${_selectedBots}]} && ${Me.CleanName.Equal[${_chatSender}]} || ${Select[group,${_selectedBots}]} || ${Select[zone,${_selectedBots}]} && ${SpawnCount[pc ${_chatSender}]}) /varset botSelected TRUE
	
/return ${botSelected}


| Evaluate and Argument string in E3 Format eg: \only|CLR,DRU,SHM
SUB argueString(WhatToFind, givenData)
	|/echo \aoEvaluating Arguments \aw==> \ag${WhatToFind}  \aw=> \ay${givenData}
	/declare i 			int 	local 1
	/declare foundData 	string 	local NULL
	
	:nextArg
	/if (${givenData.Find[/]}) {
		/if (${givenData.Arg[${i},/].Find[${WhatToFind}]}) {
			/varset foundData ${givenData.Arg[${i},/].Arg[2,|]}		
		} else {
			/varcalc i ${i} + 1
			/if (${givenData.Arg[${i},/].Length}) /goto :nextArg
		}
	} else {
		/if (${givenData.Arg[${i}, ].Find[${WhatToFind}]}) {
			/varset foundData ${givenData.Arg[${i}, ].Arg[2,|]}
		} else {
			/varcalc i ${i} + 1
			/if (${givenData.Arg[${i}, ].Length}) /goto :nextArg
		}
	}

/return ${foundData}

|----------------------------------------------------|
|  Resets XTarget window 1-5 elements to auto		 |
|----------------------------------------------------|
Sub Event_RefreshXTarget
	/call RefreshXTarget
/return 


SUB RefreshXTarget
	/if (${timer_xtargetRefresh}) /return
	/echo \aoRefreshing \aoXTarget \ay${timer_xtargetRefresh}
	/declare i int local 1
	/for i 1 to 5
		/xtarget set ${i} grouptank
		/delay 1
		/xtarget set ${i} autohater
	/next i
	/varset timer_xtargetRefresh 60s
/return




|----------------------------------------------------|
| Save INVENTORY Slots		 |
|----------------------------------------------------|
|--Save Events
Sub EVENT_SaveJewelry
	/call SaveRingset
	/call SaveFaceSlot
	/call SaveEarrings
/return 

Sub EVENT_SaveRingset
	/call SaveRingset
/return

Sub EVENT_SaveFaceSlot
	/call SaveFaceSlot
/return 

Sub EVENT_SaveEarrings
	/call SaveEarrings
/return 

|-----
|-- Save Subs
Sub SaveRingset
	/declare leftfinger 	string	local	${Me.Inventory[leftfinger].Name}
	/declare rightfinger 	string	local	${Me.Inventory[rightfinger].Name}

	/echo \aoSaving Ring Slots \awleft: \ag${leftfinger} \awright: \ag${rightfinger}
	/ini "INI_CHAR" "INVENTORY" "leftfinger" "${leftfinger}"
	/ini "INI_CHAR" "INVENTORY" "rightfinger" "${rightfinger}"
/return

Sub SaveFaceSlot
	/declare face			string	local	${Me.Inventory[face].Name}
	/echo \aoSaving Face Slot \awface: \ag${face}
	/ini "INI_CHAR" "INVENTORY" "face" "${face}"
/return 

Sub SaveEarrings 
	/declare leftear 	string	local	${Me.Inventory[leftear].Name}
	/declare rightear 	string	local	${Me.Inventory[rightear].Name}

	/echo \aoSaving Ear Slots \awleft: \ag${leftear} \awright: \ag${rightear}
	/ini "INI_CHAR" "INVENTORY" "leftear" "${leftear}"
	/ini "INI_CHAR" "INVENTORY" "rightear" "${rightear}"
/return 

|-----
Sub EVENT_RemoveRingset
	/echo removingRingset
	/call RemoveRingset
/return

Sub RemoveRingset
	/declare notemptyCount int local 0
	/declare sEmpty string	local NULL
	/declare sBag   string	local NULL
	/declare sSlot  string	local NULL

	:emptyCursor
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :emptyCursor

	:openBags
	/keypress open_inv_bags

	:leftFinger
	/call FindEmptyBagSlot
	/varset sBag    ${Macro.Return.Arg[1,,]}
	/varset sSlot   ${Macro.Return.Arg[2,,]}
	/echo sBag:${sBag} sSlot:${sSlot}
	/if (${sSlot.Equal[NULL]} || ${Me.Inventory[leftfinger].Name.Equal[NULL]}) {
		/echo \arNo EmptySlots or No Left Finger Equiped!
		/goto :rightFinger
	} else {
		/itemnotify leftfinger leftmouseup
		/delay 5
		/echo ${sBag}|${sSlot}
		/itemnotify in pack${sBag} ${sSlot} leftmouseup
	}

	:rightFinger
	/call FindEmptyBagSlot
	/varset sBag    ${Macro.Return.Arg[1,,]}
	/varset sSlot   ${Macro.Return.Arg[2,,]}
	/echo sBag:${sBag} sSlot:${sSlot}
	/if (${sSlot.Equal[NULL]} || ${Me.Inventory[rightfinger].Name.Equal[NULL]}) {
		/echo \arNo EmptySlots or No Right Finger Equiped!
	} else {
		/itemnotify rightfinger leftmouseup
		/delay 5
		/echo ${sBag}|${sSlot}
		/itemnotify in pack${sBag} ${sSlot} leftmouseup
	}
/return

Sub FindEmptyBagSlot
	/echo \aoFindEmptyBagSlot
	/declare bag 			int local -1
	/declare bagSlot 		int local -1
	/declare bagSlotCount 	int local
	/declare i int local 
	/declare j int local 
	/for i 23 to 32
		/varset bagSlotCount ${Me.Inventory[${i}].Container}
		/echo \awBag:\ag${i} \awSlots:\ag${bagSlotCount}
		/if (!${bagSlotCount}) /continue
		/for j 1 to ${bagSlotCount}

			/if (!${Me.Inventory[${i}].Item[${j}].ID}) {
				/varset bag ${i}
				/varset bagSlot ${j}
				/echo \ao First Empty Slot is \awBag:\ag${bag} \awSlot:\ag${bagSlot}
				/goto :return
			}
		/next j
	/next i
	
 
:return
	| using itemnotify, bags are invslot 1-8
	/if (${bag} != -1) /varcalc bag ${bag}-22
/return ${bag},${bagSlot}

|-- method to load the saved rings from INI (primarily for Trial of Foresight)
Sub EVENT_LoadRingset
	/echo \ayLoading Ringset
	/call LoadRingset
/return

Sub LoadRingset
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/declare lr string local ${Ini[INI_CHAR,INVENTORY,leftfinger,NULL]}
	/declare rr string local ${Ini[INI_CHAR,INVENTORY,rightfinger,NULL]}
	/echo \aoLoading Ringset \awLeft:\ag${lr} \awRight:\ag${rr}

	/declare notemptyCount int local 0
	:emptyCursor
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :emptyCursor

	:leftRing
	/if (${lr.NotEqual[NULL]}) {
		/exchange "${lr}" leftfinger
		/delay 10
	}
	
	:rightRing
	/if (${rr.NotEqual[NULL]}) {
		/exchange "${rr}" rightfinger
		/delay 10
	}
	| return out
	/echo \agSaved Ringset Equipped 
	/autoinventory
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley ${str_currentMedley}
/return 

Sub Event_LoadFaceSlot
	/call LoadFaceSlot
/return 

Sub LoadFaceSlot
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/declare face string local ${Ini[INI_CHAR,INVENTORY,face,NULL]}
	
	/declare notemptyCount int local 0
	:emptyCursor
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :emptyCursor
	
	:faceSlot 
	/if (${face.NotEqual[NULL]}) {
		/exchange "${face}" face
		/delay 10
	}
	| return out
	/echo \agSaved Faceslot Equipped 
	/autoinventory
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley ${str_currentMedley}
/return 

|-- method to load the saved weapons from INI (primarily for Trial of Specialization)
Sub EVENT_LoadWeaponset(string _line, string _sender, string _evtParams)
	/echo \ayLoading Weaponset <${_evtParams}> (${_sender})
	/if (${_evtParams.NotEqual[NULL]}) /call LoadWeaponset ${_evtParams}
/return


| TODO rewrite this like we did for rings
Sub LoadWeaponset(string _weaponset)
	|/if (${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[PRIEST]} && ${_weaponset.NotEqual[default]}) /return
	/if (${_weaponset.Equal[NULL]}) /varset _weaponset default
	/declare set string local ${Ini[INI_CHAR,WEAPS,${_weaponset}]}
	/if (${set.Equal[NULL]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/echo \aw ${_weaponset}: ${set}
	/declare slot string local 
	/declare weap string local
	/declare posi string local 

	|-- auto inventory
	/declare notemptyCount int local 0
	:notempty
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :notempty

	|-- empty both hands
	/if (${Me.Inventory[mainhand].Name.NotEqual[NULL]}) {
		/echo \aoUnequip mainhand
		/unequip mainhand
		/autoinventory
	}
	/delay 12
	/if (${Me.Inventory[offhand].Name.NotEqual[NULL]}) {
		/echo \aoUnequip offhand
		/unequip offhand
		/autoinventory
	}

	/if (${_weaponset.Equal[EMPTY]}) /goto :return

	
	/delay 12
	:mainhand
	|-- do mainhand
	/varset slot ${set.Token[1,,]}
	/varset weap ${slot.Token[1,|]}
	/varset posi ${slot.Token[2,|]}

	/if (${weap.Equal[NULL]}) {
		/autoinventory
		/echo \ay Weaponset ${_weaponset} mainhand is NULL! Removing.
		/if (${Me.Inventory[mainhand].Name.NotEqual[NULL]}) {
			/echo unequip mainhand
			/unequip mainhand
			/autoinventory
		} else {
			/echo Mainhand was already Empty!
		}
	} else {
		/autoinventory
		/echo \ag Moving "${weap}" to ${posi}
		/if (${FindItemCount[${weap}]} == 0) {
			/docommand /${announceChannel} Could not find ${weap}, fix my ini!
			/goto :return
		} else {
			/exchange "${weap}" ${posi}
		}
	}

	|-- auto inventory
	/autoinventory
	/delay 12
	:offhand
	|-- do secondary first in case its 2 handed weapon
	/varset slot ${set.Token[2,,]}
	/varset weap ${slot.Token[1,|]}
	/varset posi ${slot.Token[2,|]}
	/if (${weap.Equal[NULL]}) {
		/autoinventory
		/if (${Me.Inventory[offhand].Name.NotEqual[NULL]}) {
			/echo \ayWeaponset ${_weaponset} offhand is NULL! Removing.
			/echo \aoUnequipping Offhand
			/unequip offhand
			/autoinventory
		} else {
			/echo \aoOffhand was already Empty!
		}
		
	} else {
		/autoinventory
		/echo \ag Moving "${weap}" to ${posi}
		/if (${FindItemCount[${weap}]} == 0) {
			/docommand /${announceChannel} Could not find ${weap}, fix my ini!
		} else {
			/exchange "${weap}" ${posi}
		}
		
	} 

	:return
	/autoinventory
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley
/return

|#EVENT SaveWeaponset  		"#1#SaveWeaponset #2#"
Sub EVENT_SaveWeaponset(string _line, string _sender, string _eventParams)
	/echo \aySavingWeaponset ${_sender} ${_eventParams}
	/declare key string local ${_eventParams.Arg[1,=]}
	/declare weapons string local ${_eventParams.Arg[2,=]}
	/echo  key: ${key} weapons: ${weapons}
	/if (${_eventParams.NotEqual[NULL]}) /call SaveWeaponset ${_eventParams.Token[1,=]} ${_eventParams.Token[2,=]}
/return

Sub SaveWeaponset(string _key, string _weaponset)
	|weaponset line should be [_key] [item1|mainhand,item2|offhand]
	|example: default=Longsword of Execration|mainhand,Aegis of Midnight|offhand
	/echo writing key: ${_key} weaponset: ${_weaponset}
	/ini INI_CHAR "Weaponsets" "${_key}" "${_weaponset}"
/return

|#EVENT SaveCurrentWeaponset "#1#SaveCurrentWeaponset #2#"
Sub EVENT_SaveCurrentWeaponset(string _line, string _sender, string _eventParams)
	/call SaveCurrentWeaponset ${_eventParams}
/return

Sub SaveCurrentWeaponset(string _key)
	/echo Saving Current Weaponset as ${_key}
	/if (${_key.Equal[NULL]}) /varset _key test
	/declare pri string local ${Me.Inventory[mainhand].Name}
	/declare off string local ${Me.Inventory[offhand].Name}
	
 	/ini INI_CHAR "Weaponsets" "${_key}" "${pri}|mainhand,${off}|offhand"
/return


Sub Event_SkillCheck(string _line, string _sender, string _eventParams)
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[TANK]} || ${Me.Class.ShortName.Equal[BRD]}) /return
	/echo SkillCheck: ${_line} ${_sender}
	/echo params ${_eventParams}

	/declare sval int local  ${Me.Skill[${_eventParams}]}
	/if (${sval} == 0) {
		/docommand /${announceChannel} ${Me.Skill[${_eventParams}]}
		/nav wp lobby
	}

/return


Sub EVENT_MELEE_OVERRIDE(_line, _sender, _eventParams)
	/echo \agMELEE_OVERRIDE \aw${_sender} \ar${_eventParams}
	/varset bln_meleeOverride ${_eventParams}
/return