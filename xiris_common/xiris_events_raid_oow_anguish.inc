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
#EVENT JelvanPhase2		"#*#JelvanP2#*#"

|	Warden Hanvar
#EVENT HanvarSetup  	"#*#HanvarSetup#*#"
#EVENT HanvarStart		"#*#HanvarStart#*#"
#EVENT HanvarRetreat    "#1# tells the raid#*#'HanvarRetreat #2#'"
#EVENT HanvarKill       "#1# tells the raid#*#'HanvarKill #2#'"

|	Arch Magus Vangl
#EVENT AMVStart			"#*#AMVStart#*#"
#EVENT MarkOfDeath		"#*#You have been marked for death#*#"
#EVENT AMVRetreat 		"#1# tells the raid#*#'AMVRetreat #2#'"
#EVENT AMVKill 			"#1# tells the raid#*#'AMVKill #2#'"

|   OMM
#EVENT GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#EVENT GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#EVENT GazeDone			"#*#Mata Muram's spell has been reflected#*#"



Sub InitEvents_OOW_Anguish
	/cecho \ag Initializing Anguish
	/if (!${Defined[bln_jelvanEngaged]}) /declare bln_jelvanEngaged bool outer FALSE
	/declare bln_hanvarEngaged bool outer FALSE
	/declare bln_amvEngaged	   bool outer FALSE
	/declare timer_hanvar_ae   timer outer 0
	/declare timer_hanvarDebuff timer outer 30s

	/declare jelvanTarget 	   string outer Tantho
	/declare jelvanResists     string outer Tanthi

	/call RegisterRaidEvents Events_OOW_Anguish 15


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
		} else {
			/doevents JelvanPhase2
			/if (${str_classArchetype.Equal[PRIEST]} || ${Me.Class.ShortName.Equal[PAL]}) /call chkJelvanCures
			/if (${Me.Class.ShortName.Equal[SHM]} || ${str_classArchetype.Equal[CASTER]}) /call chkJelvanResists
			/if (${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[ENC]}) /call chkJelvanSlows
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
		/stand on
		/stick off
		/nav wp tureSafe
		/delay 10
		/call chkNeedCure
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]}) {
		/pet hold on
		/pet back off
	}
/return 

Sub Event_TureRampOff 
	/if (!${bln_engaged}) /return
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call TrueTarget ${int_targetID}
		/nav wp tureDPS
		:ture_run 
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active}) /goto :ture_run
		/squelch /killthis
		/squelch /attack on
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]}) {
		/pet attack
	}
/return 

Sub Event_TureWanton 
	/cecho \aw Warning! \arWanton Destruction \awon me!
	/call chkNeedCure
/return

|----------------------------
| Jelvan
|----------------------------

Sub Event_JelvanStart 
	/echo JelvanStart
	/if (${Me.Class.ShortName.NotEqual[WAR]} && ${Me.Class.ShortName.NotEqual[PAL]} && ${Me.Class.ShortName.NotEqual[SHD]}) {
		/varset bln_jelvanEngaged TRUE
	} else {
		:Tormentor_Checks
		/cecho \ayRunning Tormentor Checks!
	|-------------	
	| West, 1 MT	
		/if (${Me.CleanName.Equal[Xiria]}) {
			/if (${SpawnCount[npc Tanthi]} == 1 && !${bln_jelvanEngaged}) {
				/call TrueTarget ${Spawn[npc Tanthi].ID}
				/varset bln_jelvanEngaged TRUE
				/killthis
				/disc Stonewall
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
				|/disc Stonewall
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

	}
/return

Sub EVENT_JelvanPhase2
	/if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]}) /nav wp JelvanCaster
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
	/cecho \ag HANVAR EVENT STARTED \aw@ \ao${Time.Time24}
	/varset bln_hanvarEngaged TRUE
	/doevents flush HanvarStart
/return

