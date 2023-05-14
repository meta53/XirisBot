|**
----------------------------
xiris_buffing.inc
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
	01.02.23	xiris	Removed chkOOC buffs as auto casting due to never using it, and mostly getting in the way.
	01.03.23	xiris	Reworked buffing slightly (naming convention mostly)
**|

#EVENT DORAIDBUFFS			"#1# tells the group#*#'doRaidBuffs #2#'"
#EVENT DORAIDBUFFS			"#1# tells you#*#'doRaidBuffs #2#'"
#EVENT DORAIDBUFFS			"#1# tells the raid#*#'doRaidBuffs #2#'"
#EVENT DORAIDBUFFS 			"[ #1# #*#]#*#doRaidBuffs #2#"

#EVENT DOSINGLETARGETBUFFS	"#1# tells the group#*#'doSingleTargetBuffs #2#'"
#EVENT DOSINGLETARGETBUFFS	"#1# tells you#*#'doSingleTargetBuffs #2#'"
#EVENT DOSINGLETARGETBUFFS	"#1# tells the raid#*#'doSingleTargetBuffs #2#'"
#EVENT DOSINGLETARGETBUFFS 	"[ #1# #*#]#*#doSingleTargetBuffs #2#"

#EVENT DOCHARBUFFS			"#1# tells the group#*#'doCharBuffs #2#'"
#EVENT DOCHARBUFFS			"#1# tells you#*#'doCharBuffs #2#'"
#EVENT DOCHARBUFFS			"#1# tells the raid#*#'doCharBuffs #2#'"
#EVENT DOCHARBUFFS 			"[ #1# #*#]#*#doCharBuffs #2#"

#EVENT REMOVEBUFF			"#1# tells the group#*#REMOVEBUFF #2#'"
#EVENT REMOVEBUFF			"#1# tells you#*#REMOVEBUFF #2#'"
#EVENT REMOVEBUFF			"#1# tells the raid#*#REMOVEBUFF #2#'"
#EVENT REMOVEBUFF 			"[ #1# #*#]#*#REMOVEBUFF #2#"


#EVENT REMOVEALLBUFFS		"#1# tells the group#*#'removeallbuffs#*#'"
#EVENT REMOVEALLBUFFS		"#1# tells you#*#'removeallbuffs#*#'"
#EVENT REMOVEALLBUFFS		"#1# tells the raid#*#'removeallbuffs#*#'"
#EVENT REMOVEALLBUFFS 		"[ #1# #*#]#*#removeallbuffs #*#"



Sub InitEvents_Buffing
	/call RegisterBackgroundEvents Events_Buffing 6
/return

Sub Events_Buffing
	/doevents DORAIDBUFFS
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
	/if (!${bln_doRaidBuff} && !${bln_doSingleTargetBuff}) /return
	/echo Doing Char Buffs on ${_eventParams}
	/memspellset buff
	/delay 3s
	/call doGroupBuffOnTarget ${_eventParams}
	/call doSingleTargetBuffOnChar ${_eventParams}
	/dgt ${Me.Class.Name} CHAR BUFF [${_eventParams}] COMPLETE
	/delay 10
	| Restore the previous spellset
	/memspellset ${str_spellset}
	
	/doevents flush DOCHARBUFFS
/return


Sub EVENT_DOSINGLETARGETBUFFS(_line, _chatSender, _eventParams)
	/if (!${bln_doSingleTargetBuff}) /return

	/memspellset buff
	/if (${Raid.Members}) {
		/call doSingleTargetBuffOnRaid
	} else {
		/call doSingleTargetBuffOnChar ${_eventParams}
	}
	/memspellset ${str_spellset}
	
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
	/memspellset ${str_spellset}
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
 Note: Currently only targets the MT
 ----------------------------
