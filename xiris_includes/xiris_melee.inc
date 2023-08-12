|**
----------------------------
xiris_melee.inc
----------------------------
This library supplements|replaces mq2melee functionality

Manual Downshits since mq2melee was being flakey at the time
	Storing downflags in the BOT_CLS_Tooname.ini under the [BUFFS_SELF]
	note the second token area isd the casting arguments in full
	Usage downflagX=BuffName|itemID item|(opt)Target

	[BUFFS_SELF]
		downflagCount=6
		downflag1=E'ci's Icy Familiar|"E'ci's Icy Familiar" alt
		downflag2=Form of Endurance II|9435 item
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	Removed unused checks/subs that are retained by MQ2Melee.dll
	06.28.18	xiris	Moved downshits back to MQ2Melee since ROF2 Emu actually supports Combat.State
	11.22.21	xiris	Keeping this around as its useful in situations where I dont want to use melee
	03.22.22	xiris 	Updated KillMob Event to be cleaner (ID first)
	04.27.22	xiris 	Added chkCombatAbilities which can be called when we need to turn off
						MQ2Melee for whatever reason (like.. MPG Specialization not working on berserkers!)
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only
	01.31.23	xiris 	Implemented UHC's downshit fix (arround line 260)
	07.31.23	xiris	Rewrite using new INI structures
						Removed downflags as that is now in Buffing (selfbuffs)
**|

#EVENT Melee_ReStick		"#*#restick#*#"
#EVENT Melee_FireAEMelee	"#*#aemeleeON#*#"
#EVENT Melee_GetCloser		"#*#Your target is too far away#*#"
#EVENT Melee_CantSeeTarget	"#*#You cannot see your target#*#"
#EVENT Melee_CantNavTarget	"#*#Could not locate#*#"

#EVENT Melee_AutoAssist		"#1# tells the group#*#'AutoAssist #2#'"
#EVENT Melee_AutoAssist		"#1# tells you#*#'AutoAssist #2#'"
#EVENT Melee_AutoAssist		"#1# tells the raid#*#'AutoAssist #2#'"
#EVENT Melee_AutoAssist 	"[ #1# #*#]#*#AutoAssist #2#"

#EVENT Melee_UpdatesStickCommand	"#1# tells the group#*#'UpdateStick #2#'"
#EVENT Melee_UpdatesStickCommand	"#1# tells you#*#'UpdateStick #2#'"
#EVENT Melee_UpdatesStickCommand	"#1# tells the raid#*#'UpdateStick #2#'"
#EVENT Melee_UpdatesStickCommand	"[ #1# #*#]#*#UpdateStick #2#"


Sub initEvents_Melee
	/if (${Select[${Me.Class.ShortName},BER,BST,BRD,MNK,PAL,RNG,ROG,SHD,WAR]}) /call RegisterBackgroundEvents  Events_Melee_Background 2
    /if (${Select[${Me.Class.ShortName},BER,BST,BRD,MNK,PAL,RNG,ROG,SHD,WAR]}) /call RegisterRaidEvents        Events_Melee_Raid       3
    /if (${Select[${Me.Class.ShortName},BER,BST,BRD,MNK,PAL,RNG,ROG,SHD,WAR]}) /call RegisterPriorityEvents    Events_Melee_Priority   1
/return

Sub Events_Melee_Background
	/doevents Melee_AutoAssist
	/doevents Melee_UpdatesStickCommand
    /if (${bln_components_enabled} && !${timer_component_check}) /call chkComponents
/return

Sub Events_Melee_Raid
	/doevents Melee_FireAEMelee
	/doevents Melee_GetCloser
	/doevents Melee_CantSeeTarget
	/if (${bln_engaged} && ${Select[${Me.Class.ShortName},BER,BST,BRD,MNK,PAL,RNG,ROG,SHD,WAR]}) /call chkCA_${Me.Class.ShortName}
