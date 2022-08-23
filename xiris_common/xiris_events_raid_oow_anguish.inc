|**
----------------------------
EVENTS_OOW_ANGUISH.INC
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	07.20.22    xiris	INITIAL REVISION
______________________________________________
**|	

|-- Citadel of Anguish
|	Keldovan
#EVENT KeldovanStart	"#*##*#"

|   Ture
#EVENT TureRampOn       "#*#roars with fury#*#"
#EVENT TureRampOff      "#*#calms and regains#*#"
#EVENT TureWanton		"#*#dissolves into a landscape#*#"

|	Jelvan
#EVENT JelvanStart  	"#*#Hail, Jelvan#*#"
#EVENT Jelvan1 			"#*#MD1#*#"
#EVENT Jelvan2			"#*#MD2#*#"
#EVENT Jelvan3 			"#*#MD3#*#"

|	Warden Hanvar
#EVENT HanvarStart		"#*#hanvarStart#*#"
#EVENT HanvarCaster		"#*#hanvarCaster#*#"

|	Arch Magus Vangl
#EVENT AMVPrep			"#*#AMVPrep#*#"

|   OMM
#EVENT GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#EVENT GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#EVENT GazeDone			"#*#Mata Muram's spell has been reflected#*#"



Sub InitEvents_OOW_Anguish
	/cecho \ag Initializing Anguish
	/declare bln_jelvanEngaged bool outer FALSE
	/declare bln_hanvarEngaged bool outer FALSE
	/declare bln_amvEngaged	   bool outer FALSE
	/call RegisterRaidEvents Events_OOW_Anguish 15

	|-- Waypoints
| 	Pulled from MQ2Nav		   X	   Y	  Z
	/declare J1T string outer -163.085 2127.05 -152.985
	/declare J1C string outer -126.979 2093.99 -152.917
	/declare J2T string outer -236.852 2003.96 -152.739
	/declare J2C string outer -253.437 1970.36 -152.672
	/declare J3T string outer -351.176 2117.72 -152.972
	/declare J3C string outer -390.152 2100.88 -152.939

	/declare J1TX int outer ${J1T.Arg[1, ]}
	/declare J1TY int outer ${J1T.Arg[2, ]}
	/declare J1TZ int outer ${J1T.Arg[3, ]}

	/declare J1CX int outer ${J1C.Arg[1, ]}
	/declare J1CY int outer ${J1C.Arg[2, ]}
	/declare J1CZ int outer ${J1C.Arg[3, ]}
	
	/declare J2TX int outer ${J2T.Arg[1, ]}
	/declare J2TY int outer ${J2T.Arg[2, ]}
	/declare J2TZ int outer ${J2T.Arg[3, ]}
	
	/declare J2CX int outer ${J2C.Arg[1, ]}
	/declare J2CY int outer ${J2C.Arg[2, ]}
	/declare J2CZ int outer ${J2C.Arg[3, ]}
	
	/declare J3TX int outer ${J3T.Arg[1, ]}
	/declare J3TY int outer ${J3T.Arg[2, ]}
	/declare J3TZ int outer ${J3T.Arg[3, ]}
	
	/declare J3CX int outer ${J3C.Arg[1, ]}
	/declare J3CY int outer ${J3C.Arg[2, ]}
	/declare J3CZ int outer ${J3C.Arg[3, ]}

	| Hanvar
	/declare H1C string outer 505.688 4307.43 206.677

	/declare H1CX int outer ${H1C.Arg[1, ]}
	/declare H1CY int outer ${H1C.Arg[2, ]} 
	/declare H1CZ int outer ${H1C.Arg[3, ]}
/return

