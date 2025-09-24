|**
----------------------------
xiris_curing.inc
----------------------------
This include contains logic for curing self/group debuffs.
Useful for raids like Txevu

Works for all classes. Will set a group member as dedicated curer.

Will cure with
RGC, Antidote, Radiant Cure for effects that DRAIN HP or MANA. Resist/Movement debuffs are ignored.
______________________________________________
REVISION HISTORY
    03.19.16    xiris	INITIAL REVISION
	12.20.16	xiris 	On curing group, checks for own debuff before casting on self, 
						potentially saving time not curing self 
						(note, can't see other group members debuff on eqtitan)
	09.03.18	xiris   Added to common include, so this is initialized. All toons now ask for cures
						If in combat, only important debuffs, out of combat, all debuffs
	03.17.22	xiris	Updated cure request EVENT and cleaned up some code
						Adding curer interrogation on start up to bypass un-equiped (no spell) potential group curers
	05.23.22	xiris	Reduced events to only have BC
	06.13.22	xiris	Restored events for Group/Party/Raid due to need to split raids sometimes (multiple camp pulling, jelvan balancing etc)	
						Added 'simple' cureMe request handler for manual entry (usually by MT)
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only			
	07.28.22	xiris   Updated the chkNeedCure request to have more must cure spells		
**|

|-- Single PC Cure Request eg: /g cureme poison
#EVENT cureMe			"#1# tells the group#*#'cureMe #2#'"
#EVENT cureMe			"#1# tells you#*#'cureMe #2#'"
#EVENT cureMe			"#1# tells the raid#*#'cureMe #2#'"
#EVENT cureMe  			"[ #1# #*#]#*#cureMe #2#"

|-- Macro generated cure requests/rejections
#EVENT cureRequest  	"[ #1# #*#]#*#cureRequest #2#"
#EVENT cureReceived  	"[ #1# #*#]#*#I cured you#*#(#2#)#*#"
#EVENT cureRejected  	"[ #1# #*#]#*#I cannot cure you#*#"

|-- Poison
#EVENT cureGroupPoison	"#1# tells the group#*#'cureGroupPoison #2#'"
#EVENT cureGroupPoison	"#1# tells you#*#'cureGroupPoison #2#'"
#EVENT cureGroupPoison	"#1# tells the raid#*#'cureGroupPoison #2#'"
#EVENT cureGroupPoison 	"[ #1# #*#]#*#cureGroupPoison #2#"

|-- Disease
#EVENT cureGroupDisease	"#1# tells the group#*#'cureGroupDisease #2#'"
#EVENT cureGroupDisease	"#1# tells you#*#'cureGroupDisease #2#'"
#EVENT cureGroupDisease	"#1# tells the raid#*#'cureGroupDisease #2#'"
#EVENT cureGroupDisease "[ #1# #*#]#*#cureGroupDisease #2#"

|-- Curse
#EVENT cureGroupCurse	"#1# tells the group#*#'cureGroupCurse #2#'"
#EVENT cureGroupCurse	"#1# tells you#*#'cureGroupCurse #2#'"
#EVENT cureGroupCurse	"#1# tells the raid#*#'cureGroupCurse #2#'"
#EVENT cureGroupCurse 	"[ #1# #*#]#*#cureGroupCurse #2#"

|-- Corruption (N/A until TSS)



|-- Other Events
#EVENT ResetGroupCurer	"#1# tells you#*#'ResetGroupCurer#*#'"
#EVENT ResetGroupCurer	"#1# tells the group#*#'ResetGroupCurer#*#'"
#EVENT ResetGroupCurer	"#1# tells the raid#*#'ResetGroupCurer#*#'"
#EVENT ResetGroupCurer	"[ #1# #*#]#*#ResetGroupCurer#*#"

#EVENT AutoCureMTOn		"#*#AutoCureMTOn#*#"
#EVENT AutoCureMTOn		"#*#AutoCureMTOff#*#"

|-- Forcing a fire of group cure *(shaman, nadox)
#EVENT FireNadox		"#1# tells the group#*#'fireNadox#*#'"
#EVENT FireNadox		"#1# tells you#*#'fireNadox#*#'"
#EVENT FireNadox		"#1# tells the raid#*#'fireNadox#*#'"
#EVENT FireNadox 		"[ #1# #*#]#*#fireNadox#*#"

Sub InitEvents_Curing
	/if (${str_classArchetype.Equal[PRIEST]} || ${Me.Class.ShortName.Equal[PAL]}) /call RegisterClassEvents Events_Curing 5
	/call RegisterBackgroundEvents Events_Curing_Background 3
/return

Sub Events_Curing
	/doevents cureMe
	/doevents cureRequest
	/doevents cureGroupPoison
	/doevents cureGroupDisease
	/doevents cureGroupCurse
	/doevents AutoCureMTOn
	/doevents AutoCureMTOff
	/if (${Me.Class.ShortName.Equal[SHM]}) /doevents fireNadox
/return

Sub Events_Curing_Background
	/doevents cureReceived
	/doevents cureRejected
	/doevents ResetGroupCurer
/return

Sub Event_AutoCureMTOn
	/if (!${Select[${Me.Class.ShortName.},CLR,DRU,SHM]}) /return 
	/echo \agNow Automatically Curing MT
	/varset bln_autoCureMT TRUE
/return 

Sub Event_AutoCureMTOff
	/echo \awNo Longer Automatically Curing MT
	/varset bln_autoCureMT FALSE
/return

Sub Event_FireNadox
	/echo \agFiring Group: Blood of Nadox!
	/call TrueTarget ${Me.ID}
	/call MQ2Cast "${str_cure_poison}" gem4 8s
/return 

|-- Single target cure, simple syntax (for manual entry)
|   Counters determined by Dannet query
|	string _pcName, string _type
Sub Event_cureMe(_line, _sender, _eventParams)
	/declare name		string local ${_sender}
	/declare curetype	string local ${_eventParams}
	/declare qs 		string 	local	NULL

	/call getDebuffPluginQuery ${cureType}
	/varset qs ${Macro.Return}
	/call getCureCounters ${name} ${qs}
	/declare counters int local ${Macro.Return}

	/echo \awCure Request \ayname: \ag${name}(${Spawn[pc ${name}].ID}) \aytype: \ag${curetype} \aycounters: \ag${counters}
	/call handleCureRequest ${name} ${Spawn[pc ${name}].ID} ${curetype} ${counters} FALSE
	
/return


|-- Single target cure, generated syntax (for macro call)
|   Counters determined by DanNet query
Sub Event_cureRequest(_line, _sender, _eventParams)	
	/declare name 		string	local	${_eventParams.Arg[1, ]}
	/declare id 		int 	local	${_eventParams.Arg[2, ]}
	/declare curetype 	string 	local	${_eventParams.Arg[3, ]}
	/declare counters 	int 	local	${_eventParams.Arg[4, ]}
	/declare forced 	bool 	local	${_eventParams.Arg[5, ]}
	/declare qs 		string 	local	NULL

	/call getDebuffPluginQuery ${cureType}
	/varset qs ${Macro.Return}
	/call getCureCounters ${name} ${qs}
	/if (${counters} == 0) /varset counters ${Macro.Return}

	/echo \awCure Request \ayname: \ag${name}(${id}) \aytype: \ag${curetype} \aycounters: \ag${counters} \ayforced: \ag${forced}
	/call handleCureRequest ${name} ${id} ${curetype} ${counters} ${forced}
	
/return

Sub Event_cureReceived(_line, _sender, _eventParams)
	/declare id int local ${Int[${_eventParams}]}
	/if (${id} == ${Me.ID}) /call resetCureState
	
/return

Sub Event_cureGroupPoison(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/docommand /${announceChannel} Curing Group of Poisons with: ${str_cure_poison}
	/call cureGroup poison ${_eventParams}
	
/return

Sub Event_cureGroupDisease(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/docommand /${announceChannel} Curing Group of Disease with: ${str_cure_disease}
	/call cureGroup disease ${_eventParams}
	
/return

Sub Event_cureGroupCurse(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} &&${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/docommand /${announceChannel} Curing Group of Curses with: ${str_cure_curse}
	/call cureGroup curse ${_eventParams}
	
/return

Sub Event_cureGroupCorruptiuon(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} &&${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/docommand /${announceChannel} Curing Group of Corruption with: ${str_cure_corruption}
	/call cureGroup corruption ${_eventParams}
/return

Sub resetCureState
	/echo \agCure State Reset
	/varset bln_askedForCure 	FALSE
	/varset bln_needsCure		FALSE
	/varset timer_cure_request  10s
	/varset bln_navCure			FALSE
/return 




|-- Check if I need a cure (all classes)
Sub chkNeedCure
	|-- Check Need cure is disabled for OMM since we are only caring about Mark
	/if (${bln_OMMEngaged} || ${cache_zoneShort.Equal[arena]}) /return
	|-- If I have bln_autoCureMT Set to TRUE, always check if the MT needs cure and do it proactively
	/if (${bln_autoCureMT}) /call cureMTAuto
	|-- Verify that I actually have debuffs even if it says i need to cure, reset that
	/if ((!${Debuff} && ${bln_needsCure})) /call resetCureState
	|-- Verify need if I havent asked and recently
	/if (${Debuff.Count} == 0 || ${Debuff.Counters} > 200 || ${timer_cure_request}) 	/return
	|-- Skip if nocure mode is on
	/if (!${bln_useCures} && ${Me.Class.ShortName.NotEqual[CLR]} || ${Me.Class.ShortName.NotEqual[SHM]}) /return
	
	|-- Force the cure?
	/declare forceCure		bool 	local	FALSE
	/declare cureType 		string 	local 	NOTSET
	/declare counters 		int 	local
	/declare debuffName		string  local   NOTSET

	|-- Check for Forcings
	|-- If I am tanking, and I have under 100 counters (some debuffs are uncureable) or we aren't engaged, then force the cure request
	/if (${bln_tanking} && (${Debuff.Counters} < 100 && ${Debuff.Count}) || !${bln_engaged}) /varset bln_needsCure TRUE
	
	|-- Mana Drains (casting classes)
	/if ((${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${Me.Class.ShortName.Equal[PAL]} || ${Me.Class.ShortName.Equal[SHD]}) && (${Debuff.ManaDrain} >= ${int_manadrain_threshold}) ) /varset bln_needsCure TRUE

	|-- HP Drains
	/if  (${Debuff.HPDrain} >= ${int_hpdrain_threshold}) /varset bln_needsCure TRUE

	|-- Slows (melee)
	/if ((${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[MELEE_END]}) && ${Debuff.Slowed} && (${Debuff.Counters} < 100 )) /varset bln_needsCure TRUE

	|-- Slows (caster)
	/if ((${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) && ${Debuff.SpellSlowed} && (${Debuff.Counters} < 100 )) /varset bln_needsCure TRUE


	|-- Debuffs that CANNOT be removed || Need to let our healer specific events handle it
	/if ((${Me.Buff[Gaze of Anguish].ID} && (${bln_jelvanEngaged} || ${bln_amvEngaged}))  || ${Me.Buff[Test of Efficiency].ID} || ${Me.Buff[Chains of Anguish].ID} || ${Me.Buff[Aura of Destruction].ID} || ${Me.Buff[Rigidity].ID} || ${Me.Buff[Curse of Misfortune].ID} || ${Me.Buff[Imprecision].ID} || ${Me.Buff[Impurity].ID} || ${Me.Buff[Ressurection Sickness].ID} || ${Me.Buff[Clockwork Doom]}) {
		/call resetCureState
		/goto :return

	|-- Debuffs that MUST be removed (DISEASE)
	|  Force cure these in all case
	} else /if ((${Me.Buff[Gaze of Anguish].ID})) {
		|-- Gaze of Anguish must be cured!
		/varset cureType disease
		/varset counters ${Debuff.Diseased}
		/varset forceCure TRUE
		/varset debuffName "GazeOfAnguish"
		/goto :get_curer

	|-- Debuffs that MUST be removed (CURSE)	
	|   Only force cure these if we are a priest or caster
	} else /if ((${Me.Buff[Torment of Body].ID} || ${Me.Buff[Wanton Destruction].ID} || (${Me.Buff[Feedback Dispersion].ID}) && (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}))) {
		/varset cureType curse
		/varset counters ${Debuff.Cursed}
		/varset forceCure TRUE
		/varset debuffName "TormentOfBody|Wanton Destruction|Feedback Dispersion"
		/goto :get_curer
		
	|-- Debuffs that MUST be removed (POISON)
	|   Only force cure these if we are a priest or caster
	} else /if ((${Me.Buff[Fulmination].ID}) && (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]})) {
		/varset cureType poison
		/varset counters ${Debuff.Poisoned}
		/varset forceCure TRUE
		/varset debuffName "Fulmination"
		/goto :get_curer

	}  

	
	:cure_determine
	/if ( ${forceCure} || ${bln_needsCure}) {
		/varset bln_needsCure TRUE

		| Get the required cureType
		/if (${Debuff.Diseased} >= 1 && ${cureType.Equal[NOTSET]}) {
			|-- Handle Disease
			/varset cureType disease
			/varset counters ${Debuff.Diseased}
		} else /if (${Debuff.Poisoned} >= 1 && ${cureType.Equal[NOTSET]}) {
			|-- Handle Posion
			/varset cureType poison
			/varset counters ${Debuff.Poisoned}
		} else /if (${Debuff.Cursed} >= 1 && ${cureType.Equal[NOTSET]}) {
			|-- Handle Curse
			/varset cureType curse
			/varset counters ${Debuff.Cursed}
		} else /if (${Debuff.Corrupted} >= 1 && ${cureType.Equal[NOTSET]}) {
			|-- Handle Corruption
			/varset cureType corruption
			/varset counters ${Debuff.Corrupted}
		}

		| Return if we can't figure out what cure type
		/if (${cureType.Equal[NOTSET]}) /return

		| Get my curer for this type
		:get_curer
		/call getGroupCurer ${cureType}
		/varset str_myCurer ${Macro.Return}
		/if (${str_myCurer.Equal[NOTFOUND]}) {
			/shout \ar I HAVE NO CURER!
			/call resetCureState
			/varset timer_cure_request 60s
			/varset bln_askedForCure TRUE
			/goto :return
		}

		:get_cureRange
		| Get the range of this cure
		|/call setCureRange ${cureType}

		| Start asking for cure
		:request_cure
		/echo \arCURE REQUIRED:\ay Counters:\ag${counters} \ayType:\ag${cureType} \ayRange:\ag${cureRange} \aoName:${debuffName}
		/if (${str_myCurer.Equal[${Me.Name}]}) {
			/call handleCureRequest ${Me.Name} ${Me.ID} ${cureType} ${counters} ${forceCure}
		} else {
			/if (${Spawn[pc ${str_myCurer}].ID}) {
				/echo \aoCurer:\ag${str_myCurer} \aoDist:\ag${Spawn[pc ${str_myCurer}].Distance3D} \aoRange:\ag${cureRange}
				/if (${Spawn[pc ${str_myCurer}].Distance3D} < ${cureRange}) {
					/echo \aoCURE REQUESTED:\ay Counters:\ag${counters} \ayType:\ag${cureType} \ayForced:\ag${forceCure}
					/call requestCure ${cureType} ${counters} ${forceCure}
				} else {
					/echo \arMy Curer is too far away
					/varset timer_cure_request 10s
					/varset bln_askedForCure TRUE
					/goto :return
				}
			} else {
				/echo \arMy Curer is dead or not in zone
				/varset timer_cure_request 10s
				/varset bln_askedForCure TRUE
				/goto :return
			}
		}
		/varset timer_cure_request 30s
		/varset bln_askedForCure TRUE
	}
	
:return
/return

Sub requestCure(string _type, int _counters, bool _force) 
	|/shout REQUESTED CURE from ${str_myCurer} ${_type} ${_counters} ${_force}
	/dt ${str_myCurer} cureRequest ${Me.Name} ${Me.ID} ${_type} ${_counters} ${_force}
	/declare waitCounter int local 0
	/if (${bln_navCure}) {
		/echo \awWaiting for cure since my initial spot is outside of cure range - \ag${cureRange}
		:wait_loop
		/doevents cureReceived
		/varcalc waitCounter ${waitCounter}+1
		/delay 10
		/if (${waitCounter}<30 && ${bln_needsCure}) /goto :wait_loop
		/nav loc ${str_navCureStart}
	}
/return

Sub EVENT_ResetGroupCurer 
	/call getGroupCurer DEFAULT
	/varset str_myCurer ${Macro.Return}
/return 

|-- Will get your curer, prefers shaman -> cleric -> druid -> paladin
Sub getGroupCurer(string _cureType)
	/echo \aggetGroupCurer \ao ${_cureType}
	/if (${_cureType.Equal[NULL]}) /varset  _cureType NOTSET
	/declare 	myCurer			string	local	NOTFOUND
	/declare	gmember			int 	local	0
	/declare 	groupShaman		string 	local 	NOTFOUND
	/declare 	groupCleric     string  local 	NOTFOUND
	/declare 	groupPaladin    string  local 	NOTFOUND
	/declare 	groupDruid    	string  local 	NOTFOUND
	/if (${str_classArchetype.Equal[PRIEST]} || ${Me.Class.ShortName.Equal[PAL]}) {
		/varset myCurer ${Me.Name}
	} else {
		/for gmember 0 to ${Group} 
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[SHM]} && ${SpawnCount[pc ${Group.Member[${gmember}]}]} > 0) /varset groupShaman  ${Group.Member[${gmember}]}
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[CLR]} && ${SpawnCount[pc ${Group.Member[${gmember}]}]} > 0) /varset groupCleric  ${Group.Member[${gmember}]}
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[PAL]} && ${SpawnCount[pc ${Group.Member[${gmember}]}]} > 0) /varset groupPaladin ${Group.Member[${gmember}]}
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[DRU]} && ${SpawnCount[pc ${Group.Member[${gmember}]}]} > 0) /varset groupDruid   ${Group.Member[${gmember}]}
		/next gmember
	}
	
	/echo \awSHM:\ag${groupShaman} \awCLR:\ag${groupCleric} \awPAL:\ag${groupPaladin} \awDRU:\ag${groupDruid}

	/if (${_cureType.Equal[DEFAULT]}) {
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[PAL]}) {
			/varset myCurer ${Me.Name}
		} else {
			| Order of precedence for default curing SHM,DRU,PAL,CLR
			/if (${groupShaman.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupShaman}
			} else /if (${groupDruid.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupDruid}
			} else /if (${groupPaladin.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupPaladin}
			} else /if (${groupCleric.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupCleric}
			}
		}

	} else /if (${_cureType.Equal[poison]}) {
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[PAL]}) {
			/varset myCurer ${Me.Name}
		} else {
			| Order of precedence for default curing CLR,PAL,DRU,SHM
			/if (${groupCleric.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupCleric}
			} else /if (${groupPaladin.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupPaladin}
			} else /if (${groupDruid.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupDruid}
			} else /if (${groupShaman.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupShaman}
			}
		}
	} else /if (${_cureType.Equal[disease]}) {
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[PAL]}) {
			/varset myCurer ${Me.Name}
		} else {
			| Order of precedence for default curing SHM,PAL,DRU,CLR
			/if (${groupShaman.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupShaman}
			} else /if (${groupPaladin.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupPaladin}
			} else /if (${groupDruid.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupDruid}
			} else /if (${groupCleric.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupCleric}
			}
		}
	} else /if (${_cureType.Equal[curse]}) {
		/if (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[PAL]}) {
			/varset myCurer ${Me.Name}
		} else {
			| Order of precedence for default curing NO precedence since everyon has RGC, make the druid do it though!
			/if (${groupDruid.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupDruid}
			} else /if (${groupPaladin.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupPaladin}
			} else /if (${groupDruid.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupDruid}
			} else /if (${groupCleric.NotEqual[NOTFOUND]}) {
				/varset myCurer ${groupCleric}
			}
		}
	}
	/if (${myCurer.Equal[NOTFOUND]} && !${timer_cure_complain}) {
		/docommand /${announceChannel} No curer found!
		/varset timer_cure_complain 30s
	} else {
		/echo \aggetGroupCurer: \ao${myCurer}  \aw(Cure Type: \ag${_cureType}\aw)
	}
