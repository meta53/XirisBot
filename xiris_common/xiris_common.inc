|**
----------------------------
xiris_common.inc version 09.17.18
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
______________________________________________

**|


#define botINI xiris_class_ini/BOT_${Me.Class.ShortName}_${Me.CleanName}.ini

#include xiris_common/xiris_buffing.inc
#include xiris_common/xiris_buffing_lines.inc
#include xiris_common/xiris_curing.inc
#include xiris_common/xiris_casting.inc
#include xiris_common/xiris_debuffing.inc
#include xiris_common/xiris_events.inc 
#include xiris_common/xiris_exclude.inc
#include xiris_common/xiris_healing.inc
#include xiris_common/xiris_looting.inc
#include xiris_common/xiris_melee.inc
#include xiris_common/xiris_offtank.inc
#include xiris_common/xiris_pets.inc
#include xiris_common/xiris_follow.inc

#include xiris_common/xiris_spell_routines.inc

|COMMON
#EVENT KillMob "<#1#> KillMob #2#"
#EVENT KillMob "[#1#(msg)] KillMob #2#"
#EVENT KillMob "#1# tells you, 'KillMob #2#'"
#EVENT KillMob "#1# tells the group, 'KillMob #2#'"
#EVENT KillMob "#1# you tell your party, 'KillMob #2#'"

#EVENT Zoned				"You have entered#*#"
#EVENT Zoned				"LOADING, PLEASE WAIT..."
#EVENT EndMac				"#*#end macro#*#"

#EVENT BackOff				"<#1#> BackOff"
#EVENT BackOff				"[#1#(msg)] BackOff"

#EVENT ChangeMT "<#1#> changeMT #2#"
#EVENT ChangeMT "[#1#(msg)] changeMT #2#"
#EVENT ChangeMT "#1# tells you, 'changeMT #2#'"
#EVENT ChangeMT "#1# tells the group, 'changeMT #2#'"
#EVENT ChangeMT "#1# you tell your party, 'changeMT #2#'"

#EVENT ChangeSA "<#1#> changeSA #2#"
#EVENT ChangeSA "[#1#(msg)] changeSA #2#"
#EVENT ChangeSA "#1# tells you, 'changeSA #2#'"
#EVENT ChangeSA "#1# tells the group, 'changeSA #2#'"
#EVENT ChangeSA "#1# you tell your party, 'changeSA #2#'"

#EVENT ChangeAP "<#1#> changeAP #2#"
#EVENT ChangeAP "[#1#(msg)] changeAP #2#"
#EVENT ChangeAP "#1# tells you, 'changeAP #2#'"
#EVENT ChangeAP "#1# tells the group, 'changeAP #2#'"
#EVENT ChangeAP "#1# you tell your party, 'changeAP #2#'"




|MISC events
#EVENT ConnectEQBC			"#*#ConnectEQBC#*#"
#EVENT CheckNaked			"#*#CheckNaked#*#"
#EVENT BurnOnALL			"#*#BurnOnAll#*#"
#EVENT BurnOn1				"#*#BurnOn1#*#"
#EVENT BurnOn2				"#*#BurnOn2#*#"
#EVENT BurnOff				"#*#BurnOff#*#"
#EVENT IntensityOn			"#*#IntensityOn#*#"
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

Sub InitEvents_Common
	/call RegisterRaidEvents Events_Raid 9
	/call RegisterBackgroundEvents Events_Common 17
/return

Sub Events_Raid 
	/doevents BurnOnALL
	/doevents BurnOn1
	/doevents BurnOn2
	/doevents BurnOff
	/doevents BackOff
	/doevents ChangeMT
	/doevents ChangeSA
	/doevents ChangeAP
	/doevents IntensityOn
/return

Sub Events_Common 
	/doevents KillMob
	/doevents Zoned	
	/doevents CheckNaked
	/doevents RageOff
	/doevents RageOn
	/doevents ImDead
	/doevents ImDead
	/doevents ImDead
	/doevents LoadRingset
	/doevents LoadWeaponset
	/doevents SaveWeaponset
	/doevents SaveCurrentWeaponset
	/doevents SaveRingset
	/doevents InvisOFF
/return


|**
 ----------------------------
 INITIALIZATION Sub
 ----------------------------
 xbot_initialize has all the common variables
 used in the bot set. Unioned with all the modern macros
 ----------------------------
**|   

