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


Sub initEvents_Casting
    /call RegisterRaidEvents Events_Casting_Raid
/return 

Sub Events_Casting_Raid
    /if (${Select[${Me.Class.ShortName.Equal[WAR,ROG,MNK,BER]}]}) /return
    /if (${bln_engaged}) /call chkOffensiveSpells
    /doevents SetResistType
	/doevents SetUseFastOnly
/return

Sub xbot_initialize_casting
    /if (${Select[${Me.Class.ShortName.Equal[WAR,ROG,MNK,BER]}]}) /return
    /echo \n \apInitialize: \aoCasting \aw-------------------------

    |-- Spellset & Resists
    /declare str_casting_spellSet           string  outer   NULL
    /declare str_casting_spellSet_saved     string  outer   NULL
    /declare str_defaultResistTypes	        string	outer	Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
	/declare str_useResistTypes		        string	outer	${str_defaultResistTypes}
    /declare int_cast_offense_total         int     outer   0
    /declare int_cast_default_gem           int     outer   9
    | Return out for non casters
    /call processCastingTemplates
    /call processCastingSpells
    /call initEvents_Casting
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
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /call processOffensiveSpellSettings ${arr_offenseSpellTypes[${i}]}
    /next i
    /echo \a-w    Offensive Spells Processed: Count:\ag${int_cast_offense_total}
/return

Sub processOffensiveSpellSettings(string _type)
    /echo \awprocessOffensiveSpellSettings \ay${_type}

    /declare bln_cast_${_type}_enabled      bool   outer   FALSE
    /declare int_cast_${_type}_total        int    outer   0
    /declare int_cast_${_type}_mana         int    outer   10
    /declare int_cast_${_type}_start        int    outer   99
    /declare int_cast_${_type}_stop         int    outer   0
    

    |; _enabled|_delay|_minMana|_startHP|_stopHP
    /if (${Ini[INI_CHAR,${_type},settings].Arg[1,|]})  /varset bln_cast_${_type}_enabled   ${Ini[INI_CHAR,${_type},settings].Arg[1,|]}
    /if (${Ini[INI_CHAR,${_type},settings].Arg[3,|]})  /varset int_cast_${_type}_mana      ${Ini[INI_CHAR,${_type},settings].Arg[3,|]}
    /if (${Ini[INI_CHAR,${_type},settings].Arg[4,|]})  /varset int_cast_${_type}_start     ${Ini[INI_CHAR,${_type},settings].Arg[4,|]}
    /if (${Ini[INI_CHAR,${_type},settings].Arg[5,|]})  /varset int_cast_${_type}_stop      ${Ini[INI_CHAR,${_type},settings].Arg[5,|]}

    /echo \a-w    Settings Processed: \aw${_type} Enabled:\ag${bln_cast_${_type}_enabled} \awTotal:\ag${int_cast_${_type}_total} \awMinMana:\ag${int_cast_${_type}_mana} \awStart:\ag${int_cast_${_type}_start} \awStop:\ag${int_cast_${_type}_stop}
    
    /if (!${bln_cast_${_type}_enabled}) {    
        /varset int_cast_${_type}_total 0
        /echo \a-w        ${_type} Processed: Count:\ao[\ag${int_cast_${_type}_total}\ao] \awNo ${_type} Spells
        /return
    }


    |;_spell|_use|_gem|_maxTries|_namedOnly|_startHP|_stopHP|_hasCastOnce|_delay
    /declare raw_casting[100]		string 	local NULL
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
            /varset int_cast_${_type}_total ${sz}
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
        /varcalc int_cast_offense_total ${int_cast_offense_total} + 1 
        /echo \ao        ${_type}[\ag${i}\ao]: \awspell:\ag${${_type}_${i}_spell}
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
    /if (!${Target.ID} || !${bln_engaged} || ${Target.Type.NotEqual[NPC]} || !${int_cast_offense_total}) /return
    /echo chkOffensiveSpells ${arr_offenseSpellTypes.Size}
    /declare i      int     local 0
    /declare j      int     local 0
    /call TargetNamedCheck
    /declare targetIsNamed  bool    local   ${Macro.Return}
    /for i 1 to ${arr_offenseSpellTypes.Size}
        /echo enabled ${arr_offenseSpellTypes[${i}]} ${bln_cast_${arr_offenseSpellTypes[${i}]}_enabled} ${int_cast_${arr_offenseSpellTypes[${i}]}_total}
        /if (!${bln_cast_${arr_offenseSpellTypes[${i}]}_enabled}) /goto :nextOffensiveSpell
        /for j 1 to ${int_cast_${arr_offenseSpellTypes[${i}]}_total}
            /if (!${${arr_offenseSpellTypes[${i}]}_${j}_use} || (!${targetIsNamed} && ${${arr_offenseSpellTypes[${i}]}_${j}_namedOnly} || ${Target.Buff[${${arr_offenseSpellTypes[${i}]}_${j}_spell}].ID})) /continue
            /call attemptOffensiveSpell ${arr_offenseSpellTypes[${i}]} ${j}
			/call RunAllEvents
        /next j
    :nextOffensiveSpell
    /next i
