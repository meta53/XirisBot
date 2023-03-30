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

#EVENT BackOff				"<#1#> BackOff"
#EVENT BackOff				"#1# tells the raid#*#'BackOff#*#'"
#EVENT BackOff 				"[ #1# #*#]#*#BackOff#*#"

#EVENT Disengage   			"#*#Disengage#*#"
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
#EVENT LoadRingset 			"#1#LoadRingset#*#"
#EVENT LoadWeaponset		"#1#LoadWeaponset #2#"
#EVENT SaveRingset      	"#1#SaveRingset#*#"
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

Sub InitEvents_Common
	/call RegisterBackgroundEvents Events_Common 19
	/call RegisterRaidEvents Events_Raid 9
	
/return

Sub Events_Raid 
	/doevents RageOn
	/doevents RageOff
	/doevents BackOff
	/doevents ChangeMT
	/doevents ChangeSA
	/doevents ChangeAP
	/doevents UseStaunchRecovery
	/doevents UseIntensity
	/doevents UseServants
	/doevents UseInfusion
/return

Sub Events_Common 
	/doevents KillMob
	/doevents Zoned	
	/doevents CheckNaked
	|/doevents LoadRingset
	|/doevents LoadWeaponset
	|/doevents SaveWeaponset
	|/doevents SaveCurrentWeaponset
	|/doevents SaveRingset
	|/doevents InvisOFF
	|/doevents SkillCheck
	/doevents Disengage
	/doevents SetupRaid
	/doevents RefreshXTarget
	/doevents FixCorpses
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

	|-- [Settings] : Communications & INI Declarations
	/declare 	cmdChannel 				string	outer	dgt
	/declare 	mq2meleeEnabled			bool    outer   TRUE
	/declare 	int_macroMaximumResponseDist	int outer 1000
	/dnet fullnames off

	|-- [Settings] : Class Archetype (used in case switching)
	/declare 	str_classArchetype		string	outer	UNKNOWN
	/call 	 	getClassArchetype 		${Me.Class.ShortName}
	/varset 	str_classArchetype		${Macro.Return}
	/declare 	MeleeClasses 			string	outer	BRD,BER,BST,MNK,PAL,RNG,ROG,SHD,WAR
	/declare 	PriestClasses 			string	outer	CLR,DRU,SHM
	/declare 	CasterClasses 			string	outer	ENC,MAG,NEC,WIZ
	/declare 	RangedClasses			string	outer	RNG
	/declare 	str_raidGroupMode		string  outer   DEFAULT
	/declare 	RaidGroupLeaders[9] 	string  outer 	NULL 
	/declare 	RaidDIClerics[6] 		string  outer 	NULL

	/call SetupRaid ${str_raidGroupMode}

	/declare timer_di_request timer outer 30s
	/declare i int local 0
	/for i 1 to ${RaidDIClerics.Size}
		/declare timer_di_${i}		timer outer 0
	/next i
	
	|-- [Settings] :Tank Settings
	/declare	bln_tanking				bool	outer	FALSE
	/declare	bln_aggro				bool	outer	FALSE
	/declare    bln_battleCleric		bool    outer   FALSE
	/if (${_tank1Name.Equal[${Me.Name}]}) 	/varset bln_tanking TRUE
	/if (${_tank1Name.Equal[${Me.Name}]}) 	/varset bln_aggro TRUE
	/if (${bln_tanking}  || ${bln_aggro}) 	/squelch /melee aggro=1 
	/if (${bln_tanking}  || ${bln_aggro}) 	/squelch /melee taunt=1 
	/if (!${bln_tanking} || !${bln_aggro}) 	/squelch /melee aggro=0
	/if (!${bln_tanking} || !${bln_aggro}) 	/squelch /melee taunt=0
	|-- Set the default tanks
	/declare	str_defTank1			string	outer	${Ini[INI_CHAR,Settings,defaultTank1,Xiris]}
	/declare	str_defTank2			string	outer	${Ini[INI_CHAR,Settings,defaultTank2,Xiria]}
	/declare	str_defTank3			string	outer	${Ini[INI_CHAR,Settings,defaultTank3,Xirea]}
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
	/declare 	int_STID				int		outer	${Spawn[${str_tankName2}].ID}
	/declare 	str_STName				string  outer 	${str_tankName2}
	/declare	int_TTID				int	 	outer 	${Spawn[${str_tankName3}].ID}
	/declare 	str_TTName				string	outer 	${str_tankName3}

	/echo \aw Tanks \ayMT:\ag${str_MTName} \ayST:\ag${str_STName} \ayTT:\ag${str_TTName}

	|-- [Settings] :Auto Assist Settings
	/declare 	bln_autoAssist			bool	outer 	FALSE
	/declare	int_defAssistAt			int		outer   ${Ini[INI_CHAR,Settings,assistAt,98]}
	/declare 	int_assistAt			int		outer   ${int_defAssistAt}
	/if (${_assistArgs.Length}) {
		/varset bln_autoAssist ${Bool[${_assistArgs.Token[1,,]}]}
		/varset int_assistAt ${_assistArgs.Token[2,,]}
	} 
	/echo \ayAuto Assist settings: \awAuto?\ag${bln_autoAssist} \awAt?\ag${int_assistAt} \awArgs:\ag${_assistArgs}

	|-- [Settings] : Miscellaneous Defaults
	/declare	str_spellSet			string	outer	${Ini[INI_CHAR,Settings,str_spellSet,default]}
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
	/declare 	timer_xtargetRefresh	timer	outer 	30s
	| strFaceItem storage (for OMM event have to swap masks)
	/declare 	strFaceItem				string  outer   ${InvSlot[3].Item}

	|-- [Settings] : Meditation & Utilities
	/declare int_sitRadius   		int     outer ${Ini[INI_CHAR,Settings,sitRadius,50]}
	/declare int_medPercent       	int     outer ${Ini[INI_CHAR,Settings,medPercent,100]}
	/declare timer_medDelay			timer	outer ${Ini[INI_CHAR,Settings,medDelay,45s]}
	/declare timer_sitDelay			timer	outer ${Ini[INI_CHAR,Settings,sitDelay,10s]}
	/declare timer_check_utilities	timer   outer 0
	
	|-- [Settings] : Camp
	/declare bln_useCAMP			bool	outer 	FALSE
	/declare int_campX 				int 	outer 	${Me.X}
	/declare int_campY 				int 	outer 	${Me.Y}
	/declare int_campZ 				int 	outer 	${Me.Z}
	/declare int_campRadius			int		outer	85			
	/declare SZ_shortName			string	outer ${Zone.ShortName}
	
	|--include the custom event initializations & other things
	/declare bln_outside			bool	outer	TRUE
	/declare bln_slowUse			bool	outer	FALSE
	/declare bln_slowed				bool	outer	FALSE
	/declare int_slowResisted		int		outer	0
	/declare int_slowMaxTries		int		outer	3
	
	:init_naked_wait
	/call chkNaked 
	/if (${bln_naked}) {
		/echo \arI am naked, waiting 30s and rechecking before I initialize!
		/call LootSelf
		/call chkNaked 
		/if (${bln_naked}) {
			/delay 30s
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
	|-- Mana Check
	/if (${str_classArchetype.Equal[CASTER]} ||  ${str_classArchetype.Equal[PRIEST]}) {
		| Sit my ass down
		/if ( ${Me.PctMana}<=${int_medPercent} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sitRadius} zradius 55]} == 0)) {
			|/echo I should be sitting my ass down for MANA Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit
			/delay 2s
		} 
		
	}
	|-- Endurance Check
	/if (${str_classArchetype.Equal[MELEE_END]} ||  ${str_classArchetype.Equal[TANK]}) {
		/if ( ${Me.PctEndurance}<=${int_medPercent} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sitRadius} zradius 55]} == 0)) {
			|/echo I should be sitting my ass down for Endurance Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit on
			/delay 2s
		} 
	}
