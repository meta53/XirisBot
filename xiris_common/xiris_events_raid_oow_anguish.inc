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
#EVENT AMVKill 			"#1# tells the raid#*#'AMVKill #2#'"
#EVENT AMVRetreat		"#1# tells the raid#*#'AMVRetreat #2#'"

|   OMM
#EVENT OMMStart			"#*#OMMStart#*#"
#EVENT GazeYou			"#*#feel a gaze of deadly power#*#" 
#EVENT GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#EVENT GazeDone			"#*#Mata Muram's spell has been reflected#*#"
#EVENT OMMWarCry		"#*#OMMWarCry#*#"

|	Mark of Death (OMM & VANGL)
#EVENT MarkOfDeath		"#*#You have been marked for death#*#"
#EVENT MarkOfDeathFades "#*#The mark has faded#*#"

|	Root Tanking
#EVENT RootTankOn		"#*#RootTankOn#*#"
#EVENT RootTankOff		"#*#RootTankOff#*#"

Sub chkHasShaman
	/declare i int local 0
	/for i 1 to ${Group.GroupSize}
		/if (${Group.Member[${i}].Class.ShortName.Equal[SHM]}) /varset bln_groupHasShaman TRUE
	/next i
/return 

Sub InitEvents_OOW_Anguish
	/echo \ag Initializing Anguish
	/declare	bln_jelvanEngaged		bool	outer FALSE
	/declare	bln_hanvarEngaged		bool	outer FALSE
	/declare	bln_amvEngaged			bool	outer FALSE
	/declare	bln_OMMEngaged			bool	outer FALSE
	/declare	bln_groupHasShaman		bool  	outer FALSE
	
	/declare	ommCleric_X				int		outer 504
	/declare	ommCleric_Y				int		outer 4772
	/declare	eventTarget	   			string	outer NULL
	/declare	timer_omm_charm 		timer	outer 2s

	| Root Tanking
	/declare bln_rootTank			bool	outer FALSE
	/declare root_X					int	    outer 0
	/declare root_Y					int	    outer 0
	/declare root_Z					int	    outer 0
	/declare target_X				int 	outer 0
	/declare target_Y				int 	outer 0
	/declare target_Z				int 	outer 0
	/declare rootDist				int		outer 0

	| Berserk Cry List
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

	/declare 	bln_isMarked 			bool	outer FALSE
	/declare    timer_markCheck			timer   outer 0

	/call RegisterRaidEvents 	Events_OOW_Anguish 1


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
		/call chkOMMActions
		/call chkOMMEvents
	} 
	
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
	
	/if ((${Me.Class.ShortName.Equal[CLR]} && !${bln_chChainMember})|| ${Me.Class.ShortName.Equal[SHM]}) /call SQLCureMark 
	/call SQLCheckMark
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

Sub chkOMMActions
	|/echo ommEvents
	/doevents MarkOfDeath
	/doevents MarkOfDeathFades
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
	} else /if (${Me.Class.ShortName.Equal[WAR]} && (${int_MTID} != ${Me.ID})) {	
		/call chkOMMAdds
		/return
	} else /if (${Me.Class.ShortName.Equal[WAR]} && (${int_MTID} == ${Me.ID})) {
		:mt_loop
			/if (${Math.Distance[${target_Y},${target_X},${target_Z}:${root_Y} ${root_X} ${root_Z}]} > ${rootDist} && ${bln_rootTank}) /warp loc ${root_Y} ${root_X} ${root_Z}
			/doevents MarkOfDeath
			/doevents MarkOfDeathFades
			/doevents OMMStart
			/doevents GazeYou
			/doevents GazeBurst
			/doevents GazeDone 
			/call SQLCheckMark 
			/delay 10
		/goto :mt_loop	
	} else /if (${Me.Class.ShortName.Equal[SHD]}) {
		|/if (!${bln_OMMEngaged}) /return
		:shd_loop
			/doevents MarkOfDeath
			/doevents MarkOfDeathFades
			/doevents OMMStart
			/doevents GazeYou
			/doevents GazeBurst
			/doevents GazeDone 
			/call chkOMMFrenzied
			/call SQLCheckMark 
			/delay 10
		/goto :shd_loop
	}
	/if (${Select[${str_classArchetype},MELEE_MANA,MELEE_END]}) /call chkOMMFrenzied
