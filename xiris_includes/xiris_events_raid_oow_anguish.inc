|**
----------------------------
xiris_events_raid_oow_anguish.inc
----------------------------
Event Handlers for the OOW Citadel of Anguish raid.
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

|	Jelvan
#Event JelvanSetup		"#*#JelvanSetup#*#"
#EVENT JelvanStart  	"#*#Hail, Jelvan#*#"


|	Warden Hanvar
#EVENT HanvarSetup  	"#*#HanvarSetup#*#"
#EVENT HanvarStart		"#*#HanvarStart#*#"
#EVENT HanvarRetreat    "#1# tells the raid#*#'HanvarRetreat #2#'"
#EVENT HanvarKill       "#1# tells the raid#*#'HanvarKill #2#'"

|	Arch Magus Vangl
#EVENT AMVStart			"#*#AMVStart#*#"
#EVENT MarkOfDeath		"#*#You have been marked for death#*#"
#EVENT AMVKill 			"#1# tells the raid#*#'AMVKill #2#'"

|   OMM
#EVENT GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#EVENT GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#EVENT GazeDone			"#*#Mata Muram's spell has been reflected#*#"



Sub initEvents_OOW_Anguish
	/echo \ag Initializing Anguish
	/if (!${Defined[bln_jelvanEngaged]}) 	/declare bln_jelvanEngaged 	bool	outer FALSE
	/if (!${Defined[bln_hanvarEngaged]}) 	/declare bln_hanvarEngaged 	bool	outer FALSE
	/if (!${Defined[bln_amvEngaged]}) 		/declare bln_amvEngaged	   	bool	outer FALSE
	/if (!${Defined[bln_OMMEngaged]}) 		/declare bln_OMMEngaged    	bool	outer FALSE
	/if (!${Defined[ommCleric_X]}) 			/declare ommCleric_X		int		outer 504
	/if (!${Defined[ommCleric_Y]}) 			/declare ommCleric_Y		int		outer 4772

	/declare eventTarget	   	string outer NULL
	
	/call RegisterRaidEvents Events_OOW_Anguish 15
	/call RegisterPriorityEvents Events_OOW_Anguish_Rapid 3


/return

Sub Events_OOW_Anguish 
	/if ((${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]}) && ${bln_engaged}) /call chkDivineIntervention

	| Ture Events
	/if (${Bool[${SpawnCount[npc Ture]}]} && ${Spawn[npc Ture].Distance3D} < 400) {
		/doevents TureRampOn
		/doevents TureRampOff
	}

	| Jelvan Events
	/if (${Bool[${SpawnCount[npc Jelvan]}]} && ${Spawn[npc Jelvan].Distance3D} < 400) {
		/if (!${bln_jelvanEngaged}) {
			/doevents JelvanStart
		}
	}

	| Hanvar Events
	/if (${Bool[${SpawnCount[npc Hanvar]}]} && ${Spawn[npc Hanvar].Distance3D} < 300) {
		/call chkHanvarEvents
	}


	| Vangl Events
	/if (${Bool[${SpawnCount[npc Vangl]}]} && ${Spawn[npc Vangl].Distance3D} < 300) {
		/call chkAMVEvents
	}

	| OMM Events
	/if (${Bool[${SpawnCount[npc Overlord]}]} && ${Spawn[npc Overlord].Distance3D} < 300) {
		/call chkOMMEvents
	} 

/return 

Sub Events_OOW_Anguish_Rapid 
	| Ture Events
	/if (${Bool[${SpawnCount[npc Ture]}]} && ${Spawn[npc Ture].Distance3D} < 400) {
		/doevents TureRampOn
	}

	| Vangl Events
	/if (${Bool[${SpawnCount[npc Vangl]}]} && ${Spawn[npc Vangl].Distance3D} < 300) {
		/call chkAMVEventsRapid
	}

	| OMM Events
	/if (${Bool[${SpawnCount[npc Overlord]}]} && ${Spawn[npc Overlord].Distance3D} < 300) {
		/call chkOMMEventsRapid
	} 
/return

|----------------------------
| Ture
|----------------------------

Sub Event_TureRampOn
	/echo \ao TURE RAMPAGE: \awengaged?\ag${bln_engaged} \awtanking?\ag${bln_tanking}
	/if (!${bln_engaged} || ${bln_tanking}) /return
	/if (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]} || ${str_class_type.Equal[TANK]}) {
		/echo \arTURE RAMPAGE
		/squelch /attack off 
		/squelch /stick off
		/stand on
		/stick off
		/nav wp tureSafe
		/delay 10
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]} || ${Me.Class.ShortName.Equal[BST]}) {
		/pet hold on
		/pet back off
		/pet hold on
		/pet back off
	}
