|**
----------------------------
HEAL CASTING HANDLERS
----------------------------
This includes the healing setup (INI READ)
and sub routines as necessary
______________________________________________
REVISION HISTORY
    03.24.16    xiris	INITIAL REVISION - Consolidation of several macro embedded routines
	10.15.16	xiris	Cleaned up (removed CH) and added group healing event
	01.13.22	xiris	Added Rez routines
	04.25.22	xiris 	Added healing potion routines
	05.08.22	xiris	Integrated AERez.mac
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
						Updated the rez portion to work with new dannet handlers
**|



|-- Settings and Triggers
#event GroupHeal  	"#*#DoGroupHeal#*#"
#EVENT InterruptON 	"#*#InterruptON#*#"
#EVENT InterruptOFF "#*#InterruptOFF#*#"

#EVENT ChangeHP	"#1# tells you#*#'ChangeHP #2#'"
#EVENT ChangeHP	"#1# tells the group#*#'ChangeHP #2#'"
#EVENT ChangeHP	"#1# tells the raid#*#'ChangeHP #2#'"
#EVENT ChangeHP "[ #1# #*#]#*#ChangeHP #2#"

#EVENT HealMode	"#1# tells you#*#'HealMode #2#'"
#EVENT HealMode	"#1# tells the group#*#'HealMode #2#'"
#EVENT HealMode	"#1# tells the raid#*#'HealMode #2#'"
#EVENT HealMode "[ #1# #*#]#*#HealMode #2#"

|-- Rez Events
#event CanRez     	"This corpse can be resurrected."
#event CannotRez  	"This corpse cannot be resurrected."
#event NotOnline  	"You told #1#, '#2# is not online at this time'"
#event NotOnline  	"Your tell to #1# has been queued"

|-- CHEvents should only be listened for in the class handlers (cleric, druid)
#EVENT CHStart	"#1# tells you#*#'CHStart #2#'"
#EVENT CHStart	"#1# tells the group#*#'CHStart #2#'"
#EVENT CHStart	"#1# tells the raid#*#'CHStart #2#'"
#EVENT CHStart	"[ #1# #*#]#*#CHStart #2#"

#EVENT CHStop	"#1# tells you#*#'CHStop #2#'"
#EVENT CHStop	"#1# tells the group#*#'CHStop #2#'"
#EVENT CHStop	"#1# tells the raid#*#'CHStop #2#'"
#EVENT CHStop	"[ #1# #*#]#*#CHStop #2#"

#EVENT CHPause	"#1# tells you#*#'CHPause #2#'"
#EVENT CHPause	"#1# tells the group#*#'CHPause #2#'"
#EVENT CHPause	"#1# tells the raid#*#'CHPause #2#'"
#EVENT CHPause	"[ #1# #*#]#*#CHPause #2#"

#EVENT CHResume	"#1# tells you#*#'CHResume#*#'"
#EVENT CHResume	"#1# tells the group#*#'CHResume#*#'"
#EVENT CHResume	"#1# tells the raid#*#'CHResume#*#'"
#EVENT CHResume "[ #1# #*#]#*#CHResume#*#"

#EVENT CHSwitch	"#1# tells you#*#'CHSwitch #2#'"
#EVENT CHSwitch	"#1# tells the group#*#'CHSwitch #2#'"
#EVENT CHSwitch	"#1# tells the raid#*#'CHSwitch #2#'"
#EVENT CHSwitch "[ #1# #*#]#*#CHSwitch #2#"

|-- Celestial Regeneration Events

#EVENT RegenerateCRList "#*# RegenerateCRList#*#"

#EVENT CelRegenStart	"#1# tells you#*#'CRStart#*#'"
#EVENT CelRegenStart	"#1# tells the group#*#'CRStart#*#'"
#EVENT CelRegenStart	"#1# tells the raid#*#'CRStart#*#'"
#EVENT CelRegenStart	"[ #1# #*#]#*#CRStart#*#"

#EVENT CelRegenStop	"#1# tells you#*#'CRStop#*#'"
#EVENT CelRegenStop	"#1# tells the group#*#'CRStop#*#'"
#EVENT CelRegenStop	"#1# tells the raid#*#'CRStop#*#'"
#EVENT CelRegenStop "[ #1# #*#]#*#CRStop#*#"




Sub InitEvents_Healing
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[MELEE_MANA]} ) /call RegisterBackgroundEvents Events_Healing 6
	/if (${str_classArchetype.Equal[PRIEST]}) /call RegisterBackgroundEvents Events_CH 7
/return

Sub Events_Healing
	/doevents GroupHeal
	/doevents CanRez
	/doevents CannotRez
	/doevents NotOnline
	/doevents ChangeHP
	/doevents HealMode
	/doevents InterruptON
	/doevents InterruptOFF
/return

Sub Events_CH 
	/doevents CHStart
	/doevents CHStop
	/doevents CHPause
	/doevents CHResume
	/doevents CHSwitch
	/if (${bln_hasCelRegen}) {
		/doevents CelRegenStart
		/doevents CelRegenStop
		/if (${bln_celRegenActive}) /call chkCelRegen
	} 
/return 


Sub EVENT_GroupHeal
	/echo Blasting Group Heal!
	/call MQ2Cast "${str_heal_group}" gem1 5s
	/doevents flush GroupHeal
/return

Sub EVENT_ChangeHP(string chatText)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo HP (HealPoint) Change called! Changing to: ${chatText.Arg[3]}
		/declare HP int local ${chatText.Arg[3]}
		/varset	int_point_tankNormal ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tankNormal}
	}
	/doevents flush ChangeHP
/return

