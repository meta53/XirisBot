|**
----------------------------
xiris_casting.inc
----------------------------
SPELL/CASTING HANDLERS. This library supplements/abstracts mq2cast functionality
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	Added QuickNukes and AENukes
	03.31.22	xiris	Updated the named checking as well as normalizing all calls to MQ2Cast
	04.16.22	xiris	Added chkStun and support for Stun Locking
	05.08.22	xiris	Added support for bards in Nuke/DOT
	06.13.22	xiris   Removed resist events as the xiris_spell_routines.inc handles that.
	12.31.22	xiris	Changed Bards medley calls to use new method in spell_routines.inc
	07.30.23	xiris	Rewrite using new INI structures, culled > 700 LOC
**|

|RESIST TYPE stuff
#EVENT SetResistType     	"#1# tells the group#*#'ResistTypes #2#'"
#EVENT SetResistType		"#1# tells you#*#'ResistTypes #2#'"
#EVENT SetResistType		"#1# tells the raid#*#ResistTypes #2#'"
#EVENT SetResistType 		"[ #1# #*#]#*#ResistTypes #2#"

|SETTING Triggers
#EVENT SetUseFastOnly     	"#1# tells the group#*#'UseFastOnly #2#'"
#EVENT SetUseFastOnly		"#1# tells you#*#'UseFastOnly #2#'"
#EVENT SetUseFastOnly		"#1# tells the raid#*#UseFastOnly #2#'"
#EVENT SetUseFastOnly 		"[ #1# #*#]#*#UseFastOnly #2#"

|MISC Casting (usually class based)
#EVENT FIRETL 	            "#*#firetl#*#"
#EVENT DOSHRINK 			"[#1#] doShrink#*#"
#EVENT DOSHRINK 			"<#1#> doShrink#*#"
#EVENT DOSHRINK				"#1# tells the group#*#'doShrink#*#'"
#EVENT DOSHRINK				"#1# tells you#*#'doShrink#*#'"
#EVENT DOSHRINK				"#1# tells the raid#*#'doShrink#*#'"
#EVENT DOSHRINK				"#1# tell your raid#*#'doShrink#*#'"
#EVENT DOSHRINK 			"[ #1# #*#]#*#doShrink#*#"

#EVENT AEON 	            "#*#aeNukeON"
#EVENT AEOFF	            "#*#aeNukeOFF"

Sub initEvents_Casting
    /call RegisterRaidEvents        Events_Casting_Raid
    /call RegisterBackgroundEvents  Events_Casting_Background
/return 

Sub Events_Casting_Raid
    /if (${Select[${Me.Class.ShortName.Equal[WAR,ROG,MNK,BER]}]})   /return
    /if (${bln_engaged} && ${bln_killing} && !${bln_tanking} && !${bln_offTank})      /call chkOffensiveSpells
    /if (${bln_engaged} && ${bln_killing} && !${bln_tanking} && !${bln_offTank})      /call chkDefensiveSpells
    /doevents SetResistType
	/doevents SetUseFastOnly
    /if (${Me.Class.ShortName.Equal[MAG]} || ${Me.Class.ShortName.Equal[WIZ]}) {
        /doevents AEON
        /doevents AEOFF
    } 
/return

Sub Events_Casting_Background 
    /if (${Me.Class.ShortName.Equal[WIZ]}) /doevents FIRETL
/return 


Sub xbot_initialize_casting
    /if (${Select[${Me.Class.ShortName.Equal[WAR,ROG,MNK,BER]}]}) /return
    /echo \n \apInitialize: \aoCasting \aw-------------------------

    |-- Spellset & Resists
    /declare str_casting_spellSet           string  outer   default
    /declare str_casting_spellSet_saved     string  outer   NULL
    /declare str_defaultResistTypes	        string	outer	Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
	/declare str_useResistTypes		        string	outer	${str_defaultResistTypes}
    /declare int_cast_offense_total         int     outer   0
    /declare int_cast_default_gem           int     outer   9
    | Return out for non casters
    /call processCastingTemplates
    /call processCastingSpells
    /call initEvents_Casting
    /memspellset ${str_casting_spellSet}