/return 

Sub Event_TureRampOff 
	/if (!${bln_engaged} || ${bln_tanking}) /return
	/if (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]} || ${str_class_type.Equal[TANK]}) {
		/echo \arTURERAMPAGE
		/call TrueTarget ${int_targetID}
		/nav wp tureDPS
		:ture_run 
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active}) /goto :ture_run
		/squelch /attack on
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]} || ${Me.Class.ShortName.Equal[BST]}) {
		/pet attack
	}
/return 

Sub Event_TureWanton 
	/echo \aw Warning! \arWanton Destruction \awon me!
/return

|----------------------------
| Jelvan
|----------------------------

Sub Event_JelvanStart 
	/echo JelvanStart
	/varset eventTarget Jelvan
	/if (${Me.Class.ShortName.NotEqual[WAR]} && ${Me.Class.ShortName.NotEqual[PAL]} && ${Me.Class.ShortName.NotEqual[SHD]}) {
		/varset bln_jelvanEngaged TRUE
	} else {
		:Tormentor_Checks
		/echo \ayRunning Tormentor Checks!
	|-------------	
	| West, 1 MT	
		/if (${Me.CleanName.Equal[Xiria]}) {
			/if (${SpawnCount[npc Tanthi]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthi].ID}
				/varset bln_jelvanEngaged TRUE
				/stick 10
				/attack on
				/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
				/delay 20
				/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
				/delay 20
				/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
				/squelch /attack on 
				/rs KillMob ${Target.ID}
				/delay 5s 
				/docommand /${announceChannel} DebuffTarget ${Target.ID}
				/docommand /${announceChannel} singsongs
				
				/stick off
				/nav wp jelvan1tank
			}
	| West, 1 RT
		} else /if (${Me.CleanName.Equal[Sadelia]}) {
			/if (${SpawnCount[npc Tanthi]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthi].ID}
				/varset bln_jelvanEngaged TRUE
				/squelch /attack on 
				/stick 20
				/delay 5s 
				/stick off
				/nav wp jelvan1tank  
			}
	|-------------
	| South, 2 MT
		} else /if (${Me.CleanName.Equal[Xiris]}) {
			/if (${SpawnCount[npc Tanthu]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthu].ID}
				/varset bln_jelvanEngaged TRUE
				/stick 10
				/attack on
				/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
				/delay 20
				/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
				/delay 20
				/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
				/squelch /attack on 
				/delay 5s 
				/docommand /${announceChannel} DebuffTarget ${Target.ID}
				/docommand /${announceChannel} singsongs
				
				/stick off
				/nav wp jelvan2tank
			}

	| South, 2 RT		
		} else /if (${Me.CleanName.Equal[Erinyes]}) {
			/if (${SpawnCount[npc Tanthu]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthu].ID}
				/varset bln_jelvanEngaged TRUE
				/squelch /attack on 
				/stick 20
				/delay 5s 
				/stick off
				/nav wp jelvan2tank 
			}

	|-------------
	| East, 3 MT
		} else /if (${Me.CleanName.Equal[Xirea]}) {
			/echo \awchecking for \agTantho! ${SpawnCount[npc Tantho]} 
			/if (${SpawnCount[npc Tantho]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tantho].ID}
				/varset bln_jelvanEngaged TRUE
				/stick 10
				/attack on
				/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
				/delay 20
				/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
				/delay 20
				/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
				/squelch /attack on 
				/delay 5s 
				/docommand /${announceChannel} DebuffTarget ${Target.ID}
				/docommand /${announceChannel} singsongs
				
				/stick off
				/nav wp jelvan3tank
			}

	| East, 3 RT
		} else /if (${Me.CleanName.Equal[Xirin]}) {
			/if (${SpawnCount[npc Tantho]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tantho].ID}
				/varset bln_jelvanEngaged TRUE
				/squelch /attack on 
				/stick 20
				/delay 5s 
				/stick off
				/nav wp jelvan3tank
			}
		}

		| Loop de derp
		/if ((${bln_jelvanEngaged} && !${Target.ID}) || !${bln_jelvanEngaged}) {
			/delay 10
			/goto :Tormentor_Checks
		}
	}
/return


Sub chkJelvanResists
	/if (${Target.Name.Find[Tanthi]} && ${str_useResistTypes.NotEqual[cold|disease|fire]}) /call SetResistTypes cold|disease|fire
	/if (${Target.Name.Find[Tanthu]} && ${str_useResistTypes.NotEqual[fire|magic]}) /call SetResistTypes fire|magic
	/if (${Target.Name.Find[Tantho]} && ${str_useResistTypes.NotEqual[cold|magic]}) /call SetResistTypes cold|magic
