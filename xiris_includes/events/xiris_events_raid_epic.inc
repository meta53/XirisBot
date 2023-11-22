|**
----------------------------
xiris_events_raid_epic.inc
----------------------------
Event Handlers for the Raid Events for Epic 1, 1.5, and 2.0
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	12.10.22    xiris	INITIAL REVISION
______________________________________________
**|	



Sub initEvents_Epic
	/echo \ag Initializing EPIC Raid events
    /call RegisterRaidEvents Events_Epic 2
/return

Sub Events_Epic
    | Rogue 1.5 fight in Greater Faydark
    /if (${Zone.ShortName.Equal[gfaydark]}) /call chkMarsingerCursor
    | Rogue 1.5 fight in Steamfont
    /if (${Zone.ShortName.Equal[steamfont]}) /call chkClockworkDoom
/return 


Sub chkMarsingerCursor
    /if (${Cursor.ID}==3574) /destroy
/return 

Sub chkClockworkDoom
    /if (${Me.Class.ShortName.Equal[SHM]} && ${Me.Class.ShortName.Equal[CLR]}) /call chkGroupClockworkDoom
/return

Sub chkGroupClockworkDoom
	/if (${Me.Class.ShortName.Equal[CLR]} && (${bln_chChainMember} || ${bln_focusHealMT})) /return
 
	/declare i int local 0
	/declare haveMark int local 0
	/declare gcount[6]  int local 0
	/declare foundMark bool local FALSE
	/declare tankIndex int local 10
	/declare cureWaitCount int local 0
	/declare gm int local 1

	:CheckGroup
	/varset haveMark 	0
	/varset foundMark 	FALSE
	
	| Remember, Group.Member needs 0-5, whereas the array needs 1-6
	/for i 1 to 6
		/varcalc gm ${i}-1
		/if (${Group.Member[${gm}].CleanName.Equal[${str_MTName}]}) /varset tankIndex ${gm}
		/dquery ${Group.Member[${gm}]} -q "Me.Buff[Clockwork Doom].ID"
		/delay 20 ${DanNet.Query.Received}
		/if (${Bool[${DanNet.Q}]}) {
			/echo \ar${Group.Member[${gm}]} \awreply:${Bool[${DanNet.Q}]}
			/echo \ar CLOCKWORK DOOM FOUND
			/varset  foundMark TRUE
			/varcalc haveMark ${haveMark}+1
			/call getCureCounters ${Group.Member[${gm}]} poison
			/varset gcount[${i}] ${Int[${Macro.Return}]}
		} 

		
	/next i
	
	| No Marks found? return
	/if (!${foundMark}) /goto :return

	/echo \ar${haveMark} \aoInstances of Clockwork Doom found!
	| If Marks are less than 3, or I am a paladin then single target cure them
	
	| If I am a cleric or shaman, cast group cure, and then recheck counters
	:GroupCure 
	/echo \awGroupCure: \ag${str_heal_group} \ay${Me.SpellReady[${str_heal_group}]} \aw(\ag${Me.PctMana}) \ay${haveMark}>=2
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.PctMana} > 30 && ${haveMark} >= 2) {
		
		:GroupCureWait 
		/varcalc cureWaitCount ${cureWaitCount}+1
		/if (!${Me.SpellReady[${str_heal_group}]}) {
			/delay 5
			/if (${cureWaitCount} > 10) {
				/echo \arGroupCure failed due to wait, trying by counters
				/goto :CureByCounters
			} else {
				/echo \arGroupCure waiting for ${str_heal_group} ${Me.SpellReady[${str_heal_group}]}
				/goto :GroupCureWait
			}
		} else {
			/echo \agGroupCuring with ${str_heal_group}
			/call MQ2Cast "${str_heal_group}" gem4 2s 
			/goto :CheckGroup
		}
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/echo \agGroupCuring with ${str_cure_poison}
		/g Group Heal incoming, get close!
		/call MQ2Cast "${str_cure_poison}" gem7 8s
		/goto :CheckGroup
	}

	| All else fails so cure anyone with a poison counter singly
	:CureByCounters
	/echo \awCureByCounters: \ag${str_cure_poison} \ayReady?${Me.SpellReady[${str_cure_poison}]} \aw(\ag${Me.PctMana}) \ay${haveMark}< 2
	/for i 1 to 6
		/varcalc gm ${i}-1
		/call chkHeal_MT
		/if (${gcount[${i}]} > 0) {
			/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} poison ${gcount[${i}]} TRUE
		}
	/next i

	:CheckMT
	/if (${tankIndex}<10) {
		
		/dquery ${Group.Member[${tankIndex}]} -q "Me.Buff[Clockwork Doom].ID"
		/delay 20 ${DanNet.Query.Received}
		
		/if (${Bool[${DanNet.Q}]}) {
			/echo \ar${Group.Member[${tankIndex}]} \awreply: ${DanNet.Q} ${Bool[${DanNet.Q}]}
			/echo \ar CLOCKWORK DOOM FOUND on TANK
			/call getCureCounters ${Group.Member[${tankIndex}].Name} poison
			/call handleCureRequest ${Group.Member[${tankIndex}].Name} ${Group.Member[${tankIndex}].ID} poison ${Int[${Macro.Return}]} TRUE
		}
	}
:return
/return