/return ${myCurer}


Sub cureGroup(string _cureType, int _counters)
	/if (${bln_chChainMember} || ${bln_focusHealMT}) /return
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/g Curing Group ${_cureType} ${_counters}
	| Cure Group with Group Cure
	/declare ct string local ${_cureType}
	/declare cnt int local ${_counters}
	/if (${_cureType.Equal[disease]} && ${is_disease_group}) {
		/call handleCureRequest ${Me.Name} ${Me.ID} ${ct} ${cnt}
	} else /if (${_cureType.Equal[posion]} && ${is_poison_group}) {
		/call handleCureRequest ${Me.Name} ${Me.ID} ${ct} ${cnt}
	} else /if (${_cureType.Equal[curse]} && ${is_curse_group}) {
		/call handleCureRequest ${Me.Name} ${Me.ID} ${ct} ${cnt}
	} else /if (${_cureType.Equal[corruption]} && ${is_corruption_group}) {
		/call handleCureRequest ${Me.Name} ${Me.ID} ${ct} ${cnt}
	} else {
		| Cure Group by Each
		/declare	gmember			int local	0
		/declare    counter         int local   0
		/declare 	qs				string local NULL
		/call getDebuffPluginQuery ${_cureType}
		/varset qs ${Macro.Return}
		/for gmember 0 to ${Group}
			/echo \aoCureGroup: \ag${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} ${ct} ${cnt}
			| If I am a cleric, quickly check MT health
			/if (${Me.Class.ShortName.Equal[CLR]} ) /call chkHeal_MT
			| Check to see if the toon actually needs to be cured, else skip
			/call getCureCounters ${Group.Member[${gmember}].Name} ${qs}
			/varset counter ${Macro.Return}
			/if (${counter}>0) /call handleCureRequest ${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} ${ct} ${cnt}
			/call RunRaidEvents
		/next gmember
	}
