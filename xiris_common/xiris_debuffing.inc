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

Sub InitEvents_Debuffing
	/if (${DEBUFF_Total} > 0 ) {
		/call RegisterRaidEvents Events_Debuffing 2
	}	
/return

Sub Events_Debuffing
	/doevents DebuffTarget
	/doevents SlowTarget
	/doevents TestDebuff
/return

|-- Force casting debuffs on target (FORCED cast, will ignore checks)
Sub EVENT_DebuffTarget(_line, _sender, _eventParams)
	/if (!${DEBUFF_Total}) /return 
	/call CastDebuffsOnTarget ${_eventParams.Arg[1, ]} TRUE
	/doevents flush DebuffTarget
/return

Sub EVENT_SlowTarget(_line, _chatSender, _eventParams) 
	/if (${Me.Class.ShortName.NotEqual[SHM]} || ${Me.Class.ShortName.NotEqual[ENC]}) /return
	/call SlowTarget ${_eventParams}
	/doevents flush SlowTarget
/return

Sub SlowTarget(string _resist)
	/if (${Me.Class.ShortName.NotEqual[SHM]} || ${Me.Class.ShortName.NotEqual[ENC]} || ${Me.Class.ShortName.NotEqual[BRD]}) /return
	/declare db int local 
	/for db 1 to ${DEBUFF_Total}
		/if (${DEBUFF_${db}_Type.Find[SLOW]} && ${DEBUFF_${db}_ResistType.Find[${_resist}]} && ${DEBUFF_${db}_Trigger.Replace[#,$]}) {
			/call chkSpellMemorized "DEBUFF_${db} " TRUE DEBUFF_${db}_Gem 
			/call MQ2Cast "DEBUFF_${db}"
		}
	/next i
/return

|-- Do not use the target array to check the targets, lets just check a single (current) target
|   Useful for bards, and others that do not need to AE/Mez/Slow whatever will check if engaged on current target
|   Note we currently disabled the array of targets so this is the only debuff check method
|   Additionally once debuffs have been cast (either sucesffully or through max tries) the str_debuffedTargets is updated
|   with the target ID, so we don't continually try to debuff the thing
Sub chkDebuffsOnSingleTarget(int _targetID)
	/if (!${DEBUFF_Total} || !${_targetID} || ${Target.Name.Find[s_corpse]}) /return
	/if (!${Spawn[id ${_targetID}].LineOfSight}) /return
	/if (${DEBUFF_Total} == 1 && !${DEBUFF_1_Trigger}) /return
	/call CastDebuffsOnTarget ${_targetID} FALSE
	
/return


|-- Loop through all debuffs, and attempt to cast one at a time
|   Will check to see if the target already has the debuff and if so has more than 30s left on it
|**
**|
Sub debuffs_trigger
	|putrid decay
	/if ((!${Target.Maloed.ID} || ${Target.Maloed.Duration.TotalSeconds} < 30) && ${str_useResistTypes.Find[disease]})
	|((!#{Target.Maloed.ID} && #{Target.Maloed.Duration.TotalSeconds} < 30) && #{str_useResistTypes.Find[disease]})

	|malos
	/if (!${Target.Maloed.ID} || ${Target.Maloed.Duration.TotalSeconds} < 30)
	|(!#{Target.Maloed.ID} && #{Target.Maloed.Duration.TotalSeconds} < 30)

	|turgur
	/if ((!${Target.Slowed.ID} || ${Target.Slowed.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID})
	|((!#{Target.Slowed.ID} || #{Target.Slowed.Duration.TotalSeconds} < 10) && #{str_useResistTypes.Find[magic]} && #{Target.Maloed.ID})

	|balance of discord
	/if ((!${Target.Slowed.ID} || ${Target.Slowed.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID})
	|((!#{Target.Slowed.ID} || #{Target.Slowed.Duration.TotalSeconds} < 10) && #{str_useResistTypes.Find[magic]} && #{Target.Maloed.ID})
	
	|howl of tashan
	/if (!${Target.Tashed.ID} || ${Target.Tashed.Duration.TotalSeconds} < 30)
	|(!#{Target.Tashed.ID} && #{Target.Tashed.Duration.TotalSeconds} < 30)

	| desolate deeds
	/if ((!${Target.Slowed.ID} || ${Target.Slowed.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID})
	|((!#{Target.Slowed.ID} || #{Target.Slowed.Duration.TotalSeconds} < 10) && #{str_useResistTypes.Find[magic]} && #{Target.Maloed.ID})

	| cripple
	/if ((!${Target.Crippled.ID} || ${Target.Crippled.Duration.TotalSeconds} < 10) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID} && ${Target.Tashed.ID})
	|((!#{Target.Crippled.ID} || #{Target.Crippled.Duration.TotalSeconds} < 10) && #{str_useResistTypes.Find[magic]} && #{Target.Maloed.ID} && #{Target.Tashed.ID})

	| sun's corona
	/if ((!${Target.Buff[${DEBUFF_1_Spell}].ID} && ${Target.Buff[${DEBUFF_1_Spell}].Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[fire]} && ${Target.Maloed.ID})
	|((!#{Target.Buff[#{DEBUFF_1_Spell}].ID} && #{Target.Buff[${DEBUFF_1_Spell}].Duration.TotalSeconds} < 20) && #{str_useResistTypes.Find[fire]} && #{Target.Maloed.ID})

	| glacier breath
	/if ((!${Target.Buff[${DEBUFF_2_Spell}].ID} && ${Target.Buff[${DEBUFF_2_Spell}].Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[cold]} && ${Target.Maloed.ID})
	|((!#{Target.Buff[#{DEBUFF_2_Spell}].ID} || #{Target.Buff[#{DEBUFF_2_Spell}].Duration.TotalSeconds} < 20) && #{str_useResistTypes.Find[cold]} && #{Target.Maloed.ID})
	

	| hungry vines
	/if ((!${Target.Buff[${DEBUFF_3_Spell}].ID} && ${Target.Buff[${DEBUFF_3_Spell}].Duration.TotalSeconds} < 20) && ${str_useResistTypes.Find[magic]} && ${Target.Maloed.ID} && ${Target.Tashed.ID})
	|((!#{Target.Buff[#{DEBUFF_3_Spell}].ID} && #{Target.Buff[#{DEBUFF_3_Spell}].Duration.TotalSeconds} < 20) && #{str_useResistTypes.Find[magic]} && #{Target.Maloed.ID} && #{Target.Tashed.ID})

/return

Sub CastDebuffsOnTarget(int _targetID, bool _forced)
	/if (!${Spawn[id ${_targetID}].LineOfSight}) /return
	| Check if named, or we are forcing this
	/call TargetNamedCheck ${_targetID}
	/declare isNamed	bool	local	${Macro.Return}

	/if (${_forced}) /varset isNamed TRUE

	|-- Global Checking for mana 
	/if (${isNamed} && ${Me.PctMana} < ${DEBUFF_Mana_NamedMinimum}) /return
	/if (!${isNamed} && ${Me.PctMana} < ${DEBUFF_Mana_Minimum}) /return
	
	|-- Mana Check Passed, Check individual debuffs now
	:debuffTarget
	/declare	db		int		local	${DEBUFF_Total}
	/for db 1 to ${DEBUFF_Total}
		/call RunRaidEvents
		:debuffChecks

		| Forcing Check - if this is forced, just cast the damn thing
		/if (${_forced}) /goto :debuffSpellMemorized

		| Named Check - If this is a named only debuff, and target is not named, skipp
		/if (${DEBUFF_${db}_NamedOnly} && !${isNamed}) /goto :nextDB

		| Start HP/Stop HP Check
		/call TrueTarget ${_targetID}
		/if (${Target.PctHPs} > ${DEBUFF_${db}_StartHP} || ${Target.PctHPs} < ${DEBUFF_${db}_StopHP}) /goto :nextDB

		| Trigger Check (triggers are stored as strings with $ replaced with #. Run this through a replace and check);
		| This is usually a complex string such as DEBUFF_1_Trigger=(!${Target.Maloed.ID} || ${Target.Buff[${DEBUFF_1_Spell}].Duration.TotalSeconds} < 30)
		

		/echo \ag ${DEBUFF_${db}_Trigger} 
		/echo \ao ${DEBUFF_${db}_Trigger.Replace[#,$]}
		/echo \ar !${DEBUFF_${db}_Trigger}

		
		/if (!${DEBUFF_${db}_Trigger}) {
			/echo \ay ${DEBUFF_${db}_Spell} Failed use Check ${DEBUFF_${db}_Trigger}
			/goto :nextDB
		}

		| Checks passed, check if the spell is memorized. Skip if not
		:debuffSpellMemorized
			/call chkSpellMemorized "${DEBUFF_${db}_Spell}" ${_forced} ${DEBUFF_${db}_Gem}
		:debuffSpellReadyWait 
			| If this is a forced cast, wait for the spell to be ready
			/if ((!${Me.SpellReady[${DEBUFF_${db}_Spell}]} && ${_forced}) || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :debuffSpellReadyWait
			} 

		:debuffCast
			
			/call TrueTarget ${_targetID}
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				/call MQ2Medley SONG "${DEBUFF_${db}_Spell}" ${_targetID} TRUE
				/docommand /${cmdChannel} << ${Target.Name} >> ${DEBUFF_${db}_Spell} 
			} else {
				/call CastDebuff "${DEBUFF_${db}_Spell}" ${DEBUFF_${db}_Gem} ${DEBUFF_${db}_MaxTries} ${_targetID} ${_forced}
			}
	:nextDB
		/call RunAllEvents
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
		|/echo \aoAttempting Debuff: \aw${_debuff} on \ao${Spawn[id ${_targetID}].Name}
		|/call chkTargetStatus
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
		/call MQ2Cast "${_debuff}" ${_gem} 5s
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
	/varset str_debuffedTargets "${str_debuffedTargets},${_targetID}"
/return

| Reloads entire debuff lineup from INI
| Note the method addSpellToLineup is found in the xiris_casting.inc
Sub CreateDebuffLineup
	/declare i 							int		local
	|-- Debuffs
	/declare DEBUFF_Total				int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_Total,0]}
	/declare DEBUFF_HasCastOnce			bool 	outer	FALSE
	/declare DEBUFF_Mana_Minimum		int 	outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_Mana_Minimum,10]}
	/declare DEBUFF_Mana_NamedMinimum	int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_Mana_NamedMinimum,10]} 
	/declare DEBUFF_StartHP				int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_StartHP,100]}
	/declare DEBUFF_StopHP				int		outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_StopHP,10]}

	/if (${Bool[${DEBUFF_Total}]}) {
		/for i 1 to ${DEBUFF_Total} 
			/call addSpellToLineup DEBUFF ${i} ${Ini[INI_CHAR,DEBUFF,DEBUFF_${i}]}
			/declare DEBUFF_${i}_Trigger	string	outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_${i}_Trigger,FALSE, noparse]}
			/declare DEBUFF_${i}_Type		string	outer	${Ini[INI_CHAR,DEBUFF,DEBUFF_${i}_Type,RESIST_DEBUFF]}
		/next i
	}

/return


Sub xbot_initialize_debuffing
	/echo \ao xbot_initialize_debuffing

	|-- Templates
	/declare DEBUFF_Template			string outer	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	
	|-- Debuff Settings/Misc
	/declare str_debuffedTargets		string  outer   NULL

	|-- Return out if not a caster
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return

	|-- Load the debuffs set in the INI file
	/call CreateDebuffLineup

	|-- Init Events
	/call InitEvents_Debuffing
	/echo \awDebuffs: \ag${DEBUFF_Total}
	
/return