/return 

Sub chkOMMEvents 
	/doevents MarkOfDeath
	/doevents MarkOfDeathFades
	/doevents OMMStart
	/doevents GazeYou
	/doevents GazeBurst
	/doevents GazeDone 
/return 

Sub Event_OMMStart 
	/doevents flush OMMStart
	/varset bln_OMMEngaged TRUE 
	/varset eventTarget Overlord

	/if (${Select[${Me.Class.ShortName},ENC]}) {
		/varset bln_activeCharming TRUE
		/return
	}
	/if (${Select[${Me.Class.ShortName},WAR,PAL]} && (${int_MTID} != ${Me.ID})) {
		/varset bln_offTank TRUE
		/varset int_offtankRadius 200
		/return
	} 
	/if (${int_MTID} == ${Me.ID}) {
		/call OMM_MTEngage
	}
/return

Sub OMM_MTEngage
	| Engagement sub for MT
	:OMMTarget
	/if (!${Target.ID}) {
		/target Overlord
		/goto :OMMTarget
	}
	/if (!${Target.Name.Find[Overlord]}) {
		/echo \ar OMM not targetted. Try that again!
		/target Overlord
		/goto :OMMTarget
	}

	/if (${Me.CombatAbilityReady[Stonewall Discipline]}) /doability "Stonewall Discipline"
    /delay 5
	/echo Hammering aggro
	/if (${Me.CombatAbilityReady[Ancient: Chaos Cry]}) /doability "Ancient: Chaos Cry"
	/delay 10
	/if (${Me.CombatAbilityReady[Bazu Bellow]}) /doability "Bazu Bellow"
	/delay 10
	/dgt SingSongs
	/face fast
/return 

Sub Event_GazeYou 
	/doevents flush GazeYou
	/call Interrupt
	/delay 20
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
		/if (${Me.Class.ShortName.NotEqual[BRD]}) {
			/echo \arInterrupting cast!
			/call Interrupt
		} 

		/echo \ao Mask in Slot\aw ${FindItem[Mirrored Mask].InvSlot}
		/if (${FindItem[Mirrored Mask].InvSlot} != 3) {
			/echo \ag Swapping Mask to Face!
			
			/call Interrupt
			/call Interrupt
			/exchange "Mirrored Mask" face
			/delay 10
		}
		/echo \agClicking the Mask!
		/if (${Me.Class.ShortName.Equal[BRD]}) {
			/shout Clicking my Mirrored Mask!
			/echo \ag Attempting Clicking \aoMirrored Mask \ag${Time.Time24}!
			/medley queue "Mirrored Mask" -interrupt
		} else {
			/shout Clicking my Mirrored Mask!
			/echo \ag Attempting Clicking \aoMirrored Mask \ag${Time.Time24}!
			/if (!${Defined[interruptFlag]}) /declare interruptFlag bool outer FALSE	
			/call MQ2Cast "Mirrored Mask" ITEM
			/call MQ2Cast "Mirrored Mask" ITEM
		}
		/delay 20
		/call LoadFaceSlot
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
					/call chkOMMEvents
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
					/call chkOMMEvents
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
				/call chkOMMEvents
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
	/doevents flush MarkOfDeath
	/echo \arMarked via EVENT
	/if (!${bln_amvEngaged} && ${eventTarget.Equal[Vangl]})    /varset bln_amvEngaged TRUE
	/if (!${bln_OMMEngaged} && ${eventTarget.Equal[Overlord]}) /varset bln_OMMEngaged TRUE
	/call SQLCheckMark TRUE
/return

Sub Event_MarkOfDeathFades 
	/doevents flush MarkOfDeathFades  
	/echo \agMarkFaded via EVENT
	/call SQLCheckMark TRUE
/return 