/return 

Sub cureMTAuto
	| Specifically for a cleric or other curer to constantly watch the MT and auto cure them
	/declare c int local 0

	| Check Conspecifics
	| Packmaster's Curse
	/varset c 0
	/dquery ${str_MTName} -q "Me.Buff[Packmaster’s Curse].ID"
	/delay 20 ${DanNet.Query.Received}
	/varset c ${DanNet.Q}
	/if (${c}) {
		/call handleCureRequest ${str_MTName} ${int_MTID} curse 16 TRUE
		/goto :return
	}

	| Check Poison
	/varset c 0
	/dquery ${str_MTName} -q "Debuff.Poisoned"
	/delay 20 ${DanNet.Query.Received}
	/varset c ${DanNet.Q}
	/if (${c}) {
		/call handleCureRequest ${str_MTName} ${int_MTID} poison ${c} TRUE
		/goto :return
	}


	| Check Curse
	/varset c 0
	/dquery ${str_MTName} -q "Debuff.Cursed"
	/delay 20 ${DanNet.Query.Received}
	/varset c ${DanNet.Q}
	/if (${c}) {
		/call handleCureRequest ${str_MTName} ${int_MTID} curse ${c} TRUE
		/goto :return
	}

	| Check Disease
	/varset c 0
	/dquery ${str_MTName} -q "Debuff.Diseased"
	/delay 20 ${DanNet.Query.Received}
	/varset c ${DanNet.Q}
	/if (${c}) {
		/call handleCureRequest ${str_MTName} ${int_MTID} disease ${c} TRUE
		/goto :return
	}
	:return