/return


|--Engage Target EVENT, calls EngageTarget
Sub EVENT_KillMob(_line, _sender, _eventParams)
	/echo \ayKMCalled ${_sender} ${_eventParams}
	/if (${_eventParams.Arg[1, ].Equal[NULL]}) {
		/echo \aoNULL Target, exiting!
		/return
	}
	/if (${_sender.Equal[You]}) /varset _sender ${Me.Name}
	|-> #EVENT KillMob "<#1#> KillMob #2#"
	|-> Sender> == #1
	|->_eventParams> == #2
	|-> _eventParams: ${Target.ID} "${Target.Name}" ${Time.Time24}
	/call EngageTarget ${_eventParams.Arg[1, ]}
	/doevents flush KillMob
/return

|-- Engage Target Sub, handles class specific modifications (minor)
Sub EngageTarget(int _targetID)
	/if (!${Bool[${_targetID}]}) /return
	
	| If I am following, stop it!
	/if (${bln_following}) /call StopFollowing
	
	| Target the NPC
	/varset int_targetID ${_targetID}
	/varset str_targetName ${Spawn[id ${_targetID}].Name}
	/varset bln_engaged TRUE
	/echo \awEngaging Target \ag${Spawn[id ${_targetID}].Name} \ay(${_targetID}) \ao${Time.Time24}
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /call TrueTarget ${int_targetID}
	| Stand Up
	/if ((${Me.Feigning} && (${Me.PctHPs} > 75)) || !${Me.Standing}) /stand
	| Reset recast timers (nukes, dots, etc)
	/if (${str_classArchetype.NotEqual[MELEE_END]} || ${Me.Class.ShortName.NotEqual[WAR]}) /call ResetCastTimers
	| Sicc Pet
	/if (${Me.Pet.ID}) /call chkPetAttack
	| Start Swinging
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]} || (${Me.Class.ShortName.Equal[CLR]} && ${bln_battleCleric})) {
		/echo \awKilling Target \ag${Spawn[id ${_targetID}].Name} \ay(${_targetID})  \ao${Time.Time24}
		/squelch /melee reset
		/varset int_maxStickDistance ${Target.MaxMeleeTo}
		/call EngageTargetMelee
	}
	| Stop Sticking if you are MT, cause it's annoying
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
	/if (!${Bool[${Target.ID}]} && !${bln_engaged}) /goto :return
	| No target, or I am at my bound zone, if engaged, kill
	/if (${int_targetID} == 0 || !${Bool[${Target.ID}]} || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) {
		/if (${bln_engaged}) /goto :disengage
		/goto :return
	}
	
	:chkTargetValid
	| Assumes bln_engaged == true
	| Check for target being corpse or non-existant
	/if ((!${Bool[SpawnCount npc id ${int_targetID}]} || ${Spawn[id ${int_targetID}].Type.Equal[corpse]} || ${Spawn[id ${int_targetID}].Name.Find[s_corpse]})) {
		/echo \awNo Targets with ID(\ay${int_targetID}\aw) or is a CORPSE \aoDisengaging!
		/goto :disengage
	} 
	
	| Check to see if my target is NPC and there is no NPCs around
	/if (${Bool[${Target.ID}]} && ${Target.ID} == ${int_MTID} && (!${SpawnCount[npc id ${int_targetID} radius 200]} || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]})) {
		/echo \awTarget is MT, and No NPC in radius 200 (or at bind) \aoDisengaging!
		/goto :disengage
	}

	| Check to see if there are any NPCs on XTarget and Radius
	/if ((${Me.XTarget}==0 && !${SpawnCount[npc radius 200]})) {
		/echo \awNo Targets within 200, or on XTarget \aoDisengaging!
		/goto :disengage
	} 

	| If there is something on XTarget, and its over 400 away, ignore it.
	/if (${Bool[${Me.XTarget}]} && (!${SpawnCount[npc radius 400]}  || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]})) {
		/echo \awNo Targets within 400, even if they are on XTarget \aoDisengaging!
		/goto :disengage
	}

	| Priests can skip next check and just go to check hott aggro
	/if (${Me.Class.ShortName.Equal[SHM]} && ${Me.Class.ShortName.Equal[CLR]} && ${Me.Class.ShortName.Equal[DRU]}) /goto :chkTargetTarget

	| I am engaged, check we are targetting correct NPC
	/if ((${Target.ID} != ${int_targetID}) && (${Spawn[id ${int_targetID}].PctHPs} >= 0 && !${Spawn[id ${int_targetID}].Name.Find[s_corpse]})) {
		| I am not targetting the correct NPC
		/echo \aoCurrent Target \arNOT \ao${Spawn[id ${int_targetID}]}(${int_targetID})\aw, re-targetting
		/call TrueTarget ${int_targetID}
	} 
	
	:chkTargetTarget
	/if (${Target.Type.Equal[NPC]} && ${Me.TargetOfTarget.CleanName.Equal[${Me.CleanName}]} && !${bln_aggro}) {
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
			/if (${Zone.ShortName.Equal[tacvi]} && (${Target.Name.Find[Pirik]} || ${Target.Name.Find[Yihst]} || ${Target.Name.Find[Tunat]})) {
				/varset use_riposte TRUE
			} else /if (${Zone.ShortName.Equal[anguish]} && (${Target.Name.Find[Keldovan]} || ${Target.Name.Find[Ture]} || ${Target.Name.Find[Vangl]} || ${Target.Name.Find[Zun]} || ${Target.Name.Find[Hanvar]} || ${Target.Name.Find[Overlord]})) {
				/varset use_riposte TRUE
			} else /if (${Zone.ShortName.Find[chambers]} &&  ${Target.Name.Find[Master]}) {
				/varset use_riposte TRUE
			} 
			/if (${Me.Class.ShortName.Equal[SHD]}) {
				/if (${Me.AltAbilityReady[Death Peace]})   	/call MQ2Cast "Death Peace" alt
				/if (!${Me.Standing}) /stand
			} else /if (${Me.Class.ShortName.Equal[BER]}) { 
				/if (${use_riposte}) /call FireRiposteDisc
				/if (${Me.CombatAbilityReady[Baffling Strike]} && ${FindItemCount[Axe of the Destroyer]}>0) /disc Baffling Strike
				/if (${Me.CombatAbilityReady[Confusing Strike]} && ${FindItemCount[Rage Axe]}>0) 			/disc Confusing Strike
				/if (${Me.CombatAbilityReady[Distracting Strike]} && ${FindItemCount[Bonesplicer Axe]}>0) 	/disc Distracting Strike
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
		/call Disengage
		/goto :return
	:retarget
	
	:return
/return


|-------------------------------------------------------------------------------- 
|SUB: Event_BackOff!
|--------------------------------------------------------------------------------

Sub Event_BackOff
	/doevents flush BackOff
	/call Disengage
/return

Sub EVENT_Disengage
	/doevents flush Disengage
	/call Disengage
/return 

Sub Disengage 
	/echo \awDisengaging \ao${Time.Time24}
	|-- global reset
	/varset bln_engaged 		FALSE
	/varset int_targetID		0
	/varset str_targetName		NULL
	/varset str_debuffedTargets NULL
	
	|-- clean up potential XTarget issues
	/if (!${bln_pulling} && (!${SpawnCount[npc radius 200]} || (${Me.XTarget[0].Name.Find[s_corpse]} || (!${Bool[${Me.XTarget[0]}]} && ${Bool[${Me.XTarget[1]}]}) || (!${Bool[${Me.XTarget[0]}]} && !${Bool[${Me.XTarget[1]}]} && ${Bool[${Me.XTarget[2]}]})) && (!${timer_xtargetRefresh}))) {
		/call RefreshXTarget
	}
	|-- class archetype resets
	/if (${str_classArchetype.Equal[PRIEST]}) {
		| Do nothing
	} else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/squelch /stick off
		/squelch /melee reset
		/squelch /target clear
	} else /if (${str_classArchetype.Equal[CASTER]}) {
		/if (${Me.Pet.ID}) {
			/squelch /pet back off
			/squelch /pet hold on
		}
		/squelch /target clear
	} 
/return


Sub chkMobDead
	/if ((${SpawnCount[npc radius 200]} == 0 || ${Me.XTarget} == 0) && ${bln_engaged} ) {
		/echo chkMobDead triggered XTarget:${Me.XTarget} || SpawnCount:!${SpawnCount[npc id ${int_targetID} radius 250]}  && engaged:${bln_engaged}
		/call Disengage
	}
/return

Sub TrueTarget(int _targetID)
	|/echo TrueTarget(${_targetID})
	| Check if the target is in zone.
	/if (${Target.ID} == ${_targetID}) /return
	/if (${SpawnCount[id ${_targetID}]}) {
		/declare retryTimer timer local 30
		:Target_Loop
		|/echo targetting ${_targetID}
		/squelch /target id ${_targetID}
		/delay 2 ${Target.ID} == ${_targetID}
		
		/if (${Target.ID} != ${_targetID} && ${SpawnCount[id ${_targetID}]}) {
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
	/if (${Bool[${SpawnCount[PC ${str_MTName}]}]} && (${int_MTID} == ${Spawn[PC ${str_MTName}].ID})) /goto :return 

	|--MT, ST, and TT are not in zone or dead!
	/if (${Bool[${SpawnCount[PC ${str_MTName}]}]} && ${Bool[${SpawnCount[PC ${str_STName}]}]} && ${Bool[${SpawnCount[PC ${str_TTName}]}]}) {
		/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		/echo \arALL TANKS ARE DEAD!
		/call getMTbyPrecedence
		/varset int_MTID ${Spawn[PC ${Macro.Return}].ID}
		/varset str_MTName ${Spawn[PC ${Macro.Return}].Name}
		/goto :hasChanged
	}

	|--MT is alive but ID is wrong; 
	/if (${Bool[${SpawnCount[PC ${str_MTName}]}]} && (${int_MTID} != ${Spawn[PC ${str_MTName}].ID})) {
		/echo \ay MT ${str_MTName} is alive, and my MTID ${int_MTID} is wrong
		/varset int_MTID ${Spawn[${str_MTName}].ID}
		/goto :hasChanged
	}

	|--ST is still alive; 
	/if (${Bool[${SpawnCount[PC ${str_STName}]}]} && (${int_MTID} != ${Spawn[PC ${str_STName}].ID})) {
		/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		/echo \ao ST  is still alive, switching!
		/varset str_MTName ${str_STName}
		/varset str_STName ${str_TTName}
		/varset int_MTID ${Spawn[${str_STName}].ID}
		/goto :hasChanged
	}
	
	|--TT is still alive; 
	/if (${Bool[${SpawnCount[PC ${str_TTName}]}]} && (${int_MTID} != ${Spawn[PC ${str_TTName}].ID})) {
		/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		/echo \ao TT is still alive, switching!
		/varset str_MTName ${str_TTName}
		/varset int_MTID ${Spawn[${str_TTName}].ID}
		/goto :hasChanged
	}
	:hasChanged
	/if (${Me.Name.Equal[${str_MTName}]}) {
		/varset bln_tanking TRUE
		/melee aggro=1
		/melee taunt=1
	}
	:return
	|-- If set as target, target the new MT immediately
	/if (${_setAsTarget} && (${Target.ID} != ${int_MTID})) {
		/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		/call TrueTarget ${int_MTID}
	}
/return




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
		/rsay ${_eventParams.Arg[1,|]}:${Me.Skill[${_eventParams.Arg[1,|]}]}
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
	/doevents flush SetupRaid
/return 


Sub SetupRaid(string _mode)
	|-- Pull from the common ini settings file to grab the group leaders/di clerics etc
	|/echo Setting raid group mode to ${_mode}
	|/varset _mode TRIAD
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
	/doevents flush EndMac
/return




| AA Special Events
Sub EVENT_UseStaunchRecovery
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /return

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
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley ALT "Intensity of the Resolute" ${int_targetID} TRUE
	} else {
		/casting "Intensity of the Resolute" alt
	}
/return

Sub EVENT_UseServants
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley ALT "Steadfast Servant" ${int_targetID} FALSE
	} else {
		/casting "Steadfast Servant" alt
	}
/return

Sub EVENT_UseInfusion
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /return
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
	/if (!${mq2meleeEnabled} && (${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${Target.DisplayName}]}) && (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]})) {
		|/attack off
	}
	/if (((${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${Target.DisplayName}]}) && ${Me.Pet.ID}) && !${Me.PetBuff[Master's Aura Effect]}) {
		/echo Enrage detected! Backing pet off.
		/pet back off
		/varset bln_enraged TRUE
	}
	/doevents flush RageOn
/return

Sub Event_RageOff(string line)
	/if (!${mq2meleeEnabled} && (${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${Target.DisplayName}]}) && (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]})) {
		/attack on
	}
	/if ((${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${Target.DisplayName}]}) && ${Me.Pet.ID}) {
		/pet attack
		/varset bln_enraged FALSE
	}
	/doevents flush RageOff
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
	/doevents flush DuckOn