**|
Sub cmbtBuffMT
	/if (!${bln_cmbtBuffUse} || !${int_cmbtBuffTotal}) /return


	|-- check if MT has the single target buff on him (think DI/Pyrillen Skin/Panoply)
	/declare i 			int 	local  
	/declare bSpell		string	local	${CMBTBUFF_1}
	/declare bDuration  string	local
	/declare bGem		string	local	${CMBTBUFF_1_Gem}
	/declare delay		int		local	0

	/for i 1 to ${int_cmbtBuffTotal}
		/varset bSpell 	${CMBTBUFF_${i}}
		/varset bGem 	${CMBTBUFF_${i}_Gem}
		/call chkSpellMemorized "${bSpell}" FALSE ${bGem}
		/if (${Me.SpellReady[${bSpell}]}) {
			/dquery ${str_MTName} -q "Me.Buff[${bSpell}].Duration"
			/delay 20 ${DanNet.Query.Received}
			/varset bDuration ${DanNet.Q}

			/if (!${Bool[${bDuration}]} ) {
				/call TrueTarget ${Spawn[pc =${str_MTName}].ID}
				/call MQ2Cast "${bSpell}" ${bGem}
				/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
					/echo \ag ${bSpell} ${Cast.Result} : DELAYING for \ay ${Spell[${bSpell}].Duration.TotalSeconds}s \agrefresh
				}
			}
		}
	:nextCombatBuff 
	/next i

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
	/if (${int_cmbtBuffTotal} == 0 || ${CMBTBUFF_1_Timer} || !${bln_cmbtBuffUse} || !${bln_engaged}) /return

	|-- loop through group members that get combat buff
	/declare i    		  int		local
	/declare g			  int		local
	/declare gName 	  	  string	local
	/declare gClass       string    local
	/declare buffSpell    string	local
	/declare buffGem	  string	local
	/declare delay		  int       local	0
	/declare bStatus 	  string    local 
	/declare bDuration    int       local
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

			|-- Check to see if member has the buff - if not in zone, skip
			/varset gName ${Group.Member[${g}]}
			/if (!${SpawnCount[pc =${gName}]}) /goto :nextGroupMember
			/varset gClass ${Spawn[pc =${gName}].Class.ShortName}

			/if (${Select[${gClass},WAR,SHD,PAL,MNK,RNG,ROG,BRD,BER]}) {
				/dquery ${gName} -q "Me.Buff[${buffSpell}].Duration.TotalSeconds"
				/delay 20 ${DanNet.Query.Received}
				/varcalc bDuration ${DanNet.Q}
				/echo cmbtBuffGRP: \aw${gName} -q \ag"Me.Buff[${buffSpell}].Duration" == \ar ${bDuration} \awbool \ag${Bool[${bDuration}]}
				/if (${Bool[${bDuration}]} ) {
					
					/if (${bDuration} < 3 ) {
						/echo \awbDuration: \ay${bDuration} \aw<3s, \agrecasting
						/goto :castCMBTBuff
					} else {
						/echo \awDuration: \ay${bDuration} \aw>= 3s, \aoskipping 
						/goto :nextGroupMember
					}
				} else {
					/goto :castCMBTBuff
				}
			} else {
				/goto :nextGroupMember
			}

			:castCMBTBuff
			|/echo \ayCasting ${buffSpell} on ${gName}
			
			/call TrueTarget ${Spawn[pc =${gName}].ID}
			/call MQ2Cast "${buffSpell}" ${buffGem}
			/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
				/echo \ag ${buffSpell} ${Cast.Result} : DELAYING for \ay ${Spell[${buffSpell}].Duration.TotalSeconds}s \agrefresh
				/varset delay	${Spell[${buffSpell}].Duration.TotalSeconds}
				/varset CMBTBUFF_${i}_Timer ${delay}s
			}
		:nextGroupMember
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
	/declare buffUse  			bool    local
	/declare buffGem	  		string	local
	/declare spellReadyCount	int 	local 0
	/target pc ${tgt}
	/delay 10

	/for i 1 to ${int_oocBuffTotal}
		/varset 	buffUse		 ${Bool[${OOCBUFF_${i}_Use}]} 
		/if (!${buffUse}) /goto :NextGB
		/varset 	buffSpell    ${OOCBUFF_${i}}
		/varcalc	buffDuration ${Spell[${OOCBUFF_${i}}].Duration.TotalSeconds}
		/varset		buffGem		 ${OOCBUFF_${i}_Gem}

		|/echo spell:${buffSpell} duration: ${buffDuration}s gem: ${buffGem}
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
					
			/if (${Cast.Result.Equal[CAST_FIZZLE]}) { 		
				/goto :spellReadyWait
			} else /if (${Cast.Result.Equal[CAST_OUTOFMANA]}) { 	
				/goto :spellReadyWait
			} else /if (${Cast.Result.Equal[CAST_CANNOTSEE]}) { 	
				/goto :spellReadyWait
			} else /if (${Cast.Result.Equal[CAST_OUTOFRANGE]}) {	
				/goto :spellReadyWait
			} else /if (${Cast.Result.Equal[CAST_INTERRUPTED]}) {	
				/goto :spellReadyWait
			} else /if (${Cast.Result.Equal[CAST_NOTREADY]}) {	
				/goto :spellReadyWait
			} else /if (${Cast.Result.Equal[CAST_OUTOFREAGENTS]}) {
				/echo \ar ${buffSpell} ${Cast.Result} Disabling
				/docommand /${announceChannel} Out of reagents for ${buffSpell}
				/varset OOCBUFF_${i}_Use FALSE
			} else /if (${Cast.Result.Equal[CAST_SUCCESS]}) {
				/echo \ag ${buffSpell} ${Cast.Result}
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

			/echo \ay${buffSpell} ${buffTargets} ${buffGem} ${buffUse}

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
					/echo \ag${Raid.Member[${i}]} is a valid Single Buff Target
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
			/if (${Cast.Result.Equal[CAST_FIZZLE]})  		/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_OUTOFMANA]})  	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_CANNOTSEE]})  	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_OUTOFRANGE]}) 	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_INTERRUPTED]}) 	/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_NOTREADY]}) 		/goto :spellReadyWait2
			/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
				/echo \ag ${buffSpell} ${Cast.Result}
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



