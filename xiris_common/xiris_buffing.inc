|**
----------------------------
BUFFING HANDLERS updated version 01.14.22 
----------------------------
This include contains logic for buffing raid/groups

Supported by all classes based on INI settings
CLR/DRU/SHM/PAL/RNG are the buff classes I  use on raid

Will use the section [OOCBuffs] && [TargetBuffs] from ini.
Note if there is no section in INI it will quit out
______________________________________________
REVISION HISTORY
    10.24.16    xiris 	INITIAL REVISION
	11.06.16	xiris	ADDED CHAR BUFFS
	11.17.21	xiris	UPDATED the EVENTS to handle the auto puller checking buffs and asking for them
	12.28.21	xiris	Added Buff Groupings so clients can request "Symbol" and will get whatever the highest symbol the cleric has. Etc.
	01.14.22	xiris	Continued work on buffing logic
	02.15.22	xiris	EQTitan returns nonsense for raid.group stuff now, so we have to make it so we pass in the group leaders names (ugh)
						Added Array named RaidGroupLeaders to store this manually (laaame)
	03.01.22	xiris	Added Exclusions for Buffing use command /only|EXCLUSIONS eg: doRaidBuffs /only|ENC		
	05.13.22	xiris	Reduced events to BC only		
	06.13.22	xiris	Restored events for Group/Party/Raid due to need to split raids sometimes (multiple camp pulling)	
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
**|

#EVENT DORAIDBUFFS			"#1# tells the group#*#'doRaidBuffs #2#'"
#EVENT DORAIDBUFFS			"#1# tells you#*#'doRaidBuffs #2#'"
#EVENT DORAIDBUFFS			"#1# tells the raid#*#'doRaidBuffs #2#'"
#EVENT DORAIDBUFFS 			"[ #1# #*#]#*#doRaidBuffs #2#"

#EVENT DOGROUPBUFFS			"#1# tells the group#*#'doGroupBuffs #2#'"
#EVENT DOGROUPBUFFS			"#1# tells you#*#'doGroupBuffs #2#'"
#EVENT DOGROUPBUFFS			"#1# tells the raid#*#doGroupBuffs #2#'"
#EVENT DOGROUPBUFFS 		"[ #1# #*#]#*#doGroupBuffs #2#"

#EVENT DOSINGLETARGETBUFFS	"#1# tells the group#*#'doSingleTargetBuffs #2#'"
#EVENT DOSINGLETARGETBUFFS	"#1# tells you#*#'doSingleTargetBuffs #2#'"
#EVENT DOSINGLETARGETBUFFS	"#1# tells the raid#*#'doSingleTargetBuffs #2#'"
#EVENT DOSINGLETARGETBUFFS 	"[ #1# #*#]#*#doSingleTargetBuffs #2#"

#EVENT DOCHARBUFFS			"#1# tells the group#*#'doCharBuffs #2#'"
#EVENT DOCHARBUFFS			"#1# tells you#*#'doCharBuffs #2#'"
#EVENT DOCHARBUFFS			"#1# tells the raid#*#'doCharBuffs #2#'"
#EVENT DOCHARBUFFS 			"[ #1# #*#]#*#doCharBuffs #2#"

#EVENT REMOVEBUFF			"#1# tells the group#*#'removebuff #2#'"
#EVENT REMOVEBUFF			"#1# tells you#*#'removebuff #2#'"
#EVENT REMOVEBUFF			"#1# tells the raid#*#'removebuff #2#'"
#EVENT REMOVEBUFF 			"[ #1# #*#]#*#doCharBuffs #2#"

#EVENT REMOVEALLBUFFS		"#1# tells the group#*#'removeallbuffs#*#'"
#EVENT REMOVEALLBUFFS		"#1# tells you#*#'removeallbuffs#*#'"
#EVENT REMOVEALLBUFFS		"#1# tells the raid#*#'removeallbuffs#*#'"
#EVENT REMOVEALLBUFFS 		"[ #1# #*#]#*#removeallbuffs #*#"



Sub InitEvents_Buffing
	/call RegisterBackgroundEvents Events_Buffing 5
/return