Sub Events_OOW_Anguish 
	/if ((${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]}) && ${bln_engaged}) /call chkDivineIntervention

	| Ture Events
	/if (${Bool[${SpawnCount[npc Ture]}]} && ${Spawn[npc Ture].Distance3D} < 300) {
		/doevents TureRampOn
		/doevents TureRampOff
	}

	| Jelvan Events
	/if (${Bool[${SpawnCount[npc Jelvan]}]} && ${Spawn[npc Jelvan].Distance3D} < 300) {
		/call chkJelvanCures
		/doevents JelvanStart
		/doevents Jelvan1
		/doevents Jelvan2
		/doevents Jelvan3
	}
	| Hanvar Events
	/if (${Bool[${SpawnCount[npc Hanvar]}]} && ${Spawn[npc Hanvar].Distance3D} < 500) {
		/doevents HanvarStart
		/doevents HanvarCaster
		/if (${bln_hanvarEngaged} && ${Me.Class.ShortName.Equal[CLR]}) {
			/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${H1CY},${H1CX},${H1CZ}]} > 30) {
				/if (!${Navigation.Active}) /nav wp hanvarCleric
			} else /if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${H1CY},${H1CX},${H1CZ}]} <= 30 && (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${H1CY},${H1CX},${H1CZ}]} > 10)) {
				/if (${Navigation.Active}) /nav stop
				/warp wp hanvarCleric
			}
		}
	}

	| Vangl Events
	/if (${Bool[${SpawnCount[npc Vangl]}]} && ${Spawn[npc Vangl].Distance3D} < 500) {
		/doevents AMVPrep
		/if (${Me.Buff[Feedback Dispersion].ID} && ${Me.AltAbilityReady[Purify Soul]} && ${Me.Class.ShortName.Equal[CLR]}) {
			/target id ${Me.ID}
			/casting "Purify Soul" alt
		}
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[PAL]}) /call chkMarkedForDeath
		
	} 

	| OMM Events
	/if (${Bool[${SpawnCount[npc Overlord]}]} && ${Spawn[npc Overlord].Distance3D} < 300) {
		/doevents GazeYou
		/doevents GazeBurst
		/doevents GazeDone
	} 

/return 

|----------------------------
| Ture
|----------------------------

Sub Event_TureRampOn
	/if (!${bln_engaged}) /return
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/squelch /melee reset
		/squelch /melee reset
		/squelch /attack off
		/stick off
		/nav wp tureSafe
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]}) {
		/pet hold on
		/pet back off
	}
/return 


Sub Event_TureRampOff 
	/if (!${bln_engaged}) /return
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/target npc Ture
		/squelch /killthis
		/nav wp tureDPS
		:ture_run 
		/if (${Target.Distance} < 40) /nav stop
		/if (${Navigation.Active}) /goto :ture_run
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]}) {
		/pet attack
	}
/return 

Sub Event_TureWanton 
	/cecho \aw Warning! \arWanton Destruction \awon me!
/return

|----------------------------
| Jelvan
|----------------------------

Sub Event_JelvanStart 
	/cecho \aw JelvanStart fired! Already Engaged? \ag${bln_jelvanEngaged}
	/if (${bln_jelvanEngaged}) /return
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
		/if (!${bln_jelvanEngaged}) /varset bln_jelvanEngaged TRUE
		/return
	}

	:Tormentor_Checks
	/cecho \ayRunning Tormentor Checks!
