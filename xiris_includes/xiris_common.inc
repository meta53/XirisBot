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
	08.12.23	xiris	As past of the substantial v2.0 rewrite, refactored much of this library
						Added raid builder, moved functionality from standalone macro
						Removed auto assist as I never use it
	11.22.23	xiris 	Added support for MQ2SQLite
______________________________________________

**|

| NPC Engagement Events
#EVENT EngageNPC			"[#1#] EngageMob #2#"
#EVENT EngageNPC			"<#1#> EngageMob #2#"
#EVENT EngageNPC			"#1# tells the group#*#'EngageMob #2#'"
#EVENT EngageNPC			"#1# tells you#*#'EngageMob #2#'"
#EVENT EngageNPC			"#1# tells the raid#*#'EngageMob #2#'"
#EVENT EngageNPC			"#1# tell your raid#*#'EngageMob #2#'"
#EVENT EngageNPC			"#1# tell your party, #*#'EngageMob #2#'"
#EVENT EngageNPC			"[ #1# #*#]#*#EngageMob #2#"

#EVENT KillNPC 				"[#1#] KillMob #2#"
#EVENT KillNPC 				"<#1#> KillMob #2#"
#EVENT KillNPC				"#1# tells the group#*#'KillMob #2#'"
#EVENT KillNPC				"#1# tells you#*#'KillMob #2#'"
#EVENT KillNPC				"#1# tells the raid#*#'KillMob #2#'"
#EVENT KillNPC				"#1# tell your raid#*#'KillMob #2#'"
#EVENT KillNPC				"#1# tell your party, #*#'KillMob #2#'"
#EVENT KillNPC 				"[ #1# #*#]#*#KillMob #2#"

#EVENT Disengage 			"[#1#] BackOff #2#"
#EVENT Disengage 			"<#1#> BackOff #2#"
#EVENT Disengage			"#1# tells the group#*#'BackOff#*#'"
#EVENT Disengage			"#1# tells you#*#'BackOff#*#'"
#EVENT Disengage			"#1# tells the raid#*#'BackOff#*#'"
#EVENT Disengage			"#1# tell your raid#*#'BackOff#*#'"
#EVENT Disengage 			"[ #1# #*#]#*#BackOff#*#"

#EVENT RageOff				"#*#is no longer enraged#*#"
#EVENT RageOn				"#*#has become ENRAGED#*#"

| Tank Setting Events
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

| Raid Building Events
#EVENT RaidReform 			"#1# tells you#*#'RaidReform #2#'"
#EVENT RaidReform			"#1# tells the group#*#'RaidReform #2#'"
#EVENT RaidReform			"#1# tells the raid#*#'RaidReform #2#'"
#EVENT RaidReform 			"[ #1# #*#]#*#RaidReform #2#"

#EVENT RaidBuildByTemplate	"#1# tells you#*#'RaidBuild #2#'"
#EVENT RaidBuildByTemplate	"#1# tells the group#*#'RaidBuild #2#'"
#EVENT RaidBuildByTemplate	"#1# tells the raid#*#'RaidBuild #2#'"
#EVENT RaidBuildByTemplate	"[ #1# #*#]#*#RaidBuild #2#"
#EVENT RaidBuildByTemplate	"#1# tells you#*#'Voltron #2#'"
#EVENT RaidBuildByTemplate	"#1# tells the group#*#'Voltron #2#'"
#EVENT RaidBuildByTemplate	"#1# tells the raid#*#'Voltron #2#'"
#EVENT RaidBuildByTemplate	"[ #1# #*#]#*#Voltron #2#"

| MISC events
#EVENT CheckNaked			"#*#CheckNaked#*#"
#EVENT Zoned				"You have entered#*#"
#EVENT Zoned				"LOADING, PLEASE WAIT..."

#EVENT RemoveRingset		"#1#RemoveRingset#*#"
#EVENT LoadRingset 			"#1#LoadRingset#*#"
#EVENT LoadWeaponset		"#1#LoadWeaponset #2#"
#EVENT SaveJewelry			"#1#SaveJewelry#*#"
#EVENT SaveRingset      	"#1#SaveRingset#*#"
#EVENT SaveFaceSlot      	"#1#SaveFaceSlot#*#"
#EVENT SaveWeaponset  		"#1#SaveWeaponset #2#"
#EVENT SaveCurrentWeaponset "#1#SaveCurrentWeaponset #2#"

#EVENT UseStaunchRecovery   "#*#DoStaunchRecovery#*#"
#EVENT UseIntensity			"#*#DoIntensity#*#"
#EVENT UseServants			"#*#DoServants#*#"
#Event UseInfusion			"#*#DoInfusion#*#"

Sub initEvents_Common
	/call RegisterBackgroundEvents	Events_Common_Background
	/call RegisterRaidEvents		Events_Common_Raid
	/call RegisterPriorityEvents	Events_Common_Priority
/return

Sub Events_Common_Background
	/doevents CheckNaked

	/if (!${bln_engaged}) {
		/doevents RaidReform
		/doevents RaidBuildByTemplate
		/doevents Zoned	
		| Iventory Remove Events
		/doevents RemoveRingset

		| Inventory Load Events
		/doevents LoadFaceSlot
		/doevents LoadRingset
		/doevents LoadWeaponset

		| Inventory Save Events
		/doevents SaveJewelry
		/doevents SaveFaceSlot
		/doevents SaveRingset
		/doevents SaveEarrings
		/doevents SaveWeaponset
		/doevents SaveCurrentWeaponset

		/doevents RefreshXTarget
		/if (!${timer_check_utilities}) /call chkUtils

			
	}
/return

Sub Events_Common_Raid
	/if (${bln_engaged}) /call ${If[${Me.Class.ShortName.Equal[CLR]},chkTargetStatusNonDPS,chkTargetStatus]}
	/doevents RageOn
	/doevents RageOff
	/doevents ChangeMT
	/doevents ChangeSA
	/doevents ChangeAP
	/doevents UseStaunchRecovery
	/doevents UseIntensity
	/doevents UseServants
	/doevents UseInfusion
/return 

Sub Events_Common_Priority
	/doevents KillNPC 
	/doevents EngageNPC
	/doevents Disengage
/return 