/return

Sub chkJelvanSlows
	/if (${str_targetName.Find[Tanthi]}) {
		/call TrueTarget ${Target.ID}
		/if (!${Bool[${Target.Slowed}]}) /call SlowTarget disease
	} else /if (${str_targetName.Find[Tanthu]}) {
		/call TrueTarget ${Target.ID}
		/if (!${Bool[${Target.Slowed}]}) /call SlowTarget magic
	} else /if (${str_targetName.Find[Tantho]}) {
		/call TrueTarget ${Target.ID}
		/if (!${Bool[${Target.Slowed}]}) /call SlowTarget magic
	}
/return

Sub chkJelvanCures
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



|----------------------------
| Warden Hanvar
|----------------------------

Sub chkHanvarEvents
	| Hanvar Events
	/doevents HanvarStart
	/doevents HanvarSetup
	
	/doevents HanvarRetreat
	/doevents HanvarKill

	/if (${bln_hanvarEngaged}) /call chkHanvarCaster
	
/return

Sub Event_HanvarStart
	/echo \ag HANVAR EVENT STARTED \aw@ \ao${Time.Time24}
	/varset eventTarget Hanvar
	/varset bln_hanvarEngaged TRUE
	/doevents flush HanvarStart
/return

Sub Event_HanvarSetup
	/echo Hanvar Setup
	/call SetResistTypes Cold,Poison,Magic
	/declare timer_hanvar_ae   	timer outer 0
	/declare timer_hanvarDebuff timer outer 30s

	/if (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]} || ${str_class_type.Equal[TANK]}) {
		/nav wp hanvarSafe
	} else /if (${str_class_type.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]} ||  ${Me.Class.ShortName.Equal[DRU]}) {
		/nav wp hanvarCaster
	}
	| We need to light up the fucker with DOTs and Cold Nukes
	|/if (${Me.Class.ShortName.Equal[SHM]}) /call setSpell
	/doevents flush HanvarSetup
/return

Sub Event_HanvarRetreat(string _sender, string _line, string _eventParams)
	/echo \arHanvar Retreat Triggered \agStarting \aw${_eventParams}s \ag Timer
	/if (${Target.Name.Find[Hanvar]} || ${Spawn[npc Hanvar].Distance} < 100) {
		/if (${MeleeClasses.Find[${Me.Class.ShortName}]}) {
			/call Disengage
			/nav wp hanvarSafe
		} 
	}
	/doevents flush HanvarKill
	/doevents flush HanvarRetreat
/return

Sub Event_HanvarKill(string _sender, string _line, string _eventParams)
	/echo \arHanvar Kill Triggered \agStarting \aw${_eventParams}s \ag Timer
	/if (${SpawnCount[npc guard]}) /return
	/if (${MeleeClasses.Find[${Me.Class.ShortName}]} ) {
		/call TrueTarget ${Spawn[npc Hanvar].ID}
		/if (${Target.Distance3D} > 20) {
			/nav wp hanvarTank
		}
		/call EngageTarget ${Spawn[npc Hanvar].ID}
	} else /if (${CasterClasses.Find[${Me.Class.ShortName}]}) {
		/call EngageTarget ${Spawn[npc Hanvar].ID}
	}

	:return
	/doevents flush HanvarKill
	/doevents flush HanvarRetreat
/return


Sub chkHanvarCaster
	| Feedback Dispersion Test
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.Buff[Feedback Dispersion].ID} && ${Me.AltAbilityReady[Purify Soul]}) {
		/call TrueTarget ${Me.ID}
		/casting "Purify Soul" alt
	}
/return


|----------------------------
| Arch Magus Vangl
|----------------------------
| Raid Events called consistently, but not inside other sub loops
Sub chkAMVEvents
	/doevents AMVStart
	/doevents AMVKill
/return 

| Events that are constantly called, even inside loops like nuke/debuff/etc - ie: super important
Sub chkAMVEventsRapid
	/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[SHM]}  && !${bln_chChainMember}) /call chkGroupMarkedForDeath 
	/if (${Me.Class.ShortName.Equal[PAL]}) /call chkSelfMarkedForDeath
/return


Sub Event_AMVStart 
	/varset bln_amvEngaged TRUE 
	/varset eventTarget Vangl
	/doevents flush AMVStart
/return