Sub Events_Buffing
	/doevents DORAIDBUFFS
	/doevents DOGROUPBUFFS
	/doevents DOSINGLETARGETBUFFS
	/doevents DOCHARBUFFS
	/doevents REMOVEBUFF
	/doevents REMOVEALLBUFFS
/return


Sub EVENT_DORAIDBUFFS(_line, _chatSender, _eventParams)
	
	/if (!${bln_doRaidBuff} && !${bln_doSingleTargetBuff}) /return
	/echo Doing Raid Buffs on ${_eventParams}
	/memspellset buff
	/delay 3s
	/if (${bln_doRaidBuff}) 		/call doRaidBuffs
	/if (${bln_doSingleTargetBuff}) /call doSingleTargetBuffOnRaid
	/dgt ${Me.Class.Name} RAID BUFF [${_eventParams}] COMPLETE
	/delay 10
	| Restore the previous spellset
	/memspellset ${str_spellset}

	/doevents flush DORAIDBUFFS
/return

Sub EVENT_DOCHARBUFFS(_line, _chatSender, _eventParams)
	/if (!${bln_doRaidBuff}) /return

	/memspellset buff
	/delay 3s
	/echo Doing Group Buffs on ${_eventParams}
	/delay 10
	/call doGroupBuffOnTarget ${_eventParams}
	/call doSingleTargetBuffOnChar ${_eventParams}
	/dgt ${Me.Class.Name} CHAR BUFF [${_eventParams}] COMPLETE
	/delay 10
	| Restore the previous spellset
	/memspellset ${str_spellset}
	
	/doevents flush DOCHARBUFFS
/return


Sub EVENT_DOGROUPBUFFS(_line, _chatSender, _eventParams)
	/if (!${bln_doGroupBuff}) /return

	/memspellset buff
	/echo Doing Group Buffs [OOCBUFF]
	/call doGroupBuffOnTarget ${Me.Name}
	/dgt ${Me.Class.Name} GROUP BUFF COMPLETE
	/delay 10
	/memspellset default
	
	/doevents flush DOGROUPBUFFS
/return

Sub EVENT_DOSINGLETARGETBUFFS(_line, _chatSender, _eventParams)
	/if (!${bln_doSingleTargetBuff}) /return

	/memspellset buff
	/if (${Raid.Members}) {
		/call doSingleTargetBuffOnRaid
	} else {
		/call doSingleTargetBuffOnChar ${_eventParams}
	}
	/memspellset default
	
	/doevents flush DOSINGLETARGETBUFFS
/return

Sub EVENT_DOSPECIFICBUFFONCHAR(string _text, string _sender, string _buffer, string _spell);
	|/echo EVENT_DOSPECIFICBUFFONCHAR 
	|/echo _text: ${_text}
	|/echo _sender: ${_sender}
	|/echo _buffer ${_buffer}
	|/echo _spell: ${_spell}
	|/echo ${_buffer} Doing a Specific Buff (${_spell} -> ${_sender}) gb?:${bln_doGroupBuff} nameMatch?:${Me.CleanName.NotEqual[${_buffer}]}
	/if (!${bln_doGroupBuff}) /return
	/if (${Me.CleanName.NotEqual[${_buffer}]}) /return
	
	/memspellset buff
	/call doSpecificBuffOnChar ${_sender} "${_spell}"
	/memspellset default
	/doevents flush DOSPECIFICBUFFONCHAR
/return


|**
 ----------------------------
 Raid Buffs
 ----------------------------
 Will cast the combat buffSpell
 Will run through each raid group and cast the group buff(s) on the group leader
 Note: since ROF2 EMU on Titan 2 is broken vis-a-vis raid stuff we had to change this to a preset array.
 ----------------------------
**|
Sub doRaidBuffs 
	/if (!${bln_doRaidBuff}) /return
	/if (${Raid.Members} <= 6) /return
	/echo Doing Raid Buffs
	/declare i int local 1
	
	/for i 1 to ${RaidGroupLeaders.Size}
		/echo Targetting group leader (${i}): ${RaidGroupLeaders[${i}]}
		/if (${RaidGroupLeaders[${i}].NotEqual[NULL]}) {
			/call doGroupBuffOnTarget ${RaidGroupLeaders[${i}]}
		}
	/next i
	/call TrueTarget ${int_targetID}