| Called by the raidleader to the raid to tell them to reprocess the new template
Sub EVENT_RaidReform(string _sender, string _line, string _eventParams)
	/echo \atRaid Reform \ag${_eventParams}
	/doevents flush RaidReform
	/call processRaidTemplate ${_eventParams} FALSE
/return

| Called by the raidleader to herself by /echo BuildRaid TEMPLATE
Sub EVENT_RaidBuildByTemplate(string _sender, string _line, string _eventParams)
	/echo \atRaid Build By Template \ag${_eventParams}
	/doevents flush RaidBuildByTemplate
	/call processRaidTemplate ${_eventParams} TRUE
/return




Sub xbot_initialize_common(string _t1, string _t2, string _t3, int _tankHealPointOverride) 
	/echo \apInitialize: \aoCommon \aw-------------------------

	|-- [Telemetry]
	/declare	float_startTime 		float	outer   ${EverQuest.Running}
	/declare	float_endTime   		float	outer   0
	/declare	float_processed 		float	outer   0
	/declare	bln_debug 				bool	outer  	TRUE

	|-- [Base Settings]
	/declare	str_command_channel		string	outer	${Ini[INI_COMMON,SETTINGS,str_command_channel,dga]}
	/declare 	str_announce_channel	string	outer	${Ini[INI_COMMON,SETTINGS,str_announce_channel,dgt]}
	/dnet 		fullnames 							  	${Ini[INI_COMMON,SETTINGS,dnet_fullNames,off]}
	/declare 	gbl_bln_useWarp    		bool    outer	${Ini[INI_COMMON,SETTINGS,useWarp,FALSE]}
	/declare 	bln_announce			bool 	outer 	TRUE
	/declare 	gbl_db_path				string local 	${MacroQuest.Path[macros]}/xiris_settings_global

	
	|-- [Class/Grouping/Raid Settings]
	/call processClassTypes
	/declare 	str_class_type			string 	outer ${Macro.Return}
	/declare 	str_raid_mode			string  outer DEFAULT
	/call processRaidTemplate ${str_raid_mode}	FALSE

	|-- [Tank List & Default Tank Heal Point]
	/declare	str_default_t1			string	outer	${Ini[INI_COMMON,SETTINGS,defaultTank1,Xiris]}
	/declare 	str_default_t2			string	outer	${Ini[INI_COMMON,SETTINGS,defaultTank2,Xiria]}
	/declare	str_default_t3			string	outer	${Ini[INI_COMMON,SETTINGS,defaultTank3,Xirea]}			
	/declare 	str_t1_name				string	local	${str_default_t1}
	/declare    str_t2_name 			string  local 	${str_default_t2}
	/declare	str_t3_name				string	local	${str_default_t3}

	|-- Insert passed in values for tanks if we have the parameters passed in
	/if (${_t1.Length} > 3)				/varset str_t1_name ${_t1}
	/if (${_t2.Length} > 3)				/varset str_t2_name ${_t2}
	/if (${_t3.Length} > 3)				/varset str_t3_name ${_t3}

	|-- ID and Name here keep track of *current* tank/second tank. They are swapped as needed.
	/declare	int_MTID           		int		outer	${Spawn[${str_t1_name}].ID}
	/declare 	str_MTName				string  outer  	${str_t1_name}	
	/declare 	int_STID				int		outer	${Spawn[${str_t2_name}].ID}
	/declare 	str_STName				string  outer 	${str_t2_name}
	/declare	int_TTID				int	 	outer 	${Spawn[${str_t3_name}].ID}
	/declare 	str_TTName				string	outer 	${str_t3_name}

	|-- Setting base tank settings
	/declare	bln_tanking				bool	outer	${If[${Me.ID} == ${int_MTID},TRUE,FALSE]}
	/declare	bln_taunting			bool 	outer	${If[${Me.ID} == ${int_MTID},TRUE,FALSE]}

	/echo \ao        Tanks \awMT:\ag${str_MTName} \awST:\ag${str_STName} \awTT:\ag${str_TTName}

	| Create a string of group member names for SELECT statements
	/declare	str_groupMembers		string  outer  	${Me.Name} 
	/declare  	str_groupClerics		string  outer   NULL
	/declare 	int_groupClerics 		int 	outer  	1
	/call createGroupList
	/echo \ao        Group \awMembers:\ag${str_groupMembers} \awClerics:(\ag${int_groupClerics}\aw) \ag${str_groupClerics}


	|-- [Settings] : Miscellaneous Defaults 
	| Engaged is 'engagement' which means a tank has picked up an aggro NPC and is tanking it
	| and the auto slowing/debuffing can happen, but NOT assist in killing
	| Killing is the actuall KillMob call, which causes everyone to assist.
	/declare	bln_engaged				bool    outer   FALSE
	/declare 	bln_killing				bool 	outer	FALSE
	/declare 	bln_enraged				bool 	outer	FALSE
	/declare 	int_targetID			int 	outer	0
	/declare 	str_targetName			string  outer   NULL
	/declare 	bln_naked 				bool	outer	FALSE
	/declare 	timer_announce_naked 	timer	outer	0
	/call chkNaked
	/if (${bln_naked}) /multiline ; /delay 30m !${bln_naked} 
	/declare 	bln_ducking				bool 	outer	FALSE
	/declare 	timer_xtarget_refresh	timer	outer 	600
	| TODO: Create Inventory Handler 
	/declare 	str_item_face_default	string  outer   ${InvSlot[3].Item}

	|-- [Settings] : Meditation & Utilities
	/declare int_sittingRadius   		int     outer ${Ini[INI_COMMON,SETTINGS,sitRadius,50]}
	/declare int_meditationPctMana       int     outer ${Ini[INI_COMMON,SETTINGS,medPercent,100]}
	/declare timer_meditation			timer	outer ${Ini[INI_COMMON,SETTINGS,medDelay,45s]}
	/declare timer_sitting				timer	outer ${Ini[INI_COMMON,SETTINGS,sitDelay,10s]}
	/declare timer_check_utilities		timer   outer 0


	|-- [Settings] : Camp Information
	/declare int_campX 					int 	outer 	${Me.X}
	/declare int_campY 					int 	outer 	${Me.Y}
	/declare int_campZ 					int 	outer 	${Me.Z}
	/declare int_campRadius				int		outer	85
	/declare str_zone_initial			string 	outer   ${Zone.ShortName}
	/declare str_zone_bound 			string  outer 	${Me.BoundLocation[0]}

	|-- INITIALIZE Libraries
    /call xbot_initialize_dannet
	/call xbot_initialize_spellRoutines
    /call xbot_initialize_events
	/call xbot_initialize_buffing
	/call xbot_initialize_burning
    /call xbot_initialize_casting
	/call xbot_initialize_clicking
    /call xbot_initialize_curing
	/call xbot_initialize_exclusions
	/call xbot_initialize_healing
    /call xbot_initialize_looting
	/call xbot_initialize_melee
	/call xbot_initialize_movement
	/call xbot_initialize_offtanking
	/call xbot_initialize_pets 
	/call xbot_initialize_class_${Me.Class.ShortName}

	| Initialize the Common Events
	/call initEvents_Common

	/varset float_endTime ${EverQuest.Running}
	/varcalc float_processed ${float_endTime} - ${float_startTime}
	/echo \arSETUP COMPLETE \ay${float_processed}ms  \ag${Macro.MemUse}MB  \ao${Macro.Undeclared}

