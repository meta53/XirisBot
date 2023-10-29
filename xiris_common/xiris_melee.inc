|**
----------------------------
xiris_melee.inc
----------------------------
This library supplements|replaces mq2melee functionality

Manual Downshits since mq2melee was being flakey at the time
	Storing downflags in the BOT_CLS_Tooname.ini under the [downflags]
	note the second token area isd the casting arguments in full
	Usage downflagX=BuffName|itemID item|(opt)Target

	[Downflags]
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
**|

|-- 
#EVENT RESTICK			"#*#restick#*#"
#EVENT AERAMP			"#*#aerampON#*#"
#EVENT GETCLOSER		"#*#Your target is too far away#*#"
#EVENT CANTSEETARGET	"#*#You cannot see your target#*#"
#EVENT CANTNAVTARGET	"#*#Could not locate#*#"

#EVENT RiposteDisc		"#1# tells the group#*#'RiposteDisc#*#'"
#EVENT RiposteDisc		"#1# tells you#*#'RiposteDisc#*#'"
#EVENT RiposteDisc		"#1# tells the raid#*#'RiposteDisc#*#'"
#EVENT RiposteDisc 		"[ #1# #*#]#*#RiposteDisc#*#"

#EVENT AutoAssist		"#1# tells the group#*#'AutoAssist #2#'"
#EVENT AutoAssist		"#1# tells you#*#'AutoAssist #2#'"
#EVENT AutoAssist		"#1# tells the raid#*#'AutoAssist #2#'"
#EVENT AutoAssist 		"[ #1# #*#]#*#AutoAssist #2#"

#EVENT UpdateStickCMD	"#1# tells the group#*#'UpdateStick #2#'"
#EVENT UpdateStickCMD	"#1# tells you#*#'UpdateStick #2#'"
#EVENT UpdateStickCMD	"#1# tells the raid#*#'UpdateStick #2#'"
#EVENT UpdateStickCMD	"[ #1# #*#]#*#UpdateStick #2#"

#define dsINI xiris_common/xiris_damageshields.ini

Sub InitEvents_Melee
 	/call RegisterBackgroundEvents Events_Melee 15
	/call RegisterRaidEvents Events_Melee_Engaged 4
	/call RegisterRapidEvents Events_Melee_Rapid 1
/return

Sub Events_Melee
	/doevents Leashing
	/doevents AutoAssist
	/doevents CANTNAVTARGET
/return

Sub Events_Melee_Engaged
	/doevents AERAMP
	/doevents GETCLOSER
	/doevents CANTSEETARGET
	/doevents RiposteDisc
/return

Sub Events_Melee_Rapid 
	/doevents RESTICK
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
	/varset mq2meleeEnabled ${_state}
	/if (${state}) {
		/docommand /${announceChannel} MQ2MELEE ENABLED
		/melee plugin=1
	} else {
		/docommand /${announceChannel} MQ2MELEE DISABLED
		/melee plugin=0
	}
/return

Sub Event_UpdateStickCMD(_line, _sender, _eventParams)
	/echo updateStickCMD ${_sender}
	/echo _eventParams	${_eventParams}

	/doevents flush UpdateStickCMD
/return

Sub EVENT_RiposteDisc(_line, _sender, _eventParams)
	/echo turning riposte disc on
	/call FireRiposteDisc
	/doevents flush RiposteDisc
/return

Sub FireRiposteDisc
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


Sub Event_AutoAssist(string _eventParams)
	/if (${_eventParams.Equal[on]}) /call AutoAssistON 
	/if (${_eventParams.NotEqual[on]}) /call AutoAssistOFF
/return

Sub AutoAssistON
	/dgt ${Me.Name} is now Auto Assisting!
	/varset bln_autoAssist TRUE
/return

Sub AutoAssistOFF
	/dgt ${Me.Name} is no longer Auto Assisting.
	/varset bln_autoAssist FALSE
/return


Sub doBandolier(string _bando)
	/echo \ay Equipping set ${_bando}
	/autoinv
	/autoinv
	/delay 5
	/if (${InvSlot[offhand].Item.ID}) {
		/unequip 14
		/delay 5
		/autoinv
	}
	/if (${InvSlot[mainhand].Item.ID}) {
		/unequip 13
		/delay 5
		/autoinv
	}

	/equipset ${_bando}
/return

