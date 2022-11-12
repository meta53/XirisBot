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
#Event JelvanSetup		"#*#JelvanSetup#*#"
#EVENT JelvanStart  	"#*#Hail, Jelvan#*#"

|	Warden Hanvar
#EVENT HanvarSetup  	"#*#HanvarSetup#*#"
#EVENT HanvarStart		"#*#HanvarStart#*#"
#EVENT HanvarRetreat    "#1# tells the raid#*#'HanvarRetreat #2#'"
#EVENT HanvarKill       "#1# tells the raid#*#'HanvarKill #2#'"



|	Arch Magus Vangl
#EVENT AMVStart			"#*#AMVStart#*#"

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
			/if (${str_classArchetype.Equal[PRIEST]} || ${Me.Class.ShortName.NotEqual[PAL]}) /call chkJelvanCures
			/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) /call chkJelvanResists
			/if (${Me.Class.ShortName.NotEqual[PAL]} || ${Me.Class.ShortName.NotEqual[ENC]}) /call chkJelvanSlows
		} 
	}

	| Hanvar Events
	/if (${Bool[${SpawnCount[npc Hanvar]}]} && ${Spawn[npc Hanvar].Distance3D} < 400) {
		/call chkHanvarEvents
	}


	| Vangl Events
	/if (${Bool[${SpawnCount[npc Vangl]}]} && ${Spawn[npc Vangl].Distance3D} < 400) {
		/if (!${bln_amvEngaged}) {
			/doevents AMVStart
		} else {
			/if (${Me.Buff[Feedback Dispersion].ID} && ${Me.AltAbilityReady[Purify Soul]} && ${Me.Class.ShortName.Equal[CLR]}) {
				/target id ${Me.ID}
				/casting "Purify Soul" alt
			}
			/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[PAL]}) {
				/call chkMarkedForDeath
			} else {
				/call chkMarkedForDeathCureDist
			}
		}
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
Sub Event_HanvarSetup
	/echo Hanvar Setup
	/call SetResistTypes Cold,Poison,Magic
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/nav wp hanvarCaster
	} else /if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]} ||  ${Me.Class.ShortName.Equal[DRU]}) {
		/nav wp hanvarCaster
	} else /if (${Me.Class.ShortName.Equal[CLR]}) {
		/nav wp hanvarCleric
	}
	| We need to light up the fucker with DOTs and Cold Nukes
	/if (${Me.Class.ShortName.Equal[SHM]}) /call setupForHanvarSHM
	/doevents flush HanvarSetup
/return

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

Sub Event_HanvarRetreat(string _sender, string _line, string _eventParams)
	/cecho \arHanvar Retreat Triggered \agStarting \aw${_eventParams}s \ag Timer
	/if (${Target.Name.Find[Hanvar]} || ${Spawn[npc Hanvar].Distance} < 100) {
		/if (${MeleeClasses.Find[${Me.Class.ShortName}]} ) {
			/call Disengage
			/warp wp hanvarSafe
		} 
	}
	/doevents flush HanvarRetreat
/return

Sub Event_HanvarKill(string _sender, string _line, string _eventParams)
	/cecho \arHanvar Kill Triggered \agStarting \aw${_eventParams}s \ag Timer
	/if (${SpawnCount[npc guard]}) /return
	/if (${MeleeClasses.Find[${Me.Class.ShortName}]} ) {
		/call TrueTarget ${Spawn[npc Hanvar].ID}
		/warp b 10
		/call EngageTarget ${Spawn[npc Hanvar].ID}
	} else /if (${CasterClasses.Find[${Me.Class.ShortName}]}) {
		/call EngageTarget ${Spawn[npc Hanvar].ID}
	}

	:return
	/doevents flush HanvarKill
/return


Sub setupForHanvarSHM

	/echo setupForHanvarSHM
	/varset str_spellset hanvar
	/memspellset ${str_spellset}
	
	| Reset my DOT types
	| |_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer

	| "Blood of Yoppa"|TRUE|1|2|TRUE|100|0|Poison|FALSE|0

	/call clearSpellLineUp DOT
	/varset DOT_Total 2
	/call addSpellToLineup DOT 1 "Blood of Yoppa"|TRUE|1|2|TRUE|100|0|Poison|FALSE|0
	/call addSpellToLineup DOT 2 "Blood of Saryrn"|TRUE|1|2|TRUE|100|0|Poison|FALSE|0

	/call clearSpellLineUp NUKE
	/varset NUKE_Total 1
	/call addSpellToLineup NUKE 1 "Ice Age"|TRUE|1|2|TRUE|100|0|Poison|FALSE|0
	
	| Reset my DEBUFF types
	/varset int_debuffTotal 2
	/varset DEBUFF_1 "Malos"
	/varset DEBUFF_1_Use TRUE 
	/varset DEBUFF_1_Resist ${Spell[${DEBUFF_1}].ResistType}
	/varset DEBUFF_2 "Balance of Discord"
	/varset DEBUFF_2_Use TRUE 
	/varset DEBUFF_2_Resist ${Spell[${DEBUFF_2}].ResistType}
/return

