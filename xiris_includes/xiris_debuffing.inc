|**
----------------------------
xiris_debuffing.inc
----------------------------
This library supplements/abstracts mq2cast functionality
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	INITIAL REVISION
	03.31.22	xiris	Updated the named checking as well as normalizing all calls to MQ2Cast
	05.08.22	xiris	Added support for bards in Debuffs
	05.13.22	xiris   Cleaned up events
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
	07.29.22	xiris   Updated CastDebuff to handle some more error states (So we don't try to debuff failed targets)
	11.08.22	xiris 	Added SlowTarget to manually slow a target with a resist type
	11.14.22	xiris 	Updated chkDebuffsOnSingleTarget to not continually fire if debuffs have already been attempted
	11.15.22	xiris 	Updated DEBUFF INI file entries and code to look more like NUKE/DOT etc ie: single lines of ini entry to define
	02.02.23	xiris 	Updated logic in chkDebuffsOnSingleTarget to call CastDebuffsOnTarget if we have more than 1 debuff, which then
						will check if the other debuffs fufill requirements. This is to allow for toons with a single debuff to not
						have to call the larger logic block of CastDebuffsOnTarget, as it will check inline for the total=1 && trigger logic
	03.08.23	xiris	Removed Trigger parsing, now uses set method chkDebuffTrigger which has everything enumerated
						based on a key _TYPE in the ini. THis is because parsing the trigger is a PITA, and didn't work well
**|

#EVENT DebuffTarget	"#1# tells you#*#'DebuffTarget #2#'"
#EVENT DebuffTarget	"#1# tells the group#*#'DebuffTarget #2#'"
#EVENT DebuffTarget	"#1# tells the raid#*#'DebuffTarget #2#'"
#EVENT DebuffTarget "[ #1# #*#]#*#DebuffTarget #2#"

#EVENT SlowTarget 	"#1# tells you#*#'SlowTarget #2#'"
#EVENT SlowTarget	"#1# tells the group#*#'SlowTarget #2#'"
#EVENT SlowTarget	"#1# tells the raid#*#'SlowTarget #2#'"
#EVENT SlowTarget 	"[ #1# #*#]#*#SlowTarget #2#"

#EVENT TestDebuff   "#*#TestDebuff#*#"

Sub initEvents_debuffing
	/call RegisterRaidEvents Events_debuffing_Raid 3	
/return

Sub Events_debuffing_Raid
	/doevents DebuffTarget
	/doevents SlowTarget
	/doevents TestDebuff
/return



Sub xbot_initialize_debuffing
	/echo \n \apInitialize: \aoDeBuffing \aw-------------------------

	| process the character ini file
	/call processDebuffs

	| initialize the buffing events
	/call initEvents_debuffing
/return


Sub processDebuffs
	/echo \awprocessDebuffs

	/declare raw_debuff[100]		string 	local NULL
	/declare i						int 	local 0
	/declare sz 					int     local 0
    /declare test                   string  local NULL

	| debuffs
    /declare timer_check_debuff		timer   outer   0
    /declare int_debuff_total		int     outer   0
	/declare bln_debuff_enabled		bool	outer	TRUE
	/declare int_debuff_minMana		int     outer   0
	/declare int_debuff_startHP		int     outer   99
	/declare int_debuff_stopHP		int     outer   0
	/declare bln_debuff_automatic  	bool    outer   TRUE

	|; _enabled|_minMana|_startHP|_stopHP|_automatic
	|settings=TRUE|0|99|5|TRUE
    /if (${Ini[INI_CHAR,DEBUFFS,settings].Arg[1,|]})  /varset bln_targetedBuffs_enabled     ${Ini[INI_CHAR,DEBUFFS,settings].Arg[1,|]}
    /if (${Ini[INI_CHAR,DEBUFFS,settings].Arg[2,|]})  /varset int_debuff_minMana  			${Ini[INI_CHAR,DEBUFFS,settings].Arg[2,|]}
    /if (${Ini[INI_CHAR,DEBUFFS,settings].Arg[3,|]})  /varset int_debuff_startHP			${Ini[INI_CHAR,DEBUFFS,settings].Arg[3,|]}
    /if (${Ini[INI_CHAR,DEBUFFS,settings].Arg[4,|]})  /varset int_debuff_stopHP  			${Ini[INI_CHAR,DEBUFFS,settings].Arg[4,|]}
	/if (${Ini[INI_CHAR,DEBUFFS,settings].Arg[5,|]})  /varset bln_debuff_automatic  		${Ini[INI_CHAR,DEBUFFS,settings].Arg[5,|]}

	/echo \a-w    Settings Processed: \awDEBUFF Enabled:\ag${bln_targetedBuffs_enabled}  \awMinMana:\ag${int_debuff_minMana} \awStart:\ag${int_debuff_startHP} \awStop:\ag${int_debuff_stopHP} \awAutomatic:\ag${bln_debuff_automatic}


	|; _debuff|_enabled|_type|_gem|_maxTries|_namedOnly|_startHP|_stopHP|_hasCastOnce|_delay
	|DEBUFF_1="Malos"|TRUE|MALOS|8|2|TRUE|99|10|FALSE|0|
	
	/for i 1 to ${raw_debuff.Size}
        /varset test ${Ini[INI_CHAR,DEBUFFS,DEBUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_debuff_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare DEBUFF_${i}				string	outer   ${test.Arg[1,|]}
		/declare DEBUFF_${i}_enabled		string  outer   ${test.Arg[2,|]}
		/declare DEBUFF_${i}_type			string  outer   ${test.Arg[3,|]}
		/declare DEBUFF_${i}_gem			string  outer   ${test.Arg[4,|]}
		/declare DEBUFF_${i}_maxTries		string  outer   ${test.Arg[5,|]}
		/declare DEBUFF_${i}_namedOnly		string  outer   ${test.Arg[6,|]}
		/declare DEBUFF_${i}_startHP		string  outer   ${test.Arg[7,|]}
		/declare DEBUFF_${i}_stopHP			string  outer   ${test.Arg[8,|]}
		/declare DEBUFF_${i}_hasCastOnce	string  outer   ${test.Arg[9,|]}
		/declare DEBUFF_${i}_delay			string  outer   ${test.Arg[10,|]}
        /echo \ao        DEBUFF_[\ag${i}\ao]: \awDEBUFF:\ag${DEBUFF_${i}} \awType:\ag${DEBUFF_${i}_type}
	/next i
	
/return

Sub EVENT_SlowTarget(string _line, string _sender, string _eventParams) 
	/if (${Me.Class.ShortName.NotEqual[SHM]} || ${Me.Class.ShortName.NotEqual[ENC]}) /return
	/call SlowTarget ${_eventParams}
	/doevents flush SlowTarget
/return

Sub SlowTarget(string _resist)
	/if (${Me.Class.ShortName.NotEqual[SHM]} || ${Me.Class.ShortName.NotEqual[ENC]} || ${Me.Class.ShortName.NotEqual[BRD]}) /return
	/declare db int local 
	/for db 1 to ${int_debuff_total}
		/if (${DEBUFF_${db}_type.Find[SLOW]} && ${DEBUFF_${db}_ResistType.Find[${_resist}]} && ${DEBUFF_${db}_Trigger.Replace[#,$]}) {
			/call chkSpellMemorized "DEBUFF_${db}" TRUE ${DEBUFF_${db}_gem} 
			/call MQ2Cast "DEBUFF_${db}" gem${DEBUFF_${db}_gem} 
		}
	/next i
/return

|-- Do not use the target array to check the targets, lets just check a single (current) target
|   Useful for bards, and others that do not need to AE/Mez/Slow whatever will check if engaged on current target
|   Note we currently disabled the array of targets so this is the only debuff check method
Sub chkDebuffsOnSingleTarget(int _targetID)
	/if (!${int_debuff_total} || !${_targetID} || ${Target.Name.Find[s_corpse]} || !${Spawn[id ${_targetID}].LineOfSight}) /return
	/if (${timer_check_debuffs}) /return
	
	/call CastDebuffsOnTarget ${_targetID} FALSE
	:setTimer 
	/varset timer_check_debuffs 10s
/return


|-- Loop through all debuffs, and attempt to cast one at a time
|   Will check to see if the target already has the debuff and if so has more than 30s left on it
|**
**|
Sub chkDebuffTrigger(string _key, string _spell)
	/echo \agDebuff Trigger \ao${_key} \ay${_spell}
	/if (${_key.Equal[PUTRID_DECAY]}) {
		|putrid decay
		/if ((!${Target.Maloed.ID} || ${Target.Maloed.Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[disease]}) {
			/return TRUE
		}
	} else /if (${_key.Equal[MALOS]}) { 
		|malos
		|/echo checking malos id?${Target.Maloed.ID} duration? ${Target.Maloed.Duration.TotalSeconds}
		/if (!${Target.Maloed.ID} || ${Target.Maloed.Duration.TotalSeconds} < 30) {
			/return TRUE
		}
	} else /if (${_key.Equal[TURGURS]}) { 
		|turgur
		|/echo checking malos id?${Target.Slowed.ID} duration? ${Target.Slowed.Duration.TotalSeconds} ${str_useResistTypes} ${Target.Maloed.ID}
		/if ((!${Target.Slowed.ID} || ${Target.Slowed.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[BALANCE_DISCORD]}) { 
		|balance of discord
		/if ((!${Target.Slowed.ID} || ${Target.Slowed.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[TASHAN]}) { 
		|howl of tashan
		/if (!${Target.Tashed.ID} || ${Target.Tashed.Duration.TotalSeconds} < 30) {
			/return TRUE
		}
	} else /if (${_key.Equal[DESOLATE_DEEDS]}) { 
		| desolate deeds
		/if ((!${Target.Slowed.ID} || ${Target.Slowed.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[CRIPPLE]}) { 
		| cripple
		/if ((!${Target.Crippled.ID} || ${Target.Crippled.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID} && ${Target.Tashed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[DRUID_SUN]}) { 
		| sun's corona
		/if ((!${Target.Buff[${_spell}].ID} && ${Target.Buff[${_spell}].Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[fire]} && ${Target.Maloed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[DRUID_GLACIER]}) { 
		| glacier breath
		/if ((!${Target.Buff[${_spell}].ID} && ${Target.Buff[${_spell}].Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[cold]} && ${Target.Maloed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[DRUID_VINES]}) { 
		| hungry vines
		/if ((!${Target.Buff[${_spell}].ID} && ${Target.Buff[${_spell}].Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID} && ${Target.Tashed.ID}) {
			/return TRUE
		}
	} else /if (${_key.Equal[HARMONY]}) {
		| harmony of sound 
		/if (${Target.Buff[${_spell}].Duration.TotalSeconds} < 30) {
			/return TRUE
		}
	} else /if (${_key.Equal[REVERSE_DS]}) {
		| mark of the blameless
		/if ((!${Target.Buff[${_spell}].ID} || ${Target.Buff[${_spell}].Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]}) {
			/return TRUE
		}
	}
/return FALSE

|-- Force casting debuffs on target (FORCED cast, will ignore checks)
Sub EVENT_DebuffTarget(string _line, string _sender, string _eventParams)
	/echo Debuffing Target ${int_debuff_total}
	/if (!${int_debuff_total}) /return 
	/call CastDebuffsOnTarget ${_eventParams.Arg[1, ]} TRUE
	/doevents flush DebuffTarget
/return

Sub CastDebuffsOnTarget(int _targetID, bool _forced)
	/echo CastDebuffsOnTarget ${_targetID} ${_forced}
	/if (!${Spawn[id ${_targetID}].LineOfSight}) /return
	| Check if named, or we are forcing this
	/call TargetNamedCheck ${_targetID}
	/declare isNamed	bool	local	${Macro.Return}

	/if (${_forced}) /varset isNamed TRUE

	|-- Global Checking for mana 
	/if (${Me.PctMana} < ${int_debuff_minMana}) /return
	
	|-- Mana Check Passed, Check individual debuffs now
	:debuffTarget
	/declare	db		int		local	${int_debuff_total}
	/declare 	valid   bool    local   FALSE
	/for db 1 to ${int_debuff_total}
		:debuffChecks

		| Forcing Check - if this is forced, just cast the damn thing
		/if (${_forced}) /goto :debuffSpellMemorized

		| Named Check - If this is a named only debuff, and target is not named, skip
		/if (${DEBUFF_${db}_namedOnly} && !${isNamed}) /goto :nextDB

		| Start HP/Stop HP Check
		/call TrueTarget ${_targetID}
		/if (${Target.PctHPs} > ${DEBUFF_${db}_startHP} || ${Target.PctHPs} < ${DEBUFF_${db}_stopHP}) /goto :nextDB

		| Trigger Check based on type (Calls chkCharmTrigger)
		/call chkDebuffTrigger ${DEBUFF_${db}_type} ${DEBUFF_${db}}
		/if (!${Macro.Return}) /goto :nextDB

		| Checks passed, check if the spell is memorized. Skip if not
		:debuffSpellMemorized
		/call chkSpellMemorized "${DEBUFF_${db}}" ${_forced} ${DEBUFF_${db}_gem}
		:debuffSpellReadyWait 
		| If this is a forced cast, wait for the spell to be ready
		/if ((!${Me.SpellReady[${DEBUFF_${db}}]} && ${_forced}) || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
			/delay 2
			/goto :debuffSpellReadyWait
		} 

		:debuffCast
			/call TrueTarget ${_targetID}
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				/call MQ2Medley "${DEBUFF_${db}}" SONG ${_targetID} TRUE
				/docommand /${str_command_channel} << ${Target.Name} >> ${DEBUFF_${db}} 
			} else {
				/call CastDebuff "${DEBUFF_${db}}" ${DEBUFF_${db}_gem} ${DEBUFF_${db}_maxTries} ${_targetID} ${_forced}
			}
		

		/doevents GazeYou
	:nextDB
		/call RunRaidEvents
		/next db
	:return
/return



|-- Cast a Specific Debuff spell on the target
Sub CastDebuff(string _debuff, string _gem,  int _maxTries, int _targetID, bool _forced)
	/echo \aoCastDebuff \aw_spell: \ag${_debuff}\aw, _gem: \ag${_gem}\aw, _maxTries: \ag${_maxTries}\aw, _targetID: \ag${_targetID}\aw, _forced: \ag${_forced}
	
	/declare	try		int		local	0
	/declare    tgtID   int     local ${_targetID}
	/declare 	rslt	string  local

	/if (!${Target.ID} || (${Target.ID}  && (${Target.ID} != ${_targetID}))) {
		/echo \aoWRONG TARGET\aw Re-target ${Spawn[npc id ${_targetID}]}(${_targetID})
		/call TrueTarget ${_targetID}
	} 
	
	/declare 	tgtName string  local ${Target.CleanName}
	:AttemptDebuff
		| Run the rapid events before we retry or anything else
		/call RunPriorityEvents

		/echo \aoAttempting Debuff: \aw${_debuff} on \ao${Spawn[id ${_targetID}].Name}

		/if (${_targetID} == 0 && !${_forced}) {
			/echo \ar Debuff Failure\aw Target Already Dead
			/varset rslt TARGET_ALREADY_DEAD
			/goto :DebuffFailure
		}
		|pre-check before attempt
		/if (${Target.PctHPs} == 0 || !${Target.ID} || (${Target.ID} != ${tgtID}) || ${Target.Name.Find['s_corpse']} || ${Target.Type.Equal[corpse]} || ${Target.Type.Equal[pc]}) {
			/echo \ar Debuff Failure\aw Target Error
			/varset rslt TARGET_ERROR
			/goto :DebuffFailure
		}
		/if (${try} > ${_maxTries}) {
			/echo \ar Debuff Failure\aw Max Tries reached
			/varset rslt MAX_TRIES
			/goto :DebuffFailure
		}				
		|if I am a cleric, check MT health
		/if (${Me.Class.ShortName.Equal[CLR]}) /call chkHeal_MT	

		|attempt
		/call MQ2Cast "${_debuff}" gem${_gem} 5s RunPriorityEvents
		/varset rslt ${Macro.Return}
		/varcalc try ${try}+1

		|handle result
		/if (${rslt.Equal[CAST_OUTOFRANGE]}) 	/goto :DebuffFailure
		/if (${rslt.Equal[CAST_OUTOFMANA]}) 	/goto :DebuffFailure
		/if (${rslt.Equal[CAST_NOTHOLD]}) 		/goto :DebuffFailure
		/if (${rslt.Equal[CAST_INVULNERABLE]}) 	/goto :DebuffFailure
		/if (${rslt.Equal[CAST_NOTARGET]}) 		/goto :DebuffFailure
		/if (${rslt.Equal[CAST_IMMUNE]}) 		/goto :DebuffFailure
		/if (${rslt.Equal[CAST_SUCCESS]}) 		/goto :DebuffSuccess
		|retry
		
		/goto :AttemptDebuff
	:DebuffFailure
		/dgt DEBUFF_FAILURE::[${_debuff}] ! ${tgtName} (result:${rslt})
		/goto :return
	
	:DebuffSuccess
		/dgt DEBUFF_SUCCESS::[${_debuff}] -> ${tgtName} (try:${try})
:return
/return

| Reloads entire debuff lineup from INI
| Note the method addSpellToLineup is found in the xiris_casting.inc
Sub CreateDebuffLineup
	/declare i 							int		local
	|-- Debuffs
	/declare int_debuff_total				int		outer	${Ini[INI_CHAR,DEBUFF,int_debuff_total,0]}
	/declare DEBUFF_HasCastOnce			bool 	outer	FALSE
	/declare DEBUFF_Mana_Minimum		int 	outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_Mana_Minimum,10]}
	/declare DEBUFF_Mana_NamedMinimum	int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_Mana_NamedMinimum,10]} 
	/declare DEBUFF_startHP				int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_startHP,100]}
	/declare DEBUFF_stopHP				int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_stopHP,10]}

	/if (${Bool[${int_debuff_total}]}) {
		/for i 1 to ${int_debuff_total} 
			/call addSpellToLineup DEBUFF ${i} ${Ini[INI_CHAR,DEBUFF,DEBUFF_${i}]}
			/declare DEBUFF_${i}_type		string	outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_${i}_type,RESIST_DEBUFF]}
		/next i
	}

/return