/return


Sub processClassTypes
	/echo \awprocessClassTypes
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]})		/return PRIEST
	/if (${Select[${Me.Class.ShortName},NEC,WIZ,MAG,ENC]})	/return CASTER
	/if (${Select[${Me.Class.ShortName},ROG,BER,MNK]})		/return MELEE_END
	/if (${Select[${Me.Class.ShortName},BST,RNG,BRD]})		/return MELEE_MANA
	/if (${Select[${Me.Class.ShortName},WAR,SHD,PAL]})		/return TANK	
/return


Sub processRaidTemplate(string _template, bool _build)
	/if (!${Raid.Members} && !${_build}) /return
	/echo \awprocessRaidTemplate \ay${_template} \a-y${_build}
	/declare test 				string	local	NULL
	| Make sure the template is enumerated
	/varset test	${Ini[INI_RAID,SETTINGS,Templates]}
	/if (!${test.Find[${_template}]}) /multiline ; /docommand /${str_announce_channel} Raid Template ${_template} NOT Found, Aborting ; /return

	| Since this method can be called more than once, we want to kill previous arrays
	/if (${Defined[arr_raid_diClerics]}) 		/deletevar arr_raid_diClerics
	/if (${Defined[arr_raid_groupLeaders]})		/deletevar arr_raid_groupLeaders

	/declare int_raidCount		int		local	${Ini[INI_RAID,RAID_${_template},RaidCount]}
	/declare str_groupCounts	string	local	${Ini[INI_RAID,RAID_${_template},GroupCount]}
	/declare int_leader			int 	local	0
	/declare di                 int		local	0
	/declare g					int  	local	0
	/declare i					int		local	0
	/declare j					int 	local 	0
	/declare bln_heal_useDivineIntervention bool	outer	TRUE

	/if (!${Raid.Members}) /goto :return
	/for i 1 to ${int_raidCount}
		/varset test ${Ini[INI_RAID,RAID_${_template},Raid_${i}_Leaders]}
		/if (${test.Find[${Raid.Leader}]}) {
			/varset int_leader	${i}
			/varset g			${str_groupCounts.Arg[${i},|]}
			/echo \aw        Setting Raid Group mode to \ao${_template} \aw:: \awFound my membership in Raid \ao${i}
			
			| Populate Raid Group Leaders List
			/echo \aw        Raid Group Leaders List
			/declare arr_raid_groupLeaders[${g}]	string	outer	NULL
			/for j 1 to ${g}
				/varset	arr_raid_groupLeaders[${j}]  ${test.Arg[${j},|]}
				/echo \ag        Raid Group Leader \aw[\ao${j}\aw] \ay${arr_raid_groupLeaders[${j}]}
			/next j
			
			| Set the Divine Intervention Clerics for my Raid
			/echo \aw        Divine Intervention List
			/varset 	di ${Ini[INI_RAID,RAID_${_template},Raid_${i}_DI_Count]}
			/declare 	dl ${Ini[INI_RAID,RAID_${_template},Raid_${i}_DI_Clerics]}
			/declare arr_raid_diClerics[${di}]		string	outer	NULL
			/for j 1 to ${di}
				/varset arr_raid_diClerics[${j}]	${dl.Arg[${j},|]}
				/if (${arr_raid_diClerics[${j}].Equal[${Me.CleanName}]}) /varset bln_heal_useDivineIntervention TRUE
				/echo \ag        Divine Intervention \aw[\ao${j}\aw] \ay${arr_raid_diClerics[${j}]}
			/next j
			/goto :return
		}
	/next i
	:fail_GroupNotFound
		/echo \awSetting Raid Group Mode \arERROR
		/echo \aw   Unable to find our raid leader in the leader list for the \ao${_template} \awraid type.
		/declare 	arr_raid_groupLeaders[1]	string 	outer NULL
		/declare 	arr_raid_diClerics[1]		string	outer NULL

	:return
	/if (${_build}) /call raidBuildByTemplate ${_template}
/return

Sub raidBuildByTemplate(string _template)
	/squelch /docommand /${str_command_channel} /raiddisband 
	/squelch /docommand /${str_command_channel} /keypress ctrl+d
	/call raidBuildGroups ${_template}
	/delay 20
	/docommand /${str_announce_channel} RaidReform ${_template}
/return

