
#EVENT DebuffTarget "<#1#> DebuffTarget #2#"
#EVENT DebuffTarget "[#1#(msg)] DebuffTarget #2#"
#EVENT DebuffTarget "#1# tells you, 'DebuffTarget #2#"
#EVENT DebuffTarget "#1# tells the group, 'DebuffTarget #2#'"
#EVENT DebuffTarget "#1# you tell your party, 'DebuffTarget #2#'"

#event Debuffed				"#1# looks very uncomfortable#*#"
#event Debuffed				"#1# is surrounded by a dark haze#*#"
#event Debuffed				"#1# is marked by the scent of afterlight#*#"
#event Debuffed				"#1# is marked by the scent of doom#*#"
#event DebuffResist			"#*#Your target resisted the |${str_debuffSpell}|#*#"

Sub InitEvents_Debuffing
	/call RegisterRaidEvents Events_Debuffing 1
	/if (${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[NEC]}) {
		/call RegisterClassEvents Events_Debuffed 2
	}
		
/return

Sub Events_Debuffed
	/doevents Debuffed
	/doevents DebuffResist
/return

Sub Events_Debuffing
	/doevents DebuffTarget
/return


| Specifically debuff a target
Sub EVENT_DebuffTarget(_line, _sender, _eventParams)
	/if (${int_debuffTotal} == 0) /return 
	/echo EVENT_KM line: ${_line} 
	/echo EVENT_KM sender: ${_sender} 
	/echo EVENT_KM _eventParams: ${_eventParams}
	/echo EVENT_KM _targetID: ${_eventParams.Arg[1, ]} 

	/declare db int local
	/declare intTries int local 0
	/call TrueTarget ${_eventParams.Arg[1, ]}
	/for db 1 to ${int_debuffTotal}
		
		/echo Forced casting of  ${DEBUFF_${db}}
		|-- Cast the Debuff
		/docommand /${cmdChannel} << ${Target.Name} >> being debuffed with ${DEBUFF_${db}}
		:attemptDebuff
		/varset intTries 0
			:waitDB
				/if (!${Me.SpellReady[${DEBUFF_${db}}]}) {
					/delay 2
					/goto :waitDB
				}
			/call Cast "${DEBUFF_${db}}" ${DEBUFF_${db}_Gem} 5s
			/varset intTries   ${intTries}+1
			
			|-- Parse Cast Return
			/echo debuffCast ${Macro.Return}
			/if (${Macro.Return.Equal["CAST_INTERRUPTED"]}) /goto :debuffLoop
			/if (${Macro.Return.Equal["CAST_IMMUNE"]}) {
				/docommand /${cmdChannel} << ${Target.Name} >> is IMMUNE to ${DEBUFF_${db}}.
				/goto :nextDB
			} 
			/if (${Macro.Return.Equal["CAST_RESISTED"]}) {
				/varset intTries   ${intTries}+1
				/if (${intTries} == ${DEBUFF_${db}_MaxTries}) {
					/docommand /${cmdChannel} << ${Target.Name} >> has RESISTED ${DEBUFF_${db}} ${intTries} times. Giving up.
					/goto :nextDB
				} else {
					/goto :attemptDebuff
				}
			}
			
			/if (${Macro.Return.Equal["CAST_SUCCESS"]}) {
				/docommand /${cmdChannel} << ${Target.Name} >> has DEBUFFED with ${DEBUFF_${db}}.
				/goto :nextDB
			}
	/call RunRaidEvents
	:nextDB
	/next db
	/doevents flush DebuffTarget
/return


|**
 ----------------------------
	chkDebuffs
 ----------------------------
 Will loop through the target Array and cast each debuff on each target as long as that target has HOTT
 Each Target will be fully debuffed before next one is debuffed.
 Will check value of debuff index in the target array eg: [targetID, debuffIndex (1-n)]
   if -1 then it hasnt been debufffed and will call debuffTarget
 ----------------------------