Sub SQLCheckMark(bool _overrideTimer)
	|/echo \awSQLTest \aoSQLCheckMark \awOveride:\ag${_overrideTimer} \awTimer:\ag${timer_markCheck}
	/if (${_overrideTimer}) /varset timer_markCheck 0
	/if (${timer_markCheck}) {
			/return
	}

    |/sqlite advquery mycon dummy INSERT OR REPLACE INTO Characters(Name,SpawnID,ZoneID) VALUES ('${Me.Name}', ${Me.ID}, ${Zone.ID});
    /if (${Me.Buff[Mark of Death].ID}) {
		| /if (${Select[${Me.Class.ShortName},CLR,SHM]}) {
		| 	/echo \agCuringSelf of Mark!
		| 	/call handleCureMarkOfDeath ${Me.Name} ${Me.ID} FALSE ${Debuff.Diseased}
		| } else {
			/echo \agFlagging myself as having Mark
			/sqlite open CheckMarkofDeath "${_pathDB}" SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE 
			/sqlite advquery CheckMarkofDeath markchk SELECT * FROM MarkOfDeath WHERE SpawnID = '${Me.ID}';

			/if (!${sqlite.rows[markchk]}) {
				/varset bln_isMarked TRUE
				/echo \aoMarked for Death! \ar${Time.Time24} \a-wRemoving Timer, will autocheck for cure
				/echo \awAttempting DB Entry \arINSERT \ayMark of Death
				/echo \a-w  =>advquery mycon dummy INSERT OR REPLACE INTO MarkOfDeath(SpawnID,Name,Counters,CureInProcess) VALUES(${Me.ID},'${Me.Name}',${Debuff.Diseased},NULL)  
				| INSERT OR REPLACE INTO MarkOfDeath(SpawnID, Name ,Counters,CureInProcess) VALUES (22, 'Xiris', 14, NULL);
				/sqlite advquery CheckMarkofDeath markchk INSERT OR REPLACE INTO MarkOfDeath(SpawnID, Name ,Counters,CurerName,CurerID) VALUES(${Me.ID}, '${Me.Name}', ${Debuff.Diseased}, NULL,0);
				/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
					/if (${bln_amvEngaged} && ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
						/echo disengaging and moving to cleric
						/call Disengage
						/nav wp amvCleric
					}
					/if (${bln_OMMEngaged} && ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
						/echo disengaging and moving to cleric
						/call Disengage
						/nav wp ommCleric
					}
				}
			|} 
		}
		/varset timer_markCheck 40
		/sqlite clear markchk
    	/sqlite close CheckMarkofDeath
    } else {
		| No longer have mark of death, clearing if fentry still found.
		/sqlite open CheckMarkofDeath "${_pathDB}" SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE 
		/sqlite advquery CheckMarkofDeath markchk SELECT * FROM MarkOfDeath WHERE SpawnID = '${Me.ID}';
		/if (${sqlite.rows[markchk]}) {
			/echo \a-wAttempting DB Entry \agDELETE \ayMark of Death
			/sqlite advquery CheckMarkofDeath markchk DELETE FROM MarkOfDeath WHERE SpawnID = '${Me.ID}';
		}

		/if (${bln_amvEngaged} && ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
			/call EngageTarget ${Spawn[npc Vangl].ID}
		}
		/sqlite clear markchk
		/sqlite close CheckMarkofDeath
		|/echo \a-wResetting timer to 1s
		/varset timer_markCheck 10
	}
/return 

