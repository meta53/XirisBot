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
#event OFFTANKON 			"[#1#] OFFTANKON#*#"
#EVENT OFFTANKON 			"[#1#(msg)] OFFTANKON#*#"
#event OFFTANKON 			"<#1#> OFFTANKON#*#"
#event OFFTANKON 			"#1# tells you, 'OFFTANKON#*#'"
#event OFFTANKON 			"#1# tells the group, 'OFFTANKON#*#'"
#event OFFTANKON 			"#1# tells the party, 'OFFTANKON#*#'"

#event OFFTANKOFF 			"[#1#] OFFTANKOFF #2#"
#EVENT OFFTANKOFF 			"[#1#(msg)] OFFTANKOFF#*#"
#event OFFTANKOFF 			"<#1#> OFFTANKOFF#*#"
#event OFFTANKOFF 			"#1# tells you, 'OFFTANKOFF#*#'"
#event OFFTANKOFF 			"#1# tells the group, 'OFFTANKOFF#*#'"
#event OFFTANKOFF 			"#1# tells the party, 'OFFTANKOFF#*#'"

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
		/bc ${Me.Name} is now OFF TANKING!
		/varset  bln_offTank TRUE
		/melee aggro=on
		/varset meleeAggro TRUE
	}
	/if (!${_blnOT}) {
		/bc ${Me.Name} is ENDING OFF TANKING.
		/varset  bln_offTank FALSE
		/melee aggro=off
		/varset meleeAggro FALSE
	}
	
/return

Sub chkOffTanking	
	/if (!${bln_offTank}) 	 /return
	
	/declare i int local
	/declare victimType string local
	/declare max int local

	:checkOTStart
	/call RunRaidEvents	
	
	/if (${Me.XTarget} == 0) {
		/if (${Melee.AggroMode}) /melee aggro=off
		/return
	}
	
	|-- There is one target, and its engaged, so make sure I am smacking it
	/if (${Me.XTarget} == 1 && ${Me.XTarget[1].ID} && ${bln_engaged}) {
		/call TrueTarget ${Me.XTarget[1].ID}
		/if (${Melee.AggroMode}) /melee aggro=off
		/killthis
		/return
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
				/echo XTarget is on exclusion list, continue
				/goto :next
			}
			
			|-- Is this NPC within Offtanking Range? If not, skip it
			/if (${Me.XTarget[${i}].Distance} >  ${int_OT_dist}) { 
				/echo XTarget is too far away to OT, continue
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
				/echo victim is me or tank, continue
				/goto :next
			}
				 
			|-- Is this NPC banging on a PLATE class already?
			/if (${Me.TargetOfTarget.ID}) {
				/call getClassArchetype ${Me.TargetOfTarget.Class.ShortName}
				/varset victimType			${Macro.Return}
				/if (${victimType.Equal[TANK]}) {
					/echo victim is tank, continue
					/goto :next
				}
			}
			
			|-- Exhausted the options, let's get the mobs attention
		    /squelch /melee aggro=on
			/call aggroByID ${Me.XTarget[1].ID}
		/delay 30
	
		
		:next
		/melee aggro=off
		/next i
		
	}
	
	/goto :checkOTStart
	
/return

Sub aggroByID(int _targetID)	
	/if (!${Me.Standing}) /stand on
	/if (!${bln_engaged}) /varset bln_engaged TRUE
	/echo aggroByID ${_targetID}

	:aggroLoop
	
	|-- if SK/PAL try casting on it, if not, smack it;
	/if (${Me.Class.ShortName.Equal[PAL]}) {
		| Paladin should try to stun the NPC
		/call chkStun
	} else /if (${Me.Class.ShortName.Equal[SHD]}) {
		| SK should try hate spell
		/call chkHate
	} else {
		/if (!${Melee.Attack}) {
			/squelch /stick off
			/squelch /stick id ${_targetID}
			/squelch /face fast
			/squelch /killthis
		}
	}
		
	/if ((${Me.TargetOfTarget.ID} == ${Me.ID}) || (${Me.TargetOfTarget.ID} == ${int_MTID})) {
		|/echo ${Target.Name} has been aggrod/offtanked, giving it 5s
		/delay 5s
		/melee reset
		/return
	}
	/if (!${Target.ID}) {
		/melee reset
		/return
	}
	
	/goto :aggroLoop
/return


|**
 ----------------------------
 Stunning
 ----------------------------
 Checks current target, stuns if not on MT or Self
 ----------------------------
**|

Sub chkStun
	/if (!${bln_engaged}) /return
	/if (!${Target.ID}) /return
	/if ( ${Me.SpellReady[${str_stun1Spell}]}) {
		/echo chkStun: Stunning with ${str_stun1Spell}
		/call Cast "${str_stun1Spell}" ${str_stun1Gem} 2s
	} else /if ( ${Me.SpellReady[${str_stun2Spell}]}) {
		/echo chkStun: Stunning with ${str_stun2Spell}
		/call Cast "${str_stun2Spell}" ${str_stun2Gem} 2s
	}
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
		/call Cast "${spell_hate}" ${spell_hateGem} 2s
	}

/return




Sub xbot_initialize_offtanking
	/cecho \ao xbot_initialize_offtanking
	/call InitEvents_Offtanking
	
	|-- [Settings] : Off Tanking
	/declare bln_offTank		 	bool 	outer FALSE
	/declare int_OT_dist 			int 	outer 65
	
	

/return
 