**|
Sub chkDebuffs
	
	/if (!${bln_engaged}) /return
	/if (${int_debuffTotal} == 0) /return 
	
	/declare i int local 1
	/declare j int local 1
	/declare dbfShift       int  local
	/declare isNamed 		bool local  FALSE
	/declare namedOnly      bool local	FALSE

	|-- clean the array
	/call CleanTargetArray
	
	|-- loop through targets and check debuff status
	
	/for i 1 to ${SpawnCount[NPC radius 100 range 50 100 noalert 4]} 
		:targetCheckStart
		|/echo targetCheckStart ${i}
		/if (${targetArray[${i},1]} != 0) {
			
			|-- Handle an uncaught NULL (they get annoying)
			/if (${NearestSpawn[NPC ID ${targetArray[${i},1]}].ID} == NULL) {
				/echo found a null, calling clean array
				/call CleanTarget ${i}
				/goto :targetCheckEnd
			} 
			
			|-- Check to see if MOB has a target of target and you have LOS
			|-- Make sure you aren't going to pull it!
			/target id 	${targetArray[${i},1]}
			/delay 30 ${Target.ID} == ${targetArray[${i},1]}
			/delay 10
			
			/if (!${Me.TargetOfTarget.ID} || !${Me.LineOfSight}) {
				/echo ${Me.TargetOfTarget.ID} || ${Me.LineOfSight} this target has no target or is out of LOS, not going to debuff yet
				/goto :targetCheckEnd
			}

			|-- Named Target check
			/call TargetNamedCheck ${Target.ID}
			/varset 	isNamed ${Macro.Return}
			|/echo named: ${isNamed}

			/for j 1 to ${int_debuffTotal}
			
				:debuffCheckStart
				|/echo :debuffCheckStart [${DEBUFF_${j}}] namedOnly? [${DEBUFF_${j}_NamedOnly}]
				/varcalc dbfShift (${int_debuffTotal}+1)
				
				|-- if we only cast this spell on named, and this is NOT a named target, skip
				/if (!${isNamed} && ${DEBUFF_${j}_NamedOnly}) {
					/goto :debuffCheckEnd
				}
				
				/echo Checking casting of  ${DEBUFF_${j}} staus: ${targetArray[${i},${dbfShift}]} on ID[${i}]: ${targetArray[${i},1]}
				/echo ---
				/echo :debuffCheckStart [${DEBUFF_${j}}] namedOnly? [${DEBUFF_${j}_NamedOnly}]
				/echo (${targetArray[${i},${dbfShift}]} == 0) &&  ${Me.LineOfSight}
				/echo ---
				
				/if ((${targetArray[${i},${dbfShift}]} == 0) && ${Me.LineOfSight}) {
					|-- check if this is debuff ready
					/if (${Me.PctHPs} <= ${int_debuffStartHP}) {
						|-  do the debuff
						/call debuffTarget ${i} ${j} FALSE
						:debuffCheckEnd	
						
					} else {
						|- HP to start at not reached;
					}
				} else {
					/echo Mob already debuffed with ${DEBUFF_${j}}
				}
			:debuffCheckEnd	
			/call RunRaidEvents
			/next j
		}
	:targetCheckEnd
	|/echo targetCheckEnd
	/next i
	
/return





|**
 ----------------------------
 Debuff Target
 ----------------------------
 Will cast the desired debuff on the target
 Passed in index of the target array to target
 as well as the index in the debuffs array (1-n) to cast.
 Override Array used if we want to manually define a target (not used)
 ----------------------------
**|