Sub EVENT_AMVKill
	/if (${MeleeClasses.Find[${Me.Class.ShortName}]} ) {
		/call TrueTarget ${Spawn[npc Vangl].ID}
		/if (${Target.Distance3D} > 20) {
			/nav wp amvTank
		}
		/call EngageTarget ${Spawn[npc Vangl].ID}
	} else /if (${CasterClasses.Find[${Me.Class.ShortName}]}) {
		/call EngageTarget ${Spawn[npc Vangl].ID}
	}

	:return
	/doevents flush AMVKill
/return 



|----------------------------
| Overlord Mata'Muram
|----------------------------
Sub Event_OMMStart 
	/varset bln_OMMEngaged TRUE 
	/if (${${Me.Class.ShortName.Equal[ENC]}}) {
		/varset bln_activeCharming TRUE
	}
	/if (${${Me.Class.ShortName.Equal[WAR]}}) {
		/varset bln_offTank TRUE
		/varset int_offtankRadius 200
	}

	/varset eventTarget Overlord
	/doevents flush OMMStart
/return

Sub Event_GazeYou 
	/echo \arGAZE OF MATA MURAM on ME!
	/echo \a-w Attempting Clicking \agMirrored Mask \a-w${Time.Time24}
	/if (!${FindItem[Mirrored Mask].ID} && ${Zone.ShortName.Equal[anguish]}) {
		/shout NO MIRRORED MASK! WE ARE DOOOOMED!
		/dgt FireTL
		/shout Calling for TL!
	} else {
		/if (${Bool[${Me.Casting.ID}]}) /stopcast
		/if (${Bool[${Me.Casting.ID}]}) /stopcast
		:gazeCastWait
		/call Disengage
		/if (${Navigation.Active}) /nav stop
		/if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) {
			/echo \arCasting, waiting to stop, then click!
			/if (${Bool[${Me.Casting.ID}]}) /stopcast
			/delay 1
		}

		/echo \ao Mask in Slot\aw ${FindItem[Mirrored Mask].InvSlot}
		/if (${FindItem[Mirrored Mask].InvSlot} != 3) {
			/echo \ag Swapping Mask to Face!
			/exchange "Mirrored Mask" face
			/delay 5
		}
		/if (${Me.Class.ShortName.Equal[BRD]}) {
			/shout Clicking my Mirrored Mask!
			/echo \ag Attempting Clicking \aoMirrored Mask \ag${Time.Time24}!
			/medley queue "Mirrored Mask" -interrupt
		} else {
			/shout Clicking my Mirrored Mask!
			/echo \ag Attempting Clicking \aoMirrored Mask \ag${Time.Time24}!
			/if (!${Defined[interruptFlag]}) /declare interruptFlag bool outer FALSE	
			/call MQ2Cast "Mirrored Mask" ITEM
		}
		/delay 20
		/call RestoreFaceSlot
		/doevents flush GazeYou
		/if (!${SpawnCount[npc frenzied]}) {
			/call TrueTarget ${Spawn[npc Overlord].ID}
			/call EngageTarget ${Spawn[npc Overlord].ID}
		} 
	}
/return 

Sub Event_GazeBurst
	/echo \arGAZE BURST \aw${Time.Time24}
/return 

Sub Event_GazeDone
/echo \arGAZE DONE \aw${Time.Time24}
/return


Sub chkOMMEvents
	/doevents OMMStart
	/doevents GazeYou
	/doevents GazeBurst
	/doevents GazeDone 
	| TODO: Have Bards keep a charm memmed for OMM?
	/if (${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) {
		/call chkOMMPositioning
	}
	/if (${Me.Class.ShortName.Equal[ENC]}) {
		/call chkOMMCharm
		/call chkDivineIntervention
	} else /if (${Me.Class.ShortName.Equal[CLR]} && !${bln_chChainMember}) {
		/call chkGroupMarkedForDeath 
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/call chkGroupMarkedForDeath 
	} else /if (${Me.Class.ShortName.Equal[PAL]}) {
		/call chkOMMAdds
		/call chkSelfMarkedForDeath
	} else /if (${Me.Class.ShortName.Equal[WAR]}) {	
		/call chkOMMAdds
	}
	|} else /if (${Me.Class.ShortName.Equal[MAG]} || ${Me.Class.ShortName.Equal[WIZ]}) {
		/call chkOMMFrenzied
	|}
/return 

Sub chkOMMPositioning
	/if (${Me.Casting.ID}) /return 
	/if (${Math.Distance[${Me.Y},${Me.X}:${ommCleric_Y},${ommCleric_X}]} > 20) {
		/warp wp ommCleric
	}
/return