Sub chkDownshits
	/if ((${Target.ID} && ${Target.Type.Equal[NPC]}) || ${bln_engaged} || ${timer_check_downshits}) /return
	|-- we don't buff at bind point because we might be naked.
	/call chkNaked
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
			/declare svdTGTID 	string 	local	${Target.ID}
			/declare tgtID 		int 	local
			/declare dsType  	string  local   SPELL
			/declare wornSlot   string  local 
			/declare itemInSlot string  local
			/declare hadToMemBuffs bool     local FALSE
			
			/for df 1 to ${downflagCount} 
				| down flags look like: 
				| downflag1=Cloak of Discord|"Cloak of Discord" gem6
				| downflag2=Legacy of Nettles|"Legacy of Nettles" gem 8 -targetid|${Me.ID}  
				| downflag3=Fiery Might|28988 item
				| downflag4=Eldritch Rune|Eldritch Rune alt
				/if (!${Me.FreeBuffSlots}) /return
				/varset buffName  ${downflag${df}.Token[1,|]}
				/varset castArg   ${downflag${df}.Token[2,|]}|${downflag${df}.Token[3,|]}
				/varset tgtID     ${downflag${df}.Token[3,|]}
				/varset dsType SPELL
				/if (${castArg.Find[item]}>0) {
					/varset dsType ITEM
				} else /if (${castArg.Find[alt]}>0) {
					/varset dsType ALT
				}

				|- If we have Damage Shields turned off, check to see if this is a damage shield before casting it
				|  If found, skip the downflag
				/if (!${bln_useDS}) {
					/for ds 1 to ${dsSpellTotal}
						/if (${buffName.Equal[${dsSpells[${ds}]}]}) /goto :nextDF
					/next ds
				}
				/call RunAllEvents	
				

				/if (${bln_engaged}) /return
			
				|/echo BuffName: ${buffName} castArg: ${castArg} tgtID: ${tgtID} ready: ${Me.SpellReady[${buffName}]} dsType: ${dsType} ?: ${castArg.Find[item]}
				|/echo Stacks? ${Spell[${buffName}].Stacks} && Do I have it? ${Bool[${Me.Buff[${buffName}].ID}]}
				/if (${Spell[${buffName}].Stacks} && !${Bool[${Me.Buff[${buffName}].ID}]}) {
					
					/if (${dsType.Equal[SPELL]} && !${Me.SpellReady[${buffName}]}) {
						/echo \ay${buffName} \awis \arNOT \awReady, Memorizing
						| Spell is not ready, memorize it
						/varset hadToMemBuffs TRUE
						/call chkSpellMemorized ${castArg} TRUE
						/delay 10
					} 

					|- save existing target for buffs that take a target argument (3)
					/if (${SpawnCount[ID ${tgtID}]} >= 1 )  /call TrueTarget ${tgtID}
					|/echo downflag${df}: ${downflag${df}.Token[1,|]} | ${downflag${df}.Token[2,|]}|${downflag${df}.Token[3,|]}
					
					| - Bard specific handler using mq2medley
					| - Note mq2medley will not auto swap items!
					/varset itemArgs ${downflag${df}.Token[2,|]}
					/varset itemID ${itemArgs.Token[1, ]}
					/varset itemName ${FindItem[${itemID}].Name}
					/if (${dsType.Equal[ITEM]} && !${FindItem[${itemID}].ID}) {
						/echo \arCannot find downflag_${df}: \aw${itemID}!
						/goto :nextDF
					}
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/echo ${FindItem[${itemName}].TimerReady} ?
						/if (${dsType.Equal[ITEM]} && ${FindItem[${itemName}].TimerReady}==0) {
							/call MQ2Medley ITEM "${itemName}" ${Me.ID} TRUE
						} else /if (${dsType.Equal[SPELL]}) {
							/call MQ2Medley SONG ${buffName} ${Me.ID} TRUE
						} else /if (${dsType.Equal[ALT]}) {
							/call MQ2Medley ALT ${buffName} ${Me.ID} TRUE
						}
					} else {
						/echo \ayDownflag (\ag${df}\ay) Casting \ay${buffName} \ag${downflag${df}.Token[2,|]}
						/call MQ2Cast ${castArg}
					}
					:dfcast
					/if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /goto :dfcast
				}
			:nextDF
			/next df
			|- return existing spellset
			/if (${hadToMemBuffs}) /memspellset ${str_spellset}
			|- return existing target
			/if (${Bool[${svdTGTID}]}) /call TrueTarget ${svdTGTID}

			}

		}
		/varset timer_check_downshits 30s
	}
/return

