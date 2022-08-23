|**
----------------------------
CURING DEBUFF HANDLERS updated version 07.28.22
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


Sub InitEvents_Curing
	/if (${str_classArchetype.Equal[PRIEST]} || ${Me.Class.ShortName.Equal[PAL]}) /call RegisterClassEvents Events_Curing 5
	/call RegisterBackgroundEvents Events_Curing_Background 2
/return

Sub Events_Curing
	/doevents cureMe
	/doevents cureRequest
	/doevents cureGroupPoison
	/doevents cureGroupDisease
	/doevents cureGroupCurse
/return

Sub Events_Curing_Background
	/doevents cureReceived
	/doevents cureRejected
/return

|-- Single target cure, simple syntax (for manual entry)
|   Counters determined by NetCure
Sub Event_cureMe(_line, _sender, _eventParams)
	/echo EVENT_CUREME ${_sender} type: ${_eventParams}
	|string _pcName, string _type
	/declare name string local ${_sender}
	/declare curetype string local ${_eventParams}
	/call getCureCounters ${name} ${curetype}
	/declare counters int local ${Macro.Return}
	/call handleCureRequest ${_sender} ${Spawn[pc name ${pcName}].ID} ${curetype} ${counters}
	/doevents flush cureMe
/return


|-- Single target cure, generated syntax (for macro call)
|   Counters determined by DanNet query
Sub Event_cureRequest(_line, _sender, _eventParams)	
	/declare name string local ${_eventParams.Arg[1, ]}
	/declare id int local ${_eventParams.Arg[2, ]}
	/declare curetype string local  ${_eventParams.Arg[3, ]}
	/call getCureCounters ${name} ${curetype}
	/declare counters int local  ${_eventParams.Arg[4, ]}
	/declare forced bool local ${_eventParams.Arg[5, ]}

	/if (${counters} == 0) /varset counters ${Macro.Return}
	/cecho \awCure Request \ayname: \ag${name}(${id}) \aytype: \ag${curetype} \aycounters: \ag${counters} \ayforced: \ag${forced}
	/doevents flush cureRequest

	/call handleCureRequest ${name} ${id} ${curetype} ${counters} ${forced}
	
/return

Sub Event_cureReceived(_line, _sender, _eventParams)
	/declare id int local ${Int[${_eventParams}]}
	/if (${id} == ${Me.ID}) /call resetCureState
	/doevents flush cureReceived
/return