Sub raidBuildGroups(string _template)
	/echo \aw RaidBuildGroups ${_template}
	/declare int_raidCount		int		local	${Ini[INI_RAID,RAID_${_template},RaidCount]}
	/declare str_groupCounts	string	local	${Ini[INI_RAID,RAID_${_template},GroupCount]}
	/declare int_groupCounts 	int     local 	${str_groupCounts.Count[|]}
	/echo \ag    RaidCount: \ao${int_raidCount} \aggroupCounts: \ao${str_groupCounts} \aggroupCountsForThisRaid \ao${int_groupCounts}

	/declare raid_leaders		string	local	NULL
	/declare raid_leader		string  local   NULL

	/declare group_leader		string  local 	NULL
	/declare group_count		int 	local   0
	
	/declare group_members		string  local 	NULL
	/declare group_member		string  local   NULL
	/declare member_count		int 	local 	0
	/declare g					int  	local	0
	/declare i					int		local	0
	/declare j					int 	local 	0
	
	
	/for i 1 to ${int_raidCount}
		/varset 	group_count ${str_groupCounts.Arg[${i},|]}
		/varset 	raid_leaders ${Ini[INI_RAID,RAID_${_template},Raid_${i}_Leaders]}
		/varset     raid_leader  ${raid_leaders.Arg[1,|]}
		/for j 1 to ${group_count}
			
			/echo \aw    Raid \ag${i}\aw RaidLeader:\ag${raid_leader}\aw Group \ag${j}
			/varset 	group_leader	${raid_leaders.Arg[${j},|]} 
			/varset 	group_members	${Ini[INI_RAID,RAID_${_template},Raid_${i}_${j}]}
			/varset 	member_count 		${group_members.Count[|]}
			/for g 1 to ${member_count}
				/varset group_member 		${group_members.Arg[${g},|]}
				/echo \a-w     Group Leader:\ag${group_leader} \ao=> \a-wMember:\ag${group_member}
				/if (${group_leader.Equal[${Me.CleanName}]}) {
					/if (${group_member.NotEqual[${Me.CleanName}]}) /invite ${group_member}
				} else {
					/if (${group_member.NotEqual[${group_leader}]}) /dex ${group_leader} /invite ${group_member}
				}
				/delay 2
			/next g
			
			/echo \ag${raid_leader} \ao=> \ag${group_leader}
			/if (${raid_leader.Equal[${Me.CleanName}]}) {
				/if (${group_leader.NotEqual[${Me.CleanName}]}) /raidinvite ${group_leader}
			} else {
				/if (${raid_leader.NotEqual[${group_leader}]}) /dex ${raid_leader} /raidinvite ${group_leader}
			}
		/next j
	/next i
/return

|**
 ----------------------------
 Engagement | Targetting & Killing NPCs
 ----------------------------
 Note:	There is a difference now between engaging a NPC and killing it
 		This was created so we can start debuffing automatically without calling kill
 ----------------------------
**|

Sub EVENT_EngageNPC(string _line, string _sender, string _eventParams)
	/doevents flush EngageNPC
	/call EngageNPC ${_eventParams.Arg[1, ]}
/return

Sub EngageNPC(int _targetID)
	/if (!${_targetID}) 	/return
	/if (${bln_following}) /call StopFollowing

	/echo \awEngaging NPC \ag${Spawn[id ${_targetID}].Name} \ay(${_targetID}) \ao${Time.Time24} \ay${MacroQuest.Running}
	| Set Target Flags
	/varset int_targetID ${_targetID}
	/varset str_targetName ${Spawn[id ${_targetID}].Name}
	/varset bln_engaged TRUE
	/varset bln_killing FALSE

	| Clerics are done here
	/if (${Me.Class.ShortName.Equal[CLR]}) /return

		| True Target the NPC
	/call TrueTarget ${_targetID}

	| Reset recast timers (nukes, dots, etc)
	/if (${Select[${Me.Class.ShortName},BRD,BST,RNG,SHD,ENC,WIZ,NEC,MAG]}) /call resetCastTimers




/return

Sub EVENT_KillNPC(string _line, string _sender, string _eventParams)
	/echo \atKill NPC ${_eventParams}
	/doevents flush KillNPC
	/call KillNPC ${_eventParams.Arg[1, ]}
/return 

Sub KillNPC(int _targetID)
	/if (!${_targetID}) 	/return
	/if (${bln_following})	/call StopFollowing

	/echo \awKilling NPC \ag${Spawn[id ${_targetID}].Name} \ay(${_targetID}) \ao${Time.Time24} \ay${MacroQuest.Running}
	| Set Target Flags
	/varset int_targetID ${_targetID}
	/varset str_targetName ${Spawn[id ${_targetID}].Name}
	/varset bln_engaged TRUE
	/varset bln_killing TRUE

	| Clerics are done here
	/if (${Me.Class.ShortName.Equal[CLR]}) /return

	| Stand if Feigning
	/if ((${Me.Feigning} && (${Me.PctHPs} > 50))) /stand

	| True Target the NPC
	/call TrueTarget ${_targetID}
	
	| Start the Stick Cycle
	/if (${Select[${Me.Class.ShortName},BER,BRD,BST,MNK,RNG,ROG,SHD,WAR]})		/call killTargetMelee

	| Reset recast timers (nukes, dots, etc)
	/if (${Select[${Me.Class.ShortName},BRD,BST,RNG,SHD,ENC,WIZ,NEC,MAG]})		/call resetCastTimers

	| Sicc Pet
	/if ((${Select[${Me.Class.ShortName},BST,ENC,MAG,NEC,SHM]}) &&${Pet.ID})	/call chkPetAttack

	| Stop Sticking if you are MT, cause it's annoying
	/if (${Me.ID} == ${int_MTID} || ${bln_tanking}) /squelch /stick off
/return 