Sub chkComponentsAll
	/if (${bln_componentCheck} && !${timer_component}) {
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
				/echo calling chkComponents
				/call chkComponents ${component} ${count} ${compType} ${summoner} ${reagent}
				/delay 10
				/if (${Macro.Return.Equal[SUMMON_ITEM_NOT_FOUND]}) {
					/echo SUMMON_ITEM_NOT_FOUND turning  this component OFF.
					/docommand /${announceChannel} SUMMON_ITEM_NOT_FOUND  ${summoner} turning  this component OFF.
					/varset COMPONENT_${i}_Check FALSE
				}
				/if (${Macro.Return.Equal[OUT_OF_REAGENTS]}) {
					/echo OUT_OF_REAGENTS turning  this component OFF.
					/docommand /${announceChannel} OUT_OF_REAGENTS  ${component} turning  this component OFF.
					/varset COMPONENT_${i}_Check FALSE
				}
			}
			/autoinv
		/next i
	}
/return

Sub EVENT_DISC_NOTAVAILABLE(string line)
	/echo EVENT_DISC_NOTAVAILABLE 
	/echo $(line)
	/doevents flush DISC_NOTAVAILABLE
/return


Sub chkComponents(string _component, int _count, string _type, string _summoner, string _reagent)
	/echo chkComponents ${_component}, ${_count}, ${_type}, ${_summoner}, ${_reagent}
	/echo (${FindItemCount[=${_component}]}>=${_count}?)
	/if (${SpawnCount[npc radius ${int_campRadius}]} > 0 && ${Zone.ShortName.NotEqual[thundercrest]}  && ${Zone.ShortName.NotEqual[guildhall]}  && ${Zone.ShortName.NotEqual[guildlobby]}) {
		/echo \a-rReturning due to spawn
		/return
	} 

	/if (!${Defined[_reagent]})	/declare _reagent string local NULL
	/if (!${Defined[message]})	/declare message string local SUCCCESS

	:chkComponent
		/echo \awChecking components: \aodesired:\ay${_count} \aohave: \ag${FindItemCount[=${_component}]}

		/if (${FindItemCount[=${_component}]} >= ${_count}) {
			/varset message COUNT_REACHED
			/echo \agReturning, have ${_count} ${_component}
			/goto :return
		} 

		/autoinv

		/if (${_type.Equal[ITEM]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${_reagent} 
			/echo summoner > 0 ? ${FindItemCount[=${_summoner}]}
			/if (${FindItemCount[=${_summoner}]}>0) {
				
				/if (${_reagent.Equal[NULL]}) {
						/echo ${FindItemCount[=${_summoner}]}
						/echo ${_summoner} summoning ${_component}	
						/call MQ2Cast "${_summoner}" item
						/delay 10
						/autoinv
						/goto :chkComponent
				} else {
					/if (${FindItemCount[=${_reagent}]}>=10) {
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
				/varset message SUMMON_ITEM_NOT_FOUND
				/goto :return
			}
			
		} else /if (${_type.Equal[DISC]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${_reagent}
			/if (${FindItemCount[=${_reagent}]}>=20) {
				/doability "${_summoner}"
				/delay 10
				/autoinv
			} else {
				/varset message OUT_OF_REAGENTS
				/goto :return
			}
			
		} else /if (${_type.Equal[ALT]}) {
			/echo ${_type}: ${_summoner} summoning ${_component} with ${_reagent}
			/if (${FindItemCount[=${_reagent}]}>=20) {
					/call MQ2Cast "${_summoner}" alt
					/delay 10
					/autoinv
				} else {
					/varset message OUT_OF_REAGENTS
					/goto :return
				}
		}
		
:return
/varset timer_component 30s
/return	${message}


Sub EVENT_GETCLOSER
	/if (!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged} || ${timer_getCloser}) /return 
	/doevents flush GETCLOSER
	/varset timer_getCloser 4s
	| If the target is less than max stick distance, adjust the stick
	/call StickAdjust
/return

SUB EVENT_CANTSEETARGET
	/if (!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged} || ${timer_cantSee}) /return 
	/doevents flush CANTSEETARGET
	/varset timer_cantSee 10s 
	/face fast
/return

SUB EVENT_CANTNAVTARGET
	/if (!${Target.ID} || ${Target.PctHPs} == 0 || !${bln_engaged} || ${timer_cantNav}) /return 
	/doevents flush CANTNAVTARGET
	/varset timer_cantNav 4s 
	/echo \ar Cannot NAVTARGET ${bln_following}
	| If the target is less than 50 range
	/if (!${bln_following}) {
		/if (${Select[${Me.Class.ShortName},BRD,BER,MNK,ROG,RNG,PAL,SHD,WAR]} && ${Target.ID}) {
			/if (!${bln_tanking})  {
				/warp loc ${Target.Y} ${Target.X} ${Target.Z}
			} else {
				/warp f 10
			} 
		}
	} else {
		/echo \aoFollowing ${str_followTarget}
		/call TrueTarget ${Spawn[pc ${str_followTarget}].ID}
		/delay 5
		/warp loc ${Target.Y} ${Target.X} ${Target.Z}
	}
/return

Sub StickAdjust
	
	/if (${int_currentStickDistance} > ${int_minStickDistance} && ${Target.Distance} <= ${int_currentStickDistance}) {
		/echo \agStickAdjust \ao${int_currentStickDistance} \aw> \ao${int_minStickDistance} \ay(\aw${Target.Distance})
		/varcalc int_currentStickDistance ${int_currentStickDistance} - 5
		/if (${int_currentStickDistance} < ${int_minStickDistance}) /varset int_currentStickDistance ${int_minStickDistance} 
		/stick ${stickCmd} ${int_currentStickDistance}
	} 
/return

Sub EngageTargetMelee
	/call StickToTarget ${bln_engaged}
/return


Sub StickToTarget(bool _engageWhenInRange)
	/if (${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[PRIEST]}) /return
	/varset int_startStickDistance ${Spawn[id ${int_targetID}].MaxRangeTo}
	/varset int_currentStickDistance ${int_startStickDistance}
	/echo \aoStickToTarget \aw_engageWhenInRange:\ar ${_engageWhenInRange} \awdist: \ag${Target.Distance} \awcurrentstick: \ag${int_currentStickDistance} \awminstick: \ag${int_minStickDistance} \awmaxstick: \ag${int_startStickDistance} \n\awStick Command String:\ag ${stickCmd} ${int_currentStickDistance}
	
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
	/stick ${stickCmd} ${int_currentStickDistance}

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


Sub EVENT_RESTICK
	/if (${bln_engaged} && (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}))  {
		/call StickToTarget ${bln_engaged}
	}
	/doevents flush RESTICK
/return


Sub chkTargetDist
	/if (!${bln_engaged} || !${Me.Combat}) /return
	/if (!${Me.AutoFire} && (${Target.Distance} > ${Math.Calc[${int_currentStickDistance}*1.1].Int}  )) /call StickToTarget ${bln_engaged}
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
	/if (${mq2meleeEnabled}) /return
	/call chkCA_${Me.Class.ShortName}
/return

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
	/if (${bln_tanking} && ${bln_taunting} && ${Me.AbilityReady[Taunt]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.Name}]}) /doability "Taunt"
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
				/if (${Spawn[${Me.TargetOfTarget}].CleanName.Equal[${Me.Name}]}) /varset provokeTimer 600
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
		 /echo |- attemptProvoke -| I have not met my recast timer for [${_provokeDisc}] - [${provokeTimer}].
	} else {
	
		/if (${Me.Endurance} < ${war_minBellowEnd}) {
			 /echo |- attemptProvoke -| I do not have enough endurance to use [${_provokeDisc}].
		} else {

			/if (!${Me.CombatAbilityReady[${_provokeDisc}]}) {
				/echo |- attemptProvoke -| [${_provokeDisc}] is not ready.
			} else {
				/doability "${_provokeDisc}"
				/if (${Spawn[${Me.TargetOfTarget}].CleanName.Equal[${Me}]}) /varset provokeTimer 20
			}
		}
	}
