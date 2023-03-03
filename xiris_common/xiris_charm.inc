|**
----------------------------
xiris_charm.inc
----------------------------
This includes the charm setup (see xiris_charm.ini) which has mobs to force 
charm on check (ie: Specialization mote of frost, OMM lieutenants, etc)
______________________________________________
REVISION HISTORY
    06.24.22    xiris	INITIAL REVISION
    06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
**|



#EVENT  CharmON         "#*#CharmON#*#"
#EVENT  CharmOFF        "#*#CharmOFF#*#"

#EVENT  CharmNPCByID    "#1# tells the group#*#'CharmNPCByID #2#'"
#EVENT  CharmNPCByID    "#1# tells you#*#'CharmNPCByID #2#'"
#EVENT  CharmNPCByID    "#1# tells the raid#*#CharmNPCByID #2#'"
#EVENT  CharmNPCByID    "[ #1# #*#]#*#CharmNPCByID #2#"

#EVENT  CharmNPCByName    "#1# tells the group#*#'CharmNPCByName #2#'"
#EVENT  CharmNPCByName    "#1# tells you#*#'CharmNPCByName #2#'"
#EVENT  CharmNPCByName    "#1# tells the raid#*#CharmNPCByName #2#'"
#EVENT  CharmNPCByName  "[ #1# #*#]#*#CharmNPCByName #2#"

Sub InitEvents_Charm
    /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Me.Class.ShortName.NotEqual[ENC]} && ${Me.Class.ShortName.NotEqual[NEC]}) /return
    /call RegisterBackgroundEvents Events_Charm 4
/return 

Sub Events_Charm 
    /doevents CharmON 
    /doevents CharmOFF
    /doevents CharmNPCByID
    /doevents CharmNPCByName
/return

Sub EVENT_CharmON 
    /echo \agCharming Activated
    /varset bln_activeCharming TRUE
/return 

Sub EVENT_CharmOFF 
    /echo \aoCharming De-activated
    /varset bln_activeCharming FALSE
/return 

Sub EVENT_CharmNPCByID(_line, _chatSender, _eventParams)
    /varset bln_activeCharming TRUE
    /echo \ayCharming NPC by ID \ag${_eventParams}
    /call CharmNPC ${_eventParams}
/return

Sub EVENT_CharmNPCByName(_line, _chatSender,_eventParams)
    /varset bln_activeCharming TRUE
    /echo \ayCharming NPC by Name \ag${_eventParams}
    /call CharmNPC ${Spawn[npc ${_eventParams}].ID}
/return 

Sub chkCharmSpell

/return