Sub TrueTarget(int _targetID)
	/if (${SpawnCount[id ${_targetID}]} && ${Target.ID} != ${_targetID} ) {
		/declare retryTimer timer local 30
		:Target_Loop
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
 Target & Status Checking
 ----------------------------
 Determines if the main assist has a new target, and whether we can attack
 TODO: #3 We can probably simplify this.
 ----------------------------
**|

Sub chkTargetStatus
	/declare reason string local TARGET_VALID
	/if (!${bln_engaged}) /goto :return

	| Target doesnt exist
	/if (!${SpawnCount[id ${int_targetID}]}) {
		/multiline ; /echo \awTarget with ID \ao${int_targetID} \awNot Found! \ay${str_zone_bound} ; /varset reason TARGET_DOESNT_EXIST 
		/goto :disengage
	}

	| No target, or I am at my bound zone, if engaged, kill
	/if (${int_targetID} == 0 || !${Target.ID} || ${Zone.ShortName.Equal[${str_zone_bound}]}) {
		/multiline ; /echo \awNo Target (${int_targetID}, ${Target.ID}) or at bind! \ay${str_zone_bound} ; /varset reason NOTARGET_or_ATBIND
		/goto :disengage
	}
	
	:chkTargetValid
	| Assumes bln_engaged == true
	| Check for target being corpse or non-existant
	/if ((!${Bool[SpawnCount npc id ${int_targetID}]} || ${Spawn[id ${int_targetID}].Type.Equal[corpse]} || ${Spawn[id ${int_targetID}].Name.Find[s_corpse]})) {
		/multiline ; /echo \awNo Targets with ID(\ay${int_targetID}\aw) or is a CORPSE \aoDisengaging! ; /varset reason TARGET_IS_CORPSE ; /goto :disengage
	} else /if (${Bool[${Target.ID}]} && ${Target.ID} == ${int_MTID} && (!${SpawnCount[npc id ${int_targetID} radius 200]} || ${Zone.ShortName.Equal[${str_zone_bound}]})) {
		/multiline ; /echo \awTarget is MT, and No NPC in radius 200 (or at bind) \aoDisengaging! ; /varset reason TARGETMT_or_ATBIND 
	} else  /if ((${Me.XTarget}==0 && !${SpawnCount[npc radius 200]})) {
		/multiline ; /echo \awNo Targets within 200, or on XTarget \aoDisengaging! ;/varset reason NOTARGETSWITHIN200_AND_NOXTARGET ;/goto :disengage
	} else /if (${Bool[${Me.XTarget}]} && (!${SpawnCount[npc radius 400]}  || ${Zone.ShortName.Equal[${str_zone_bound}]})) {
		/multiline ; /echo \awNo Targets within 400, even if they are on XTarget \aoDisengaging! ; /varset reason NOTARGETWITHIN400_AND_HASXTARGET ; /goto :disengage
	}

	| Priests can skip next check and just go to check hott aggro
	/if (${Select[${Me.Class.ShortName},SHM,CLR,DRU]}) /goto :chkTargetTarget

	| I am engaged, check we are targetting correct NPC (not if we are tanking becausse that is annoying)
	/if ((${Target.ID} != ${int_targetID}) && (${Spawn[id ${int_targetID}].PctHPs} >= 0 && !${Spawn[id ${int_targetID}].Name.Find[s_corpse]}) && !${bln_tanking}) {
		| I am not targetting the correct NPC
		/echo \aoCurrent Target \arNOT \ao${Spawn[id ${int_targetID}].Name}(${int_targetID})\aw, re-targetting
		/call TrueTarget ${int_targetID}
	} else /if ((${Target.ID} != ${int_targetID}) && (${Target.PctHPs} >= 0 && !${Spawn[id ${int_targetID}].Name.Find[s_corpse]}) && ${bln_tanking}) {
		/varset int_targetID ${Target.ID}
	}
	
	:chkTargetTarget
	/call TargetNamedCheck ${int_targetID}
    /declare isNamed bool local ${Macro.Return}
	/if (${Target.Type.Equal[NPC]} && ${Me.TargetOfTarget.CleanName.Equal[${Me.CleanName}]} && !${bln_tanking} && ${isNamed}) {
		/call attemptDumpAggro
	}
	/return

	:disengage
		/echo \aoChkTargetStatus \awTriggering Disengage: \ag${reason}
		/if (${reason.NotEqual[TARGET_VALID]}) /call Disengage

	:return
/return


Sub chkTargetStatusNonDPS
	/if ((${SpawnCount[npc radius 300]} == 0 || ${Me.XTarget} == 0) && ${bln_killing} ) {
		/echo chkTargetStatusNonDPS triggered XTarget:${Me.XTarget} || ${SpawnCount[npc id ${int_targetID} radius 300]} SpawnCount:!${SpawnCount[npc id ${int_targetID} radius 250]}  && engaged:${bln_engaged}
		/call Disengage
	}
/return

Sub TargetNamedCheck(_id)
	/if (${Target.Named}) /return TRUE
	/if ((${Spawn[npc id ${_id}].Named} || ${Spawn[npc id ${_id}].Name.Find[#]}) && ${Target.Level} >= 75) 				/return TRUE
	/if (${Select[${Zone.ShortName},tacvi,chambersa,chambersb,chambersc,chambersd,chamberse]} && ${Target.Level} >=71)	/return TRUE
	/if (${Zone.ShortName.Equal[anguish]}  && ${Target.Level} >= 75)													/return TRUE
	/if (${Zone.ShortName.Equal[arena]}) 																				/return TRUE
	/if (${Zone.ShortName.Equal[thundercrest]}) 																		/return TRUE
/return FALSE

|TODO #4
Sub attemptDumpAggro
	| Priest/Caster (with some sort of a aggro dump ability)
	/if (${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) {
		| Wizard
		/if (${Me.Class.ShortName.Equal[WIZ]}) {
			/if (${Me.AltAbilityReady[Mind Crash]})   	/call MQ2Cast "Mind Crash" alt
			/call chkDefensiveSpells
		} else /if (${Me.Class.ShortName.Equal[NEC]}) {
			/call chkDefensiveSpells
			/if (!${Me.Standing}) /stand
		} else /if (${Me.Class.ShortName.Equal[ENC]}) {
			/if (${Me.AltAbilityReady[Soothing Words]}) /call MQ2Cast "Soothing Words" alt	
		} else /if (${Me.Class.ShortName.Equal[MAG]}) {

		} else /if (${Me.Class.ShortName.Equal[CLR]}) {
			/if (${Me.AltAbilityReady[Sanctuary]}) 		/call MQ2Cast "Sanctuary" alt	
		} else /if (${Me.Class.ShortName.Equal[SHM]}) {

		} else /if (${Me.Class.ShortName.Equal[DRU]}) { 

		}
	} else /if (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]} || ${str_class_type.Equal[TANK]}) {
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
			/if (${use_riposte}) /call FireDisc_Riposte
			/if (${Me.CombatAbilityReady[Baffling Strike]} && ${FindItemCount[Axe of the Destroyer]}>0) /doability "Baffling Strike"
			/if (${Me.CombatAbilityReady[Confusing Strike]} && ${FindItemCount[Rage Axe]}>0) 			/doability "Confusing Strike"
			/if (${Me.CombatAbilityReady[Distracting Strike]} && ${FindItemCount[Bonesplicer Axe]}>0) 	/doability "Distracting Strike"
		} else /if (${Me.Class.ShortName.Equal[ROG]}) { 
			/if (${use_riposte}) /call FireDisc_Riposte
		} else /if (${Me.Class.ShortName.Equal[RNG]}) { 
			/if (${Me.SpellReady[${str_concSpell}]})	/call MQ2Cast "${str_concSpell}" ${str_concGem}
			/if (${use_riposte}) /call FireDisc_Riposte	
		} else /if (${Me.Class.ShortName.Equal[MNK]}) { 
			/if (${Me.AbilityReady[Feign Death]}) /doability "Feign Death"
			/if (${use_riposte}) /call FireDisc_Riposte	
			/if (!${Me.Standing}) /stand
		}
	}
/return

Sub Event_RefreshXTarget
	/call RefreshXTarget
/return 

SUB RefreshXTarget
	/if (${timer_xtarget_refresh}) /return
	/echo \aoRefreshing \aoXTarget \ay${timer_xtarget_refresh}
	/declare i int local 1
	/for i 1 to 5
		/xtarget set ${i} grouptank
		/delay 1
		/xtarget set ${i} autohater
	/next i
	/varset timer_xtarget_refresh 60s
/return

Sub Event_RageOn(string _line, string _sender, string _eventParams)
	/doevents flush RageOn
	/if ((${_line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${_line.Find[${Target.DisplayName}]}) && (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]} || ${str_class_type.Equal[TANK]})) {
		/attack off
	}
	/if (((${_line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${_line.Find[${Target.DisplayName}]}) &&${Pet.ID}) && !${Me.PetBuff[Master's Aura Effect]}) {
		/echo Enrage detected! Backing pet off.
		/pet back off
		/varset bln_enraged TRUE
	}
	
/return

Sub Event_RageOff(string _line, string _sender, string _eventParams)
	/doevents flush RageOff
	/if (${_line.Find[${Spawn[${int_targetID}].DisplayName}]} || ${_line.Find[${Target.DisplayName}]}) {
		/if (${Select[${str_class_type},MELEE_END,MELEE_MANA,TANK]}) /attack on
		/if (${Pet.ID}) /call chkPetAttack
	}
	/if ((${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]} || ${str_class_type.Equal[TANK]}) && ${_line.Find[${Target.DisplayName}]}) {
		/echo  \awRageOff :tryKill \ag Attacking 
		/attack on
	}
	
/return

Sub chkNaked
	/doevents flush chkNaked
	/if (${Me.Inventory[2].Name.Equal[NULL]} && !${timer_announce_naked}) {
		/docommand /${str_announce_channel} I am NAKED. 
		/varset bln_naked 				TRUE
		/varset timer_announce_naked	30s
	} else {
		/varset bln_naked				FALSE
	}
/return

|** 
 ----------------------------
 Disengagement
 ----------------------------
 Turns off attack/casting routines on NPC, etc
 ----------------------------
**|

Sub EVENT_Disengage 
	/doevents flush Disengage
	/call Disengage
/return 

Sub Disengage
		/echo \awDisengaging \ao${Time.Time24}
	|-- global reset
	/varset bln_engaged 		FALSE
	/varset bln_killing			FALSE
	/varset int_targetID		0
	/varset str_targetName		NULL
	|/varset str_debuffedTargets NULL

	|-- back off pet
	/if (${Pet.ID}) /multiline ; /pet back off ; /pet hold on
	/call RunPriorityEvents

	| Priests have nothing left to do here;
	/if (${str_class_type.Equal[PRIEST]}) /return

	|-- class archetype resets
	/if (${Select[${str_class_type},MELEE_END,MELEE_MANA,TANK]}) {
		/echo \ag Disengage: Attack Off
		/squelch /attack off
		/delay 5
		/squelch /attack off
		/stick off
	}
/return

Sub EVENT_Zoned 
	/doevents flush Zoned
	/echo \awEvent_Zoned ${Time.Time24}
	/call setExclusions
	/if (${MacroQuest.GameState.Equal[CHARSELECT]}) /end
  	
/return 

|** 
 ----------------------------
 Tank Checking
 ----------------------------
 Determines if the main tank is dead, if so, set new MT
 ----------------------------
**|

Sub chkMTAlive(bool _setAsTarget)
	|--MT is a live and ID is correct
	/if (${Bool[${SpawnCount[PC ${str_MTName}]}]} && (${int_MTID} == ${Spawn[PC ${str_MTName}].ID})) /return

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
		/varset int_STID ${Spawn[${str_TTName}].ID}
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
		/varset bln_tanking TRUE
		/varset bln_taunting TRUE
	}
	
	|-- If set as target, target the new MT immediately
	/if (${_setAsTarget} && (${Target.ID} != ${int_MTID})) {
		/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		/call TrueTarget ${int_MTID}
	}
/return

Sub EVENT_ChangeMT(string _line, string _sender, string _eventParams)
	/doevents flush ChangeMT
	/call setMT ${_eventParams.Arg[1, ]}
/return

Sub setMT(string _tankName)
	/echo \agSetMT(${_tankName}):: ${Spawn[${_tankName}].ID}
	/varset str_MTName 		${_tankName}
	/varset int_MTID 		${Spawn[${_tankName}].ID}
	/varset bln_tanking 	${Me.Name.Equal[${_tankName}]}
	/varset bln_taunting 	${Me.Name.Equal[${_tankName}]}
	/if (${Me.Class.ShortName.Equal[CLR]}) /call TrueTarget ${int_MTID}
	/if (${str_class_type.Equal[PRIEST]}) /docommand /${str_announce_channel} [${Me.Class.ShortName}] ${Me.Name}  ${Spawn[id ${int_MTID}].CleanName}
/return


Sub getMTbyPrecedence
	|-- Start with the defined backup Tanks
	/if (${SpawnCount[pc ${str_MTName}]}) {
		/varset str_MTName ${str_MTName}
	} else /if (${SpawnCount[pc ${str_STName}]}) {
		/varset str_MTName ${str_STName}
	}  else /if (${SpawnCount[pc ${str_TTName}]}) {
		/varset str_MTName ${str_TTName}
	} else /if (${NearestSpawn[pc WAR].CleanName.NotEqual[NULL]}) {
		/varset str_MTName ${NearestSpawn[PC WAR].CleanName}
	} else /if (${NearestSpawn[pc SK].CleanName.NotEqual[NULL]}) {
		/varset str_MTName ${NearestSpawn[PC SHD].CleanName}
	} else /if (${NearestSpawn[pc PAL].CleanName.NotEqual[NULL]}) {
		/varset str_MTName ${NearestSpawn[PC PAL].CleanName}
	} else /if (${NearestSpawn[pc MNK].CleanName.NotEqual[NULL]}) {
		/varset str_MTName ${NearestSpawn[PC MNK].CleanName}
	} else {
		/echo NO Suitable Replacement MT's met, set self to MT
		/varset str_MTName ${Me.Name}
	}
	/varset int_MTID 	${Spawn[PC ${str_MTName}].ID}
	
	/echo \arGetMTbyPrecedence: \ag${str_MTName} \ao(\ag${int_MTID}\ao)
/return ${newID}

|** 
 ----------------------------
 AA Special Events
 ----------------------------
 Determines if the main tank is dead, if so, set new MT
 ----------------------------
**|
Sub EVENT_enabledStaunchRecovery
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${str_zone_bound}]}) /return
	/if ((${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) && ${Me.PctMana} < 20) { 
		/call MQ2Cast "Staunch Recovery" alt 
	} 
/return

Sub EVENT_enabledIntensity
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${str_zone_bound}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley "Intensity of the Resolute" ALT ${int_targetID} TRUE
	} else {
		/call MQ2Cast "Intensity of the Resolute" alt
	}
