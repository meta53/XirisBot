|**
----------------------------
CURING DEBUFF HANDLERS updated version 03.17.22
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
**|


#EVENT cureRequest 				"[#1#(msg)] cureRequest #2#"
#EVENT cureReceived             "[#1#(msg)] I cured you"
#EVENT cureRejected				"[#1#(msg)] I cannot cure you"

#EVENT cureGroupPoison "<#1#> cureGroupPoison #2#"
#EVENT cureGroupPoison "[#1#(msg)] cureGroupPoison #2#"
#EVENT cureGroupPoison "#1# tells you, 'cureGroupPoison #2#'"
#EVENT cureGroupPoison "#1# tells the group, 'cureGroupPoison #2#'"
#EVENT cureGroupPoison "#1# you tell your party, 'cureGroupPoison #2#'"

#EVENT cureGroupDisease "<#1#> cureGroupDisease #2#"
#EVENT cureGroupDisease "[#1#(msg)] cureGroupDisease #2#"
#EVENT cureGroupDisease "#1# tells you, 'cureGroupDisease #2#'"
#EVENT cureGroupDisease "#1# tells the group, 'cureGroupDisease #2#'"
#EVENT cureGroupDisease "#1# you tell your party, 'cureGroupDisease #2#'"

#EVENT cureGroupCurse "<#1#> cureGroupCurse #2#"
#EVENT cureGroupCurse "[#1#(msg)] cureGroupCurse #2#"
#EVENT cureGroupCurse "#1# tells you, 'cureGroupCurse #2#'"
#EVENT cureGroupCurse "#1# tells the group, 'cureGroupCurse #2#'"
#EVENT cureGroupCurse "#1# you tell your party, 'cureGroupCurse #2#'"

Sub InitEvents_Curing
	/if (${str_classArchetype.Equal[PRIEST]}) /call RegisterClassEvents Events_Curing 34
	/call RegisterBackgroundEvents Events_Curing_Background
/return

Sub Events_Curing
	/doevents cureRequest
	/doevents cureGroupPoison
	/doevents cureGroupDisease
	/doevents cureGroupCurse
/return

Sub Events_Curing_Background
	/doevents cureReceived
	/doevents cureRejected
/return

Sub Event_cureRequest(_line, _sender, _eventParams)
	/echo EVT_CUREREQUEST: _line:${_line}
	/echo _sender: ${_sender}
	/echo _eventParams:${_eventParams}
	/doevents flush
	/declare name string local ${_eventParams.Arg[1, ]}
	/echo name: ${name}
	/declare id int local ${_eventParams.Arg[2, ]}
	/echo id: ${id}
	/declare curetype string local  ${_eventParams.Arg[3, ]}
	/echo curetype: ${curetype}
	/declare count int local  ${_eventParams.Arg[4, ]}
	/echo count: ${count}
	/call handleCureRequest ${name} ${id} ${curetype} ${count}
	/doevents flush cureRequest
/return


Sub Event_cureReceived
	/echo cure Received
	/call resetCureState
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
	/rs Done Curing Group of Poisons
	/doevents flush cureGroupPoison
/return