/return 

Sub Events_Melee_Priority
    /doevents Melee_ReStick
/return 


Sub EVENT_Melee_AutoAssist(string _line, string _chatSender, string _eventParams)
    /doevents flush Melee_AutoAssist
	/if (${_eventParams.Equal[on]})     /call AutoAssist TRUE
	/if (${_eventParams.NotEqual[on]})  /call AutoAssist FALSE
/return

Sub EVENT_Melee_GetCloser
    /doevents flush Melee_GetCloser
	/if (!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged} || ${timer_getCloser}) /return 
	/varset timer_getCloser 50
	| If the target is less than max stick distance, adjust the stick
	/call StickAdjust
/return

SUB EVENT_Melee_CantSeeTarget
    /doevents flush Melee_CantSeeTarget
	/if (!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged} || ${timer_cantSee}) /return 
	/varset timer_cantSee 100
	/face fast
/return

SUB EVENT_Melee_CantNavTarget
    /doevents flush Melee_CantNavTarget
	/if (!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged} || ${timer_cantNav}) /return 
	/echo \ar Cannot NAVTARGET ${bln_following}
	| If the target is less than 50 range
	/if (!${bln_following}) {
		/if (${Select[${Me.Class.ShortName},BRD,BER,MNK,ROG,RNG,PAL,SHD,WAR]} && ${Target.ID}) {
			/if (!${bln_tanking})  {
				/if (${gbl_bln_useWarp}) /warp loc ${Target.Y} ${Target.X} ${Target.Z}
			} else {
				/if (${gbl_bln_useWarp}) /warp f 10
			} 
		}
	} else {
		/echo \aoFollowing ${str_followTarget}
		/call TrueTarget ${Spawn[pc ${str_followTarget}].ID}
		/delay 5
		/if (${gbl_bln_useWarp}) /warp loc ${Target.Y} ${Target.X} ${Target.Z}
	}
    /varset timer_cantNav 50
/return

SUB EVENT_Melee_UpdatesStickCommand(string _line, string _chatSender, string _eventParams)
    /doevents flush Melee_UpdatesStickCommand
	/varset str_stickCommand ${_eventParams}
/return 

Sub EVENT_Melee_FireAEMelee
    /doevents flush Melee_FireAEMelee
    /call FireAEMelee
/return

Sub EVENT_Melee_ReStick
    /doevents flush Melee_ReStick
	/call StickToTarget TRUE
/return

|**
 ----------------------------
 Melee Routines Initialization
 ----------------------------
 Sets default variables, and initializes the event listeners
 ----------------------------
**|

Sub xbot_initialize_melee
	/echo \n \apInitialize: \aoMelee \aw-------------------------

	/declare 	int_currentStickDistance		int		outer   30
	/declare	int_startStickDistance 	int 	outer 	40 	| Base stick distance
	/declare 	int_minStickDistance	int		outer 	10	| Minimum stick distance
	/declare 	int_maxStickDistance	int     outer   100 | Maximum attempted stick distance
	/declare    int_maxNavDistance		int		outer   300	| Maximum attempted nav distance

    /declare	bln_autoAssist			bool    outer   FALSE
    /declare	timer_component_check	timer   outer   0
    /declare	timer_cantNav			timer   outer   50
    /declare	timer_cantSee			timer   outer   100 
    /declare	timer_getCloser			timer   outer   50
	/declare	timer_provoke			timer 	outer 	50

	/declare 	str_stickCommand		string  outer  ${Ini[INI_COMMON,SETTINGS,stickCommand,snaproll rear 30]}
	/if (${Ini[INI_CHAR,SETTINGS,stickcmd].NotEqual[NULL]}) /varset str_stickCommand ${Ini[INI_CHAR,SETTINGS,stickcmd]}

	/call processProvoke
	/call processComponents
	/call processRangedItems
	
	| initialize the melee events
	/call initEvents_Melee
	