/return

Sub EVENT_enabledServants
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${str_zone_bound}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley "Steadfast Servant" ALT ${int_targetID} FALSE
	} else {
		/call MQ2Cast "Steadfast Servant" ALT
	}
/return

Sub EVENT_enabledInfusion
	| Do not use if at bind point (ie: dead)
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${str_zone_bound}]}) /return
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley "Infusion of the Faithful" ALT ${int_targetID} FALSE
	} else {
		/call MQ2Cast "Infusion of the Faithful" ALT
	}
/return

|** 
 ----------------------------
 Inventory Management
 ----------------------------
 TODO #6
 ----------------------------
**|
Sub RestoreFaceSlot
	/exchange "${str_item_face_default}" face
	/delay 5
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
	|/if (${str_class_type.Equal[CASTER]} || ${str_class_type.Equal[PRIEST]} && ${_weaponset.NotEqual[default]}) /return
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
			/docommand /${str_announce_channel} Could not find ${weap}, fix my ini!
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
			/docommand /${str_announce_channel} Could not find ${weap}, fix my ini!
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
Sub EVENT_SaveEquipedWeaponset(string _line, string _sender, string _eventParams)
	/call SaveCurrentWeaponset ${_eventParams}
/return

Sub SaveEquipedWeaponse(string _key)
	/echo Saving Current Weaponset as ${_key}
	/if (${_key.Equal[NULL]}) /varset _key test
	/declare pri string local ${Me.Inventory[mainhand].Name}
	/declare off string local ${Me.Inventory[offhand].Name}
	
 	/ini INI_CHAR "Weaponsets" "${_key}" "${pri}|mainhand,${off}|offhand"