|----------------------------------------------------|
|- Attempts to remove specified buffs on Dannet.	-|
|----------------------------------------------------|
|- Use, /dgt Remove [Name], or /dgt Remove 'Name'.	-|
|----------------------------------------------------|

Sub EVENT_REMOVEBUFF(_line, _chatSender, _eventParams)
	| Note these Buffs need to replace single quote with pipe due to chat mangling quotes: eg Brell|s Brawny Bulwark
	/declare buffToRemove string local ${_eventParams.Arg[1,-]}
	/echo \awEvent Remove Buff called: ${_eventParams} -> ${buffToRemove}
	/call RemoveBuff "${buffToRemove}"
	/doevents flush REMOVEBUFF
/return

Sub EVENT_REMOVEALLBUFFS(_line, _chatSender, _eventParams)
	/call RemoveAllBuffs
	/doevents flush REMOVEALLBUFFS
/return

Sub RemoveAllBuffs
	/declare n int local
	/for n 1 to 25
		/if (${Me.Buff[${n}].ID}) /call RemoveBuff "${Me.Buff[${n}]}"
	/next n
/return

Sub RemoveBuff(_badBuff)
	/declare i int local
	/declare buffName ${_badBuff.Replace[|,']}
	/echo \agRemoving Buff \ao${buffName} (was ${_badBuff})
	/for i 0 to 24
		|/echo Remove_Buff ${i}
		|/echo 1: ${Bool[${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip}]}
		|/echo 2: ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip}
		|/echo 3: ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Find[${buffName}]}
		/if (${Bool[${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip}]} && ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Find[${buffName}]}) {
			/varset buffName ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Arg[1,(]}
			/varset buffName ${buffName.Left[${Math.Calc[${buffName.Length}-1]}]}
			|/echo \awBuff \ao${buffName} \awfound. \agRemoving.
			/if (${Bool[${buffName}]}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[${buffName}].ID}-1].Int} leftmouseup
		}
		
	/next i
/return


|**
----------------------------
Champion|Ferocity Attack Buff Checking
Champion is 1500 mana, so we don't want to cast it unless the timer is up
Ferocity of Irionu is 2 minute recast single target
----------------------------  
**|

