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
#EVENT TureWanton		"#*#dissolves into a landscape#*#"

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
#EVENT AMVRetreat		"#1# tells the raid#*#'AMVRetreat #2#'"

|   OMM
#EVENT OMMStart			"#*#OMMStart#*#"
#EVENT GazeYou			"#*#feel a gaze of deadly power#*#" 
#EVENT GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#EVENT GazeDone			"#*#Mata Muram's spell has been reflected#*#"
#EVENT OMMWarCry		"#*#OMMWarCry#*#"


Sub InitEvents_OOW_Anguish
	/echo \ag Initializing Anguish
	/declare	bln_jelvanEngaged		bool	outer FALSE
	/declare	bln_hanvarEngaged		bool	outer FALSE
	/declare	bln_amvEngaged			bool	outer FALSE
	/declare	bln_OMMEngaged			bool	outer FALSE
	/declare	bln_groupHasShaman		bool  	outer FALSE
	/declare i int local 0
	/for i 1 to ${Group.GroupSize}
		/if (${Group.Member[${i}].Class.ShortName.Equal[SHM]}) /varset bln_groupHasShaman TRUE
	/next i
	/declare	ommCleric_X				int		outer 504
	/declare	ommCleric_Y				int		outer 4772
	/declare	eventTarget	   			string	outer NULL
	/declare	timer_omm_charm 		timer	outer 2s

	/declare	arr_bersekerList[10]	string	outer NULL
	/varset		arr_bersekerList[1]		Ophidia 
	/varset 	arr_bersekerList[2]		Ophelia
	/varset		arr_bersekerList[3]		Ophemia
	/varset 	arr_bersekerList[4]		Ophenia
	/varset 	arr_bersekerList[5]		Opheoia
	/varset 	arr_bersekerList[6]		Ophexia
	/varset 	arr_bersekerList[7]		Opheyia
	/varset 	arr_bersekerList[8]		Ophezia
	/varset 	arr_bersekerList[9] 	Ophesia
	/varset 	arr_bersekerList[10]	Opheria

	/call RegisterBackgroundEvents 	Events_OOW_Anguish 1
	/call RegisterRapidEvents 		Events_OOW_Mark 1


/return

Sub Events_OOW_Anguish 
	/if (${Zone.ShortName.NotEqual[anguish]} && ${Zone.ShortName.NotEqual[arena]} ) /return
	/if (${Select[${Me.Class.ShortName},WAR,SHD,PAL]} && ${bln_engaged}) /call chkDivineIntervention

	| Ture Events
	/if (${Bool[${SpawnCount[npc Ture]}]} && ${Spawn[npc Ture].Distance3D} < 400) {
		/call chkTureEvents
	}

	| Jelvan Events
	/if (${Bool[${SpawnCount[npc Jelvan]}]} && ${Spawn[npc Jelvan].Distance3D} < 400) {
		/call chkJelvanEvents
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
	/if ((${Bool[${SpawnCount[npc Overlord]}]} && ${Spawn[npc Overlord].Distance3D} < 400) || ${Zone.ShortName.Equal[arena]}) {
		/call chkOMMEvents
	} 

/return 

Sub Events_OOW_Mark
	/if (${Zone.ShortName.NotEqual[anguish]}) /return
	/doevents GazeYou
	/doevents MarkOfDeath
	|** /if (${Select[${Me.Class.ShortName},SHM,CLR,DRU]}) {
		|/doevents CheckGroupMarkOfDeath
		/call chkGroupMarkedForDeath
	} **|
/return

|----------------------------
| Ture
|----------------------------

Sub chkTureEvents
	/doevents TureRampOn
	/doevents TureRampOff
/return 

Sub Event_TureRampOn
	/doevents flush TureRampOn
	/echo \ao TURE RAMPAGE: \awengaged?\ag${bln_engaged} \awtanking?\ag${bln_tanking}
	/if (${bln_tanking}) /return
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/echo \arTURE RAMPAGE
		/call Disengage
		/squelch /attack off 
		/squelch /stick off
		/stand on
		/stick off
		/nav wp tureSafe
		/delay 10
		/call chkNeedCure
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]} || ${Me.Class.ShortName.Equal[BST]}) {
		/pet hold on
		/pet back off
		/pet hold on
		/pet back off
	}
	
