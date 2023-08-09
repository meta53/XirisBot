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
						Added Array named arr_raidLeaders to store this manually (laaame)
	03.01.22	xiris	Added Exclusions for Buffing use command /only|EXCLUSIONS eg: doRaidBuffs /only|ENC		
	05.13.22	xiris	Reduced events to BC only		
	06.13.22	xiris	Restored events for Group/Party/Raid due to need to split raids sometimes (multiple camp pulling)	
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
	01.02.23	xiris	Removed chkOOC buffs as auto casting due to never using it, and mostly getting in the way.
	01.03.23	xiris	Reworked buffing slightly (naming convention mostly)
	07.21.23	xiris 	Rewrite using new INI structures
**|

#EVENT Buffing_Buff_OnRaid		"#1# tells the group#*#'doRaidBuffs#*#'"
#EVENT Buffing_Buff_OnRaid		"#1# tells you#*#'doRaidBuffs#*#'"
#EVENT Buffing_Buff_OnRaid		"#1# tells the raid#*#'doRaidBuffs#*#'"
#EVENT Buffing_Buff_OnRaid		"[ #1# #*#]#*#doRaidBuffs#*#"

#EVENT Buffing_Buff_OnChar	    "#1# tells the group#*#'doCharBuffs #2#'"
#EVENT Buffing_Buff_OnChar	    "#1# tells you#*#'doCharBuffs #2#'"
#EVENT Buffing_Buff_OnChar	    "#1# tells the raid#*#'doCharBuffs #2#'"
#EVENT Buffing_Buff_OnChar	    "[ #1# #*#]#*#doCharBuffs #2#"

#EVENT Buffing_DS_Disable	    "#1# tells the group#*#'NoDS#*#'"
#EVENT Buffing_DS_Disable	    "#1# tells you#*#'NoDS#*#'"
#EVENT Buffing_DS_Disable	    "#1# tells the raid#*#'NoDS#*#'"
#EVENT Buffing_DS_Disable	    "[ #1# #*#]#*#NoDS#*#"

#EVENT Buffing_DS_Enable	    "#1# tells the group#*#'YesDS #*#'"
#EVENT Buffing_DS_Enable	    "#1# tells you#*#'YesDS #*#'"
#EVENT Buffing_DS_Enable	    "#1# tells the raid#*#'YesDS #*#'"
#EVENT Buffing_DS_Enable	    "[ #1# #*#]#*#YesDS#*#"

#EVENT Buffing_Buff_Remove      "#1# tells the group#*#REMOVEBUFF #2#'"
#EVENT Buffing_Buff_Remove      "#1# tells you#*#REMOVEBUFF #2#'"
#EVENT Buffing_Buff_Remove      "#1# tells the raid#*#REMOVEBUFF #2#'"
#EVENT Buffing_Buff_Remove      "[ #1# #*#]#*#REMOVEBUFF #2#"

#EVENT Buffing_Buff_RemoveAll   "#1# tells the group#*#'removeallbuffs#*#'"
#EVENT Buffing_Buff_RemoveAll   "#1# tells you#*#'removeallbuffs#*#'"
#EVENT Buffing_Buff_RemoveAll   "#1# tells the raid#*#'removeallbuffs#*#'"
#EVENT Buffing_Buff_RemoveAll   "[ #1# #*#]#*#removeallbuffs #*#"

Sub initEvents_buffing
	/call RegisterBackgroundEvents Events_Buffing 6
/return

Sub Events_Buffing
	/doevents Buffing_DS_Disable
	/doevents Buffing_DS_Enable

	/doevents Buffing_Buff_OnRaid
	/doevents Buffing_Buff_OnChar

	/doevents Buffing_Buff_Remove
	/doevents Buffing_Buff_RemoveAll
/return

|**
 ----------------------------
 BUFFING Routines Initialization
 ----------------------------
 Will process the self buffs defined in character INI
 Will process the targeted buffs defined in the character INI
 Will process the combat buffs defined in the character INI

 Sets default variables, and initializes the event listeners
 ----------------------------