Sub Event_cureGroupPoison(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGPoison: _line:${_line}
	/doevents flush
	/echo _sender: ${_sender}
	/echo _eventParams:${_eventParams}
	/rs Curing Group of Poisons with: ${str_cure_poison}
	/call cureGroup poison ${_eventParams}
	/doevents flush cureGroupPoison
/return

Sub Event_cureGroupDisease(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGDisease: _line:${_line}
	/doevents flush
	/rs Curing Group of Disease with: ${str_cure_disease}
	/call cureGroup disease ${_eventParams}
	/doevents flush cureGroupDisease
/return

Sub Event_cureGroupCurse(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} &&${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGCurse: _line:${_line}
	/doevents flush
	/rs Curing Group of Curses with: ${str_cure_curse}
	/call cureGroup curse ${_eventParams}
	/doevents flush cureGroupCurse
/return

Sub Event_cureGroupCorruptiuon(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} &&${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGCoruption: _line:${_line}
	/doevents flush
	/rs Curing Group of Corruption with: ${str_cure_corruption}
	/call cureGroup corruption ${_eventParams}
	/doevents flush Event_cureGroupCorruptiuon
/return

Sub resetCureState
	/cecho \agCure State Reset
	/varset bln_askedForCure 	FALSE
	/varset bln_needsCure		FALSE
	/varset timer_cure_request  10s
	/varset bln_navCure			FALSE
/return 




|-- Check if I need a cure (all classes)
Sub chkNeedCure
	|-- Verify that I actually have debuffs even if it says i need to cure, reset that
	/if (!${Debuff} && ${bln_needsCure}) /call resetCureState
	|-- Verify need if I havent asked and recently
	/if (${bln_askedForCure} == TRUE && ${timer_cure_request}) 	/return
	|-- Verify debuff counters
	/if (${Debuff.Count} == 0 || ${Debuff.Counters} > 200) /return
	|-- Verify ability to be cured
	/if (${str_myCurer.Equal[NULL]}) 	/return
	|-- Verify I am not tanking
	/if (${Spawn[pc ${str_myCurer}].Distance3D} > 100 && ${bln_tanking}) /return
	
	|-- Force the cure?
	/declare forceCure		bool 	local	FALSE
	/declare cureType 		string 	local 	NOTSET
	/declare counters 		int 	local
	

	|-- Check for Forcings
	|-- If I am tanking, and I have under 100 counters (some debuffs are uncureable) or we aren't engaged, then force the cure request
	/if (${bln_tanking} && (${Debuff.Counters} < 100 && ${Debuff.Count}) || !${bln_engaged}) /varset bln_needsCure TRUE
	
	|-- Mana Drains (casting classes)
	/if ((${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[PAL]} || ${Me.Class.ShortName.Equal[SHD]}) && (${Debuff.ManaDrain} >= ${int_manadrain_threshold}) ) /varset bln_needsCure TRUE

	|-- HP Drains
	/if  (${Debuff.HPDrain} >= ${int_hpdrain_threshold}) /varset bln_needsCure TRUE

	|-- Slows (melee)
	/if (${Debuff.Slowed} && (${Debuff.Counters} < 100 )) /varset bln_needsCure TRUE

	
	/if (${Me.Buff[Mark of Death].ID}) {
		|-- Mark of Death must be cured!
		/echo I have [Mark of Death], a debuff that my ${str_myCurer} will already be handling~
		/cecho \ar Mark of Death \ay(${Debuff.Diseased})
		/call resetCureState
		/goto :return

	} else /if (${Me.Buff[Gaze of Anguish].ID}) {
		|-- Gaze of Anguish must be cured!
		/varset cureType disease
		/varset counters ${Debuff.Diseased}
		/varset forceCure TRUE

	} else /if (${Me.Buff[Wanton Destruction].ID}) {
		|-- Wanton Destruction must be cured!
		/echo I have [Wanton Destruction], a debuff that I must have cured!
		/varset cureType curse
		/varset counters ${Debuff.Cursed}
		/varset forceCure TRUE

	} else /if (${Me.Buff[Feedback Dispersion].ID} && ${str_classArchetype.Equal[PRIEST]}) {
		|-- Feedback Dispersion must be cured!
		/echo I have [Feedback Dispersion], a debuff that I must cure myself!
		/varset cureType curse
		/varset counters ${Debuff.Cursed}
		/varset forceCure TRUE

	} else /if (${Me.Buff[Chains of Anguish].ID}) {
		|-- Chains of Anguish cannot be cured!
		/echo I have [Chains of Anguish], a debuff that I cannot cure!
		/call resetCureState
		/goto :return

	} else /if (${Me.Buff[Aura of Destruction].ID}) {
		|-- Aura of Destruction cannot be cured!
		/echo I have [Aura of Destruction], a debuff that I cannot cure!
		/call resetCureState
		/goto :return

	} else /if (${Me.Buff[Rigidity].ID} || ${Me.Buff[Curse of Misfortune].ID} || ${Me.Buff[Imprecision].ID} || ${Me.Buff[Impurity].ID}) {
		|-- Specialization Trials debuffs should not be cured (wastes time/mana)!
		/echo I have a [MPG Specialization Debuff], a debuff that I cannot cure!
		/call resetCureState
		/goto :return
	} else /if (${Me.Buff[Ressurection Sickness].ID}) {
		|-- Aura of Destruction cannot be cured!
		/echo I have [Resurrection Sickness], a debuff that I cannot cure!
		/call resetCureState
		/goto :return
	}
	
	:cure_determine
	/if ( ${forceCure} || ${bln_needsCure}) {
		/varset bln_needsCure TRUE
		
		|-- check to see if my curer is still alive
		/if (${SpawnCount[PC ${str_myCurer}]} == 0) {
			:new_curer
			/call getGroupCurer
			/varset str_myCurer ${Macro.Return}
		} 

		/if (${str_myCurer.Equal[NULL]}) {
			/cecho \ar I HAVE NO CURER!
			/call resetCureState
			/goto :return
		}

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

		/cecho \arCURE REQUIRED:\ay Counters:\ag${counters} \ayType:\ag${cureType}

		/call getCureRange curse
		/varset cureRange ${Int[${Macro.Return}]}	
		/echo cureRange: ${Macro.Return} ${cureRange}
		
		/if (${cureType.NotEqual[NOTSET]}) {
			/if (${str_myCurer.Equal[${Me.Name}]}) {
				/call handleCureRequest ${Me.Name} ${Me.ID} ${cureType} ${counters} ${forceCure}
			} else {
				/if (${Bool[${Spawn[pc ${str_myCurer}].ID}]}) {
					/cecho \aoCurer:\ag${str_myCurer} \aoDist:\ag${Spawn[pc ${str_myCurer}].Distance3D} \aoRange:\ag${cureRange}
					/if (${Spawn[pc ${str_myCurer}].Distance3D} < ${cureRange}) {
						/cecho \arCURE REQUESTING:\ay Counters:\ag${counters} \ayType:\ag${cureType} \ayForced:\ag${forceCure}
						/call requestCure ${cureType} ${counters} ${forceCure}
					} else /if (${Spawn[pc ${str_myCurer}].Distance3D} > ${cureRange} && ${Spawn[pc ${str_myCurer}].Distance3D} < 250 && !${bln_tanking}) {
						/varset str_navCureStart ${Me.X} ${Me.Y} ${Me.Z}
						/cecho \awNavigating to Curer because range is too far for cure
						/varset bln_navCure TRUE
						/navigate spawn ${str_myCurer}
						:nav_curer
						/if (${Spawn[pc ${str_myCurer}].Distance3D} < ${cureRange}) /nav stop
						/if (${Navigation.Active}) /goto :nav_curer
						/cecho \arCURE REQUESTING:\ay Counters:\ag${counters} \ayType:\ag${cureType} \ayForced:\ag${forceCure}
						/call requestCure ${cureType} ${counters} ${forceCure}
					} else {
						/cecho \arMy Curer is too far away
						/goto :new_curer
					}
				} else {
					/cecho \arMy Curer is dead or not in zone
					/goto :new_curer
				}
			}
			/varset timer_cure_request 30s
			/varset bln_askedForCure TRUE
		}
	}
:return
/return

Sub requestCure(string _type, int _counters, bool _force) 
	|/shout REQUESTED CURE from ${str_myCurer} ${_type} ${_counters} ${_force}
	/dt ${str_myCurer} cureRequest ${Me.Name} ${Me.ID} ${_type} ${_counters} ${_force}
	/declare waitCounter int local 0
	/if (${bln_navCure}) {
		/cecho \awWaiting for cure since my initial spot is outside of cure range - \ag${cureRange}
		:wait_loop
		/doevents cureReceived
		/varcalc waitCounter ${waitCounter}+1
		/delay 10
		/if (${waitCounter}<30 && ${bln_needsCure}) /goto :wait_loop
		/nav loc ${str_navCureStart}
	}
/return

|-- Will get your curer, prefers shaman -> cleric -> druid -> paladin
Sub getGroupCurer 
	/declare 	myCurer			string	local	NULL
	/declare	gmember			int 	local	0
	/declare 	groupShaman		string 	local 	NULL
	/declare 	groupCleric     string  local 	NULL
	/declare 	groupPaladin    string  local 	NULL
	/declare 	groupDruid    	string  local 	NULL
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
	
	/cecho \awSHM:\ag${groupShaman} \awCLR:\ag${groupCleric} \awPAL:\ag${groupPaladin} \awDRU:\ag${groupDruid}

	| curer order of precedence
	/if (${groupDruid.NotEqual[NULL]}) 		/varset myCurer ${groupDruid}
	/if (${groupCleric.NotEqual[NULL]}) 	/varset myCurer ${groupCleric}
	/if (${groupPaladin.NotEqual[NULL]}) 	/varset myCurer ${groupPaladin}
	/if (${groupShaman.NotEqual[NULL]}) 	/varset myCurer ${groupShaman}
	/cecho \aggetGroupCurer: \ao${myCurer}
	
/return ${myCurer}

Sub getCureRange(string _cureType) 
	/if (${_cureType.Equal[poison]}) {
		/if (${Spawn[${str_myCurer}].Class.ShortName.Equal[SHM]}) /return 100 
		/if (${Spawn[${str_myCurer}].Class.ShortName.Equal[PAL]}) /return 100 
		/if (${Spawn[${str_myCurer}].Class.ShortName.Equal[DRU]}) /return 100 
		/return 200
	} else /if (${_cureType.Equal[curse]}) { 
		/return 100
	} else /if (${_cureType.Equal[disease]}) { 
		/return 100
	} else /if (${_cureType.Equal[corruption]}) { 
		/return 100
	}

/return 

|-- Use DanNet to get the counters required
|   Query via Dannet
Sub getCureCounters(string _pcName, string _type) 
	/declare c int local 0
	/if (${_type.Equal[poison]}) {
		/dquery ${_pcName} -q "Me.Poisoned"
	} else /if (${_type.Equal[curse]}) {
		/dquery ${_pcName} -q "Me.Cursed"
	} else /if (${_type.Equal[disease]}) {
		/dquery ${_pcName} -q "Me.Diseased"
	}
	/delay 20 ${DanNet.Query.Received}
	/varset c ${DanNet.Q}
/return ${c}

Sub cureGroup(string _cureType, int _counters)
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
		/for gmember 0 to ${Group}
			/echo handling: ${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} ${ct} ${cnt}
			| Check to see if the toon actually needs to be cured, else skip
			/call getCureCounters ${Group.Member[${gmember}].Name} ${_cureType}
			/varset counter ${Int[${Macro.Return}]}
			/if (${counter}>0) /call handleCureRequest ${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} ${ct} ${cnt}
			/call RunRaidEvents
		/next gmember
	}
/return 


Sub cureSelf(string _cureType, int _counters)
	/g Curing Self ${_cureType} ${_counters}
	| Cure Self
	/call handleCureRequest ${Me.CleanName} ${Me.ID} ${_cureType} ${_counters}
/return


Sub handleCureRequest(string _name, int _id, string _cureType, int _counters, bool _forced) 
	/cecho \ayhandleCureRequest: ${_name}, ${_id}, ${_cureType}, ${_counters}
	|-- find cure spell string & counters it clears
	/declare	str_spell	  string	local	${str_cure_disease}
	/declare    int_count     int		local   ${_counters}
	/declare    int_castCount float     local   1

	/if (!${Bool[${_counters}]}) {
		/dt ${_name} I cannot cure you, you sent no counters!
		/return
	}

	/if (${_cureType.Equal[disease]}) {
		/cecho \aoDISEASE CURE selected.
		/if (!${bln_hasDiseaseCure}) {
			/echo I cannot cure Disease
			/dt ${_name} I cannot cure you
			/return
		}
		/varset str_spell 	${str_cure_disease}
		/varset int_count 	${int_cure_disease}
	} else /if (${_cureType.Equal[poison]}) {
		/cecho \agPOISON CURE selected.
		/if (!${bln_hasPoisonCure}) {
			/echo I cannot cure Poison
			/dt ${_name} I cannot cure you
			/return
		}			
		/varset str_spell 	${str_cure_poison}
		/varset int_count 	${int_cure_poison}
	} else /if (${_cureType.Equal[curse]}) {
		/cecho \ayCURSE CURE selected.
		/if (!${bln_hasCurseCure}) {
			/echo I cannot cure Curses
			/dt ${_name} I cannot cure you
			/return
		}	
		/varset str_spell 	${str_cure_curse}
		/varset int_count 	${int_cure_curse}
	} else /if (${_cureType.Equal[corruption]}) {
		/cecho \apCORRUPTION CURE selected.
		/if (!${bln_hasCorruptionCure}) {
			/echo I cannot cure Corruption
			/dt ${_name} I cannot cure you
			/return
		}	
		/varset str_spell 	${str_cure_corruption}
		/varset int_count 	${int_cure_corruption}
	}
	
	|-- calculate how many times we need to cast the cure counters: note MQ2 Natively does not have a round method
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
	/cecho \ar${str_spell} casting: ${int_castCount}x times (${_counters} / ${int_count})

	| need to mem curespell if I dont have it up
	/target id ${_id}
	:cureReadyWait1
	/call chkSpellMemorized "${str_spell}" TRUE
	/if (${Me.SpellReady[${str_spell}]} && (${Target.Distance}<=200)) {
		/call castCure ${_name} ${_id} "${str_spell}" ${int_castCount} ${_cureType}
	} else {
		/goto :cureReadyWait1
	}
/return

|-- Casts a cure, for _count many times, on a _tgtID
Sub castCure(string _name, int _id, string _spell, int _castCount, string _cureType, int _counters) 
	/cecho \aw CastCure ${_name}, ${_id}, ${_spell}, ${_count}
	/call TrueTarget ${_id}
	/declare i int local 1
	/for i 1 to ${_castCount}
		/cecho \aw Casting ${_spell}
		/call MQ2Cast "${_spell}" gem7 8s
		/delay 10
	/next i 
	/dt ${_name} I cured you ${_name}(${_id}) of ${_count} ${_cureType} counters.
/return


Sub xbot_initialize_curing

	/declare 	int_manadrain_threshold int		outer 	150
	/declare	int_hpdrain_threshold	int		outer	300
	/declare 	bln_needsCure 			bool	outer	FALSE
	/declare    bln_askedForCure        bool    outer   FALSE
	/declare    str_myCurer				string	outer 	NULL
	/declare	timer_cure_request		timer   outer
	/call 		getGroupCurer
	/varset 	str_myCurer ${Macro.Return}
	/declare 	cureRange      int     outer   100
	/declare    bln_navCure   			bool    outer   FALSE
	/declare    str_navCureStart  		string  outer 

	/declare    str_cure_curse			string  outer   ${Ini[botINI,Curing,str_cure_curse,"Remove Greater Curse"]}
	/declare    int_cure_curse			string  outer   ${Ini[botINI,Curing,int_cure_curse,36]}
	/declare    is_curse_group          bool    outer   ${Ini[botINI,Curing,is_curse_group, FALSE]}
	
	/declare    str_cure_corruption		string  outer   ${Ini[botINI,Curing,str_cure_corruption,"NULL"]}
	/declare    int_cure_corruption		string  outer   ${Ini[botINI,Curing,int_cure_corruption,36]}
	/declare    is_corr_group           bool    outer   ${Ini[botINI,Curing,is_corr_group, FALSE]}
	
	/declare    str_cure_poison			string  outer   ${Ini[botINI,Curing,str_cure_poison,"Antidote"]}
	/declare    int_cure_poison			string  outer   ${Ini[botINI,Curing,int_cure_poison,64]}
	/declare    is_poison_group         bool    outer   ${Ini[botINI,Curing,is_poison_group, FALSE]}
	
	/declare    str_cure_disease		string  outer   ${Ini[botINI,Curing,str_cure_disease,"Pure Blood"]}
	/declare    int_cure_disease		string  outer   ${Ini[botINI,Curing,int_cure_disease,18]}
	/declare    is_disease_group        bool    outer   ${Ini[botINI,Curing,is_disease_group, FALSE]}
	
	/declare    str_secondary_radiant	string	outer   ${Ini[botINI,Curing,str_secondary_radiant,"NULL"]} 
	

	/declare	bln_hasDiseaseCure		bool	outer	TRUE
	/declare	bln_hasPoisonCure		bool	outer	TRUE
	/declare 	bln_hasCurseCure		bool	outer	TRUE
	/declare 	bln_hasCorruptionCure	bool	outer	FALSE 
	
	
	/if (${str_cure_curse.Equal[NULL]}) 	/varset bln_hasCurseCure 	FALSE
	/if (${str_cure_poison.Equal[NULL]}) 	/varset bln_hasPoisonCure 	FALSE
	/if (${str_cure_disease.Equal[NULL]}) 	/varset bln_hasDiseaseCure 	FALSE
	
	/cecho \ao xbot_initialize_curing
	/call InitEvents_Curing
	
/return