|-------------	
| West, 1 MT	
	/if (${Me.CleanName.Equal[Xiria]}) {
		/if (${SpawnCount[npc Tanthi]} == 1 && !${bln_jelvanEngaged}) {
			/call TrueTarget ${Spawn[npc Tanthi].ID}
			/varset bln_jelvanEngaged TRUE
			/disc Stonewall
			/killthis
			/rs DebuffTarget ${Target.ID}
			/rs sing melee
			/if (${Me.CombatAbilityReady[Bazu Bellow]}) /disc Bazu Bellow
			/delay 20
			/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /disc Ancient: Chaos Cry
		}
| West, 1 RT
	} else /if (${Me.CleanName.Equal[Sadelia]}) {
		/if (${SpawnCount[npc Tanthi]} == 1 && !${bln_jelvanEngaged}) {
			/call TrueTarget ${Spawn[npc Tanthi].ID}
			/varset bln_jelvanEngaged TRUE
			/killthis 
		}
|-------------
| South, 2 MT
	} else /if (${Me.CleanName.Equal[Xiris]}) {
		/if (${SpawnCount[npc Tanthu]} == 1 && !${bln_jelvanEngaged}) {
			/call TrueTarget ${Spawn[npc Tanthu].ID}
			/varset bln_jelvanEngaged TRUE
			/disc Stonewall
			/killthis 
			/rs DebuffTarget ${Target.ID}
			/rs sing melee
			/if (${Me.CombatAbilityReady[Bazu Bellow]}) /disc Bazu Bellow
			/delay 20
			/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /disc Ancient: Chaos Cry
		}

| South, 2 RT		
	} else /if (${Me.CleanName.Equal[Erinyes]}) {
		/if (${SpawnCount[npc Tanthu]} == 1 && !${bln_jelvanEngaged}) {
			/call TrueTarget ${Spawn[npc Tanthu].ID}
			/varset bln_jelvanEngaged TRUE
			/killthis 
		}

|-------------
| East, 3 MT
	} else /if (${Me.CleanName.Equal[Xirea]}) {
		/cecho \awchecking for \agTantho! ${SpawnCount[npc Tantho]} 
		/if (${SpawnCount[npc Tantho]} == 1 && !${bln_jelvanEngaged}) {
			/call TrueTarget ${Spawn[npc Tantho].ID}
			/varset bln_jelvanEngaged TRUE
			/disc Stonewall
			/killthis 
			/rs DebuffTarget ${Target.ID}
			/rs sing melee
			/if (${Me.CombatAbilityReady[Bazu Bellow]}) /disc Bazu Bellow
			/delay 20
			/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /disc Ancient: Chaos Cry
		}

| East, 3 RT
	} else /if (${Me.CleanName.Equal[Xirin]}) {
		/if (${SpawnCount[npc Tantho]} == 1 && !${bln_jelvanEngaged}) {
			/call TrueTarget ${Spawn[npc Tantho].ID}
			/varset bln_jelvanEngaged TRUE
			/killthis 
		}
	}

	| Loop de derp
	/if (!${bln_jelvanEngaged}) {
		/delay 10
		/goto :Tormentor_Checks
	}


/return

Sub chkJelvanCures
	/if (${str_classArchetype.NotEqual[PRIEST]} || ${Me.Class.ShortName.NotEqual[PAL]}) /return

	| Gaze of Anguish (hp/mana drain)
	/if (${Me.Buff[Gaze of Anguish].ID}) {
		/if (${Me.Class.ShortName.Equal[SHM]}) {
			/call cureGroup disease 30
		} else /if (${Me.Class.ShortName.Equal[CLR]}) {
			/call cureSelf disease 30
		} else /if (${Me.Class.ShortName.Equal[DRU]}) {
			/call cureSelf disease 30
		} else /if (${Me.Class.ShortName.Equal[PAL]}) {
			/call cureSelf disease 30
		}		
	} else /if (${Me.Buff[Relinquish Spirit].ID}) {
		/if (${Me.Class.ShortName.Equal[SHM]}) {
			/call cureGroup curse 24
		} else /if (${Me.Class.ShortName.Equal[CLR]}) {
			/call cureSelf curse 24
		} else /if (${Me.Class.ShortName.Equal[DRU]}) {
			/call cureSelf curse 24
		} else /if (${Me.Class.ShortName.Equal[PAL]}) {
			/call cureSelf curse 24
		}		
	}
/return 


| Movement on Jelvan
Sub EVENT_Jelvan1
	/doevents flush Jelvan1
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop

	/if (${Me.Class.ShortName.Equal[CLR]}) {
		/return 
		|/nav wp jelvan1cleric
		|/call MoveToPosition ${J1CY} ${J1CX} ${J1CZ} 30 FALSE
	} else /if (${str_classArchetype.Equal[CASTER]}) {
		/return
	} else {
		/nav wp jelvan1tank
		/call MoveToPosition ${J1TY} ${J1TX} ${J1TZ} 30 FALSE
	}
