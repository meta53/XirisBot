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
    /call RegisterBackgroundEvents Events_Burn_Background 1
    /call RegisterRaidEvents Events_Burn 4
/return 

Sub Events_Burn 
    /doevents BurnOnALL
	/doevents BurnOn1
	/doevents BurnOn2
	/doevents BurnOff
/return 

Sub Events_Burn_Background 
    /doevents ReportBurnKey
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
	/if (!${bln_engaged} || !${bln_burning} || (${BURN_Total} < 1)) /return
	/if (!${Target.ID} || ${Target.Type.NotEqual[NPC]}) /return
	/if (${Me.Casting.ID} || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return

	/declare i int local
	|/echo BurnMode Activated! Total Burns: ${BURN_Total}
	/for i 1 to ${BURN_Total} 
		
		/if (${Target.ID} && ${Target.Type.Equal[NPC]}) {
			| ---------------
			| Combat Ability
			| Note: Run the DISC if it is a combat type, it is ready, and we aren't already discing
			| ---------------
			/if (${Burn_${i}_Type.Equal[combat]} && ${Me.CombatAbilityReady[${Burn_${i}}]} && ${Burn_${i}_Use} && !${Bool[${Melee.DiscID}]}) {
				/cecho \aoBurn is combat type, trying ${Burn_${i}}
				| Check to see if the burn Before disc/alt (0 if not) is already run
				/if (${Burn_${i}_After} > 0) {
					| Check to see if the burn Before is an alt or combatdisc (two different ways of checking)
					/if (${Burn_${Burn_${i}_After}_Type.Equal[combat]} && !${Me.CombatAbilityReady[${Burn_${Burn_${i}_After}}]}) {
						/goto :doCombatAbility
					} else /if (${Burn_${Burn_${i}_After}_Type.Equal[alt]} && !${Me.AltAbilityReady[${Burn_${Burn_${i}_After}}]}) {
						/goto :doCombatAbility
					}
				} else {
					:doCombatAbility
					| Bards can click discs while singing
					/doability "${Burn_${i}}"
					/dgt DISC ${Burn_${i}} clicked!
					/cecho \agDISC \ay${Burn_${i}} \agclicked!
					/goto :nextBurn		
				}
				
			
			| ---------------	
			| Alt Ability
			| Note: Additionally, if MGBable, (few) we have Burn_1_MGB = TRUE to look for (default FALSE)
			| ---------------
			} else /if (${Burn_${i}_Type.Equal[alt]} && ${Me.AltAbilityReady[${Burn_${i}}]} && ${Burn_${i}_Use} && ${Spell[${Burn_${i}}].Stacks}) {
				| Check to see if the burn Before disc/alt (0 if not) is already run
				/if (${Burn_${i}_After} > 0) {
					| Check to see if the burn Before is an alt or combatdisc  (two different ways of checking)
					/if (${Burn_${Burn_${i}_After}_Type.Equal[combat]} && !${Me.CombatAbilityReady[${Burn_${Burn_${i}_After}}]}) {
						/goto :doAltAbility
					} else /if (${Burn_${Burn_${i}_After}_Type.Equal[alt]} && !${Me.AltAbilityReady[${Burn_${Burn_${i}_After}}]}) {
						/goto :doAltAbility
					}
				} else {
					:doAltAbility
						/if (${Burn_${i}_MGB}) {
							/call MQ2Cast "Mass Group Buff" alt
							/delay 5
							/cecho \ag Mass Group Buff Activated!
						}
						/if (${Me.Class.ShortName.Equal[BRD]}) {
                            /call MQ2Medley ALT "${Burn_${i}}" ${int_targetID} FALSE
						} else {
							/call MQ2Cast "${Burn_${i}}" alt
						}
						/dgt AA ${Burn_${i}} clicked!
						/cecho \agAA \ay${Burn_${i}} \agclicked!
						/goto :nextBurn	
				}
			| ---------------	
			| Item
			| Check to see if  burn == item &&  useable && (item in inventory && there is no timer) && (I am not Casting || I am a bard)
			| ---------------
			} else /if (${Burn_${i}_Type.Equal[item]} && ${Burn_${i}_Use} && (${Bool[${FindItem[${Burn_${i}}].ID}]} && ${FindItem[${Burn_${i}}].TimerReady} == 0) && (!${Me.Casting.ID} || ${Me.Class.ShortName.Equal[BRD]})) {
				:doItemClick
					/if (${Me.Class.ShortName.Equal[BRD]}) {
                        /call MQ2Medley ITEM "${Burn_${i}}" ${int_targetID} FALSE
					} else {
						/call MQ2Cast "${Burn_${i}}" item 2s
					}
					/dgt Item ${Burn_${i}} clicked!
					/cecho \agItem \ay${Burn_${i}} \agclicked!
					/goto :nextBurn	
			}
		}
	:nextBurn
	/next burn
/return


Sub AddBurnToLineup(string _type, int _index, string _line)
    |/cecho \arAdding Burn to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
    /declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

    | Add the new Burn by type 
	|_Burn|_Type|_Use|_AfterIndex|_MGB
	/cecho \ayCreating New \ag ${_type} \ay: ${_type}_${_index}: \ag ${_line.Token[1,|]}
    /declare ${_type}_${_index} string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
		|	|/cecho \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
            |/cecho \ay${property}:\ag ${_line.Token[${j},|]}
		}
		
	/next j