/return

Sub processCastingTemplates
    /echo \awprocessCastingTemplates
    /declare i                  int local       0
    /declare sz                 int     local   0
    /declare rawSpells[10]      string  outer   NULL
    /declare test               string  local   NULL
    /for i 1 to ${rawSpells.Size}
        /varset test ${Ini[INI_COMMON,OFFENSIVE_SPELL_TYPE,Type_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) /continue
        /varcalc sz ${sz}+1
    /next i

    | Populate Spell Types
    /declare arr_offenseSpellTypes[${sz}] string outer NULL
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /varset test ${Ini[INI_COMMON,OFFENSIVE_SPELL_TYPE,Type_${i}]}
        /varset arr_offenseSpellTypes[${i}] ${test.Arg[1,~]}
    /next i

    | Populate Spell Templates
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /varset test ${Ini[INI_COMMON,OFFENSIVE_SPELL_TYPE,Type_${i}]}
        /declare ${arr_offenseSpellTypes[${i}]}_Template  string outer ${test.Arg[2,~]}
    /next i
/return

Sub processCastingSpells
    /declare i int local 
    | Offensive Spells
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /call processOffensiveSpellSettings ${arr_offenseSpellTypes[${i}]}
    /next i
    /echo \a-w    Offensive Spells Processed: Count:\ag${int_cast_offense_total}
    | Jolt Spells
    /call processDefensiveSettings

/return

| Note defensive settings include AA|DISC|SPELL just like other AA/DISCS
Sub processDefensiveSettings
    /echo \awprocessDefensiveSettings \ay

    /declare bln_cast_JOLT_enabled          bool    outer   FALSE
    /declare int_cast_JOLT_total            int     outer   0
    /declare timer_cast_JOLT                timer   outer   0
    /declare int_cast_JOLT_minMana          int     outer   10
    /declare int_cast_JOLT_startHP          int     outer   99
    /declare int_cast_JOLT_stopHP           int     outer   0  
    

     |; _enabled|_delay|_minMana|_startHP|_stopHP
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[1,|]})  /varset bln_cast_JOLT_enabled    ${Ini[INI_CHAR,JOLT,settings].Arg[1,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[1,|]})  /varset timer_cast_JOLT          ${Ini[INI_CHAR,JOLT,settings].Arg[2,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[3,|]})  /varset int_cast_JOLT_minMana    ${Ini[INI_CHAR,JOLT,settings].Arg[3,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[4,|]})  /varset int_cast_JOLT_startHP    ${Ini[INI_CHAR,JOLT,settings].Arg[4,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[5,|]})  /varset int_cast_JOLT_stopHP     ${Ini[INI_CHAR,JOLT,settings].Arg[5,|]}

        /echo \a-w    Settings Processed: \awJOLT Enabled:\ag${bln_cast_JOLT_enabled} \awTotal:\ag${int_cast_JOLT_total} \awMinMana:\ag${int_cast_JOLT_minMana} \awStart:\ag${int_cast_JOLT_startHP} \awStop:\ag${int_cast_JOLT_stopHP}
    /if (!${bln_cast_JOLT_enabled}) {    
        /varset int_cast_JOLT_total 0
        /echo \a-w        JOLT Processed: Count:\ao[\ag${int_cast_JOLT_total}\ao] \awNo JOLT Spells
        /return
    }

   | ; _jolt|_type|_enabled|_gem|_maxTries|_namedOnly|_startHP|_stopHP|_hasCastOnce|_delay
    /declare raw_jolt[12]		    string 	local NULL
    /declare i						int 	local 0
    /declare j                      int     local 0
	/declare sz 					int     local 0
    /declare test                   string  local   NULL

    /for i 1 to ${raw_jolt.Size}
        /varset test ${Ini[INI_CHAR,JOLT,JOLT_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_cast_JOLT_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
        /declare JOLT_${i}		        string	outer   ${test.Arg[1,|]}
		/declare JOLT_${i}_type		    string	outer   ${test.Arg[2,|]}
        /declare JOLT_${i}_enabled	    string  outer   ${test.Arg[3,|]}
		/declare JOLT_${i}_gem	        string  outer   ${test.Arg[4,|]}
		/declare JOLT_${i}_maxTries	    string  outer   ${test.Arg[5,|]}
        /declare JOLT_${i}_namedOnly	string  outer   ${test.Arg[6,|]}
        /declare JOLT_${i}_startHP	    string  outer   ${test.Arg[7,|]}
        /declare JOLT_${i}_stopHP	    string  outer   ${test.Arg[8,|]}
        /declare JOLT_${i}_hasCastOnce  string  outer   ${test.Arg[9,|]}
        /declare JOLT_${i}_delay	    string  outer   ${test.Arg[10,|]}
        /declare JOLT_${i}_timer        timer   outer   ${JOLT${i}_delay}	
        /echo \ao        JOLT[\ag${i}\ao]:\ag${JOLT_${i}} \awType:\ag${JOLT_${i}_type} \awUse:\ag${JOLT_${i}_enabled} 
    /next i


/return 

Sub processOffensiveSpellSettings(string _type)
    /echo \awprocessOffensiveSpellSettings \ay${_type}

    /declare bln_cast_${_type}_enabled      bool    outer   FALSE
    /declare int_cast_${_type}_total        int     outer   0
    /declare timer_cast_${_type}            timer   outer   0
    /declare int_cast_${_type}_minMana      int     outer   10
    /declare int_cast_${_type}_startHP      int     outer   99
    /declare int_cast_${_type}_stopHP       int     outer   0

    

    |; _enabled|_delay|_minMana|_startHP|_stopHP
    /if (${Ini[INI_CHAR,${_type},settings].Arg[1,|]})  /varset bln_cast_${_type}_enabled    ${Ini[INI_CHAR,${_type},settings].Arg[1,|]}
    /if (${Ini[INI_CHAR,${_type},settings].Arg[3,|]})  /varset int_cast_${_type}_minMana    ${Ini[INI_CHAR,${_type},settings].Arg[3,|]}
    /if (${Ini[INI_CHAR,${_type},settings].Arg[4,|]})  /varset int_cast_${_type}_startHP    ${Ini[INI_CHAR,${_type},settings].Arg[4,|]}
    /if (${Ini[INI_CHAR,${_type},settings].Arg[5,|]})  /varset int_cast_${_type}_stopHP     ${Ini[INI_CHAR,${_type},settings].Arg[5,|]}

    /echo \a-w    Settings Processed: \aw${_type} Enabled:\ag${bln_cast_${_type}_enabled} \awTotal:\ag${int_cast_${_type}_total} \awMinMana:\ag${int_cast_${_type}_minMana} \awStart:\ag${int_cast_${_type}_startHP} \awStop:\ag${int_cast_${_type}_stopHP}
    /if (!${bln_cast_${_type}_enabled}) {    
        /varset int_cast_${_type}_total 0
        /echo \a-w        ${_type} Processed: Count:\ao[\ag${int_cast_${_type}_total}\ao] \awNo ${_type} Spells
        /return
    }


    |;_spell|_enabled|_gem|_maxTries|_namedOnly|_startHP|_stopHP|_hasCastOnce|_delay
    /declare raw_casting[12]		string 	local NULL
	/declare i						int 	local 0
    /declare j                      int     local 0
	/declare sz 					int     local 0
    /declare test                   string  local NULL
    /declare template               string  local ${${_type}_Template}
    /declare pcount                 int     local ${template.Count[|]}
    /declare property               string  local NULL
    /declare key                    string  local NULL

	/varcalc pcount ${pcount}+1

    /for i 1 to ${raw_casting.Size}
        /varset test ${Ini[INI_CHAR,${_type},${_type}_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_cast_${_type}_total ${sz}
            /echo \a-w        ${_type} Processed: Count:\ao[\ag${sz}\ao]
            /return
        }
        | Not null, set the values based on ${template}
        /varcalc sz ${sz}+1

        |/declare ${_type}_${i}  string  outer  ${template.Arg[${i},|]}
        |/echo \ar ${_type}_${i} ${${_type}_${i}}
        
        /for j 1 to ${pcount}
            /varset key ${_type}_${i}${template.Arg[${j},|]}
            /varset property ${test.Arg[${j},|]}
            /declare ${key} string outer ${property}
        /next j
        | Add the timer based on delay
        /declare ${_type}_${i}_timer timer outer ${${_type}_${i}_delay}
        
        /varcalc int_cast_offense_total ${int_cast_offense_total} + 1 
        /echo \ao        ${_type}_[\ag${i}\ao]: \awspell:\ag${${_type}_${i}_spell} \awenabled:\ag${${_type}_${i}_enabled} \awgem:\ag${${_type}_${i}_gem} \awmaxtries:\ag${${_type}_${i}_maxTries} \awnamedOnly:\ag${${_type}_${i}_namedOnly} \awstartHP:\ag${${_type}_${i}_startHP} \awstopHP:\ag${${_type}_${i}_stopHP} \awhascastonce:\ag${${_type}_${i}_hasCastOnce} \awdelay:\ag${${_type}_${i}_delay}    
    /next i
    
/return 

|**
 ----------------------------
 Check Offensive Spells vs Target
 ----------------------------
 This method checks the target status (named, etc), and loops through the offensive spell types and children
 and calls casts if named and use conditions are met.
 ----------------------------
**|
Sub chkOffensiveSpells
    /if (!${Target.ID} || !${bln_engaged} || !${bln_killing} || ${Target.Type.NotEqual[NPC]} || !${int_cast_offense_total}) /return
    /echo chkOffensiveSpells ${arr_offenseSpellTypes.Size}
    /declare i              int     local 0
    /declare j              int     local 0
    /declare oType          string  local NULL
    /declare key            string  local NULL
    /call TargetNamedCheck
    /declare targetIsNamed  bool    local   ${Macro.Return}
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /varset oType ${arr_offenseSpellTypes[${i}]}

        | Check Settings for this type
        /if (!${bln_cast_${oType}_enabled}) /continue
        /if (${timer_cast_${oType}} ||(${Me.PctMana} < ${int_cast_${oType}_minMana} ||  ${Target.PctHPs} >= ${int_cast_${oType}_startHP})) /continue

        /for j 1 to ${int_cast_${arr_offenseSpellTypes[${i}]}_total}
            /varset key   ${oType}_${j}

            | Check Triggers for this entry
            /if ((${Target.PctHPs} <= ${${key}_startHP} && ${Target.PctHPs} >= ${${key}_stopHP}) &&!${${key}_enabled} || (!${targetIsNamed} && ${${key}_namedOnly}) || ${Target.Buff[${${key}_spell}].ID} || ${${key}_timer}) /continue

            /call attemptCastingSpell ${key}
			/call RunAllEvents
            /varset timer_cast_${key} ${timer_cast_${key}.OriginalValue}
        /next j
        /varset timer_cast_${oType} ${timer_cast_${oType}.OriginalValue}
    /next i
    
/return



|-- Note the named only check happens in the chkOffensiveSpells subroutine, to minimize calls to TargetNamedCheck sub
Sub attemptCastingSpell(string _key)
    
    /declare s string local           ${${_key}_spell}
    /echo \aoattemptOffensiveSpell[\ag${s}\ao](\ag${${_key}_type}\ao)\ap ${${key}_gem}
    /declare gem        int     local ${${_key}_gem}
    /declare tries      int     local ${${_key}_maxTries}

    | Logic Checks
    /call chkSpellMemorized "${s}" FALSE ${gem}
    /if (${Me.Class.ShortName.Equal[BRD]}) {
        /call MQ2Medley "${s}" SONG ${Target.ID} TRUE
    } else {
        /call MQ2Cast "${s}" gem${gem} 1s chkCastTarget
    }
    /varset ${_key}_hasCastOnce TRUE
    /varset ${_key}_timer       ${${_key}_delay} 
/return 

|-- Reset the timers on everything DOT/DD/STUN
Sub resetCastTimers
	/if (${Select[${Me.Class.ShortName},WAR,ROG,MNK,BER]}) /return

    /declare i      int     local 0
    /declare j      int     local 0

    | offensive spell reset
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /if (!${bln_cast_${arr_offenseSpellTypes[${i}]}_enabled}) /continue
        /for j 1 to ${int_cast_${arr_offenseSpellTypes[${i}]}_total}
            /varset ${arr_offenseSpellTypes[${i}]}_${j}_hasCastOnce FALSE
        /next j
    /next i
	
	/echo       \a-wCasting: Reset all timers
	:return
/return 

|**
 ----------------------------
 Check Defensive Spells vs Target (JOLT)
 ----------------------------
 This method checks the target status (named, etc), and loops through the defensive spell types and children
 and calls casts if named and use conditions are met. Note: these can be AA/DISC/SPELL jolts
 ----------------------------
**|
Sub chkDefensiveSpells
    /if (!${Target.ID} || !${bln_engaged} || ${Target.Type.NotEqual[NPC]} || !${bln_cast_JOLT_enabled}) /return
    /echo chkDefensiveSpells ${int_cast_JOLT_total}
    /declare i      int     local 0
    /declare j      int     local 0
    /declare dtype   string  local NULL
    /declare key    string  local NULL
    /call TargetNamedCheck
    /declare targetIsNamed  bool    local   ${Macro.Return}
                
    /for i 1 to ${int_cast_JOLT_total}
        /varset dtype    JOLT

        | Check Settings for this type
         /if (!${bln_cast_${dtype}_enabled}) /continue
        /if (${timer_cast_${dtype}} || (${Me.PctMana} < ${int_cast_${dtype}_minMana} ||  ${Target.PctHPs} >= ${int_cast_${dtype}_startHP})) /continue

        | Check Each entry for this typoe
        /for j 1 to ${int_cast_${dtype}_total}
            /varset key   ${dtype}_${j}

            | Check Triggers for this entry
            /if ((${Target.PctHPs} <= ${${key}_startHP} && ${Target.PctHPs} >= ${${key}_stopHP}) && !${${key}_enabled} || (!${targetIsNamed} && ${${key}_namedOnly}) || ${${key}_timer}) /continue
            /call attemptCastingDefensiveSpell ${key} ${key}
			/call RunAllEvents
            /varset timer_cast_${key} ${timer_cast_${key}.OriginalValue}
        /next j
    /next i
    /varset timer_cast_${dtype} ${timer_cast_${dtype}.OriginalValue}
/return

| Defensive spells can be ALT/DISC/SPELL
Sub attemptCastingDefensiveSpell(string _key){
    /declare _type string local ${${_key}_type}

   /if (${Me.Class.ShortName.Equal[BRD]}) {
        /if (${_type.Equal[AA]}     &&   ${Me.AltAbilityReady[${_key}]})    /call MQ2Medley "${_key}" ALT  ${int_targetID} TRUE
        /if (${_type.Equal[DISC]}   && ${Me.CombatAbilityReady[${_key}]})   /doability      "${_key}"
        /if (${_type.Equal[ITEM]}   && !${FindItem[=${_key}].TimerReady})   /call MQ2Medley "${_key}" ITEM ${int_targetID} TRUE
        /if (${_type.Equal[SPELL]})                                         /call MQ2Medley "${_key}" SONG
    } else {
        /if (${_type.Equal[AA]}     && ${Me.AltAbilityReady[${_key}]})      /call MQ2Cast   "${_key}" ALT
        /if (${_type.Equal[DISC]}   && ${Me.CombatAbilityReady[${_key}]})   /doability      "${_key}"
        /if (${_type.Equal[ITEM]}   && !${FindItem[=${_key}].TimerReady})   /call MQ2Cast   "${_key}" ITEM
        /if (${_type.Equal[SPELL]})                                         /call MQ2Cast   "${_key}"
    }
    /varset ${_key}_hasCastOnce TRUE
    /varset ${_key}_timer       ${${_key}_delay} 
}


|**
 ----------------------------
 Check Cast Target
 ----------------------------
 This is called (looping) while a spell is being cast
 This will verify if the target is still alive while casting nukes, etc 
 allowing ducking out if npc is dead
 Additionally calls RunPriorityEvents for raid events that need immediate attention (Gaze, etc)
 ----------------------------
**|
Sub chkCastTarget
	/call RunPriorityEvents
	/if (!${Bool[${Me.Casting.ID}]}) /goto :return

	| If the Target is Dead... stop
	/if (${Target.Name.Find[s_corpse]}) /goto :interrupt

	| If the Target has a Stop Cast HP setting (like... say balancer)
	/if (${Target.PctHPs} <= ${int_temp_stopCastPoint}) /goto :interrupt

	/goto :return

	:interrupt
	/stopcast

    :return
/return


|**
 ----------------------------
 Check Spell Memorized
 ----------------------------
 Method will memorize a spell, to the optional attributed gem, 
 and if wait is true, will wait until its ready to return
 note will convert "gem7" to an integer 7 for gem slot
 ----------------------------
**|
Sub chkSpellMemorized(string _spell, bool _wait, string _gem)
    |/echo \a-ochkSpellMemorized  \a-g${_spell}
	/if (${_spell.Equal[NULL]}) {
       /docommand /${str_announce_channel} Ther was an error trying to memorize a NULL spell!;
       /return
    }
	/if (${Me.Gem[${_spell}]})  /return

    /declare int_gem int local ${int_cast_default_gem}
    /if (${_gem.NotEqual[NULL]}) {
         /if (${_gem.Length} > 1) {
            /varset int_gem ${_gem.Right[1]}
         }
    } else {
        /varset int_gem ${_gem}
    } 
   
	:mem
	    /echo \aochkSpellMemorized\ag "${_spell}" \awgem:\ao${int_gem} \awwait?:\ao${_wait}
		/memspell ${int_gem} "${_spell}" 
		/if (!${_wait}) /return

        /echo Waiting for ${_spell}
        /delay 50 ${Me.SpellReady[${_spell}]}
        /echo Spell Memorized
        /if (${Me.SpellReady[${_spell}]}) /return
        /docommand /${str_announce_channel} There was a timeout error trying to memorize "${_spell}"
/return 



|-------------------------------------------------------------------------------- 
| EVENTS: RESISTS, ETC
| Set the resist type for this specific NPC (usually for Anguish)
| CSV value Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
|--------------------------------------------------------------------------------

Sub EVENT_SetResistType(string _line, string _sender, string _eventParams) 
	/if (${str_class_type.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return
	/declare _strResists string local ${_eventParams}
	/echo \awSetting resist types: \ag${_eventParams}
	
	/if (${_strResists.Equal[ALL]}) /varset _strResists ${str_defaultResistTypes}
	/call SetResistTypes ${_strResists}
	/doevents flush SetResistType
/return 

Sub SetResistTypes(string _typesCSV)
	/if (${str_class_type.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return
	/varset str_useResistTypes ${_typesCSV}
/return


Sub SetSpellSet(string _type)
    /varset str_casting_spellSet_saved ${str_casting_spellSet}
	/memspellset ${_type}
	/varset str_casting_spellSet ${_type}
/return 

|-------------------------------------------------------------------------------- 
| EVENTS: CLASS Specific, ETC
| Teleport, other class things
|--------------------------------------------------------------------------------

SUB EVENT_FIRETL
	/doevents flush
	/dgt Firing TL
	/call MQ2Cast "Teleport" gem6
	/delay 1m
	/memspellset ${str_casting_spellSet}
/return


Sub EVENT_DOSHRINK(string _line, string _sender, string _eventParams)
	/echo \ay doShrinkEventCalled
    /if (${FindItemCount[Umbracite Swarm Orb]}) {
		/echo \agShrinking
		/call MQ2Cast "Umbracite Swarm Orb" item
	} else {
		/declare i int local 0
		/for i 0 to 5
			/call TrueTarget ${Group.Member[${i}].ID}
			/delay 5
			/call MQ2Cast "Shrink" gem1 5s
			/delay 10
		/next i
	}
	}
/return