/return

sub chk_taunt
	/if (${Me.AbilityReady[Taunt]}) /doability Taunt
/return 




Sub xbot_initialize_melee
	/echo \ao xbot_initialize_melee
	
	/declare 	int_currentStickDistance		int		outer   30
	/declare	int_startStickDistance 	int 	outer 	40 	| Base stick distance
	/declare 	int_minStickDistance	int		outer 	10	| Minimum stick distance
	/declare 	int_maxStickDistance	int     outer   100 | Maximum attempted stick distance
	/declare    int_maxNavDistance		int		outer   300	| Maximum attempted nav distance

	/declare	timer_getCloser			timer	outer	5s
	/declare 	timer_check_downshits	timer	outer 	0
	/declare 	timer_cantSee			timer   outer   10s
	/declare    timer_cantNav			timer   outer   10s
	/declare 	timer_component			timer   outer   0

	|-- ALL CLASSES COMMON
	|-- [Settings] : Downflags
	/declare	downflagCount			int		outer	${Ini[INI_CHAR,Downflags,downflagCount,0]}
	/declare df int local
		/for df 1 to ${downflagCount} 
			/declare downflag${df} 		string 	outer	${Ini[INI_CHAR,Downflags,downflag${df}]}
		/next df
	
	|-- [Settings] : Holyflags
	/declare	holyflagCount			int		outer	${Ini[INI_CHAR,Holyflags,holyflagCount,0]}
	/declare hf int local
		/for hf 1 to ${holyflagCount} 
			/declare holyflag${hf} 		string 	outer	${Ini[INI_CHAR,Holyflags,holyflag${hf}]}
		/next hf


	|-- MELEE AND HYBRID CLASSES ONLY
	|- [Settings] : Components
	/declare	int_componentTotal		int		outer	${Ini[INI_CHAR,Components,int_componentTotal,0]}
	/declare 	bln_componentCheck		bool	outer   ${If[${int_componentTotal}>0,TRUE,FALSE]}
		/declare comp int local
		/for comp 1 to ${int_componentTotal} 
			/declare COMPONENT_${comp} 			string 	outer ${Ini[INI_CHAR,Components,COMPONENT_${comp}]}
			/declare COMPONENT_${comp}_Check	bool 	outer ${Ini[INI_CHAR,Components,COMPONENT_${comp}_Check,FALSE]}
			/declare COMPONENT_${comp}_Count 	int 	outer ${Ini[INI_CHAR,Components,COMPONENT_${comp}_Count]}
			/declare COMPONENT_${comp}_Type  	string 	outer ${Ini[INI_CHAR,Components,COMPONENT_${comp}_Type]}
			/declare COMPONENT_${comp}_Summoner string	outer ${Ini[INI_CHAR,Components,COMPONENT_${comp}_Summoner]}
			/declare COMPONENT_${comp}_Reagent 	string	outer ${Ini[INI_CHAR,Components,COMPONENT_${comp}_Reagent]}
			|/echo \a-w${COMPONENT_${comp}} ${COMPONENT_${comp}_Check} ${COMPONENT_${comp}_Count} ${COMPONENT_${comp}_Type} ${COMPONENT_${comp}_Summoner} ${COMPONENT_${comp}_Reagent}
		/next comp 
	|/echo \awComponents:\ag${int_componentTotal} ${bln_componentCheck}
	|/call chkComponentsAll

	|--[Settings] : Spell & Support Definitions
	/declare item_pull				string	outer	${Ini[INI_CHAR,Puller,item_pull,"Shuriken of Eternity"]}
	/declare item_pull_clean		string	outer	${Ini[INI_CHAR,Puller,item_pull_clean,Shuriken of Eternity]}
	/declare item_summon			string	outer	${Ini[INI_CHAR,Puller,item_summon,"Ton Po's Mystical Pouch"]}
	
	/declare spell_pull				string	outer	${Ini[INI_CHAR,Puller,spell_pull,"Terror of Discord"]}
	/declare spell_pullGem			string 	outer	${Ini[INI_CHAR,Puller,spell_pullGem,"gem2"]}
	/declare spell_hate				string	outer	${Ini[INI_CHAR,Puller,spell_hate,"Terror of Discord"]}
	/declare spell_hateGem			string  outer   ${Ini[INI_CHAR,Puller,spell_hateGem,"gem2"]}
	/declare spell_aehate			string	outer	${Ini[INI_CHAR,Puller,spell_aehate,"Dread Gaze"]}
	/declare spell_aehateGem		string	outer	${Ini[INI_CHAR,Puller,spell_aehate,"gem3"]}
	/declare spell_snare			string	outer	${Ini[INI_CHAR,Puller,spell_snare,"Festering Darkness"]}
	/declare bln_useSnare			bool	outer	${Ini[INI_CHAR,Puller,bln_useSnare,FALSE]}
	/declare war_ca_hate_1			string	outer	${Ini[INI_CHAR,Puller,war_ca_hate_1,Bazu Bellow]}
	/declare war_ca_hate_2			string	outer 	${Ini[INI_CHAR,Puller,war_ca_hate_2,Bellow of the Mastruq]}
	/declare int_castDistance		int		outer	${Ini[INI_CHAR,Puller,int_castDistance,120]}

	|-- [Settings] : Melee Defaults
	/declare 	MinStrikeEndurance 		int		outer	200
	/declare 	MinBellowEndurance		int		outer	200
	/declare    provokeTimer            timer   outer   0
	/declare 	stickCmd				string	outer   ${Ini[INI_CHAR,Sticking,stickCmd,snaproll rear]}
	/declare 	bln_chkComponents		bool	outer	TRUE
	/declare 	bln_tgtDeadSoon			bool	outer	FALSE
	/declare	timer_reAssist			timer	outer	5s
	/declare    bln_riposting			bool    outer   FALSE

	
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

	| Initialize Library Events
	/call InitEvents_Melee
/return
