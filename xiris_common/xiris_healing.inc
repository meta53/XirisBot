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
**|

#event GroupHeal  "#*#DoGroupHeal#*#"
#event CanRez     "This corpse can be resurrected."
#event CannotRez  "This corpse cannot be resurrected."
#event NotOnline  "You told #1#, '#2# is not online at this time'"
#event NotOnline  "Your tell to #1# has been queued"
#event ChangeHP   "#*#ChangeHP #1#"

#EVENT HealMode "<#1#> HealMode #2#"
#EVENT HealMode "[#1#(msg)] HealMode #2#"
#EVENT HealMode "#1# tells you, 'HealMode #2#"
#EVENT HealMode "#1# tells the group, 'HealMode #2#'"
#EVENT HealMode "#1# you tell your party, 'HealMode #2#'"


#EVENT InterruptON "<#1#> InterruptON#*#"
#EVENT InterruptON "[#1#(msg)] InterruptON#*#"
#EVENT InterruptON "#1# tells you, 'InterruptON#*#"
#EVENT InterruptON "#1# tells the group, 'InterruptON#*#'"
#EVENT InterruptON "#1# you tell your party, 'InterruptON#*#'"


#EVENT InterruptOFF "<#1#> InterruptOFF#*#"
#EVENT InterruptOFF "[#1#(msg)] InterruptOFF#*#"
#EVENT InterruptOFF "#1# tells you, 'InterruptOFF#*#"
#EVENT InterruptOFF "#1# tells the group, 'InterruptOFF#*#'"
#EVENT InterruptOFF "#1# you tell your party, 'InterruptOFF#*#'"

|-- CHEvents should only be listened for in the class handlers (cleric, druid)
#EVENT CHStart "<#1#> CHStart #2#"
#EVENT CHStart "[#1#(msg)] CHStart #2#"
#EVENT CHStart "#1# tells you, 'CHStart #2#'"
#EVENT CHStart "#1# tells the group, 'CHStart #2#'"
#EVENT CHStart "#1# you tell your party, 'CHStart #2#'"

#EVENT CHStop "<#1#> CHStop #2#"
#EVENT CHStop "[#1#(msg)] CHStop #2#"
#EVENT CHStop "#1# tells you, 'CHStop #2#'"
#EVENT CHStop "#1# tells the group, 'CHStop #2#'"
#EVENT CHStop "#1# you tell your party, 'CHStop #2#'"

#EVENT CHPause "<#1#> CHPause #2#"
#EVENT CHPause "[#1#(msg)] CHPause #2#"
#EVENT CHPause "#1# tells you, 'CHPause #2#'"
#EVENT CHPause "#1# tells the group, 'CHPause #2#'"
#EVENT CHPause "#1# you tell your party, 'CHPause #2#'"

#EVENT CHResume "<#1#> CHResume#*#"
#EVENT CHResume "[#1#(msg)] CHResume#*#"
#EVENT CHResume "#1# tells you, 'CHResume#*#'"
#EVENT CHResume "#1# tells the group, 'CHResume#*#'"
#EVENT CHResume "#1# you tell your party, 'CHResume#*#'"

#EVENT CHSwitch "<#1#> CHSwitch #2#"
#EVENT CHSwitch "[#1#(msg)] CHSwitch #2#"
#EVENT CHSwitch "#1# tells you, 'CHSwitch #2#'"
#EVENT CHSwitch "#1# tells the group, 'CHSwitch #2#'"
#EVENT CHSwitch "#1# you tell your party, 'CHSwitch #2#'"


Sub InitEvents_Healing
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[MELEE_MANA]} ) /call RegisterBackgroundEvents Events_Healing 6
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
		/bc [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tankNormal}
	}
	/doevents flush ChangeHP
/return