/return

Sub chkAA
	/if (${AA_Total} == 0) 	/return 
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	

    | Check AA Type, if offensive we do the standard targetchecks
    | if defensive we check ourselves

    /declare i int local 1
    /call TargetNamedCheck ${int_targetID}
    /declare isNamed bool local ${Macro.Return}
    /for i 1 to ${AA_Total}
        /if (${${Me.AltAbilityReady[${AA_${i}}]}}) {
            | If Offense, make sure we are engaged | If Defense, doesn't matter
            /if ((${AA_${i}_Type.Equal[OFFENSE]} && ${bln_engaged}) || ${AA_${i}_Type.Equal[DEFENSE]} ) {
                 | HP Check
                /if (${AA_${i}_Type.Equal[OFFENSE]} && (${Target.PctHPs} > ${AA_${i}_StartHP} || ${Target.PctHPs} < ${AA_${i}_StopHP})) /goto :nextAA
                | Offense named-only check
                /if (${AA_${i}_Type.Equal[OFFENSE]} && !${isNamed} && ${AA_${i}_NamedOnly}) /goto :nextAA
                | Trigger Check
                /if (${Bool[${AA_${i}_Trigger.Replace[#,$]}]}) /goto :nextAA
            }

        }
           
        :castingAA
        /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (${AA_${i}_Type.Equal[DEFENSE]}) {
                /call MQ2Medley ALT "${AA_${i}}" ${Me.ID} FALSE
            } else {
                /call MQ2Medley ALT "${AA_${i}}" ${int_targetID} FALSE
            }
        } else {                
            /if (${AA_${i}_Type.Equal[DEFENSE]}) {
                /target id ${Me.ID}
            } else {
                /if (${Target.ID} != ${int_targetID}) /call TrueTarget ${int_targetID}
            }
            /call MQ2Cast "${AA_${i}}" alt
        }
        

    :nextAA
    /next i
/return

Sub AddAAToLineup(string _type, int _index, string _line)
    |/cecho \arAdding AA to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
    /declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

    /cecho \ayCreating New \ag ${_type} \ay: ${_type}_${_index}: \ag${_line.Token[1,|]}
    /declare ${_type}_${_index} string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
		|	|/cecho \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
            |/cecho \ay${property}:\ag ${_line.Token[${j},|]}
		}
		
	/next j

/return 

Sub chkClicky
	/if (${CLICKY_Total} == 0) 	/return 
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${Me.Feigning}) /return
	

    | Check AA Type, if offensive we do the standard targetchecks
    | if defensive we check ourselves

    /declare i int local 1
    /call TargetNamedCheck ${int_targetID}
    /declare isNamed bool local ${Macro.Return}
    /for i 1 to ${CLICKY_Total}
        /if (${${Me.AltAbilityReady[${CLICKY_${i}}]}}) {
            | If Offense, make sure we are engaged | If Defense, doesn't matter
            /if ((${CLICKY_${i}_Type.Equal[OFFENSE]} && ${bln_engaged}) || ${CLICKY_${i}_Type.Equal[DEFENSE]} ) {
                | Offense named-only check
                /if (${CLICKY_${i}_Type.Equal[OFFENSE]} && !${isNamed} && ${CLICKY_${i}_NamedOnly}) /goto :nextClicky
                | Trigger Check
                /if (${Bool[${CLICKY_${i}_Trigger.Replace[#,$]}]}) /goto :nextClicky
                /goto :castingClicky
            }
        }
           
        :castingClicky
        /cecho \ag ${CLICKY_${aa}} : FIRED	
        /if (${Me.Class.ShortName.Equal[BRD]}) {
            | We need to make sure BARDs (with medley) have the item in a worn slot or slot 30
            /if (${CLICKY_${i}_Type.Equal[DEFENSE]}) {
                /call MQ2Medley ITEM "${CLICKY_${i}}" ${Me.ID} FALSE
            } else {
                /call MQ2Medley ITEM "${CLICKY_${i}}" ${int_targetID} FALSE
            }
        } else {                
            /if (${CLICKY_${i}_Type.Equal[DEFENSE]}) {
                /target id ${Me.ID}
            } else {
                /if (${Target.ID} != ${int_targetID}) /call TrueTarget ${int_targetID}
            }
            /call MQ2Cast "${CLICKY_${i}}" item
        }
        

    :nextClicky
    /next i
/return

Sub AddClickyToLineup(string _type, int _index, string _line)
    |/cecho \arAdding Clicky to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
     /declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

    /cecho \ayCreating New \ag ${_type} \ay: ${_type}_${_index}:\ag${_line.Token[1,|]}
    /declare ${_type}_${_index} string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
		|	|/cecho \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
            |/cecho \ay${property}:\ag ${_line.Token[${j},|]}
		}
		
	/next j
/return


Sub CreateBurnLineup
    |   Burn_Index=_Burn|_Type|_Use|_AfterIndex|_MGB
    |	BURN_1="Ancestral Aid"|alt|TRUE|0|FALSE

    /declare BURN_Total             int     outer   ${Ini[botINI,BURN,BURN_Total,0]}
	/declare BURN_Group             int     outer   ${Ini[botINI,BURN,BURN_Group,ALL]}


    /declare i int local
    /if (${Bool[${BURN_Total}]}) {
        /for i 1 to ${BURN_Total}
            /call AddBurnToLineup BURN ${i} ${Ini[botINI,BURN,BURN_${i}]}
        /next i
    }
/return

Sub CreateAALineup
    |_AA|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger
	|AA_1="Assault Rk. II"|OFFENSE|TRUE|FALSE|97|15|#{Me.PctHps}>0

    /declare AA_Total               int		outer	${Ini[botINI,AA,AA_Total,0]}

    /declare i int local
    /if (${Bool[${AA_Total}]}) {
        /for i 1 to ${AA_Total}
            /call AddAAToLineup AA ${i} ${Ini[botINI,AA,AA_${i}]}
        /next i
    }

/return 

Sub CreateClickyLineup
	|_Item|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger
	|CLICKY_1="Nightshade, Blade of Entropy"|OFFENSE|TRUE|FALSE|97|15|#{Me.PctHps}>0

    /declare CLICKY_Total		    int		outer	${Ini[botINI,CLICKY,CLICKY_Total,0]}

    /declare i int local
    /if (${Bool[${CLICKY_Total}]}) {
        /for i 1 to ${CLICKY_Total}
            /call AddClickyToLineup CLICKY ${i} ${Ini[botINI,CLICKY,CLICKY_${i}]}
        /next i
    }
/return

Sub xbot_initialize_burns
    /cecho \aoxbot_initialize_burns
    |- [Settings] : BurnMode
	/declare bln_burning			bool	outer FALSE

    | Template
    /declare BURN_Template          string  outer _Burn|_Type|_Use|_AfterIndex|_MGB
    /declare AA_Template            string  outer _AA|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger
    /declare CLICKY_Template        string  outer _Item|_Type|_Use|_NamedOnly|_StartHP|_StopHP|_Trigger

    | Load the burns set in the INI file
    /call CreateBurnLineup
    /call CreateAALineup
    /call CreateClickyLineup
	
/return