/return


|**
 ----------------------------
 Single Target Combat Buffs
 ----------------------------
 Will cast the combat buffSpell
 as defined by the ini file. Will only cast on the heal target (think tank)
 Note: this is only really for clerics to keep DI on, or Vie
 ----------------------------
**|
Sub cmbtBuffSingle
	/echo Doing Single Target Combat Buffs
		|-- loop through group members that get combat buff
	/declare gmember 	  int 		local
	/declare i    		  int		local
	/declare buffDuration int		local
	/declare buffSpell    string	local
	/declare buffGem	  string	local
	/call healGRP
	
/return

|**
 ----------------------------
 Group Combat Buffs
 ----------------------------
 Will loop through the group and cast the combat buffSpell
 as defined by the ini file. Will only cast on melee (no pets, no casters)
 ----------------------------
**|
Sub cmbtBuffGRP
	/if (!${bln_engaged})     /return
	/if (!${bln_cmbtBuffUse}) /return
	/if (${int_cmbtBuffTotal} == 0 && ${CMBTBUFF_1_Timer}) /return

	|-- loop through group members that get combat buff
	/declare i    		  int		local
	/declare g			  int		local
	/declare gName 	  	  string	local
	/declare gClass       string    local
	/declare buffSpell    string	local
	/declare buffGem	  string	local
	/declare delay		  int       local	0
	/declare bStatus 	  string    local 
	/declare bDuration    string    local
	/declare bId          string    local

	| -- loop through the combat buffs
	/for i 1 to ${int_cmbtBuffTotal}
		/if (${CMBTBUFF_${i}_Timer}) /goto :nextCMBT
		| -- check to see if this combat buff is on group members, and if it is missing, slap it on the first appropriate member (if ready)
		/varset		buffSpell	 ${CMBTBUFF_${i}}
		/varset		buffGem		 ${CMBTBUFF_${i}_Gem}
		/call chkSpellMemorized "${buffSpell}" FALSE ${buffGem}
		

		/for g 0 to ${Group}
			|-- check group health
			/if (${Me.Class.ShortName.Equal[SHM]}) /call healGRP

			|-- Check to see if member has the buff
			/varset gName ${Group.Member[${g}]}
			
			/varset gClass ${Spawn[pc =${gName}].Class.ShortName}
			/if (${Select[${gClass},WAR,SHD,PAL,MNK,RNG,ROG,BRD,BER]}) {
				
				/dquery ${gName} -q "Me.Buff[${buffSpell}].Duration"
				/delay 20 ${DanNet.Query.Received}
				/varset bDuration ${DanNet.Q}
				/cecho cmbtBuffGRP: \aw${gName} -q \ag"Me.Buff[${buffSpell}].Duration" == \ar ${bDuration}
				/if (!${Bool[${bDuration}]} ) {
					/cecho \awbDuration: \ay${bDuration}
					/if (${bDuration} < 3 ) {
						/goto :castCMBTBuff
					} else {
						/goto :nextGroupMember
					}
				} else {
					/goto :castCMBTBuff
				}
			} else {
				/goto :nextGroupMember
			}

			:castCMBTBuff
			|/cecho \ayCasting ${buffSpell} on ${gName}
			
			/call TrueTarget ${Spawn[pc =${gName}].ID}
			/call MQ2Cast "${buffSpell}" ${buffGem}
			/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
				/cecho \ag ${buffSpell} ${Cast.Result} : DELAYING for \ay ${Spell[${buffSpell}].Duration}s \agrefresh
				/varcalc delay	(${Spell[${buffSpell}].Duration} * 6) - 10
				/varset CMBTBUFF_${i}_Timer ${delay}s
			}
		:nextGroupMember
		|/echo NextGroupMember ${g}
		/next g
	:nextCMBT
	
	/next i

	/call RunRaidEvents
	/call TrueTarget ${int_targetID}
/return


|**
 ----------------------------
 OOC Buffs
 ----------------------------
 Will loop through the group and cast the out of combat buffSpell
 as defined by the ini file. Will only cast once on entire group
 **PH FROM OLD CODE, WILL REWRITE**
 ----------------------------