Sub debuffTarget(int _targetIndex, int _debuffIndex, bool _overrideArray)
	| eg 342, 2 as in second index in the target array
	
	/echo debuffTarget ${_targetIndex}, ${_debuffIndex}, ${_overrideArray}
	
	/declare tgtArrayDBFIndex	int		local 
	
	/declare dbfShift       int  local
	/varcalc dbfShift 		${_debuffIndex}+1
	
	/declare debuffKey		string	local DEBUFF_${_debuffIndex}
	/declare debuffSpell    string  local ${DEBUFF_${_debuffIndex}}
	/declare isNamed        bool    local FALSE
	/declare intTries		int		local 0
	
	
	
	|-- Target ID
	/if (!${_overrideArray}) {
		/call TrueTarget ${targetArray[${_targetIndex},1]}
		/delay 10 ${Target.ID} == ${targetArray[${_targetIndex},1]}
	}
	
	|-- Cast the Debuff
	/echo << %t >> being debuffed with ${debuffSpell}.
	:debuffLoop
		/if (!${Target.ID} || ${Target.Type.NotEqual[NPC]}) /return
		:debuffReadyWait
			/if (!${Me.SpellReady[${debuffSpell}]}) {
				/delay 2
				/goto :debuffLoop
			}
		/call Cast "${debuffSpell}" gem3 5s chkCastTarget ${_targetIndex}
		/varset intTries   ${intTries}+1
		
	|-- Parse Cast Return
	:debuffCast
		/echo debuffCast ${Macro.Return}
		/if (${Macro.Return.Equal["CAST_INTERRUPTED"]}) /goto :debuffLoop
		/if (${Macro.Return.Equal["CAST_IMMUNE"]}) {
			/if (${bln_annouce}) /${cmdChannel} << %t >> is IMMUNE to ${debuffSpell}.
			/varset targetArray[${_targetIndex},${dbfShift}] 1
			/return
		} 
		/if (${Macro.Return.Equal["CAST_RESISTED"]}) {
			/varset intTries   ${intTries}+1
			/if (${intTries} == ${${debuffKey}_maxtries}) {
				/if (${bln_annouce}) /${cmdChannel} << %t >> has RESISTED  ${debuffSpell} ${intTries} times. Giving up.
				/varset targetArray[${_targetIndex},${dbfShift}] 1
				/return
			}
		}
		
		/if (${Macro.Return.Equal["CAST_SUCCESS"]}) {
			/if (${verbose}) /${cmdChannel} << %t >> has DEBUFFED with ${debuffSpell}.
			/varset targetArray[${_targetIndex},${dbfShift}] 1
			/return
		}
	/call RunRaidEvents	
/return

|**
 ----------------------------
 chkCastTarget
 ----------------------------
 Called while casting debuffs. If the target no longer exists, 
 (dead) we interrupt the cast, and clear it from array
 ----------------------------
**|
Sub chkCastTarget
	/if (!${Target.ID} || ${Target.Type.NotEqual[NPC]}) { 
		/call Interrupt
		/call CleanTarget ${int_tmpIndex}
	}
/return