/return 

Sub Event_TureRampOff 
	/if (${bln_tanking}) /return
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/echo \arTURERAMPAGE
		/call TrueTarget ${int_targetID}
		/nav wp tureDPS
		:ture_run 
		/if (${Target.Distance} < 35) /nav stop
		/if (${Navigation.Active}) /goto :ture_run
		/call EngageTarget ${int_targetID}
		/squelch /attack on
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]} || ${Me.Class.ShortName.Equal[BST]}) {
		/pet attack
	}
	/doevents flush TureRampOn
/return 

Sub Event_TureWanton 
	/echo \aw Warning! \arWanton Destruction \awon me!
	/call chkNeedCure
	/doevents flush TureWanton
/return

|----------------------------
| Jelvan
|----------------------------

sub chkJelvanEvents
	| Jelvan Events
	/if (!${bln_jelvanEngaged}) {
			/doevents JelvanStart
	} else {
		/if (${Me.Class.ShortName.Equal[PRIEST]} || ${Me.Class.ShortName.Equal[PAL]}) 		/call chkJelvanCures
		/if (${Me.Class.ShortName.Equal[SHM]} || ${str_classArchetype.Equal[CASTER]}) 	/call chkJelvanResists
		/if (${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[ENC]}) 		/call chkJelvanSlows
	} 
/return

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
				|/stick 10
				/attack on
				/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
				/delay 20
				/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
				/delay 20
				/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
				/squelch /attack on 
				|/rs KillMob ${Target.ID}
				|/delay 5s 
				|/docommand /${announceChannel} DebuffTarget ${Target.ID}
				|/docommand /${announceChannel} singsongs
				
				/stick off
				/nav wp jelvan1tank
				:nav11
				/if (${Navigation.Active}) /goto :nav11
				/squelch /attack on
				/delay 10
				/stick 10 front
			}
	| West, 1 RT
		} else /if (${Me.CleanName.Equal[Sadelia]}) {
			/if (${SpawnCount[npc Tanthi]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthi].ID}
				/varset bln_jelvanEngaged TRUE
				|/stick 20
				|/delay 5s 
				/casting "Serene Command"
				/stick off
				/nav wp jelvan1tank
				:nav12
				/if (${Navigation.Active}) /goto :nav12
				/squelch /attack on
				/delay 10
				/stick 10 front
			}
	|-------------
	| South, 2 MT
		} else /if (${Me.CleanName.Equal[Xiris]}) {
			/if (${SpawnCount[npc Tanthu]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthu].ID}
				/varset bln_jelvanEngaged TRUE
				|/stick 10
				/attack on
				/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
				/delay 20
				/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
				/delay 20
				/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
				 
				|/delay 5s 
				|/docommand /${announceChannel} DebuffTarget ${Target.ID}
				|/docommand /${announceChannel} singsongs
				
				/stick off
				/nav wp jelvan2tank
				:nav21
				/if (${Navigation.Active}) /goto :nav21
				/face fast 
				/squelch /attack on
				/delay 10
				/stick 10 front
			}

	| South, 2 RT		
		} else /if (${Me.CleanName.Equal[Ertres]}) {
			/if (${SpawnCount[npc Tanthu]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthu].ID}
				/varset bln_jelvanEngaged TRUE
				|/stick off
				|/squelch /attack on 
				|/stick 20
				|/delay 5s 
				/ranged
				/stick off
				/nav wp jelvan2tank
				:nav22
				/if (${Navigation.Active}) /goto :nav22
				/face fast 
				/squelch /attack on
				/delay 10
				/stick 10 front
			}

	|-------------
	| East, 3 MT
		} else /if (${Me.CleanName.Equal[Xirea]}) {
			/if (${SpawnCount[npc Tantho]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tantho].ID}
				/varset bln_jelvanEngaged TRUE
				|/stick 10
				/attack on
				/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
				/delay 20
				/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
				/delay 20
				/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
				/squelch /attack on 
				/delay 5s 
				|/docommand /${announceChannel} DebuffTarget ${Target.ID}
				|/docommand /${announceChannel} singsongs
				
				/stick off
				/nav wp jelvan3tank
				:nav31
				/if (${Navigation.Active}) /goto :nav31
				/face fast 
				/squelch /attack on
				/delay 10
				/stick 10 front
			}

	| East, 3 RT
		} else /if (${Me.CleanName.Equal[Xirin]}) {
			/if (${SpawnCount[npc Tantho]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tantho].ID}
				/varset bln_jelvanEngaged TRUE
				/stick off
				/squelch /attack on 
				|/stick 20
				|/delay 5s 
				
				/stick off
				/nav wp jelvan3tank
				:nav32
				/if (${Navigation.Active}) /goto :nav32
				/face fast 
				/squelch /attack on
				/delay 10
				/stick 10 front
			}
		}

		| Loop de derp
		/if ((${bln_jelvanEngaged} && !${Target.ID}) || !${bln_jelvanEngaged}) {
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
			/call healMAINTANK
			/call cureGroup disease 30
		} else /if (${Me.Class.ShortName.Equal[CLR]}) {
			/call healMAINTANK
			/call cureSelf disease 30
		} else /if (${Me.Class.ShortName.Equal[DRU]}) {
			/call healMAINTANK
			/call cureSelf disease 30
		} else /if (${Me.Class.ShortName.Equal[PAL]}) {
			/call cureSelf disease 30
		}		
	} else /if (${Me.Buff[Relinquish Spirit].ID}) {
		/if (${Me.Class.ShortName.Equal[SHM]}) {
			/call healMAINTANK
			/call cureGroup curse 24
		} else /if (${Me.Class.ShortName.Equal[CLR]}) {
			/call healMAINTANK
			/call cureSelf curse 24
		} else /if (${Me.Class.ShortName.Equal[DRU]}) {
			/call healMAINTANK
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

	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/nav wp hanvarSafe
	} else /if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]} ||  ${Me.Class.ShortName.Equal[DRU]}) {
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
Sub chkAMVEvents
	| AMV Events
	/doevents AMVStart
	/doevents AMVKill
	
	/if (${bln_amvEngaged} || ${Spawn[npc Vangl].Distance3D} < 200) {
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[SHM]}  && !${bln_chChainMember}) /call SQLCureMark 
	}

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