Sub Event_HanvarSetup
	/echo Hanvar Setup
	/call SetResistTypes Cold,Poison,Magic
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
	/cecho \arHanvar Retreat Triggered \agStarting \aw${_eventParams}s \ag Timer
	/if (${Target.Name.Find[Hanvar]} || ${Spawn[npc Hanvar].Distance} < 100) {
		/if (${MeleeClasses.Find[${Me.Class.ShortName}]}) {
			/call Disengage
			/nav wp hanvarSafe
		} 
	}
	/doevents flush HanvarRetreat
/return

Sub Event_HanvarKill(string _sender, string _line, string _eventParams)
	/cecho \arHanvar Kill Triggered \agStarting \aw${_eventParams}s \ag Timer
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
/return


Sub chkHanvarCaster
	| Feedback Dispersion Test
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.Buff[Feedback Dispersion].ID} && ${Me.AltAbilityReady[Purify Soul]}) {
		/call TrueTarget ${Me.ID}
		/casting "Purify Soul" alt
	}

	/if (${Me.Class.ShortName.Equal[DRU]} && !${timer_hanvarDebuff}) {
		| Check to make sure it's debuffed correctly
		/call chkDebuffsOnSingleTarget ${Spawn[npc Hanvar].ID}
	}

	/if (${Me.Class.ShortName.Equal[SHM]} && !${timer_hanvarDebuff}) {
		| Check to make sure it's slowed|malod
		/call chkDebuffsOnSingleTarget ${Spawn[npc Hanvar].ID}
	}

	/if (${Me.Class.ShortName.Equal[ENC]} && !${timer_hanvarDebuff}) {
		| Check to make sure it's slowed|tashed
		/call chkDebuffsOnSingleTarget ${Spawn[npc Hanvar].ID}
	}

	/if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[WIZ]} && ${Target.ID} != ${Spawn[npc Hanvar].ID}) {
		/call TrueTarget ${Spawn[npc Hanvar].ID}
		/if (${Target.PctHPs} <= 99 && !${bln_engaged}) /call EngageTarget ${Target.ID}
	} 
/return

Sub chkHanvarMelee
	/declare targetToKill int local
	/if (${timer_hanvar_ae} < 80 ) {
		| GTFO he is about to AE
		/cecho \arHavar AE in ${timer_hanvar_ae}!
		/if (${Spawn[npc Hanvar].Distance} < 100 && !${Bool[${Target.Name.Find[guard]}]} ) {
			/echo stopping melee! heading to safe point
			/squelch /target clear
			/squelch /melee reset
			/warp wp hanvarSafe
		}
	}

	/if (${SpawnCount[npc guard]}) {
		/cecho \aoGuard is UP, assisting Xirin
		/squelch /target clear
		/squelch /melee reset
		:waitForGuard
		/dquery Xirin -q "Target.ID"
		/delay 20 ${DanNet.Query.Received}
		/varset targetToKill ${DanNet.Q}
		/echo ${targetToKill} Xirin has target
		/if (${Bool[${targetToKill}]}) {
			/if (${Spawn[id ${targetToKill}].PctHPs}<=99) {
				/call EngageTarget ${targetToKill}
			}
		} else {
			/goto :waitForGuard
		}
	} else {
		/cecho \agGuard is Down, assisting ${str_MTName}
		/squelch /target clear
		/squelch /melee reset
		/dquery  ${str_MTName} -q "Target.ID"
		/delay 20 ${DanNet.Query.Received}
		/varset targetToKill ${DanNet.Q}
		/if (${Bool[${targetToKill}]}) {
			/if (${Spawn[id ${targetToKill}].PctHPs}<=99) {
				/call EngageTarget ${targetToKill}
			}
		}
	}
/return




|----------------------------
| Arch Magus Vangl
|----------------------------
Sub chkAMVEvents
	| AMV Events
	/doevents AMVStart
	
	/doevents AMVRetreat
	/doevents AMVKill
	
	/if (${bln_amvEngaged} || ${Spawn[npc Vangl].Distance3D} < 200) {
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[SHM]}  && !${bln_chChainMember}) /call chkGroupMarkedForDeath 
		/if (${Me.Class.ShortName.Equal[PAL]}) /call chkSelfMarkedForDeath
	}