**|
Sub doGroupBuffOnTarget(string tgt)
	
	/declare i    		  		int		local
	/declare buffDuration 		int		local
	/declare buffSpell    		string	local
	/declare buffGem	  		string	local
	/declare spellReadyCount	int 	local 0
	/target pc ${tgt}
	/delay 10

	/for i 1 to ${int_oocBuffTotal}
		/varset 	buffSpell    ${OOCBUFF_${i}}
		/varcalc	buffDuration ${Spell[${OOCBUFF_${i}}].Duration} * 6
		/varset		buffGem		 ${OOCBUFF_${i}_Gem}

		|/echo spell:${buffSpell} duration: ${buffDuration} gem: ${buffGem}
		:spellReadyWait
		/varcalc spellReadyCount ${spellReadyCount}+1
		/if (${spellReadyCount} > 50) {
			/echo Waited for 50s, it aint happening.
			/varset int_oocBuffTotal 0
			/return
		}
		/if (!${Me.SpellReady[${buffSpell}]} || ${Me.CurrentMana} < ${Spell[${buffSpell}].Mana}) {
			|/echo Waiting for ${buffSpell} Half a Tick (3s)
			/if (${Me.Class.ShortName.Equal[SHM]}) /call chkManaConvert
			/if (${Me.Standing}) /sit
			/delay 10
			/call RunAllEvents
			/if (${bln_engaged}) /return
			/goto :spellReadyWait
		}

		/if (!${Me.Standing}) /stand on
		/if (${Me.SpellReady[${buffSpell}]} && (${Target.Distance}<=200)) {
			/echo Cast "${buffSpell}" on TARGET: ${tgt}
			/call MQ2Cast "${buffSpell}" ${buffGem} 10s
			/delay 10
			/if (${Cast.Result.Equal[CAST_OUTOFMANA]})  	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_CANNOTSEE]})  	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_OUTOFRANGE]}) 	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_INTERRUPTED]}) 	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_NOTREADY]}) 		/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
				/cecho \ag ${buffSpell} ${Cast.Result}
			}
		}
	:NextGB
	/next i
	/call TrueTarget ${int_targetID}
/return		




Sub doSingleTargetBuffOnRaid

	/if (!${bln_doSingleTargetBuff}) /return
	/if (${int_stBuffTotal}==0)		 /return
	
	/echo Doing Single Target Buffs [TargetBuffs] on RAID

	/declare	i	int 	local
	/declare    b   int     local
	/declare buffSpell    	string	local
	/declare buffTargets	string	local
	/declare buffGem		string  local
	/declare buffUse		bool	local	TRUE
	/declare charName       string  local
	/declare charClass		string  local
	
		/for b 1 to ${int_stBuffTotal} 
			:startSTBuff
			/varset buffSpell	${STBUFF_${b}}
			/varset	buffTargets	${STBUFF_${b}_Targets}
			/varset buffGem		${STBUFF_${b}_Gem}
			/varset buffUse		${STBUFF_${b}_Use}

			/cecho \ay${buffSpell} ${buffTargets} ${buffGem} ${buffUse}

			/if (!${buffUse}) {
				/if (${b} < ${int_stBuffTotal})  /goto :next_b0
				/if (${b} >= ${int_stBuffTotal}) /return
				/goto :startSTBuff
			}

			/call chkSpellMemorized "${buffSpell}" TRUE ${buffGem}
			
			/for i 1 to ${Raid.Members} 

				/varset charClass ${Spawn[pc =${Raid.Member[${i}]}].Class.ShortName}
	
				/echo  ${Raid.Member[${i}]} ${charClass} : ${buffTargets.Find[${charClass}]}
				/if (${buffTargets.Find[*]} || ${buffTargets.Find[${charClass}]}) {
					/cecho \ag${Raid.Member[${i}]} is a valid Single Buff Target
					/call castSingleTargetBuff ${Raid.Member[${i}]} "${buffSpell}" ${buffGem}
				}
				
			
			/next i
			
		:next_b0
		/next b
	
	/call TrueTarget ${int_targetID}
/return

