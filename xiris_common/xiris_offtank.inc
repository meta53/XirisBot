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

Sub chkOfftankingPriorityTargets

/return

Sub chkOffTanking	
	/if (!${bln_offTank}) 	 /return
	
	/declare i int local
	/declare victimType string local
	/declare max int local

	:checkOTStart
	/call RunRaidEvents	
	/if (!${timer_xtargetRefresh} && !${SpawnCount[npc radius 100]}) {
		/call doRefreshXTarget
		/varset timer_xtargetRefresh ${timer_xtargetRefresh.OriginalValue}
	}
	/if (${SpawnCount[npc radius 100]} == 0) /goto :return
	/if (${Me.XTarget} == 0)  /goto :return
		
	
	|-- There is one target, and its engaged, so make sure I am smacking it
	/if (${Me.XTarget} == 1 && ${Me.XTarget[1].ID} && ${bln_engaged} ) {
		/call TrueTarget ${Me.XTarget[1].ID}
		/if (${Me.TargetOfTarget.ID} && (${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]})) {
			/squelch /killthis
			/goto :return
		}
		
	} else { 
	|-- There is more than one target, let us cycle through them and inspect buffs (make sure they aren't mezzed)
		/varset max ${Me.XTarget}
		/varset i 1
		/echo OffTanking ${max} Adds (${Me.XTarget} > 1) && (${Me.XTarget[1].ID}) && (${bln_engaged})
		
		|--- Loop through the targets
		/for i 1 to ${max} 
			:checkOTTarget
			/call RunRaidEvents	
			/delay 5
	

			|-- Is this NPC in the Exclude List? If so, skip it
			/if (${ExcludeList.Find[${Spawn[id ${Me.XTarget[${i}].ID}].CleanName}]}) { 
				/echo XTarget is on exclusion list, next
				/goto :next
			}
			
			|-- Is this NPC within Offtanking Range? If not, skip it
			/if (${Me.XTarget[${i}].Distance} >  ${int_OT_dist}) { 
				/echo XTarget is too far away to OT, next
				/goto :next
			}
			
			|-- Next step: Target the NPC so we can get ToT
			/echo Targetting: ${i}:  ${Me.XTarget[${i}].Name} :: ${Target.Name}
			/call TrueTarget ${Me.XTarget[${i}].ID}
			/delay 10

			|-- Is this NPC actually targetting anything?
			/if (!${Me.TargetOfTarget.ID}) { 
				/echo XTarget has no target, continue
				/goto :next
			}
			
			|-- Is this NPC banging on ME or the MT already?
			/if ((${Me.TargetOfTarget.ID} == ${Me.ID}) || (${Me.TargetOfTarget.ID} == ${int_MTID})) { 
				/echo Victim is me or tank, next
				/goto :next
			}
				 
			|-- Is this NPC banging on a PLATE class already?
			/if (${Me.TargetOfTarget.ID} && (${Me.TargetOfTarget.Class.ShortName.Equal[PAL]} || ${Me.TargetOfTarget.Class.ShortName.Equal[SHD]} || ${Me.TargetOfTarget.Class.ShortName.Equal[WAR]})) {
				/echo victim is tank, next
				/goto :next
			}
			
			|-- Exhausted the options, let's get the mobs attention
			/call aggroByID ${Me.XTarget[1].ID}
		/delay 30
	
		
		:next
		/next i
		
	}
	
	/goto :checkOTStart
	
	:return
	/if (${Melee.AggroMode}) /melee aggro=off
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
			/if (!${Melee.AggroMode}) /squelch /melee aggro=on
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

	|-- [Settings] : Off Tanking
	/declare bln_offTank		 	bool 	outer FALSE
	/declare int_OT_dist 			int 	outer 65
	/declare timer_xtargetRefresh	timer	outer 30s
	
	/cecho \ao xbot_initialize_offtanking
	/call InitEvents_Offtanking
	

/return
 