Sub EVENT_HealMode(_line, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/echo Changing Heal Mode?
	/echo line ${_line}
	/echo eventParams ${_eventParams}
	/call ChangeHealMode ${_eventParams}
	/doevents flush HealMode
/return

|**
 ----------------------------
 HealMode
 ----------------------------
 We need to have several different healing modes based on what encounter we want setup
 Example, Zun`Muram Kvxe Pirik needs a very efficient heal mode from the spam healers for 
 the first 70% of the event, so for clerics that would be Ethereal Lights and CHChain
 this will be handled in the setHealingSpell sub.
 NORMAL|EFFICIENT|CHCHAIN
 ----------------------------
**|
Sub ChangeHealMode(string _strMode)
	|- Mem the appropriate spell set
	/if (${_strMode.Find[CHCHAIN]}) {
		/echo Changing healing mode to DEFAULT
		/varset str_spellset default
		/varset str_healMode CHCHAIN
	} else /if (${_strMode.Find[EFFICIENT]}) {
		/echo Changing healing mode to EFFICIENT
		/varset str_spellset efficient
		/varset use_heal_HoT FALSE
		/varset str_healMode EFFICIENT
	} else {
		/echo Changing healing mode to DEFAULT
		/varset str_spellset default
		/varset use_heal_HoT TRUE
		/varset str_healMode DEFAULT
	}
	/memspellset ${str_spellset}
/return

|**
 ----------------------------
 CELESTIAL REGENERATION EVENT HANDLERS
 ----------------------------
	Celestial Regen chain is nice to have for events with massive, consecutive AEs
	such as Mastery of Specialization, etc. We cannot get the information we need
	directly out of netbots, so we are going to try to write something for MQ2DanNet
**|

Sub setCelRegenChain
	/declare i int local 1
	/declare pPos int local 0
	/declare nbName string local
	/for i 1 to ${int_celRegenTotal}
		| get the Name from the INI
		/varset nbName  ${Ini[${healINI},CelestialRegeneration,cr_${i}_name]}
		| set the array value to the bot Name, and the celregen to READY
		/varset arr_celRegen[${i}] ${nbName}
		/if (${nbName.Equal[${Me.Name}]}) {
			/echo setting my regen position to ${i}
			/varset int_celRegenPosition ${i}
			/varcalc pPos ${int_celRegenPosition} - 1
			/varset str_celRegenPPriest ${arr_celRegen[${pPos}]}
		}
	/next i
/return

Sub EVENT_CelRegenStart
	/varset bln_celRegenActive TRUE
	/call chkCelRegen
	/doevents flush CelRegenStart
/return 

Sub EVENT_CelRegenStop 
	/varset bln_celRegenActive FALSE 
	/doevents flush CelRegenStop
/return 



Sub chkCelRegen
	/if (!${bln_celRegenActive}) /return 
	/if (${timer_celregen}) /return
	| CelRegenChain is active, lets see if we should be firing this.
	| A. Is my Celestial Regen ready?
	|	N --> /goto :return
	|   Y --> /goto :B

	| B. Am I in position #1?
	|	N --> /goto :C
	|   Y --> /goto :FireCelRegen

	| C. Is there a previous priest in zone?
	|	N --> /goto :FireCelRegen
	|	Y --> /goto :D

	| D. The nearest Living Previous Priest has he fired his celregen?
	|	N --> /goto :return
	|	Y --> /goto :FireCelRegen

	| A |
	/if (!${Me.AltAbilityReady[Celestial Regeneration]} && !${Me.AltAbilityReady[Mass Group Buff]}) /goto :return

	| B |
	/if (${int_celRegenPosition} == 1) /goto :FireCelRegen

	| C |
	/call getPreviousLivingPriest
	/declare pp string local ${Macro.Return}
	/if (${pp.Equal[NONE]}) /goto :FireCelRegen

	| D | Determine previous priest cast status
	/declare pMGB string local 
	/declare pCR string local 
	/dquery ${pp} -q "Me.AltAbilityReady[Mass Group Buff]" -o pMGB
	/dquery ${pp} -q "Me.AltAbilityReady[Celestial Regeneration]" -o pCR
	/delay 20 ${DanNet[${pp}].Query["Me.AltAbilityReady[Mass Group Buff]"].Received}
	/if (!${Bool[${pMGB}]} || !${Bool[${pMGB}]}) {
		/cecho \ar${pp}:\arDOES NOT \awhave MGB and Celestial Regen Ready. \agFiring!
		/goto :FireCelRegen
	} else {
		/echo \ag${pp}:\aw \agHAS \awMGB and Celestial Regen Ready. \agWaiting...
		|-- give the correct cleric a moment to fire their stuff
		/varset timer_celregen 5s
		/goto :return
	}
	
	:FireCelRegen
	/rs <FIRING CELESTIAL REGENERATION>
	/call MQ2Cast "Mass Group Buff" alt 2s
	/delay 10
	/call MQ2Cast "Celestial Regeneration" alt 2s
	/varset bln_celRegenActive FALSE
	:return
/return

| Note the xiris_healing.ini contains the list of Celestial Regen capable clerics
| We want to check if anyone previous to my index is alive, and if they are, we will
| use that result to check to see if they have fired CE, and if not, fire it ourselves
Sub getPreviousLivingPriest
	/declare i int local
	/declare pPos	int local 
	/varcalc pPos ${int_celRegenPosition} - 1 
	/declare prevPriest string local NONE
	| decrement through list 
	/for i ${pPos} downto 1 
		/varset prevPriest ${arr_celRegen[${i}]}
		/if (${Spawn[pc ${prevPriest}].Name.Equal[${Me.Name}]}) /goto :nextPosition
		/if (!${SpawnCount[pc ${prevPriest}]})  /goto :nextPosition
		/goto :return
	:nextPosition
	/next i
	
	:return
/return ${prevPriest}


| Utility method to determine which clerics have Celestial Regen and MGB available
Sub listCelestialRegenCapable
	/for i ${int_celRegenTotal} downto 1 
		/varset prevPriest ${arr_celRegen[${i}]}
		/dquery ${prevPriest} -q "Me.AltAbilityReady[Mass Group Buff]" -o pMGB
		/dquery ${prevPriest} -q "Me.AltAbilityReady[Celestial Regeneration]" -o pCR
		/delay 20 ${DanNet[${prevPriest}].Query["Me.AltAbilityReady[Celestial Regeneration]"].Received}
		/if (${pMGB.Equal[FALSE]}) /varset c1 \ar
		/if (${pCR.Equal[FALSE]})  /varset c2 \ar
		/if (${pMGB.Equal[FALSE]} || (${pCR.Equal[FALSE]})) {
			/cecho \awprevPriest[${i}]: \ar${prevPriest} \awMGB:${c1}${pMGB}\aw Regen:${c2}${pCR}
		} else {
			/cecho \awprevPriest[${i}]: \ag${prevPriest} \awMGB:${c1}${pMGB}\aw Regen:${c2}${pCR}
		}

	:nextPosition
	/next i

/return

|**
----------------------------
Check|Set the current heal spell
Note: this now checks to see what healing mode we are in.
----------------------------  
**|	
Sub setHealingSpell(string _class, int _hp)
	| determine the target type based on _class
	/declare archetype string local
	/declare aType     string local grp
	/call getClassArchetype ${_class}
	/varset archetype ${Macro.Return}

	/if (${archetype.Equal[TANK]}) 		/varset aType tank
	/if (${archetype.Equal[PRIEST]}) 	/varset aType self

	/if (${str_healMode.Equal[NORMAL]}) {
		/if (${_hp} < ${int_point_stopHeal}) {
			/if ((${_hp} <= ${int_currentFPoint}) && ${bln_FHealReady}) {
				/varset str_currentHeal	${str_heal_frantic}
			} else {
				/varset str_currentHeal ${str_heal_normal}
			}
		}	
	} else /if (${str_healMode.Equal[EFFICIENT]}) {
		/if (${_hp} < ${int_point_calcStopHeal}) {
			/if ((${_hp} <= ${int_currentFPoint}) && ${bln_FHealReady}) {
				/varset str_currentHeal	${str_heal_frantic_efficient}
			} else {
				/varset str_currentHeal ${str_heal_normal_efficient}
			}
		}	
	} else /if (${str_healMode.Equal[CHCHAIN]}) {
		|- Note, if we are in CHCHAIN mode we are not really going to use this  method as we will bypass all 
		|  normal healing routes
	} 
	/cecho \aysetHealingSpell ${_class} ${_hp} ${str_currentHeal}
/return


|**
 ----------------------------
 Check Health Subroutines
 ----------------------------
 These three subroutines are called in order from the Class Macro MainLoop
 checksMT	(and heals)
 checksSELF	(and heals)
 checksGRP	(and heals)
 ----------------------------
**|

Sub chkHeal_MT
	/if (${int_healMode}== 0) /return
	| Use the power of Greyskull and <strike>Netbots</strike> DANNET to get the MTs HP (todo)
	
	/call chkMTAlive ${Me.Class.ShortName.Equal[CLR]} 

	/if ((${Spawn[pc ${str_tankName1}].PctHPs} < ${int_point_tankNormal}) && (${Spawn[pc ${str_tankName1}].Distance} < ${int_range_normal})) {
		/call healMAINTANK
	}

/return


Sub chkHeal_Self
	/if (${int_healMode}>=0 && ${Me.PctHPs}<${int_point_selfNormal}) {
		/call healSELF		
	}
/return


Sub chkHeal_Group
	/if (${int_healMode}>=2) {
        /call healGRP		
	}
/return

|**
 ----------------------------
 HealMainTank
 ----------------------------
 Healing Logic for the Main Tank
 Determines which spell to cast, then casts it.
 Note MT may be changed to be a non-plate class due to deaths, etc
 ----------------------------
**|

Sub healMAINTANK
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_tankNormal}*1.1]}	
	/if (${Target.ID} != ${int_MTID}) /call TrueTarget ${int_MTID}
	/echo healMAINTANK (healing.inc): ${int_MTID} ${Target.Name} "${str_heal_normal}"
	/call healCast ${int_MTID} ${Target.Name} "${str_heal_normal}"
	/varset int_point_calcStopHeal	${int_point_tankNormal}
/return

|**
 ----------------------------
 HealSelf
 ----------------------------
 Healing Logic for the ranger
 Determines which spell to cast, then casts it.
 ----------------------------
**|

Sub healSELF
	/varcalc 	int_point_calcStopHeal ${Math.Calc[${int_point_selfNormal}*1.1]}
	/if (${Target.ID} != ${Me.ID}) /call TrueTarget ${Me.ID}
	/echo healSELF (healing.inc): ${Me.ID} ${Target.Name} "${str_heal_normal}"
	/call healCast ${Me.ID} ${Me.Name} "${str_heal_normal}"
	/varset int_point_calcStopHeal	${int_point_selfNormal}
/return

|**
 ----------------------------
 HealGroup
 ----------------------------
 Healing Logic for the Group
 Determines which spell to cast, then casts it.
 Note: 	There are variations between all the priest classes healGRP methods. 
 		So this is a default method that only casts 1 heal type
 ----------------------------
**|
Sub healGRP
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_grpNormal}*1.1]}
	/declare	int_worstHP		int	local	100
	/declare	int_worstID		int local	0
	/declare	int_majorHurt	int local	0
	/declare	gmember			int local	0
	/declare    str_worstName	string local NULL
	
	| -- Cycle through the group and set the worst HP
	/for gmember 0 to ${Group}
		/if ( (${Group.Member[${gmember}].Distance}<=${int_range_normal})) { 	
			/if (${Group.Member[${gmember}].PctHPs} < ${int_worstHP}) { 
				/varset int_worstID     	${Group.Member[${gmember}].ID} 
				/varset int_worstHP         ${Group.Member[${gmember}].PctHPs} 
				/varset str_worstName		${Group.Member[${gmember}].Name}
			} 
		} 
        /if ((${Group.Member[${gmember}].PctHPs}<=${int_point_calcStopHeal})&&(${Group.Member[${gmember}].PctHPs}< ${int_point_grpPatch}))   /varcalc int_majorHurt ${int_majorHurt}+1 
     /next gmember

	| -- If no one needs healing...
	/if ((${int_worstHP} > ${int_point_calcStopHeal}) || ${int_worstHP} == 0 ) /return

	| -- Determine appropriate heal
	/varset int_currentHP ${int_worstHP}

	| -- Single Target Heal
	/if (${int_majorHurt} >= 1 && (${int_worstHP} < ${int_point_grpPatch})) {
		|/echo PATCH single heal (Many have only 1 heal type, for multiple heal types the class method will override this)
		/varset str_currentHeal ${str_heal_normal}
		/echo HealGroupMember ${int_MTID} ${Target.Name} "${str_currentHeal}"
		/call healCast ${int_worstID} ${str_worstName} "${str_currentHeal}"
	}
/return




|**
 ----------------------------
 HealCasting
 ----------------------------
 Healing Cast pass through to Cast and spellroutines.inc
 This takes the set values from the above 3 logic routines and casts
 Calls chkHP while being cast in case we need to change
 ----------------------------
**|
Sub healCast(int _id, string _name, string _spell)
	/if (${_name.Equal[NULL]} || ${_id} == 0) {
		/echo HEAL NAME is NULL || ID is 0 something is fucked;
		/return
	}
	/echo healCast Target:${_name}(${_id}) cHP: ${Spawn[pc ${_name}].PctHPs} cSH: ${int_point_calcStopHeal} spell: "${_spell}"
	/if (${Spawn[pc ${_name}].PctHPs} < ${int_point_calcStopHeal} && (${Me.CurrentMana} > ${Spell[${_spell}].Mana})) {
		/if (${Target.ID} != ${_id}) /call TrueTarget ${_id}
		/call chkSpellMemorized "${_spell}" TRUE 2
		/call MQ2Cast "${_spell}" gem2 1s chkHP
		/if (${Macro.Return.Equal[CAST_SUCCESS]} && ${bln_annouce}) /${cmdChannel} ${_spell} > ${Target.Name} hp:${Target.PctHPs} m:${Me.PctMana} d:${Target.Distance}
		/return ${Macro.Return}
	}
/return

|**
 ----------------------------
 HealDivineArbitration
 ----------------------------
 Healing Divine Arb
 Switches on the Divine Arb that is available and casts it
 Calls chkDA while being cast in case we can duck out of it and save it
 ----------------------------
**|
Sub healDivineArbitration
	/if (${bln_annouce}) /dgt Incoming Divine Arbitration 
 
    /if (${Me.AltAbilityReady[divine arbitration]}) {
        /call MQ2Cast "Divine Arbitration" alt 2s chkDA
    } else /if (!${FindItem[${str_epicName}].Timer}) {
        /call MQ2Cast ${str_epicName} item 2s chkDA
    } else {
        /if (${bln_annouce}) /dgt No Saves Ready, we are boned.
    }
    /if ((${bln_healAfterArb})&&(${Me.PctMana}>40)) { 
        /if (${bln_annouce}) /dgt Incoming ${str_heal_group}
        /call MQ2Cast "${str_heal_group}" gem4 2s 
    } 
/return



|**
 ----------------------------
 Check Healing Subroutines
 ----------------------------
 This is called (looping) while a spell is being cast
 This will then call several check subs which initiates the chain chkHP -> , chkDARB, chkTargetDead [chkMTDead {getMT <setMT>}] ,
 ----------------------------
**|
Sub chkHP
	| Status Checks
	/call chkTargetDead

	| Return Checks
   	| -------------------------
	| Target Dead, Not Casting, need to heal self, or healing tank
	
	/if (${bln_TRGTDead}) {
		/if (${bln_MTDead}) {
			/call Interrupt
			/call chkMTAlive TRUE
			/return
		}
		/call Interrupt
		/return
	}


	| I'm no longer casting, or no longer need to stop it ie, target is MT
	/if (!${Window[CastingWindow].Open}) /return 

	| Check to see if I should interrupt because target is healed
	/if ((${bln_autointerrupt}) && (${Target.PctHPs} > ${int_point_calcStopHeal})) { 
		/call Interrupt 
		/return
	}

	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/if (!${use_heal_divineArb}) /return
	| Divine Arbitration Checks
   	| -------------------------
 	| Check to see if I should cast Divine Arbitration
	| Loop through group members to check to see if anyone needs Divine Arbitration
    /if (${bln_DARBReady}) {
		/declare gmember int local 0
		/for gmember 0 to ${Group.Members}
    		/if (${Group.Member[${gmember}].PctHPs}<=${int_point_divineArb}) {
    			| add a check for member in group here
    			/dgt ${Group.Member[${gmember}].Name} triggered Divine Arbitration!
    			/call Interrupt
    			/call healDivineArbitration
    			/return
    		}
		/next gmember
    }
/return

|**
----------------------------
Check|Set to see if the target is dead
----------------------------  
**|
Sub chkTargetDead
	/varset bln_TRGTDead	FALSE
	/varset bln_MTDead		FALSE
	/if (!${Target.Type.Equal[PC]} ) {
		/varset bln_TRGTDead TRUE
		/if (${Target.ID} == ${int_MTID}) {
			/varset bln_MTDead TRUE
		}
	}
/return


|**
 ------------------------
 CHCHAIN EVENT HANDLERS
	Note: CHCHAIN is only listened for by PRIEST classes
 ------------------------
**|
Sub EVENT_CHStart(_line, _sender, _eventParams)
	/cecho \ayComplete Heal Start \ag${_eventParams}
	/call GetCHPosition ${_eventParams.Arg[5]}
	/echo 1: chainID   ${_eventParams.Arg[1]}
	/echo 2: tank1     ${_eventParams.Arg[2]}
	/echo 3: tank2     ${_eventParams.Arg[3]}
	/echo 4: delay     ${_eventParams.Arg[4]}
	/echo 5: order     ${_eventParams.Arg[5]}
	/declare postion 	int local ${Macro.Return}
	/echo 6: position  ${postion}						
	/if (${postion} > 0) {
		/call heal_chStart "${_eventParams}" ${postion}	
	}
/return

Sub GetCHPosition(string _order)
	/echo GetCHPosition ${_order}
	/declare included 	bool	local FALSE
	/declare position 	int		local 0
	/declare len 		int		local ${Math.Calc[${_order.Count[,]}+1]}
	/declare x 			int 	local
	/for x 0 to ${len}
		/echo ${_order.Arg[${x},,]} ${Me.CleanName}
		/if (${_order.Arg[${x},,].Equal[${Me.CleanName}]}) {
			/varset included TRUE
			/varset position ${x}
		}
	/next x
/return ${position}

Sub heal_chStatus
/return

Sub heal_chStart(string _chSettings, _position)
	/cecho \agStarting CH id:${_chSettings.Arg[1]} tank1:${_chSettings.Arg[2]} tank2:${_chSettings.Arg[3]} delay:${_chSettings.Arg[4]} order:${_chSettings.Arg[5]}  position:${_position}
	/varset str_savedHealMode ${str_healMode}
	/call ChangeHealMode CHCHAIN
	/varset bln_chChainMember TRUE

	/varset  int_chChainID ${_chSettings.Arg[1]}
	/declare tank1    string	local	${_chSettings.Arg[2]}
	/varset  int_chTarget				${Spawn[pc ${tank1}].ID}
	/declare tank2    string	local	${_chSettings.Arg[3]}
	/declare delay    int		local	${_chSettings.Arg[4]}
	/declare order    int		local	${_chSettings.Arg[5]}
	/declare pos 	  int		local	${_position}
	/declare sDelay   int 		local   0

	/varcalc sDelay ${Math.Calc[(${delay}*${pos})+1]}
	/echo startDelay: ${sDelay} = [${delay}*${pos}+1]
	/call chkSpellMemorized "${str_heal_complete}" TRUE 9
	
	:chStart 
	/varset tank1 ${Spawn[pc id ${int_chTarget}].Name}
	/echo Starting Complete Healing, delaying ${sDelay}ms
	/delay ${sDelay}
	:chLoop
		/if (${Spawn[pc ${tank1}].ID} != ${int_chTarget}) {
			/echo CHCHain must have switched! new tank! restart with proper delay!
			/goto :chStart
		}
		/if (!${bln_chChainMember}) /goto :return
		/if (!${Spawn[pc ${tank1}].PctHPs} && !${tank1.Equal[${tank2}]}) /varset tank1 ${tank2}
		/if (!${Spawn[pc ${tank1}].PctHPs}) {
			/cecho \arCHChain ${int_chChainID} Cannot Find Tank of Last Resort ${tank2}
			/goto :return
		}
		/if (${Me.CurrentMana} < ${Math.Calc[${Spell[${str_heal_complete}].Mana}+20]}) {
			/cecho \arCHChain ${int_chChainID} OOM
			/goto :return
		}
		/echo chLoop Valid, running
		/if (${Spawn[pc ${tank1}].Distance} > ${int_range_complete}) {
			/echo CHChain ${int_chChainID} ${tank1} OOR!
			/goto :chLoop
		}
		/if (!${Me.Standing}) /stand
		/if (${bln_chPaused}) /goto :chPause
		/call TrueTarget ${int_chTarget}
		:chCast
	 	/dgt [${int_chChainID}]:[${pos}] CHC->${tank1} hp:${Target.PctHPs} d:${Target.Distance} m:${Me.PctMana}
		/call MQ2Cast "${str_heal_complete}" gem9 1s RunClassEvents
		/echo CH Cast ${Macro.Return}, delaying: ${delay}ms
		/if (${Macro.Return.Equal[CAST_FIZZLE]}) {
			/goto :chCast
		} else /if (${Macro.Return.Equal[CAST_CANCELLED]}) {
			/goto :chLoop
		} else {
			/delay ${delay}
		}
		/call RunAllEvents
		/goto :chLoop
	:chPause
		/echo chPaused ${timer_chPaused}

	:chPauseLoop
		/echo paused: ${timer_chPaused} remain
		/call RunClassEvents
		/call RunRaidEvents
		/delay 10
		/if (${timer_chPaused}) /goto :chPauseLoop
		/if (!${timer_chPaused}) /call heal_chResume
		/goto :chLoop
	:return
	/varset bln_chChainMember FALSE
	/doevents flush CHStart
/return


|-- Switch the MT of the ch chain. /dgt CHStop 1
Sub EVENT_CHPause(_line, _sender, _eventParams)
	/cecho \awPausing CH ${_eventParams}
	/call heal_chPause ${_eventParams}
	/doevents flush CHPause
/return

Sub heal_chPause(int _timeMS)
	/echo chPause triggered ${_timeMS}ms
	/varset bln_chPaused TRUE 
	/varset timer_chPaused ${_timeMS}
	/echo timer now ${timer_chPaused}
/return

Sub Event_CHResume
	/cecho \awResuming CH
	/call heal_chResume
	/doevents flush CHResume
/return

Sub heal_chResume
	/echo chResume triggered
	/varset bln_chPaused FALSE 
	/varset timer_chPaused 0
/return

|-- Switch the MT of the ch chain. /dgt CHStop 1
Sub EVENT_CHStop(_line, _sender, _eventParams)
	/if (!${bln_chChainMember}) /return
	/cecho \awStopping CH ${_eventParams}
	/call heal_chStop ${_eventParams}
	/doevents flush CHStop
/return

Sub heal_chStop(int _chainID)
	/echo chStop triggered ${_chainID}==? ${int_chChainID}
	/if ((${_chainID} == ${int_chChainID}) && ${bln_chChainMember}) {
		/if (${Me.Casting.ID}) /call Interrupt
		/varset bln_chChainMember FALSE
		/call ChangeHealMode ${str_savedHealMode}
	}
	
/return

|-- Switch the MT of the ch chain. /dgt CHSwitch 1 Xiria
Sub EVENT_CHSwitch(_line, _sender, _eventParams)
	/if (!${bln_chChainMember}) /return
	
	/cecho \awSwitching CH ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
	/call heal_chSwitch ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
/return

Sub heal_chSwitch(int _chainID, string _targetName)
	/echo chSwitch triggered ${_chainID} ${_targetName}(${Spawn[pc ${_targetName}].ID})
	/if ((${_chainID} == ${int_chChainID}) && ${bln_chChainMember}) {
		/if (${Me.Casting.ID}) /call Interrupt 
		/varset int_chTarget ${Spawn[pc ${_targetName}].ID}
	}
/return

|-------------------------------------------------------------------------------- 
|SUB: Check Resurrection
|--------------------------------------------------------------------------------
Sub chkRez
	/if (!${bln_autoRez}) /return
	/if (!${bln_combatRez} && ${bln_engaged}) /return

	| find all corpses in range	
	/squelch /hidecorpse npc
	/declare int_corpses int local ${SpawnCount[pc corpse radius ${int_range_rez}]}
	/if (!${int_corpses}) /return

	| if corpses, then aerez
	/call AERez ${int_corpses}
/return

Sub AERez
	/squelch /hidecorpse npc
	/declare int_corpses int local ${SpawnCount[pc corpse radius ${int_range_rez}]}
	/declare arr_corpses[${int_corpses}] int local 
	/declare str_corpse		string local

	/declare i 				int local
	/declare x				int local
	/declare id 			int local
	/declare charID int local

	/for i 1 to ${int_corpses}
		/varset arr_corpses[${i}] ${NearestSpawn[${i}, pc corpse radius ${int_range_rez}].ID}
	/next x
	
	:consider 
	/for i 1 to ${int_corpses}
		/varset id ${arr_corpses[${i}]}
		/varset bln_skipCorpse FALSE

		| get the name from corpse	
		/varset str_corpse ${Spawn[id ${id}].CleanName.Arg[1,']}
		/if (!${Bool[${str_corpse}]}) /goto :next_corpse

		| check corpse name 
		/dannet ${str_corpse} Me.ID
		/echo contacting ${str_corpse} ${Macro.Return}
		/varset charID ${Macro.Return}
		| make sure the target is actually a corpse, and online
		/if (${Spawn[id ${id}].CleanName.Find[corpse]} == NULL || !${charID}) /varset bln_skipCorpse TRUE
		/if (${bln_skipCorpse}) /goto :next_corpse


		| check ignore list
		/if (${int_rezIgnore}) {
			/for x 1 to ${int_rezIgnore}
				/if (${arr_corpses[${x}]} == ${arr_rezIgnore[${x}]}) /varset bln_skipCorpse TRUE
			/next x
		}
		/if (${bln_skipCorpse}) /goto :next_corpse

		| check online
		/call CheckOnline ${str_corpse}
		/if (${bln_skipCorpse}) /goto :next_corpse

		| consider the corpse
		/call TrueTarget ${id}
		/consider
		/delay 10
		/doevents CannotRez
		/if (${bln_skipCorpse}) /goto :next_corpse
			
		| resurrect
		/call Ressurect ${id}

	:next_corpse
	/next i

	:clean_list
	/call CleanRezIgnoreList
/return

Sub Ressurect(int _id)
	/call TrueTarget ${_id}
	/bct  ${Spawn[id ${_id}].CleanName.Arg[1,']} //consent ${Me.Name}
	/delay 10
	/corpse 

	| Find my epic (if cleric)
	/if (${FindItem[Water Sprinkler of Nem Ankh].InvSlot}) {
		/call MQ2Cast "Water Sprinkler of Nem Ankh" item
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
			/call Add2RezIgnore ${Target.ID}
			/rs Rezzed ${Target.Name}
		}
	} else {	
	| No epic or not cleric, use spell
		/call chkSpellMemorized ${str_rezSpell} TRUE ${str_rezGem}
		/call MQ2Cast ${str_rezSpell}
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
			/call Add2RezIgnore ${Target.ID}
			/rs Rezzed ${Target.Name}
		}
	}
/return


Sub CheckOnline(string _name)
	/tell ${_name} ${TellMessage}
	/doevents NotOnline
/return

Sub Event_NotOnline(Line, Name)
	| sent a tell on rez but got an offline message - interrupt cast
	/if (${Target.CleanName.Arg[1,'].Equal[${Name}]}) /call Interrupt
	/call Add2RezIgnore ${Target.ID}
	/varset bln_skipCorpse TRUE
/return

Sub Event_CannotRez
	/if (${Target.ID}) {
		/echo Could not rez ${Target.CleanName}, adding to ignore list.
		/call Add2RezIgnore ${Target.ID}
		/varset bln_skipCorpse TRUE
	}
/return

Sub Add2RezIgnore(int _id)
	| ignore list is empty - create it with new id
	/if (!${int_rezIgnore}) {
		/varset int_rezIgnore 1
		/if (${Defined[arr_rezIgnore]}) /deletevar arr_rezIgnore
		/declare arr_rezIgnore[1] int outer
		/varset arr_rezIgnore[1] ${_id}
		/return
	}
	| copy existing list temp list, one entry larger
	/declare arrTemp[${Math.Calc[${int_rezIgnore}+1]}] int local
	/declare x int local
	/for x 1 to ${int_rezIgnore}
		| return if new idis already in the ignore list
		/if (${_id} == ${arr_rezIgnore[${x}]}) /return
		/varset Temp[${x}] ${arr_rezIgnore[${x}]}
	/next x
	/varset int_rezIgnore ${Math.Calc[${int_rezIgnore}+1]}
	/varset Temp[${int_rezIgnore}] ${_id}
	| delete old ignore list and copy temp list to new ignore list
	/deletevar arr_rezIgnore
	/declare arr_rezIgnore[${int_rezIgnore}] int outer
	/for x 1 to ${int_rezIgnore}
		/varset arr_rezIgnore[${x}] ${Temp[${x}]}
	/next x
/return

Sub CleanRezIgnoreList
	/if (!${int_rezIgnore}) /return
	| remove ids from ignore list that are no longer in zone
	/declare x int local
	/declare found int local 0
	/declare arrTemp[${int_rezIgnore}] int local

	/for x 1 to ${int_rezIgnore}
		| skip corpse id if it's not in the zone spawn list any more
		/if (!${Spawn[id ${arr_rezIgnore[${x}]}].ID}) /goto :nextIgnore
		/varset found ${Math.Calc[${found}+1]}
		/varset arrTemp[${found}] ${arr_rezIgnore[${x}]}
	:nextIgnore
	/next x
	
	| shrink ignore list
	/if (${found} != ${int_rezIgnore}) {
		/deletevar arr_rezIgnore
		/varset int_rezIgnore ${found}
		/declare arr_rezIgnore[${int_rezIgnore}] int outer
		/for x 1 to ${int_rezIgnore}
			/varset arr_rezIgnore[${x}] ${arrTemp[${x}]}
		/next x
	}
/return


Sub EVENT_InterruptON
	/cecho \ayAutoInterrupt: \agON
	/call InterruptON
/return

Sub EVENT_InterruptOFF
	/cecho \ayAutoInterrupt: \arOFF
	/call InterruptOFF
/return

Sub InterruptON
	/varset bln_autointerrupt TRUE
/return

Sub InterruptOFF 
	/varset bln_autointerrupt FALSE
/return
|**
|----------------------------
| INITIALIZATION Subs
| ----------------------------
| Initialization of values for this specific libary
| ----------------------------
|**| 

Sub xbot_initialize_healing(int _healPoint)
	|/if (${str_classArchetype.Equal[MELEE_END]})	/return
	|/if (${Me.Class.ShortName.Equal[WAR]}) 			/return

	/declare 	healINI				string  outer	xiris_common/xiris_healing.ini

	|-- Set spellset to default so we can probe the properties of these
	/memspellset default

	|-- Heal Settings
	/declare    bln_defaultAutoInterrupt 	bool	outer  	${Ini[healINI,Heal_Settings,bln_autointerrupt]}
	/declare 	bln_autointerrupt			bool	outer	${Ini[botINI,Healing_Settings,bln_autointerrupt,${bln_defaultAutoInterrupt}]}
	/declare	str_spellset				string  outer 	default
	| 	Heal Mode: NORMAL|EFFICIENT|CHCHAIN
	/declare    str_healMode				string  outer 	NORMAL
	/declare	str_savedHealMode			string  outer   ${str_healMode}
	| 	Heal Mode: 0|Self Only, 1|Self/MT 2|Self/MT/Group
	/declare 	int_defaultHealMode			int 	outer   ${Ini[healINI,Heal_Settings,int_healMode]}
	/declare	int_healMode    			int     outer   ${Ini[botINI,Healing_Settings,int_healMode,${int_defaultHealMode}]}
	/declare 	use_heal_defaultDivineArb	bool	outer 	${Ini[healINI,Heal_Settings,use_heal_divineArb]}
	/declare	use_heal_divineArb			bool	outer	${Ini[botINI,Healing_Settings,use_heal_divineArb,${use_heal_divineArb}]}
	
	|-- Healing Spells
	/declare 	str_heal_normal_efficient	string	outer	${Ini[botINI,Healing_Spells,str_heal_normal_efficient,NULL]}
	/declare 	str_heal_normal				string	outer	${Ini[botINI,Healing_Spells,str_heal_normal,NULL]}
	/declare 	str_heal_frantic			string	outer	${Ini[botINI,Healing_Spells,str_heal_frantic,NULL]}
	/declare 	str_heal_frantic_efficient 	string	outer	${Ini[botINI,Healing_Spells,str_heal_frantic_efficient,NULL]}
	/declare 	str_heal_HoT				string	outer	${Ini[botINI,Healing_Spells,str_heal_HoT,NULL]}
	/declare 	str_heal_group				string	outer	${Ini[botINI,Healing_Spells,str_heal_group,NULL]}
	/declare 	str_heal_groupHoT			string	outer	${Ini[botINI,Healing_Spells,str_heal_groupHoT,NULL]}
	/declare 	str_heal_complete			string	outer	${Ini[botINI,Healing_Spells,str_heal_complete,NULL]}
	
	|-- Ressurection
	/declare	bln_autoRez					bool	outer	FALSE
	/declare 	bln_combatRez				bool	outer	FALSE
	/declare	str_rezSpell				string	outer	${Ini[botINI,Healing_Spells,str_rezSpell,Reviviscence]}
	/declare  	str_rezGem					string	outer	${Ini[botINI,Healing_Spells,str_rezGem,gem7]}

	/echo ---------------
	/echo Heal Spells:  Normal:${str_heal_normal}  || Efficient:${str_heal_frantic_efficient} || CH:${str_heal_complete}
	/echo				Frantic:${str_heal_frantic} || Frantic Efficient:${str_heal_frantic_efficient} || HOT: ${str_heal_HoT}
	/echo               Group:${str_heal_group} || GroupHoT:${str_heal_groupHoT} || Rez:${str_rezSpell}
	/echo ---------------
	|-- Heal Use (note, if null, or cannot find, we cannot use (gotta figure out how to tell if I have that spell...)
	/declare 	use_heal_patch			bool	outer	FALSE
		/if (${str_heal_normal_efficient.NotEqual[NULL]}) 	/varset use_heal_patch			TRUE

	/declare 	use_heal_normal			bool	outer	FALSE
		/if (${str_heal_normal.NotEqual[NULL]}) 	/varset use_heal_normal 	TRUE

	/declare 	use_heal_frantic		bool	outer	FALSE
		/if (${str_heal_frantic.NotEqual[NULL]}) 	/varset use_heal_frantic 	TRUE
		
	/declare 	use_heal_HoT			bool	outer	FALSE
		/if (${str_heal_HoT.NotEqual[NULL]}) 		/varset use_heal_HoT 		TRUE
		
	/declare 	use_heal_group			bool	outer	FALSE
		/if (${str_heal_group.NotEqual[NULL]}) 		/varset use_heal_group 		TRUE
		
	/declare 	use_heal_groupHoT		bool	outer	FALSE
		/if (${str_heal_groupHoT.NotEqual[NULL]}) 	/varset use_heal_groupHoT 	TRUE

	/echo Heal Usage:  Normal:${use_heal_normal} || Frantic:${use_heal_frantic} || HoT::${use_heal_HoT} || Group:${use_heal_group} || GroupHoT:${use_heal_groupHoT} || AutoRez:${bln_autoRez}
	/echo ---------------

	|-- Heal Ranges
	/declare	int_range_normal_efficient int	outer	${Spell[${str_heal_normal_efficient}].MyRange}
	/declare	int_range_normal		int		outer	${Spell[${str_heal_normal}].MyRange}
	/declare	int_range_frantic		int		outer	${Spell[${str_heal_frantic}].MyRange}
	/declare	int_range_HoT			int		outer	${Spell[${str_heal_HoT}].MyRange}
	/declare	int_range_group			int		outer	${Spell[${str_heal_group}].MyRange} |--Note this is a PBAE
	/declare	int_range_groupHoT		int		outer	${Spell[${str_heal_groupHoT}].MyRange}
	/declare 	int_range_complete		int		outer 	${Spell[${str_heal_complete}].MyRange}
	/declare	int_range_rez			int		outer	100
	/declare 	int_range_divineArb		int		outer	100

	
	/echo Heal Range: Patch:${int_range_normal_efficient} || Normal:${int_range_normal} || Frantic:${int_range_frantic} || HoT::${int_range_HoT} || Group:${int_range_group} || GroupHoT:${int_range_groupHoT} || Rez:${int_range_rez}
	/echo ---------------
	|-- Heal Points
		|-- SELF (Defaults)
		/declare 	int_def_selfPatch		int     outer   ${Ini[healINI,Heal_Settings,int_point_selfPatch]}
		/declare 	int_def_selfNormal		int     outer   ${Ini[healINI,Heal_Settings,int_point_selfNormal]}
		/declare 	int_def_selfFrantic		int     outer   ${Ini[healINI,Heal_Settings,int_point_selfFrantic]}
		/declare 	int_def_selfHot			int     outer   ${Ini[healINI,Heal_Settings,int_point_selfHot]}

		|-- SELF (Overrides)
		/declare	int_point_selfPatch		int 	outer 	${Ini[botINI,Heal_Points,int_point_selfPatch,${int_def_selfPatch}]}
		/declare	int_point_selfNormal	int		outer	${Ini[botINI,Heal_Points,int_point_selfNormal,${int_def_selfNormal}]}
		/declare	int_point_selfFrantic	int		outer	${Ini[botINI,Heal_Points,int_point_selfFrantic,${int_def_selfFrantic}]}
		/declare	int_point_selfHot		int		outer	${Ini[botINI,Heal_Points,int_point_selfHot,${int_def_selfHot}]}
	
		|-- TANK (Defaults)
		/declare 	int_def_tankPatch		int     outer   ${Ini[healINI,Heal_Settings,int_point_tankPatch]}
		/declare 	int_def_tankNormal		int     outer   ${Ini[healINI,Heal_Settings,int_point_tankNormal]}
		/declare 	int_def_tankFrantic		int     outer   ${Ini[healINI,Heal_Settings,int_point_tankFrantic]}
		/declare 	int_def_tankHot			int     outer   ${Ini[healINI,Heal_Settings,int_point_tankHot]}

		|-- TANK (Overrides)
		/declare	int_point_tankPatch		int		outer	${Ini[botINI,Heal_Points,int_point_tankPatch,${int_def_tankPatch}]}
		/declare	int_point_tankNormal	int		outer	${Ini[botINI,Heal_Points,int_point_tankNormal,${int_def_tankNormal}]}	
		/if (${_healPoint} > 0) {
			/cecho \agHealpoint setting overriding defaults: ${_healPoint}	
			/varset	int_point_tankNormal	${_healPoint}
		} else {
			/echo No healpoint defined using INI values
		}
		/declare	int_point_tankFrantic	int		outer	${Ini[botINI,Heal_Points,int_point_tankFrantic,${int_def_tankFrantic}]}
		/declare	int_point_tankHot		int		outer	${Ini[botINI,Heal_Points,int_point_tankHot,${int_def_tankHot}]}
	
		|-- Rest of Group (Defaults)
		/declare 	int_def_grpPatch		int     outer   ${Ini[healINI,Heal_Settings,int_point_grpPatch]}
		/declare 	int_def_grpNormal		int     outer   ${Ini[healINI,Heal_Settings,int_point_grpNormal]}
		/declare 	int_def_grpFrantic		int     outer   ${Ini[healINI,Heal_Settings,int_point_grpFrantic]}
		/declare 	int_def_grpHot			int     outer   ${Ini[healINI,Heal_Settings,int_point_grpHot]}

		|-- Rest of Group (Overrides)
		/declare	int_point_grpPatch		int		outer	${Ini[botINI,Heal_Points,int_point_grpPatch,${int_def_grpPatch}]}	
		/declare	int_point_grpNormal		int		outer	${Ini[botINI,Heal_Points,int_point_grpNormal,${int_def_grpNormal}]}	
		/declare	int_point_grpFrantic	int		outer	${Ini[botINI,Heal_Points,int_point_grpFrantic,${int_def_grpFrantic}]}
		/declare	int_point_grpHot		int		outer	${Ini[botINI,Heal_Points,int_point_grpHot,${int_def_grpHot}]}	

		|-- Divine Arbitration && Misc
		/declare    int_point_divineArb     int		outer	${Ini[botINI,Heal_Points,int_point_divineArb,20]}
		/declare    int_point_stopHeal    	int		outer   ${Ini[botINI,Heal_Points,int_point_stopHeal,90]}	
		/declare	int_point_calcStopHeal	int		outer	100
		/declare    use_interrupt_selfHeal  bool	outer   TRUE
		/declare    int_point_interrupt		int		outer   ${Ini[botINI,Heal_Points,int_point_interrupt,50]}
	
	|-- Celestial Regeneration Chain
	/declare bln_hasCelRegen 				bool    outer   ${Me.Class.ShortName.Equal[CLR]}
	/if (${bln_hasCelRegen}) {
		/declare bln_celRegenActive 			bool	outer	FALSE 
		/declare int_celRegenTotal 				int 	outer	${Ini[${healINI},CelestialRegeneration,int_totalCR]}
		/declare arr_celRegen[${int_celRegenTotal}] string outer
		/declare timer_celregen					timer   outer	0
		|   Parse the chain's character list to an array
		/declare int_celRegenPosition			int		outer   1
		/declare str_celRegenPPriest            string  outer   NONE
		/call setCelRegenChain

		/echo Celestial Regeneration:${bln_hasCelRegen} || Total Chars:${int_celRegenTotal} || Position:${int_celRegenPosition} || Previous::${str_celRegenPPriest}
		/echo ---------------
	}
	|-- Various Flags
	/declare	int_currentHP			int 	outer	100
	/declare 	str_currentHeal			string	outer	${str_heal_normal}
	/declare 	bln_chChainMember		bool	outer	FALSE
	/declare	int_chTarget			int  	outer   0
	/declare    int_chDelay				int		outer	2s
	/declare    bln_chPaused			bool	outer	FALSE
	/declare	timer_chPaused			timer   outer   30s
	/declare   	int_chChainID			int		outer	0
	/declare	int_rezIgnore			int     outer   0 
	/declare 	arr_rezIgnore[1]		int		outer
	/declare 	bln_skipCorpse			bool	outer	FALSE

	|-- OLD Healing Spells
	/declare    bln_rezzable			bool    outer   TRUE
	
	/declare	bln_MTDead				bool	outer   FALSE
	/declare	bln_TRGTDead			bool	outer	FALSE

	
	|-- OLD Heal Points
	

	|-- Hard Coded Complete Heal Spells
	/declare 	str_completeHeal		string	 outer
	/if (${Me.Class.Name.Equal[Cleric]}) /varset str_completeHeal Complete Heal
    /if (${Me.Class.Name.Equal[Druid]})  /varset str_completeHeal Karana's Renewal
    /if (${Me.Class.Name.Equal[Shaman]}) /varset str_completeHeal Kragg's Mending
	/declare	str_chGem				string	outer   ${Ini[botINI,Healing_Spells,int_grpHealRNG,gem9]}
	
	
	|-- Complete Healing Arguments
	/declare CHealRunning 	bool 	outer FALSE
    /declare CHealTank 		string	outer
    /declare CHealTankTwo 	string	outer
    /declare CHealDelay		string 	outer
    /declare CHealNext		string	outer
	
	
	|-- Group HOT Timers
	/declare i int local
	/declare timer_HoT_0	timer 	outer 	0
	/declare timer_HoT_1	timer 	outer 	0
	/declare timer_HoT_2	timer 	outer 	0
	/declare timer_HoT_3	timer 	outer 	0
	/declare timer_HoT_4	timer 	outer 	0
	/declare timer_HoT_5	timer 	outer 	0

	/declare timer_groupHoT  timer outer 0

	/call InitEvents_Healing
	/cecho \ao xbot_initialize_healing ${_healPoint}
/return