Sub doSpecificBuffOnChar(string _char, string _spell) 
	/echo doingSpecificBuffOnChar ${_char} ${_spell}
	/delay 10
	/target ${_char}
	/delay 10
	:spellReadyWait
	/if (${Me.SpellReady[${_spell}]} && (${Target.Distance}<=200)) {
		/call MQ2Cast "${_spell}" gem1 10s
	} else {
		/goto :spellReadyWait
	}
	/call TrueTarget ${int_targetID}
/return

Sub doSingleTargetBuffOnChar(string char)

	/if (!${bln_doSingleTargetBuff}) /return
	/if (${int_stBuffTotal}==0)		 /return
	
	/declare	i	int 	local
	/declare    b   int     local
	/declare buffSpell    	string	local
	/declare buffTargets	string	local
	/declare buffGem		string  local
	/declare buffUse		bool	local	TRUE

	
	/for b 1 to ${int_stBuffTotal} 
		:startSTBuf
			/varset buffSpell	${STBUFF_${b}}
			/varset	buffTargets	${STBUFF_${b}_Targets}
			/varset buffGem		${STBUFF_${b}_Gem}
			/varset buffUse		${STBUFF_${b}_Use}
			
			/if (!${buffUse}) {
				/if (${b} < ${int_stBuffTotal})  /goto :next_b1
				/if (${b} >= ${int_stBuffTotal}) /return
				/goto :startSTBuff
			}

			/if (${buffTargets.Find[*]} || (${buffTargets.Find[${NearestSpawn[pc ${char}].Class.ShortName}]} )) {
					/echo ALL CLASSES VALID
					/call castSingleTargetBuff ${char} "${buffSpell}" ${buffGem}
			}
				
			
		:next_b1
		/next b
	/call TrueTarget ${int_targetID}
/return

|-- TODO: make so the errors will cause a counter to be reached (hah! I did this 10 years later! 03.31.22)
Sub castSingleTargetBuff(string tgt, string buffSpell, string buffGem)
	/declare spellReadyCount int local 0
	/target pc ${tgt}
	
	:spellReadyWait2
		/varcalc spellReadyCount ${spellReadyCount}+1
		/if (${spellReadyCount} > 2) {
			/echo waited for 30s, it aint happening.
			/varset int_oocBuffTotal 0
			/return
		}
		/if (!${Me.SpellReady[${buffSpell}]} || ${Me.CurrentMana} < ${Spell[${buffSpell}].Mana}) {
			/echo Waiting for ${buffSpell} a Tick (6s)
			/if (${Me.Class.ShortName.Equal[SHM]}) /call chkManaConvert
			/if (${Me.Standing}) /sit
			/delay 6s
			/call RunAllEvents
			/if (${bln_engaged}) /return
			/goto :spellReadyWait2
		}

		/if (!${Me.Standing}) /stand on
		/if (${Me.SpellReady[${buffSpell}]} && (${Target.Distance}<=200)) {
			/echo Cast "${buffSpell}" on TARGET: ${tgt}
			/call MQ2Cast "${buffSpell}" ${buffGem} 10s
			/delay 10
			/if (${Cast.Result.Equal[CAST_OUTOFMANA]})  	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_CANNOTSEE]})  	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_OUTOFRANGE]}) 	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_INTERRUPTED]}) 	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_NOTREADY]}) 		/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
				/cecho \ag ${buffSpell} ${Cast.Result}
			}
		}
		
	/delay 10
	/call TrueTarget ${int_targetID}
/return