Sub chkHanvarCaster
	| Feedback Dispersion Test
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.Buff[Feedback Dispersion].ID} && ${Me.AltAbilityReady[Purify Soul]}) {
		/call TrueTarget ${Me.ID}
		/casting "Purify Soul" alt
	}

	/if (${Me.Class.ShortName.Equal[DRU]}) {
		| Check to make sure it's slowed
		/call chkDebuffsOnSingleTarget ${Spawn[npc Hanvar].ID}
	}

	/if (${Me.Class.ShortName.Equal[SHM]}) {
		| Check to make sure it's slowed
		/call chkDebuffsOnSingleTarget ${Spawn[npc Hanvar].ID}
	}

	/if (${Me.Class.ShortName.Equal[ENC]}) {
		| Check to make sure it's slowed
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
Sub Event_AMVStart 
	/varset bln_amvEngaged TRUE 
	/if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[DRU]}) {
		/nav wp amvCaster
	} else /if (${Me.Class.ShortName.Equal[CLR]}) {
		/nav wp amvCleric
	} else {
		/nav wp amvDPS
	}
	/doevents flush AMVStart
/return

Sub chkMarkedForDeathCureDist
	:markLoop
	/if (!${Bool[${SpawnCount[pc ${str_myCurer}]}]}) {
		/call 		getGroupCurer disease
		/varset 	str_myCurer ${Macro.Return}
	}

	/if (${Me.Buff[Mark of Death].ID}) {
		/if (${Spawn[pc ${str_myCurer}].Distance3D} > 80 && ${Spawn[pc ${str_myCurer}].Distance3D} < 250 && !${bln_tanking}) {
			/varset str_navCureStart ${Me.X} ${Me.Y} ${Me.Z}
			/cecho \awNavigating to Curer because range is too far for cure
			/varset bln_navCure TRUE
			/navigate spawn ${str_myCurer}
			:nav_curer
			/if (${Spawn[pc ${str_myCurer}].Distance3D} <= 80) /nav stop
			/if (${Navigation.Active}) /goto :nav_curer
			/goto :markLoop
		} else /if (${Spawn[pc ${str_myCurer}].Distance3D} <= 80) {
			/cecho \ayWaiting for cure of Mark of Death
		} else /if (${Spawn[pc ${str_myCurer}].Distance3D} >= 250) {
			/cecho \arMyCurer \ay${str_myCurer} \aris too far away!
			/rs Cannot be cured!
		}
	}
	:return
/return 

Sub chkMarkedForDeath
	/if (${bln_chChainMember}) /return
 
	/declare gm int local 1
	/declare haveMark int local 0
	/declare gcount[5]  int local 0
	/declare foundMark bool local FALSE
	/declare tankIndex int local 10

	:CheckGroup
	/varset haveMark 	0
	/varset foundMark 	FALSE
	/for gm 0 to ${Group}
		/if (${Group.Member[${gm}].CleanName.Equal[Xiris]}) /varset tankIndex ${gm}
		/dquery ${Group.Member[${gm}]} -q "Me.Buff[Mark of Death].ID"
		/delay 20 ${DanNet.Query.Received}
		
		/if (${Bool[${DanNet.Q}]}) {
			/cecho \ar${Group.Member[${gm}]} \awreply:${Bool[${DanNet.Q}]}
			/cecho \ar MARK OF DEATH FOUND
			/varset  foundMark TRUE
			/varcalc haveMark ${haveMark}+1
			/call getCureCounters ${Group.Member[${gm}]} disease
			/varset gcount[${gm}] ${Int[${Macro.Return}]}
		} else {
			|/cecho \ag${Group.Member[${gm}]} reply: ${Bool[${DanNet.Q}]}
		}
	/next gm
	
	| No Marks found? return
	/if (!${foundMark}) /goto :return

	/cecho \ar${haveMark} \aoInstances of Mark of Death found!
	| If Marks are less than 3, or I am a paladin then single target cure them
	:SingleCure
	/if (${Me.Class.ShortName.Equal[PAL]} || ${haveMark} < 2) /goto :CureByCounters
	
	| If I am a cleric or shaman, cast group cure, and then recheck counters
	:GroupCure 
	/cecho \awGroupCure: \ag${str_heal_group} \ay${Me.SpellReady[${str_heal_group}]} \aw(\ag${Me.PctMana}) \ay${haveMark}>=3
	/if (${Me.Class.ShortName.Equal[CLR]} && ${Me.SpellReady[${str_heal_group}]} && ${Me.PctMana} > 30 && ${haveMark} >= 2) {
		/call MQ2Cast "${str_heal_group}" gem4 2s 
		/goto :CheckGroup
	} else /if (${Me.Class.ShortName.Equal[SHM]}) {
		/call cureSelf disease 18 
		/goto :CheckGroup
	}

	| All else fails so cure anyone with a disease counter singly
	:CureByCounters
	/cecho \awCureByCounters: \ag${str_cure_disease} \ay${Me.SpellReady[${str_heal_group}]} \aw(\ag${Me.PctMana}) \ay${haveMark}< 2
	/for gm 0 to ${Group} 
		/if (${gcount[${gm}]} > 0) {
			/call handleCureRequest ${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${gcount[${gm}]}
		}
	/next gm

	:CheckMT
	/if (${tankIndex}<10) {
		/dquery ${Group.Member[${tankIndex}]} -q "Me.Buff[Mark of Death].ID"
		/delay 20 ${DanNet.Query.Received}
		
		/if (${Bool[${DanNet.Q}]}) {
			/cecho \ar${Group.Member[${tankIndex}]} \awreply: ${DanNet.Q} ${Bool[${DanNet.Q}]}
			/cecho \ar MARK OF DEATH FOUND on TANK
			/call getCureCounters ${Group.Member[${tankIndex}].Name} disease
			/call handleCureRequest ${Group.Member[${tankIndex}].Name} ${Group.Member[${tankIndex}].ID} disease ${Int[${Macro.Return}]}
		}
	}
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