Sub EVENT_AMVRetreat
	/doevents flush AMVRetreat
	/call chkMarkedForDeathCureDist 
/return





|----------------------------
| Overlord Mata'Muram
|----------------------------
Sub Event_OMMStart 
	/doevents flush OMMStart
	/varset bln_OMMEngaged TRUE 
	/if (${Select[${Me.Class.ShortName},ENC]}) {
		/varset bln_activeCharming TRUE
	}
	/if (${Select[${Me.Class.ShortName},WAR,PAL]}) {
		/varset bln_offTank TRUE
		/varset int_offtankRadius 200
	}

	/varset eventTarget Overlord
	
/return

Sub Event_GazeYou 
	/doevents flush GazeYou
	/call Interrupt
	/echo \arGAZE OF MATA MURAM on ME!
	/echo \a-w Attempting Clicking \agMirrored Mask \a-w${Time.Time24}
	/if (!${FindItem[Mirrored Mask].ID} && ${Zone.ShortName.Equal[anguish]}) {
		/shout NO MIRRORED MASK! WE ARE DOOOOMED!
		/dgt FireTL
		/shout Calling for TL!
	} else {
		:gazeCastWait
		/call Disengage
		/if (${Navigation.Active}) /nav stop
		/if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) {
			/echo \arCasting, waiting to stop, then click!
			/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		}

		/echo \ao Mask in Slot\aw ${FindItem[Mirrored Mask].InvSlot}
		/if (${FindItem[Mirrored Mask].InvSlot} != 3) {
			/echo \ag Swapping Mask to Face!
			/exchange "Mirrored Mask" face
			/delay 10
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
		/call LoadFaceSlot
		
		| /if (!${SpawnCount[npc frenzied]}) {
		| 	/call TrueTarget ${Spawn[npc Overlord].ID}
		| 	/call EngageTarget ${Spawn[npc Overlord].ID}
		| } 
	}