|-- Determine if I have a buff by type, and if not
Sub chkBuffByType(string _key) 

	/echo chkBuffByType(${_key})

	/declare hasBuffType bool local FALSE
	/declare buffToCheck string local
	/declare li listiterator
	

	/if (${_key.Equal[aego]}) {
		/vardata li l_aegolism.First
		/echo checking aego
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	
	} else /if (${_key.Equal[symbol]}) {
		/echo checking symbol
		/vardata li l_symbol.First
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	
	} else /if (${_key.Equal[acbuff]}) {
		/echo checking acbuff
		/vardata li l_clrAC.First
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}

	} else /if (${_key.Equal[nine]}) {
		/echo checking nine
		/vardata li l_nine.First

		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	
	} else /if (${_key.Equal[encHaste]}) {
		/echo checking encHaste
		/vardata li l_encHaste.First
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	
	} else /if (${_key.Equal[shmHaste]}) {
		/echo checking shmHaste
		/vardata li l_shmHaste.First
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	
	} else /if (${_key.Equal[focus]}) {
		/echo checking focus
		/vardata li l_focus.First
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	
	} else /if (${_key.Equal[brells]}) {
		/echo checking brells
		/vardata li l_brells.First
		/while (!${li.IsEnd}) {
			/echo ${li.Value}
			/varset buffToCheck ${li.Value}
			/if (${li.Advance}) {
				/echo 'Iterator advanced to next element.'
			}
		}
	}
/return ${hasBuffType}



|-- check OOC Buffs for myself
Sub chkOOCBuffs
	|- downshitif (this is essentially a downshit)
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || (${SpawnCount[npc radius 55]}>0 && (${Zone.ID} != 340 && ${Zone.ID} != 344 && ${Zone.ID} != 345)) || ${Me.Feigning} || ${Me.Invis}) { 
		/return
	}

	/declare i    		  int		local
	/declare buffDuration int		local
	/declare buffSpell    string	local
	/declare buffGem	  string	local

	/for i 1 to ${int_oocBuffTotal}
		/call RunAllEvents
		/if (${bln_engaged}) /return
		/varset 	buffSpell    ${OOCBUFF_${i}}
		/if (!${Bool[${Me.Spell[${buffSpell}]}]}) {
			|/echo I dont have this spell
			/goto :next
		}
		/if (${Bool[${Me.Buff[${buffSpell}].ID}]}) {
			|/echo already have buff, doing next
			/goto :next
		}

		/memspellset buff
		/cecho \awOOC Buff: \ag${buffSpell}
		/varcalc	buffDuration ${Spell[${OOCBUFF_${i}}].Duration} * 6
		/varset		buffGem		 ${OOCBUFF_${i}_Gem}

		:spellReadyWait
		/call RunAllEvents

		
		/delay 3s
		/if (!${Me.SpellReady[${buffSpell}]} || ${Me.CurrentMana} < ${Spell[${buffSpell}].Mana}) {
			/echo Waiting for ${buffSpell} for SELF buff
			/if (${Me.Standing}) /sit
			/goto :spellReadyWait
		}

		/if (!${Me.Standing}) /stand on
		/keypress f1
		/if (${Me.SpellReady[${buffSpell}]}) {
			/echo Cast "${buffSpell}" on MYSELF
			/call MQ2Cast "${buffSpell}" ${buffGem} 10s
			/delay 10
			/if (${Cast.Result.Equal[CAST_OUTOFMANA]})  	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_CANNOTSEE]})  	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_OUTOFRANGE]}) 	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_INTERRUPTED]}) 	/goto :spellReadyWait
			/if (${Cast.Result.Equal[CAST_NOTREADY]}) 		/goto :spellReadyWait
		}
		
	:next
		/next i
		/memspellset default
		/call TrueTarget ${int_targetID}
/return



|----------------------------------------------------|
|- Attempts to remove specified buffs on Dannet.	-|
|----------------------------------------------------|
|- Use, /dgt Remove [Name], or /dgt Remove 'Name'.	-|
|----------------------------------------------------|

Sub EVENT_REMOVEBUFF(_line, _chatSender, _eventParams)
	/declare buffToRemove string local ${_eventParams.Arg[1,-]}
	/call RemoveBuff ${buffToRemove}
	/doevents flush REMOVEBUFF
/return

Sub EVENT_REMOVEALLBUFFS(_line, _chatSender, _eventParams)
	/call RemoveAllBuffs
	/doevents flush REMOVEALLBUFFS
/return

Sub RemoveAllBuffs
	/declare n int local
	/for n 1 to 25
		/if (${Me.Buff[${n}].ID}) /call RemoveBuff ${Me.Buff[${n}]}
	/next n
/return