/return

Sub cureSelf(string _cureType, int _counters)
	/g Curing Self ${_cureType} ${_counters}
	| Cure Self
	/call handleCureRequest ${Me.CleanName} ${Me.ID} ${_cureType} ${_counters} TRUE
/return

Sub chkHasCureForType(string _ct)
	/if (${_ct.Equal[disease]}	&& ${bln_hasDiseaseCure})	/return TRUE
	/if (${_ct.Equal[poison]}	&& ${bln_hasPoisonCure})	/return TRUE
	/if (${_ct.Equal[curse]}	&& ${bln_hasCurseCure})		/return TRUE
	/if (${_ct.Equal[corruption]} && ${bln_hasCorruptionCure}) /return TRUE
/return FALSE

| Handle Incoming cure request.
| TODO: If cleric, check MT hp before healing unless FORCED is true.
Sub handleCureRequest(string _name, int _id, string _cureType, int _counters, bool _forced) 
	/echo \ayhandleCureRequest: ${_name}, ${_id}, ${_cureType}, ${_counters}, ${_forced}
	|-- if requester is not in zone, ignore it
	/if (!${SpawnCount[pc ${_name}]}) {
		/echo \arI cannot cure \aw${_name}, \arthey are dead
		/dt ${_name} I cannot cure you, you are dead!
		/return
	}

	/call chkHasCureForType ${_cureType}
	/if (!${Macro.Return}) {
		/echo \arI cannot cure \aw${_cureType}
		/dt ${_name} I cannot cure you of ${_cureType}
		/return
	}

	/declare	str_spell	  	string		local	${str_cure_disease}
	/declare    int_count     	int			local   ${_counters}
	
	

	|-- check that we can cure this type
	/if (${_cureType.Equal[disease]}) {
		/echo \aoDISEASE CURE selected.
		/if (!${bln_hasDiseaseCure}) {
			/echo I cannot cure Disease
			/dt ${_name} I cannot cure you
			/return
		}
		/varset str_spell 	${str_cure_disease}
		/varset int_count 	${int_cure_disease}
	} else /if (${_cureType.Equal[poison]}) {
		/echo \agPOISON CURE selected.
		/if (!${bln_hasPoisonCure}) {
			/echo I cannot cure Poison
			/dt ${_name} I cannot cure you
			/return
		}			
		/varset str_spell 	${str_cure_poison}
		/varset int_count 	${int_cure_poison}
	} else /if (${_cureType.Equal[curse]}) {
		/echo \ayCURSE CURE selected.
		/if (!${bln_hasCurseCure}) {
			/echo I cannot cure Curses
			/dt ${_name} I cannot cure you
			/return
		}	
		/varset str_spell 	${str_cure_curse}
		/varset int_count 	${int_cure_curse}
	} else /if (${_cureType.Equal[corruption]}) {
		/echo \apCORRUPTION CURE selected.
		/if (!${bln_hasCorruptionCure}) {
			/echo I cannot cure Corruption
			/dt ${_name} I cannot cure you
			/return
		}	
		/varset str_spell 	${str_cure_corruption}
		/varset int_count 	${int_cure_corruption}
	}
	
	|-- find cure spell string & counters it clears

	/declare    qstring  	  	string   	local
	/declare 	c 				int 		local	0

	/call getDebuffPluginQuery ${_cureType}
	/varset qstring ${Macro.Return}

	| If I am a cleric and this is unforced, quickly check MT health
	/if (${Me.Class.ShortName.Equal[CLR]} && !${_forced}) /call chkHeal_MT

	|-- Get the range of this cured
	/call setCureRange ${_cureType}

	| If cure target is out of range, return
	/if (${Spawn[pc ${_name}].Distance} > ${Spell[${str_spell}].MyRange}) {
		/echo \ar ${_name} \awis out of cure range! \ao${Spawn[pc ${_name}].Distance} > ${cureRange}
		/return
	}

	|-- Sanity check their cure request with a query *(if we arent forced!)
	:counterCheck
	/if (!${_forced}) {
		/echo Verifying Need ${_name} ${qstring}
		/call getCureCounters ${_name} "${qstring}"
		/varset c ${Macro.Return}
		/if (!${c}) {
			/echo \ao${_name} \agdid NOT need curing, returning
			/goto :return
		} else {
			/echo \ag${_name} \awNEEDS curing ${c}, continuing
		}
	}

	
	|-- calculate how many times we need to cast the cure counters: note MQ2 Natively does not have a round method
	/declare    int_castCount 	float    	local   1
	/varset int_castCount ${Math.Calc[${_counters} / ${int_count}]}
	/if (${int_castCount} < 1) {
		/varset int_castCount 1
	} else /if (${int_castCount} > 1 && ${int_castCount} < 2) {
		/varset int_castCount 2
	} else /if (${int_castCount} > 2 && ${int_castCount} < 3) { 
		/varset int_castCount 3
	} else /if (${int_castCount} > 3 && ${int_castCount} < 4) { 
		/varset int_castCount 4
	} else /if (${int_castCount} > 4 && ${int_castCount} < 5) { 
		/varset int_castCount 5
	} else /if (${int_castCount} > 5 && ${int_castCount} < 6) { 
		/varset int_castCount 5
	}
	/echo \ar${str_spell} casting: ${int_castCount}x times (${_counters} / ${int_count})

	| need to mem curespell if I dont have it up
	/target id ${_id}
	:cureReadyWait1
	/call chkSpellMemorized "${str_spell}" TRUE
	/if (${Me.SpellReady[${str_spell}]} && (${Target.Distance}<=200)) {
		/call castCure ${_name} ${_id} "${str_spell}" ${int_castCount} ${_cureType} ${_counters} ${_forced}
	} else {
		/goto :cureReadyWait1
	}
	:return
