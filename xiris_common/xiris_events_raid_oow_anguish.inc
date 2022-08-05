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
#EVENT HanvarStart		"#*#Hail, Warden Hanvar#*#"

|	Arch Magus Vangl
#EVENT MarkedForDeath	"#*#You have been marked for death#*#"

|   OMM
#EVENT GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#EVENT GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#EVENT GazeDone			"#*#Mata Muram's spell has been reflected#*#"



Sub InitEvents_OOW_Anguish
	/cecho \ag Initializing Anguish
	/declare bln_jelvanEngaged bool outer FALSE
	/call RegisterRaidEvents Events_OOW_Anguish 5

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
/return

Sub Events_OOW_Anguish 
	/if (${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]}) /call chkDivineIntervention

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
	/if (${Bool[${SpawnCount[npc Hanvar]}]} && ${Spawn[npc Hanvar].Distance3D} < 300) {
		/doevents HanvarStart
	}

	| Vangl Events
	/if (${Bool[${SpawnCount[npc Vangl]}]} && ${Spawn[npc Vangl].Distance3D} < 300) {
		/doevents MarkedForDeath
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
		/nav wp jelvan1cleric
		/call MoveToPosition ${J1CY} ${J1CX} ${J1CZ} 30 FALSE
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
		/nav wp jelvan3cleric
		/call MoveToPosition ${J3CY} ${J3CX} ${J3CZ} 30 FALSE
	} else {
		/nav wp jelvan3tank
		/call MoveToPosition ${J3TY} ${J3TX} ${J3TZ} 30 FALSE
	}
/return






|----------------------------
| Warden Hanvar
|----------------------------
Sub Event_HanvarStart
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call TrueTarget ${Spawn[npc hanvar]}
		/nav target
	}
/return 

|----------------------------
| Arch Magus Vangl
|----------------------------
Sub Event_MarkedForDeath
	/if (${Me.Class.ShortName.Equal[SHM]}) /call cureSelf disease 14
	/if (${Me.Class.ShortName.Equal[CLR]}) /call cureSelf disease 14
	/if (${Me.Class.ShortName.Equal[DRU]}) /call cureSelf disease 14
	/if (${Me.Class.ShortName.Equal[PAL]}) /call cureSelf disease 14
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