Sub RemoveBuff(_badBuff)
	/declare i int local

	/for i 0 to 24
		|/echo Remove_Buff ${i}
		|/echo 1: ${Bool[${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip}]}
		|/echo 2: ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Find[${_badBuff}]}
		/if (${Bool[${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip}]} && ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Find[${_badBuff}]}) {
			/varset _badBuff ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Arg[1,(]}
			/varset _badBuff ${_badBuff.Left[${Math.Calc[${_badBuff.Length}-1]}]}
			/if (${Bool[${_badBuff}]}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[${_badBuff}].ID}-1].Int} leftmouseup
		}
		
	/next i
/return




Sub xbot_initialize_buffing



	| |--Buff Chains - Characters can check on this list to see if they have a buff archetype
	| /declare l_aegolism	list outer
	| /declare str_aegolism_request	string	outer 	aego
	| /if (${l_aegolism.Append[Blessing of Aegolism,Hand of Virtue,Hand of Conviction]}) {
	| 	|/echo 'l_aegolism Append succeeded. Count: ${l_aegolism.Count}.'
	| } else {
	| 	/echo 'l_aegolism List insertion failed.'
	| }

	| /declare l_symbol list	outer
	| /declare str_symbol_request		string	outer	symbol
	| /if (${l_symbol.Append[Marzin's Mark,Kazad's Mark,Balikor's Mark]}) {
	| 	|/echo 'l_symbol Append succeeded. Count: ${l_symbol.Count}.'
	| } else {
	| 	/echo 'l_symbol List insertion failed.'
	| }

	| /declare l_clrAC list outer
	| /declare str_ac_request			string	outer	acbuff
	| /if (${l_clrAC.Append[Bulwark of Faith,Ward of Gallantry,Ward of Valiance]}) {
	| 	|/echo 'l_clrAC Append succeeded. Count: ${l_clrAC.Count}.'
	| } else {
	| 	/echo 'l_clrAC List insertion failed.'
	| }

	| /declare l_nine	list outer	
	| /declare str_nine_request		string	outer	nine
	| /if (${l_nine.Append[Blessing of the Nine,Blessing of Steeloak]}) {
	| 	|/echo 'l_nine Append succeeded. Count: ${l_nine.Count}.'
	| } else {
	| 	/echo 'l_nine List insertion failed.'
	| }
	
	| /declare l_encHaste	list outer
	| /declare str_encHaste_request	string	outer	encHaste
	| /if (${l_encHaste.Append[Speed of the Brood,Vallon's Quickening,Hastening of Salik]}) {
	| 	|/echo 'l_encHaste Append succeeded. Count: ${l_encHaste.Count}.'
	| } else {
	| 	/echo 'l_encHaste List insertion failed.'
	| }
	
	| /declare l_shmHaste list outer
	| /declare str_shmHaste_request	string	outer	shmHaste
	| /if (${l_shmHaste.Append[Talisman of Celerity]}) {
	| 	|/echo 'l_shmHaste Append succeeded. Count: ${l_shmHaste.Count}.'
	| } else {
	| 	/echo 'l_shmHaste List insertion failed.'
	| }

	
	| /declare l_focus list outer
	| /declare str_focus_request		string	outer	focus
	| /if (${l_focus.Append[Focus of the Seventh, Talisman of Wunshi]}) {
	| 	|/echo 'l_focus Append succeeded. Count: ${l_focus.Count}.'
	| } else {
	| 	/echo 'l_focus List insertion failed.'
	| }

	
	| /declare l_brells list outer
	| /declare str_brells_request		string	outer	brells
	| /if (${l_brells.Append[Brell's Mountainous Barrier,Brell's Stalwart Shield,Brell's Brawny Bulwark]}) {
	| 	|/echo 'l_brells Append succeeded. Count: ${l_brells.Count}.'
	| } else {
	| 	/echo 'l_brells List insertion failed.'
	| }

	
	|/call chkBuffByType nine
	|/echo checking if we have nine ${Macro.Return}

	|--OOC Buff(s)
	/declare bln_doRaidBuff				bool	outer	${Ini[botINI,OOCBuffs,bln_doRaidBuff,FALSE]}
	/declare bln_doGroupBuff			bool	outer	${Ini[botINI,OOCBuffs,bln_doGroupBuff,FALSE]}
	/declare bln_doAutoBuff				bool	outer	${Ini[botINI,OOCBuffs,bln_doGroupBuff,FALSE]}
	/declare int_oocBuffTotal			int		outer	${Ini[botINI,OOCBuffs,int_oocBuffTotal,0]}
	
	/if (${int_oocBuffTotal} > 0) {
		/declare ooct						int		local
		/for ooct 1 to ${int_oocBuffTotal} 
			/declare OOCBUFF_${ooct} 				string	outer	${Ini[botINI,OOCBuffs,OOCBUFF_${ooct}]}
			/declare OOCBUFF_${ooct}_Use 			bool	outer	${Ini[botINI,OOCBuffs,OOCBUFF_${ooct}_Use]}
			/declare OOCBUFF_${ooct}_Gem 			string	outer	${Ini[botINI,OOCBuffs,OOCBUFF_${ooct}_Gem]}
			/declare OOCBUFF_${ooct}_Active			bool	outer 	FALSE
			/declare OOCBUFF_${ooct}_Timer	timer   outer   0
		/next ooct
	}
	
	|--Single Target Buff(s)
	|--eg: Divine Intervention, Mage Huge DS, Ranger AC Buff, Cleric AC buff (if no group version)
	/declare bln_doSingleTargetBuff		bool	outer	${Ini[botINI,TargetBuffs,bln_doSingleTargetBuff,FALSE]}
	/declare int_stBuffTotal			int		outer	${Ini[botINI,TargetBuffs,int_stBuffTotal,0]}
	
	/echo ${int_stBuffTotal}
	/if (${int_stBuffTotal} > 0) {
		/declare stbuff						int		local
		/for stbuff 1 to ${int_stBuffTotal} 
			/declare STBUFF_${stbuff} 				string	outer	${Ini[botINI,TargetBuffs,STBUFF_${stbuff}]}
			/declare STBUFF_${stbuff}_Use 			bool	outer	${Ini[botINI,TargetBuffs,STBUFF_${stbuff}_Use]}
			/declare STBUFF_${stbuff}_Gem 			string	outer	${Ini[botINI,TargetBuffs,STBUFF_${stbuff}_Gem]}
			/declare STBUFF_${stbuff}_Targets		string	outer	${Ini[botINI,TargetBuffs,STBUFF_${stbuff}_Targets]}
			/echo ${STBUFF_${stbuff}_Targets}
			/declare STBUFF_${stbuff}_Active		bool	outer 	FALSE
			/declare STBUFF_${stbuff}_Timer	timer   outer   0
		/next stbuff
	}
	
	
	|--Combat Buff(s)
	|--eg: Spirit of the Jaguar
	/declare bln_cmbtBuffUse			bool	outer	${Ini[botINI,CombatBuffs,bln_cmbtBuffUse,TRUE]}
	/declare int_cmbtBuffTotal			int		outer	${Ini[botINI,CombatBuffs,int_cmbtBuffTotal,0]}
	/declare cmbt						int		local
	/declare cmbtG						int		local



	/for cmbt 1 to ${int_cmbtBuffTotal} 
		/declare CMBTBUFF_${cmbt} 				string	outer	${Ini[botINI,CombatBuffs,CMBTBUFF_${cmbt}]}
		/declare CMBTBUFF_${cmbt}_Use 			bool	outer	${Ini[botINI,CombatBuffs,CMBTBUFF_${cmbt}_Use]}
		/declare CMBTBUFF_${cmbt}_Gem 			string	outer	${Ini[botINI,CombatBuffs,CMBTBUFF_${cmbt}_Gem]}
		/declare CMBTBUFF_${cmbt}_Active		bool	outer 	FALSE
		/declare CMBTBUFF_${cmbt}_Timer			timer   outer   ${Spell[CMBTBUFF_${cmbt}].Duration} * 6
		/for cmbtG 0 to ${Group}
			/declare CMBTBUFF_${cmbt}_${cmbtG}_Timer	timer   outer   0
		/next cmbtG
	
	/next cmbt


	/cecho \aoxbot_initialize_buffing
	/call InitEvents_Buffing
	
/return









