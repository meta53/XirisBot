|**
----------------------------
MELEE HANDLERS updated version 08.02.18
----------------------------
This library suppliments mq2melee functionality
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	Removed unused checks/subs that are retained by MQ2Melee.dll
	06.28.18	xiris	Moved downshits back to MQ2Melee since ROF2 Emu actually supports Combat.State
	11.22.21	xiris	Keeping this around as its useful in situations where I dont want to use melee
	03.22.22	xiris 	Updated KillMob Event to be cleaner (ID first)
	04.27.22	xiris 	Added chkCombatAbilities which can be called when we need to turn off
						MQ2Melee for whatever reason (like.. MPG Specialization not working on berserkers!)
**|

|-- Manual Downshits since mq2melee was being flakey at the time
|   Storing downflags in the BOT_CLS_Tooname.ini under the [downflags]
|   note the second token area isd the casting arguments in full
|   Usage downflagX=BuffName|itemID item|(opt)Target

|	[Downflags]
|		downflagCount=1
|		downflag1=Aura of White Petals|28953 item

#EVENT RESTICK		"#*#restick#*#"
#EVENT AERAMP		"#*#aerampON#*#"
#EVENT GETCLOSER	"Your target is too far away, get closer!"

#EVENT NoDS "<#1#> NoDS"
#EVENT NoDS "[#1#(msg)] NoDS#*#"

#EVENT YesDS "<#1#> YesDS#*#"
#EVENT YesDS "[#1#(msg)] YesDS#*#"

#EVENT Sing "<#1#> Sing #2#"
#EVENT Sing "[#1#(msg)] Sing #2#"

#EVENT Hush "<#1#> Hush#*#"
#EVENT Hush "[#1#(msg)] Hush#*#"

#EVENT RiposteDisc "<#1#> RiposteDisc#*#"
#EVENT RiposteDisc "[#1#(msg)] RiposteDisc#*#"

#EVENT AutoAssist "<#1#> AutoAssist #2#"
#EVENT AutoAssist "[#1#(msg)] AutoAssist #2#"

#EVENT Leashing "<#1#> Leashing #2#"
#EVENT Leashing "[#1#(msg)] Leashing #2#"

#EVENT XTargetRefresh "<#1#> RefreshXTarget#*#"
#EVENT XTargetRefresh "[#1#(msg)] RefreshXTarget#*#"


#define dsINI xiris_common/xiris_damageshields.ini

Sub InitEvents_Melee
 	/call RegisterBackgroundEvents Events_Melee 15
	/call RegisterRaidEvents Events_Melee_Engaged 4
/return

Sub Events_Melee
	/doevents NoDS
	/doevents YesDS
	/doevents Sing
	/doevents Hush
	/doevents ToggleMQ2Melee
	/doevents Leashing
	/doevents AutoAssist
	/doevents XTargetRefresh
/return

Sub Events_Melee_Engaged
	/doevents RESTICK
	/doevents AERAMP
	/doevents GETCLOSER
	/doevents RiposteDisc
/return

Sub EVENT_ToggleMQ2Melee(string _pluginState)
	/if (${_pluginState.Equal[ON]}) {
		/call toggleMQ2Melee TRUE
	} else {
		/call toggleMQ2Melee FALSE
	}
	/doevents flush TOGGLEMQ2MELEE
/return

Sub toggleMQ2Melee(bool _state) 
	/rs MQ2MELEE DISABLED
	/melee plugin=${_state}
	/melee save
/return


Sub EVENT_NoDS(_line, _sender, _eventParams)
	/varset bln_useDS FALSE
	| check to see if I have a DS, and if so kill it
	/declare ds int local 
	/for ds 1 to ${dsSpellTotal} 
		/call RemoveBuff ${dsSpells[${ds}]}
	/next ds

	| if I am a bard, switch to the noDS medley
	/if (${Me.Class.ShortName.Equal[BRD]} && ${Medley.Active})	/medley stop
	/if (${Me.Class.ShortName.Equal[BRD]} && !${Medley.Active}) /medley nods
	/if (${Me.Class.ShortName.Equal[BRD]}) /varset str_currentMedly nods
	/doevents flush NoDS