/return

Sub EVENT_DUCKOFF
	/echo end ducking!
	/if (${Me.Ducking}) /keypress x
	/varset bln_ducking FALSE
	/doevents flush DuckOff
/return



|-------------------------------------------------------------------------------- 
|SUB: Event_ZONE HANDLERS
|--------------------------------------------------------------------------------
Sub Event_Zoned
	/echo \awEvent_Zoned ${Time.Time24}
	/call setExclusions
	/if (${MacroQuest.GameState.Equal[CHARSELECT]}) /end
  	/doevents flush Zoned
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
Sub setMT(string _tankName)
	/echo setMT(${_tankName}):: ${Spawn[${_tankName}].ID}
	/varset str_MTName ${_tankName}
	/varset int_MTID ${Spawn[${_tankName}].ID}
/return
	
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
	/declare name string local ${_eventParams.Arg[1, ]}
	/varset	int_MTID ${Spawn[pc ${name}].ID}
	/varset str_MTName ${name} 

	/echo MTID: ${int_MTID} MTName: ${name}
	/if (${str_classArchetype.Equal[PRIEST]}) /dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${healPoint}
	/if (${Me.Class.ShortName.Equal[CLR]}) /call TrueTarget ${int_MTID}
	/if (${Me.Name.Equal[${name}]}) {
		/varset bln_tanking TRUE
		/varset bln_aggro 	TRUE
		/melee aggro=1
		/melee taunt=1
	} else {
		/varset bln_tanking FALSE
		/varset bln_aggro 	FALSE
	}
	/doevents flush ChangeMT