|**
 ----------------------------
 Checking Debuff (note, used by macros that don't keep a target array)
 ----------------------------
 Will cast the desired debuff on the target
 ----------------------------
**|
Sub chkSingleTargetDebuff
	/if (!${bln_engaged}) /return
	/if (${int_debuffTotal} == 0) /return
	|-- is target named?
	/call TargetNamedCheck ${NearestSpawn[NPC ID ${targetArray[${i},1]}].ID}
	/declare tgtArrayDBFIndex	int		local 
	/declare j int local 0
	/declare isNamed bool local FALSE
	
	/varset isNamed ${Macro.Return}
	
	|/echo chkSingleTargetDebuff total: #{int_debuffTotal}
	
	
	/for j 1 to ${int_debuffTotal}
		:debuffCheckStart

		/varcalc tgtArrayDBFIndex ${j}
		
		|-- is the debuff named only?
		/varset namedOnly ${DEBUFF_${j}_NamedOnly}

		/if (${namedOnly} && !${isNamed}) {
			|- I should only be casting this debuff on named;
			/varset targetArray[${i},${tgtArrayDBFIndex}] 1
			/goto :nextDebuff
		} else /if (${targetArray[${i},${tgtArrayDBFIndex}]} == 0 && ${NearestSpawn[NPC ID ${targetArray[${i},1]}].LineOfSight}) {
			|- Not named, but debuff should happen, check HP
			/if (${NearestSpawn[NPC ID ${targetArray[${i},1]}].PctHPs} <= ${int_debuffStartHP}) {
				|-  do the debuff
				/call debuffTarget ${i} ${j} FALSE
			} else {
				|- HP to start at not reached;
			}
		}
	:nextDebuff	
	/next j
/return

Sub ListTargets
	/echo
	/echo -----
	/echo Target Array
	/echo -----
	/declare i int local 1
	/for i 1 to ${targetArray.Size[1]}
		/echo targetArray[${i}][id] == ${targetArray[${i},1]}
	/next i
/return

|**
 ----------------------------
 Target Acquisition
 ----------------------------
 Cleans Array and Then Acquires Targets
 ----------------------------
**|
Sub getTargets
	/call CleanTargetArray
	/call AcquireTargets
/return

|-- Clears Entire Array of Entries
Sub ClearTargetArray
	/declare i int local 1
	/declare j int local 1
	/for i 1 to ${targetArray.Size[1]}
		/call CleanTarget ${i}
	/next i
/return

|-- Cleans Array of Dead Entries
Sub CleanTargetArray
	/if (${bln_engaged}) /return
	/declare tgt int local 0
	/declare i int local 1
	/declare j int local 1
	|/squelch /alert clear 4
	/for i 1 to ${targetArray.Size[1]}
		:loopStart
		/call RunRaidEvents	
		/if (${bln_engaged}) /return
		/if (${targetArray[${i},1]} == 0 &&  ${i}<${targetArray.Size[1]}) {
			/goto :nextTargetArrayIndex
		}
		
		/varset tgt ${Spawn[NPC id ${targetArray[${i},1]}].ID}	
		/delay 1

		/if (${tgt} == 0 && ${i}>=${targetArray.Size[1]}) {
			/return
		}
		/if ((${tgt} == NULL || ${Spawn[id ${tgt}].PctHPs} == 0 || ${Spawn[id ${targetArray[${i},1]}].Name.Find[corpse]}>0) && ${targetArray[${i},1]} != -1 ) {
			/call CleanTarget ${i}
		}
	:nextTargetArrayIndex
	/next i
	/call RunRaidEvents	
/return

|-- Removes a single target (by index) from the Target Array	

Sub CleanTarget(int arrIndex) 
	/if (${arrIndex} == -1 || ${arrIndex} == 0) /return
	|/echo CleanTarget Index:(${arrIndex})
	|- clear from debuff list(s)
	/varset targetArray[${arrIndex},1] 0
	
	/declare i 				int	local 2
	/declare dbfShift       int	local 1
	
	/varcalc dbfShift ${int_debuffTotal}+1
	/for i 2 to ${dbfShift}
		|/if (${i} == -1) /return
		|/if (${arrIndex} == -1) /return
		/echo ${arrIndex}: ${i} :: ${targetArray[${arrIndex},${i}]}
		/varset targetArray[${arrIndex},${i}] 0
	/next i
	:return
/return	



|**
 ----------------------------
 Acquire & Update Targets
 ----------------------------
 Will loop through the target Array and cast each debuff on each target.
 Each Target will be fully debuffed before next one is debuffed.
 Will check value of debuff index in the target array eg: [targetID, debuffIndex (1-n)]
   if -1 then it hasnt been debufffed and will call debuffTarget
 ----------------------------
**|	
Sub AcquireTargets
	/declare t_id		int		local -1
	/declare i			int		local 1
	/declare j 			int		local 1
	/declare k 			int		local 1
	/declare dbf		int 	local 1
	/declare found		bool	local FALSE
	/declare nt			int		local 1
	/declare freeslot 	int 	local -1
	
	|--get nearest spawns and see if they are already in list.
	/for i 1 to ${SpawnCount[NPC radius 100 range 50 100 noalert 4]}
		:acquireStart
		/varset t_id ${NearestSpawn[${i},NPC radius 100 range 50 100 noalert 4].ID}
		
		/if (${t_id} == NULL) /goto :acquireEnd
		/delay 10

		|-- mob has 0 HP
		/if (${Spawn[id ${t_id}].PctHPs} == 0) {
			/squelch /alert add 4 id ${t_id}
			/goto :acquireEnd
		}
		
		|-- mob is not in LOS
		|/echo LOS check
		/if (${Spawn[id ${t_id}].LineOfSight} != TRUE) {
			/squelch /alert add 4 id ${t_id}
			/goto :acquireEnd
		}
		
		|-- mob is on ignore list
		/if (${ExcludeList.Find[${Spawn[id ${t_id}].CleanName}]}) {
			/squelch /alert add 4 id ${t_id}
			/goto :acquireEnd
		}
		|-- mob is already added
		/for j 1 to ${targetArray.Size} 
			/if (${targetArray[${j},1]} == ${t_id}) {
				/goto :acquireEnd
			}
		/next j
		
		|-- Add this mob
		/call Add2TargetArray ${t_id}
	
	:acquireEnd
	/next i	
	/call RunRaidEvents	
/return	

|**
 ----------------------------
 Target Checking
 ----------------------------
 Determines if there are any targets in the vicinity and I am currently engaged
 ----------------------------
**|

Sub chkTargetList
    /if ((!${Target.ID} || ${Target.Type.NotEqual[NPC]}) && ${bln_engaged}) {
		/if (${SpawnCount[NPC radius 100 range 50 100]} == 0) {
			/echo All Mobs are dead. Engage off.
			/varset bln_engaged FALSE
			/varset int_targetID 0
			/call ClearTargetArray
		}
	}
	/call RunRaidEvents	
/return


|-- Small sub to get the first empty slot in the target array
Sub getEmptySlot
	/declare slot 	int 	local 1
	/declare i		int		local 1
	/for i 1 to ${targetArray.Size[1]}
		/if (${targetArray[${i},1]} == -1 || ${targetArray[${i},1]} == 0) {
			/varset slot ${i}
			/return ${slot}
		}
	/next i
/return ${slot}

Sub Add2TargetArray(int mobID)
	/declare i 			int		local 1
	/declare found 		bool	local FALSE
	/declare freeslot   int     local 1

	
	/for i 1 to ${targetArray.Size[1]}
		|-- Found target in array, skip adding this one
		/if (${targetArray[${i},1]} == ${mobID}) {
			/varset found TRUE
		}
	/next i	

	/if (!${found}) {
		/call getEmptySlot
		/varset freeslot ${Macro.Return}
		/varset targetArray[${freeslot},1] ${mobID}
		/call ResetDebuffsOnIndex ${freeslot}
	}
/return



Sub ResetDebuffsOnIndex(int _targetIndex)
	/declare i			int 	local 1
	/declare dbfShift       int  local
	/varcalc dbfShift ${int_debuffTotal}+1
				
	/for i 2 to ${dbfShift}
		/varset targetArray[${_targetIndex},${i}] 0
	/next i

/return 

Sub Event_Debuffed (string Line, string MobName)
	/varset	bln_debuffActive	TRUE
	/varset timer_debuff		120s
	/doevents flush Debuffed
/return

Sub Event_DebuffResist
	/varcalc int_debuffResist	${int_debuffResist}+1
	/if (${int_debuffResist} >3) /varset bln_debuffActive TRUE
	/doevents flush DebuffResist
/return

Sub xbot_initialize_debuffing
	/if (${str_classArchetype.Equal[MELEE_END]}) /return
	/if (${Me.Class.ShortName.Equal[WAR]}) /return
	
	/cecho \ao xbot_initialize_debuffing
	/call InitEvents_Debuffing
	
	|--Debuff(s)
	/declare bln_debuffUse				bool	outer	${Ini[botINI,Debuff,bln_debuffUse,TRUE]}
	/declare bln_debuffActive			bool	outer	FALSE
	/declare timer_debuff				timer	outer	30s
	/declare int_debuffResist			int		outer	0
	/declare int_tmpIndex				int		outer   0
	/alert clear 4
	
	/declare int_debuffStartHP			int		outer   ${Ini[botINI,Debuff,int_debuffStartHP,100]}
	/declare int_debuffStopHP			int		outer   ${Ini[botINI,Debuff,int_debuffStartHP,10]}
	
	/declare int_debuffTotal			int		outer	${Ini[botINI,Debuff,int_debuffTotal,0]}
	/declare int_debuffTotalIfNamed		int		outer	${Ini[botINI,Debuff,int_debuffTotalIfNamed,${int_debuffTotal}]}
	/declare db							int		local
	/for db 1 to ${int_debuffTotalIfNamed} 
		/declare DEBUFF_${db} 				string	outer	${Ini[botINI,Debuff,DEBUFF_${db}]}
		/declare DEBUFF_${db}_Use 			bool	outer	${Ini[botINI,Debuff,DEBUFF_${db}_Use]}
		/declare DEBUFF_${db}_Gem 			string	outer	${Ini[botINI,Debuff,DEBUFF_${db}_Gem]}
		/declare DEBUFF_${db}_MaxTries 		int		outer	${Ini[botINI,Debuff,DEBUFF_${db}_MaxTries]}
		/declare DEBUFF_${db}_NamedOnly		bool	outer	${Ini[botINI,Debuff,DEBUFF_${db}_NamedOnly]}
		/declare DEBUFF_${db}_Resisted		int		outer 	0
		/declare DEBUFF_${db}_Active		bool	outer 	FALSE
		/declare DEBUFF_${db}_Timer			timer   outer   0
	/next db
	
	/declare dbidx						int 	local
	/varcalc dbidx	${int_debuffTotalIfNamed}+1
	/declare targetArray[12,6] 	int		outer 0
	

/return