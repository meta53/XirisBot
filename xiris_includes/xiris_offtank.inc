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
	08.11.2023	xiris 	Slight re-write to handle 2.0 INI files, etc
______________________________________________
**|	
#EVENT OffTankON			"#1# tells the group#*#'OfftankOn#*#'"
#EVENT OffTankON			"#1# tells you#*#'OfftankOn#*#'"
#EVENT OffTankON			"#1# tells the raid#*#OfftankOn#*#'"
#EVENT OffTankON	 		"[ #1# #*#]#*#OfftankOn#*#"

#EVENT OffTankOFF			"#1# tells the group#*#'OfftankOff#*#'"
#EVENT OffTankOFF			"#1# tells you#*#'OfftankOff#*#'"
#EVENT OffTankOFF			"#1# tells the raid#*#OfftankOff#*#'"
#EVENT OffTankOFF	 		"[ #1# #*#]#*#OfftankOff#*#"

#EVENT OffTankRadius		"#1# tells the group#*#'OfftankRadius #2#'"
#EVENT OffTankRadius		"#1# tells you#*#'OfftankRadius #2#'"
#EVENT OffTankRadius		"#1# tells the raid#*#OfftankRadius #2#'"
#EVENT OffTankRadius	 	"[ #1# #*#]#*#OfftankRadius #2#"

#EVENT OffTankDivineInterventionON		"#1# tells the group#*#'DIOn#*#'"
#EVENT OffTankDivineInterventionON		"#1# tells you#*#'DIOn#*#'"
#EVENT OffTankDivineInterventionON		"#1# tells the raid#*#DIOn#*#'"
#EVENT OffTankDivineInterventionON		"[ #1# #*#]#*#DIOn#*#"

#EVENT OffTankDivineInterventionOFF		"#1# tells the group#*#'DIOff#*#'"
#EVENT OffTankDivineInterventionOFF		"#1# tells you#*#'DIOff#*#'"
#EVENT OffTankDivineInterventionOFF		"#1# tells the raid#*#DIOff#*#'"
#EVENT OffTankDivineInterventionOFF		"[ #1# #*#]#*#DIOff#*#"


Sub initEvents_Offtanking 
	/call RegisterBackgroundEvents Events_Offtanking_Background
/return

Sub Events_Offtanking_Background 
	/doevents OffTankON
	/doevents OffTankOFF
	/doevents OffTankRadius
	/doevents OffTankDivineInterventionON
	/doevents OffTankDivineInterventionOFF
/return

Sub EVENT_OffTankON
	/doevents flush OffTankON
	/call ToggleOfftanking TRUE
/return

Sub EVENT_OffTankOFF
	/doevents flush OffTankOFF
	/call ToggleOfftanking FALSE
/return

Sub EVENT_OffTankRadius(string _line, string _sender, string _eventParams) 
	/doevents flush OffTankRadius
	/call ChangeOfftankRadius ${_eventParams}
/return

SUB EVENT_OffTankDivineInterventionON
	/doevents flush OffTankDivineInterventionON
	/varset bln_useDI TRUE
/return 

Sub EVENT_OffTankDivineInterventionOFF
	/doevents flush OffTankDivineInterventionOFF
	/varset bln_useDI FALSE
/return

Sub xbot_initialize_offtanking
	/if (!${Select[${Me.Class.ShortName},PAL,SHD,WAR]}) /return
	/echo \n \apInitialize: \aoOfftanking \aw-------------------------
	
	|-- [Settings] : Off Tanking
	/declare int_offtankRadius		int     outer 85
	/declare bln_useDI				bool    outer FALSE

	/call initEvents_Offtanking
	/squelch /alert clear 5
	| Specific NPCs to never offtank
	| Note in future pull from exclude list
	/squelch /alert add 5 "Huhn-Abram"
	/squelch /alert add 5 "Voracious Feran"
	/squelch /alert add 5 "Uchti"
	/squelch /alert add 5 "Keldovan"
	/squelch /alert add 5 "Jomica"
	/squelch /alert add 5 "cursebearer"
/return
 
 Sub ChangeOfftankRadius(int _radius)
	/varset int_offtankRadius ${_radius}
/return

Sub ToggleOfftanking(bool _blnOT)
	|--Toggle Offtanking
	/if (${bln_tanking}) /return
	/if (${_blnOT}) {
		/dgt ${Me.Name} is now OFF TANKING!
		/varset  bln_offTank	TRUE
		/varset  bln_tanking	TRUE
		/varset  bln_taunting 	TRUE
	}
	/if (!${_blnOT}) {
		/dgt ${Me.Name} is ENDING OFF TANKING.
		/varset  bln_offTank	FALSE
		/varset  bln_tanking	FALSE
		/varset  bln_taunting 	FALSE
	}
	
/return



Sub chkOffTanking
	/if (${Zone.ID} == 340 || ${Zone.ID} == 344 || ${Zone.ID} == 345 || ${Zone.ShortName.Equal[${str_zone_bound}]} || ${bln_following}) /return
	/if (${bln_engaged} && ${bln_useDI}) /call chkDivineIntervention

	| If there are no spawns within OT Distance, return
	/if (!${SpawnCount[npc radius ${int_offtankRadius} range 46 80]}) {
		| Refresh XTarget if we haven't done it in the last 60s
		/if (!${timer_xtarget_refresh}) /call RefreshXTarget
		/return
	}
	:OffTankCheck
	| Check Raid Events
	/call RunRaidEvents
	/if (!${bln_offTank} || ${bln_following}) 	 /return

	| Check all NPCs within offtankRadius
	/declare i int local
	/for i 1 to ${SpawnCount[npc radius ${int_offtankRadius} range 46 80 noalert 5]}
		/call RunPriorityEvents
		/call TrueTarget ${NearestSpawn[${i}, npc radius ${int_offtankRadius} range 46 80 noalert 5].ID}
		:CheckAggro
		/if (!${Me.TargetOfTarget.ID} || (${ExcludeList.Find[${Target.CleanName}]})) /goto :nextTarget
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
	/call RunPriorityEvents
	/if (${Target.ID} != ${_targetID}) /call TrueTarget ${_targetID}
	/call chkTaunt
	
	/if (!${Me.Combat}  && ${Target.ID}) {
		/echo  \awaggroByID \ag Attacking 
		/attack on
	} 
	/delay 20

	
	/if (${Select[${Me.TargetOfTarget.Class.ShortName},WAR,PAL,SHD]} || ${Target.Name.Find['s_corpse']} || ${Target.Type.Equal[corpse]} ) /goto :return		

	/goto :aggroLoop

	:return
	/varset bln_tanking		FALSE
	/varset bln_taunting	FALSE
/return