Sub Event_cureGroupDisease(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGDisease: _line:${_line}
	/doevents flush
	/rs Curing Group of Disease with: ${str_cure_disease}
	/call cureGroup disease ${_eventParams}
	/rs Done  Curing Group of Diseases
	/doevents flush cureGroupDisease
/return

Sub Event_cureGroupCurse(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} &&${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGCurse: _line:${_line}
	/doevents flush
	/rs Curing Group of Curses with: ${str_cure_curse}
	/call cureGroup curse ${_eventParams}
	/rs Done  Curing Group of Curses
	/doevents flush cureGroupCurse
/return

Sub Event_cureGroupCorruptiuon(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]} &&${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/echo EVT_CGCoruption: _line:${_line}
	/doevents flush
	/rs Curing Group of Corruption with: ${str_cure_corruption}
	/call cureGroup corruption ${_eventParams}
	/rs Done  Curing Group of Corruption
	/doevents flush Event_cureGroupCorruptiuon
/return

Sub resetCureState
	/echo cure state reset
	/varset bln_askedForCure 	FALSE
	/varset bln_needsCure		FALSE
/return 


|-- Check if I need a cure (all classes)
Sub chkNeedCure
	|-- Verify need
	/if (!${Debuff} && ${bln_needsCure}) {
		/call resetCureState
		/return
	}

	|-- Verify need
	/if (${bln_askedForCure} == TRUE) 	/return
	|-- Verify need/ability
	/if (${Debuff.Count} == 0 || ${Debuff.Counters} > 200) /return
	|-- Verify ability to be cured
	/if (${str_myCurer.Equal[NULL]}) 	/return
	
	|-- Force the cure?
	/declare forceCure		bool 	local	FALSE
	/declare cureType 		string 	local 	NULL
	/declare counters 		int 	local
	|-- If I am tanking, and I have under 100 counters (some debuffs are uncureable) or we aren't engaged, then force the cure request
	/if (${bln_tanking} && (${Debuff.Counters} < 100 && ${Debuff.Count}) || !${bln_engaged}) /varset forceCure TRUE
	
	|-- Mark of Death must be cured!
	/if (${Me.Buff[Mark of Death].ID}) {
		/echo I have [Mark of Death], a debuff that I must cure!
		/varset forceCure TRUE
	}
	
	|-- Wanton Destruction must be cured!
	/if (${Me.Buff[Wanton Destruction].ID}) {
		/echo I have [Wanton Destruction], a debuff that I must cure!
		/varset forceCure TRUE
	}
	
	|-- Aura of Destruction cannot be cured!
	/if (${Me.Buff[Aura of Destruction].ID}) {
		/echo I have [Wanton Destruction], a debuff that I cannot cure!
		/varset bln_askedForCure TRUE
		/return
	}

	|-- Mana Drains (casting classes)
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[PAL]}) {
		/if (${Debuff.ManaDrain} >= ${int_manadrain_threshold}) /varset forceCure TRUE
	} 

	|-- HP Drains
	/if  (${Debuff.HPDrain} >= ${int_hpdrain_threshold}) /varset forceCure TRUE

	|-- Slows (melee)
	/if (${Debuff.Slowed} && (${Debuff.Counters} < 100 )) /varset forceCure TRUE

	/if ( ${forceCure} || ${bln_needsCure}) {
		/varset bln_needsCure TRUE
		
		|-- check to see if my curer is still alive
		/if (${SpawnCount[PC ${str_myCurer}]} == 0) {
			/call getGroupCurer
		} 

		/if (${str_myCurer.Equal[NULL]}) {
			/cecho \ar I HAVE NO CURER!
			/varset bln_askedForCure TRUE
			/varset bln_needsCure FALSE
			/return 
		}
			
		/if (${Debuff.Diseased} > 1) {
			|-- Handle Disease
			/varset cureType disease
			/varset counters ${Debuff.Diseased}
		} else /if (${Debuff.Poisoned} > 1) {
			|-- Handle Posion
			/varset cureType poison
			/varset counters ${Debuff.Poisoned}
		} else /if (${Debuff.Cursed} > 1) {
			|-- Handle Curse
			/varset cureType curse
			/varset counters ${Debuff.Cursed}
		} else /if (${Debuff.Corrupted} > 1) {
			|-- Handle Corruption
			/varset cureType corruption
			/varset counters ${Debuff.Corrupted}
		}
		
		/if (${cureType.NotEqual[NULL]}) {
			/g Asking for cure ${Me.Name} ${Me.ID} ${cureType} ${counters}
			/if (${str_myCurer.Equal[${Me.Name}]}) {
					/call handleCureRequest ${Me.Name} ${Me.ID} ${cureType} ${counters}
			} else {
					/bct ${str_myCurer} cureRequest ${Me.Name} ${Me.ID} ${cureType} ${counters}
			}
		}
		/varset bln_askedForCure TRUE
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
	
	/for gmember 0 to ${Group} 
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[SHM]}) /varset groupShaman ${Group.Member[${gmember}]}
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[CLR]}) /varset groupCleric ${Group.Member[${gmember}]}
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[PAL]}) /varset groupPaladin ${Group.Member[${gmember}]}
		/if (${Group.Member[${gmember}].Class.ShortName.Equal[DRU]}) /varset groupDruid ${Group.Member[${gmember}]}
	/next gmember

	
	/if (${groupPaladin.NotEqual[NULL]}) 	/varset myCurer ${groupPaladin}
	/if (${groupDruid.NotEqual[NULL]}) 		/varset myCurer ${groupDruid}
	/if (${groupCleric.NotEqual[NULL]}) 	/varset myCurer ${groupCleric}
	/if (${groupShaman.NotEqual[NULL]}) 	/varset myCurer ${groupShaman}