Sub chkOMMLasherPositions
	/if (!${SpawnCount[npc lasher]}) /return
	/if (${NearestSpawn[npc lasher].Distance3D} < 30) {
		/face fast lasher
		/if (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]}) /dt ${Me.Name} KillMob ${NearestSpawn[npc lasher].ID} ${NearestSpawn[npc lasher].Name} 
	} else {
		/if (${str_class_type.Equal[MELEE_END]} || ${str_class_type.Equal[MELEE_MANA]}) /dt ${Me.Name} KillMob ${NearestSpawn[npc Overlord].ID} ${NearestSpawn[npc Overlord].Name} 
	}
/return


Sub chkOMMCharm 
	/echo \aochkOMMCharm \awPet? \ag${Me.Pet.ID}
	/if (!${Me.Pet.ID}) {
		/if (${bln_hasCharmedPet}) /shout LOST CONTROL OF PET!
		/if (${FindItem[=Mindreaver's Vest of Coercion].ID} && ${FindItem[=Mindreaver's Vest of Coercion].TimerReady}==0) {
			/call MQ2Cast "Mindreaver's Vest of Coercion" item 2s
		}
		/varset bln_hasCharmedPet FALSE
		/if (${SpawnCount[npc Coerced]}) {
			| Eurydice (Charm 1)
			/if (${Me.Name.Equal[Eurydice]}) {
				:Charm_1
				/if (!${timer_charmAnnounce}) /echo \agCoerced_Lieutenant_000 Charm Attempt \ayDist:\ar${Spawn[npc Coerced_Lieutenant_000].Distance3D} \ayLOS:${Spawn[npc Coerced_Lieutenant000].LineOfSight}
				/if (${Spawn[npc Coerced_Lieutenant_000].Distance3D} < 200 && ${Spawn[npc Coerced_Lieutenant_000].LineOfSight}) {
					/call TrueTarget ${Spawn[npc Coerced_Lieutenant_000].ID}
					/if (${Target.Charmed.ID}) /goto :Charm_2
					/if (!${timer_charmAnnounce}) {
						/shout Attempting Charm! ${Target.Name} (${Target.ID}) d:${Target.Distance3D}
						/echo \ar CHARMING ${Target.Name} (${Target.ID}) d:${Target.Distance3D}
						/varset timer_charmAnnounce 10s
					}
					/call CharmNPC ${Target.ID} TRUE TRUE ${Spawn[npc Overlord].ID}
					/goto :petattack_check
				}
			}

			| Erisa (Charm 2)
			/if (${Me.Name.Equal[Erisa]}) {
				:Charm_2
				/if (!${timer_charmAnnounce}) /echo \agCoerced_Lieutenant000 Charm Attempt \ayDist:\ar${Spawn[npc Coerced_Lieutenant_000].Distance3D} \ayLOS:${Spawn[npc Coerced_Lieutenant000].LineOfSight}
				/if (${Spawn[npc Coerced_Lieutenant000].Distance3D} < 200 && ${Spawn[npc Coerced_Lieutenant000].LineOfSight}) {
					/call TrueTarget ${Spawn[npc Coerced_Lieutenant000].ID}
					/if (${Target.Charmed.ID}) /goto :Charm_1
					/if (!${timer_charmAnnounce}) {
						/shout Attempting Charm! ${Target.Name} (${Target.ID}) d:${Target.Distance3D}
						/echo \ar CHARMING ${Target.Name} (${Target.ID}) d:${Target.Distance3D}
						/varset timer_charmAnnounce 10s
					}
					/call CharmNPC ${Target.ID} TRUE TRUE ${Spawn[npc Overlord].ID}
					/goto :petattack_check
				}
				/call CharmNPC ${charmTgtID} TRUE TRUE ${Spawn[npc Overlord].ID}
			} else {
				/echo \ao Waiting \awfor \ao${charmTgtName}\ag(\ao${charmTgtID}\ag) to be in \aoDist:${Spawn[id ${charmTgtID}].Distance3D} \aw< 200 & \aoLOS:${Spawn[id ${charmTgtID}].LineOfSight}
			}
		}
	} else {
		/if (!${bln_hasCharmedPet}) /varset bln_hasCharmedPet TRUE
		:petattack_check
		/if (!${bln_hasCharmedPet}) /varset bln_hasCharmedPet TRUE
		/if (${Me.Pet.ID} && ${Me.Pet.Target.ID} != ${Spawn[npc Overlord].ID}) {
			/echo \agSiccing pet!
			/call TrueTarget ${Spawn[npc Overlord].ID}
			/pet attack
			/if (!${Me.Pet.Buff[Speed of Salik]}) {
				/echo \agBuffing Pet!
				/target id ${Me.Pet.ID}
				/call chkSpellMemorized "Speed of Salik" TRUE gem8
				/call MQ2Cast "Speed of Salik" 
			} 
		}
	}