|Sub xbot_initialize(string _strTankName, int _intAssistAt, int _intTankHealPoint, string _strTankName2)
Sub xbot_initialize(string _tank1Name, string _tank2Name, string _tank3Name, int _healPoint, string _assistArgs) 
	/cecho \aoxbot_initialize 1:${_tank1Name} 2:${_tank2Name} 3:${_tank3Name} 4:${_healPoint} 5:${_assistArgs}

	|-- [Settings] : Communications & INI Declarations
	/declare Debug 						bool	outer 	FALSE
	/declare Verbosity					bool	outer	FALSE
	/declare cmdChannel 				string	outer	${Ini[botINI,Settings,cmdChannel,bc]}
	
	|-- [Settings] : Class Archetype (used in case switching)
	/declare 	str_classArchetype		string	outer	UNKNOWN
	/call 	 	getClassArchetype 		${Me.Class.ShortName}
	/varset 	str_classArchetype		${Macro.Return}
	/declare 	MeleeClasses 			string	outer	BRD,BER,BST,MNK,PAL,RNG,ROG,SHD,WAR
	/declare 	PriestClasses 			string	outer	CLR,DRU,SHM
	/declare 	CasterClasses 			string	outer	ENC,MAG,NEC,WIZ
	/declare 	RangedClasses			string	outer	RNG
	
	|-- [Settings] :Tank Settings
	/declare	bln_tanking				bool	outer	FALSE
	/if (${_tank1Name.Equal[${Me.Name}]}) /varset bln_tanking TRUE
	/declare	str_defTank1			string	outer	${Ini[botINI,Settings,defaultTank1,Xiris]}
	/declare	str_defTank2			string	outer	${Ini[botINI,Settings,defaultTank2,Xiria]}
	/declare	str_defTank3			string	outer	${Ini[botINI,Settings,defaultTank3,Xirea]}
	/declare 	str_tankName1			string	outer 	${str_defTank1}
	/declare	str_tankName2			string	outer 	${str_defTank2}
	/declare 	str_tankName3			string  outer	${str_defTank3}

	|-- [Settings] :Insert default values for downstream methods if we arent getting all the params
	/if (${_tank1Name.Length} && ${_tank1Name.NotEqual[NULL]}) /varset str_tankName1 ${_tank1Name}
	/if (${_tank2Name.Length} && ${_tank2Name.NotEqual[NULL]}) /varset str_tankName2 ${_tank2Name}
	/if (${_tank3Name.Length} && ${_tank3Name.NotEqual[NULL]}) /varset str_tankName3 ${_tank3Name}


	|-- ID and Name here keep track of *current* tank/second tank. They are swapped as needed.
	/declare	int_MTID           		int		outer	${Spawn[${str_tankName1}].ID}
	/declare 	str_MTName				string  outer  	${str_tankName1}	
	/declare 	int_STID				int		outer	${Spawn[${str_tankName2}].ID}
	/declare 	str_STName				string  outer 	${str_tankName2}
	/declare	int_TTID				int	 	outer 	${Spawn[${str_tankName3}].ID}
	/declare 	str_TTName				string	outer 	${str_tankName3}

	|-- [Settings] :Auto Assist Settings
	/declare 	bln_autoAssist			bool	outer 	FALSE
	/declare	int_defAssistAt			int		outer   ${Ini[botINI,Settings,assistAt,98]}
	/declare 	int_assistAt			int		outer   ${int_defAssistAt}
	/if (${_assistArgs.Length}) {
		/cecho \agAutoAssist settings: ${_assistArgs}
		/varset bln_autoAssist ${_assistArgs.Token[1,|]}
		/varset int_assistAt ${_assistArgs.Token[2,|]}
	} else {
		/echo No autoassist settings defined.
	}
	
	
	|-- [Settings] : Miscellaneous Defaults
	/declare	str_spellSet			string	outer	${Ini[botINI,Settings,str_spellSet,default]}
	/declare	bln_engaged				bool	outer	FALSE
	/declare	bln_enraged				bool	outer	FALSE
	/declare 	int_targetID			int		outer 	0
	/declare 	aggroAnim 				string 	outer 	|5|8|12|17|18|42|44|80|106|129|144|
	/declare    bln_naked               bool	outer   FALSE
	/declare 	bln_ducking				bool	outer	FALSE
	/declare	timer_naked 			timer   outer	30s

	|-- [Settings] : Meditation
	/declare int_sitRadius   		int     outer ${Ini[botINI,Settings,sitRadius,50]}
	/declare int_medPercent       	int     outer ${Ini[botINI,Settings,medPercent,100]}
	/declare timer_medDelay			timer	outer ${Ini[botINI,Settings,medDelay,45s]}
	/declare timer_sitDelay			timer	outer ${Ini[botINI,Settings,sitDelay,10s]}
	
	|-- [Settings] : Camp
	/declare bln_useCAMP			bool	outer 	FALSE
	/declare int_campX 				int 	outer 	${Me.X}
	/declare int_campY 				int 	outer 	${Me.Y}
	/declare int_campZ 				int 	outer 	${Me.Z}
	
	|-- [Settings] : Leashing
	/declare bln_useLeash			bool	outer ${Ini[botINI,Settings,leashUse,FALSE]}
	/declare int_leashLen			int		outer ${Ini[botINI,Settings,leashLength,100]}
	/declare int_leashX				int		outer ${Me.X} 
	/declare int_leashY				int		outer ${Me.Y}
	/declare int_leashZ				int		outer ${Me.Z}
	/declare int_leashHeading		int		outer ${Me.Heading.DegreesCCW}
	/declare int_leashZoneID		int		outer ${Zone.ID}
	/declare bln_leashCamp			bool	outer ${Ini[botINI,Settings,leashCamp,FALSE]}
	/declare bln_leashAutoSit		bool	outer ${Ini[botINI,Settings,leashAutoSit,TRUE]}
	/declare SZ_shortName			string	outer ${Zone.ShortName}
	
	|-- [Settings] : AAs & CAs
	/declare	int_aaStopHP		int		outer	${Ini[botINI,AA,int_aaStopHP,5]}
	/declare	int_aaStartHP		int		outer	${Ini[botINI,AA,int_aaStartHP,99]}
	/declare 	int_aaNamedStopHP	int		outer 	${Ini[botINI,AA,int_aaNamedStopHP,5]}
	/declare    int_aaNamedStartHP	int		outer 	${Ini[botINI,AA,int_aaNamedStartHP,99]}
	/declare 	int_aaTotal			int		outer	${Ini[botINI,AA,int_aaTotal,0]}
		/declare aa int local
			/for aa 1 to ${int_aaTotal}
				/declare AA_${aa} 				string 	outer ${Ini[botINI,AA,AA_${aa}]}
				/declare AA_${aa}_Use 			bool 	outer ${Ini[botINI,AA,AA_${aa}_Use]}
				/declare AA_${aa}_Gem 			string 	outer ${Ini[botINI,AA,AA_${aa}_Gem]}
				/declare AA_${aa}_NamedOnly		int		outer ${Ini[botINI,AA,AA_${aa}_NamedOnly]}
				/declare AA_${aa}_MyHP			int		outer ${Ini[botINI,AA,AA_${aa}_MyHP,100]}
				/declare AA_${aa}_StartHP		int		outer ${Ini[botINI,AA,AA_${aa}_StartHP]}
				/declare AA_${aa}_StopHP		int		outer ${Ini[botINI,AA,AA_${aa}_StopHP]}
			/next aa
			
	|- [Settings] : OffensiveClickies
	/declare	int_clickStopHP		int		outer	${Ini[botINI,OffensiveClickies,int_clickStopHP,5]}
	/declare	int_clickTotal		int		outer	${Ini[botINI,OffensiveClickies,int_clickTotal,0]}
		/declare oc int local
			/for oc 1 to ${int_clickTotal} 
				/declare Click_${oc} 			string 	outer ${Ini[botINI,OffensiveClickies,Click_${oc}]}
				/declare Click_${oc}_Use		bool 	outer ${Ini[botINI,OffensiveClickies,Click_${oc}_Use]}
				/declare Click_${oc}_StartHP 	int 	outer ${Ini[botINI,OffensiveClickies,Click_${oc}_StartHP]}
				/declare Click_${oc}_StopHP  	int 	outer ${Ini[botINI,OffensiveClickies,Click_${oc}_StopHP]}
				/declare Click_${oc}_NamedOnly  bool	outer ${Ini[botINI,OffensiveClickies,Click_${oc}_NamedOnly]}
				/declare Click_${oc}_Recast 	int		outer ${Ini[botINI,OffensiveClickies,Click_${oc}_Recast]}
				/declare Click_${oc}_Timer  	timer	outer 0
			/next oc

	|- [Settings] : BurnMode
	/declare bln_BURNMode			bool	outer FALSE
	/declare int_burnModeTotal int outer ${Ini[botINI,BurnMode,int_burnModeTotal,0]}
	/declare str_burnKey       int outer ${Ini[botINI,BurnMode,str_burnKey,ALL]}
	
		/declare bm int local
		/for bm 1 to ${int_burnModeTotal} 
			/declare Burn_${bm} 			string	outer ${Ini[botINI,BurnMode,Burn_${bm}]}
			/declare Burn_${bm}_Type 		string	outer ${Ini[botINI,BurnMode,Burn_${bm}_Type]}
			/declare Burn_${bm}_Use 		bool	outer ${Ini[botINI,BurnMode,Burn_${bm}_Use]}
			/declare Burn_${bm}_Num 		bool	outer ${Ini[botINI,BurnMode,Burn_${bm}_Num]}
			/declare Burn_${bm}_After 		int		outer ${Ini[botINI,BurnMode,Burn_${bm}_After]}
			/declare Burn_${bm}_MGB			bool	outer ${Ini[botINI,BurnMode,Burn_${bm}_MGB,FALSE]}
		/next bm
			
	|--include the custom event initializations & other things
	/declare bln_outside			bool	outer	TRUE
	/declare bln_slowUse			bool	outer	FALSE
	/declare bln_slowed				bool	outer	FALSE
	/declare int_slowResisted		int		outer	0
	/declare int_slowMaxTries		int		outer	3
	

	|-- init common events
	/call xbot_initialize_events
	/call InitEvents_Common

	|-- init other libraries
	/call xbot_initialize_spellRoutines
	/call xbot_initialize_buffing
	/call xbot_initialize_bufflines
	/call xbot_initialize_casting
	/call xbot_initialize_curing
	/call xbot_initialize_debuffing
	/call xbot_initialize_exclusions
	/call xbot_initialize_healing ${_healPoint}
	/call xbot_initialize_looting
	/call xbot_initialize_melee
	/call xbot_initialize_offtanking
	/call xbot_initialize_pets
	/call xbot_initialize_following

	|-- report that the common things are initialized
	/cecho \agxbot_initialize: [ MT: ${str_MTName}(${int_MTID}) @ ${int_point_tankNormal} ] STName: ${str_tankName2}(${int_STID})

