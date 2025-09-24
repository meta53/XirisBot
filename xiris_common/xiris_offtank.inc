|**
----------------------------
xiris_offtank.inc
----------------------------
Offtank module for Xiris bots
Determines if there are mobs within a certain radius, and if they are not currently
tanked by MT or OT.
______________________________________________
REQUIRED PLUGINS
__________________________
REVISION HISTORY
	12.04.11    xiris 	INITIAL REVISION
	10.12.2017	xiris	Cleaned up nulls
	11.24.2021	xiris	Changed target looping to use XTarget since that is now supported on EMU
	01.09.2022	xiris	Made sure to turn aggro off when only hitting one mob
	09.15.2022	xiris	Slight re-write of the offtank logic to definitively fix the 'stuck on corpse' issue
	02.09.2023	xiris 	Fixed the trying to offtank NPCs in area that aren't aggro.
______________________________________________
**|	
#EVENT OFFTANKON			"#1# tells the group#*#'OfftankOn#*#'"
#EVENT OFFTANKON			"#1# tells you#*#'OfftankOn#*#'"
#EVENT OFFTANKON			"#1# tells the raid#*#OfftankOn#*#'"
#EVENT OFFTANKON	 		"[ #1# #*#]#*#OfftankOn#*#"

#EVENT OFFTANKOFF			"#1# tells the group#*#'OfftankOff#*#'"
#EVENT OFFTANKOFF			"#1# tells you#*#'OfftankOff#*#'"
#EVENT OFFTANKOFF			"#1# tells the raid#*#OfftankOff#*#'"
#EVENT OFFTANKOFF	 		"[ #1# #*#]#*#OfftankOff#*#"

#EVENT OFFTANKRADIUS		"#1# tells the group#*#'OfftankRadius #2#'"
#EVENT OFFTANKRADIUS		"#1# tells you#*#'OfftankRadius #2#'"
#EVENT OFFTANKRADIUS		"#1# tells the raid#*#OfftankRadius #2#'"
#EVENT OFFTANKRADIUS	 	"[ #1# #*#]#*#OfftankRadius #2#"

#EVENT DION					"#1# tells the group#*#'DIOn#*#'"
#EVENT DION					"#1# tells you#*#'DIOn#*#'"
#EVENT DION					"#1# tells the raid#*#DIOn#*#'"
#EVENT DION	 				"[ #1# #*#]#*#DIOn#*#"

#EVENT DIOFF				"#1# tells the group#*#'DIOff#*#'"
#EVENT DIOFF				"#1# tells you#*#'DIOff#*#'"
#EVENT DIOFF				"#1# tells the raid#*#DIOff#*#'"
#EVENT DIOFF	 			"[ #1# #*#]#*#DIOff#*#"


Sub InitEvents_Offtanking 
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) /call RegisterBackgroundEvents Events_Offtanking 5
/return

Sub Events_Offtanking 
	/doevents OFFTANKON
	/doevents OFFTANKOFF
	/doevents OFFTANKRADIUS
	/doevents DION
	/doevents DIOFF
/return

Sub EVENT_OFFTANKON
	/call ToggleOfftanking TRUE
/return

Sub EVENT_OFFTANKOFF
	/call ToggleOfftanking FALSE
/return

Sub EVENT_OFFTANKRADIUS(_line, _chatSender, _eventParams) 
	/call ChangeOfftankRadius ${_eventParams}
/return

Sub ChangeOfftankRadius(_radius)
	/varset int_offtankRadius ${Int[${_radius}]}
/return

SUB EVENT_DION
	/varset bln_useDI TRUE
/return 

Sub EVENT_DIOFF
	/varset bln_useDI FALSE
/return

Sub ToggleOfftanking(bool _blnOT)
	|--Toggle Offtanking
	/if (${bln_tanking}) /return
	/if (${_blnOT}) {
		/dgt ${Me.Name} is now OFF TANKING!
		/varset  bln_offTank	TRUE
		/varset  bln_tanking		TRUE
		/varset  bln_taunting 	TRUE
		/varset  bln_tanking	TRUE
	}
	/if (!${_blnOT}) {
		/dgt ${Me.Name} is ENDING OFF TANKING.
		/varset  bln_offTank	FALSE
		/varset  bln_tanking		FALSE
		/varset  bln_taunting 	FALSE
		/varset  bln_tanking	FALSE
	}
	
/return