Sub CharmNPC(int _targetID, bool _forced, bool _attack, int _attackID)
    /echo \aoCharmNPC\aw(\ag${_targetID}\aw ${bln_activeCharming})
    /if (!${bln_activeCharming} && !${_forced}) /return
    /if (${Me.Pet.ID}) /return

    | /declare CHARM_Template         string  outer   _Spell|_Gem|_MaxTries|_KeepAlive|_Resist|_RecastTimer
    | Check the logic in charm (ie: if we can use ancient if enc, etc)
    :charmTarget
    /declare i int local ${CHARM_Total}
    /for i  1 to ${CHARM_Total}
        /call TrueTarget ${_targetID}
        | Trigger Check (triggers are stored as strings with $ replaced with #. Run this through a replace and check);
		| This is usually a complex string such as CHARM_1_Trigger=(!${Target.Maloed.ID} || ${Target.Buff[${DEBUFF_1_Spell}].Duration.TotalSeconds} < 30)
		/if (!${CHARM_${i}_Trigger}) {
			/echo \ar ${CHARM_${i}_Spell} Failed use Check ${CHARM_${i}_Trigger}
			/goto :nextCharm
		}
        | Checks passed, check if the spell is memorized. Skip if not
		:charmSpellMemorized
			/call chkSpellMemorized "${CHARM_${i}_Spell}" ${_forced} ${CHARM_${i}_Gem}
        :charmSpellReadyWait 
			| If this is a forced cast, wait for the spell to be ready
			/if ((!${Me.SpellReady[${CHARM_${i}_Spell}]} && ${_forced}) || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :charmSpellReadyWait
			} 
        :charmCast
			/echo \agCastDebuff OnTarget:\ao"${CHARM_${i}_Spell}" \awID:\ay${_targetID}  \awforcing?:\ay${_forced}
			/call TrueTarget ${_targetID}

			/if (${Me.Class.ShortName.Equal[BRD]}) {
				/call MQ2Medley SONG "${CHARM_${i}_Spell}" ${_targetID} TRUE
				/docommand /${cmdChannel} << ${Target.Name} >> ${CHARM_${i}_Spell} 
			} else {
				/call CastCharm "${CHARM_${i}_Spell}" ${CHARM_${db}_Gem} ${CHARM_${i}_MaxTries} ${Target.ID} ${_forced}
                /docommand /${cmdChannel} << ${Target.Name} >> ${CHARM_${i}_Spell} 
            }    
    :nextCharm
        /call RunRaidEvents
        /next i
    :return 
    /if (${Me.Pet.ID} && ${Me.Pet.Target.ID} != ${_attackID} && ${_attack}) {
        /echo \awEngaging Target(\ag${_targetID}\aw) with Charmed Pet \ag${Me.Pet.Name}! 
        /shout Engaging Target(${_targetID}) with Charmed Pet ${Me.Pet.Name}! 
        /call EngageTarget ${_targetID}
    } 
/return
    
Sub CastCharm(string _charm, string _gem, int _maxTries, int _targetID, bool _forced)
    /echo \aoCastCharm \aw_spell: \ag${_charm}\aw, _gem: \ag${_gem}\aw, _maxTries: \ag${_maxTries}\aw, _targetID: \ag${_targetID}\aw, _forced: \ag${_forced}
	/declare	try		int		local	0
	/declare    tgtID   int     local ${_targetID}
	/declare 	rslt	string  local

    /if (!${Target.ID} || (${Target.ID}  && (${Target.ID} != ${_targetID}))) {
		/echo \aoWRONG TARGET\aw Re-target ${Spawn[npc id ${_targetID}]}(${_targetID})
		/call TrueTarget ${_targetID}
	} 

    /declare 	tgtName string  local ${Target.CleanName}
    :AttemptCharm 
        /echo \aoAttempting Charm: \aw${_charm} on \ao${Spawn[id ${_targetID}].Name}
		|/call chkTargetStatus
		/if (${_targetID} == 0 && !${_forced}) {
			/echo \ar Charm Failure\aw Target Already Dead
			/varset rslt TARGET_ALREADY_DEAD
			/goto :CharmFailure
		}
		|pre-check before attempt
		/if (${Target.PctHPs} == 0 || !${Target.ID} || (${Target.ID} != ${tgtID}) || ${Target.Name.Find['s_corpse']} || ${Target.Type.Equal[corpse]} || ${Target.Type.Equal[pc]}) {
			/echo \ar Charm Failure\aw Target Error
			/varset rslt TARGET_ERROR
			/goto :CharmFailure
		}
		/if (${try} > ${_maxTries}) {
			/echo \ar Charm Failure\aw Max Tries reached
			/varset rslt MAX_TRIES
			/goto :CharmFailure
		}				

		|attempt
		/call MQ2Cast "${_charm}" ${_gem} 5s
		/varset rslt ${Macro.Return}
		/varcalc try ${try}+1
	
		|handle result
        /if (${rslt.Equal[CAST_RESIST]})        /goto :CharmShield
		/if (${rslt.Equal[CAST_OUTOFRANGE]}) 	/goto :CharmFailure
		/if (${rslt.Equal[CAST_OUTOFMANA]}) 	/goto :CharmFailure
		/if (${rslt.Equal[CAST_NOTHOLD]}) 		/goto :CharmFailure
		/if (${rslt.Equal[CAST_INVULNERABLE]}) 	/goto :CharmFailure
		/if (${rslt.Equal[CAST_NOTARGET]}) 		/goto :CharmFailure
		/if (${rslt.Equal[CAST_IMMUNE]}) 		/goto :CharmFailure
		/if (${rslt.Equal[CAST_SUCCESS]}) 		/goto :CharmSuccess
		|retry
		/goto :AttemptCharm

    :CharmShield 
        /dge war /shield ${Me.Name}
        /goto :AttemptCharm
    :CharmFailure
        /dgt CHARM_FAILURE::[${_charm}] ! ${tgtName} (result:${rslt})
        /shout CHARM_FAILURE::[${_charm}] ! ${tgtName} (result:${rslt})
        /goto :return
    :CharmSuccess
        /dgt CHARM_SUCCESS::[${_charm}] -> ${tgtName} (try:${try})
        /shout CHARM_SUCCESS::[${_charm}] -> ${tgtName} (try:${try})
    :return
/return

| Reloads entire charm lineup from INI
| Note the method addSpellToLineup is found in the xiris_casting.inc
Sub CreateCharmLineup
    /declare i                      int     local
    | Charms
    /declare CHARM_Total            int     outer   ${Ini[INI_CHAR,CHARM,CHARM_Total,0]}
    /declare CHARM_ChainCharming    bool    outer   FALSE       

    /echo CHARM_Total: ${CHARM_Total}

    /if (${Bool[${CHARM_Total}]}) {
        /for i 1 to ${CHARM_Total}
            /call addSpellToLineup CHARM ${i} ${Ini[INI_CHAR,CHARM,CHARM_${i}]} 
            /declare CHARM_${i}_Trigger 	string	outer	${Ini[INI_CHAR,CHARM,CHARM_${i}_Trigger,FALSE, noparse]}
            /declare CHARM_${i}_Type  		string	outer	${Ini[INI_CHAR,CHARM,CHARM_${i}_Type,NORMAL]}
        /next i
    }
    
/return 


Sub charm_trigger
    | true name
    /if ((!${Me.Pet.ID} || ${Target.Buff[${CHARM_1_SPELL}].Duration.TotalSeconds} < 5) && ${Me.SpellReady[${CHARM_1_Spell}]} && !${Me.SpellReady[${CHARM_2_Spell}]} && ${Target.Tashed.ID})
    |((!#{Me.Pet.ID} || #{Target.Buff[#{CHARM_1_SPELL}].Duration.TotalSeconds} < 5) && #{Me.SpellReady[#{CHARM_1_Spell}]} && !#{Me.SpellReady[#{CHARM_2_Spell}]} && #{Target.Tashed.ID})

    | ancient: voice of Muram
    /if ((!${Me.Pet.ID} || ${Target.Charmed.Duration.TotalSeconds} < 6) && ${Me.SpellReady[${CHARM_2_Spell}]} && ${Target.Tashed.ID})
    |((!#{Me.Pet.ID} || #{Target.Charmed.Duration.TotalSeconds} < 6) && #{Me.SpellReady[#{CHARM_2_Spell}]} && #{Target.Tashed.ID})

/return

Sub xbot_initialize_charming
    /echo \ao xbot_initialize_charming

    |-- Templates
    /declare CHARM_Template         string  outer   _Spell|_Gem|_MaxTries|_KeepAlive|_Resist|_RecastTimer

    |-- Charm Settings/Misc
    /declare bln_useCharm           bool    outer   ${Ini[INI_CHAR,Charm,UseCharm,FALSE]}
    /declare bln_activeCharming     bool    outer   TRUE

    /declare timer_ancientCharmActive       timer   outer   1.5m
    /declare timer_ancientCharmRefresh      timer   outer   2m

    |-- Load the charms set in the INI file
	/call CreateCharmLineup
 
    |-- Init Events
    /call InitEvents_Charm
    /echo \awCharms: \ag${CHARM_Total}

/return 