/return 

Sub Event_GazeBurst
	/doevents flush GazeBurst
	/echo \arGAZE BURST \aw${Time.Time24}
/return 

Sub Event_GazeDone
	/doevents flush GazeDone
	/echo \arGAZE DONE \aw${Time.Time24}
/return


Sub chkOMMEvents
	/doevents OMMStart
	/doevents GazeYou
	/doevents GazeBurst
	/doevents GazeDone 
	/call SQLCheckMark

	/if (${Me.Class.ShortName.Equal[ENC]}) {
		/doevents OMMWarCry
		/call chkOMMCharm
		/call chkDivineIntervention
		/return
	} else /if (${Me.Class.ShortName.Equal[CLR]} && !${bln_chChainMember}) {
		/call SQLCureMark 
		/return
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/call SQLCureMark 
		/return
	} else /if (${Me.Class.ShortName.Equal[PAL]}) {
		/call chkOMMAdds
		/return
	} else /if (${Me.Class.ShortName.Equal[WAR]}) {	
		/call chkOMMAdds
		/return
	}
	/if (${Select[${str_classArchetype},MELEE_MANA,MELEE_END]}) /call chkOMMFrenzied
/return 



Sub chkOMMCharm 
	/if (${timer_omm_charm}) /return
	/if (!${Pet.ID}) {
		/echo \aochkOMMCharm \awPet? \agID:${Pet.ID} \awCoerced? \ag${SpawnCount[npc Coerced]}
		/if (${bln_hasCharmedPet}) /shout LOST CONTROL OF PET!
		/varset bln_hasCharmedPet FALSE
		/if (${FindItem[=Mindreaver's Vest of Coercion].ID} && ${FindItem[=Mindreaver's Vest of Coercion].TimerReady}==0) {
			/call MQ2Cast "Mindreaver's Vest of Coercion" item 2s
		}
		
		/if (${SpawnCount[npc Coerced]}) {
			| Eurydice (Charm 1)
			/if (${Me.Name.Equal[Eurydice]}) {
				/echo \aoCoerced_Lieutenant_000 \awCharm Attempt
				:Charm_1
				/if (!${timer_charmAnnounce}) /echo \agCoerced_Lieutenant_000 Charm Attempt \ayDist:\ar${Spawn[npc Coerced_Lieutenant_000].Distance3D} \ayLOS:${Spawn[npc Coerced_Lieutenant000].LineOfSight}
				/if (${Spawn[npc Coerced_Lieutenant_000].Distance3D} < 200 && ${Spawn[npc Coerced_Lieutenant_000].LineOfSight}) {
					/call TrueTarget ${Spawn[npc Coerced_Lieutenant_000].ID}
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
				/echo \aoCoerced_Lieutenant000 \awCharm Attempt
				:Charm_2
				/if (!${timer_charmAnnounce}) /echo \agCoerced_Lieutenant000 Charm Attempt \ayDist:\ar${Spawn[npc Coerced_Lieutenant000].Distance3D} \ayLOS:${Spawn[npc Coerced_Lieutenant000].LineOfSight}
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
			}
		}
	} else {
		:petattack_check
		/if (!${bln_hasCharmedPet}) /varset bln_hasCharmedPet TRUE
		/if (${Pet.ID} && (${Pet.Target.ID} != ${Spawn[npc Overlord].ID})) {
			/echo \agSiccing pet!
			/call TrueTarget ${Spawn[npc Overlord].ID}
			/pet attack
			/if (!${Me.Pet.Buff[Speed of Salik]}) {
				/echo \agBuffing Pet!
				/target id ${Pet.ID}
				/call chkSpellMemorized "Speed of Salik" TRUE gem8
				/call MQ2Cast "Speed of Salik" 
			} 
		}
	}
:return
|/echo \agReturning out of chkOMMCharm
/varset timer_omm_charm 2s
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
		|/if (${bln_engaged}) /call EngageTarget ${Spawn[npc Overlord].ID}
	}