/return 

Sub EVENT_Jelvan2
	/doevents flush Jelvan2
	/if (${Navigation.Active}) /nav stop

	/if (${Me.Class.ShortName.Equal[CLR]}) { 
		/nav wp jelvan2cleric
		/call MoveToPosition ${J2CY} ${J2CX} ${J2CZ} 30 FALSE
	} else {
		/nav wp jelvan2tank
		/call MoveToPosition ${J2TY} ${J2TX} ${J2TZ} 30 FALSE
	}
/return 

Sub EVENT_Jelvan3
	/doevents flush Jelvan3
	/if (${Navigation.Active}) /nav stop

	/if (${Me.Class.ShortName.Equal[CLR]}) {
		/return 
		|/nav wp jelvan3cleric
		|/call MoveToPosition ${J3CY} ${J3CX} ${J3CZ} 30 FALSE
	} else /if (${str_classArchetype.Equal[CASTER]}) {
		/return
	} else {
		/nav wp jelvan3tank
		/call MoveToPosition ${J3TY} ${J3TX} ${J3TZ} 30 FALSE
	}
/return






|----------------------------
| Warden Hanvar
|----------------------------
Sub Event_HanvarStart
	/varset bln_hanvarEngaged TRUE
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call TrueTarget ${Spawn[npc hanvar]}
		/nav target
	}
	/doevents flush HanvarStart
/return

Sub Event_HanvarCaster
	/varset bln_hanvarEngaged TRUE 
	/if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]} ||  ${Me.Class.ShortName.Equal[DRU]}) {
		/nav wp hanvarCaster
	}
	/doevents flush HanvarCaster
/return


|----------------------------
| Arch Magus Vangl
|----------------------------
Sub Event_AMVPrep 
	/varset bln_amvEngaged TRUE 
	/if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[DRU]}) {
		/nav wp amvCaster
	} else /if (${Me.Class.ShortName.Equal[CLR]}) {
		/nav wp amvCleric
	} else {
		/nav wp amvDPS
	}
	/doevents flush AMVPrep
/return

Sub chkMarkedForDeath
	/if (${bln_chChainMember}) /return
 
	/declare i int local 1
	/declare haveMark int local 0
	/declare gcount[5]  int local 0
	/declare foundMark bool local FALSE

	:CheckGroup
	/varset haveMark 	0
	/varset foundMark 	FALSE
	/for i 0 to ${Group}
		/dquery ${Group.Member[${i}]} -q "Me.Buff[Mark of Death].ID"
		/delay 20 ${DanNet.Query.Received}
		/if (${Bool[${DanNet.Q}]}) {
			/varset  foundMark TRUE
			/varcalc haveMark ${haveMark}+1
			/call getCureCounters ${Group.Member[${gmember}].Name} ${_cureType}
			/varset gcount[${i}] ${Int[${Macro.Return}]}
		}
	/next i
	
	| No Marks found? return
	/if (!${foundMark}) /goto :return

	| If Marks are less than 3, or I am a paladin then single target cure them
	:SingleCure
	/if (${Me.Class.ShortName.Equal[PAL]} || ${haveMark} < 3) /goto :CureByCounters
	
	| If I am a cleric or shaman, cast group cure, and then recheck counters
	:GroupCure 
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.SpellReady[${str_heal_group}]} && ${Me.PctMana} > 30 && ${haveMark} >= 3) {
		/call MQ2Cast "${str_heal_group}" gem4 2s 
		/goto :CheckGroup
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/call cureSelf disease 18 
		/goto :CheckGroup
	}

	| All else fails so cure anyone with a disease counter singly
	:CureByCounters
	/for i 0 to ${Group} 
		/if (${gcount[${i}]} > 0) {
			/call handleCureRequest ${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} disease ${gcount[${i}]}
		}
	/next i
:return
/return

|----------------------------
| Overlord Mata'Muram
|----------------------------
Sub Event_GazeYou 
/return 

Sub Event_GazeBurst
/return 

Sub Event_GazeDone
/return