Sub chkATTKBuffs
	/if (${ATTK_1_Timer}) /return
	/if (!${Me.SpellReady[${ATTK_1_Spell}]}) /return

	/declare bTarget 	string 	local
	/declare bDuration	int		local 
	/declare bTgtCnt	int     local
	/declare i 			int     local

	|-- Check to see if the Target has the buff
	/for i 1 to ${ATTK_1_TargetCount} 
		/varset bTarget ${ATTK_1_Targets.Token[${i},,]}
		/if (${SpawnCount[pc ${bTarget}]} == 0 || ${Spawn[pc ${bTarget}].Distance3D} > 100) /goto :nextATTKTarget
		/dquery ${bTarget} -q "Me.Buff[${ATTK_1_Spell}].Duration.TotalSeconds"
		/delay 20 ${DanNet.Query.Received}
		/varcalc bDuration ${DanNet.Q}
		|/echo chkATTKBuffs: \aw${bTarget} -q \ag"Me.Buff[${ATTK_1_Spell}].Duration" == \ar ${bDuration} \awbool \ag${Bool[${bDuration}]}
		/if (${Bool[${bDuration}]} ) {
			/if (${bDuration} < 3 ) {
				|/echo \arbDuration < 3! casting!
				/goto :castATTKBuff
			} else {
				|/echo \aghas duration, skipping
				/goto :nextATTKTarget
			}
		} else {
			/goto :castATTKBuff
		}

	:nextATTKTarget
	/next i
	
	
	/goto :return

	:castATTKBuff 
	/echo castATTKBuff: \aw${bTarget} -q \ag"Me.Buff[${ATTK_1_Spell}].Duration" == \ar ${bDuration} \awbool \ag${Bool[${bDuration}]}
	/call TrueTarget ${Spawn[pc =${bTarget}].ID}
	/call MQ2Cast "${ATTK_1_Spell}"
	/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
		/echo \ag ${ATTK_1_Spell} ${Cast.Result} : DELAYING for \ay ${ATTK_1_Recast}s \agrefresh
		/varset ATTK_1_Timer ${ATTK_1_Recast}s
	}

:return
/return