/return


|--Engage Target EVENT, calls engageTarget which is in the class macros since each class handles this differently
Sub EVENT_KillMob(_line, _sender, _eventParams)
	/cecho \ayKMCalled ${_eventParams}
	|-> #EVENT KillMob "<#1#> KillMob #2#"
	|-> Sender> == #1
	|->_eventParams> == #2
	|-> _eventParams: ${Target.ID} "${Target.Name}" ${Time.Time24}
	/call engageTarget ${_eventParams.Arg[1, ]}
	/doevents flush KillMob
/return


|**
 ----------------------------
 Fail States
 ----------------------------
 Check things that should be/should not be
 ----------------------------
**|

Sub chkEmotes
	|/doevents
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
		| Check Mod Rods Rod of Spectral Transvergence
		/if (!${Me.Invis} && ${Me.PctMana}<=${int_medPercent} && ${FindItem[Wand of Spectral Transvergance].InvSlot} && !${FindItem[Wand of Spectral Transvergance].TimerReady}) /call MQ2Cast "Wand of Spectral Transvergance" item
		/if (!${Me.Invis} && ${Me.PctMana}<=${int_medPercent} && ${FindItem[Wand of Ethereal Transvergance].InvSlot} && !${FindItem[Wand of Ethereal Transvergance].TimerReady}) /call MQ2Cast "Wand of Ethereal Transvergance" item
		/if (!${Me.Invis} && ${Me.PctMana}<=${int_medPercent} && ${FindItem[Wand of Elemental Transvergance].InvSlot} && !${FindItem[Wand of Elemental Transvergance].TimerReady}) /call MQ2Cast "Wand of Elemental Transvergance" item
		/if (!${Me.Invis} && ${Me.PctMana}<=${int_medPercent} && ${FindItem[Rod of Mystical Transvergance].InvSlot} && !${FindItem[Rod of Mystical Transvergance].TimerReady}) /call MQ2Cast "Rod of Mystical Transvergance" item
		/if (!${Me.Invis} && ${Me.PctMana}<=${int_medPercent} && ${FindItem[Summoned: Modulating Rod].InvSlot} && !${FindItem[Summoned: Modulating Rod].TimerReady}) /call MQ2Cast "Summoned: Modulating Rod" item
	
		| Sit my ass down
		/if ( ${Me.PctMana}<=${int_medPercent} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sitRadius} zradius 55]} == 0) && (${Melee.Casted} > 3000) ) {
			|/echo I should be sitting my ass down for MANA Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit
			/delay 2s
		} 
		
	}
	|-- Endurance Check
	/if (${str_classArchetype.Equal[MELEE_END]} ||  ${str_classArchetype.Equal[TANK]}) {
		/if ( ${Me.PctEndurance}<=${int_medPercent} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sitRadius} zradius 55]} == 0) && (${Melee.Casted} > 3000) ) {
			|/echo I should be sitting my ass down for Endurance Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit on
			/delay 2s
		} 
	}
/return



|**
 ----------------------------
 Target Checking
 ----------------------------
 Determines if the main assist has a new target, and whether we can attack
 ----------------------------
**|
Sub chkTarget
    /if ((!${Target.ID} || ${Target.Type.NotEqual[NPC]}) && ${bln_engaged}) {
		/echo Mob is dead. Engage off.
        /varset bln_engaged FALSE
		/varset int_targetID 0
    }
	/if (${Cursor.ID}) /autoinventory
/return

|**
 ----------------------------
 Assist Checking
 ----------------------------
 Determines if the main assist is dead, if so, set new MA
 ----------------------------