Sub EVENT_HealMode(_line, _sender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/echo Changing Heal Mode?
	/call ChangeHealMode ${_eventParams.Arg[1, ]}
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
	/echo Changing healing mode to ${_strMode}
	/varset str_healMode ${_strMode}
	|- Mem the appropriate spell set
	/if (${_strMode.Equal[CHCHAIN]}) {
		/varset str_spellset default
	} else /if (${_strMode.Equal[EFFICIENT]}) {
		/varset str_spellset efficient
		/varset use_heal_HoT FALSE
	} else {
		/varset str_spellset default
		/varset use_heal_HoT TRUE
	}
	/memspellset ${str_spellset}
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
	| Use the power of Greyskull and Netbots to get the MTs HP
	/if ((${NetBots[${Spawn[id ${int_MTID}].Name}].PctHPs}< ${int_point_tankNormal}) && (${NetBots[${Spawn[id ${int_MTID}].Name}].Distance}<${int_range_normal})) { 
		/call chkMTAlive ${Me.Class.ShortName.Equal[CLR]} 
		/call healMAINTANK
	}
/return


Sub chkHeal_Self
	/if (${int_healmode}>=1 && ${Me.PctHPs}<${int_point_selfNormal}) {
		/call healSELF		
	}
/return


Sub chkHeal_Group
	/if (${int_healmode}>=2) {
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
	/echo healMainTank(healing.inc)
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_tankNormal}*1.1]}	
	/echo healMAINTANK ${int_MTID} ${Target.Name} "${str_heal_normal}"
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
	/echo healSELF ${Me.ID} ${Target.Name} "${str_heal_normal}"
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
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_grpHealPoint}*1.1]}

	/declare	int_numDamaged	int	local	0
	/declare	int_worstHP		int	local	100
	/declare	int_worstID		int local	0
	/declare	int_minorHurt	int local	0
	/declare	int_majorHurt	int local	0
	/declare	gmember			int local	0
	/declare    str_worstName	string local NULL
	/declare	str_worstClass	string local NULL
	
	| -- Cycle through the group and set the worst HP
	/for gmember 0 to ${Group}
		/if ( (${Group.Member[${gmember}].Distance}<=${int_range_normal})) { 	
			/if (${Group.Member[${gmember}].PctHPs} < ${int_worstHP}) { 
				/varset int_worstID     	${Group.Member[${gmember}].ID} 
				/varset int_worstHP         ${Group.Member[${gmember}].PctHPs} 
				/varset str_worstName		${Group.Member[${gmember}].Name}
				/varset str_worstClass      ${Group.Member[${gmember}].Class.Name} 
			} 
		} 
        /if ((${Group.Member[${gmember}].Distance}<=${int_range_normal})&&(${Group.Member[${gmember}].PctHPs}<=${int_point_calcStopHeal})) /varcalc int_numDamaged ${int_numDamaged}+1 
        /if ((${Group.Member[${gmember}].PctHPs}<=${int_point_calcStopHeal})&&(${Group.Member[${gmember}].PctHPs}>=${int_point_grpPatch}))   /varcalc int_minorHurt ${int_minorHurt}+1 
        /if ((${Group.Member[${gmember}].PctHPs}<=${int_point_calcStopHeal})&&(${Group.Member[${gmember}].PctHPs}< ${int_point_grpPatch}))   /varcalc int_majorHurt ${int_majorHurt}+1 
     /next gmember

	| -- If no one needs healing...
	/if ((${int_worstHP} > ${int_point_calcStopHeal}) || ${int_worstHP} == 0 ) /return

	| -- Determine appropriate heal
	/varset int_currentHP ${int_worstHP}

	| -- Single Target Heal
	/if (${int_majorHurt} >= 1) {
		/if (${int_worstHP} > ${int_point_grpPatch}) /return
		/echo PATCH single heal (Many have only 1 heal type, for multiple heal types the class method will override this)
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
		/echo HEAL NAME is NULL something is fucked;
		/return
	}
	/echo healCast Target:${_name}(${_id}) cHP: ${NetBots[${_name}].PctHPs} cSH: ${int_point_calcStopHeal} spell: "${_spell}"
	/if (${NetBots[${_name}].PctHPs} < ${int_point_calcStopHeal} && (${Me.CurrentMana} > ${Spell[${_spell}].Mana})) {
		/if (${Target.ID} != ${_id}) /call TrueTarget ${_id}
		/delay 2s (${Target.ID} == ${_id})
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
	/if (${bln_annouce}) /bc Incoming Divine Arbitration 
 
    /if (${Me.AltAbilityReady[divine arbitration]}) {
        /call MQ2Cast "Divine Arbitration" alt 2s chkDA
    } else /if (!${FindItem[${str_epicName}].Timer}) {
        /call MQ2Cast ${str_epicName} item 2s chkDA
    } else {
        /if (${bln_annouce}) /bc No Saves Ready, we are boned.
    }
    /if ((${bln_healAfterArb})&&(${Me.PctMana}>40)) { 
        /if (${bln_annouce}) /bc Incoming ${str_heal_group}
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
	/if (( ${use_interrupt_selfHeal})&&(${Target.ID}==${int_MTID}))	/if ((${Me.PctHPs}>=90)&&(${castEndTime}>=12)) /return
	/if ((!${use_interrupt_selfHeal})&&(${Target.ID}==${int_MTID}))	/if (${castEndTime}>=12) /return 

	| Check to see if I should interrupt because target is healed
	/if ((${bln_autointerrupt})&&(${Target.PctHPs}>${int_point_calcStopHeal})) { 
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
    			/bc ${Group.Member[${gmember}].Name} triggered Divine Arbitration!
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
	Note: CHCHAIN is handled by seperate macro. 
	Leaving stubbs here only to prevent breakage
 ------------------------
**|
Sub EVENT_CHStart(_line, _sender, _eventParams)
	/cecho \ayCompleteHealChain \ag${_eventParams}
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
	/cecho \agheal_chStart id:${_chSettings.Arg[1]} tank1:${_chSettings.Arg[2]} tank2:${_chSettings.Arg[3]} delay:${_chSettings.Arg[4]} order:${_chSettings.Arg[5]}  position:${_position}
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
	 	/bc [${int_chChainID}]:[${pos}] CHC->${tank1} hp:${Target.PctHPs} d:${Target.Distance} m:${Me.PctMana}
		/call MQ2Cast "${str_heal_complete}" gem9 1s RunClassEvents
		/echo CH Cast ${Macro.Return}, delaying: ${delay}ms
		/if (${Macro.Return.Equal[CAST_FIZZLE]}) {
			/goto :chCast
		} else /if (${Macro.Return.Equal[CAST_CANCELLED]}) {
			/goto :chLoop
		} else {
			/delay ${delay}
		}
		
		/call RunClassEvents
		/call RunRaidEvents
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
/return


|-- Switch the MT of the ch chain. /bc CHStop 1
Sub EVENT_CHPause(_line, _sender, _eventParams)
	/cecho \awCHPause ${_eventParams}
	/call heal_chPause ${_eventParams}
/return

Sub heal_chPause(int _timeMS)
	/echo chPause triggered ${_timeMS}ms
	/varset bln_chPaused TRUE 
	/varset timer_chPaused ${_timeMS}
	/echo timer now ${timer_chPaused}
/return

Sub Event_CHResume
	/cecho \awCHResume
	/call heal_chResume
/return

Sub heal_chResume
	/echo chResume triggered
	/varset bln_chPaused FALSE 
	/varset timer_chPaused 0
/return

|-- Switch the MT of the ch chain. /bc CHStop 1
Sub EVENT_CHStop(_line, _sender, _eventParams)
	/if (!${bln_chChainMember}) /return
	/cecho \awCHStop ${_eventParams}
	/call heal_chStop ${_eventParams}
/return

Sub heal_chStop(int _chainID)
	/echo chStop triggered ${_chainID}==? ${int_chChainID}
	/if ((${_chainID} == ${int_chChainID}) && ${bln_chChainMember}) {
		/if (${Me.Casting.ID}) /call Interrupt
		/varset bln_chChainMember FALSE
		/call ChangeHealMode NORMAL
	}
/return

|-- Switch the MT of the ch chain. /bc CHSwitch 1 Xiria
Sub EVENT_CHSwitch(_line, _sender, _eventParams)
	/if (!${bln_chChainMember}) /return
	
	/cecho \awCHSwitch ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
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
	/declare i int local
	/declare CorpseCount int local ${SpawnCount[pc corpse radius ${int_range_rez}]}
	/for i 1 to ${CorpseCount}
		/target ${NearestSpawn[${i}, corpse pc radius ${int_range_rez}]}'s
		
		/delay 3s ${Target.ID} == ${NearestSpawn[${i}, corpse pc radius ${int_range_rez}].ID}
		
		/if (!${Target.LineOfSight}) /goto :next
		/consider
		/delay 3s
		/call RunRaidEvents	
		/delay 1s
	:next
	/next i
/return

Sub Event_CanRez
	/declare TargetName string local ${Target.CleanName.Arg[1,']}
	/declare TellMessage string local rezinc

	
	/if (${Target.ID}) {
		/varset DoingARez TRUE
		/bct ${TargetName} //consent ${Me.Name}
		
		| announce the rez if quiet option not set
		/tell ${Target.CleanName.Arg[1,']} ${TellMessage}
		

		| use custom spell if defined, otherwise epic, or fall back to Reviviscence
		/if (${FindItem[Water Sprinkler of Nem Ankh].InvSlot}) {
			/corpse
			/call MQ2Cast "Water Sprinkler of Nem Ankh" item 40 CheckOnline
			/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
				/call AddIgnoreList ${Target.ID}
				/bc Rezzed ${Target.Name}
				/rs Rezzed ${Target.Name}
			}
		} else {
			/corpse
			/call MQ2Cast "${str_rezSpell}" ${str_rezGem} 20 CheckOnline
			/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
				/call AddIgnoreList ${Target.ID}
				/bc Rezzed ${Target.Name}
				/rs Rezzed ${Target.Name}
			}
		}
		/varset DoingARez FALSE
	}
/return

Sub CheckOnline
	/doevents NotOnline
/return

Sub Event_NotOnline(Line, Name)
	| sent a tell on rez but got an offline message - interrupt cast
	/if (${Target.CleanName.Arg[1,'].Equal[${Name}]}) /call Interrupt
	/varset bln_rezzable FALSE
/return

Sub Event_CannotRez
	/if (${Target.ID} && (${DoingAERez} || ${AlwaysRezOnCon})) {
		/echo Could not rez ${Target.CleanName}, adding to ignore list.
		/varset bln_rezzable FALSE
	}
/return

Sub AddIgnoreList(int NewID)
	/if (!${Defined[RezIgnoreCount]}) /declare RezIgnoreCount int outer 0
	| ignore list is empty - create it with new id
	/if (!${RezIgnoreCount}) {
		/varset RezIgnoreCount 1
		/if (${Defined[RezIgnoreList]}) /deletevar RezIgnoreList
		/declare RezIgnoreList[1] int outer
		/varset RezIgnoreList[1] ${NewID}
		/return
	}
	| copy existing list temp list, one entry larger
	/declare Temp[${Math.Calc[${RezIgnoreCount}+1]}] int local
	/declare x int local
	/for x 1 to ${RezIgnoreCount}
		| return if new idis already in the ignore list
		/if (${NewID} == ${RezIgnoreList[${x}]}) /return
		/varset Temp[${x}] ${RezIgnoreList[${x}]}
	/next x
	/varset RezIgnoreCount ${Math.Calc[${RezIgnoreCount}+1]}
	/varset Temp[${RezIgnoreCount}] ${NewID}
	| delete old ignore list and copy temp list to new ignore list
	/deletevar RezIgnoreList
	/declare RezIgnoreList[${RezIgnoreCount}] int outer
	/for x 1 to ${RezIgnoreCount}
		/varset RezIgnoreList[${x}] ${Temp[${x}]}
	/next x
/return

Sub CleanIgnoreList
	/if (!${RezIgnoreCount}) /return
	| remove ids from ignore list that are no longer in zone
	/declare x int local
	/declare Found int local 0
	/declare Temp[${RezIgnoreCount}] int local

	/for x 1 to ${RezIgnoreCount}
		| skip corpse id if it's not in the zone spawn list any more
		/if (!${Spawn[id ${RezIgnoreList[${x}]}].ID}) /goto :nextIgnore
		/varset Found ${Math.Calc[${Found}+1]}
		/varset Temp[${Found}] ${RezIgnoreList[${x}]}
	:nextIgnore
	/next x
	
	| shrink ignore list
	/if (${Found} != ${RezIgnoreCount}) {
		/deletevar RezIgnoreList
		/varset RezIgnoreCount ${Found}
		/declare RezIgnoreList[${RezIgnoreCount}] int outer
		/for x 1 to ${RezIgnoreCount}
			/varset RezIgnoreList[${x}] ${Temp[${x}]}
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

	/call InitEvents_Healing
	/cecho \ao xbot_initialize_healing ${_healPoint}
	
	|-- Set spellset to default so we can probe the properties of these
	/memspellset default

	|-- Heal Settings
	/declare 	bln_autointerrupt			bool	outer	TRUE
	/declare	str_spellset				string  outer 	default
	| 	Heal Mode NORMAL|EFFICIENT|CHCHAIN
	/declare    str_healMode				string  outer 	NORMAL
	| 	Heal Mode 1 - self, 2 - group (default)
	/declare	int_healmode    			int     outer   ${Ini[botINI,Healing_Settings,int_healmode,2]}
	/declare	use_heal_divineArb			bool	outer	${Ini[botINI,Healing_Settings,use_heal_divineArb,FALSE]}
	
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
		|-- Cleric (self)
		/declare	int_point_selfPatch		int 	outer 	${Ini[botINI,Heal_Points,int_point_selfPatch,85]}
		/declare	int_point_selfNormal	int		outer	${Ini[botINI,Heal_Points,int_point_selfNormal,65]}
		/declare	int_point_selfFrantic	int		outer	${Ini[botINI,Heal_Points,int_point_selfFrantic,40]}
		/declare	int_point_selfHot		int		outer	${Ini[botINI,Heal_Points,int_point_selfHot,95]}
	
		|-- Tank
		/declare	int_point_tankPatch		int		outer	${Ini[botINI,Heal_Points,int_point_tankPatch,85]}
		/declare	int_point_tankNormal	int		outer	${Ini[botINI,Heal_Points,int_point_tankNormal,65]}	
		/if (${_healPoint} > 0) {
			/cecho \agHealpoint setting overriding defaults: ${_healPoint}	
			/varset	int_point_tankNormal	${_healPoint}
		} else {
			/echo No healpoint defined using INI values
		}


		/declare	int_point_tankFrantic	int		outer	${Ini[botINI,Heal_Points,int_point_tankFrantic,40]}
		/declare	int_point_tankHot		int		outer	${Ini[botINI,Heal_Points,int_point_tankHot,95]}
	
		|-- Rest of Group
		/declare	int_point_grpPatch		int		outer	${Ini[botINI,Heal_Points,int_point_grpPatch,75]}	
		/declare	int_point_grpNormal		int		outer	${Ini[botINI,Heal_Points,int_point_grpNormal,60]}	
		/declare	int_point_grpFrantic	int		outer	${Ini[botINI,Heal_Points,int_point_grpFrantic,40]}
		/declare	int_point_grpHot		int		outer	${Ini[botINI,Heal_Points,int_point_grpHot,85]}	

		|-- Divine Arbitration && Misc
		/declare    int_point_divineArb     int		outer	${Ini[botINI,Heal_Points,int_point_divineArb,20]}
		/declare    int_point_stopHeal    	int		outer   ${Ini[botINI,Heal_Points,int_point_stopHeal,90]}	
		/declare	int_point_calcStopHeal	int		outer	100
		/declare    use_interrupt_selfHeal  bool	outer   TRUE
		/declare    int_point_interrupt		int		outer   ${Ini[botINI,Heal_Points,int_point_interrupt,50]}
	
	|-- Various Flags
	/declare	int_currentHP			int 	outer	100
	/declare 	str_currentHeal			string	outer	${str_heal_normal}
	/declare 	bln_chChainMember		bool	outer	FALSE
	/declare	int_chTarget			int  	outer   0
	/declare    int_chDelay				int		outer	2s
	/declare    bln_chPaused			bool	outer	FALSE
	/declare	timer_chPaused			timer   outer   30s
	/declare   	int_chChainID			int		outer	0

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


/return