/return


|**
 ----------------------------
 Cast Target Loop
 ----------------------------
 This method casts the offensive spell, if the start and stop are within bounds
 and the spell is memorized.
 Note the named only check happens in the chkOffensiveSpells subroutine, to minimize
 calls to TargetNamedCheck sub
 ----------------------------
**|
Sub attemptOffensiveSpell(string _type, int _index)
    
    /declare s string local ${${_type}_${_index}_spell}
    /echo \aoattemptOffensiveSpell[\ag${_index}\ao](\ag${_type}\ao)\ap ${s}

    /declare use        bool    local ${${_type}_${_index}_use}
    /declare gem        int     local ${${_type}_${_index}_gem}
    /declare tries      int     local ${${_type}_${_index}_maxTries}
    /declare start      int     local ${${_type}_${_index}_startHP}
    /declare stop       int     local ${${_type}_${_index}_stopHP}

    | Logic Checks
    /call chkSpellMemorized "${s}" FALSE ${gem}
    /if (${Target.PctHPs} <= ${start} && ${Target.PctHPs} >= ${stop}) {
        /if (${Me.Class.ShortName.Equal[BRD]}) {
            /call MQ2Medley "${s}" SONG ${Target.ID} TRUE
        } else {
            /call MQ2Cast "${s}" ${gem} 1s chkCastTarget
        }
		/varset ${_type}_${_index}_hasCastOnce TRUE
    }
    
/return 



|-- Reset the timers on everything DOT/DD/STUN
Sub ResetCastTimers
	/if (${Select[${Me.Class.ShortName.Equal[WAR,ROG,MNK,BER]}]}) /return

    /declare i      int     local 0
    /declare j      int     local 0

    /for i 1 to ${arr_offenseSpellTypes.Size}
        /for j 1 to ${int_cast_${arr_offenseSpellTypes[${i}]}_total}
            /varset ${arr_offenseSpellTypes[${i}]}_${j}_hasCastOnce FALSE
        /next j
    /next i
	
	/echo       \a-wCasting: Reset all timers
	:return
/return 


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
    /echo \a-ochkSpellMemorized  \a-g${_spell}
	/if (${_spell.Equal[NULL]}) {
       /docommand /${announceChannel} Ther was an error trying to memorize a NULL spell!;
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
        /docommand /${announceChannel} There was a timeout error trying to memorize "${_spell}"
/return 



|-------------------------------------------------------------------------------- 
| EVENTS: RESISTS, ETC
| Set the resist type for this specific NPC (usually for Anguish)
| CSV value Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
|--------------------------------------------------------------------------------

Sub EVENT_SetResistType(_line, _chatSender, _eventParams) 
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
	/memspellset ${_type}
	/varset str_casting_spellSet ${_type}
/return 