Sub SQLCureMark
	/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /return
	/call chkHasShaman
	/if (${bln_groupHasShaman} && ${Me.Class.ShortName.NotEqual[SHM]}) {
		/echo group has a shaman, I dont need to check
		/return
	}

	/declare d int local ${Math.Rand[0,40]}
	/delay ${d}

 	/sqlite close CureMarkOfDeath
    /sqlite open CureMarkOfDeath "${_pathDB}" SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE 
    /sqlite advquery CureMarkOfDeath docure SELECT * FROM MarkOfDeath;

	|/echo ${sqlite.rows[docure]} len
    /if (!${sqlite.rows[docure]}) /goto :return
	|/echo \awSQLCureMark: \aoReading MarkOfDeath
	
    /declare i int local 0
    /declare c int local 0
	/declare g int local 0
	/declare maxCounters 	int local  0
	/declare useGroup 		bool local FALSE
	/declare alreadyCuring  bool local FALSE
	/declare n 				string local NULL
	/declare id  			int local 0
	/declare cn 			string local NULL
	/declare cid			int local 0
	/declare uc 			int local 0
    |/echo \a-w  Marked Rowcount: ${sqlite.rows[docure]} : Finding everyone that is in my group
	/for i 1 to ${sqlite.rows[docure]}
		/if (${Select[${sqlite.result[docure ${i} Name]},${str_groupMembers}]}) {
			/varcalc g ${g}+1
			/if (${sqlite.result[docure ${i} Counters]} > ${maxCounters}) /varset maxCounters ${sqlite.result[docure ${i} Counters]}
		}
	/next i
	
	/if (!${g}) /return
	/echo \a-w		Found \ay${g} Mark of Death in Group
	| Cast Group Cure
	/if (${g} > 2|| ${Me.Class.ShortName.Equal[SHM]} || ${bln_amvEngaged}) {
		/varset useGroup TRUE
		/rs ${myGroupNum}. => Casting GROUP cure on ${g} Chars ${Time.Time24}
		/for i 1 to ${sqlite.rows[docure]}|
			/if (${Select[${sqlite.result[docure ${i} Name]},${str_groupMembers}]}) {
				/sqlite advquery CureMarkOfDeath doupdate UPDATE MarkOfDeath SET CurerName = '${Me.Name}', CurerID = ${Me.ID} WHERE SpawnID = ${sqlite.result[docure ${i} SpawnID]};
			}
		/next i
		/doevents GazeYou
		/call handleCureMarkOfDeath ${Me.Name} ${Me.ID} TRUE ${maxCounters}
		/call SQLCheckMark TRUE
	} else {
		/rs ${myGroupNum}. => Casting SINGLE cure on ${g} Chars ${Time.Time24}
		| Cast Single Cure
		/for i 1 to ${sqlite.rows[docure]}
		 
			/varset id 	${sqlite.result[docure ${i} SpawnID]}
			/varset n  	${sqlite.result[docure ${i} Name]}
			/varset c 	${sqlite.result[docure ${i} Counters]}
			/varset cn 	${sqlite.result[docure ${i} CurerName]}
			

			/if (!${Select[${n},${str_groupMembers}]}) {
				/echo \a-w Not a group member, skipping
				/continue
			} else /if (!${SpawnCount[pc ${n}]}) {
				/echo \a-w Group Member not in zone
				|/sqlite advquery CureMarkOfDeath doupdate DELETE FROM MarkOfDeath WHERE SpawnID = ${id};
				/continue
			} 
			
			/if (${c} > 0) {
				/if (${Me.Class.ShortName.Equal[CLR]} && ${int_groupClerics} > 1) {
					
					/echo \a-wMore than 1 cleric in the group delaying \ag${d}
					/say delaying ${d}
					/delay ${d}
				}
				/varset cid ${sqlite.result[docure ${i} CurerID]}
				/echo \awPC: \aoRow\aw[\ao${i}\aw].\aoName \ag${n} \awCounters: \ar${c} \awInProcess? \ao${cn}\aw(\ag${cid}\aw) 
		
				
				/echo ${cid} != ${Me.ID} ${cid} > 0
				/if (${cid} != ${Me.ID} && ${cid}>0) {
					/echo \aoSomone Already Curing ${n}! ${Time.Time24}
					/continue
				}
				| update  the entry to note that you are healing
				/sqlite advquery CureMarkOfDeath doupdate UPDATE MarkOfDeath SET CurerName = '${Me.Name}', CurerID = ${Me.ID} WHERE SpawnID = ${sqlite.result[docure ${i} SpawnID]};

				/echo \ar${n} MARKED FOR DEATH
				/echo \a-w    Curing ${c} Counters
				
				
				/call TrueTarget ${id}
				/doevents GazeYou
				/call handleCureMarkOfDeath "${n}" ${id} FALSE ${c}
				
				/varcalc c ${c}-14
				/if (${c} < 0) /varset c 0
				
			}
		/next i
	}
    |/sqlite advquery mycon docure DELETE FROM MarkOfDeath WHERE Counters=0;
    :return
	/sqlite clear docure
   
/return


Sub EVENT_RootTankOn

/return 

Sub EVENT_RootTankOff
	/doevents flush RootTankOff
	/echo \aoUn-rooting myself!
	/varset bln_rootTank FALSE
/return