/return 
Sub Event_AMVStart 
	/varset bln_amvEngaged TRUE 
	/doevents flush AMVStart
/return

Sub EVENT_AMVRetreat
	/if (${Target.Name.Find[Hanvar]} || ${Spawn[npc Hanvar].Distance} < 100) {
		/if (${MeleeClasses.Find[${Me.Class.ShortName}]}) {
			/call Disengage
			/nav wp amvCleric
		} 
	}
	/doevents flush HanvarRetreat
	/if (${str_classArchetype.NotEqual[PRIEST]}) {
		/warp wp amvCleric
		/call Disengage
		/call chkMarkedForDeathCureDist
	}
/return 

Sub EVENT_AMVKill
	/if (${MeleeClasses.Find[${Me.Class.ShortName}]} ) {
		/call TrueTarget ${Spawn[npc Vangl].ID}
		/if (${Target.Distance3D} > 20) {
			/nav wp hanvarTank
		}
		/call EngageTarget ${Spawn[npc Vangl].ID}
	} else /if (${CasterClasses.Find[${Me.Class.ShortName}]}) {
		/call EngageTarget ${Spawn[npc Vangl].ID}
	}

	:return
	/doevents flush HanvarKill
/return 

Sub chkMarkedForDeathCureDist
	/nav wp amvCleric
	:markLoop
	/doevents
	/if (${Me.Buff[Mark of Death].ID}) {
		/cecho \ar MARK OF DEATH \awis \ag${str_myCurer} \aw@ \ag${Spawn[pc ${str_myCurer}].Distance3D}
		/cecho \awMoving to Curer!
		/call Disengage
		/melee reset
		/melee reset
		/goto :markLoop
	} 
	/if (!${Me.Target.Name.Find[Vangl]}) {
		/call TrueTarget ${Spawn[npc Vangl].ID}
	}
	/call EngageTarget ${Spawn[npc Vangl].ID}
	:return
/return 

Sub EVENT_MarkOfDeath
	/cecho \aoMarked for Death!
	/if (!${bln_amvEngaged}) /varset bln_amvEngaged TRUE
	/doevents flush MarkOfDeath
	/call chkMarkedForDeathCureDist
/return

Sub chkSelfMarkedForDeath
	/if (${Me.Buff[Mark of Death].ID}) /call handleCureRequest ${Me.Name} ${Me.ID} disease 14 TRUE
/return