/return

Sub processProvoke
	/echo \awprocessProvoke
	/declare arr_rawProvoke[10]	    	string	local 
	/declare i						    int 	local	0
	/declare sz 					    int     local	0
    /declare test                       string  local	NULL

	/declare bln_provokes_enabled		bool 	outer	FALSE
	/declare int_provokes_total			int		outer	0

	|; = _enabled|
	|settings=TRUE
	/if (!${Ini[INI_CHAR,PROVOKE,settings].Arg[1,|]}) /multiline ; /echo \a-w    Settings Processed: \awEnabled:\ag${bln_provokes_enabled} ; /return
   

	|-; _provoke|_type|_use|_cost
	|PROVOKE_1="Ancient: Chaos Cry"|DISC|TRUE|30
	/for i 1 to ${arr_rawProvoke.Size}
        /varset test ${Ini[INI_CHAR,PROVOKE,PROVOKE_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_provokes_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varset bln_provokes_enabled TRUE
		/varcalc sz	${sz}+1
		/declare PROVOKE_${i}		    string	outer   ${test.Arg[1,|]}
        /declare PROVOKE_${i}_Type	    string  outer   ${test.Arg[2,|]}
		/declare PROVOKE_${i}_Use	    string  outer   ${test.Arg[3,|]}
		/declare PROVOKE_${i}_Cost	    string  outer   ${test.Arg[4,|]}
        /echo \ao        PROVOKE[\ag${i}\ao]: \awProvoke:\ag${PROVOKE_${i}} \awUse:\ag${PROVOKE_${i}_Use} \awType:\ag${PROVOKE_${i}_Type} \awCost:\ag${PROVOKE_${i}_Cost}
    /next i


/return 

Sub processComponents
    /echo \awprocessComponents
   
	/declare arr_rawComponents[10]	    string	local 
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL


    /declare bln_components_enabled     bool	outer FALSE
    /declare int_components_total		int  	outer 0

	| Run a test to see if there are no entries
	/if (!${Ini[INI_CHAR,COMPONENTS,COMPONENT_1].Arg[1,|]}) /multiline ; /echo \a-w    Settings Processed: \awEnabled:\ag${bln_components_enabled} ; /return

	|; =_component|_use|_type|_count|_summoner|_reagent
	| COMPONENT_1="Axe of the Destroyer"|TRUE|DISC|200|"Axe of the Destroyer"|"Balanced Axe Components"
    /for i 1 to ${arr_rawComponents.Size}
        /varset test ${Ini[INI_CHAR,COMPONENTS,COMPONENT_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_components_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
        /varset  bln_components_enabled TRUE
		/varcalc sz	${sz}+1
		/declare COMPONENT_${i}		        string	outer   ${test.Arg[1,|]}
        /declare COMPONENT_${i}_Use	        string  outer   ${test.Arg[2,|]}
		/declare COMPONENT_${i}_Type	    string  outer   ${test.Arg[3,|]}
		/declare COMPONENT_${i}_Count	    string  outer   ${test.Arg[4,|]}
        /declare COMPONENT_${i}_Summoner	string  outer   ${test.Arg[5,|]}
        /declare COMPONENT_${i}_Reagent     string  outer   ${test.Arg[6,|]}
        /echo \ao        COMPONENT[\ag${i}\ao]: \awComponent:\ag${COMPONENT_${i}} \awUse:\ag${COMPONENT_${i}_Use} \awType:\ag${COMPONENT_${i}_Type} \awCount:\ag${COMPONENT_${i}_Count} \awSummoner:\ag${COMPONENT_${i}_Summoner} \awReagent:\ag${COMPONENT_${i}_Reagent}
		/echo \aw							 \a-wDesired: \ay${COMPONENT_${i}_Count} \a-wInventory: \ag${FindItemCount[=${COMPONENT_${i}}]}
	/next i
/return

Sub processRangedItems
 	/echo \awprocessRangedItems
   
	/declare arr_rawRanged[10]			string	local 
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL


    /declare bln_ranged_enabled			bool	outer FALSE
    /declare int_ranged_total			int  	outer 0

	
	| Run a test to see if there are no entries
	/if (!${Ini[INI_CHAR,RANGED,settings].Arg[1,|]})  /multiline ;  /echo \a-w    Settings Processed: \awEnabled:\ag${bln_ranged_enabled} ; /return

	|; _rangedItem|_use|_component
	| RANGED_1="Shuriken of Eternity"|TRUE|COMPONENT_1|
    /for i 1 to ${arr_rawRanged.Size}
        /varset test ${Ini[INI_CHAR,RANGED,RANGED_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_ranged_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
        /varset  bln_ranged_enabled TRUE
		/varcalc sz	${sz}+1
		/declare RANGED_${i}		        string	outer   ${test.Arg[1,|]}
        /declare RANGED_${i}_Use	        string  outer   ${test.Arg[2,|]}
		/declare RANGED_${i}_Component	    string  outer   ${test.Arg[3,|]}

        /echo \ao        RANGED[\ag${i}\ao]: \awComponent:\ag${RANGED_${i}} \awUse:\ag${RANGED_${i}_Use} \awComponent:\ag${RANGED_${i}_Component}
    /next i
/return

Sub FireAEMelee 
    /if (${Me.Combat} && ${Me.Class.ShortName.Equal[MNK]})  /multiline ; /call MQ2Cast "Destructive Force" ALT ; /return
    /if (${Me.Combat} && ${Me.Class.ShortName.Equal[WAR]})  /multiline ; /call MQ2Cast "Rampage" ALT ;  /return
    /if (${Me.Combat} &&  ${Me.Class.ShortName.Equal[BER]}) /multiline ; /call MQ2Cast "Rampage" ALT ;  /return
/return 


Sub chkComponents
	/if (${bln_components_enabled} && !${timer_component_check}) {
		/declare i 			int 	local	1
		/declare component	string 	local   NULL
		/declare compType	string  local   NULL
		/declare count		int		local	0
		/declare summoner	string	local	NULL
		/declare reagent	string  local   NULL
		/for i 1 to ${int_components_total} 
			/if (!${COMPONENT_${i}_Use}) /continue
			/call chkComponent "${COMPONENT_${i}}" ${COMPONENT_${i}_Count} ${COMPONENT_${i}_Type} "${COMPONENT_${i}_Summoner}" "${COMPONENT_${i}_Reagent}"
			/delay 10
			/if (${Macro.Return.Equal[SUMMON_ITEM_NOT_FOUND]}) {
				/echo SUMMON_ITEM_NOT_FOUND turning  this component OFF.
				/docommand /${announceChannel} SUMMON_ITEM_NOT_FOUND  ${COMPONENT_${i}} ${COMPONENT_${i}_Summoner} turning  this component OFF.
				/varset COMPONENT_${i}_Use FALSE
			}
			/if (${Macro.Return.Equal[OUT_OF_REAGENTS]}) {
				/echo OUT_OF_REAGENTS turning  this component OFF.
				/docommand /${announceChannel} OUT_OF_REAGENTS ${COMPONENT_${i}} ${COMPONENT_${i}_Reagent} turning  this component OFF.
				/varset COMPONENT_${i}_Use FALSE
			}
			/autoinv
		/next i
	}
/return


Sub chkComponent(string _component, int _count, string _type, string _summoner, string _reagent)
	|/echo chkComponents ${_component}, ${_count}, ${_type}, ${_summoner}, ${_reagent}
	|/echo (${FindItemCount[=${_component}]}>=${_count})
	/if (${Zone.ShortName.NotEqual[thundercrest]} && ${Zone.ShortName.NotEqual[guildhall]}) /goto :chkComponent
	/if (${SpawnCount[npc radius ${int_campRadius}]} > 0) /return

	/if (!${Defined[_reagent]})	/declare reagent string local NULL
	/if (!${Defined[message]})	/declare message string local SUCCCESS

	:chkComponent
		|/echo \awChecking components: \aodesired:\ay${_count} \aohave: \ag${FindItemCount[=${_component}]}

		/if (${FindItemCount[=${_component}]} >= ${_count}) {
			/varset message COUNT_REACHED
			|/echo \agReturning, have ${_count} ${_component}
			/goto :return
		} 

		/autoinv

		/if (${_type.Equal[ITEM]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${reagent} 
			|/echo summoner > 0 ? ${FindItemCount[=${_summoner}]}
			/if (${FindItemCount[=${_summoner}]}>0) {
				
				/if (${reagent.Equal[NULL]}) {
						/echo ${FindItemCount[=${_summoner}]}
						/echo ${_summoner} summoning ${_component}	
						/call MQ2Cast "${_summoner}" item
						/delay 10
						/autoinv
						/goto :chkComponent
				} else {
					/if (${FindItemCount[=${reagent}]}>=10) {
						/echo ${FindItemCount[=${_summoner}]}
						/echo ${_summoner} summoning ${_component}	
						/call MQ2Cast "${_summoner}" item
						/delay 10
						/autoinv
						/goto :chkComponent
					} else {
						/varset message OUT_OF_REAGENTS
						/goto :return
					}
				}
				
			} else {
				/varset message SUMMON_CLICK_NOT_FOUND
				/goto :return
			}
			
		} else /if (${_type.Equal[DISC]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${reagent}
			/if (${FindItemCount[=${reagent}]}>=20) {
				/doability "${_summoner}"
				/delay 10
				/autoinv
			} else {
				/varset message OUT_OF_REAGENTS
				/goto :return
			}
			
		} else /if (${_type.Equal[ALT]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${reagent}
			/if (${FindItemCount[=${reagent}]}>=20) {
					/call MQ2Cast "${_summoner}" alt
					/delay 10
					/autoinv
				} else {
					/varset message OUT_OF_REAGENTS
					/goto :return
				}
		}
		
	:return
	/varset timer_component_check 300
/return	${message}


Sub FireDisc_Riposte
	/echo \aoFiring Riposte Disc!
	:monk
	/if (${Me.Class.ShortName.Equal[MNK]}) {
		/if (${Me.CombatAbilityReady[Whirlwind Discipline]}) {
			/doability "Whirlwind Discipline"
			/delay 10
			/varset bln_riposting TRUE
		} else {
			/varset bln_riposting FALSE
		}

	:rogue
	} else /if (${Me.Class.ShortName.Equal[ROG]}) {
		/if (${Me.CombatAbilityReady[Counterattack Discipline]}) {
			/doability "Counterattack Discipline"
			/delay 10
			/varset bln_riposting TRUE
		} else {
			/varset bln_riposting FALSE
		}
	
	:berserker
	} else /if (${Me.Class.ShortName.Equal[BER]}) {
		/if (${Me.CombatAbilityReady[Reckless Discipline]}) {
			/doability "Reckless Discipline"
			/delay 10
			/varset bln_riposting TRUE
		} else {
			/varset bln_riposting FALSE
		}
	
	:ranger
	} else /if (${Me.Class.ShortName.Equal[RNG]}) {
		/if (${Me.CombatAbilityReady[Weapon Shield Discipline]}) {
			/doability "Weapon Shield Discipline"
			/delay 10
			/varset bln_riposting TRUE
		} else {
			/varset bln_riposting FALSE
		}
	:bard
	} else /if (${Me.Class.ShortName.Equal[BRD]}) {
		/if (${Me.CombatAbilityReady[Deftdance Discipline]}) {
			/doability "Deftdance Discipline"
			/delay 10
			/varset bln_riposting TRUE
		} else {
			/varset bln_riposting FALSE
		}
	}
	:return
/return

Sub AutoAssist(bool _enabled)
	/varset bln_autoAssist ${_enabled}
/return

Sub EngageTargetMelee
	/call StickToTarget ${bln_engaged}
/return

Sub StickToTarget(bool _engageWhenInRange)
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,ENC,MAG,NEC,WIZ]}) /return
	/varset int_startStickDistance ${Spawn[id ${int_targetID}].MaxRangeTo}
	/varset int_currentStickDistance ${int_startStickDistance}
	/echo \aoStickToTarget \aw_engageWhenInRange:\ar ${_engageWhenInRange} \awdist: \ag${Target.Distance} \awcurrentstick: \ag${int_currentStickDistance} \awminstick: \ag${int_minStickDistance} \awmaxstick: \ag${int_startStickDistance} \n\awStick Command String:\ag ${str_stickCommand} ${int_currentStickDistance}
	
	:determineStick
	/if (${Target.Distance3D} > ${int_maxNavDistance}) {
		/docommand /${announceChannel} Target is too far away to get to!
		/if (${Navigation.Active}) /nav stop
		/echo \ao StickToTarget Target is too far, disengaging.
		/call Disengage
		/goto :return
	} else /if (${Target.Distance3D} > ${int_maxStickDistance} && ${Target.Distance} < ${int_maxNavDistance}) {
		/if (${Navigation.PathExists[target]}) {
			/echo \ar StickToTarget Target is within nav range, navigating
			/if (${Navigation.Active}) /nav stop 
			/declare navDelayWait int local 0
			/call Disengage

			:navToMeleeRange
				/nav target
				/delay 10
				/varcalc navDelayWait ${navDelayWait}+1
				/doevents CANTNAVTARGET
				/if (${Target.Distance3D} <= ${int_startStickDistance}) {
					/nav stop
					/goto :tryStick
				}
			/if (${navDelayWait} < 20) /goto :navToMeleeRange
		} else {
			/docommand /${announceChannel} NO PATH to target! Aborting!
			/echo \arNO PATH\aw to target! Aborting!
			/goto :return
		}
		
	}
	:tryStick
	/stick ${str_stickCommand} ${int_currentStickDistance}

	:tryKill
	/if (${_engageWhenInRange}) {
		/if (${Target.ID} && !${Target.Name.Find['s_corpse']} && ${Target.Type.Equal[NPC]} && !${Me.Combat}) {
			/echo  \awStickToTarget :tryKill \ag Attacking 
			/attack on
		} else /if (!${Target.ID} || ${Target.Name.Find['s_corpse']} || ${Target.Type.NotEqual[NPC]}) {
			/echo \awStickToTarget :tryKill \ar Failed! \awID?\ao${Target.ID} \awCorpse?\ao${Target.Name.Find['s_corpse']} \awNPC?\ao${Target.Type.NotEqual[NPC]}
			/call Disengage
		}
	}
	
:return
/return


Sub chkTargetDist
	/if (!${bln_engaged} || !${Me.Combat}) /return
	/if (!${Me.AutoFire} && (${Target.Distance} > ${Math.Calc[${int_currentStickDistance}*1.1].Int}  )) /call StickToTarget ${bln_engaged}
/return 

|**
 ----------------------------
 COMBAT ABILITy Routines
 ----------------------------
 Per class methods, does the basic kick/etc stuff
 ----------------------------
**|

Sub chkCA_BER
	/if (${Me.AbilityReady[Frenzy]}) /doability "Frenzy"
	/if (${Me.CombatAbilityReady[Rage Volley]}) /doability "Rage Volley"
/return

Sub chkCA_BRD
/return 

Sub chkCA_BST
	/if (${Me.AbilityReady[Kick]}) /doability "Kick"
/return

Sub chkCA_MNK
	/if (${Me.AbilityReady[Tiger Claw]}) /doability "Tiger Claw"
	/if (${Me.AbilityReady[Flying Kick]}) /doability "Flying Kick"
/return

Sub chkCA_PAL
	/if (${Me.AbilityReady[Bash]}) /doability "Bash"
	/call chkTaunt
/return 

Sub chkCA_RNG 
	/if (${Me.AbilityReady[Slam]} ) /doability "Slam"
/return 

Sub chkCA_ROG
	/if (${Me.AbilityReady[Backstab]}) /doability "Backstab"
/return

Sub chkCA_SHD
	/if (${Me.AbilityReady[Bash]}) /doability "Bash"
	/call chkTaunt
/return 

Sub chkCA_WAR
	/if (${Me.AbilityReady[Kick]}) /doability "Kick"
	/if (${Me.AbilityReady[Bash]}) /doability "Bash"
	/call chkTaunt
/return 

Sub chkTaunt 
	/if (${bln_tanking} && ${bln_taunting} && ${Me.AbilityReady[Taunt]} && ${Me.TargetOfTarget.CleanName.NotEqual[${Me.CleanName}]}) /doability "Taunt"
	/if (${bln_tanking} && ${bln_taunting} && !${timer_provoke} && ${bln_provokes_enabled}) /call chkProvoke
/return 




Sub chkProvoke
	/declare i int local 
	/for i 1 to ${int_provokes_total}	
		/if (${PROVOKE_${i}_Type.Equal[DISC]} && (${Me.Endurance} > ${PROVOKE_${i}_Cost}) &&  ${Me.CombatAbilityReady[${PROVOKE_${i}}]} ) {
			/echo \a-gProvoking \ag${Target.Name} \a-w with \ag${PROVOKE_${i}}
			/doability "${PROVOKE_${i}}"
		} else /if (${PROVOKE_${i}_Type.Equal[SPELL]} && (${Me.CurrentMana} > ${PROVOKE_${i}_Cost}) &&  ${Me.SpellReady[${PROVOKE_${i}}]}) {
			/echo \a-gProvoking \ag${Target.Name} \a-w with \ag${PROVOKE_${i}}
			/call MQ2Cast "${PROVOKE_${i}}"
		}
		/if (${Me.TargetOfTarget.CleanName.Equal[${Me.CleanName}]}) /goto :return
	/next i
	:return
	/if (${Me.TargetOfTarget.CleanName.Equal[${Me.CleanName}]}) /varset timer_provoke 500
/return


Sub chkRangedItem
	/if (${bln_ranged_enabled} && ${int_ranged_total}) {
		| make sure the ranged item is equiped
		/if (${FindItem[${RANGED_1}].InvSlot} != 11) {
			/echo \ag Swapping ${RANGED_1} to Slot 11 (Ranged)
			/exchange "${RANGED_1}" ranged
			/delay 10
		}
		| make sure our ammo is equiped in the ammoslot
		/if (${FindItemCount[${${RANGED_1_Component}}]}) {
			/if (${FindItem[${${RANGED_1_Component}}].InvSlot} != 22 && ${FindItem[${${RANGED_1_Component}}].InvSlot} != 11 ) {
				/echo \ag Swapping in "${${RANGED_1_Component}}" ammo
				/exchange "${${RANGED_1_Component}}" ammo
				/delay 10
			}
		} else {
			/echo \arOUT OF ${${RANGED_1_Component}}!
			/return
		}
		
		| Attempt to use ranged to hit target
		/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight} && ${Target.Distance} < ${FindItem[${RANGED_1}].Range}) {
			/ranged
			/doevents TooClose
		}
	}

/return

Sub pullTarget
	/if (${bln_provokes_enabled}) {
		/call chkProvoke
	} else /if (${bln_ranged_enabled}) {
		/call chkRangedItem
	}
/return 