:return
/echo \agReturning out of chkOMMCharm
/return

| Adds need to be AE DDd
Sub chkOMMFrenzied

	/if (${SpawnCount[npc lasher]}) {
		/if (${NearestSpawn[1, npc lasher].Distance3D} < 100) {
			/if ((${Me.Class.ShortName.Equal[WIZ]} || ${Me.Class.ShortName.Equal[MAG]})) {
				/varset AENUKE_Active TRUE
				|/echo \arAENUKING THE FUCKERS
				/call TrueTarget ${NearestSpawn[1, npc lasher].ID}
				/call chkAENuke
			} else /if (${Select[${Me.Class.ShortName},${MeleeClasses}]}) {
				/call TrueTarget ${NearestSpawn[1, npc lasher].ID}
				/call EngageTarget ${Target.ID}
			} else {
				/declare lx int local ${NearestSpawn[1, npc lasher].X}
				/declare ly int local ${NearestSpawn[1, npc lasher].Y}
				/declare lz int local ${NearestSpawn[1, npc lasher].Z}
				/face loc ${ly},${lx},${lz} fast
			}
		}
	} else {
		/if ((${Me.Class.ShortName.Equal[WIZ]} || ${Me.Class.ShortName.Equal[MAG]})) {
			/varset AENUKE_Active	FALSE
		}
		/if (${bln_engaged}) /call EngageTarget ${Spawn[npc Overlord].ID}
	}
/return

Sub chkOMMAdds 
	|
	/declare i int local 
	/if (!${SpawnCount[npc Coerced]} && !${SpawnCount[npc lasher]}) /return

	| Lieutenants
	/for i 1 to ${SpawnCount[npc Coerced radius 160]}
		/call TrueTarget ${NearestSpawn[${i}, npc Coerced].ID}
		/if (${Me.TargetOfTarget.Class.ShortName.Equal[WAR]} || ${Me.TargetOfTarget.Class.ShortName.Equal[PAL]} || ${Me.TargetOfTarget.Class.ShortName.Equal[SHD]}) /goto :nextTarget
		/if (!${Target.Charmed.ID}) /call aggroByID ${Target.ID}
		/squelch /target clear
	:nextTarget	
	| coerced test
	/next i

	| Lashers
	/for i 1 to ${SpawnCount[npc lasher radius 200]}
		/call TrueTarget ${NearestSpawn[${i}, npc lasher].ID}
		/if (${Me.TargetOfTarget.Class.ShortName.Equal[WAR]} || ${Me.TargetOfTarget.Class.ShortName.Equal[PAL]} || ${Me.TargetOfTarget.Class.ShortName.Equal[SHD]}) /goto :nextTarget2
		/if ((${Target.Distance3D} < 200)) {
			/call aggroByID ${Target.ID}
			/if (${Spawn[pc Xanshia].Distance3D} > 50) {
				/nav wp ommCleric
				/delay 20
			}
		} else {
			/squelch /target clear
		}
	:nextTarget2	
	|lasher test
	/next i
/return



|----------------------------
| Common
|----------------------------

Sub EVENT_MarkOfDeath
	/echo \arMarked for Death! \aw${Time.Time24}
	/if (${bln_tanking}) /return
	/if (!${bln_amvEngaged} && ${eventTarget.Equal[Vangl]}) /varset bln_amvEngaged TRUE
	/if (!${bln_OMMEngaged} && ${eventTarget.Equal[Overlord]}) /varset bln_OMMEngaged TRUE
	/doevents flush MarkOfDeath
	/call chkMarkedForDeathCureDist 
/return