**|
Sub chkMTAlive(bool _setAsTarget)

	|--MT is still alive; ignore
	/if (${Spawn[PC ${str_tankName1}].ID}) {
		/if (${_setAsTarget} && (${Target.ID} != ${Spawn[PC ${str_tankName1}].ID})) {
			/call TrueTarget ${int_MTID}
		}
		/return
	}
	
	/echo chkMTAlive ${_setAsTarget} || ${str_tankName1}::${str_MTName} ${str_tankName2}::${str_tankName2}
	|--MT is dead, and I am BT
	/if (${str_tankName2.Equal[${Me.CleanName}]}) {
		/varset str_tankName1 ${str_tankName2}
		/varset str_tankName2 NULL
		/melee aggro=on
		/return
	}
	
	|--MT is dead, check to see if BT is alive; chkBTAlive
	/if (${Spawn[PC ${str_tankName2}].ID}) {
		/echo setting ${str_tankName2} to MT 
		/if (${Spawn[PC ${str_tankName2}].ID}) {
			|-- BT is alive, set it as the new MT
			/varset str_tankName1 ${str_tankName2}
			/varset str_tankName2 NULL
		} else {
			|-- BT is dead, get MT by Precedence
			/echo Secondary Tank also dead, get MT by Precedence!
			/call getMTbyPrecedence
			/varset str_tankName1 {Spawn[PC ${Macro.Return}].ID}
			/varset str_tankName2 NULL
		}
		|-- Set the MT ID
		/varset int_MTID ${Spawn[${str_tankName1}].ID}
	}
	
	|-- If set as target, target the new MT immediately
	/if (${_setAsTarget} && (${Target.ID} != ${int_MTID})) {
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



|**
 ----------------------------
 AA routine
 ----------------------------
 Fires the AAs, controlled via INI
 Changed this to require an alt-activate number to be more reliable
 ----------------------------
**|
Sub chkAAs
	/if (${int_aaTotal} == 0) 	/return 
	/if (!${bln_engaged}) 		/return
	
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	
	/if (${Target.Type.NotEqual[NPC]}) /return
	/echo chkAAs
	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP check
	/if (${isNamed}  && (${Target.PctHPs} < ${int_aaNamedStopHP}))	/return
	/if (${isNamed}  && (${Target.PctHPs} > ${int_aaNamedStartHP}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${int_aaStopHP}))		/return
	/if (!${isNamed} && (${Target.PctHPs} > ${int_aaStartHP}))		/return

	:fd_check
	/if (${Me.Feigning}) /stand

	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {
		/declare aa int local

		/for aa 1 to ${int_aaTotal} 
			:AALoopStart
			|-- skip if disabled
			/if (!${AA_${aa}_Use}) /goto :NextAA

			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
			/if (!${isNamed} && ${AA_${i}_NamedOnly}) {
				/echo Skipping Nuke ${AA_${i}} is named only
				/goto :NextAA
			}

			| Check that the AA is ready
			/if (${Me.AltAbilityReady[${AA_${aa}}]} || ${Me.CombatAbilityReady[${AA_${aa}}]} || ${Me.SpellReady[${AA_${aa}}]}) {
				
				| Check that the hitpoints of target fall into the range
				| Check to see that my HP is at or below the trigger range (for things like desperation)
				/if ((${Target.ID}==${int_targetID}) && (${Target.PctHPs}<${AA_${aa}_StartHP}) && (${Me.PctHPs} <= ${AA_${aa}_MyHP}) && (${Target.PctHPs}>${AA_${aa}_StopHP})) {
					/echo checking ${AA_${aa}}
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/if (${AAGem${aa}.Equal[combat]} && (${Me.PctEndurance} > 10)) {
							/doability "${AA_${aa}}"
						} else {
							/medley queue "${AA_${aa}}"
						}
						/cecho \ag ${AA_${aa}}  : FIRED	
					} else {
						/if (${AAGem${aa}.Equal[combat]} && (${Me.PctEndurance} > 10)) {
							/doability "${AA_${aa}}"
						} else {
							/call MQ2Cast "${AA_${aa}}"	alt
							/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${AA_${aa}} : FIRED	
							}
						}
					}
						
				}
			}
		:NextAA
		/next aa
	}
/return
|**
 ----------------------------
 Check BURN routine
 ----------------------------
 Fires the burn setup if an event has set bln_BURNMode TRUE
 This is a little awkward due to the mix of combat abilities and alt abilities
 I made this verbose for clarity
 ----------------------------
**|

Sub chkBurn
	/echo chkBurn
	/if (!${bln_engaged}) /return
	/if (${Me.Casting.ID} || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	/if (!${bln_BURNMode}) /return
	/if (${int_burnModeTotal} < 1) /return 
	
	/declare burn int local
	/echo BurnMode Activated! Total Burns: ${int_burnModeTotal}
	/for burn 1 to ${int_burnModeTotal} 
		
		/if (${Target.ID} && !${Me.Casting.ID} && ${Target.Type.Equal[NPC]}) {

			| ---------------
			| Combat Ability
			| Note: Run the DISC if it is a combat type, it is ready, and we aren't already discing
			| ---------------
			/if (${Burn_${burn}_Type.Equal[combat]} && ${Me.CombatAbilityReady[${Burn_${burn}}]} && ${Burn_${burn}_Use} && !${Melee.DiscID}) {
				| Check to see if the burn Before disc/alt (0 if not) is already run
				/if (${Burn_${burn}_After} > 0) {
					| Check to see if the burn Before is an alt or combatdisc (two different ways of checking)
					/if (${Burn_${Burn_${burn}_After}_Type.Equal[combat]} && !${Me.CombatAbilityReady[${Burn_${Burn_${burn}_After}}]}) {
						/goto :doCombatAbility
					} else /if (${Burn_${Burn_${burn}_After}_Type.Equal[alt]} && !${Me.AltAbilityReady[${Burn_${Burn_${burn}_After}}]}) {
						/goto :doCombatAbility
					}
				}
				:doCombatAbility
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley "${Burn_${burn}}"
					} else {
						/doability "${Burn_${burn}}"
					}
					/cecho \agDISC \ayBurn_${burn} \agclicked!
			
			| ---------------	
			| Alt Ability
			| Note: We prefer Alt abilities with NUMBERS, they are quicker to fire, with less lag
			| 		Additionally, if MGBable, (few) we have Burn_1_MGB = TRUE to look for (default FALSE)
			| ---------------
			} else /if ((${Burn_${burn}_Type.Equal[alt]}) && (${Me.AltAbilityReady[${Burn_${burn}}]}) && (${Burn_${burn}_Use}) && (${Spell[${Burn_${burn}}].Stacks})) {
				| Check to see if the burn Before disc/alt (0 if not) is already run
				/if (${Burn_${burn}_After} > 0) {
					| Check to see if the burn Before is an alt or combatdisc  (two different ways of checking)
					/if (${Burn${Burn_${burn}_After}_Type.Equal[combat]} && !${Me.CombatAbilityReady[${Burn_${Burn_${burn}_After}}]}) {
						/goto :doAltAbility
					} else /if (${Burn_${Burn_${burn}_After}_Type.Equal[alt]} && !${Me.AltAbilityReady[${Burn_${Burn_${burn}_After}}]}) {
						/goto :doAltAbility
					}
				} 
				:doAltAbility
					/if (${Burn_${burn}_MGB}) {
						/call MQ2Cast "Mass Group Buff" alt
						/delay 5
						/cecho \ag Mass Group Buff Activated!
					}
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley "${Burn_${burn}}"
					} else {
						/call MQ2Cast "${Burn_${burn}}" alt
					}
					/cecho \agAA \ayBurn_${burn} \agclicked!
			
			| ---------------	
			| Item
			| 		Additionally, if MGBable, (few) we have Burn_1_MGB = TRUE to look for (default FALSE)
			| ---------------		
			} else /if (${Burn_${burn}_Type.Equal[item]} && ${FindItem[${Burn_${burn}}].TimerReady} && (${Burn_${burn}_Use}) ) {
				:doItemClick
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${Burn_${burn}}"
					} else {
						/call MQ2Cast "${Burn_${burn}}" item 2s
					}
					/cecho \agItem \ayBurn_${burn} \agclicked!
					
			}
		}
	:nextBurn
	/next burn