/return

Sub chkOMMAdds 
	|
	/declare i int local 
	/if (!${SpawnCount[npc Coerced]} && !${SpawnCount[npc lasher]}) /return

	| Lieutenants
	/for i 1 to ${SpawnCount[npc Coerced radius 140]}
		/call TrueTarget ${NearestSpawn[${i}, npc Coerced].ID}
		/if (${Me.TargetOfTarget.Class.ShortName.Equal[WAR]} || ${Me.TargetOfTarget.Class.ShortName.Equal[PAL]} || ${Me.TargetOfTarget.Class.ShortName.Equal[SHD]}) /goto :nextTarget
		/if (!${Target.Charmed.ID}) /call aggroByID ${Target.ID}
		/nav wp ommCleric
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

Sub EVENT_OMMWarCry
	/doevents flush OMMWarCry 
	/if (${Me.Class.ShortName.NotEqual[ENC]}) /return 
	/if (!${Pet.ID}) /return
	/if (${Pet.Buff[Ancient: Cry of Chaos]}) /return
	/shout Calling OMM War Cry on my Pet!
	/call RequestBerserkerWarCry ${Pet.ID}
/return


Sub RequestBerserkerWarCry(int _targetID)
	/echo \ag WARCRY CALLED! \ao${_targetID} \aw ${Time.Time24}
	/declare bStatus 	string	local FALSE
	/declare berTest 	string  local NULL
	/declare berName 	string 	local NULL

	/declare i			int 	local 1
	/declare foundCount int 	local 0
	/declare sz  		int     local 0
	/declare t   		int     local ${arr_bersekerList.Size}
	/if (${Me.CleanName.Equal[Erisa]}) {
		/varset  sz 1
		/varcalc t  ${t}-1
	}
	/for i 1 to ${t}
		/if (${Me.CleanName.Equal[Erisa]}) /varcalc i ${i}+${sz}
		/varset berTest ${arr_bersekerList[${i}]}
		/if (!${SpawnCount[pc ${berTest}]}) /goto :next_berserker
		/dquery ${berTest} -q "Me.CombatAbilityReady[Ancient: Cry of Chaos]" -o bStatus
		/delay 20 ${DanNet.Query.Received}
		/echo \aw ${berTest} Cry? ${If[${bStatus},\ag${bStatus},\ar${bStatus}]}
		/if (${bStatus.Equal[TRUE]}) {
			/varcalc foundCount ${foundCount}+1
			/varset berName ${berTest}
		}
		/if (${foundCount}) /goto :CastCry
	/next i

	:CastCry
	/echo \aoAsking \ag${berName}\aw to cast \agAncient: Cry of Chaos\aw on ${_targetID}
	/dt ${berName} FireWarCry ${_targetID}
/return


|----------------------------
| Common
|----------------------------

Sub EVENT_MarkOfDeath
	/echo \aoMarked for Death!
	/1 Marked for Death!
	/doevents flush MarkOfDeath
	/if (!${bln_amvEngaged} && ${eventTarget.Equal[Vangl]})    /varset bln_amvEngaged TRUE
	/if (!${bln_OMMEngaged} && ${eventTarget.Equal[Overlord]}) /varset bln_OMMEngaged TRUE
	
	/call SQLCheckMark
/return