/return ${myCurer}

Sub cureGroup(string _cureType, int _counters)
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[DRU]} && ${Me.Class.ShortName.NotEqual[SHM]} && ${Me.Class.ShortName.NotEqual[PAL]}) /return
	/g cureGroup ${_cureType} ${_counters}
	| Cure group
	/declare ct string local ${_cureType}
	/declare cnt int local ${_counters}
	/declare	gmember			int local	0
	/for gmember 0 to ${Group}
			/echo handling: ${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} ${ct} ${cnt}
			/call handleCureRequest ${Group.Member[${gmember}].Name} ${Group.Member[${gmember}].ID} ${ct} ${cnt}
			/delay 10
		/call RunRaidEvents
	/next gmember

/return 


Sub handleCureRequest(_name, _id, _cureType, _counters) 
	/cecho \ayhandleCureRequest: ${_name}, ${_id}, ${_cureType}, ${_counters}
	|-- find cure spell string & counters it clears
	/declare	str_spell	string	local	${str_cure_disease}
	/declare    int_count   int		local   ${_counters}
	/declare    int_castCount int   local   1

	/if (${_cureType.Equal[disease]}) {
		/cecho \aoDISEASE CURE selected.
		/if (!${bln_hasDiseaseCure}) {
			/echo I cannot cure Disease
			/bct ${_name} I cannot cure you
			/return
		}
		/varset str_spell 	${str_cure_disease}
		/varset int_count 	${int_cure_disease}
	} else /if (${_cureType.Equal[poison]}) {
		/cecho \agPOISON CURE selected.
		/if (!${bln_hasPoisonCure}) {
			/echo I cannot cure Poison
			/bct ${_name} I cannot cure you
			/return
		}			
		/varset str_spell 	${str_cure_poison}
		/varset int_count 	${int_cure_poison}
	} else /if (${_cureType.Equal[curse]}) {
		/cecho \ayCURSE CURE selected.
		/if (!${bln_hasCurseCure}) {
			/echo I cannot cure Curses
			/bct ${_name} I cannot cure you
			/return
		}	
		/varset str_spell 	${str_cure_curse}
		/varset int_count 	${int_cure_curse}
	} else /if (${_cureType.Equal[corruption]}) {
		/cecho \apCORRUPTION CURE selected.
		/if (!${bln_hasCorruptionCure}) {
			/echo I cannot cure Corruption
			/bct ${_name} I cannot cure you
			/return
		}	
		/varset str_spell 	${str_cure_corruption}
		/varset int_count 	${int_cure_corruption}
	}
	
	|-- calculate how many times we need to cast the cure counters
	/varcalc int_castCount (${_counters} / ${int_count})
	/if (${int_castCount} < 1) /varset int_castCount 1
	/cecho \ar${str_spell} casting: ${int_castCount}x times (${_counters} / ${int_count})
	| need to mem curespell if I dont have it up
	/target id ${_id}
	:cureReadyWait1
	/call chkSpellMemorized "${str_spell}" TRUE
	/if (${Me.SpellReady[${str_spell}]} && (${Target.Distance}<=200)) {
		/call castCure ${_name} ${_id} "${str_spell}" ${int_castCount}
	} else {
		/goto :cureReadyWait1
	}
/return


|-- Casts a cure, for _count many times, on a _tgtID
Sub castCure(string _name, int _id, string _spell, int _count) 
	/echo \aw castCure ${_name}, ${_id}, ${_spell}, ${_count}
	/call TrueTarget ${_id}
	/declare i int local 1
	/for i 1 to ${_count}
		/cecho \aw Casting ${_spell}
		/call MQ2Cast "${_spell}" gem7 8s
		/delay 10
	/next i 
	/bct ${_name} I cured you.
/return





Sub xbot_initialize_curing
	/cecho \ao xbot_initialize_curing
	/call InitEvents_Curing
	
	/declare 	int_manadrain_threshold int		outer 	200
	/declare	int_hpdrain_threshold	int		outer	500
	/declare 	bln_needsCure 			bool	outer	FALSE
	/declare    bln_askedForCure        bool    outer   FALSE
	/declare    str_myCurer				string	outer 	NULL
	/call 		getGroupCurer
	/varset 	str_myCurer ${Macro.Return}
	
	/echo My curer is ${str_myCurer}

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
	
	
/return