/return

|** 
 ----------------------------
 Out of Combat (!${bln_engaged}) Utility Checking
 ----------------------------
 Defined by Class 
 ----------------------------
**|

Sub chkUtils
	/call chkSit
	/if (${Select[${Me.Class.ShortName},BST,MAG,NEC]}) /call chkPet
	/varset timer_check_utilities 20s
/return 

Sub chkSit 
	/if (${Me.Moving} || ${Navigation.Active} || ${bln_tanking}) /return
	|-- Mana Check
	/if (${str_class_type.Equal[CASTER]} ||  ${str_class_type.Equal[PRIEST]}) {
		| Sit my ass down
		/if ( ${Me.PctMana}<=${int_meditationPctMana} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sittingRadius} zradius 55]} == 0)) {
			|/echo I should be sitting my ass down for MANA Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit
		} 
		
	}
	|-- Endurance Check
	/if (${str_class_type.Equal[MELEE_END]} ||  ${str_class_type.Equal[TANK]}) {
		/if ( ${Me.PctEndurance}<=${int_meditationPctMana} && !${bln_engaged} && (${SpawnCount[npc radius ${int_sittingRadius} zradius 55]} == 0)) {
			|/echo I should be sitting my ass down for Endurance Am I sitting? ${Me.Sitting}
			/if (!${Me.Sitting}) /sit 
		} 
	}
/return

Sub chkUtils_BER
/return 

Sub chkUtils_BRD 
/return 

Sub chkUtils_BST
/return 

Sub chkUtils_CLR
/return 

Sub chkUtils_ENC
/return 

Sub chkUtils_MAG
/return 

Sub chkUtils_MNK
/return 

Sub chkUtils_NEC
/return 

Sub chkUtils_PAL
/return 

Sub chkUtils_RNG
/return 

Sub chkUtils_ROG
/return 

Sub chkUtils_SHD
/return 

Sub chkUtils_SHM
/return 

Sub chkUtils_WAR 
/return 

Sub chkUtils_WIZ
/return 

|** 
 ----------------------------
 Dannet Observer Utilities
 ----------------------------
 Create/Remove Observers
 Requires MQ2SQLite and a SQLite database
 ----------------------------
**|