/return

Sub EVENT_YesDS(_line, _sender, _eventParams)
	/echo turning on DS
	/varset bln_useDS TRUE

	| if I am a bard, switch to the normal medley
	/if (${Me.Class.ShortName.Equal[BRD]} && ${Medley.Active})	/medley stop
	/if (${Me.Class.ShortName.Equal[BRD]} && !${Medley.Active}) /medley melee
	/if (${Me.Class.ShortName.Equal[BRD]}) /varset str_currentMedly melee
	/doevents flush YesDS
/return

Sub EVENT_Sing(_line, _sender, _eventParams)	
	/if (${Me.Class.ShortName.Equal[BRD]} && ${Medley.Active}) /medley stop
	/if (${Me.Class.ShortName.Equal[BRD]} && !${Medley.Active}) /medley ${_eventParams}
	/if (${Me.Class.ShortName.Equal[BRD]}) /varset str_currentMedly ${_eventParams}
	/doevents flush Sing
/return

Sub EVENT_Hush(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.Equal[BRD]} && ${Medley.Active}) /medley stop
	/doevents flush Hush
/return

Sub EVENT_RiposteDisc(_line, _sender, _eventParams)
	/echo turning riposte disc on
	/call FireRiposteDisc
	/doevents flush RiposteDisc
/return

Sub FireRiposteDisc
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


Sub Event_AutoAssist(string _eventParams)
	/if (${_eventParams.EQUAL[on]}) /call AutoAssistON 
	/if (${_eventParams.NOTEQUAL[on]}) /call AutoAssistOFF
/return

Sub AutoAssistON
	/bc ${Me.Name} is now AutoAssisting!
	/varset bln_autoAssist TRUE
/return

Sub AutoAssistOFF
	/bc ${Me.Name} is no longer AutoAssisting.
	/varset bln_autoAssist FALSE
/return

Sub Event_Leashing(string _eventParams)
	/if (${_eventParams.EQUAL[on]}) /call LeashingON 
	/if (${_eventParams.NOTEQUAL[on]}) /call LeashingOFF
/return 

Sub LeashingON
	/bc ${Me.Name} is now Leashing!
	/varset bln_useLeash TRUE
/return 

Sub LeashingOFF
	/bc ${Me.Name} is no longer Leashing.
	/varset bln_useLeash FALSE
/return

Sub Event_XTargetRefresh
	/call doRefreshXTarget
/return 


Sub doBandolier(string _bando)
	/cecho \ay Equipping set ${_bando}
	/equipset ${_bando}
	/delay 10
	/equipset ${_bando}
	/delay 10
/return