/return


|${Group.Member[${gm}].Name} ${Group.Member[${gm}].ID} disease ${gcount[${i}]} TRUE
Sub handleCureMarkOfDeath(string _name, int _id, bool _group, int _dCount) 
	|/1	handleCureMarkOfDeath(${_name}) Group? ${_group} Counters? ${_dCount}
	/echo \a-w      handleCureMarkOfDeath\aw(\ag${_name}\aw) Group? \ag${_group} \awCounters? \ag${_dCount}
	/declare str_cureToUse 		string   local ${str_cure_disease}
	/declare int_countersCured 	int      local 18
	/declare int_startCount   	float    local 1
	/declare int_finalCount     float    local 1

	/varcalc int_startCount ${Math.Abs[${Math.Calc[${_dCount} / ${int_countersCured}]}]}
	/varcalc int_finalCount ${Math.Abs[${Math.Calc[${int_startCount} / ${int_groupClerics}]}]}

	/varcalc int_finalCount ${int_finalCount} * 100
	/varcalc int_finalCount ${int_finalCount} + 0.5
	/varset  int_finalCount ${Int[${int_finalCount}]}
	/varcalc int_finalCount ${Int[${Math.Calc[${int_finalCount}/100 + 1]}]}

	| Note since the server is janky and not calculating counters except for the initial counter we
	| have to cast more than once. All clerics in group will cast at least once.
	
	/doevents GazeYou
	/if (${_group} && ${Me.Class.ShortName.Equal[CLR]}) /varset str_cureToUse ${str_heal_group}
	/echo \ao CastCure =>\awstr_cureToUse \ag${str_cureToUse} \awint_startCount: \ag${int_startCount} \awint_groupClerics: \ag${int_groupClerics} \awint_finalCount: \ag${int_finalCount}
	| recheck one last time so we don't double this up!
	/if (!${_group}) {
		/declare i int local 0
		/sqlite advquery CureMarkOfDeath docure SELECT * FROM MarkOfDeath WHERE SpawnID = ${_id};
		/if (!${sqlite.rows[docure]}) {
			/shout Already cured ${_name} ${_id}
			/return
		}
	} 
	/call castCure ${_name} ${_id} "${str_cureToUse}" ${int_finalCount} disease ${_dCount} TRUE
	/call TrueTarget ${int_MTID}