**|

Sub xbot_initialize_buffing
	/echo \n \apInitialize: \aoBuffing \aw-------------------------

	| process the character ini file
	/call processSelfBuffs
	/call processTargetedBuffs
	/call processCombatBuffs 

	| initialize the buffing events
	/call initEvents_buffing
	
/return




| Note int_selfBuffs_total is set here, as well as the list of selfbuffs
Sub processSelfBuffs
    /echo \awprocessSelfBuffs

	/declare raw_selfBuffs[100]		string 	local NULL
	/declare i						int 	local 0
	/declare sz 					int     local 0
    /declare test                   string  local NULL

    | self buffs
    /declare timer_check_selfBuffs          timer   outer   0
    /declare int_selfBuffs_total            int     outer   0
	/declare bln_selfBuffs_enabled			bool	outer	TRUE

    |; =_buff|_type|_itemName
	|BUFF_1="Reyfin's Racing Thoughts"|ITEM|"Xxeric's Matted-Fur Mask"
	/for i 1 to ${raw_selfBuffs.Size}
        | if the value of the entry ends with a quoted string, we need to wrap it all in quotes again.
        /varset test ${Ini[INI_CHAR,BUFFS_SELF,BUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_selfBuffs_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare BUFFSELF_${i}		string	outer   ${test.Arg[1,|]}
		/declare BUFFSELF_${i}_Type	string  outer   ${test.Arg[2,|]}
		/declare BUFFSELF_${i}_Name	string  outer   ${test.Arg[3,|]}
        /echo \ao        BUFFSELF[\ag${i}\ao]: \awBuff:\ag${BUFFSELF_${i}} \awType:\ag${BUFFSELF_${i}_Type} \awName:\ag${BUFFSELF_${i}_Name}
	/next i
/return

Sub processTargetedBuffs
    /echo \awprocessTargetedBuffs

    /declare raw_targetedBuffs[100]		string 	local NULL
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL

    | targeted buffs
    /declare timer_check_targetedBuffs      timer   outer   0
    /declare int_targetedBuffs_total        int     outer   0
    /declare bln_targetedBuffs_enabled      bool    outer   TRUE
    /declare bln_targetedBuffs_allowRaid    bool    outer   TRUE
    /declare bln_targetedBuffs_allowGroup   bool    outer   TRUE 
    /declare bln_targetedBuffs_automatic    bool    outer   TRUE

    |; = _enabled|_allowRaid|_allowGroup|_automatic
    |settings=TRUE|TRUE|TRUE|FALSE
    /if (!${Ini[INI_CHAR,BUFFS_TARGETED,settings].Arg[1,|]})  /varset bln_targetedBuffs_enabled    FALSE
    /if (!${Ini[INI_CHAR,BUFFS_TARGETED,settings].Arg[2,|]})  /varset bln_targetedBuffs_allowRaid  FALSE
    /if (!${Ini[INI_CHAR,BUFFS_TARGETED,settings].Arg[3,|]})  /varset bln_targetedBuffs_allowGroup FALSE
    /if (!${Ini[INI_CHAR,BUFFS_TARGETED,settings].Arg[4,|]})  /varset bln_targetedBuffs_automatic  FALSE

    /echo \a-w    Settings Processed: \awEnabled:\ag${bln_targetedBuffs_enabled} \awAllowRaid:\ag${bln_targetedBuffs_allowRaid} \awAllowGroup:\ag${bln_targetedBuffs_allowGroup} \awAutomatic:\ag${bln_targetedBuffs_automatic}
    
    |;=_buff|_use|_targetType|_gem|_classes
    |BUFF_1="Balikor's Mark"|TRUE|GROUP|5|ALL
    /for i 1 to ${raw_targetedBuffs.Size}
        /varset test ${Ini[INI_CHAR,BUFFS_TARGETED,BUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_targetedBuffs_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare BUFFTARGETED_${i}		    string	outer   ${test.Arg[1,|]}
        /declare BUFFTARGETED_${i}_Use	    string  outer   ${test.Arg[2,|]}
		/declare BUFFTARGETED_${i}_Type	    string  outer   ${test.Arg[3,|]}
		/declare BUFFTARGETED_${i}_Gem	    string  outer   ${test.Arg[4,|]}
        /declare BUFFTARGETED_${i}_Targets	string  outer   ${test.Arg[5,|]}
        /echo \ao        BUFFTARGETED[\ag${i}\ao]: \awBuff:\ag${BUFFTARGETED_${i}} \awUse:\ag${BUFFTARGETED_${i}_Use} \awType:\ag${BUFFTARGETED_${i}_Type} \awGem:\ag${BUFFTARGETED_${i}_Gem} \awTargets:\ag${BUFFTARGETED_${i}_Targets}
    /next i

/return

Sub processCombatBuffs
    /echo \awprocessCombatBuffs
    /declare raw_combatBuffs[100]		string 	local NULL
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL

    | combat buffs
    /declare timer_check_combatBuffs        timer   outer   0
    /declare int_combatBuffs_total          int     outer   0
    /declare bln_combatBuffs_enabled        bool    outer   TRUE
    /declare bln_combatBuffs_allowRaid      bool    outer   TRUE
    /declare bln_combatBuffs_allowGroup     bool    outer   TRUE 

    |; = _enabled|_allowRaid|_allowGroup|_automatic
    |settings=TRUE|TRUE|TRUE|FALSE
    /if (!${Ini[INI_CHAR,BUFFS_COMBAT,settings].Arg[1,|]})  /varset bln_combatBuffs_enabled         FALSE
    /if (!${Ini[INI_CHAR,BUFFS_COMBAT,settings].Arg[2,|]})  /varset bln_combatBuffs_allowRaid       FALSE
    /if (!${Ini[INI_CHAR,BUFFS_COMBAT,settings].Arg[3,|]})  /varset bln_combatBuffs_allowGroup      FALSE

    /echo \a-w    Settings Processed: \awEnabled:\ag${bln_combatBuffs_enabled} \awAllowRaid:\ag${bln_combatBuffs_allowRaid} \awAllowGroup:\ag${bln_combatBuffs_allowGroup}
    |;=_buff|_use|_targetType|_gem|_classes
    |BUFF_1="Panoply of Vie"|TRUE|SINGLE|5|MT
   
    /for i 1 to ${raw_combatBuffs.Size}
        /varset test ${Ini[INI_CHAR,BUFFS_COMBAT,BUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_combatBuffs_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare BUFFCOMBAT_${i}		    string	outer   ${test.Arg[1,|]}
        /declare BUFFCOMBAT_${i}_Use	    string  outer   ${test.Arg[2,|]}
		/declare BUFFCOMBAT_${i}_Type	    string  outer   ${test.Arg[3,|]}
		/declare BUFFCOMBAT_${i}_Gem	    string  outer   ${test.Arg[4,|]}
        /declare BUFFCOMBAT_${i}_Targets	string  outer   ${test.Arg[5,|]}
        /echo \ao        BUFFCOMBAT[\ag${i}\ao]: \awBuff:\ag${BUFFCOMBAT_${i}} \awUse:\ag${BUFFCOMBAT_${i}_Use} \awType:\ag${BUFFCOMBAT_${i}_Type} \awGem:\ag${BUFFCOMBAT_${i}_Gem} \awTargets:\ag${BUFFCOMBAT_${i}_Targets}
    /next i

/return


|**
 ----------------------------
 Self Buffs
 ----------------------------
 Will determine if the self buff stacks
 Will determine if the item||spell||alt is ready for casting
 Will cast if the above are both TRUE and we are not engaged
 ----------------------------
**|
Sub chkSelfBuffs
	/if (!${int_selfBuffs_total} || !${bln_selfBuffs_enabled}) /return

    /echo \aochkSelfBuffs\aw(\ag${int_selfBuffs_total}\aw)
    /declare i  int                 local 0
    /declare ds int                 local 0
    /declare brdCastingArgs string  local NULL
    /declare allCastingArgs string  local NULL
    /declare bln_buffsCast  bool    local FALSE
    | We need to target ourselves since this is a self buff
    /target id ${Me.ID}
    /for i 1 to ${int_selfBuffs_total}
        | Check to see if the buff is a DS and if bln_useDS if FALSE, skip the buff
        /if (!${bln_useDS}) {
            /for ds 1 to ${int_damageShieldList_total}
                /if (${buffName.Equal[${dsSpells[${ds}]}]}) /goto :next_buff
            /next ds
        }

        | Check the events for enagement to stop this sub
        |/call RunAllEvents
        /if (${bln_engaged}) /return

        | Check to see if the buff is already applied
        /echo \aw    Checking \ag${BUFFSELF_${i}} \aoStacks?\ag${Spell[${BUFFSELF_${i}}].Stacks}  \aoCurrently Have? \ag${Bool[${Me.Buff[${BUFFSELF_${i}}].ID}]}
        /if (${Spell[${BUFFSELF_${i}}].Stacks} && !${Me.Buff[${BUFFSELF_${i}}].ID}) {
            | Handle the item type buff check
            |BUFF_1="Reyfin's Racing Thoughts"|ITEM|"Xxeric's Matted-Fur Mask"
            /varset brdCastingArgs "${BUFFSELF_${i}_Name}" TRUE ${int_defaultSpellGem}
            /varset allCastingArgs ${BUFFSELF_${i}_Name}
            /if (${BUFFSELF_${i}_Type.Equal[ITEM]}) {
                /if (!${FindItem[=${BUFFSELF_${i}_Name}].ID})  /goto :next_buff
                /varset brdCastingArgs ITEM ${brdCastingArgs} ${Me.ID} TRUE
                
            } else /if (${BUFFSELF_${i}_Type.Equal[SPELL]}) {
                /call chkSpellMemorized "${BUFFSELF_${i}_Name}" TRUE ${int_defaultSpellGem}
                /varset brdCastingArgs SONG "${BUFFSELF_${i}_Name}" ${Me.ID} TRUE

            } else /if (${BUFFSELF_${i}_Type.Equal[ALT]})  {
                /varset brdCastingArgs ALT "${BUFFSELF_${i}_Name}" ${Me.ID} TRUE
            }
            /varset bln_buffsCast TRUE
            /if (${Me.Class.ShortName.Equal[BRD]})  /call MQ2Medley ${brdCastingArgs}
            /if (!${Me.Class.ShortName.Equal[BRD]}) /call MQ2Cast "${allCastingArgs}"
        }
    :next_buff
    /next i
    /if (${bln_buffsCast}) /memspellset ${str_currentSpellSet}
    /varset timer_check_selfBuffs 300
/return




|**
 ----------------------------
 Raid Buffs
 ----------------------------
 Will cast the targetted buffs on the RAID
 Will run through each raid group and cast the group/single buff(s)
 on the group leader if its a group spell, or loop through all the raid members
 that fit the ${BUFFTARGETED_${i}_Targets} key for the buff
 ----------------------------
**|
Sub EVENT_Buffing_Buff_OnRaid(_line, _chatSender, _eventParams)
	| Flush the event
	/doevents flush Buffing_Buff_OnRaid

	/echo \awDoing Raid Buffs on \agALL
	/memspellset buff
	/call chkTargetedBuffs RAID NULL TRUE

	/dgt ${Me.Class.Name} RAID BUFF [${_eventParams}] COMPLETE
	| Restore the previous spellset
	/memspellset ${str_currentSpellSet}

/return



|**
 ----------------------------
 Character Buffs
 ----------------------------
 Will cast the targetted buffs on a single CHARACTER
 Will target the character and cast the group/single target buff(s)
 ----------------------------
**|
Sub EVENT_Buffing_Buff_OnChar(_line, _chatSender, _eventParams)
	| Flush the event
	/doevents flush Buffing_Buff_OnChar

	/echo \awDoing Character Buffs on \ag${_eventParams}
	/memspellset buff
	/call chkTargetedBuffs CHARACTER ${_eventParams} TRUE
    
	/dgt ${Me.Class.Name} CHAR BUFF [${_eventParams}] COMPLETE
	| Restore the previous spellset
	/memspellset ${str_currentSpellSet}

/return

|**
 ----------------------------
 Casting Targeted Buffs
 ----------------------------
 Will cast the targetted buffs on a _targetType[RAID||GROUP||CHARACTER]
 With a specific character _targetName if type is CHARACTER, ignored for RAID||GROUP
 Will force the casting if called via event, or can be FALSE if we are autobuffing[not fully implemented]
 ----------------------------
**|
Sub chkTargetedBuffs(string _targetType,string _targetName, bool _forced)
	/if (!${int_targetedBuffs_total} || !${bln_targetedBuffs_enabled}) /return

    /echo \aochkTargetedBuffs\aw(\ag${int_targetedBuffs_total}\aw)
    /echo \awTargetType:\ag${_targetType} \awTargetName:\ag${_targetName} \awForced:\ag${_forced}
    /declare i  int                 local 0
    /declare r  int                 local 0
    /declare g  int                 local 0
    /declare allCastingArgs string  local NULL
    /declare charClass      string  local NULL
    /declare originalTargetID int   local 0
    /if (${Target.ID}) /varset originalTargetID ${Target.ID}

    | mem the buff spellset
    /memspellset buff
    /delay 30

    |;=_buff|_use|_targetType|_gem|_classes
    /for i 1 to ${int_targetedBuffs_total}
        /if (!${BUFFTARGETED_${i}_Use}) /goto :next_buff
        | Verify that the spell is memorized
        /call chkSpellMemorized "${BUFFTARGETED_${i}}" TRUE ${BUFFTARGETED_${i}_Gem}
        | Character Buffing, this will only cast the buff on a single target
        /if (${_targetType.Equal[CHARACTER]}) {
            /echo \ayCasting \agSINGLE \aybuff on \ag${_targetName}
            /call TrueTarget ${Spawn[pc ${_targetName}].ID}
            /call MQ2Cast "${BUFFTARGETED_${i}}"

        } else /if (${_targetType.Equal[GROUP]}) {
            /if (${BUFFTARGETED_${i}_Type.Equal[GROUP]}) {
                | Group Spell on my own group
                /echo \ayCasting \agGROUP \aybuff on \ag${Group.Member[0].Name}
                /call TrueTarget ${Group.Member[0].ID}
                /call MQ2Cast "${BUFFTARGETED_${i}}"

            } else /if (${BUFFTARGETED_${i}_Type.Equal[SINGLE]}) {
                 /varcalc g ${Group.GroupSize} - 1
                 /for r 0 to ${g}
					/if (${Select[${Group.Member[${r}].Class.ShortName},${BUFFTARGETED_${i}_Targets}]} || ${BUFFTARGETED_${i}_Targets.Equal[ALL]} || (${BUFFCOMBAT_${i}_Targets.Equal[MT]} && ${Group.Member[${r}].ID} == ${int_MTID})) {
						/echo \ayCasting \agSINGLE \aybuff on \ag${Group.Member[${g}].Name}
						/call TrueTarget ${Group.Member[${g}].ID}
						/call MQ2Cast "${BUFFTARGETED_${i}}"
					}
                 /next r
            }
        } else /if (${_targetType.Equal[RAID]}) {
            /if (${BUFFTARGETED_${i}_Type.Equal[GROUP]}) {
                | Raid Buffing, this will walk through raid
                /for r 1 to ${arr_raidLeaders.Size}
                    /if (${arr_raidLeaders[${r}].NotEqual[NULL]}) {
                        /echo \ayCasting \agGROUP \aybuff on \ag${arr_raidLeaders[${r}]}
                        /if (${Target.Name.NotEqual[${arr_raidLeaders[${r}]}]}) /call TrueTarget ${Spawn[pc ${arr_raidLeaders[${r}]}].ID}
                        /call MQ2Cast "${BUFFTARGETED_${i}}"
                    }       
                /next r
            } else  /if (${BUFFTARGETED_${i}_Type.Equal[SINGLE]}) {
                | Single Target Buffing in the raid, look at groups and see if a target is in each
                | Note it is probably unlikely that a SINGLE type buff will apply to ALL, but instead likely to be MT
                | or a subset of classes
                /for r 1 to ${Raid.Members}
                    | we may not be able to query the class from the raid member, we might need to dquery
                    /if (${Select[${Raid.Member[${r}].Class.ShortName},${BUFFTARGETED_${i}_Targets}]} || ${BUFFTARGETED_${i}_Targets.Equal[ALL]} || (${BUFFTARGETED_${i}_Targets.Equal[MT]} && ${Raid.Member[${r}].ID} == ${int_MTID})) {
                        /echo \ayCasting \agSINGLE \aybuff on \ag${Raid.Member[${r}].Name}
                        /call TrueTarget ${Raid.Member[${r}].ID}
                        /call MQ2Cast "${BUFFTARGETED_${i}}"
                    }
                /next r
            }
        }
       

    :next_buff
    /next i

    /memspellset ${str_currentSpellSet}
    /varset timer_check_targetedBuffs 300
    /if (${originalTargetID}) /call TrueTarget ${originalTargetID}
/return



|**
 ----------------------------
 Casting Combat Buffs
 ----------------------------
 Will cast the targetted buffs on a _targetType[RAID||GROUP||CHARACTER]
 With a specific character _targetName if type is CHARACTER, ignored for RAID||GROUP
 ----------------------------
**|
|; = _enabled|_allowRaid|_allowGroup|_automatic
|settings=TRUE|TRUE|TRUE|FALSE

|; =_buff|_use|_targetType[SINGLE|GROUP]|_gem|_classes
|BUFF_1="Panoply of Vie"|TRUE|SINGLE|5|MT
Sub chkCombatBuffs
    /if (!${int_combatBuffs_total} || !${bln_combatBuffs_enabled}) /return
    /echo \aochkCombatBuffs\aw(\ag${int_combatBuffs_total}\aw)
    /declare i                  int local 0
    /declare r                  int local 0
    /declare g                  int local 0
    /declare foundDuration      int local 0
    /declare originalTargetID   int local 0
    /if (${Target.ID}) /varset originalTargetID ${Target.ID}

    |_buff|_use|_targetType[SINGLE|GROUP]|_gem|_classes
    /for i 1 to ${int_combatBuffs_total}
        | check raid using BUFFCOMBAT_${i}_Targets which can be [(classes) WAR,ROG,BRD,MNK,BER||MT||NAMELIST]
        /if (${bln_combatBuffs_allowRaid}) {
            /for r 1 to ${Raid.Members}
                /if (${Select[${Raid.Member[${r}].Class.ShortName},${BUFFCOMBAT_${i}_Targets}]} || ${BUFFCOMBAT_${i}_Targets.Equal[ALL]} || (${BUFFCOMBAT_${i}_Targets.Equal[MT]} && ${Raid.Member[${r}].ID} == ${int_MTID})) {
                    | Check to see if the target has the buff already
                    /dquery ${Raid.Member[${r}].Name} -q "Me.Buff[${BUFFCOMBAT_${i}}].Duration"  -o foundDuration
                    /delay 20 ${DanNet.Q.Received}
                    /if (${foundDuration}) /goto :next_buff
                    /call TrueTarget ${Raid.Member[${r}].ID}
                    /call MQ2Cast "${BUFFCOMBAT_${i}}" ${int_defaultSpellGem} RunPriorityEvents
                }
            /next r
        } else /if (${bln_combatBuffs_allowGroup}) {
            /varcalc g ${Group.GroupSize} - 1
            /for r 0 to ${g} 
                /if (${Select[${Group.Member[${r}].Class.ShortName},${BUFFTARGETED_${i}_Targets}]} || ${BUFFTARGETED_${i}_Targets.Equal[ALL]} || (${BUFFCOMBAT_${i}_Targets.Equal[MT]} && ${Group.Member[${r}].ID} == ${int_MTID})) {
                    /dquery ${Group.Member[${r}].Name} -q "Me.Buff[${BUFFCOMBAT_${i}}].Duration"  -o foundDuration
                    /delay 20 ${DanNet.Q.Received}
                    /if (${foundDuration}) /goto :next_buff
                    /call TrueTarget ${Group.Member[${r}].ID}
                    /call MQ2Cast "${BUFFCOMBAT_${i}}" ${int_defaultSpellGem} RunPriorityEvents
                }
            /next r
        }
    :next_buff
    /next i
    :return 
    /varset timer_check_combatBuffs 100
    /if (${originalTargetID}) /call TrueTarget ${originalTargetID}
/return




|**
 ----------------------------
 Buff Removal
 ----------------------------
 Will attempt to remove defined buff or, if all buffs is called - all buffs currently on character.
 ----------------------------
**|

Sub EVENT_Buffing_Buff_Remove(_line, _chatSender, _eventParams)
	| Note these Buffs need to replace single quote with pipe due to chat mangling quotes: eg Brell|s Brawny Bulwark
	/declare buffToRemove string local ${_eventParams.Arg[1,-]}
	/echo \awEvent Remove Buff called: ${_eventParams} -> ${buffToRemove}
	/call Buffing_Buff_Remove "${buffToRemove}"
	/doevents flush Buffing_Buff_Remove
/return

Sub Buffing_Buff_Remove(_badBuff)
	/declare i int local
	/declare buffName ${_badBuff.Replace[|,']}
	/echo \agRemoving Buff \ao${buffName} (was ${_badBuff})
	/for i 0 to 24
		/if (${Bool[${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip}]} && ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Find[${buffName}]}) {
			/varset buffName ${Window[BuffWindow].Child[BW_Buff${i}_Button].Tooltip.Arg[1,(]}
			/varset buffName ${buffName.Left[${Math.Calc[${buffName.Length}-1]}]}
			/if (${Bool[${buffName}]}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[${buffName}].ID}-1].Int} leftmouseup
		}
	/next i
/return


Sub EVENT_Buffing_Buff_RemoveAll(_line, _chatSender, _eventParams)
	/call Buffing_Buff_RemoveAll
	/doevents flush Buffing_Buff_RemoveAll
/return

Sub Buffing_Buff_RemoveAll
	/declare n int local
	/for n 1 to 25
		/if (${Me.Buff[${n}].ID}) /call Buffing_Buff_Remove "${Me.Buff[${n}]}"
	/next n
/return



|**
 ----------------------------
 Damage Shield Buff Removal
 ----------------------------
 Will attempt to remove defined buff or, if all buffs is called - all buffs currently on character.
 ----------------------------
**|
Sub EVENT_Buffing_DS_Disable(_line, _sender, _eventParams)
	/call RemoveDS
/return

Sub RemoveDS
	/varset bln_useDS FALSE
	| check to see if I have a DS, and if so kill it
	/declare ds int local 
	/for ds 1 to ${int_damageShieldList_total} 
		/call Buffing_Buff_Remove "${dsSpells[${ds}]}"
	/next ds
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/varset str_savedMedley ${str_currentMedley}
		/varset str_currentMedley nods
		/medley ${str_currentMedley}
	}	
	:return
	/doevents flush Buffing_DS_Disable
/return 


Sub EVENT_Buffing_DS_Enable(_line, _sender, _eventParams)s
	/varset bln_useDS TRUE
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/echo \aoTurning on ${str_savedMedley} Medley
		/memspellset ${str_savedMedley}
		/delay 10
		/medley ${str_savedMedley}
	}
	:return
	/doevents flush Buffing_DS_Enable
/return