/return

Sub EVENT_ChangeSA(_line, _sender, _eventParams)
	/echo SA Change called! Changing to: ${_eventParams.Arg[1, ]}
	/echo _sender: ${_sender}
	/echo _eventParams:${_eventParams}
	/declare SA string local ${_eventParams.Arg[1, ]}
	/varset	int_STID ${Spawn[${SA}].ID}
	/doevents flush ChangeSA
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
	/doevents flush ChangeAP
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
	/dquery ${str_MTName} -q "Target.ID"
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
	/doevents flush shrinkGroup
/return
|-------------------------------------------------------------|
|- Reliably determine if target is Named				     -|
|-------------------------------------------------------------|
|- @param TargetID: The ID of the spawn you wish to check on
|-------------------------------------------------------------|
Sub TargetNamedCheck(_id)
	/if ((${Spawn[npc id ${_id}].Named} || ${Spawn[npc id ${_id}].Name.Find[#]}) && ${Target.Level} >= 75) /return TRUE
	/if (${Zone.ShortName.Equal[tacvi]} && ${Target.Level} >= 70) /return TRUE
	/if (${Zone.ShortName.Equal[chambersa]} || ${Zone.ShortName.Equal[chambersb]} || ${Zone.ShortName.Equal[chambersc]} || ${Zone.ShortName.Equal[chambersd]} || ${Zone.ShortName.Equal[chamberse]}  && ${Target.Level} >= 70) /return TRUE
	/if (${Zone.ShortName.Equal[anguish]}  && ${Target.Level} >= 75) /return TRUE
	/if (${Zone.ShortName.Equal[arena]}) /return TRUE
	/if (${Zone.ShortName.Equal[thundercrest]}) /return TRUE
/return FALSE
	



|-----------------------
|- Counts ini entries and creates a corresponding array
|-----------------------
|- @param Ini_Key: The ini file, section, and entry to create an array from
|- @param ArrayName: The name of the array you want to declare
|- @param ArrayScope: The scope of the array to be declared
|-----------------------
|- Syntax: /call IniToArray IniKey ArrayName VariableScope
|-	Ex: /call IniToArray "Settings.ini,General Settings,Debug Mode" Debug outer
|-		/call IniToVar Bot_Killians.ini,Buffs,SelfBuffs MySelfBuffs local
|-----------------------
Sub IniToArray(Ini_Key, ArrayName, ArrayScope)
	| Check for the specified ini key.
	/if (!${Ini[${Ini_Key}1].Length}) {
		/echo [${Ini_Key}] not found.
	} else {
	
		| Count ini entries
		/declare i int local 1
		/declare count int local

		:CountingLoop
		/if (${Ini[${Ini_Key}${i}].Length}) {
			/varset count ${i}
			/varcalc i ${i} + 1
			/goto :CountingLoop
		}
		
		| Declare the array and copy ini entries
		/if (${Defined[${ArrayName}]}) /deletevar ${ArrayName}
		/declare ${ArrayName}[${count}] string ${If[${Defined[ArrayScope]},${ArrayScope},outer]}
		
		/for i 1 to ${${ArrayName}.Size}
			/varset ${ArrayName}[${i}] ${Ini[${Ini_Key}${i}]}
		/next i
	}
}
/return ${ArrayName}
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
	/doevents flush CheckNaked
/return

Sub chkNaked
	|/echo chkNaked ${Bool[${Me.Inventory[2].Name}]} ${timer_naked}
	/doevents flush chkNaked
	/if (!${Bool[${Me.Inventory[2].Name}]} && !${timer_naked}) {
		/rs I am NAKED. 
		/varset bln_naked TRUE
		/varset timer_naked 30s
	} else {
		/varset bln_naked FALSE
	}
/return


Sub findslot(string _commandtext)

/return

Sub Event_FixCorpses
	/doevents flush FixCorpses
	/hidecorpse all
	/delay 2
	/hidecorpse none
	/delay 2
	/hidecorpse looted
/return


Sub saveGroup

/return

Sub ClearCursor
	/declare i int local
	:auto_inv
	/if (${Cursor.ID}) {
	/if (${Cursor.Container}) {
		|---- Inventory Bag Slots
		/for i 1 to 10   
		/if (!${InvSlot[pack${i}].Item.Container}) /nomodkey /itemnotify pack${i} leftmouseup
		/next i
		|---- Inventory Bag Slots   
	} else {
		/timed 5 /autoinventory
	}
	/goto :auto_inv
	}
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

|------------------------------------------------------------------------------------|
|- Creates timers, at the specified duration, and adds their names to timerArray.	-|
|------------------------------------------------------------------------------------|
|- @param timerName: Name of the timer you want to declare.							-|
|- @param timerSetting: Setting of the timer you're declaring.						-|
|---------------------------------BB---------------------------------------------------|
SUB createTimer(timerName, timerSetting)
	| Delete old timer.
	/if (${Defined[${timerName}]}) /deletevar ${timerName}

	| Declare new timer.
	/declare ${timerName} timer global ${timerSetting}
	
	| Add the timer's name to the timerArray.
	/call BuildArray timerArray ${timerName} global
/return



|----------------------------------------------------|
|- Deletes spent timers made by 'SUB createTimer'.	-|
|----------------------------------------------------|
SUB check_macroTimers
	/declare i int local
	/declare SpentTimerCounter int local

	| Check each entry of the timer array, count spent timers.	
	/for i 1 to ${timerArray.Size}
		/if (!${${timerArray[${i}]}}) {
			/varcalc SpentTimerCounter ${SpentTimerCounter} + 1
			/if (${Defined[${timerArray[${i}]}]}) {
				/deletevar ${timerArray[${i}]}
			}
		}
	/next i
	
	| If all the timers listed on timerArray are spent, delete timerArray.
	/if (${SpentTimerCounter}==${timerArray.Size}) {
		/deletevar timerArray
	}
/return



|----------------------------------------------------|
|- Resets XTarget window 1-5 elements to auto		-|
|----------------------------------------------------|
Sub Event_RefreshXTarget

	/call RefreshXTarget
/return 


SUB RefreshXTarget
	/echo \aoRefreshing \aoXTarget \ay${timer_xtargetRefresh}
	/declare i int local 1
	/for i 1 to 5
		/xtarget set ${i} grouptank
		/delay 1
		/xtarget set ${i} autohater
	/next i
	/varset timer_xtargetRefresh 30s
/return

|--------------------------------------------------------|
|- Creates new arrays, or stacks onto existing arrays.	-|
|--------------------------------------------------------|
|- Syntax: /call BuildArray ArrayName Data				-|
|-  Ex: /call BuildArray DebuffTargets ${Target.ID}		-|
|--------------------------------------------------------|
|- @param ArrayName: Name of the array to build.		-|
|- @param Data: Data you wish to add to the array.		-|
|--------------------------------------------------------|
SUB BuildArray(ArrayName, Data, varScope)
	/if (!${Defined[varScope]}) /declare varScope string local outer

	| If the array is not defined, declare it.
	/if (!${Defined[${ArrayName}]}) {
		/declare ${ArrayName}[1] string ${varScope}
		/varset ${ArrayName}[1] ${Data}
		
	| Else, if the array is already defined.
	} else {
	
		| Create a copy of the existing array.
		/declare ${ArrayName}Copy[${${ArrayName}.Size}] string local
		/declare i int local
		/for i 1 to ${${ArrayName}.Size}
			/varset ${ArrayName}Copy[${i}] ${${ArrayName}[${i}]}
		/next i
		
		| Delete the existing array.
		/deletevar ${ArrayName}
		
		| Build a resized array.
		/declare ${ArrayName}[${Math.Calc[${${ArrayName}Copy.Size}+1].Int}] string ${varScope}
		/for i 1 to ${${ArrayName}.Size}
			/varset ${ArrayName}[${i}] ${${ArrayName}Copy[${i}]}
		/next i
	
		| Insert new data into the array
		/varset ${ArrayName}[${${ArrayName}.Size}] ${Data}
	}
/return ${ArrayName}



|----------------------------------------------------|
|- Removes elements from an existing array.			-|
|----------------------------------------------------|
|- @param ArrayName: Name of the array to search.	-|
|- @param Data: Data you wish to remove.			-|
|----------------------------------------------------|
SUB RemoveArrayElement(ArrayName, Data)
	/declare remainingElements string local
	/declare newArraySize int local

	/declare i int local

	| Count array elements that do not match 'Data'.
	/for i 1 to ${${ArrayName}.Size}	

		/if (${${ArrayName}[${i}].NotEqual[${Data}]}) {
			/varset remainingElements ${remainingElements}${${ArrayName}[${i}]},
			/varcalc newArraySize ${newArraySize} + 1
		}

	/next i
	
	| If there are  elements remaining in the array, create a copy of the new array
	/if (${newArraySize}) {
	
		/declare newArray[${newArraySize}] string local
		
		/for i 1 to ${newArraySize}
		
			/varset newArray[${i}] ${remainingElements.Arg[${i},,]}
		
		/next i
	}
	
	| Delete the existing array.
	/deletevar ${ArrayName}
	
	| If there is a new array to make
	/if (${Defined[newArray]}) {

		/declare ${ArrayName}[${newArray.Size}] string outer
	
		/for i 1 to ${newArray.Size}
		
			/varset ${ArrayName}[${i}] ${newArray[${i}]}
		
		/next i
	}
/return

Sub EVENT_SaveRingset
	/call SaveRingset
/return

Sub SaveRingset
	
	/declare leftfinger string local ${Me.Inventory[leftfinger].Name}
	/declare rightfinger string local ${Me.Inventory[rightfinger].Name}

	/echo \aoSaving Ringset \awleft: \ag${leftfinger} \awright: \ag${rightfinger}
	/ini INI_CHAR "SavedInventory" "leftfinger" "${leftfinger}"
	/ini INI_CHAR "SavedInventory" "rightfinger" "${rightfinger}"
/return

Sub EVENT_RemoveRingset
	/call RemoveRingset
/return

Sub RemoveRingset
	/varset Ring_1_Name ${Me.Inventory[leftfinger].Name}
	/varset Ring_2_Name ${Me.Inventory[rightfinger].Name}
	/declare notemptyCount int local 0
	:notempty
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :notempty

	/echo \aoRemoving \ar${Ring_1_Name} \ayfrom leftfinger
	/unequip leftfinger
	/delay 10
	/autoinventory

	/echo \aoRemoving \ar${Ring_2_Name} \ayfrom rightfinger
	/unequip rightfinger
	/delay 10
	/autoinventory
	
/return

|-- method to load the saved rings from INI (primarily for Trial of Foresight)
Sub EVENT_LoadRingset(string _line, string _sender, string _eventParams)
	/echo \ayLoading Ringset ${_eventParams}
	/call LoadRingset
/return

Sub RestoreFaceSlot
	/exchange "${strFaceItem}" face
	/delay 5
/return 

Sub LoadRingset

	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/echo ${Ring_1_Name},  ${Ring_2_Name}

	/declare notemptyCount int local 0
	:notempty
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :notempty

	/if (${leftfinger.NotEqual[NULL]}) {
		 /exchange "${Ring_1_Name}" leftfinger
	}
	/delay 10
	/if (${rightfinger.NotEqual[NULL]}) {
		/exchange "${Ring_2_Name}" rightfinger
	}
	/echo \agSaved Ringset Equipped 
	/autoinventory
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley
/return 


|-- method to load the saved weapons from INI (primarily for Trial of Specialization)
Sub EVENT_LoadWeaponset(string _line, string _sender, string _evtParams)
	/echo \ayLoading Weaponset <${_evtParams}> (${_sender})
	/if (${_evtParams.NotEqual[NULL]}) /call LoadWeaponset ${_evtParams}
/return


Sub LoadWeaponset(string _weaponset)
	|/if (${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[PRIEST]} && ${_weaponset.NotEqual[default]}) /return
	/if (${_weaponset.Equal[NULL]}) /varset _weaponset default
	/declare set string local ${Ini[INI_CHAR,Weaponsets,${_weaponset}]}
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
		/echo unequip mainhand
		/unequip mainhand
	}
	/delay 12
	/if (${Me.Inventory[offhand].Name.NotEqual[NULL]}) {
		/echo unequip offhand
		/unequip offhand
	}

	/if (${_weaponset.Equal[EMPTY]}) /goto :return

	|-- auto inventory
	/autoinventory
	/delay 12
	:offhand
	/echo handling offhand first
	|-- do secondary first in case its 2 handed weapon
	/varset slot ${set.Token[2,,]}
	/varset weap ${slot.Token[1,|]}
	/varset posi ${slot.Token[2,|]}
	/if (${weap.Equal[NULL]}) {
		/autoinventory
		/echo \ay Weaponset ${_weaponset} offhand is NULL! Removing.
		/if (${Me.Inventory[offhand].Name.NotEqual[NULL]}) {
			/echo unequip offhand
			/unequip offhand
		} else {
			/echo Offhand was already Empty!
		}
		
	} else {
		/autoinventory
		/echo \ag Moving "${weap}" to ${posi}
		/if (${FindItemCount[${weap}]} == 0) {
			/rs Could not find ${weap}, fix my ini!
			/goto :mainhand
		} else {
			/exchange "${weap}" ${posi}
		}
		
	} 
	/delay 10
	:mainhand
	/echo handling mainhand
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
		} else {
			/echo Mainhand was already Empty!
		}
	} else {
		/autoinventory
		/echo \ag Moving "${weap}" to ${posi}
		/if (${FindItemCount[${weap}]} == 0) {
			/rs Could not find ${weap}, fix my ini!
			/goto :return
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
		/rs ${Me.Skill[${_eventParams}]}
		/nav wp lobby
	}
	/doevents flush SkillCheck

/return