/return 


|-- Casts a cure, for _count many times, on a _tgtID
Sub castCure(string _name, int _id, string _spell, int _castCount, string _cureType, int _counters, bool _isForced) 
	/echo \aoCastCure \aw${_name}, ${_id}, ${_spell}, ${_castCount}, ${_cureType}, ${_counters}, ${_isForced}
	
	/declare i 			int local 1
	/declare c 			int local 0
	/declare qs 		string 	local	NULL
	/if (${Me.Casting.ID}) /call Interrupt


	/for i 1 to ${_castCount}
		/echo \agCasting ${i} =>  ${_castCount}
		|Check for Death
		/if (!${SpawnCount[pc ${_name}]}) {
			/dt ${_name} I cannot cure you, you are dead!
			/return
		}

		|-- Sanity check their cure request with a query *(if we arent forced!)
		:counterCheck
		/if (!${_isForced}) {
			/call getDebuffPluginQuery ${_cureType}
			/call getCureCounters ${_name} "${Macro.Return}"
			/varset c ${Macro.Return}
			/if (!${c}) {
				/echo \ao${_name} \agdid NOT need further ${qs} curing, returning
				/goto :return
			} else {
				/echo \ag${_name} \awNEEDS curing ${c}, continuing
			}
		}

		| If I am a cleric and this is unforced, quickly check MT health
		/if (${Me.Class.ShortName.Equal[CLR]} && !${_isForced}) /call chkHeal_MT
		/call TrueTarget ${_id}
		/doevents GazeYou
		/call MQ2Cast "${_spell}" gem4 8s
	/next i 
	:return
	/dt ${_name} I cured you ${_name}(${_id}) of ${_counters} ${_cureType} counters.