Sub chkGroupMarkedForDeath
	/if (${Me.Class.ShortName.Equal[CLR]} && (${bln_chChainMember} || ${bln_focusHealMT})) /return
 
	/declare i 				int local 0
	/declare haveMark		int local 0
	/declare gcounters[6]		int local 0
	/declare foundMark		bool local FALSE
	/declare tankIndex		int local 10
	/declare cureWaitCount 	int local 0
	/declare gm 			int local 1

	:CheckGroup
	/varset haveMark 	0
	/varset foundMark 	FALSE
	
	| Remember, Group.Member needs 0-5, whereas the array needs 1-6
	/for i 0 to ${Group}
		/varcalc arrLen ${i}+1
		/if (${Group.Member[${i}].CleanName.Equal[${str_MTName}]}) /varset tankIndex ${i}
		/dquery ${Group.Member[${gm}]} -q "Me.Buff[Mark of Death].ID"
		/delay 15 ${DanNet.Query.Received}
		/if (${Bool[${DanNet.Q}]}) {
			/echo \ar${Group.Member[${gm}]} \awreply:${Bool[${DanNet.Q}]}
			/echo \ar MARK OF DEATH FOUND: \ao${Group.Member[${gm}]} \ayDist:\ao${Group.Member[${gm}].Distance3D}
			/call getCureCounters ${Group.Member[${gm}]} disease
			/varset  foundMark TRUE
			/varcalc haveMark ${haveMark}+1
			/varset gcount[${i}] ${Int[${Macro.Return}]}
		} 
	/next i
	
	| No Marks found? return
	/if (!${foundMark})	/goto :return

	/echo \ar${haveMark} \aoInstances of Mark of Death found!
	/if (${Me.Silenced}) {
		/shout SILENCED! I cannot cure Mark of Death!
	}
	| If Marks are less than 3, or I am a paladin then single target cure them
	:SingleCure
	/if (${Me.Class.ShortName.Equal[PAL]} || ${haveMark} < 3) /goto :CureByCounters
	
	| If I am a cleric or shaman, cast group cure, and then recheck counters
	:GroupCure 
	/echo \awGroupCure: \ag${str_group_cure_disease} \ay${Me.SpellReady[${str_group_cure_disease}]} \aw(\ag${Me.PctMana}) \ay${haveMark}>2
	/if ((${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[SHM]}) && ${Me.PctMana} > 10 && ${haveMark} > 2) {
		:GroupCureWait 
		/varcalc cureWaitCount ${cureWaitCount}+1
		/if (!${Me.SpellReady[${str_group_cure_disease}]}) {
			/delay 5
			/if (${cureWaitCount} > 10) {
				/echo \arGroupCure failed due to wait, trying by counters
				/goto :CureByCounters
			} else {
				/echo \arGroupCure waiting for ${str_heal_group} ${Me.SpellReady[${str_group_cure_disease}]}
				/goto :GroupCureWait
			}
		} else {
			/echo \agGroupCuring with ${str_group_cure_disease}
			/g Group Heal incoming, get close!
			/delay 30
			/for i 1 to 5
				/if ((${Group.Member[${i}].ID} != ${int_MTID}) && (${Group.Member[${i}].Name.NotEqual[${str_MTName}]}) && ${Select[${Group.Member[${i}].Class.ShortName},${MeleeClasses}]} && ${Group.Member[${i}].Distance} > ${Spell[${str_heal_group}].AERange} && ${Group.Member[${i}].Present}) {
					/dt  ${Group.Member[${i}]} BackOff
					/dex ${Group.Member[${i}]} /warp loc ${Me.Y} ${Me.X} ${Me.Z} 
					/dt  ${Group.Member[${i}]} BackOff
				}
			/next i
			/if (${Me.Casting.ID}) /call Interrupt
			/call cureGroup disease ${groupMaxDisease}
			/goto :CheckGroup
		}
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/echo \agGroupCuring with ${str_cure_disease}
		/g Group Heal incoming, get close!
		/delay 30
		/for i 1 to 5
			/if ((${Group.Member[${i}].ID} != ${int_MTID}) && (${Group.Member[${i}].Name.NotEqual[${str_MTName}]}) && ${Select[${Group.Member[${i}].Class.ShortName},${MeleeClasses}]} && ${Group.Member[${i}].Distance} > ${Spell[${str_heal_group}].AERange} && ${Group.Member[${i}].Present}) {
				/dt  ${Group.Member[${i}]} BackOff
				/dex ${Group.Member[${i}]} /warp loc ${Me.Y} ${Me.X} ${Me.Z} 
				/dt  ${Group.Member[${i}]} BackOff
			}
		/next i
		/if (${Me.Casting.ID}) /call Interrupt
		/call MQ2Cast "${str_cure_disease}" gem7 8s
		/varset secondCheck TRUE
		/goto :CheckGroup
		
	}

	| All else fails so cure anyone with a disease counter singly
	:CureByCounters
	/echo \awCureByCounters: \ag${str_cure_disease} \awReady?\ay${Me.SpellReady[${str_cure_disease}]} \aw(\ag${Me.PctMana}\aw) \awHaveMark:\ay${haveMark}< 2
	/declare diseaseCounters int local 0
	/for i 0 to ${Group}
		/varcalc arrLen ${i}+1
		/call getCureCounters ${Group.Member[${i}]} disease
		/varset diseaseCounters ${Int[${Macro.Return}]}
		/if (${Me.Casting.ID}) /call Interrupt
		/if (${gcount[${i}]} > 0) {
			/if (${Group.Member[${gm}].ID} != ${int_MTID} && ${Select[${Group.Member[${gm}].Class.ShortName},${MeleeClasses}]} && ${Group.Member[${gm}].Distance} >= 100  && ${Group.Member[${i}].Present}) {
				/dt  ${Group.Member[${gm}]} BackOff
				/dex ${Group.Member[${gm}]} /warp loc ${Me.Y} ${Me.X} ${Me.Z} 
				/dt  ${Group.Member[${gm}]} BackOff
				/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${gcount[${i}]} TRUE
			} else /if (${Group.Member[${gm}].ID} == ${int_MTID}) {
				/echo \ar MARK OF DEATH FOUND on TANK \aoDist:\ay${Group.Member[${gm}].Distance} \aoRange:\ay${Spell[${str_cure_disease}].MyRange}
				/if (${Group.Member[${gm}].Distance} > ${Spell[${str_cure_disease}].MyRange}) {
					/ar FUCK ${Group.Member[${gm}]} is out of cure Range! ${Group.Member[${gm}].Distance} > ${Spell[${str_cure_disease}].MyRange}
					/warp wp ommCleric2
				}
				/call getCureCounters ${Group.Member[${gm}].Name} disease
				/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${gcount[${i}]} TRUE
				/warp wp ommCleric
			} else {
				/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${gcount[${i}]} TRUE
			}
			/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${diseaseCounters} TRUE
		}
	/next i
	/goto :CheckGroup

	:CommandAttack
	/if (${Spawn[npc ${eventTarget}].PctHPs} <= 98) {
		/for i 1 to 5
				/if ((${Group.Member[${i}].ID} != ${int_MTID}) && (${Group.Member[${i}].Name.NotEqual[${str_MTName}]}) && ${Select[${Group.Member[${i}].Class.ShortName},${MeleeClasses}]}) {
					/dex ${Group.Member[${i}]} /target ${eventTarget}
					/dex ${Group.Member[${i}]} /nav spawn ${eventTarget} 
					/dt  ${Group.Member[${i}]} KillMob ${Spawn[npc ${eventTarget}].ID} "${Spawn[npc ${eventTarget}].Name}" ${Time.Time24} 
				}
			/next i
		/gsay KillMob ${Spawn[npc ${eventTarget}].ID} "${Spawn[npc ${eventTarget}].Name}" ${Time.Time24}
		/gsay Restick!
		/rsay Returning My Group to the Fight! ${eventTarget}
	}
	