Sub SQLCheckMark
	|/echo \awSQLTest \aoRunningSQLTestWrite ${Spawn[Xirin].ID} ?
 
    |/echo attempting insert into Characters
    |/sqlite advquery mycon dummy INSERT OR REPLACE INTO Characters(Name,SpawnID,ZoneID) VALUES ('${Me.Name}', ${Me.ID}, ${Zone.ID});
	/sqlite open mycon "${_pathDB}" SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE 

    /if (${Me.Buff[Mark of Death].ID}) { 
        /echo Attempting insert into MarkOfDeath
        /sqlite advquery mycon dummy INSERT OR REPLACE INTO MarkOfDeath(Name,SpawnID,Counters,CureInProcess) VALUES ('${Me.Name}', ${Me.ID},${Debuff.Diseased},FALSE);
		/if (${bln_amvEngaged} && ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
			/call Disengage
			/nav wp amvCleric
		}
    } else {
		/sqlite advquery mycon dummy SELECT * FROM MarkOfDeath WHERE Name = '${Me.Name}';
		/if (${sqlite.rows[dummy]}) {
			/echo Attempting DB Entry Removal Mark of Death
			/sqlite advquery mycon dummy DELETE FROM MarkOfDeath WHERE Name = '${Me.Name}';
		}

		/if (${bln_amvEngaged} && ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
			/call EngageTarget ${Spawn[npc Vangl].ID}
		}
	}
    /sqlite clear dummy
    /sqlite close mycon
/return 

Sub SQLCureMark
	/delay 2s
	/echo \awSQLCureMark: \aoReading MarkOfDeath table 
    |/sqlite query "{_pathDB}" build "SELECT * FROM Characters;"
    /sqlite open mycon "${_pathDB}" SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE 
    /sqlite advquery mycon dummy SELECT * FROM MarkOfDeath;
    /if (!${sqlite.rows[dummy]}) /goto :return
    /declare i int local 0
    /declare c int local 0
	/declare g int local 0
	/declare useGroup bool local FALSE
    /echo \a-w  Marked Rowcount: ${sqlite.rows[dummy]}
	/for i 1 to ${sqlite.rows[dummy]}
		 /if (${SpawnCount[pc ${sqlite.result[dummy ${i} Name]}]}) {
			/varcalc g ${g}+1
			/if (${sqlite.result[dummy ${i} Counters]} > ${maxCounters}) /varset maxCounters ${sqlite.result[dummy ${i} Counters]}
		 }
	/next i

	| Cast Group Cure
	/if (${g} > 2 || ${Me.Class.ShortName.Equal[SHM]} || ${bln_amvEngaged}) {
		/varset useGroup TRUE
		/call handleCureMarkOfDeath ${Me.Name} ${Me.ID} ${useGroup} ${maxCounters}
	} else {
		| Cast Single Cure
		/for i 1 to ${sqlite.rows[dummy]}
			/echo 
			/if (!${Select[${sqlite.result[dummy ${i} Name]},${str_groupMembers}]}) {
				|/echo \a-w Not a group member, skipping
				/continue
			} else /if (!${SpawnCount[pc ${sqlite.result[dummy ${i} Name]}]}) {
				/echo \a-w Group Member not in zone
				/continue
			} 
			
			/echo \awPC: \aoRow\aw[\ao${i}\aw].\aoName \ag${sqlite.result[dummy ${i} Name]} \awCounters: \ar${sqlite.result[dummy ${i} Counters]} \awInProcess? \ao${sqlite.result[dummy ${i} CureInProcess]} 
			/if (${sqlite.result[dummy ${i} Counters]} > 0) {
				/varset c ${sqlite.result[dummy ${i} Counters]}
				/echo \ar${sqlite.result[dummy ${i} Name]} MARKED FOR DEATH
				/echo \a-w    Curing ${sqlite.result[dummy ${i} Counters]} Counters
				/call TrueTarget ${sqlite.result[dummy ${i} SpawnID]}
				/call handleCureMarkOfDeath ${sqlite.result[dummy ${i} Name]} ${sqlite.result[dummy ${i} SpawnID]} ${useGroup} ${sqlite.result[dummy ${i} Counters]}
				/varcalc c ${c}-14
				/if (${c} < 0) /varset c 0
				/sqlite advquery mycon dummy UPDATE MarkOfDeath SET Counters = ${c}, CureInProcess = FALSE WHERE SpawnID = '${sqlite.result[dummy ${i} SpawnID]}';
			}
		/next i
	}
   

    |/sqlite advquery mycon dummy DELETE FROM MarkOfDeath WHERE Counters=0;
    
    :return
    /sqlite clear dummy
    /sqlite close mycon
/return