/return


Sub setCureRange(string _cureType) 
	/if (${_cureType.Equal[poison]}) {]
		/varset cureRange ${Spell[${str_cure_poison}].MyRange}
	} else /if (${_cureType.Equal[curse]}) { 
		/varset cureRange ${Spell[${str_cure_curse}].MyRange}
	} else /if (${_cureType.Equal[disease]}) { 
		/varset cureRange ${Spell[${str_cure_disease}].MyRange}
	} else /if (${_cureType.Equal[corruption]}) {
		/varset cureRange ${Spell[${str_cure_corruption}].MyRange} 
	}

/return 

Sub getDebuffPluginQuery(string _ct)
	/declare q string local NULL
	/if (${_ct.Equal[disease]}) /varset q Debuff.Diseased
	/if (${_ct.Equal[curse]})   /varset q Debuff.Cursed
	/if (${_ct.Equal[poison]})  /varset q Debuff.Poisoned
	/echo \a-w		getDebuffPluginQuery\aw(\ag${_ct} \aw${q}\aw)
/return ${q}

|-- Use DanNet to get the counters required
|   Query via Dannet
Sub getCureCounters(string _pcName, string _qs) 
	/declare cc int local 0
	/dquery ${_pcName} -q "${_qs}" -o cc
	/delay 20 ${DanNet.Query.Received}
	/echo \a-w		getCureCounters\aw(\ag${_pcName}\aw, \ag${_qs}\aw) == \ag${cc}