Sub chkGroupMarkedForDeath
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
		/dquery ${Group.Member[${gm}]} -q "Me.Buff[Mark of Death].ID"
		/delay 20 ${DanNet.Query.Received}
		/if (${Bool[${DanNet.Q}]}) {
			/cecho \ar${Group.Member[${gm}]} \awreply:${Bool[${DanNet.Q}]}
			/cecho \ar MARK OF DEATH FOUND
			/varset  foundMark TRUE
			/varcalc haveMark ${haveMark}+1
			/call getCureCounters ${Group.Member[${gm}]} disease
			/varset gcount[${i}] ${Int[${Macro.Return}]}
		} 

		
	/next i
	
	| No Marks found? return
	/if (!${foundMark}) /goto :return

	/cecho \ar${haveMark} \aoInstances of Mark of Death found!
	| If Marks are less than 3, or I am a paladin then single target cure them
	:SingleCure
	/if (${Me.Class.ShortName.Equal[PAL]} || ${haveMark} < 2) /goto :CureByCounters
	
	| If I am a cleric or shaman, cast group cure, and then recheck counters
	:GroupCure 
	/cecho \awGroupCure: \ag${str_heal_group} \ay${Me.SpellReady[${str_heal_group}]} \aw(\ag${Me.PctMana}) \ay${haveMark}>=2
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.PctMana} > 30 && ${haveMark} >= 2) {
		
		:GroupCureWait 
		/varcalc cureWaitCount ${cureWaitCount}+1
		/if (!${Me.SpellReady[${str_heal_group}]}) {
			/delay 5
			/if (${cureWaitCount} > 10) {
				/cecho \arGroupCure failed due to wait, trying by counters
				/goto :CureByCounters
			} else {
				/cecho \arGroupCure waiting for ${str_heal_group} ${Me.SpellReady[${str_heal_group}]}
				/goto :GroupCureWait
			}
		} else {
			/cecho \agGroupCuring with ${str_heal_group}
			/g Group Heal incoming, get close!
			/for i 1 to 5
				/if (${Group.Member[${i}].ID} != ${int_MTID} && ${Select[${Group.Member[${i}].Class.ShortName},${MeleeClasses}]}) {
					/dex ${Group.Member[${i}]} /multiline ; /mqpause on ; /melee reset ; /warp wp amvCleric ; 
				}
			/next i
			/call MQ2Cast "${str_heal_group}" gem4 2s 
			/dgge /mqpause off
			/if (${Spawn[npc Vangl].PctHPs} <= 99) /dgge KillMob ${Spawn[npc Vangl].ID} "${Spawn[npc Vangl].Name}" ${Time.Time24}
			/goto :CheckGroup
		}
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/cecho \agGroupCuring with ${str_cure_disease}
		/g Group Heal incoming, get close!
		/for i 1 to 5
			/if (${Group.Member[${i}].ID} != ${int_MTID} && ${Select[${Group.Member[${i}].Class.ShortName},${MeleeClasses}]}) {
				/dex ${Group.Member[${i}]} /warp wp amvCleric ; 
			}
		/next i
		/call MQ2Cast "${str_cure_disease}" gem7 8s
		/dgge /mqpause off
		/if (${Spawn[npc Vangl].PctHPs} <= 99) /dgge KillMob ${Spawn[npc Vangl].ID} "${Spawn[npc Vangl].Name}" ${Time.Time24}
		/goto :CheckGroup
		
	}

	| All else fails so cure anyone with a disease counter singly
	:CureByCounters
	/cecho \awCureByCounters: \ag${str_cure_disease} \ayReady?${Me.SpellReady[${str_cure_disease}]} \aw(\ag${Me.PctMana}) \ay${haveMark}< 2
	/for i 1 to 6
		/varcalc gm ${i}-1
		/call chkHeal_MT
		/if (${gcount[${i}]} > 0) {
			/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${gcount[${i}]} TRUE
		}
	/next i

	:CheckMT
	/if (${tankIndex}<10) {
		
		/dquery ${Group.Member[${tankIndex}]} -q "Me.Buff[Mark of Death].ID"
		/delay 20 ${DanNet.Query.Received}
		
		/if (${Bool[${DanNet.Q}]}) {
			/cecho \ar${Group.Member[${tankIndex}]} \awreply: ${DanNet.Q} ${Bool[${DanNet.Q}]}
			/cecho \ar MARK OF DEATH FOUND on TANK
			/call getCureCounters ${Group.Member[${tankIndex}].Name} disease
			/call handleCureRequest ${Group.Member[${tankIndex}].Name} ${Group.Member[${tankIndex}].ID} disease ${Int[${Macro.Return}]} TRUE
		}
	}
:return
/return

|----------------------------
| Overlord Mata'Muram
|----------------------------
Sub Event_GazeYou 
	/cecho \arGAZE OF MATA MURAM on ME!
	/cecho \a-w Clicking \agMirrored Mask
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/call MQ2Medley SONG "${DEBUFF_${db}_Spell}" ${int_targetID} TRUE
	} else {
		/if (!${Defined[interruptFlag]}) /declare interruptFlag bool outer FALSE	
		/if (${Bool[${Me.Casting.ID}]}) /call Interrupt
		/call MQ2Cast "Amulet of Necropotence" 
	}

/return 

Sub Event_GazeBurst
/return 

Sub Event_GazeDone
/return