/return

|**
 ----------------------------
 chkClicks Clickie Nuke routine
 ----------------------------
 Fires the Clickie Items, controlled via INI
 ----------------------------
**|
Sub chkClicks
	/echo chkClicks 0
	/if (!${bln_engaged}) /return
	/if (${Me.Casting.ID} || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	/if (${Target.PctHPs}<${int_clickStopHP}) /return
	/if (${int_clickTotal} == 0) /return
	/echo chkClicks 1
	/declare intC int local
	/for intC 1 to ${int_clickTotal} 
		
		/if (${Target.PctHPs}<=${Click_${intC}_StartHP} && ${Target.PctHPs}>=${Click_${intC}_StopHP} && ${Target.ID} && !${Me.Casting.ID} && ${Target.Type.Equal[NPC]}) {
			/if ((!${Click_${intC}_NamedOnly}||${Target.ID}==${NearestSpawn[named npc].ID}) && (${Click_${intC}_Timer} <= 0)) {
				/if (${FindItem[${Click_${intC}}].InvSlot}) {

					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${Click_${intC}}"
					} else {
						/call MQ2Cast "${Click_${intC}}" item 2s
					}
					/cecho \agItem \ayBurn_${burn} \agclicked!
					/varset Click_${intC}_Timer ${Click_${intC}_Recast}s
				}
			}
        }
    /next intC
/return



|**
 ----------------------------
 Leashing
 ----------------------------
 This is defaulted to off for all toons, controlled via INI
 ----------------------------
**|
Sub chkLeash
    /if (${bln_useLeash} && (!${Target.ID} || (${Target.ID} == ${int_MTID})) ) {
        /if ((${Math.Distance[${Me.Y},${Me.X}:${int_leashY},${int_leashX}]}>${int_leashLen})) {
            /echo I should move
            /navigate loc ${int_leashX}  ${int_leashY} ${int_leashZ}
            /goto :LeashChecks
        } else {
            /return
        }
        
        :LeashChecks
            /if (${Math.Distance[${Me.Y},${Me.X}:${int_leashY},${int_leashX}]}> 400) /navigate loc ${int_leashX}  ${int_leashY} ${int_leashZ}
            /if (${Math.Distance[${Me.Y},${Me.X}:${int_leashY},${int_leashX}]}<${int_leashLen}) {
                /echo Arrived close enough!
				/nav stop
                /return
            } else {
                /goto :LeashChecks   
            }       
        
        }
/return

Sub EVENT_LEASHON
	/varset bln_useLeash TRUE
	/doevents flush LEASHON
/return

Sub EVENT_LEASHOFF
	/varset bln_useLeash FALSE
	/doevents flush LEASHOFF
/return


|**
 ------------------------
 EVENTS
	Event Handlers defined at the top of this file
	
 ------------------------
**|

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
	/bc Someone told me to end the macro it! ${Time}
	/delay 1s
	/gate
	/delay 2m
	/camp desktop
	/end
	/doevents flush EndMac
/return

|-------------------------------------------------------------------------------- 
|SUB: Event_BURNINATE!
|--------------------------------------------------------------------------------
Sub EVENT_BurnOn
	/echo Enabling Burn Mode
	/varset bln_BURNMode TRUE
	/doevents flush BurnOn
/return

Sub EVENT_BurnOnAll
	/echo Enabling Burn on ALL
	/varset bln_BURNMode TRUE
	/doevents flush BurnOnAll
/return

Sub EVENT_BurnOn1
	/if (${str_burnKey}==1) {
		/echo Enabling Burn on Cohort 1
		/varset bln_BURNMode TRUE
	}
	/doevents flush BurnOn1
/return

Sub EVENT_BurnOn2
	/if (${str_burnKey}==2) {
		/echo Enabling Burn on Cohort 2
		/varset bln_BURNMode TRUE
	}
	/doevents flush BurnOn2
/return

Sub EVENT_BurnOff
	/echo Disabling Burn Mode
	/varset bln_BURNMode FALSE
	/doevents flush BurnOff
/return

Sub EVENT_IntensityON
	/echo Enabling Intensity of the Resolute!
	/call MQ2Cast "Intensity of the Resolute" alt
	/doevents flush IntensityOn
/return





|-------------------------------------------------------------------------------- 
|SUB: Event_ENRAGE HANDLERS
|--------------------------------------------------------------------------------
Sub Event_RageOff(string line)
	/if ((${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${Target.DisplayName}]}) && ${Me.Pet.ID}) {
		/target id ${int_targetID}
		/pet attack
		/varset bln_enraged FALSE
	}
	/doevents flush RageOff
/return

