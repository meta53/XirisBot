|**
----------------------------
xiris_burn.inc
----------------------------
Burn Mode subs. AA/CLICK/BURN items/AA/disc handlers
______________________________________________
REVISION HISTORY
	12.27.22	xiris	INITIAL REVISION - Moved from xiris_common.inc
    12.31.22    xiris   Moved AA, Clicky, Burns to this file, and changed their initialization
                        Requires ini file changes in BURN/CLICKY/AA
                        Changed Bards medley calls to use new method in spell_routines.inc

**|    

| Burninating
#EVENT BurnOnALL			"#*#BurnOnAll#*#"
#EVENT BurnOn1				"#*#BurnOn1#*#"
#EVENT BurnOn2				"#*#BurnOn2#*#"
#EVENT BurnOff				"#*#BurnOff#*#"
#EVENT ShortBurnOn1 		"#*#ShortBurn1#*#"
#EVENT ShortBurnOn2 		"#*#ShortBurn2#*#"

#EVENT LongBurnOn1			"#*#LongBurn1#*#"
#EVENT LongBurnOn2 			"#*#LongBurn2#*#"

#EVENT MaxiumumBurn			"#*#MaximumBurn#*#"
#EVENT ReportBurnKey        "#1#ReportBurnKey #2#"

Sub InitEvents_Burn
    /call RegisterRaidEvents Events_Burn 5
	/call RegisterBackgroundEvents Events_Burn_Background 2
/return 

Sub Events_Burn
    /doevents BurnOnALL
	/doevents BurnOn1
	/doevents BurnOn2


/return 

Sub Events_Burn_Background
	/doevents BurnOff
	/doevents ReportBurnKey
/return 


|-------------------------------------------------------------------------------- 
|SUB: Event_BURNINATE!
|--------------------------------------------------------------------------------
Sub EVENT_BurnOn
	/if (!${bln_engaged}) /return
	/echo \agEnabling \arBurn \agMode
	/varset bln_burning TRUE
	/varset bln_checkForCry FALSE
/return

Sub EVENT_BurnOnAll
    /echo \ao Burn on All triggered
	/if (!${bln_engaged}) /return
	/echo \agEnabling \arBurn ALL \agMode
	/varset bln_burning TRUE
	/varset bln_checkForCry FALSE
/return

Sub EVENT_BurnOn1
	/if (!${bln_engaged}) /return
	/if (${BURN_Group}==1) {
		/echo \agEnabling \arBurn \agMode on CoHort \ao1
		/varset bln_burning TRUE
	}
	/varset bln_checkForCry TRUE
/return

Sub EVENT_BurnOn2
	/if (!${bln_engaged}) /return
	/if (${BURN_Group}==2) {
		/echo \agEnabling \arBurn \agMode on CoHort \ao2
		/varset bln_burning TRUE
	}
	/varset bln_checkForCry FALSE
/return

Sub EVENT_BurnOff
	/echo \agDisablig \ayBurn \agMode
	/varset bln_burning FALSE
	/varset bln_checkForCry FALSE
	|/call chkCryActive
/return

|**
 ----------------------------
 Check BURN routine
 ----------------------------
 Fires the burn setup if an event has set bln_burning TRUE
 This is a little awkward due to the mix of combat abilities and alt abilities
 I made this verbose for clarity
 ----------------------------
**|