Sub chkDownshits
	/if (${dfTimer}) /return
	/if (${Target.ID} && ${Target.Type.Equal[NPC]}) /return
	|-- we don't buff at bind point because we might be naked.
	/call chkNaked
	/varset bln_naked ${Macro.Return}
	/if (${bln_naked}) /return
	
	|- downshitif
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || (${SpawnCount[npc radius 55]}>0 && (${Zone.ID} != 340 && ${Zone.ID} != 344 && ${Zone.ID} != 345)) || ${Me.Feigning} || ${Me.Invis}) { 
		/return
	}

	/if (!${bln_engaged} && !${Me.Combat}) {

		/if (${downflagCount} > 0) {
			|- run through array of downshits
			/declare df 		int 	local
			/declare tid 		int 	local ${Target.ID}
			/declare ds			int		local 
			/declare buffName 	string 	local
			/declare itemArgs	string  local
			/declare itemID		int     local
			/declare itemName	string  local
			/declare castArg 	string 	local
			/declare castTime	int     local
			/declare tgtName 	string 	local
			/declare tgtID 		int 	local
			/declare isSpell	bool	local	TRUE
			
			/for df 1 to ${downflagCount} 
				| down flags look like: 
				| downflag1=Cloak of Discord|"Cloak of Discord" gem6
				| downflag2=Legacy of Nettles|"Legacy of Nettles" gem 8 -targetid|${Me.ID}  
				| downflag3=Fiery Might|28988 item
				/varset buffName  ${downflag${df}.Token[1,|]}
				/varset castArg   ${downflag${df}.Token[2,|]}|${downflag${df}.Token[3,|]}
				/varset tgtName   ${downflag${df}.Token[3,|]}
				/varset isSpell TRUE
				/if (${castArg.Find[item]}>0) /varset isSpell FALSE
				/if (${isSpell} && !${Me.SpellReady[${buffName}]}) /goto :nextDF

				|- If we have Damage Shields turned off, check to see if this is a damage shield before casting it
				|  If found, skip the downflag
				/if (!${bln_useDS}) {
					/for ds 1 to ${dsSpellTotal}
						/if (${buffName.Equal[${dsSpells[${ds}]}]}) /goto :nextDF
					/next ds
				}
				/call RunRaidEvents	
				/if (${bln_engaged}) /return
				|/echo buffName: ${buffName} castArg: ${castArg} tgtName: ${tgtName} ready: ${Me.SpellReady[${buffName}]} isSpell: ${isSpell} ?: ${castArg.Find[item]}
				|/echo ${Spell[${buffName}].Stacks} && !${Me.Buff[${buffName}].ID}
				/if (${Spell[${buffName}].Stacks} && !${Me.Buff[${buffName}].ID}) {

					/delay 10
					|- save existing target for buffs that take a target argument (3)
					/if (${tgtName.NotEqual[NULL]}) 		/varset tgtID ${Spawn[pc ${tgtName}].ID}
					/if (${SpawnCount[ID ${tgtID}]} >= 1 )  /call TrueTarget ${tgtID}
					|/echo downflag${df}: ${downflag${df}.Token[1,|]} | ${downflag${df}.Token[2,|]}|${downflag${df}.Token[3,|]}
					
					| - Bard specific handler using mq2medley
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/varset itemArgs ${downflag${df}.Token[2,|]}
						/varset itemID ${itemArgs.Token[1, ]}
						/varset itemName ${FindItem[${itemID}]}
						/varcalc castTime ${FindItem[${itemID}].CastTime} / 1000
						/echo bard downshit : ${itemID} ${itemName} ${castTime}
						/if (!${Medley}) /medley ${str_currentMedly}
						/medley queue "${itemName}" -interrupt
						/delay ${castTime}s
					} else {
						/echo Downflag Cast ${castArg}
						/call MQ2Cast ${castArg}
					}
					
					:dfcast
					/if (${Me.Casting.ID}) /goto :dfcast
			
					|- return existing target
					/if (${tgtName.NotEqual[NULL]}) /call TrueTarget ${tid}
					
				}
			:nextDF
			/next df
			|- reset the DownFlag timer to 20seconds
			/varset dfTimer 20s
			
		}
	}
/return

Sub chkComponentsAll
	|/echo chkComponentsAll ${bln_componentCheck}
	/if (${bln_componentCheck}) {
		/declare i 			int 	local	1
		/declare component	string 	local   NULL
		/declare compType	string  local   NULL
		/declare count		int		local	0
		/declare summoner	string	local	NULL
		/declare reagent	string  local   NULL
		/for i 1 to ${int_componentTotal} 
			/if (${COMPONENT_${i}_Check}) {
				/varset component 	"${COMPONENT_${i}}"
				/varset compType	${COMPONENT_${i}_Type}
				/varset count		${COMPONENT_${i}_Count}
				/varset summoner	"${COMPONENT_${i}_Summoner}"
				/varset reagent		"${COMPONENT_${i}_Reagent}"
				
				/call chkComponents ${component} ${count} ${compType} ${summoner} ${reagent}
				/delay 10
				/if (${Macro.Return.Equal[SUMMON_ITEM_NOT_FOUND]}) {
					/echo SUMMON_ITEM_NOT_FOUND turning  this component OFF.
					/varset COMPONENT_${i}_Check FALSE
				}
				/if (${Macro.Return.Equal[OUT_OF_REAGENTS]}) {
					/echo OUT_OF_REAGENTS turning  this component OFF.
					/varset COMPONENT_${i}_Check FALSE
				}
				
			}
		/next i
	}