Sub xbot_initialize_buffing
	|/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return
	/echo \aoxbot_initialize_buffing

	/declare i									int		local

	|--OOC Buff(s)
	/declare bln_doRaidBuff						bool	outer	${Ini[INI_CHAR,OOCBuffs,bln_doRaidBuff,FALSE]}
	/declare bln_doGroupBuff					bool	outer	${Ini[INI_CHAR,OOCBuffs,bln_doGroupBuff,FALSE]}
	/declare bln_doAutoBuff						bool	outer	${Ini[INI_CHAR,OOCBuffs,bln_doGroupBuff,FALSE]}
	/declare int_oocBuffTotal					int		outer	${Ini[INI_CHAR,OOCBuffs,int_oocBuffTotal,0]}
	
	/if (${int_oocBuffTotal} > 0) {
		/for i 1 to ${int_oocBuffTotal} 
			/declare OOCBUFF_${i} 				string	outer	${Ini[INI_CHAR,OOCBuffs,OOCBUFF_${i}]}
			/declare OOCBUFF_${i}_Use 			bool	outer	${Ini[INI_CHAR,OOCBuffs,OOCBUFF_${i}_Use,FALSE]}
			/declare OOCBUFF_${i}_Gem 			string	outer	${Ini[INI_CHAR,OOCBuffs,OOCBUFF_${i}_Gem]}
			/declare OOCBUFF_${i}_Active		bool	outer 	FALSE
			/declare OOCBUFF_${i}_Timer			timer   outer   0
		/next i
	}
	
	|--Single Target Buff(s)
	|--eg: Divine Intervention, Mage Huge DS, Ranger AC Buff, Cleric AC buff (if no group version)
	/declare bln_doSingleTargetBuff				bool	outer	${Ini[INI_CHAR,TargetBuffs,bln_doSingleTargetBuff,FALSE]}
	/declare int_stBuffTotal					int		outer	${Ini[INI_CHAR,TargetBuffs,int_stBuffTotal,0]}
	
	/if (${int_stBuffTotal} > 0) {
		/for i 1 to ${int_stBuffTotal} 
			/declare STBUFF_${i} 				string	outer	${Ini[INI_CHAR,TargetBuffs,STBUFF_${i}]}
			/declare STBUFF_${i}_Use 			bool	outer	${Ini[INI_CHAR,TargetBuffs,STBUFF_${i}_Use,FALSE]}
			/declare STBUFF_${i}_Gem 			string	outer	${Ini[INI_CHAR,TargetBuffs,STBUFF_${i}_Gem]}
			/declare STBUFF_${i}_Targets		string	outer	${Ini[INI_CHAR,TargetBuffs,STBUFF_${i}_Targets]}
			/declare STBUFF_${i}_Active			bool	outer 	FALSE
			/declare STBUFF_${i}_Timer			timer   outer   0
		/next i
	}
	
	|--Combat Buff(s)
	|--eg: Spirit of the Jaguar
	/declare bln_cmbtBuffUse					bool	outer	${Ini[INI_CHAR,CombatBuffs,bln_cmbtBuffUse,FALSE]}
	/declare int_cmbtBuffTotal					int		outer	${Ini[INI_CHAR,CombatBuffs,int_cmbtBuffTotal,0]}
	/declare j									int		local

	/if (${int_cmbtBuffTotal} > 0) {
		/for i 1 to ${int_cmbtBuffTotal} 
			/declare CMBTBUFF_${i} 					string	outer	${Ini[INI_CHAR,CombatBuffs,CMBTBUFF_${i}]}
			/declare CMBTBUFF_${i}_Use 				bool	outer	${Ini[INI_CHAR,CombatBuffs,CMBTBUFF_${i}_Use,FALSE]}
			/declare CMBTBUFF_${i}_Gem 				string	outer	${Ini[INI_CHAR,CombatBuffs,CMBTBUFF_${i}_Gem]}
			/declare CMBTBUFF_${i}_Active			bool	outer 	FALSE
			/declare CMBTBUFF_${i}_Timer			timer   outer   ${Spell[CMBTBUFF_${i}].Duration.TotalSeconds}
			/for j 0 to ${Group}
				/declare CMBTBUFF_${i}_${j}_Timer	timer   outer   0
			/next j
		/next i
	}

	|--ATTK Buff(s)
	|--eg: Champion || Spirit of Irionu
	|--Note: only 1 ATK buff is generally used, so no real looping here, but consistency
	/declare bln_useAttackBuff					string	outer	${Ini[INI_CHAR,ATTK,bln_useAttackBuff,FALSE]}
	/declare int_attkBuffTotal					int		outer   1

	/if (${bln_useAttackBuff}) {
		/for i 1 to ${int_attkBuffTotal} 
			/declare ATTK_${i}_Spell 			string	outer	${Ini[INI_CHAR,ATTK,ATTK_${i}]}
			/declare ATTK_${i}_Use 				string	outer	${Ini[INI_CHAR,ATTK,ATTK_${i}_Use,FALSE]}
			/declare ATTK_${i}_Gem 				string	outer	${Ini[INI_CHAR,ATTK,ATTK_${i}_Gem]}
			/declare ATTK_${i}_Type				string	outer	${Ini[INI_CHAR,ATTK,ATTK_${i}_Type,SINGLE]}
			/declare ATTK_${i}_Targets 			string	outer	${Ini[INI_CHAR,ATTK,ATTK_${i}_Targets]}
			/declare ATTK_${i}_Recast 			string	outer	${Ini[INI_CHAR,ATTK,ATTK_${i}_Recast]}
			/declare ATTK_${i}_Timer			timer   outer   0
			/declare ATTK_${i}_TargetCount  	int     outer   ${ATTK_${i}_Targets.Token[1,|]}
			/varset  ATTK_${i}_Targets	${ATTK_${i}_Targets.Token[2,|]}
		/next i
	}

	/call InitEvents_Buffing
	
/return