Sub chkBurn
	/if (!${bln_engaged} || !${bln_burning} || (${BURN_Total} == 0)) /return
	/if (!${cache_targetID} || ${cache_targetType.NotEqual[NPC]}) /return
	/if (${Me.Casting.ID} || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return



	/declare i int local
	/for i 1 to ${BURN_Total} 
		/if (${cache_targetID} && ${cache_targetType.Equal[NPC]}) {
			
			| ---------------
			| Combat Ability
			| Note: Run the DISC if it is a combat type, it is ready, and we aren't already discing
			| ---------------
			/if (${BURN_${i}_Type.Equal[combat]} && ${Me.CombatAbilityReady[${BURN_${i}}]} && ${BURN_${i}_Use} && !${Me.ActiveDisc.ID}) {
				/echo \awBurn_\ag${i} \ao${BURN_${i}} \awType:\ag${BURN_${i}_Type} \awUse:\ag${BURN_${i}_Use} \awAfter:\ag${BURN_${i}_AfterIndex} \awMGB:\ag${BURN_${i}_MGB} 
				/echo \aoBurn is combat type, trying ${BURN_${i}}
				| Check to see if the burn Before disc/alt (0 if not) is already run
				|/if (${BURN_${i}_AfterIndex} > 0) {
				|	| Check to see if the burn Before is an alt or combatdisc (two different ways of checking)
				|	/if (${Burn_${BURN_${i}_AfterIndex}_Type.Equal[combat]} && !${Me.CombatAbilityReady[${Burn_${BURN_${i}_AfterIndex}}]}) {
				|		/goto :doCombatAbility
				|	} else /if (${Burn_${BURN_${i}_AfterIndex}_Type.Equal[alt]} && !${Me.AltAbilityReady[${Burn_${BURN_${i}_AfterIndex}}]}) {
				|		/goto :doCombatAbility
				|	}
				|} else {
					:doCombatAbility
					| Bards can click discs while singing
					/doability "${BURN_${i}}"
					/delay 5
					/dgt DISC ${BURN_${i}} clicked!
					/echo \agDISC \ay${BURN_${i}} \agclicked!
					/goto :nextBurn		
				|}
				
			
			| ---------------	
			| Alt Ability
			| Note: Additionally, if MGBable, (few) we have Burn_1_MGB = TRUE to look for (default FALSE)
			| ---------------
			} else /if (${BURN_${i}_Type.Equal[alt]} && ${Me.AltAbilityReady[${BURN_${i}}]} && ${BURN_${i}_Use} && ${Spell[${BURN_${i}}].Stacks}) {
				| Check to see if the burn Before disc/alt (0 if not) is already run
				/if (${BURN_${i}_AfterIndex} > 0) {
					| Check to see if the burn Before is an alt or combatdisc  (two different ways of checking)
					/if (${Burn_${BURN_${i}_AfterIndex}_Type.Equal[combat]} && !${Me.CombatAbilityReady[${Burn_${BURN_${i}_AfterIndex}}]}) {
						/goto :doAltAbility
					} else /if (${Burn_${BURN_${i}_AfterIndex}_Type.Equal[alt]} && !${Me.AltAbilityReady[${Burn_${BURN_${i}_AfterIndex}}]}) {
						/goto :doAltAbility
					}
				} else {
					/echo \awBurn_\ag${i} \ao${BURN_${i}} \awType:\ag${BURN_${i}_Type} \awUse:\ag${BURN_${i}_Use} \awAfter:\ag${BURN_${i}_AfterIndex} \awMGB:\ag${BURN_${i}_MGB} 
					/echo \aoBurn is altability type, trying ${BURN_${i}}
					:doAltAbility
						/if (${BURN_${i}_MGB}) {
							/call MQ2Cast "Mass Group Buff" alt
							/delay 5
							/echo \ag Mass Group Buff Activated!
						}
						/if (${Me.Class.ShortName.Equal[BRD]}) {
                            /call MQ2Medley ALT "${BURN_${i}}" ${int_targetID} TRUE
						} else {
							/call MQ2Cast "${BURN_${i}}" alt
						}
						/delay 5
						/dgt AA ${BURN_${i}} clicked!
						/echo \agAA \ay${BURN_${i}} \agclicked!
						/goto :nextBurn	
				}
			| ---------------	
			| Item
			| Check to see if  burn == item &&  useable && (item in inventory && there is no timer) && (I am not Casting || I am a bard)
			| ---------------
			} else /if (${BURN_${i}_Type.Equal[item]} && ${BURN_${i}_Use} && (${FindItem[${BURN_${i}}].ID} && ${FindItem[=${BURN_${i}}].TimerReady} == 0) && (!${Me.Casting.ID} || ${Me.Class.ShortName.Equal[BRD]})) {
				/if (${Spell[${FindItem[${BURN_${i}}].Clicky}].SpellType.Equal[BENEFICIAL]} && !${Me.FreeBuffSlots}) /goto :nextBurn
				/echo \awBurn_\ag${i} \ao${BURN_${i}} \awType:\ag${BURN_${i}_Type} \awUse:\ag${BURN_${i}_Use} \awAfter:\ag${BURN_${i}_AfterIndex} \awMGB:\ag${BURN_${i}_MGB} 
				/echo \aoBurn is item type, trying ${BURN_${i}}
				:doItemClick
					
					/if (${Me.Class.ShortName.Equal[BRD]} ) {
                        /call MQ2Medley ITEM "${BURN_${i}}" ${int_targetID} TRUE
						/echo \agITEM \ay${BURN_${i}} \agclicked!
					} else {
						/call MQ2Cast "${BURN_${i}}" item 2s
						/if (${Macro.Return.NotEqual[CAST_SUCCESS]}) {
							/echo \aoITEM \ay${BURN_${i}} \aofailed! \ar${Macro.Return}	
						} else {
							/dgt Item ${BURN_${i}} clicked!
							/echo \agITEM \ay${BURN_${i}} \agclicked!
						}
					}
					/delay 5
				/goto :nextBurn	
			}
		}
	:nextBurn
	/call RunRaidEvents
	/call RunRapidEvents	
	/next i
/return


| When the toon has warcry on, then we need to make sure
| we are using 2handers on MNK/RNG/SHD/PAL
|**
[WEAPS]
	default=Flayed Flesh Handwraps|mainhand,Fangs of the Serpent|offhand
	blunt=Mindreaper Club|mainhand,Death's Head Mace|offhand
	h2h=Flayed Flesh Handwraps|mainhand,Fangs of the Serpent|offhand
	2hander=Bone Staff of Wickedness|mainhand
**|
Sub chkCryActive
	/if (!${Select[${Me.Class.ShortName},MNK,RNG,SHD,PAL]} || ${timer_cryCheck} || !${bln_checkForCry}) /return
	/echo \aoChecking for Cry! \ag${Me.Song[Ancient: Cry of Chaos].ID} \ao2H Equipped? \ay${bln_cry2Hander}
	/if (${Me.Song[Ancient: Cry of Chaos].ID} && !${bln_cry2Hander}) {
		/call LoadWeaponset 2HANDER
		/varset bln_cry2Hander TRUE
	} else /if (!${Me.Song[Ancient: Cry of Chaos].ID} && ${bln_cry2Hander}) {
		/call LoadWeaponset DEFAULT
		/varset bln_cry2Hander FALSE
	}
	/varset timer_cryCheck 4s
/return 


Sub AddBurnToLineup(string _type, int _index, string _line)
    |/echo \arAdding Burn to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
    /declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

    | Add the new Burn by type 
	|_Burn|_Type|_Use|_AfterIndex|_MGB
	/echo \ayCreating New \ag ${_type} \ay: ${_type}_${_index}: \ag ${_line.Token[1,|]}
    /declare ${_type}_${_index} string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
			|/echo \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
            |/echo \ay${property}:\ag ${_line.Token[${j},|]}
		}
		
	/next j