/return

Sub EVENT_DISC_NOTAVAILABLE(string line)
	/echo EVENT_DISC_NOTAVAILABLE 
	/echo $(line)
	/doevents flush DISC_NOTAVAILABLE
/return


Sub chkComponents(string _component, int _count, string _type, string _summoner, string _reagent)
	/if (${SpawnCount[npc radius ${int_campRadius}]} > 0) /return
	/if (${FindItemCount[=${_component}]}>=${_count}) /return
	/if (${strPullType.Equal[SPELL]}) /return
	/if (!${Defined[_reagent]}) /declare _reagent string local NULL
	/if (!${Defined[message]}) /declare message string local SUCCCESS
	:chkComponent
		/echo checking components: desired:${_count} have: ${FindItemCount[=${_component}]}
		/autoinv
		/if (${FindItemCount[=${_component}]}>=${_count}) /return
		/if (${SpawnCount[npc radius ${int_campRadius}]} > 0) /return

		/if (${_type.Equal[ITEM]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${_reagent} 
			/echo summoner > 0 ? ${FindItemCount[=${_summoner}]}
			/if (${FindItemCount[=${_summoner}]}>0) {
				/echo gt10? (${FindItemCount[=${_reagent}]}>=10) || NULL? ${_reagent.Equal[NULL]]}
				/if (${_reagent.Equal[NULL]}) {
						/echo ${FindItemCount[=${_summoner}]}
						/echo ${_summoner} summoning ${_component}	
						/call MQ2Cast "${_summoner}" item
						/delay 50
						/autoinv
						/goto :chkComponent
				} else {
					/if (${FindItemCount[=${_reagent}]}>=10) {
						/echo ${FindItemCount[=${_summoner}]}
						/echo ${_summoner} summoning ${_component}	
						/call MQ2Cast "${_summoner}" item
						/delay 50
						/autoinv
						/goto :chkComponent
					} else {
						/varset message OUT_OF_REAGENTS
						/goto :return
					}
				}
				
			} else {
				/varset message SUMMON_ITEM_NOT_FOUND
				/goto :return
			}
			
		} else /if (${_type.Equal[DISC]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${_reagent}
			/if (${FindItemCount[=${_reagent}]}>=20) {
				/doability "${_summoner}"
				/delay 50
				/autoinv
			} else {
				/varset message OUT_OF_REAGENTS
				/goto :return
			}
			
		} else /if (${_type.Equal[ALT]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${_reagent}
			/if (${FindItemCount[=${_reagent}]}>=20) {
					/call MQ2Cast "${_summoner}" alt
					/delay 50
					/autoinv
				} else {
					/varset message OUT_OF_REAGENTS
					/goto :return
				}
		}
:return
/return	${message}


Sub EVENT_GETCLOSER
	/if ((!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged}) && ${timer_getCloser}) /return 
	/doevents flush GETCLOSER
	/varset timer_getCloser ${timer_getCloser.OriginalValue}
	| If the target is less than max stick distance, adjust the stick
	/call StickToTarget
/return

Sub StickToTarget
	|/echo dist: ${Target.Distance} currentstick: ${int_currentStickDistance} minstick: ${int_minStickDistance} maxstick: ${int_maxStickDistance}
	:tryStick
	
	/if (${Target.Distance} <= ${int_maxStickDistance} ) {
		|/echo currentstick: ${int_currentStickDistance} minstick: ${int_minStickDistance} maxstick: ${int_maxStickDistance}
		/if (${int_currentStickDistance} > ${int_minStickDistance})  /varcalc int_currentStickDistance ${int_currentStickDistance} - 5
		/stick ${stickCmd} ${int_currentStickDistance}
	} else /if (${Target.Distance} > ${int_maxStickDistance} && ${Target.Distance} < ${int_maxNavDistance}) {
		/declare navDelayWait int local 0
		/stick off
		/nav target
		:navDelay
		/varcalc navDelayWait ${navDelayWait}+1
		/if (${Target.Distance} < ${int_maxStickDistance}) {
			/nav stop
			/goto :tryStick
		} else /if (${navDelayWait} < 20) {
			/delay 1
			/goto :navDelay
		}
	} else {
		/rs Target is too far away to get to.
		/if (${Navigation.Active}) /nav stop
		/stick off
		/melee reset
		/return
	}
/return


Sub EVENT_RESTICK
	/if (${bln_engaged}) {
		/call StickToTarget
	}
	/doevents flush RESTICK
/return


Sub chkTargetDist
	/if (!${bln_engaged} || !${Me.Combat}) /return
	/if (!${Me.AutoFire} && (${Target.Distance} > ${Math.Calc[${int_currentStickDistance}*1.1].Int}  )) /call StickToTarget
/return 




Sub EVENT_AERAMP
	/if (${Me.Combat}) {
		/echo FIRING AERAMP
		/if (${Me.Class.ShortName.Equal[MNK]}) {
			/call MQ2Cast "256" alt
		}
		/if (${Me.Class.ShortName.Equal[WAR]}) {
			/call MQ2Cast "99" alt
		}
		
		/if (${Me.Class.ShortName.Equal[BER]}) {
			/echo I should  be firing AE RAMP
			/call MQ2Cast "99" alt
		}
	}
	/doevents flush AERAMP
/return

|-- Swap sets
Sub  swapWeaponSets(type)
	/if (${type.Equal[Ranged]}) {
	
	} else /if (${type.Equal[Melee]}) {
	
	} else /if (${type.Equal[NotBlunt]}) {
	
	} else /if (${type.Equal[NotPierce]}) {
	
	} else /if (${type.Equal[Main]}) {
	
	}
/return

|-- Not Used
Sub downshitInterrupt
/return


|-- Used 04/27/2022 due to mq2melee acting retarded in MPG: Specialization
Sub chkCombatAbilities
	/if (${Melee.Enable}) /return
	| Berserkers
	/if (${Me.AbilityReady[Frenzy]} && ${Me.Class.Name.Equal[Berserker]} && ${Me.Combat} && ${Target.Distance}<=${Target.MaxRangeTo}) /doability "Frenzy"
	/if (${Me.CombatAbilityReady[Rage Volley]} && ${Me.Class.Name.Equal[Berserker]} && ${Me.Combat} && ${Target.Distance}<=${Target.MaxRangeTo}) /doability "Rage Volley"

	| Rogues
	/if (${Me.AbilityReady[Backstab]} && ${Me.Class.Name.Equal[Rogue]} && ${Me.Combat} && ${Target.Distance}<=${Target.MaxRangeTo}) /doability "Backstab"

	| Monks
	/if (${Me.AbilityReady[Tiger Claw]} && ${Me.Class.Name.Equal[Rogue]} && ${Me.Combat} && ${Target.Distance}<=${Target.MaxRangeTo}) /doability "Tiger Claw"
	/if (${Me.AbilityReady[Flying Kick]} && ${Me.Class.Name.Equal[Rogue]} && ${Me.Combat} && ${Target.Distance}<=${Target.MaxRangeTo}) /doability "Flying Kick"
/return

Sub chkCA_BER
/return

Sub chkCA_ROG
	
/return


|-- Not Used
Sub killthis
	/killthis
/return


Sub meleeReset
	/echo meleeReset
	/melee reset
/return



Sub chk_casterProvoke(string _provokeSpell)
	/if (!${Defined[shd_minProvokeMana]}) {
		/declare shd_minProvokeMana int	outer 200
	}
	
	/if (${provokeTimer}) {
		
	} else {
		/if (${Me.CurrentMana} < ${shd_minProvokeMana}) {
			 |/echo |- attemptProvoke -| I do not have enough mana to use [${_provokeSpell}].
		} else {

			/if (!${Me.SpellReady[${_provokeSpell}]}) {
				|/echo |- attemptProvoke -| [${_provokeSpell}] is not ready.
			} else {
				/call MQ2Cast "${_provokeSpell}" ${spell_hateGem}
				/if (${Spawn[${Me.TargetOfTarget}].CleanName.Equal[${Me}]}) /varset provokeTimer 600
			}
		}
	}
/return

Sub chk_warProvoke(string _provokeDisc)
	/if (!${Defined[war_minBellowEnd]}) {
		/if (${_provokeDisc.Equal[Ancient: Chaos Cry]}) {
			/declare war_minBellowEnd int outer 300
		} else /if (${_provokeDisc.Equal[Bellow of the Mastruq]}) {
			/declare war_minBellowEnd int outer 275
		} else /if (${_provokeDisc.Equal[Incite]}) {
			/declare war_minBellowEnd int outer 250
		} else /if (${_provokeDisc.Equal[Berate]}) {
			/declare war_minBellowEnd int outer 200
		} else /if (${_provokeDisc.Equal[Bellow]}) {
			/declare war_minBellowEnd int outer 175
		} else /if (${_provokeDisc.Equal[Provoke]}) {
			/declare war_minBellowEnd int outer 85
		}
	}
	
	/if (${provokeTimer}) {
		 |/echo |- attemptProvoke -| I have not met my recast timer for [${_provokeDisc}] - [${provokeTimer}].
	} else {
	
		/if (${Me.Endurance} < ${war_minBellowEnd}) {
			 |/echo |- attemptProvoke -| I do not have enough endurance to use [${_provokeDisc}].
		} else {

			/if (!${Me.CombatAbilityReady[${_provokeDisc}]}) {
				|/echo |- attemptProvoke -| [${_provokeDisc}] is not ready.
			} else {
				/doability "${_provokeDisc}"
				/if (${Spawn[${Me.TargetOfTarget}].CleanName.Equal[${Me}]}) /varset provokeTimer 600
			}
		}
	}
/return




Sub xbot_initialize_melee

	|-- [Settings] : Melee Defaults
	/declare 	MinStrikeEndurance 		int		outer	200
	/declare 	MinBellowEndurance		int		outer	200FireRiposteDisc
	/declare    provokeTimer            timer   outer   0
	/declare 	stickCmd				string	outer   ${Ini[botINI,Sticking,stickCmd,behindonce 35]}
	/declare 	bln_chkComponents		bool	outer	TRUE
	/declare 	bln_tgtDeadSoon			bool	outer	FALSE
	/declare	timer_reAssist			timer	outer	5s
	/declare    bln_riposting			bool    outer   FALSE

	/declare 	int_currentStickDistance		int		outer   25
	/declare	int_maxStickDistance 	int 	outer 	70
	/declare 	int_minStickDistance	int		outer 	10
	/declare    int_maxNavDistance		int		outer   300
	/declare	timer_getCloser			timer	outer	30

	|-- [Settings] : Downflags
	/declare 	dfTimer					timer	outer 	0
	/declare	downflagCount			int		outer	${Ini[botINI,Downflags,downflagCount,0]}
	/declare df int local
		/for df 1 to ${downflagCount} 
			/declare downflag${df} 		string 	outer	${Ini[botINI,Downflags,downflag${df}]}
		/next df

	
	|-- [Settings] : Holyflags
	/declare	holyflagCount			int		outer	${Ini[botINI,Holyflags,holyflagCount,0]}
	/declare hf int local
		/for hf 1 to ${holyflagCount} 
			/declare holyflag${hf} 		string 	outer	${Ini[botINI,Holyflags,holyflag${hf}]}
		/next hf


	|- [Settings] : Components
	/declare	int_componentTotal		int		outer	${Ini[botINI,Components,int_componentTotal,0]}
	/declare 	bln_componentCheck		bool	outer   ${If[${int_componentTotal}>0,TRUE,FALSE]}
	/echo bln_componentCheck ${bln_componentCheck}
		/declare comp int local
			/for comp 1 to ${int_componentTotal} 
				/declare COMPONENT_${comp} 			string 	outer ${Ini[botINI,Components,COMPONENT_${comp}]}
				/declare COMPONENT_${comp}_Check	bool 	outer ${Ini[botINI,Components,COMPONENT_${comp}_Check,FALSE]}
				/declare COMPONENT_${comp}_Count 	int 	outer ${Ini[botINI,Components,COMPONENT_${comp}_Count]}
				/declare COMPONENT_${comp}_Type  	string 	outer ${Ini[botINI,Components,COMPONENT_${comp}_Type]}
				/declare COMPONENT_${comp}_Summoner string	outer ${Ini[botINI,Components,COMPONENT_${comp}_Summoner]}
				/declare COMPONENT_${comp}_Reagent 	string	outer ${Ini[botINI,Components,COMPONENT_${comp}_Reagent]}
			/next comp
	

	|--[Settings] : Spell & Support Definitions
	/declare item_pull				string	outer	${Ini[botINI,Puller,item_pull,"Shuriken of Eternity"]}
	/declare item_pull_clean		string	outer	${Ini[botINI,Puller,item_pull_clean,Shuriken of Eternity]}
	/declare item_summon			string	outer	${Ini[botINI,Puller,item_summon,"Ton Po's Mystical Pouch"]}
	
	/declare spell_pull				string	outer	${Ini[botINI,Puller,spell_pull,"Terror of Discord"]}
	/declare spell_pullGem			string 	outer	${Ini[botINI,Puller,spell_pullGem,"gem2"]}
	/declare spell_hate				string	outer	${Ini[botINI,Puller,spell_hate,"Terror of Discord"]}
	/declare spell_hateGem			string  outer   ${Ini[botINI,Puller,spell_hateGem,"gem2"]}
	/declare spell_aehate			string	outer	${Ini[botINI,Puller,spell_aehate,"Dread Gaze"]}
	/declare spell_aehateGem		string	outer	${Ini[botINI,Puller,spell_aehate,"gem3"]}
	/declare spell_snare			string	outer	${Ini[botINI,Puller,spell_snare,"Festering Darkness"]}
	/declare bln_useSnare			bool	outer	${Ini[botINI,Puller,bln_useSnare,FALSE]}
	/declare war_ca_hate_1			string	outer	${Ini[botINI,Puller,war_ca_hate_1,Bazu Bellow]}
	/declare war_ca_hate_2			string	outer 	${Ini[botINI,Puller,war_ca_hate_2,Bellow of the Mastruq]}
	/declare int_castDistance		int		outer	${Ini[botINI,Puller,int_castDistance,120]}
	
	|-- [Settings] Damage shield/ no damage shield (specifically for events like pixxt riel tavas)
	/declare bln_useDS				bool	outer	TRUE	
	/declare dsSpellTotal			int		outer   ${Ini[dsINI,Settings,SpellCount,0]}
	/declare dsSpells[${dsSpellTotal}]	string outer

	/declare ds int local 
		/for ds 1 to ${dsSpellTotal}
			/varset dsSpells[${ds}] ${Ini[dsINI,Spells,DS${ds}]}
			|/echo Added DS${ds} spell ${dsSpells[${ds}]}
		/next ds

	/declare dsSongsTotal	int		outer   ${Ini[dsListINI,Settings,SongCount,0]}
	/declare dsSongs[${dsSongsTotal}]	string outer

	/cecho \ao xbot_initialize_melee
	/call InitEvents_Melee
/return