/return ${cc}

Sub xbot_initialize_curing
	/echo \ao xbot_initialize_curing
	/declare 	int_manadrain_threshold int		outer 	100
	/declare	int_hpdrain_threshold	int		outer	300
	/declare 	bln_needsCure 			bool	outer	FALSE
	/declare    bln_askedForCure        bool    outer   FALSE
	/declare 	bln_autoCureMT			bool    outer   FALSE
	/declare    str_myCurer				string	outer 	NULL
	/declare	timer_cure_request		timer   outer
	/declare  	bln_useCures			bool    outer   TRUE
	/declare 	timer_cureCheck			timer 	outer 	10s

	| Temporary work around
	/if (!${Defined[bln_jelvanEngaged]}) /declare bln_jelvanEngaged bool outer FALSE
	
	/declare 	cureRange     			int   	outer   100
	/declare    bln_navCure   			bool    outer   FALSE
	/declare    str_navCureStart  		string  outer 
	/declare 	timer_cure_complain		timer   outer   0

	/declare    str_cure_curse			string  outer   ${Ini[INI_CHAR,Curing,str_cure_curse,"Remove Greater Curse"]}
	/declare    int_cure_curse			string  outer   ${Ini[INI_CHAR,Curing,int_cure_curse,36]}
	/declare    is_curse_group          bool    outer   ${Ini[INI_CHAR,Curing,is_curse_group, FALSE]}
	
	/declare    str_cure_corruption		string  outer   ${Ini[INI_CHAR,Curing,str_cure_corruption,"NULL"]}
	/declare    int_cure_corruption		string  outer   ${Ini[INI_CHAR,Curing,int_cure_corruption,36]}
	/declare    is_corr_group           bool    outer   ${Ini[INI_CHAR,Curing,is_corr_group, FALSE]}
	
	/declare    str_cure_poison			string  outer   ${Ini[INI_CHAR,Curing,str_cure_poison,"Antidote"]}
	/declare    int_cure_poison			string  outer   ${Ini[INI_CHAR,Curing,int_cure_poison,64]}
	/declare    is_poison_group         bool    outer   ${Ini[INI_CHAR,Curing,is_poison_group, FALSE]}
	
	/declare    str_cure_disease		string  outer   ${Ini[INI_CHAR,Curing,str_cure_disease,"Pure Blood"]}
	/declare    int_cure_disease		string  outer   ${Ini[INI_CHAR,Curing,int_cure_disease,18]}
	/declare    is_disease_group        bool    outer   ${Ini[INI_CHAR,Curing,is_disease_group, FALSE]}
	
	/declare    str_secondary_radiant	string	outer   ${Ini[INI_CHAR,Curing,str_secondary_radiant,"NULL"]} 
	

	/declare	bln_hasDiseaseCure		bool	outer	TRUE
	/declare	bln_hasPoisonCure		bool	outer	TRUE
	/declare 	bln_hasCurseCure		bool	outer	TRUE
	/declare 	bln_hasCorruptionCure	bool	outer	FALSE 
	
	
	/if (${str_cure_curse.Equal[NULL]}) 	/varset bln_hasCurseCure 	FALSE
	/if (${str_cure_poison.Equal[NULL]}) 	/varset bln_hasPoisonCure 	FALSE
	/if (${str_cure_disease.Equal[NULL]}) 	/varset bln_hasDiseaseCure 	FALSE
	
	/call 		getGroupCurer DEFAULT
	/varset 	str_myCurer ${Macro.Return}
	
	/call InitEvents_Curing
	
/return