| CREATE an observer. This is a shortcut method that takes several arguments
| _table: the SQLite table to inspect
| _action: add|drop
Sub createObserver(int _id, string _name, string _table, string _tlo, string _alias, string _command)
	/declare _count           int     local 1
	/declare _countObs        int     local 1
	/declare _action          string  local FALSE
	/declare _toon            string  local FALSE
	/declare _timestart       int     local 0
	/declare _timeend         int     local 0
	/declare _found           bool    local FALSE
	/declare _tmpVar          string  local FALSE
	/declare _tmpObs          string  local FALSE
	/declare _countFail       int     local 0

	| Since we are creating an observer, let us make sure the timer
	/call updateTimer ${_alias} 1s RESTART

	
	| create the SQL command
	/if (${_command.Equal[drop]}) {
		/varset _action -drop
	} else /if (${_command.Equal[add]}) {
		/varset _action
	} 


	| Create record in DB if not exist
	/if (!${sqlite.Rows[${_table}]}) {
		/sqlite query "${gbl_db_path}" ${_table} SELCT * FROM ${_table}
	}

	| Not working? Exit
	/if (!${sqlite.Rows[${_table}]}) {
		/return FALSE
	}

	| Look through Dannet
	/while (${_count} <= ${DanNet.Peers}) {
		/varset _timestart ${MacroQuest.Running}
    	/varset _toon ${DanNet.Peers.Value}].Arg[${_count},|]}
    	/varset _countFail 0

		| skip self
		/if (${_toon.Equal[${Me.DisplayName}]}) {
			/varcalc _count ${_count} + 1
			/continue
		}

		/varset _found FALSE
    	/varset _countObs 1
		/echo \a-w${sqlite.Rows[${_table}]}\ax
		/echo \aoToon\aw ${_count}\ax::\at${_toon}\ax

		  | start our trip throught the sql data
		/while (${_countObs} <= ${sqlite.Rows[${_table}]}) {
			/varset _tmpVar ${_toon}_${sqlite.Result[${_table} ${_countObs} env_var]}
			/varset _tmpObs ${sqlite.Result[${_table} ${_countObs} value]}

			| define or delete the variables as we need
			/if (${_action.Equal[drop]}) {
				/if (!${DanNet[${_toon}].OSet["${_tmpObs}"]}) {
					/varcalc _countObs ${_countObs} + 1
					/continue
				}

			} else /if (${_action.Equal[add]}) {
				/if (${DanNet[${_toon}].OSet["${_tmpObs}"]}) {
					/varcalc _countObs ${_countObs} + 1
					/continue
				}

				| if the observer variable does not exist create it
				/if (!${Defined[${_tmpVar}]}) {
					/declare ${_tmpVar} string outer
				}
			}

			| set observer or drop it
			/dobserve ${_toon} -q "${_tmpObs}" -o ${_tmpVar} ${_actionO}
			/delay 5 ${DanNet[${_toon}].OSet["${_tmpObs}"]}
			/if (!${DanNet[${_toon}].OSet["${_tmpObs}"]}) {
				/varcalc _countFail ${_countFail} + 1
			}
			/echo \a-t/dobserve\ax ${_toon} -q \a-w"${_tmpObs}"\ax -o ${_tmpVar} ${_actionO} (\a-w${DanNet[${_toon}].OSet["${_tmpObs}"]}\ax)

			/varset _found TRUE
			/varcalc _countObs ${_countObs} + 1
		}


		| set an end time for our test for that characer
		/varset _timeend ${MacroQuest.Running}

		| give us some response so we know that something is happening
		/if ((!${_silent} || ${_debug}) && ${_found}) {
			/invoke ${out[17, "Observe (${If[${_action.Equal[add]},\agadd\ax,\ardrop\ax]})${sep}${_table}(\a-r${_countFail}\ax)${sep}\aw${_toon}\ax${sep}\a-w${Math.Calc[((${_timeend}-${_timestart}) / 10) / 60]}s\ax"]}
		}

		| moving to the next toon in the dannet
		/varcalc _count ${_count} + 1
		/varset _found FALSE
	}
/return TRUE

Sub removeObserver(int _id, string _alias)

/return

Sub updateTimer(string _alias, string _value, string _command)

/return


|----------------------------------------------------|
| Save Jewelry Slots		 |
|----------------------------------------------------|
|--Save Events
Sub EVENT_SaveJewelry
	/doevents flush SaveJewelry 
	/call SaveRingset
	/call SaveFaceSlot
	/call SaveEarrings
/return 

Sub EVENT_SaveRingset
	/doevents flush SaveRingset
	/call SaveRingset
/return

Sub EVENT_SaveFaceSlot
	/doevents flush SaveFaceSlot 
	/call SaveFaceSlot
/return 

Sub EVENT_SaveEarrings
	/doevents flush SaveEarrings 
	/call SaveEarrings
/return 

|-----
|-- Save Subs
Sub SaveRingset
	/declare leftfinger 	string	local	${Me.Inventory[leftfinger].Name}
	/declare rightfinger 	string	local	${Me.Inventory[rightfinger].Name}

	/echo \aoSaving Ring Slots \awleft: \ag${leftfinger} \awright: \ag${rightfinger}
	/ini "INI_CHAR" "Jewelry" "leftfinger" "${leftfinger}"
	/ini "INI_CHAR" "Jewelry" "rightfinger" "${rightfinger}"
/return

Sub SaveFaceSlot
	/declare face			string	local	${Me.Inventory[face].Name}
	/echo \aoSaving Face Slot \awface: \ag${face}
	/ini "INI_CHAR" "Jewelry" "face" "${face}"
/return 

Sub SaveEarrings 
	/declare leftear 	string	local	${Me.Inventory[leftear].Name}
	/declare rightear 	string	local	${Me.Inventory[rightear].Name}

	/echo \aoSaving Ear Slots \awleft: \ag${leftear} \awright: \ag${rightear}
	/ini "INI_CHAR" "Jewelry" "leftear" "${leftear}"
	/ini "INI_CHAR" "Jewelry" "rightear" "${rightear}"
/return 


|-- method to load the saved weapons from INI (primarily for Trial of Specialization)
Sub EVENT_LoadWeaponset(string _line, string _sender, string _evtParams)
	/echo \ayLoading Weaponset <${_evtParams}> (${_sender})
	/if (${_evtParams.NotEqual[NULL]}) /call LoadWeaponset ${_evtParams}
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

| TODO rewrite this like we did for rings
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



Sub Event_SkillCheck(string _line, string _sender, string _eventParams)
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[TANK]} || ${Me.Class.ShortName.Equal[BRD]}) /return
	/echo SkillCheck: ${_line} ${_sender}
	/echo params ${_eventParams}

	/declare sval int local  ${Me.Skill[${_eventParams}]}
	/if (${sval} == 0) {
		/docommand /${announceChannel} ${Me.Skill[${_eventParams}]}
		/nav wp lobby
	}
	/doevents flush SkillCheck

/return




|-----
Sub EVENT_RemoveRingset
	/echo removingRingset
	/doevents flush RemoveRingset
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
	/doevents flush LoadRingset
	/echo \ayLoading Ringset
	/call LoadRingset
/return

Sub LoadRingset
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/declare lr string local ${Ini[INI_CHAR,Jewelry,leftfinger,NULL]}
	/declare rr string local ${Ini[INI_CHAR,Jewelry,rightfinger,NULL]}

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
	/doevents flush LoadFaceSlot
	/call LoadFaceSlot
/return 

Sub LoadFaceSlot
	/if (${Me.Class.ShortName.Equal[BRD]}) /medley stop
	/declare face string local ${Ini[INI_CHAR,Jewelry,face,NULL]}
	
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
/return 