Sub chkOffTanking
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${cache_boundZone}]} || ${bln_following}) /return
	/if (${bln_engaged} && ${bln_useDI}) /call chkDivineIntervention

	| If there are no spawns within OT Distance, return
	/if (!${SpawnCount[npc radius ${int_offtankRadius} range 46 80]}) {
		| Refresh XTarget if we haven't done it in the last 60s
		/if (!${timer_xtargetRefresh}) /call RefreshXTarget
		/return
	}
	:OffTankCheck
	| Check Raid Events
	/call RunRaidEvents
	/if (!${bln_offTank} || ${bln_following}) 	 /return

	| Check all NPCs within offtankRadius
	/declare i int local
	/for i 1 to ${SpawnCount[npc radius ${int_offtankRadius} range 46 80 noalert 5]}
		/doevents OFFTANKOFF
		/call TrueTarget ${NearestSpawn[${i}, npc radius ${int_offtankRadius} range 46 80 noalert 5].ID}
		:CheckAggro
		/if (!${Me.TargetOfTarget.ID} || !${Me.TargetOfTarget.ID} || (${ExcludeList.Find[${Target.CleanName}]})) /goto :nextTarget
		/if (${Me.TargetOfTarget.Class.ShortName.Equal[WAR]} || ${Me.TargetOfTarget.Class.ShortName.Equal[PAL]} || ${Me.TargetOfTarget.Class.ShortName.Equal[SHD]}) /goto :nextTarget
		/if ((${Target.Distance3D} < ${int_offtankRadius})) {
			/call aggroByID ${Target.ID}
		} else {
			/squelch /target clear
		}
	:nextTarget
	/delay 5
	/next i

	:NormalKillCheck
	| Check to see if there is only one NPC, and if we are engaged, turn aggro off and attack it
	/if (${SpawnCount[npc radius ${int_offtankRadius} range 46 80  noalert 5]} == 1 && ${bln_engaged}) {
		/if (${SpawnCount[npc id ${int_targetID}]}) {
			/call TrueTarget ${int_targetID}
			/if (!${Me.TargetOfTarget.ID}) /return
			/varset bln_tanking		FALSE
			/varset bln_taunting	FALSE
			/if (${Target.PctHPs} <=99  && ${Target.ID}) /attack on
		}
	}
/return


Sub aggroByID(int _targetID)	
	
	/if (!${Me.Standing}) /stand on
	/if (!${bln_engaged}) /varset bln_engaged TRUE
	/echo \ao Aggroing \ay${Target.Name}\aw(\ag${_targetID}\aw)
	/echo \aw Class being Targetted: \ar${Me.TargetOfTarget.Class.ShortName}
	:aggroLoop
	|-- if SK/PAL try casting on it, if not, smack it;
	/if (${Me.Class.ShortName.Equal[PAL]}) {
		| Paladin should try to stun the NPC
		/call chkStun TRUE
		/goto :aggroSmack
	} else /if (${Me.Class.ShortName.Equal[SHD]}) {
		| SK should try hate spell
		/call chkHate
		/goto :aggroSmack
	} 
	
	:aggroSmack
	/squelch /varset bln_tanking TRUE
	/squelch /varset bln_taunting TRUE
	/if (${Target.ID} != ${_targetID}) /call TrueTarget ${_targetID}

	/squelch /stick off
	/squelch /stick front 20
	/if (${Me.AbilityReady[Taunt]}) /doability taunt
	/squelch /face fast
	/if (!${Me.Combat}  && ${Target.ID}) {
		/echo  \awaggroByID \ag Attacking 
		/attack on
	} 
	/delay 20

	
	/if (${Me.TargetOfTarget.Class.ShortName.Equal[WAR]} || ${Me.TargetOfTarget.Class.ShortName.Equal[PAL]} || ${Me.TargetOfTarget.Class.ShortName.Equal[SHD]}) /goto :return		
	/if ((${Me.TargetOfTarget.ID} == ${Me.ID}) || (${Me.TargetOfTarget.ID} == ${int_MTID}) || !${Target.ID} || ${Target.Name.Find['s_corpse']} || ${cache_targetIsCorpse}) /goto :return
	
	/goto :aggroLoop

	:return
	/varset bln_tanking		FALSE
	/varset bln_taunting	FALSE
/return



|**
 ----------------------------
 Hate
 ----------------------------
 Checks current target, stuns if not on MT or Self
 ----------------------------
**|

Sub chkHate
	/if (!${bln_engaged}) /return
	/if (!${Target.ID}) /return
	/if ( ${Me.SpellReady[${spell_hate}]}) {
		/echo chkHate: Hating with ${spell_hate}
		/call MQ2Cast "${spell_hate}" ${spell_hateGem} 2s
	}
/return


Sub xbot_initialize_offtanking
	/echo \ao xbot_initialize_offtanking
	|-- [DEFAULT_SETTINGS] : Off Tanking
	/declare bln_offTank		 	bool 	outer FALSE
	/declare int_offtankRadius		int     outer 85
	/declare bln_useDI				bool    outer FALSE

	/call InitEvents_Offtanking
	/squelch /alert clear 5
	| Specific NPCs to never offtank
	| Note in future pull from exclude list
	/squelch /alert add 5 "Huhn-Abram"
	/squelch /alert add 5 "Voracious Feran"
	/squelch /alert add 5 "Uchti"
	/squelch /alert add 5 "Keldovan"
	/squelch /alert add 5 "Jomica"
	/squelch /alert add 5 "cursebearer"
	/squelch /alert add 5 "Ansdaicher"
	/squelch /alert add 5 "Zellucheraz"
/return
 