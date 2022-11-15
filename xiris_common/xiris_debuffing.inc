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
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
	07.29.22	xiris   Updated CastDebuff to handle some more error states (So we don't try to debuff failed targets)
	11.08.22	xiris 	Added SlowTarget to manually slow a target with a resist type
	11.14.22	xiris 	Updated chkDebuffsOnSingleTarget to not continually fire if debuffs have already been attempted
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
	/if (${int_debuffTotal} > 0 ) {
		/call RegisterRaidEvents Events_Debuffing 2
	}	
/return

Sub Events_Debuffing
	/doevents DebuffTarget
	/doevents SlowTarget
	/doevents TestDebuff
/return

|-- Force casting debuffs on target (forced to treat as named)
Sub EVENT_DebuffTarget(_line, _sender, _eventParams)
	/if (${int_debuffTotal} == 0) /return 
	/call CastDebuffsOnTarget ${_eventParams.Arg[1, ]} TRUE
/return

Sub EVENT_SlowTarget(_line, _chatSender, _eventParams) 
	/if (${Me.Class.ShortName.NotEqual[SHM]} || ${Me.Class.ShortName.NotEqual[ENC]}) /return
	/call SlowTarget ${_eventParams}
/return

Sub SlowTarget(string _resist)
	/if (${Me.Class.ShortName.NotEqual[SHM]} || ${Me.Class.ShortName.NotEqual[ENC]} || ${Me.Class.ShortName.NotEqual[BRD]}) /return
	/declare db int local 
	/for db 1 to ${int_debuffTotal}
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
	/if (!${int_debuffTotal}) /return
	/if (${str_debuffedTargets.Find[${_targetID}]}) /return
	/call CastDebuffsOnTarget ${_targetID} FALSE
/return


|-- Loop through all debuffs, and attempt to cast one at a time
|   Will check to see if the target already has the debuff and if so has more than 30s left on it
Sub CastDebuffsOnTarget(int _targetID, bool _forced) 	
	| Check if named, or we are forcing this
	/call TargetNamedCheck ${_targetID}
	/declare isNamed	bool	local	${Macro.Return}
	/if (${_forced}) /cecho \agCastDebuffsOnTarget \awID:\ay${_targetID}  \awisNamed:\ay${isNamed}  \awforcing?:\ay${_forced}
	/if (${_forced}) /varset isNamed TRUE


	|-- things passed, lets target the NPC
	:debuffTarget
	/declare	db		int		local	1
	/declare 	debuffDuration int local
	/for db 1 to ${int_debuffTotal}
		:debuffChecks

		| Forcing Check
		/if (${_forced}) /goto :debuffSpellMemorized

		| Named Check
		/if (${DEBUFF_${db}_NamedOnly} && !${isNamed}) /goto :nextDB

		| Use Trigger Check
		/call TrueTarget ${_targetID}
		/if (!${DEBUFF_${db}_Trigger.Replace[#,$]}) {
			/cecho \ar ${DEBUFF_${db}} Failed use Check ${DEBUFF_${db}_Trigger}
			/goto :nextDB
		}
		:debuffSpellMemorized
			/call chkSpellMemorized "${DEBUFF_${db}}" ${_forced} ${DEBUFF_${db}_Gem}
		:debuffSpellReadyWait 
			/if ((!${Me.SpellReady[${DEBUFF_${db}}]} && ${_forced}) || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :debuffSpellReadyWait
			} 

		:debuffCast
			/call TrueTarget ${_targetID}
			/varset debuffDuration ${Spell[${DEBUFF_${db}}].Duration.TotalSeconds}
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				/medley queue "${DEBUFF_${db}}"
				/docommand /${cmdChannel} << ${Target.Name} >> ${DEBUFF_${db}}
				/cecho \ag ${DEBUFF_${db}}  : DELAYING for \ay${debuffDuration}s \agrefresh	
			} else {
				/call CastDebuff "${DEBUFF_${db}}" ${DEBUFF_${db}_Gem} ${DEBUFF_${db}_MaxTries} ${Target.ID}
			}
	:nextDB
		/call RunRaidEvents
		/next db
	:return

/return



|-- Cast a Specific Debuff spell on the target
Sub CastDebuff(string _debuff, string _gem,  int _maxTries, int _targetID)
	/cecho \aoCastDebuff \aw_spell: \ag${_debuff}\aw, _gem: \ag${_gem}\aw, _maxTries: \ag${_maxTries}
	/declare	try		int		local	0
	/declare    tgtID   int     local ${_targetID}
	/declare 	rslt	string  local
	/if (!${Target.ID} || ${Target.ID} != ${_targetID}) /call TrueTarget ${_targetID}
	/declare 	tgtName string  local ${Target.CleanName}
	:AttemptDebuff
		|pre-check before attempt
		/if (${Target.PctHPs} == 0 || !${Target.ID} || (${Target.ID} != ${tgtID}) || ${Target.Name.Find['s_corpse']} || ${Target.Type.Equal[corpse]} || ${Target.Type.Equal[pc]}) {
			/varset rslt TARGET_ERROR
			/goto :DebuffFailure
		}
		/if (${try} > ${_maxTries}) {
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
		/dgt DEBUFF_FAILURE::[${_debuff}] ! ${tgtName} (${rslt})
	/goto :return
	
	:DebuffSuccess
	/dgt DEBUFF_SUCCESS::[${_debuff}] -> ${tgtName} (${try})

:return
/varset str_debuffedTargets "${str_debuffedTargets},${_targetID}"
/return


Sub xbot_initialize_debuffing
	/cecho \ao xbot_initialize_debuffing

	|--Debuff(s)
	/declare str_debuffedTargets		string  outer   NULL
	/declare bln_debuffUse				bool	outer	${Ini[botINI,Debuff,bln_debuffUse,TRUE]}
	/declare bln_debuffActive			bool	outer	FALSE
	/declare int_debuffResist			int		outer	0
	/declare int_tmpIndex				int		outer   0
	
	/declare int_debuffTotal			int		outer	${Ini[botINI,Debuff,int_debuffTotal,0]}
	/declare int_debuffTotalIfNamed		int		outer	${Ini[botINI,Debuff,int_debuffTotalIfNamed,${int_debuffTotal}]}
	/declare int_debuffStartHP			int		outer   ${Ini[botINI,Debuff,int_debuffStartHP,100]}
	/declare int_debuffStopHP			int		outer   ${Ini[botINI,Debuff,int_debuffStartHP,10]}
	
	/declare db							int		local
	/declare dbt string local
	/for db 1 to ${int_debuffTotalIfNamed} 
		/declare DEBUFF_${db} 				string	outer	${Ini[botINI,Debuff,DEBUFF_${db}]}
		/declare DEBUFF_${db}_Type 			string	outer	${Ini[botINI,Debuff,DEBUFF_${db}_Type,RESIST_DEBUFF]}
		/declare DEBUFF_${db}_Use 			bool	outer	${Ini[botINI,Debuff,DEBUFF_${db}_Use,TRUE]}
		/declare DEBUFF_${db}_Trigger 	string	outer		${Ini[botINI,Debuff,DEBUFF_${db}_Trigger,TRUE]}
		/cecho \aw ${db} UseTrigger \ag${DEBUFF_${db}_Trigger}
		/declare DEBUFF_${db}_Gem 			string	outer	${Ini[botINI,Debuff,DEBUFF_${db}_Gem]}
		/declare DEBUFF_${db}_MaxTries 		int		outer	${Ini[botINI,Debuff,DEBUFF_${db}_MaxTries,2]}
		/declare DEBUFF_${db}_NamedOnly		bool	outer	${Ini[botINI,Debuff,DEBUFF_${db}_NamedOnly,TRUE]}
		/declare DEBUFF_${db}_ResistType 	string  outer   ${Spell[${DEBUFF_${db}}].ResistType} 		
	/next db

	/call InitEvents_Debuffing

/return