/return

Sub chkAA
	/if (${AA_Total} == 0) 	/return 
	/if (!${cache_targetID} || ${cache_targetType.NotEqual[NPC]}) /return
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	
    | Check AA Type, if offensive we do the standard targetchecks
    | if defensive we check ourselves

    /declare i int local 1
    /call TargetNamedCheck ${int_targetID}
    /declare isNamed bool local ${Macro.Return}
    /for i 1 to ${AA_Total}
        /if (${Me.AltAbilityReady[${AA_${i}}]}) {
            | If Offense, make sure we are engaged | If Defense, doesn't matter
            /if ((${AA_${i}_Type.Equal[OFFENSE]} && ${bln_engaged}) || ${AA_${i}_Type.Equal[DEFENSE]} ) {
                 | HP Check
                /if (${AA_${i}_Type.Equal[OFFENSE]} && (${Target.PctHPs} > ${AA_${i}_StartHP} || ${Target.PctHPs} < ${AA_${i}_StopHP})) /goto :nextAA
                | Offense named-only check
                /if (${AA_${i}_Type.Equal[OFFENSE]} && !${isNamed} && ${AA_${i}_NamedOnly}) /goto :nextAA
                | Trigger Check
                /if (${Bool[${AA_${i}_Trigger.Replace[#,$]}]}) /goto :nextAA
            }

        } else {
			/goto :nextAA
		}
           
        :castingAA
        /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (${AA_${i}_Type.Equal[DEFENSE]} && ${Me.FreeBuffSlots}) {
                /call MQ2Medley ALT "${AA_${i}}" ${Me.ID} TRUE
            } else {
                /call MQ2Medley ALT "${AA_${i}}" ${int_targetID} TRUE
            }
        } else {                
            /if (${AA_${i}_Type.Equal[DEFENSE]} && ${Me.FreeBuffSlots}) {
                /target id ${Me.ID}
            } else {
                /if (${cache_targetID} != ${int_targetID}) /call TrueTarget ${int_targetID}
            }
            /call MQ2Cast "${AA_${i}}" alt
        }
        

    :nextAA
	/call RunRaidEvents
	/call RunRapidEvents	
    /next i
/return

Sub AddAAToLineup(string _type, int _index, string _line)
    |/echo \arAdding AA to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
    /declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

    /echo \ayCreating New \ag ${_type} \ay: ${_type}_${_index}: \ag${_line.Token[1,|]}
    /declare ${_type}_${_index} string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
		|	|/echo \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
            |/echo \ay${property}:\ag ${_line.Token[${j},|]}
		}
		
	/next j

/return 

Sub chkClicky
	/if (${CLICKY_Total} == 0) 	/return 
	/if (!${cache_targetID} || ${cache_targetType.NotEqual[NPC]}) /return
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	

    | Check AA Type, if offensive we do the standard targetchecks
    | if defensive we check ourselves

    /declare i int local 1
    /call TargetNamedCheck ${int_targetID}
    /declare isNamed bool local ${Macro.Return}
    /for i 1 to ${CLICKY_Total}
		| If the timer isn't ready, skip
		/if (${FindItem[${CLICKY_${i}}].TimerReady} > 0) /goto :nextClicky
		
		| If Offense, make sure we are engaged | If Defense, doesn't matter
		/if ((${CLICKY_${i}_Type.Equal[OFFENSE]} && ${bln_engaged}) || ${CLICKY_${i}_Type.Equal[DEFENSE]} ) {
			| Offense named-only check
			/if (${CLICKY_${i}_Type.Equal[OFFENSE]} && !${isNamed} && ${CLICKY_${i}_NamedOnly}) /goto :nextClicky
			| Trigger Check
			/if (${Bool[${CLICKY_${i}_Trigger.Replace[#,$]}]}) /goto :nextClicky
			/goto :castingClicky
		}
        
        | Passed the above checks, cast the clicky   
        :castingClicky
        /echo \ag ${CLICKY_${i}} : FIRED	
        /if (${Me.Class.ShortName.Equal[BRD]}) {
            | We need to make sure BARDs (with medley) have the item in a worn slot or slot 30
            /if (${CLICKY_${i}_Type.Equal[DEFENSE]}) {
                /call MQ2Medley ITEM "${CLICKY_${i}}" ${Me.ID} TRUE
            } else {
                /call MQ2Medley ITEM "${CLICKY_${i}}" ${int_targetID} TRUE
            }
        } else {                
            /if (${CLICKY_${i}_Type.Equal[DEFENSE]}) {
                /target id ${Me.ID}
            } else {
                /if (${cache_targetID} != ${int_targetID}) /call TrueTarget ${int_targetID}
            }
            /call MQ2Cast "${CLICKY_${i}}" item
        }
        

    :nextClicky
	/call RunRaidEvents
	/call RunRapidEvents	
    /next i
/return

Sub AddClickyToLineup(string _type, int _index, string _line)
    |/echo \arAdding Clicky to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
     /declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

    /echo \ayCreating New \ag ${_type} \ay: ${_type}_${_index}:\ag${_line.Token[1,|]}
    /declare ${_type}_${_index} string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
		|	|/echo \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
            |/echo \ay${property}:\ag ${_line.Token[${j},|]}
		}
		
	/next j
/return


Sub CreateBurnLineup
    |   Burn_Index=_Burn|_Type|_Use|_AfterIndex|_MGB
    |	BURN_1="Ancestral Aid"|alt|TRUE|0|FALSE

    /declare BURN_Total             int     outer   ${Ini[INI_CHAR,BURN,BURN_Total,0]}
	/declare BURN_Group             int     outer   ${Ini[INI_CHAR,BURN,BURN_Group,2]}


    /declare i int local
    /if (${BURN_Total}) {
        /for i 1 to ${BURN_Total}
            /call AddBurnToLineup BURN ${i} ${Ini[INI_CHAR,BURN,BURN_${i}]}
        /next i
    }
/return

Sub CreateAALineup
    |_AA|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger
	|AA_1="Assault Rk. II"|OFFENSE|TRUE|FALSE|97|15|#{Me.PctHps}>0

    /declare AA_Total               int		outer	${Ini[INI_CHAR,AA,AA_Total,0]}

    /declare i int local
    /if (${AA_Total}) {
        /for i 1 to ${AA_Total}
            /call AddAAToLineup AA ${i} ${Ini[INI_CHAR,AA,AA_${i}]}
        /next i
    }

/return 

Sub CreateClickyLineup
	|_Item|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger
	|CLICKY_1="Nightshade, Blade of Entropy"|OFFENSE|TRUE|FALSE|97|15|#{Me.PctHps}>0

    /declare CLICKY_Total		    int		outer	${Ini[INI_CHAR,CLICKY,CLICKY_Total,0]}

    /declare i int local
    /if (${CLICKY_Total}) {
        /for i 1 to ${CLICKY_Total}
            /call AddClickyToLineup CLICKY ${i} ${Ini[INI_CHAR,CLICKY,CLICKY_${i}]}
        /next i
    }
/return

Sub xbot_initialize_burns
    /echo \aoxbot_initialize_burns
    |- [DEFAULT_SETTINGS] : BurnMode
	/declare bln_burning			bool	outer FALSE
	/declare bln_checkForCry		bool    outer FALSE
	/declare bln_cry2Hander			bool  	outer FALSE
	/declare timer_cryCheck			timer   outer 0

    | Template
    /declare BURN_Template          string  outer _Burn|_Type|_Use|_AfterIndex|_MGB
    /declare AA_Template            string  outer _AA|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger
    /declare CLICKY_Template        string  outer _Item|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger

    | Load the burns set in the INI file
    /call CreateBurnLineup
    /call CreateAALineup
    /call CreateClickyLineup

	| Initialize Library Events
	/call InitEvents_Burn
/return

Sub Event_ReportBurnKey(_line, _sender, _eventParams)
	/declare _burnKey int local ${_eventParams.Arg[1,|]}
	/echo ${_sender} checking burnKey:${_burnKey}
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} && ${BURN_Group} == ${_burnKey}) {
		/docommand /${announceChannel} ${_burnKey} ${Me.CleanName} ${Me.Class.ShortName}
	}
/return