Sub Event_RageOn(string line)
	/if (((${line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${line.Find[${Target.DisplayName}]}) && ${Me.Pet.ID}) && !${Me.PetBuff[Master's Aura Effect]}) {
		/echo Enrage detected! Backing pet off.
		/pet back off
		/varset bln_enraged TRUE
	}
	/doevents flush RageOn
/return

|-------------------------------------------------------------------------------- 
|SUB: Event_BackOff!
|--------------------------------------------------------------------------------

Sub Event_BackOff
	/echo Told to back off
	/varset bln_engaged FALSE
	/pet back off
	/pet hold
	/call targetReset
	/doevents clear
	/doevents flush BackOff
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
	/echo event_zoned ${Time.Time24}
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

|-- CONNECT TO EQBC
Sub Event_ConnectEQBC
	/bccmd quit
	/delay 20
	/bccmd connect
	/doevents flush ConnectEQBC
/return

|-- RELOAD CURRENT MACRO



|**
----------------------------
Check|Set the target's target as the new MT
Check to see if the int_MTID of default is alive
| note not setting HoTT as target cause... no.
----------------------------  
**|
Sub getMT
	/echo attempting to get new MT currentMT: ${int_MTID} SecondaryTank: ${str_tankName2} does tank = tank2? ${str_tankName1.Equal[${str_tankName2}]}
	/call getMTbyPrecedence

	/if (${int_MTID} == ${Me.ID}) {
		/varset bln_tanking TRUE
		/melee aggro=on
		/bc changeMT ${Me.Name}
	}


/return

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

	|-- Start with the defined str_tankName1,...3
	/if (${SpawnCount[pc ${str_tankName1}]} > 0) {
		/varset newTank ${str_tankName1}
		/goto :return
	} else /if (${SpawnCount[pc ${str_tankName2}]} > 0) {
		/varset newTank ${str_tankName2}
		/goto :return
	}  else /if (${SpawnCount[pc ${str_tankName3}]} > 0) {
		/varset newTank ${str_tankName3}
		/goto :return
	}


	| No tank is still alive, lets do by class precedence
	| Try to determine the next valid tank (likely a DPS or myself)
	/if (${NearestSpawn[PC WAR].Name.NotEqual[NULL]}) {
		/varset newTank ${NearestSpawn[PC WAR].Name}
	} else /if (${NearestSpawn[PC SK].Name.NotEqual[NULL]}) {
		/varset newTank ${NearestSpawn[PC SK].Name}
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
	/echo End getMTbyPrecedence: ${newTank}(${newID})
/return ${newID}


Sub EVENT_ChangeMT(_line, _sender, _eventParams)
	/echo Changing the MT
	/echo _sender: ${_sender}
	/echo _eventParams:${_eventParams}
	/declare name string local ${_eventParams.Arg[1, ]}
	/varset	int_MTID ${Spawn[pc ${name}].ID}
	/if (${Defined[_strTankName]})	/varset str_tankName1 ${name} 
	/bc [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tankNormal}
	/if (${Me.Class.ShortName.Equal[CLR]}) /call TrueTarget ${int_MTID}
	/if (${Me.Name.Equal[${name}]}) {
		/varset bln_tanking TRUE
		/melee aggro=on
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
		/bc [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_assistAt}
	}
	/doevents flush ChangeAP
/return



Sub EVENT_MOBDEAD
	/echo EVENT_MOBDEAD triggered
	/varset bln_engaged FALSE
	/varset int_targetID 0
	/call targetReset
	/doevents flush MOBDEAD
/return

Sub chkMobDead
	/if ((!${Me.XTarget[1].ID} && ${SpawnCount[npc id ${int_targetID} radius 250]} == 0) && ${bln_engaged}) {
		|/echo chkMobDead triggered Xtarget:!${Me.XTarget[1].ID} || SpawnCount:!${SpawnCount[npc id ${int_targetID} radius 250]}  && engaged:${bln_engaged}
		/call targetReset
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


|------------------------------------------------------------|
|- Reliably targets specified Target.IDs. Ripped from E3    -|
|------------------------------------------------------------|
|- @param TargetID: The ID of the spawn you wish to target.	-|
|------------------------------------------------------------|
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

Sub chkAutoAssist
	/if (!${bln_autoAssist}) /return
	/call TrueTarget ${NetBots[${str_tankName1}].TargetID}
	/call engageTarget ${NetBots[${str_tankName1}].TargetID}
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
|------------------------------------------------------------|
|- Reliably determine if target is Named				-|
|------------------------------------------------------------|
|- @param TargetID: The ID of the spawn you wish to check on.	-|
|------------------------------------------------------------|


Sub TargetNamedCheck(_id)
	|/echo TargetNamedCheck ${_id}
	/if (${Spawn[npc id ${_id}].Named} || ${Spawn[npc id ${_id}].Name.Find[#]}) /return TRUE
	/if (${Zone.ShortName.Equal[tacvi]} || ${Zone.ShortName.Equal[txevu]}) /return TRUE
	/if (${Zone.ShortName.Equal[chambersa]} ||${Zone.ShortName.Equal[chambersb]} ||${Zone.ShortName.Equal[chambersc]} ||${Zone.ShortName.Equal[chambersd]}||${Zone.ShortName.Equal[chamberse]}) /return TRUE
	/if (${Zone.ShortName.Equal[anguish]}) /return TRUE
	/if (${Zone.ShortName.Equal[arena]}) /return TRUE
/return FALSE
	
|------------------------------------------------------------|
|- Send a box chat command to everyone in group				-|
|------------------------------------------------------------|
|- Includes out of zone group members
|------------------------------------------------------------|


Sub commandAllInGroup(command)
	/echo /noparse commandAllInGroup ${command}
	/declare i int local 0
	/for i 0 to ${Group}
		/bct ${Group.Member[${i}]} //docommand ${command}
	/next i
/return


SUB argueString(WhatToFind, givenData)
	/echo |- argueString ==> ${WhatToFind}  => ${givenData}
 
	/declare i int local 1
	/declare foundData string local NULL
	
	:nextArg
	/if (${givenData.Find[/]}) {
		
		/if (${givenData.Arg[${i},/].Find[${WhatToFind}]}) {
		
			/varset foundData ${givenData.Arg[${i},/].Arg[2,|]}
			/if (${Debug}) /echo |- argueString -| Found [${WhatToFind}] - [${foundData}]
		
		} else {
			/varcalc i ${i} + 1
			/if (${givenData.Arg[${i},/].Length}) /goto :nextArg
		}
	} else {
		/if (${givenData.Arg[${i}, ].Find[${WhatToFind}]}) {
		
			/varset foundData ${givenData.Arg[${i}, ].Arg[2,|]}
			/if (${Debug}) /echo |- argueString -| Found [${WhatToFind}] - [${foundData}]
		
		} else {
			/varcalc i ${i} + 1
			/if (${givenData.Arg[${i}, ].Length}) /goto :nextArg
		}
	}

	/echo foundData? ${foundData}
/return ${foundData}


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
	/if (${Debug}) /echo |- IniToArray [${Ini_Key}] ==>

	| Check for the specified ini key.
	/if (!${Ini[${Ini_Key}1].Length}) {
		/if (${Debug}) /echo [${Ini_Key}] not found.
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

/if (${Debug}) {
	/if (${${ArrayName}.Size}) {
		/echo Declared: ${ArrayName}[${${ArrayName}.Size}]
		/for i 1 to ${${ArrayName}.Size}
			/echo ${ArrayName}[${i}] - ${${ArrayName}[${i}]}
		/next i
	}
	/echo <== IniToArray -|
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
	/declare haveHelm bool local FALSE
	/varset haveHelm ${Bool[${Me.Inventory[2].Name}]}
	/if (!${haveHelm}) /rs I am NAKED. 
	/doevents flush CheckNaked
/return

Sub chkNaked
	/declare naked bool local FALSE
	/declare haveHelm bool local FALSE
	/varset haveHelm ${Bool[${Me.Inventory[2].Name}]}
	/if (!${haveHelm} && !${timer_naked}) {
		/rs I am NAKED. 
		/varset timer_naked 30s
	}
	/doevents flush chkNaked
/return ${naked}


Sub findslot(string _commandtext)

/return

Sub fixCorpses

/return


Sub saveGroup

/return

SUB verifyEvent(sring _chatSender, string _eventParams)
	/declare userValidated 	bool local FALSE
	/declare includeBot 	bool local TRUE
	/declare inZone 		bool local FALSE
	/declare inRange 		bool local FALSE
	/declare inAssistRange 	bool local FALSE
	/declare sender         string local ${_chatSender}
	| -Reset ChatSender.
	/if (${_chatSender.Left[1].Compare[ ]}<0) /varset sender ${_chatSender.Right[-2].Left[-1]}
	/if (${_chatSender.Equal[You]}) /varset sender ${Me.CleanName}
	/if (${_chatSender.Equal[MQ2]}) /varset sender ${Me.CleanName}
	
	| -Validate ChatSender.
	/if (${NetBots[${sender}].ID}) /varset userValidated TRUE
	
	| -Check inZone
	/if (${SpawnCount[pc =${sender}]}) /varset inZone TRUE
	
	| -Check inRange
	/if (${SpawnCount[pc =${sender} radius ${MaxResponseDist}]}) /varset inRange TRUE

	/echo verifyEvent ${sender} :: ${_eventParams}
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
	/echo returning ${sender}_${userValidated}_${includeBot}_${inZone}_${inRange}
/return ${sender}_${userValidated}_${includeBot}_${inZone}_${inRange}

SUB check_selectedBots(selectedBots, ChatSender)

	/declare botSelected bool local FALSE
	/if (${selectedBots.Find[ ]}) /varset selectedBots ${selectedBots.Replace[ ,,]}

	/if (${Select[priests,${selectedBots}]} && ${Select[${Me.Class.ShortName},${PriestClasses}]} || ${Select[healers,${selectedBots}]} && ${Select[${Me.Class.ShortName},${PriestClasses}]} || ${Select[ranged,${selectedBots}]} && ${Select[${Me.Class.ShortName},${RangedClasses}]} || ${Select[casters,${selectedBots}]} && ${Select[${Me.Class.ShortName},${CasterClasses}]} || ${Select[melee,${selectedBots}]} && ${Select[${Me.Class.ShortName},${MeleeClasses}]} || ${Select[me,${selectedBots}]} && ${ChatSender.Equal[${Me.CleanName}]} || ${Select[${Me.Class},${selectedBots}]} || ${Select[${Me.Class.ShortName},${selectedBots}]} || ${Select[${Me.CleanName},${selectedBots}]} || ${Select[all,${selectedBots}]} || ${Select[group,${selectedBots}]} && ${Me.CleanName.Equal[${_chatSender}]} || ${Select[group,${selectedBots}]} || ${Select[zone,${selectedBots}]} && ${SpawnCount[pc ${_chatSender}]}) /varset botSelected TRUE
	
/return ${botSelected}

Sub check_validBots(selectedBots, botName)
	/declare botValidated bool local false
	/if (${Select[priests,${selectedBots}]} && ${Select[${NetBots[${botName}].Class.ShortName},${PriestClasses}]} || ${Select[healers,${selectedBots}]} && ${Select[${NetBots[${botName}].Class.ShortName},${PriestClasses}]} || ${Select[ranged,${selectedBots}]} && ${Select[${NetBots[${botName}].Class.ShortName},${RangedClasses}]} || ${Select[casters,${selectedBots}]} && ${Select[${NetBots[${botName}].Class.ShortName},${CasterClasses}]} || ${Select[melee,${selectedBots}]} && ${Select[${NetBots[${botName}].Class.ShortName},${MeleeClasses}]} || ${Select[me,${selectedBots}]} && ${ChatSender.Equal[${Me.CleanName}]} || ${Select[${Me.Class},${selectedBots}]} || ${Select[${NetBots[${botName}].Class.ShortName},${selectedBots}]} || ${Select[${Me.CleanName},${selectedBots}]} || ${Select[all,${selectedBots}]} || ${Select[group,${selectedBots}]} && ${Me.CleanName.Equal[${_chatSender}]} || ${Select[group,${selectedBots}]} || ${Select[zone,${selectedBots}]} && ${SpawnCount[pc ${_chatSender}]}) /varset botValidated TRUE
	

/return ${botValidated}


|------------------------------------------------------------------------------------|
|- Creates timers, at the specified duration, and adds their names to timerArray.	-|
|------------------------------------------------------------------------------------|
|- @param timerName: Name of the timer you want to declare.							-|
|- @param timerSetting: Setting of the timer you're declaring.						-|
|------------------------------------------------------------------------------------|
SUB createTimer(timerName, timerSetting)
|/if (${Debug}) /echo |- createTimer ==>

	| Delete old timer.
	/if (${Defined[${timerName}]}) /deletevar ${timerName}

	| Declare new timer.
	/declare ${timerName} timer global ${timerSetting}
	
	| Add the timer's name to the timerArray.
	/call BuildArray timerArray ${timerName} global

|/if (${Debug}) {
|	/echo created ${timerName} [${${timerName}}]
|	/echo <== createTimer -|
|}
/return



|----------------------------------------------------|
|- Deletes spent timers made by 'SUB createTimer'.	-|
|----------------------------------------------------|
SUB check_macroTimers
/if (${Debug}) /echo |- check_macroTimers ==>

	/declare i int local
	/declare SpentTimerCounter int local

	| Check each entry of the timer array, count spent timers.	
	/for i 1 to ${timerArray.Size}
		/if (${Debug}) /echo Checking timer [${timerArray[${i}]} -- ${${timerArray[${i}]}}]
		/if (!${${timerArray[${i}]}}) {
			/varcalc SpentTimerCounter ${SpentTimerCounter} + 1
			/if (${Defined[${timerArray[${i}]}]}) {
				/if (${Debug}) /echo Deleting timer: ${timerArray[${i}]}
				/deletevar ${timerArray[${i}]}
			}
		}
	/next i
	
	| If all the timers listed on timerArray are spent, delete timerArray.
	/if (${SpentTimerCounter}==${timerArray.Size}) {
		/if (${Debug}) /echo All timers have been cleared.
		/deletevar timerArray
	}
	
/if (${Debug}) /echo <== check_macroTimers -|
/return

|----------------------------------------------------|
|- Resets Xtarget window 1-5 elements to auto		-|
|----------------------------------------------------|
SUB doRefreshXTarget
	/declare i int local 1
	/for i 1 to 5
		/xtarget set ${i} grouptank
		/delay 2
		/xtarget set ${i} autohater
	/next i
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
/if (${Debug}) /echo |- BuildArray Adding [${ArrayName}-${Data}-${varScope}] ==>

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

/if (${Debug}) /echo <== BuildArray -|
/return ${ArrayName}



|----------------------------------------------------|
|- Removes elements from an existing array.			-|
|----------------------------------------------------|
|- @param ArrayName: Name of the array to search.	-|
|- @param Data: Data you wish to remove.			-|
|----------------------------------------------------|
SUB RemoveArrayElement(ArrayName, Data)
/if (${Debug}) {
	/echo |- RemoveArrayElement ==>
	/echo |- RemoveArrayElement -| Removing [${Data}] from [${ArrayName}]
}

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
	
/if (${Debug}) /echo <== RemoveArrayElement -|
/return

Sub EVENT_SaveRingset
	/call SaveRingset
/return
Sub SaveRingset
	/echo Saving Rings
	/declare leftfinger string local ${Me.Inventory[leftfinger].Name}
	/declare rightfinger string local ${Me.Inventory[rightfinger].Name}

	/ini botINI "SavedInventory" "leftfinger" "${leftfinger}"
	/ini botINI "SavedInventory" "rightfinger" "${rightfinger}"
/return

Sub EVENT_RemoveRingset
	/cecho \ayRemoving Ringset ${_eventParams}
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

	/cecho \ayRemoving \ar${Ring_1_Name} \ayfrom leftfinger
	/unequip leftfinger
	/delay 10
	/cecho \ayRemoving \ar${Ring_2_Name} \ayfrom rightfinger
	/unequip rightfinger
	/delay 10
	/autoinventory
/return

|-- method to load the saved rings from INI (primarily for Trial of Foresight)
Sub EVENT_LoadRingset(string _line, string _sender, string _eventParams)
	/cecho \ayLoading Ringset ${_eventParams}
	/call LoadRingset
/return

Sub LoadRingset
	/declare leftfinger string local ${Ini[botINI,SavedInventory,leftfinger]}
	/declare rightfinger string local ${Ini[botINI,SavedInventory,rightfinger]}
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/echo ${leftfinger},  ${rightfinger}

	/declare notemptyCount int local 0
	:notempty
	/varcalc notemptyCount ${notemptyCount}+1
	/autoinventory
	/if (${notemptyCount} > 10) /return
	/if (${Cursor.ID}) /goto :notempty

	/if (${leftfinger.NotEqual[NULL]}) {
		 /exchange "${leftfinger}" leftfinger
	}
	/delay 10
	/if (${rightfinger.NotEqual[NULL]}) {
		/exchange "${rightfinger}" rightfinger
	}
	/cecho \agSaved Ringset Equipped 
	/autoinventory
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley
/return 


|-- method to load the saved weapons from INI (primarily for Trial of Specialization)
Sub EVENT_LoadWeaponset(string _line, string _sender, string _evtParams)
	/cecho \ayLoading Weaponset <${_evtParams}> (${_sender})
	/if (${_evtParams.NotEqual[NULL]}) /call LoadWeaponset ${_evtParams}
/return


Sub LoadWeaponset(string _weaponset)
	|/if (${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[PRIEST]} && ${_weaponset.NotEqual[default]}) /return
	/if (${_weaponset.Equal[NULL]}) /varset _weaponset default
	/declare set string local ${Ini[botINI,Weaponsets,${_weaponset}]}
	/if (${set.Equal[NULL]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/cecho \aw ${_weaponset}: ${set}
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
		/cecho \ay Weaponset ${_weaponset} offhand is NULL! Removing.
		/if (${Me.Inventory[offhand].Name.NotEqual[NULL]}) {
			/echo unequip offhand
			/unequip offhand
		} else {
			/echo Offhand was already Empty!
		}
		
	} else {
		/autoinventory
		/cecho \ag Moving "${weap}" to ${posi}
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
		/cecho \ay Weaponset ${_weaponset} mainhand is NULL! Removing.
		/if (${Me.Inventory[mainhand].Name.NotEqual[NULL]}) {
			/echo unequip mainhand
			/unequip mainhand
		} else {
			/echo Mainhand was already Empty!
		}
	} else {
		/autoinventory
		/cecho \ag Moving "${weap}" to ${posi}
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
Sub EVENT_SaveWeaponset(string _line, string _sender, string _evtParams)
	/cecho \aySavingWeaponset ${_sender} ${_evtParams}
	/declare key string local ${_evtParams.Arg[1,=]}
	/declare weapons string local ${_evtParams.Arg[2,=]}
	/echo  key: ${key} weapons: ${weapons}
	/if (${_evtParams.NotEqual[NULL]}) /call SaveWeaponset ${_evtParams.Token[1,=]} ${_evtParams.Token[2,=]}
/return

Sub SaveWeaponset(string _key, string _weaponset)
	|weaponset line should be [_key] [item1|mainhand,item2|offhand]
	|example: default=Longsword of Execration|mainhand,Aegis of Midnight|offhand
	/echo writing key: ${_key} weaponset: ${_weaponset}
	/ini botINI "Weaponsets" "${_key}" "${_weaponset}"
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
	
 	/ini botINI "Weaponsets" "${_key}" "${pri}|mainhand,${off}|offhand"
/return