|**
----------------------------
DEBUFFING HANDLERS
----------------------------
This library supplements/abstracts mq2cast functionality
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	INITIAL REVISION
	03.31.22	xiris	Updated the named checking as well as normalizing all calls to MQ2Cast
	05.08.22	xiris	Added support for bards in Debuffs
	05.13.22	xiris   Cleaned up events
**|


#EVENT DebuffTarget "<#1#> DebuffTarget #2#"
#EVENT DebuffTarget "[#1#(msg)] DebuffTarget #2#"

Sub InitEvents_Debuffing
	/if (${int_debuffTotal} > 0 ) {
		/call RegisterRaidEvents Events_Debuffing 1
	}	
/return

Sub Events_Debuffing
	/doevents DebuffTarget
/return

|-- Force casting debuffs on target (forced to treat as named)
Sub EVENT_DebuffTarget(_line, _sender, _eventParams)
	/if (${int_debuffTotal} == 0) /return 
	/call CastDebuffsOnTarget ${_eventParams.Arg[1, ]} TRUE
/return


|-- Do not use the target array to check the targets, lets just check a single (current) target
|   Useful for bards, and others that do not need to AE/Mez/Slow whatever will check if engaged on current target
|   Note we currently disabled the array of targets so this is the only debuff check method
Sub chkDebuffsOnSingleTarget(int _targetID)
	/if (!${int_debuffTotal}) /return
	/call CastDebuffsOnTarget ${_targetID} FALSE
/return


|-- Loop through all debuffs, and attempt to cast one at a time
|   Will check to see if the target already has the debuff and if so has more than 30s left on it
Sub CastDebuffsOnTarget(int _targetID, bool _forced) 	
	/if (!${int_debuffTotal}) 		/return

	/call TargetNamedCheck ${_targetID}
	/declare isNamed	bool	local	${Macro.Return}
	/if (${_forced}) /cecho \agCastDebuffsOnTarget \awID:\ay${_targetID}  \awisNamed:\ay${isNamed}  \awforcing?:\ay${_forced}
	/if (${_forced}) /varset isNamed TRUE
	/if (${_forced}) /goto :debuffTarget

	/if (${isNamed} && ${int_debuffTotalIfNamed} == 0) /goto :return
	/if (${int_debuffTotal} == 1 && ${DEBUFF_1_Timer} && !${_forced}) /goto :return



	|-- things passed, lets target the NPC
	:debuffTarget
	/call TrueTarget ${_targetID}

	/declare	db		int		local	1
	/declare 	debuffDuration int local
	/for db 1 to ${int_debuffTotal}
		/if (${_forced}) /goto :debuffSpellCheck
		/if ((${DEBUFF_${db}_NamedOnly} && !${isNamed}) || (${DEBUFF_${db}_Timer} && !${_forced})) /goto :nextDB
		/if (${Me.Class.ShortName.NotEqual[BRD]}) {
			|/echo checking if the debuff is already on ${DEBUFF_${db}}:${Target.Buff[${DEBUFF_${db}}].ID} && ${Target.BuffDuration[${DEBUFF_${db}}]} and we arent forcing
			/if (${Target.Buff[${DEBUFF_${db}}].ID} && ${Target.BuffDuration[${DEBUFF_${db}}]} > 100 && !${forced}) /goto :nextDB
		}

		:debuffSpellCheck
			/call chkSpellMemorized "${DEBUFF_${db}}" TRUE ${DEBUFF_${db}_Gem}
		:debuffReadyWait 
			/if ((!${Me.SpellReady[${DEBUFF_${db}}]} || ${Me.Casting.ID}) && ${Me.Class.ShortName.NotEqual[BRD]}) {
				/delay 2
				/goto :debuffReadyWait
			}

		:debuffCast
			|/echo CastDebuffsOnTarget ${db}:: "${DEBUFF_${db}}" ${DEBUFF_${db}_Gem} ${DEBUFF_${db}_MaxTries} ${Target.ID}
			/varcalc debuffDuration ${Spell[${DEBUFF_${db}}].Duration} * 6
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				/medley queue "${DEBUFF_${db}}"
				/docommand /${cmdChannel} << ${Target.Name} >> ${DEBUFF_${db}}
				/cecho \ag ${DEBUFF_${db}}  : DELAYING for \ay${debuffDuration}s \agrefresh	
			} else {
				/call CastDebuff "${DEBUFF_${db}}" ${DEBUFF_${db}_Gem} ${DEBUFF_${db}_MaxTries} ${Target.ID}
			}

			| duration is in ticks to set a timer, so we dont recast it immediately
			/varset DEBUFF_${db}_Timer ${debuffDuration}s
	:nextDB
		/call RunRaidEvents
		/next db
	:return

/return

|-- Cast a Specific Debuff spell on the target
Sub CastDebuff(string _debuff, string _gem,  int _maxTries, int _targetID)
	/echo CastDebuff _spell: ${_debuff}, _gem: ${_gem}, _maxTries: ${_maxTries}
	/declare	try		int		local	0
	/if (!${Target.ID} || ${Target.ID} != ${_targetID}) /call TrueTarget ${_targetID}
	:attemptDebuff
		/if (${try} > ${_maxTries}) /goto :return
		/call MQ2Cast "${_debuff}" ${_gem} 5s
		/varcalc try ${try}+1
		/if (${Macro.Return.Equal["CAST_IMMUNE"]}) /goto :return
		/if (${Macro.Return.Equal["CAST_SUCCESS"]}) /goto :return
		/goto :attemptDebuff
	:return
	/cecho \ag ${_debuff} ${Macro.Return}
	/if (${try} > ${_maxTries}) /rs Unable to ${_debuff} on ${Target.Name} after ${_maxTries} tries!
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

Sub xbot_initialize_debuffing

	|--Debuff(s)
	/declare bln_debuffUse				bool	outer	${Ini[botINI,Debuff,bln_debuffUse,TRUE]}
	/declare bln_debuffActive			bool	outer	FALSE
	/declare int_debuffResist			int		outer	0
	/declare int_tmpIndex				int		outer   0
	/alert clear 4
	
	/declare int_debuffTotal			int		outer	${Ini[botINI,Debuff,int_debuffTotal,0]}
	/declare int_debuffTotalIfNamed		int		outer	${Ini[botINI,Debuff,int_debuffTotalIfNamed,${int_debuffTotal}]}
	/declare int_debuffStartHP			int		outer   ${Ini[botINI,Debuff,int_debuffStartHP,100]}
	/declare int_debuffStopHP			int		outer   ${Ini[botINI,Debuff,int_debuffStartHP,10]}
	
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

	/cecho \ao xbot_initialize_debuffing
	/call InitEvents_Debuffing

/return