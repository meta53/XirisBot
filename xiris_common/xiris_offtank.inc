|**
----------------------------
XIRIS_OFFTANK.INC updated version 10.12.2017
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

Sub InitEvents_Offtanking 
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) /call RegisterBackgroundEvents Events_Offtanking 2
/return

Sub Events_Offtanking 
	/doevents OFFTANKON
	/doevents OFFTANKOFF
/return

Sub EVENT_OFFTANKON
	/call ToggleOfftanking TRUE
	/doevents flush OFFTANKON
/return

Sub EVENT_OFFTANKOFF
	/call ToggleOfftanking FALSE
	/doevents flush OFFTANKOFF
/return

Sub ToggleOfftanking(bool _blnOT)
	|--Toggle Offtanking
	/if (${_blnOT}) {
		/dgt ${Me.Name} is now OFF TANKING!
		/varset  bln_offTank TRUE
		/varset bln_aggro TRUE
	}
	/if (!${_blnOT}) {
		/dgt ${Me.Name} is ENDING OFF TANKING.
		/varset  bln_offTank FALSE
		/varset bln_aggro FALSE
	}
	
/return

Sub chkOffTanking
	/if (!${bln_offTank}) 	 /return

	| If there are no spawns within OT Distance, return
	/if (!${SpawnCount[npc radius ${int_offtankRadius} range 46 80]}) {
		| Refresh XTarget if we haven't done it in the last 30s
		/if (!${timer_xtargetRefresh}) /call doRefreshXTarget
		/return
	}
	:OffTankCheck
	| Check Raid Events
	/call RunRaidEvents

	| Check all NPCs within offtankRadius
	/declare i int local
	/for i 1 to ${SpawnCount[npc radius ${int_offtankRadius} range 46 80 noalert 5]}
		/call TrueTarget ${NearestSpawn[${i}, npc radius ${int_offtankRadius} range 46 80 noalert 5].ID}
		:CheckAggro
		/if (!${Bool[${Me.TargetOfTarget.ID}]} || (${ExcludeList.Find[${Target.CleanName}]})) /goto :nextTarget
		/if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < ${int_offtankRadius}) {
			/call aggroByID ${Target.ID}
		} else {
			/target clear
		}
	:nextTarget
	/next i

	:NormalKillCheck
	| Check to see if there is only one NPC, and if we are engaged, turn aggro off and attack it
	/if (${SpawnCount[npc radius ${int_offtankRadius} range 46 80  noalert 5]} == 1 && ${bln_engaged}) {
		/if (${NearestSpawn[npc radius ${int_offtankRadius} range 46 80 noalert 5].ID} == ${int_targetID}) {
			/call TrueTarget ${int_targetID}
			/squelch /melee aggro=off
			/squelch /melee taunt=off
			/if (${Target.PctHPs} <=99) /killthis
		}
	}
/return


Sub aggroByID(int _targetID)	
	/if (!${Me.Standing}) /stand on
	/if (!${bln_engaged}) /varset bln_engaged TRUE
	/echo aggroByID ${_targetID}

	:aggroLoop
	|-- if SK/PAL try casting on it, if not, smack it;
	/if (${Me.Class.ShortName.Equal[PAL]}) {
		| Paladin should try to stun the NPC
		/call chkStun FALSE
	} else /if (${Me.Class.ShortName.Equal[SHD]}) {
		| SK should try hate spell
		/call chkHate
	} else {
		/if (!${Melee.Attack}) {
			/squelch /melee aggro=on
			/squelch /melee taunt=on
			/squelch /stick off
			/squelch /stick id ${_targetID}
			/if (${Me.AbilityReady[Taunt]}) /taunt
			/squelch /face fast
			/squelch /killthis
		}
	}
		
	/if ((${Me.TargetOfTarget.ID} == ${Me.ID}) || (${Me.TargetOfTarget.ID} == ${int_MTID}) || !${Target.ID}) {
		/goto :return
	}

	/goto :aggroLoop

	:return
	/melee reset
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
	/cecho \ao xbot_initialize_offtanking
	|-- [Settings] : Off Tanking
	/declare bln_offTank		 	bool 	outer FALSE
	/declare int_OT_dist 			int 	outer 65
	/declare int_offtankRadius		int     outer 85

	

	/call InitEvents_Offtanking

	| Specific NPCs to never offtank
	/alert add 5 "Huhn-Abram"
	/alert add 5 "Voracious Feran"

/return
 