:Return
/return


Sub chkSelfMarkedForDeath
	/if (${Me.Buff[Mark of Death].ID}) /call handleCureRequest ${Me.Name} ${Me.ID} disease 14 TRUE
/return

Sub chkMarkedForDeathCureDist
	/echo \ar MARK OF DEATH \aw=>curer \ag${str_myCurer} \aw@ \ag${Spawn[pc ${str_myCurer}].Distance3D}
	/if (${Spawn[pc ${str_myCurer}].Distance3D} > 50) {
		/echo \awMoving to Curer!
		/nav spawn ${str_myCurer}
		/delay 10
	} 
	
	:markLoop
	/delay 10
	/doevents
	/if (${Me.Buff[Mark of Death].ID}) {
		/call Disengage
		/delay 10
		/echo \arHaveMark, waiting for Cure!
		/if (${Spawn[pc ${str_myCurer}].Distance3D} < 50) /nav stop 
		/goto :markLoop
	} 

	/if (${Spawn[pc ${str_myCurer}].Distance3D} > 100) {
		:get_curer
		/call getGroupCurer ${cureType}
		/varset str_myCurer ${Macro.Return}
		/if (${str_myCurer.Equal[NOTFOUND]}) {
			/echo \ar I HAVE NO CURER!
			/goto :return
		}
		/echo \ar MARK OF DEATH \aw=>curer \ag${str_myCurer} \aw@ \ag${Spawn[pc ${str_myCurer}].Distance3D}
		/echo \awMoving to Curer!
		
		/nav spawn ${str_myCurer}
		:run_loop
			/delay 10
			/call RunAllEvents
			/if  (${Spawn[pc ${str_myCurer}].Distance3D} > 100) /goto :RunLoop
		/nav stop
	} else {
		/if (${Navigation.Active}) /nav stop
	}
	
	/call RunAllEvents

	:mark_loop
	/if (${Me.Buff[Mark of Death].ID}) {
		/echo \arMARK OF DEATH \awCounters:\ao${Debuff.Diseased}) \aw${Time.Time24}
		/delay 20
		/goto :markLoop
	} else {
		/if (!${Target.Name.Find[${eventTarget}]}) {
			/call TrueTarget ${Spawn[npc ${eventTarget}].ID}
		}
		/call EngageTarget ${Spawn[npc ${eventTarget}].ID}
	}

	
	:return
/return 