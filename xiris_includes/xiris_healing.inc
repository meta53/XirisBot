|**
----------------------------
xiris_common.inc
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
	10.13.22	xiris 	Update sub healCast to ignore hp checks on group heals			
	12.31.22	xiris	Worked on Divine Arbitration handler, should be cleaner
	01.31.23	xiris 	Replaced chkGroup healing loops to use the MQ2WorstHurt plugin	
	02.02.23	xiris	Fixed long standing bug: chkHeal_MT would always call chkMTAlive, with targetting to TRUE, which would break cleric debuffing since it 
						would always target the MT before trying to cast debuff
**|



|-- Settings and Triggers
#EVENT GroupHeal  		"#*#DoGroupHeal#*#"
#EVENT InterruptON 		"#*#InterruptON#*#"
#EVENT InterruptOFF 	"#*#InterruptOFF#*#"

#EVENT ChangeHP			"#1# tells you#*#'ChangeHP #2#'"
#EVENT ChangeHP			"#1# tells the group#*#'ChangeHP #2#'"
#EVENT ChangeHP			"#1# tells the raid#*#'ChangeHP #2#'"
#EVENT ChangeHP 		"[ #1# #*#]#*#ChangeHP #2#"

#EVENT ChangeHPTank		"#1# tells you#*#'changeHPTank #2#'"
#EVENT ChangeHPTank		"#1# tells the group#*#'changeHPTank #2#'"
#EVENT ChangeHPTank		"#1# tells the raid#*#'changeHPTank #2#'"
#EVENT ChangeHPTank 	"[ #1# #*#]#*#changeHPTank #2#"

#EVENT ChangeHPSelf		"#1# tells you#*#'changeHPSelf #2#'"
#EVENT ChangeHPSelf		"#1# tells the group#*#'changeHPSelf #2#'"
#EVENT ChangeHPSelf		"#1# tells the raid#*#'changeHPSelf #2#'"
#EVENT ChangeHPSelf 	"[ #1# #*#]#*#changeHPSelf #2#"

#EVENT ChangeHPGroup	"#1# tells you#*#'changeHPGroup #2#'"
#EVENT ChangeHPGroup	"#1# tells the group#*#'changeHPGroup #2#'"
#EVENT ChangeHPGroup	"#1# tells the raid#*#'changeHPGroup #2#'"
#EVENT ChangeHPGroup 	"[ #1# #*#]#*#changeHPGroup #2#"

#EVENT HealMode			"#1# tells you#*#'HealMode #2#'"
#EVENT HealMode			"#1# tells the group#*#'HealMode #2#'"
#EVENT HealMode			"#1# tells the raid#*#'HealMode #2#'"
#EVENT HealMode 		"[ #1# #*#]#*#HealMode #2#"

#EVENT HealPoints		"#1# tells you#*#'SetHealPOINTS #2#'"
#EVENT HealPoints		"#1# tells the group#*#'SetHealPOINTS #2#'"
#EVENT HealPoints		"#1# tells the raid#*#'SetHealPOINTS #2#'"
#EVENT HealPoints		"[ #1# #*#]#*#SetHealPOINTS #2#"

|-- Heal type events
#EVENT FocusHEALMT_ON 	"#1# tells you#*#'FocusHealMTOn#*#'"
#EVENT FocusHEALMT_ON	"#1# tells the group#*#'FocusHealMTOn#*#'"
#EVENT FocusHEALMT_ON	"#1# tells the raid#*#'FocusHealMTOn#*#'"
#EVENT FocusHEALMT_ON	"[ #1# #*#]#*#FocusHealMTOn#*#"

#EVENT FocusHEALMT_OFF 	"#1# tells you#*#'FocusHealMTOff#*#'"
#EVENT FocusHEALMT_OFF	"#1# tells the group#*#'FocusHealMTOff#*#'"
#EVENT FocusHEALMT_OFF	"#1# tells the raid#*#'FocusHealMTOff#*#'"
#EVENT FocusHEALMT_OFF	"[ #1# #*#]#*#FocusHealMTOff#*#"

#EVENT HealType			"#1# tells you#*#'HealType #2#'"
#EVENT HealType			"#1# tells the group#*#'HealType #2#'"
#EVENT HealType			"#1# tells the raid#*#'HealType #2#'"
#EVENT HealType 		"[ #1# #*#]#*#HealType #2#"

|-- Rez Events
#EVENT REZZABLE     	"This corpse can be resurrected."
#EVENT CANNOTREZ  		"This corpse cannot be resurrected."
#EVENT NOTONLINE  		"You told #1#, '#2# is not online at this time'"
#EVENT NOTONLINE  		"Your tell to #1# has been queued"

|-- CHEvents should only be listened for in the class handlers (cleric, druid)
#EVENT CHStart			"#1# tells you#*#'CHStart #2#'"
#EVENT CHStart			"#1# tells the group#*#'CHStart #2#'"
#EVENT CHStart			"#1# tells the raid#*#'CHStart #2#'"
#EVENT CHStart			"[ #1# #*#]#*#CHStart #2#"

#EVENT CHStop			"#1# tells you#*#'CHStop #2#'"
#EVENT CHStop			"#1# tells the group#*#'CHStop #2#'"
#EVENT CHStop			"#1# tells the raid#*#'CHStop #2#'"
#EVENT CHStop			"[ #1# #*#]#*#CHStop #2#"

#EVENT CHPause			"#1# tells you#*#'CHPause #2#'"
#EVENT CHPause			"#1# tells the group#*#'CHPause #2#'"
#EVENT CHPause			"#1# tells the raid#*#'CHPause #2#'"
#EVENT CHPause			"[ #1# #*#]#*#CHPause #2#"

#EVENT CHResume			"#1# tells you#*#'CHResume#*#'"
#EVENT CHResume			"#1# tells the group#*#'CHResume#*#'"
#EVENT CHResume			"#1# tells the raid#*#'CHResume#*#'"
#EVENT CHResume 		"[ #1# #*#]#*#CHResume#*#"

#EVENT CHSwitch			"#1# tells you#*#'CHSwitch #2#'"
#EVENT CHSwitch			"#1# tells the group#*#'CHSwitch #2#'"
#EVENT CHSwitch			"#1# tells the raid#*#'CHSwitch #2#'"
#EVENT CHSwitch 		"[ #1# #*#]#*#CHSwitch #2#"

|-- Celestial Regeneration Events
#EVENT RegenerateCRList "#*# RegenerateCRList#*#"

#EVENT CelRegenStart	"#1# tells you#*#'CRStart#*#'"
#EVENT CelRegenStart	"#1# tells the group#*#'CRStart#*#'"
#EVENT CelRegenStart	"#1# tells the raid#*#'CRStart#*#'"
#EVENT CelRegenStart	"[ #1# #*#]#*#CRStart#*#"

#EVENT CelRegenStop		"#1# tells you#*#'CRStop#*#'"
#EVENT CelRegenStop		"#1# tells the group#*#'CRStop#*#'"
#EVENT CelRegenStop		"#1# tells the raid#*#'CRStop#*#'"
#EVENT CelRegenStop 	"[ #1# #*#]#*#CRStop#*#"

|-- Totem Events 
#EVENT FireTotem		"#1# tells you#*#'FireTotem #2#'"
#EVENT FireTotem		"#1# tells the group#*#'FireTotem #2#'"
#EVENT FireTotem		"#1# tells the raid#*#'FireTotem #2#'"
#EVENT FireTotem 		"[ #1# #*#]#*#FireTotem #2#"

|-- Regen Events (Celestial Regen [CLR], Spirit of Wood [DRU], Ancestral Aid [SHM] )
#EVENT FireRegen		"#1# tells you#*#'FireRegen #2#'"
#EVENT FireRegen		"#1# tells the group#*#'FireRegen #2#'"
#EVENT FireRegen		"#1# tells the raid#*#'FireRegen #2#'"
#EVENT FireRegen 		"[ #1# #*#]#*#FireRegen #2#"

#EVENT CheckHoTOnTarget "#*#CheckHoTOnTarget#*#"


Sub initEvents_Healing
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[MELEE_MANA]} ) /call RegisterBackgroundEvents Events_Healing 6
	/if (${str_classArchetype.Equal[PRIEST]}) 	/call RegisterRaidEvents Events_Healing_PriestsOnly 7
	/if (${Me.Class.ShortName.Equal[CLR]})		/call RegisterRaidEvents Events_Healing_ClericsOnly 2
/return

Sub Events_Healing
	/doevents GroupHeal
	/doevents ChangeHP
	/doevents ChangeHPTank
	/doevents ChangeHPSelf
	/doevents ChangeHPGroup
	/doevents HealMode
	/doevents HealType
	/doevents HealPoints
	/doevents InterruptON
	/doevents InterruptOFF
	/doevents FireTotem
	/doevents FireRegen
	/doevents CheckHoTOnTarget
	/doevents FocusHEALMT_ON
	/doevents FocusHEALMT_OFF
/return

Sub Events_Healing_ClericsOnly
	/if (${bln_hasCelRegen}) {
		/doevents CelRegenStart
		/doevents CelRegenStop
		/if (${bln_celRegenActive}) /call chkCelRegen
	} 
/return

Sub Events_Healing_PriestsOnly
	/doevents CHStart
	/doevents CHStop
	/doevents CHPause
	/doevents CHResume
	/doevents CHSwitch
/return 

Sub EVENT_FocusHEALMT_OFF
	/echo \aoFocus Healing MT \agOFF
	/varset bln_focusHealMT FALSE
	/call ChangeHealType 2
	/doevents flush FocusHEALMT_OFF
/return

Sub EVENT_FocusHEALMT_ON
	/echo \aoFocus Healing MT \agON
	/varset bln_focusHealMT TRUE
	/call ChangeHealType 1
	/doevents flush FocusHEALMT_ON
/return

Sub EVENT_CheckHoTOnTarget
	/call chkTargetHoT
	/doevents flush CheckHoTOnTarget
/return 

Sub chkTargetHoT
	/declare perTick int local 0
	/echo \aw Checking HoT on Target \ao${Target.Name} \aw: \ay"Me.SPA[100]"
	/dquery ${str_MTName} -q "Me.SPA[100]"  -o perTick
	/delay 120 ${DanNet.Q.Received}
	/echo \arReturned: ${Bool[${perTick}]}
/return ${perTick}

Sub EVENT_GroupHeal
	/echo Blasting Group Heal!
	/call MQ2Cast "${str_heal_group}" gem1 5s
	/doevents flush GroupHeal
/return

Sub EVENT_ChangeHP(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agHP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_tank_normal ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tank_normal}
	}
	/doevents flush ChangeHP
/return

Sub EVENT_ChangeHPTank(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agTank HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_tank_normal ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tank_normal}

	}
/return 

Sub EVENT_ChangeHPSelf(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agSelf HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_self_normal ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Me.CleanName} <<  @ ${int_point_self_normal}

	}
/return 

Sub EVENT_ChangeHPGroup(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agGroup HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_group_normal ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> GROUP <<  @ ${int_point_group_normal}
	}
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
Sub EVENT_HealMode(_line, _chatSender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/call ChangeHealMode ${_eventParams}
	/doevents flush HealMode
/return


Sub ChangeHealMode(string _strMode)
	|- Mem the appropriate spell set
	/if (${_strMode.Find[CHCHAIN]}) {
		/echo \a-wChanging healing \aoMODE \a-wto \agDEFAULT
		/varset str_currentSpellSet default
		/varset str_healMode CHCHAIN
		/call InterruptOFF
	} else /if (${_strMode.Find[EFFICIENT]}) {
		/echo \a-wChanging healing \aoMODE \a-wto \agEFFICIENT
		/varset str_currentSpellSet efficient
		/varset use_heal_HoT FALSE
		/varset str_healMode EFFICIENT
		/varset int_range_current_frantic	${int_range_frantic_efficient}
		/varset int_range_current_normal 	${int_range_normal_efficient}
		/call InterruptON
	} else {
		/echo \a-wChanging healing \aoMODE \a-wto \agDEFAULT
		/varset str_currentSpellSet default
		/varset use_heal_HoT TRUE
		/varset str_healMode DEFAULT
		/varset int_range_current_frantic	${int_range_frantic_efficient}
		/varset int_range_current_normal 	${int_range_normal_efficient}
		/call InterruptON
	}
	/memspellset ${str_currentSpellSet}
/return

Sub EVENT_HealPoints(_line, _chatSender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/call ChangeHealPoints ${_eventParams}
	/doevents flush HealMode
/return

Sub ChangeHealPoints(string _strHealPointKey)
	/echo \a-wChanging healing \aoMODE \a-wto \ag${_strHealPointKey}
	/if (${_strHealPointKey.Find[OMM]}) {
		/varset int_point_self_normal 	50
		/varset int_point_self_frantic	30
		/varset int_point_self_HoT		90
		/varset int_point_tank_normal	85
		/varset int_point_tank_frantic	30
		/varset int_point_tank_HoT		90
		/varset int_point_group_normal	50
		/varset int_point_group_frantic	30
		/varset int_point_group_HoT		90
		/varset use_heal_HoT TRUE
	} else {
		/varset int_point_self_normal 	${Ini[${healINI},Heal_Points,int_point_self_normal]}
		/varset int_point_self_frantic	${Ini[${healINI},Heal_Points,int_point_self_frantic]}
		/varset int_point_self_HoT		${Ini[${healINI},Heal_Points,int_point_self_HoT]}
		/varset int_point_tank_normal	${Ini[${healINI},Heal_Points,int_point_tank_normal]}
		/varset int_point_tank_frantic	${Ini[${healINI},Heal_Points,int_point_tank_normal]}
		/varset int_point_tank_HoT		${Ini[${healINI},Heal_Points,int_point_tank_normal]}
		/varset int_point_group_normal	${Ini[${healINI},Heal_Points,int_point_tank_normal]}
		/varset int_point_group_frantic	${Ini[${healINI},Heal_Points,int_point_tank_normal]}
		/varset int_point_group_HoT		${Ini[${healINI},Heal_Points,int_point_tank_normal]}
	}
/return

|**
 ----------------------------
 HealType
 ----------------------------
 Healing type changes the index of int_healMode
 0-Self (only) | 1-MT (and self) | 2-Group (and MT, includes self) | 3-GroupOnly (includes self) | 
 ----------------------------
**|
Sub EVENT_HealType(_line, _chatSender, _eventParams)
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/call ChangeHealType ${_eventParams}
	/doevents flush HealMode
/return

Sub ChangeHealType(int _type) 
	/varset int_healMode ${Int[${_type}]}
	/if (${int_healMode}==0) /echo \a-wChanging healing \aoTYPE \a-wto \ag0-Self
	/if (${int_healMode}==1) /echo \a-wChanging healing \aoTYPE \a-wto \ag1-MT (and self)
	/if (${int_healMode}==2) /echo \a-wChanging healing \aoTYPE \a-wto \ag2-Group (and MT, includes self)
	/if (${int_healMode}==3) /echo \a-wChanging healing \aoTYPE \a-wto \ag3-GroupOnly (includes self)
/return

|**
 ----------------------------
 TOTEM EVENT HANDLERS
 ----------------------------
	Handles firing groups of totems by key.
	Totems includes: 
	CLR	Exquisite Benediction
	DRU	Nature's Boon
	SHM	Call of the Ancients
**|
Sub EVENT_FireTotem(_line, _chatSender, _eventParams)
	/if (!${bln_hasTotem}) /return
	/call PlaceTotem ${Int[${_eventParams}]}
/return

Sub PlaceTotem(int _totemKey)
	/echo \agPlaceTotem \ao${_totemKey} \aw? \ao${int_totemKey}
	/if (${_totemKey} == ${int_totemKey} || ${_totemKey} == 0) {
		/call MQ2Cast "${str_totemName}" alt
		/docommand /${announceChannel} Totem: ${str_totemName} Placed
	} 
/return 


|**
 ----------------------------
 CELESTIAL REGENERATION EVENT HANDLERS
 ----------------------------
	Celestial Regen chain is nice to have for events with massive, consecutive AEs
	such as Mastery of Specialization, etc. The chain events are generally unused
	because I have found it better to just to fire regens via key like we do totems

	AE Regen includes: 
	CLR	Celestial Regeneration
	DRU	Spirit of the Wood
	SHM	Ancestral Aid

**|

Sub EVENT_FireRegen(_line, _chatSender, _eventParams)
	/if (!${bln_hasRegen}) /return 
	/call PlaceRegen ${Int[${_eventParams}]}
/return

Sub PlaceRegen(int _regenKey) 
	/echo \agPlaceRegen \ao${_regenKey} \aw? \ao${int_regenKey}
	
	/if (${Me.Class.ShortName.Equal[CLR]}) {
			/if (${Me.Song[Celestial Regeneration].Duration.TotalSeconds} > 10) /return
			/varset bln_celRegenActive TRUE
			/call chkCelRegen
			/varset bln_celRegenActive FALSE
		} else {
		/if (${_regenKey} == ${int_regenKey} || ${_regenKey} == 0) {
			/if (${Me.Song[${str_regenName}].Duration.TotalSeconds} > 10) /return
			/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[${str_regenName}]}) {
				/call MQ2Cast "Mass Group Buff" alt
				/delay 10
				/call MQ2Cast "${str_regenName}" alt
				/docommand /${announceChannel} AE Regen: ${str_regenName} Fired!
				/return
			}

			/if (${Me.AltAbilityReady[${str_regenName}]}) {
				/call MQ2Cast "${str_regenName}" alt
				/docommand /${announceChannel} AE Regen: ${str_regenName} Fired!
			}
		} 
		}
/return




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
	/delay 20 ${DanNet[${pp}].Query["Me.AltAbilityReady[Celestial Regeneration]"].Received}
	/if (!${Bool[${pMGB}]} || !${Bool[${pMGB}]}) {
		/echo \ar${pp}:\arDOES NOT \awhave MGB and Celestial Regen Ready. \agFiring!
		/goto :FireCelRegen
	} else {
		/echo \ag${pp}:\aw \agHAS \awMGB and Celestial Regen Ready. \agWaiting...
		|-- give the correct cleric a moment to fire their stuff
		/varset timer_celregen 5s
		/goto :return
	}
	
	:FireCelRegen
	/docommand /${announceChannel} AE Regen: Celestial Regeneration Fired!
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
			/echo \awprevPriest[${i}]: \ar${prevPriest} \awMGB:${c1}${pMGB}\aw Regen:${c2}${pCR}
		} else {
			/echo \awprevPriest[${i}]: \ag${prevPriest} \awMGB:${c1}${pMGB}\aw Regen:${c2}${pCR}
		}

	:nextPosition
	/next i

/return

|**
----------------------------
Check|Set the current heal spell
Note: this now checks to see what healing mode we are in.
Note: targetType is a key into the actual spell to set, ie: tank, group, self
----------------------------  
**|	
Sub setHealingSpell(int _hp, string _targetType, int _range, string _targetName, string _targetID)
	/varset str_currentHeal ${str_heal_normal}
	/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_normal_efficient}

	/declare pt int local ${Math.Calc[${int_point_${_targetType}_HoT}*1.05]}
	|/echo \awsetHealingSpell \ao${_targetType} \aw(\ag${_hp} \ao<> \ag${pt}\aw) \ar${Bool[${_hp}]} || ${Me.PctMana} == 0
	
	/declare response string local CASTOK
	/declare perTick int local 0
	/if (!${Defined[timer_HoT_${_targetID}]}) /declare timer_HoT_${_targetID} timer outer 0

	| NOT REQUIRED
	/if (!${_hp} || ${Me.PctMana} == 0 || (${_hp} > ${pt}))  {
		/varset response  NOHEAL
		/goto :return
	| FRANTIC
	} else /if (${_hp} <= ${int_point_${_targetType}_frantic}) {
		/varset str_currentHeal ${str_heal_frantic}
		/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_frantic_efficient}
		|/echo  \aoFRANTIC \ag(${str_healMode})  \ar${str_currentHeal} \awon \ag ${_targetName}\aw(\ay${_targetID}\aw)
		/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_${_targetType}_frantic}*1.1]}
		/varset response  CASTOK
		/goto :return
	| NORMAL
	} else /if (${_hp} <= ${int_point_${_targetType}_normal}) {
		/varset str_currentHeal ${str_heal_normal}
		/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_normal_efficient}
		|/echo \aoNORMAL \ag(${str_healMode})   \ay${str_currentHeal} \awon \ag ${_targetName}\aw(\ay${_targetID}\aw)
		/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_${_targetType}_normal}*1.1]}
		/varset response CASTOK
		/goto :return
	| HOT TODO: Add a timer for the remaining HoT duration so as to not spam via dquery
	} else /if (${_hp} <= ${int_point_${_targetType}_HoT} && ${use_heal_HoT} &&  !${timer_HoT_${_targetID}}) {
		/varset str_currentHeal ${str_heal_normal}
		/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_normal_efficient}

		/if (!${use_heal_HoT}) {
			/varset response CANTUSE
			/goto :return
		} 
		/if ((${Me.CurrentMana} < ${Spell[${str_heal_HoT}].Mana})) {
			/varset response OUTOFMANA
			/goto :return
		} 		
		/if (${Spawn[id ${_targetID}].Distance3D} > ${int_range_HoT}) {
			/varset response OUTOFRANGE
			/goto :return
		}	

		| Query if HoT is currently running
		/dquery ${_targetName} -q "Me.SPA[100]" -o perTick
		/delay 120 ${DanNet.Q.Received}
		|/echo \aoPerTick(\ag${perTick}\ao) \ar< \ag${int_heal_HoT_perTick}
		/if (${perTick} < ${int_heal_HoT_perTick}) {
			/varset str_currentHeal ${str_heal_HoT}
			|/echo  \aoHoT \ag(${str_healMode}) \ay${str_currentHeal} \awon \ag ${_targetName}\aw(\ay${_targetID}\aw)
			/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_${_targetType}_HoT}*1.2]}
			/varset response CASTOK
			/goto :return
		} else {
			/varset str_currentHeal ${str_heal_normal}
			/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_normal_efficient}
			|/echo  \aoHoT FAILED - ALREADY HAS HoT! \ag(${str_healMode}) \ag ${_targetName}\aw(\ay${_targetID}\aw)
			/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_${_targetType}_normal}*1.2]}
			/varset response ALREADY_HOT
			/goto :return
		}
	} else {
		/varset response  NOHEAL
	}
	:return 
	|/echo \aosetHealingSpell:\ag${response} \ao${str_currentHeal}
/return ${response}


|**
 ----------------------------
 Check Health Subroutines
 ----------------------------
 These three subroutines are called in order from the Class Macro MainLoop
 checksMT	(and heals)
 checksSELF	(and heals)
 checksGRP	(and heals)
 Healing type changes the index of int_healMode
 0-Self (only) | 1-MT (and self) | 2-Group (and MT, includes self) | 3-GroupOnly (includes self) 
 ----------------------------
**|

Sub chkHeal_MT
	/if ((${int_healMode} == 1 || ${int_healMode} == 2) && ${Spawn[id ${int_MTID}].PctHPs} < ${int_point_tank_HoT} && (${Spawn[id ${int_MTID}].Distance3D} <= ${int_range_current_normal})) /call healMAINTANK
/return

Sub chkHeal_Self
	/if ((${int_healMode} == 0 || ${int_healMode} == 2) && ${Me.PctHPs} < ${int_point_self_HoT}) /call healSELF
/return

Sub chkHeal_Group
	/if ((${int_healMode} == 2 || ${int_healMode} == 3) && ${WorstHurt.PctHPs} < ${int_point_group_HoT}) /call healGRP
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
	/call chkMTAlive TRUE
	/call setHealingSpell ${Spawn[id ${int_MTID}].PctHPs} tank ${Spawn[id ${int_MTID}].Distance3D} ${Spawn[id ${int_MTID}].CleanName} ${int_MTID}
	/if (${Macro.Return.Equal[CASTOK]}) {
		/echo \aghealMAINTANK:\ao${str_MTName}\aw(\ay${int_MTID}\aw) \aw=>\ag${Target.Name}  \ao${str_currentHeal}
		/call healCast ${int_MTID} ${Target.Name} "${str_currentHeal}" FALSE
	}
	/if (${Me.Class.ShortName.NotEqual[CLR]} && (${Target.ID} != ${int_targetID})) /call TrueTarget ${int_targetID}
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
	/call setHealingSpell ${Me.PctHPs} self 0 ${Me.Name} ${Me.ID}
	/if (${Macro.Return.Equal[CASTOK]}) {
		/echo \aghealSELF:\ao${Me.Name}\aw(\ay${Me.ID}\aw) \aw=>\ag${Target.Name}  \ao${str_currentHeal}
		/call healCast ${Me.ID} ${Me.Name} "${str_currentHeal}" FALSE
	}
	/if (${Me.Class.ShortName.NotEqual[CLR]} && (${Target.ID} != ${int_targetID}))  /call TrueTarget ${int_targetID}
/return

|**
 ----------------------------
 HealGroup
 ----------------------------
 Healing Logic for the Group
 Determines which spell to cast, then casts it.
 	int_point_group_HoT=90 
	int_point_group_normal=70
	int_point_group_frantic=40
	arr_groupHP[${j},1] ${Group.Member[${i}].PctHPs}
	arr_groupHP[${j},2] ${Group.Member[${i}].Distance3D}
	arr_groupHP[${j},3] ${Group.Member[${i}].ID}
	arr_groupHP[${j},4] ${i}  - The index in the group of the member, so we can quickly set the name even for out of zone chars
 ----------------------------
**|
Sub healGRP
	/declare 	i 			 	int local	0
	/declare    n				int local   1
	/declare 	int_inRange		int local 	0
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_group_normal}*1.2]}
	/declare 	memName 		string local 
	/declare 	memID			string  local 0
	/declare 	perTick			int local 0
	/declare    castStatus		string local 


	:determineGroupDamage
	| Method updates the group array, in order of HP, and counts the major/minor hurt
	|/call updateGroupHPupdateGroupHPDEPRECATED
	/call determineHurtCounts

	| No one is hurt, return
	|/echo determineGroupDamage healGRP major: ${int_majorHurt} minor: ${int_minorHurt}
	/if (${int_majorHurt} == 0 && ${int_minorHurt} == 0) /goto :return

	| Determine if we should use group heal
	| Check the range of the majorhurt
	/if (${int_majorHurt} >=3 && (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[PAL]}) && ${use_heal_group} && (${Me.CurrentMana} > ${Spell[${str_heal_group}].Mana})) {
		/call updateGroupRangeCount ${int_range_group}
		/varset int_inRange ${Int[${Macro.Return}]}
		/if (${int_inRange}>=3) {
			/echo  \arGROUP  \ar${str_heal_group} \awon \ay${Me.Name}
			/call healCast ${Me.ID} ${Me.Name} "${str_heal_group}" TRUE
			/goto :return
		} else {
			/echo \arGROUP not fired not enough in range \aw(\ay${int_inRange} < 3\aw)
			/goto :CastSingleTargetHeals
		}
	} else /if (${int_minorHurt} >=3 && ${Me.Class.ShortName.Equal[CLR]} && ${use_heal_groupHoT} && (${Me.CurrentMana} > ${Spell[${str_heal_groupHoT}].Mana})) {
		/if (${timer_groupHoT}) /goto :CastSingleTargetHeals
		/call updateGroupRangeCount ${int_range_groupHoT}
		/varset int_inRange ${Int[${Macro.Return}]}
		/if (${int_inRange}>=3 ) {
			/echo  \apGROUP_HOT \ap${str_heal_groupHoT} \awon \ay${Me.Name}
			/call healCast ${Me.ID} ${Me.Name} "${str_heal_groupHoT}" TRUE
			/varset timer_groupHoT 40s
			/goto :return
		} else {
			/echo \apGROUP_HOT not fired not enough in range \aw(\ay${int_inRange} < 3\aw)
			/goto :CastSingleTargetHeals
		}
	} 

	:CastSingleTargetHeals
	| Note the worst hurt will be the first in the list, and then run the heal check with N incremented by 1
	| This is so we heal 1 person, check tank, self, heal 1 person, check tank, self.
	| The only time we skip to another person in group is if they are out of frantic range.
	| Replaced the for loop with MQ2WorstHurt on 01/31/23
	
	/varset memID ${Spawn[${WorstHurt[group,${n},200,FALSE]}].ID}
	/varset memName ${Spawn[pc id ${memID}].Name}
	
	| not pet and corpse
	/if (${Spawn[id ${memID}].Type.NotEqual[Pet]} && ${Spawn[id ${memID}].Type.NotEqual[Corpse]} && ${Spawn[id ${memID}].PctHPs} <= ${int_point_group_HoT} && ${Spawn[id ${memID}].Distance3D} < ${int_range_current_frantic}) {
		| hp checks
		|/echo \awCastSingleTargetHeals  \ag ${memName} \aw(\ay${memID}\aw)
		|(int _hp, string _targetType, int _range, string _targetName, string _targetID)
		/call setHealingSpell ${Spawn[id ${memID}].PctHPs} group ${Spawn[id ${memID}].Distance3D} ${memName} ${memID}
		/varset castStatus ${Macro.Return}
		|/echo \ao${castStatus}: \ag${str_currentHeal}
		/if (${castStatus.Equal[NOHEAL]}) {
			|/echo \ar${memName} NOHEAL! \aoReturning.
			/goto :return		
		} else /if (${castStatus.Equal[CASTOK]}) {
			/echo \awTrying to cast: \ag ${memName} \aw(\ay${memID}\aw) "${str_currentHeal}"
			/call healCast ${memID} ${memName} "${str_currentHeal}" FALSE 
			/goto :return
		} else /if (${castStatus.Equal[OUTOFRANGE]}) {
			/varcalc n ${n}+1
			/echo \ar${memName} OUTOFRANGE! \aoIncrementing N(=>${n}), and checking next most damaged.
			/goto :CastSingleTargetHeals
		} else /if (${castStatus.Equal[ALREADY_HOT]}) {
			/varcalc n ${n}+1
			/goto :CastSingleTargetHeals
		} else {
			/varcalc n ${n}+1
			/echo \ar${memName} ${castStatus}! \aoIncrementing N(=>${n}), and checking next most damaged.
			/goto :CastSingleTargetHeals
		}
	}
:return
	/if (${Me.Class.ShortName.NotEqual[CLR]} && (${Target.ID} != ${int_targetID})) /call TrueTarget ${int_targetID}
/return ${castStatus}


|-- Method to determine how many in the group need healing
Sub determineHurtCounts
	/declare 	i 			 		int local	0
	/varset int_minorHurt 0
	/varset int_majorHurt 0
	/for i 0 to 5
		/if (${Group.Member[${i}].Present} && (${Group.Member[${i}].PctHPs}< 100) && (${Group.Member[${i}].PctHPs}>=${int_point_group_normal})) /varcalc int_minorHurt ${int_minorHurt}+1 
		/if (${Group.Member[${i}].Present} && (${Group.Member[${i}].PctHPs}< 100) && (${Group.Member[${i}].PctHPs}< ${int_point_group_normal})) /varcalc int_majorHurt ${int_majorHurt}+1 
	/next i
/return


Sub updateGroupRangeCount(int _range) 
	/declare inRange int local 0 
	/declare k int local 0
	/declare member string local
	/for k 0 to 5
		/if (${Group.Member[${k}].Present} && ${Group.Member[${k}].Distance3D} <= ${_range}) /varcalc inRange ${inRange}+1
	/next k
/return ${inRange}

|**
 ----------------------------
 HealCasting
 ----------------------------
 Healing Cast pass through to Cast and spellroutines.inc
 This takes the set values from the above 3 logic routines and casts
 Calls chkHP while being cast in case we need to change
 ----------------------------
**|
Sub healCast(int _id, string _name, string _spell, bool _isGroup, bool _isHoT)
	/if (${_name.Equal[NULL]} || ${_id} == 0) {
		/echo \arHEAL NAME is \ao${_name} \ar|| ID is \ao${_id}\ar something is fucked;
		/return
	}
	/if ((${_isGroup} || (${Spawn[pc ${_name}].PctHPs} < ${int_point_calcStopHeal}) && (${Me.CurrentMana} > ${Spell[${_spell}].Mana}))) {
		/echo \aoHealCast \ay:IsGroup:\ag${_isGroup} \ayTarget:\ag${_name}(${_id}) \aydist:\ag${Target.Distance} \aytargetHP:\ag${Spawn[pc ${_name}].PctHPs}\ao < \ayStopHP:\ag${int_point_calcStopHeal} \ayspell:\ag"${_spell}" \ayMyMana:\ag ${Me.CurrentMana}\ao > \ag${Spell[${_spell}].Mana}
		/g HealCast Target:${_name}(${_id}) dist:${Target.Distance} targetHP:${Spawn[pc ${_name}].PctHPs} < StopHP:${int_point_calcStopHeal} spell:"${_spell}" MyMana:${Me.CurrentMana} SpellCost:${Spell[${_spell}].Mana}
		/call TrueTarget ${_id}
		/call chkSpellMemorized "${_spell}" TRUE 2
		/if (${_isGroup}) {
			| Don't check health on group spells or you will constantly duck out if cleric doesnt need to be healed!
			/call MQ2Cast "${_spell}" gem2 1s
		} else {
			/call MQ2Cast "${_spell}" gem2 1s chkHP
		}
		
		/delay 5
		/if (!${Defined[timer_HoT_${_id}]}) /declare timer_HoT_${_id} timer outer 30s
		/if (${_isHoT} && ${Macro.Return.Equal[CAST_SUCCESS]}) /varset timer_HoT_${_id} 30s
		/if (${Macro.Return.Equal[CAST_SUCCESS]} && ${bln_annouce}) /${cmdChannel} ${_spell} > ${Target.Name} hp:${Target.PctHPs} m:${Me.PctMana} d:${Target.Distance}
		/return ${Macro.Return}
	}
/return


|**
 ----------------------------
 Check Healing Subroutines
 ----------------------------
 This is called (looping) while a spell is being cast
 This will then call several check subs which initiate from the chain chkHP
 ----------------------------
**|
Sub chkHP
	| I'm no longer casting, or no longer need to stop it ie, target is MT
	/if (!${Bool[${Me.Casting.ID}]}) /goto :return

	| If the Target is Dead... stop
	/if (${Target.Name.Find[s_corpse]}) /goto :interrupt

	| Check to see if I should interrupt because target is healed
	/if ((${bln_autointerrupt}) && (${Target.PctHPs} > ${int_point_calcStopHeal}))  /goto :interrupt


	| Divine Arbitration Checks
   	| -------------------------
 	| Check to see if I should cast Divine Arbitration
	| Loop through group members to check to see if anyone needs Divine Arbitration
	/if (${Me.Class.ShortName.NotEqual[CLR]} || !${use_heal_divineArb} || !${bln_ArbitrationReady}) /goto :return

	:divine
	/declare gmember int local 0
	/for gmember 0 to ${Group.Members}
		/if (${Group.Member[${gmember}].PctHPs}<=${int_point_divineArb} && ${Group.Member[${gmember}].Present}) {
			| add a check for member in group here
			/dgt ${Group.Member[${gmember}].Name} triggered Divine Arbitration!
			/stopcast
			/call healDivineArbitration
			/goto :return
		}
	/next gmember

	:interrupt
	/stopcast

    :return
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

Sub chkDA 

/return


|**
 ------------------------
 CHCHAIN EVENT HANDLERS
	Note: CHCHAIN is only listened for by PRIEST classes
 ------------------------
**|
Sub EVENT_CHStart(_line, _sender, _eventParams)
	/echo \ayComplete Heal Start \ag${_eventParams}
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
	/echo \agStarting CH id:${_chSettings.Arg[1]} tank1:${_chSettings.Arg[2]} tank2:${_chSettings.Arg[3]} delay:${_chSettings.Arg[4]} order:${_chSettings.Arg[5]}  position:${_position}
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
	/call RunAllEvents
	:chLoop
		/if (${Spawn[pc ${tank1}].ID} != ${int_chTarget}) {
			/echo CHCHain must have switched! new tank! restart with proper delay!
			/goto :chStart
		}
		/if (!${bln_chChainMember}) /goto :return
		/if (!${Spawn[pc ${tank1}].PctHPs} && !${tank1.Equal[${tank2}]}) /varset tank1 ${tank2}
		/if (!${Spawn[pc ${tank1}].PctHPs}) {
			/echo \arCHChain ${int_chChainID} Cannot Find Tank of Last Resort ${tank2}
			/goto :return
		}
		/if (${Me.CurrentMana} < ${Math.Calc[${Spell[${str_heal_complete}].Mana}+20]}) {
			/echo \arCHChain ${int_chChainID} OOM
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
			/call RunAllEvents
			/if (!${bln_chChainMember}) /goto :return
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
	/echo \awPausing CH ${_eventParams}
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
	/echo \awResuming CH
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
	/echo \awStopping CH ${_eventParams}
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
	
	/echo \awSwitching CH ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
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
Sub chkRessurection

	| find all corpses in range	
	/if (${timer_rezCheck}) /return

	/if (${SpawnCount[corpse]}) /squelch /hidecorpse npc
	/delay 20
	/if (!${SpawnCount[corpse radius ${int_range_rez}]}) /return
	/echo \aoPerforming Rezzes
	| if corpses, then aerez
	/call RessurectCorpses ${int_corpses}
	/varset timer_rezCheck 30s
	/echo \agRessurections \awcomplete \ag${timer_rezCheck} \aountil recheck.
/return


Sub RessurectCorpses 
	/declare int_corpseCount 					int local ${SpawnCount[corpse radius ${int_range_rez}]}
	/declare arr_corpses[${int_corpseCount}]	int	local 0
	/declare i									int local 0
	/declare id									int local 0
	/declare charName							string local NULL
	/declare online  							int local 0

	| consider the corpses
	/for i 1 to ${int_corpseCount}
		/squelch /hidecorpse npc
		/delay 10
		/varset id 		 	${NearestSpawn[${i},corpse radius ${int_range_rez}].ID}
		/varset charName 	${Spawn[id ${id}].CleanName.Token[1,']}
		/varset online 		0
		/varset bln_skipCorpse FALSE
		/echo \ag Checking on \ao${charName}\ag for Rez
		| check to see if the pc is online
		/dquery ${charName} -q "Me.ID" -o online -t 20
		/if (!${online}) /goto :nextCorpse

		| target the corpse and check if cannot rez (conder and check result)
		/call TrueTarget ${id}
		/consider
		/delay 5
		/doevents CANNOTREZ 
		/if (${bln_skipCorpse}) /goto :nextCorpse

		| resurrect
		/call Ressurect ${id}
	:nextCorpse
	
	/if (${SpawnCount[corpse]}) /squelch /hidecorpse npc
	/delay 10
	/echo \aw Checking on next corpse.
	/next i
/return 


Sub Ressurect(int _id)
	/call TrueTarget ${_id}
	/dex  ${Spawn[id ${_id}].CleanName.Arg[1,']} /consent ${Me.Name}
	/delay 5
	/corpse 

	| Find my epic (if cleric)
	/if (${FindItem[Water Sprinkler of Nem Ankh].ID}) {
		/call MQ2Cast "Water Sprinkler of Nem Ankh" item
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
			/docommand /${announceChannel} Rezzed ${Target.Name}
		}
	} else {	
		| No epic or not cleric, use spell
		/call chkSpellMemorized "${str_rezSpell}" TRUE ${str_rezGem}
		/call MQ2Cast ${str_rezSpell}
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
			/docommand /${announceChannel} Rezzed ${Target.Name}
		}
	}
/return


Sub CheckOnline(string _name)
	/tell ${_name} Checking to see if you are online for rez....
	/doevents NOTONLINE
/return

Sub Event_NOTONLINE(Line, Name)
	| sent a tell on rez but got an offline message - interrupt cast
	/if (${Target.CleanName.Arg[1,'].Equal[${Name}]}) /call Interrupt
	/varset bln_skipCorpse TRUE
/return

Sub Event_CANNOTREZ
	/if (${Target.ID}) {
		/echo Could not rez ${Target.CleanName}, skipping this time
		/varset bln_skipCorpse TRUE
	}
/return



Sub EVENT_InterruptON
	/call InterruptON
/return

Sub EVENT_InterruptOFF
	/call InterruptOFF
/return

Sub InterruptON
	|---------------------
	| Return out for classes that can't heal
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[BRD]} || ${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]} || ${str_classArchetype.Equal[CASTER]})	/return
	|---------------------
	/echo \ayAutoInterrupt: \agON
	/varset bln_autointerrupt TRUE
/return

Sub InterruptOFF 
	|---------------------
	| Return out for classes that can't heal
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[BRD]} || ${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]} || ${str_classArchetype.Equal[CASTER]})	/return
	|---------------------
	/echo \ayAutoInterrupt: \arOFF
	/varset bln_autointerrupt FALSE
/return


Sub chkDivineIntervention
	/if (${Bool[${Me.Buff[Divine Intervention].ID}]} || ${timer_di_request}) /return
	/echo \agDivine Intervention \ar NOT found. \ayRequesting!
	| Check your first DI cleric
	/declare i int local 0
	/declare avail bool local FALSE
	/declare pcName string local
	/declare requestCount int local 0
	/for i 1 to ${RaidDIClerics.Size}
		/varset pcName ${RaidDIClerics[${i}]}
		/if (!${Defined[timer_di_${i}]}) /declare timer_di_${i} timer outer 0
		/echo \awChecking \ay${pcName} \aw for DI
		/if (${timer_di_${i}} || !${Bool[${SpawnCount[pc ${pcName}]}]}) {
			/echo \ar${pcName} \awhas a DI timer or is out of zone!
			/goto :next_cleric
		} 

		/if (${Spawn[pc ${pcName}].Distance3D} > 120) {
			/echo \ar${pcName} \awis out of DI Range!
			/goto :next_cleric
		}

		/varset requestCount 0
		:query_loop
			/varcalc requestCount ${requestCount}+1
			/dquery ${pcName} -q "Me.SpellReady[Divine Intervention]"
			/delay 20 ${DanNet.Query.Received}
			/varset avail ${DanNet.Q}
			/delay 10
			| give the cleric a second in case they just got done casting somehting or is casting something
			/if (!${avail} && ${requestCount} < 2) {
				/goto :query_loop
			}
			/if (${avail}) {
				/goto :request_di
			} else {
				/varset timer_di_${i} 30s
			}
	:next_cleric
	/echo \ar${pcName} \awdoes not have DI ready!
	/next i

	:no_cleric	
	/echo \arNO CLERICs \aw have DI ready!
	/varset timer_di_request 10s
	/goto :return

	:request_di
	/echo \awRequesting \agDivine Intervention\ay from \ag${pcName}
	/varset timer_di_request 10s
	/varset timer_di_${i} 30s 
	/dt ${pcName} dime
	
	:return
/return

Sub CreateGroupHoTTimers
	|-- Group HOT Timers
	/declare i int local 0
	/declare k int local 1
	/declare timerKey string local
	/for i 0 to 5
		/if (${Bool[${Group.Member[${i}].ID}]}) {
			/varset timerKey timer_HoT_${Group.Member[${i}].ID}
		} else {
			/varset timerKey timer_HoT_NULL_${i}
		}
		/declare ${timerKey}	timer 	outer 	1m
		/varset arr_hotTimerKeys[${k}] ${timerKey}
		/varset str_groupIDs ${str_groupIDs}|${Group.Member[${i}].ID}
		/varcalc k ${k}+1
	/next i

	/for i 1 to 6
		/echo \ag${i}: ${arr_hotTimerKeys[${i}]} ${${arr_hotTimerKeys[${i}]}}
	/next i

/return

Sub ReCreateGroupHoTTimers
	/declare i int local 1
	/for i 1 to 6
		/echo \ar Deleting \ao TimerKey ${i} ${arr_hotTimerKeys[${i}]}
		/deletevar ${arr_hotTimerKeys[${i}]}
	/next i
	
	/call CreateGroupHoTTimers
/return

|**
|----------------------------
| INITIALIZATION Subs
| ----------------------------
| Initialization of values for this specific libary
| ----------------------------
|**| 

Sub xbot_initialize_healing(int _healPoint)
	| Set some global default variables for all classes
	/declare healPoint int outer ${_healPoint}

	|---------------------
	| Return out for classes that can't heal
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[BRD]} || ${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]} || ${str_classArchetype.Equal[CASTER]})	/return
	|---------------------

	/declare 	bln_useHealRandomization 	bool	outer  	TRUE
	/declare 	int_healRandomRange			int		outer 	5
	/declare	int_healRandomMin			int     0
	/declare	int_healRandomMax			int     0
	/if (${bln_useHealRandomization} && ${Bool[${_healPoint}]}) {
		/varcalc int_healRandomMin	${_healPoint}-${int_healRandomRange}
		/varcalc int_healRandomMax	${_healPoint}+${int_healRandomRange}
		/varset  _healPoint ${Math.Rand[${int_healRandomMin},${int_healRandomMax}]}
		/echo \aoRandomized Healing \ag${int_healRandomMin} \ay- \ag${int_healRandomMax} \ayPoint: \ag${_healPoint}
	}


	/declare 	healINI				string  outer	xiris_includes/xiris_healing.ini
	|-- Set spellset to default so we can probe the properties of these
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[MELEE_MANA]}) /memspellset default

	|-- Heal Settings
	/declare    bln_defaultAutoInterrupt 	bool	outer  	${Ini[healINI,Heal_Settings,bln_autointerrupt]}
	/declare 	bln_autointerrupt			bool	outer	${Ini[INI_CHAR,Healing_Settings,bln_autointerrupt,${bln_defaultAutoInterrupt}]}
	
	| 	Heal Mode: NORMAL|EFFICIENT|CHCHAIN
	/declare    str_healMode				string  outer 	NORMAL
	/declare	str_savedHealMode			string  outer   ${str_healMode}
	| 	Heal Mode: 0|Self Only, 1|Self/MT 2|Self/MT/Group

	/declare 	int_defaultHealMode			int 	outer   ${Ini[${healINI},Heal_Settings,int_healMode]}
	/declare	int_healMode    			int     outer   ${Ini[INI_CHAR,Healing_Settings,int_healMode,${int_defaultHealMode}]}
	/declare 	bln_focusHealMT				bool    outer   FALSE
	/if (${int_healMode}<=1) 				/varset bln_focusHealMT	TRUE
	/echo \arint_healMode: \ag${int_healMode} ${int_defaultHealMode}

	/declare 	use_heal_defaultDivineArb	bool	outer 	${Ini[${healINI},Heal_Settings,use_heal_divineArb, FALSE]}
	/declare	use_heal_divineArb			bool	outer	${Ini[INI_CHAR,Healing_Settings,use_heal_divineArb,${use_heal_defaultDivineArb}]}
	
	|-- Healing Spells
	/declare 	str_heal_normal				string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_normal,NULL]}
	/declare 	str_heal_normal_efficient	string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_normal_efficient,${str_heal_normal}]}
	/declare 	str_heal_frantic			string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_frantic,${str_heal_normal}]}
	/declare 	str_heal_frantic_efficient 	string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_frantic_efficient,${str_heal_frantic}]}
	/declare 	str_heal_HoT				string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_HoT,NULL]}
	/declare 	str_heal_group				string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_group,NULL]}
	/declare 	str_heal_groupHoT			string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_groupHoT,NULL]}
	/declare 	str_heal_complete			string	outer	${Ini[INI_CHAR,Healing_Spells,str_heal_complete,NULL]}
	
	|-- Ressurection
	/declare	bln_autoRez					bool	outer	FALSE
	/declare 	bln_combatRez				bool	outer	FALSE
	/declare	str_rezSpell				string	outer	${Ini[INI_CHAR,Healing_Spells,str_rezSpell,Reviviscence]}
	/declare  	str_rezGem					string	outer	${Ini[INI_CHAR,Healing_Spells,str_rezGem,gem7]}
	/declare	timer_rezCheck				timer   outer   30s

	/echo ---------------
	/echo \ayHeal Spells: \awNormal:\ag${str_heal_normal}\aw  || Efficient:\ag${str_heal_frantic_efficient}\aw || CH:\ag${str_heal_complete}
	/echo				  \awFrantic:\ag${str_heal_frantic}\aw || Frantic Efficient:\ag${str_heal_frantic_efficient}\aw || HOT:\ag${str_heal_HoT}
	/echo                 \awGroup:\ag${str_heal_group}\aw || GroupHoT:\ag${str_heal_groupHoT}\aw || Rez:\ag${str_rezSpell}
	/echo ---------------

	|-- Heal Use (note, if null, or cannot find, we cannot use (gotta figure out how to tell if I have that spell...)
	/declare 	use_heal_normal			bool	outer	FALSE
		/if (${str_heal_normal.NotEqual[NULL]}) 	/varset use_heal_normal 	TRUE

	/declare 	use_heal_frantic		bool	outer	FALSE
		/if (${str_heal_frantic.NotEqual[NULL]}) 	/varset use_heal_frantic 	TRUE
		
	/declare 	use_heal_HoT			bool	outer	FALSE
		/if (${str_heal_HoT.NotEqual[NULL]}) 		/varset use_heal_HoT 		TRUE
		/declare int_heal_HoT_perTick	string  outer	0 
	/if (${use_heal_HoT} && ${str_heal_HoT.Find[Pious Elixir]}) /varset int_heal_HoT_perTick 1170
	/if (${use_heal_HoT} && ${str_heal_HoT.Find[Holy Elixir]}) /varset int_heal_HoT_perTick 900
	/if (${use_heal_HoT} && ${str_heal_HoT.Find[Spiritual Serenity]}) /varset int_heal_HoT_perTick 820	
	/echo \awUsing Hot? \ag${use_heal_HoT} \awSpell: \ag${str_heal_HoT} \awPerTick: \ag${int_heal_HoT_perTick}
	

	/declare 	use_heal_group			bool	outer	FALSE
		/if (${str_heal_group.NotEqual[NULL]}) 		/varset use_heal_group 		TRUE
		
	/declare 	use_heal_groupHoT		bool	outer	FALSE
		/if (${str_heal_groupHoT.NotEqual[NULL]}) 	/varset use_heal_groupHoT 	TRUE

	/echo \ayHeal Usages:  \awNormal:\ag${use_heal_normal}\aw || Frantic:\ag${use_heal_frantic}\aw || HoT:\ag${use_heal_HoT}\aw || Group:\ag${use_heal_group}\aw || GroupHoT:\ag${use_heal_groupHoT}\aw || AutoRez:\ag${bln_autoRez}\aw
	/echo ---------------

	|-- Heal Ranges
	/declare	int_range_normal_efficient int	outer	${Spell[${str_heal_normal_efficient}].MyRange}
	/declare	int_range_normal		int		outer	${Spell[${str_heal_normal}].MyRange}
	/declare	int_range_frantic		int		outer	${Spell[${str_heal_frantic}].MyRange}
	/declare	int_range_frantic_efficient		int		outer	${Spell[${str_heal_frantic_efficient}].MyRange}
	/declare	int_range_HoT			int		outer	${Spell[${str_heal_HoT}].MyRange}
	/declare	int_range_group			int		outer	70 
	/declare	int_range_groupHoT		int		outer	${Spell[${str_heal_groupHoT}].MyRange}
	/declare 	int_range_complete		int		outer 	${Spell[${str_heal_complete}].MyRange}
	/declare	int_range_rez			int		outer	100
	/declare 	int_range_divineArb		int		outer	100

	/declare    int_range_current_frantic    		int 	outer ${int_range_frantic}
	/declare    int_range_current_normal  			int 	outer ${int_range_normal}

	
	/echo \ayHeal Ranges: \awPatch:\ag${int_range_normal_efficient}\aw || Normal:\ag${int_range_normal}\aw || Group:\ag${int_range_group}\aw
	/echo  			Frantic:\ag${int_range_frantic}\aw || FranticEfficient:\ag${int_range_frantic_efficient} 
	/echo 				HoT:\ag${int_range_HoT}\aw || GroupHoT:\ag${int_range_groupHoT}\aw || Rez:\ag${int_range_rez}
	/echo ---------------
	
	|-- Heal Points
	|-- SELF (Defaults)
	/declare 	int_def_self_patch		int     outer   ${Ini[${healINI},Heal_Points,int_point_self_normal]}
	/declare 	int_def_self_frantic	int     outer   ${Ini[${healINI},Heal_Points,int_point_self_frantic]}
	/declare 	int_def_self_HoT		int     outer   ${Ini[${healINI},Heal_Points,int_point_self_HoT]}

	|-- SELF (Overrides)
	/declare	int_point_self_normal	int 	outer 	${Ini[INI_CHAR,Heal_Points,int_point_self_normal,${int_def_self_patch}]}
	/declare	int_point_self_frantic	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_self_frantic,${int_def_self_frantic}]}
	/declare	int_point_self_HoT		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_self_HoT,${int_def_self_HoT}]}

	|-- TANK (Defaults)
	/declare 	int_def_tank_patch		int     outer   ${Ini[${healINI},Heal_Points,int_point_tank_normal]}
	/declare 	int_def_tank_frantic	int     outer   ${Ini[${healINI},Heal_Points,int_point_tank_frantic]}
	/declare 	int_def_tank_HoT		int     outer   ${Ini[${healINI},Heal_Points,int_point_tank_HoT]}

	|-- TANK (Overrides)
	/declare	int_point_tank_normal	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_tank_normal,${int_def_tank_patch}]}
	/if (${Bool[${_healPoint}]}) {
		/echo \agHealpoint setting overriding defaults: ${_healPoint}	
		/varset	int_point_tank_normal	${_healPoint}
	} else {
		/echo No healpoint defined using INI values
	}
	/declare	int_point_tank_frantic	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_tank_frantic,${int_def_tank_frantic}]}
	/declare	int_point_tank_HoT		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_tank_HoT,${int_def_tank_HoT}]}

	|-- Rest of Group (Defaults)
	/declare 	int_def_group_normal	int     outer   ${Ini[${healINI},Heal_Points,int_point_group_normal]}
	/declare 	int_def_group_frantic	int     outer   ${Ini[${healINI},Heal_Points,int_point_group_frantic]}
	/declare 	int_def_group_HoT		int     outer   ${Ini[${healINI},Heal_Points,int_point_group_HoT]}

	|-- Rest of Group (Overrides)
	/declare	int_point_group_normal	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_group_normal,${int_def_group_normal}]}	
	/declare	int_point_group_frantic	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_group_frantic,${int_def_group_frantic}]}
	/declare	int_point_group_HoT		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_group_HoT,${int_def_group_HoT}]}	

	|-- Divine Arbitration && Misc
	/declare    int_point_divineArb     int		outer	${Ini[INI_CHAR,Heal_Points,int_point_divineArb,20]}
	/declare    int_point_stopHeal    	int		outer   ${Ini[INI_CHAR,Heal_Points,int_point_stopHeal,90]}	
	/declare	int_point_calcStopHeal	int		outer	100
	/declare    use_interrupt_selfHeal  bool	outer   TRUE

	|-- Celestial Regeneration Chain
	/declare bln_hasCelRegen 					bool    outer   ${Me.Class.ShortName.Equal[CLR]}
	/declare int_celRegenPosition			int		outer   99
	/if (${bln_hasCelRegen}) {
		/declare bln_celRegenActive 			bool	outer	FALSE 
		/declare int_celRegenTotal 				int 	outer	${Ini[${healINI},CelestialRegeneration,int_totalCR]}
		/declare arr_celRegen[${int_celRegenTotal}] string outer
		/declare timer_celregen					timer   outer	0
		|   Parse the chain's character list to an array
		/varset int_celRegenPosition			int		outer   1
		/declare str_celRegenPPriest            string  outer   NONE
		/call setCelRegenChain

		/echo \a-w---------------
		/echo \aoCelestial Regeneration:\ag${bln_hasCelRegen} \aw|| \aoTotal Chars:\ag${int_celRegenTotal} \aw|| \aoPosition:\ag${int_celRegenPosition} \aw|| \aoPrevious::\ag${str_celRegenPPriest}
		/echo \a-w---------------
	}

	|-- AE Regens (DRU/SHM/CLR)
	/declare    bln_hasRegen			bool    outer  ${Ini[INI_CHAR,AE_REGEN,bln_hasRegen,${Me.Class.ShortName.Equal[CLR]}]}
	/declare	str_regenName			string  outer  ${Ini[INI_CHAR,AE_REGEN,str_regenName,NULL]}
	/declare	int_regenKey			int 	outer  ${Ini[INI_CHAR,AE_REGEN,int_regenKey,${int_celRegenPosition}]}	

	|-- Totems
	/declare    bln_hasTotem			bool    outer  ${Ini[INI_CHAR,TOTEM,bln_hasTotem,${Me.Class.ShortName.Equal[CLR]}]}
	/declare    str_totemName			string  outer  ${Ini[INI_CHAR,Totems,str_totemName,NULL]}	
	/declare	int_totemKey			int 	outer  ${Ini[INI_CHAR,Totems,int_totemKey,99]}	


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
	/declare 	arr_rezIgnore[1]		int		outer   0
	/declare 	bln_skipCorpse			bool	outer	FALSE
	/declare 	timer_groupHoT  		timer	outer	0

	|-- Group HitPoints 1: HP 2:Dist3D 3:ID 4: CleanName
	/declare	arr_groupHP[6,4]		int     outer   100   
	/declare	int_minorHurt			int 	outer	0
	/declare	int_majorHurt	   		int 	outer	0
	/declare 	str_groupIDs            string  outer   
	/declare 	arr_hotTimerKeys[6]     string  outer   NULL

	|/call CreateGroupHoTTimers

	|-- OLD Healing Spells
	/declare    bln_rezzable			bool    outer   TRUE
	
	/declare	bln_MTDead				bool	outer   FALSE
	/declare	bln_TRGTDead			bool	outer	FALSE


	|-- Hard Coded Complete Heal Spells
	/declare 	str_completeHeal		string	 outer
	/if (${Me.Class.Name.Equal[Cleric]}) /varset str_completeHeal Complete Heal
    /if (${Me.Class.Name.Equal[Druid]})  /varset str_completeHeal Karana's Renewal
    /if (${Me.Class.Name.Equal[Shaman]}) /varset str_completeHeal Kragg's Mending
	/declare	str_chGem				string	outer   ${Ini[INI_CHAR,Healing_Spells,int_grpHealRNG,gem9]}
	
	
	|-- Complete Healing Arguments
	/declare CHealRunning 	bool 	outer FALSE
    /declare CHealTank 		string	outer
    /declare CHealTankTwo 	string	outer
    /declare CHealDelay		string 	outer
    /declare CHealNext		string	outer
	


	

	/call initEvents_Healing
	/echo \ao xbot_initialize_healing ${int_point_tank_normal}
/return




Sub processHealing
	| Heals/Cures are not laid out in INI like the rest of the elements.
	| This is due to their complexity. We can however check for the 
	| NULL settings element to bypass it all
	/if (${Select["${Ini[INI_CHAR,HEALS,settings]}",NULL]} == 1 || ${Select[${Me.Class.ShortName},WAR,SHD,ROG,MNK,BER,NEC,WIZ,MAG]}) {
		/echo \aoprocessHealing: \arNo heals created!
		/return
	}

	| We have heals, since the settings is not NULL
	| Create the setup values from the settings entry
	| _mode|_useGroup|_useHoT|_announce|_autointerrupt|_defaultGem|_interruptToSaveSelf
	/declare bln_heals 				bool	outer FALSE
	/declare bln_divineArb			bool	outer FALSE
	| Set up a default value for the spell set NORMAL|EFFICIENT
	/declare str_heal_spellSet		string	outer NORMAL
	| SETTINGS
	/declare int_heal_mode 			int		outer ${Ini[INI_CHAR,HEALS,settings].Arg[1,|]}
	/declare bln_heal_useGroup		bool 	outer ${Ini[INI_CHAR,HEALS,settings].Arg[2,|]}
	/declare bln_heal_useHoT		bool 	outer ${Ini[INI_CHAR,HEALS,settings].Arg[3,|]}
	/declare bln_heal_announce		bool 	outer ${Ini[INI_CHAR,HEALS,settings].Arg[4,|]}
	/declare bln_heal_interrupt		bool 	outer ${Ini[INI_CHAR,HEALS,settings].Arg[5,|]}
	/declare int_heal_defaultGem	int 	outer ${Ini[INI_CHAR,HEALS,settings].Arg[6,|]}
	/declare int_heal_saveSelf		bool 	outer ${Ini[INI_CHAR,HEALS,settings].Arg[7,|]}

	| HEAL POINT Declarations
	| Default heal INI settings
	/declare 	healINI					string  outer	xiris_includes/xiris_healing.ini

	|-- SELF (Defaults)
	/declare 	int_def_self_normal		int     outer   ${Ini[${healINI},Heal_Points,int_point_self_normal]}
	/declare 	int_def_self_frantic	int     outer   ${Ini[${healINI},Heal_Points,int_point_self_frantic]}
	/declare 	int_def_self_HoT		int     outer   ${Ini[${healINI},Heal_Points,int_point_self_HoT]}
	
	|-- SELF (Overrides)
	/declare	int_point_self_normal	int 	outer 	${int_def_self_normal}
	/declare	int_point_self_frantic	int		outer	${int_def_self_frantic}
	/declare	int_point_self_HoT		int		outer	${int_def_self_HoT}

	|-- TANK (Defaults)
	/declare 	int_def_tank_normal		int     outer   ${Ini[${healINI},Heal_Points,int_point_tank_normal]}
	/declare 	int_def_tank_frantic	int     outer   ${Ini[${healINI},Heal_Points,int_point_tank_frantic]}
	/declare 	int_def_tank_HoT		int     outer   ${Ini[${healINI},Heal_Points,int_point_tank_HoT]}

	|-- TANK (Overrides)
	/declare	int_point_tank_normal	int		outer	${int_def_tank_normal}
	/declare	int_point_tank_frantic	int		outer	${int_def_tank_frantic}
	/declare	int_point_tank_HoT		int		outer	${int_def_tank_HoT}

	|-- Rest of Group (Defaults)
	/declare 	int_def_group_normal	int     outer   ${Ini[${healINI},Heal_Points,int_point_group_normal]}
	/declare 	int_def_group_frantic	int     outer   ${Ini[${healINI},Heal_Points,int_point_group_frantic]}
	/declare 	int_def_group_HoT		int     outer   ${Ini[${healINI},Heal_Points,int_point_group_HoT]}
	/declare 	int_def_group_groupHoT	int     outer   ${Ini[${healINI},Heal_Points,int_point_group_HoT]}

	|-- Rest of Group (Overrides)
	/declare	int_point_group_normal	int		outer	${int_def_group_normal}
	/declare	int_point_group_frantic	int		outer	${int_def_group_frantic}
	/declare	int_point_group_HoT		int		outer	${int_def_group_HoT}
	/declare    int_point_group_groupHoT int	outer 	${int_def_group_groupHoT}

 
	| Set each element based on the (8)_type keyes here, no loops required since the keys are unique
	| NORMAL
	/declare use_heal_normal	bool	outer	FALSE
	/if (${Ini[INI_CHAR,HEALS,HEAL_normal].NotEqual[NULL]}) {
		/varset 	use_heal_normal 			TRUE
		/declare 	str_heal_normal				string outer ${Ini[INI_CHAR,HEALS,HEAL_normal].Arg[1,|]}
		/declare 	str_heal_normal_efficient	string outer ${Ini[INI_CHAR,HEALS,HEAL_normal_efficient].Arg[1,|]}
		/declare 	int_range_normal 			int	${Spell[${str_heal_normal}].MyRange}
		/declare 	int_range_normal_efficient 	int	${Spell[${str_heal_normal_efficient}].MyRange}
		/varset		int_point_self_normal	    ${Ini[INI_CHAR,HEALS,HEAL_normal].Arg[2,|]}
		/varset		int_point_tank_normal	    ${Ini[INI_CHAR,HEALS,HEAL_normal].Arg[3,|]}
		/varset		int_point_group_normal	    ${Ini[INI_CHAR,HEALS,HEAL_normal].Arg[4,|]}
	} 

	| FRANTIC
	/declare  use_heal_frantic	bool	outer	FALSE
	/if (${Ini[INI_CHAR,HEALS,HEAL_frantic].NotEqual[NULL]}) {
		/varset 	use_heal_frantic 			TRUE
		/declare 	str_heal_frantic			string outer ${Ini[INI_CHAR,HEALS,HEAL_frantic].Arg[1,|]}
		/declare 	str_heal_frantic_efficient	string outer ${Ini[INI_CHAR,HEALS,HEAL_frantic_efficient].Arg[1,|]}
		/declare 	int_range_frantic 			int	${Spell[${int_range_frantic}].MyRange}
		/declare 	int_range_frantic_efficient int	${Spell[${int_range_frantic_efficient}].MyRange}
		/varset		int_point_self_frantic	    ${Ini[INI_CHAR,HEALS,HEAL_frantic].Arg[2,|]}
		/varset		int_point_tank_frantic	    ${Ini[INI_CHAR,HEALS,HEAL_frantic].Arg[3,|]}
		/varset		int_point_group_frantic	    ${Ini[INI_CHAR,HEALS,HEAL_frantic].Arg[4,|]}
	} 

	| HEAL OVER TIME
	| Note: even if HoT is enabled, we use dNet to query the target
	| and check to see if our HoT is better than what is current
	| /dquery ${Target.Name} -q "Me.SPA[100]" -o perTick
	/declare  use_heal_HoT		bool	outer	${bln_heal_useHoT}
	/if (${Ini[INI_CHAR,HEALS,HEAL_HoT].NotEqual[NULL]}) {
		/varset 	use_heal_HoT 			TRUE
		/declare 	str_heal_HoT			string outer ${Ini[INI_CHAR,HEALS,HEAL_HoT].Arg[1,|]}
		/declare 	int_range_HoT 			int	${Spell[${str_heal_HoT}].MyRange}
		/varset		int_point_self_HoT	    ${Ini[INI_CHAR,HEALS,HEAL_HoT].Arg[2,|]}
		/varset		int_point_tank_HoT	    ${Ini[INI_CHAR,HEALS,HEAL_HoT].Arg[3,|]}
		/varset		int_point_group_HoT	    ${Ini[INI_CHAR,HEALS,HEAL_HoT].Arg[4,|]}

		| Can you believe that you cannot get the HoT amount from any TLO? I've looked.
		/declare    int_heal_HoT_perTick	int 100
		/if (${str_heal_HoT.Find[Pious Elixir]}) 		/varset int_heal_HoT_perTick 1170
		/if (${str_heal_HoT.Find[Holy Elixir]}) 		/varset int_heal_HoT_perTick 900
		/if (${str_heal_HoT.Find[Spiritual Serenity]}) 	/varset int_heal_HoT_perTick 820
	} 

	| GROUP 
	| Group HoT, there are no good group HoTs at this point, so we don't really need this outside of a skeleton
	|		   , if use hot is false, then group hot is false
	|          , if use hot is true, and group hot is null, then it is false
	/declare  use_heal_groupHoT		bool	outer	${bln_heal_useHoT}
	/if (${Ini[INI_CHAR,HEALS,HEAL_groupHoT].NotEqual[NULL]} && ${bln_heal_useGroup} ) {
		/declare	str_heal_groupHoT			string 	outer	${Ini[INI_CHAR,HEALS,HEAL_groupHoT].Arg[1,|]}
		/declare    int_range_groupHoT			int		outer   ${Spell[${str_heal_groupHoT}].AERange}
		/varset		int_point_group_groupHoT	int		outer	${Ini[INI_CHAR,HEALS,HEAL_groupHoT].Arg[2,|]}
	} else {
		/varset use_heal_groupHoT FALSE
	}
	| Group Heal, uses a lot of mana, but also cures some counters useful on raids with heavy AE or mark of death
	/declare use_heal_group			bool	outer	${bln_heal_useGroup}
	/if (${Ini[INI_CHAR,HEALS,HEAL_group].NotEqual[NULL]} && ${bln_heal_useGroup} ) {
		/varset 	use_heal_group 			TRUE
		/declare 	str_heal_group				string 	outer 	${Ini[INI_CHAR,HEALS,HEAL_group].Arg[1,|]}
		/declare 	int_range_group 			int		outer 	${Spell[${str_heal_group}].AERange}
		/declare	int_point_self_group	    int 	outer 	${Ini[INI_CHAR,HEALS,HEAL_group].Arg[2,|]}
		/declare	int_point_tank_group	    int		outer	${Ini[INI_CHAR,HEALS,HEAL_group].Arg[3,|]}
		/declare	int_point_group_group	    int		outer	${Ini[INI_CHAR,HEALS,HEAL_group].Arg[4,|]}
	} else {
		/varset use_heal_group FALSE
	}
	| COMPLETE
	/declare use_heal_complete		bool	outer	FALSE
	/if (${Ini[INI_CHAR,HEALS,HEAL_complete].NotEqual[NULL]}) {
		/varset 	use_heal_complete 			TRUE
		/declare	str_heal_complete			string	outer	${Ini[INI_CHAR,HEALS,HEAL_complete].Arg[1,|]}
		/declare 	int_range_complete 			int 	outer	${Spell[${str_heal_complete}].MyRange}
	}

	|--- Cleric Specific
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /goto :Summary

	| RESSURECTION
	/declare use_heal_ressurection			bool	outer	FALSE
	/if (${Ini[INI_CHAR,HEALS,RESSURECTION_1].NotEqual[NULL]}) {
		/varset		use_heal_ressurection	TRUE
		/declare	str_heal_ressurection	string	outer	${Ini[INI_CHAR,HEALS,RESSURECTION_1].Arg[1,|]}
		/declare	int_range_ressurection	int		outer 	100
	}

	| DIVINE ARBITRATION
	| If divine arbitration has an entry, add the values here
	/declare use_heal_arbitration			bool 	outer 	FALSE
	/declare int_heal_arbitrationCount		int		outer	0
	/if (${Ini[INI_CHAR,HEALS,DIVINE_1].NotEqual[NULL]} && ${bln_heal_useGroup} ) {		
		/declare	use_heal_arbitration_1			bool	outer	${Ini[INI_CHAR,HEALS,DIVINE_1].Arg[3,|]} 
		/declare	str_heal_arbitration_1 			string	outer	${Ini[INI_CHAR,HEALS,DIVINE_1].Arg[1,|]}
		/declare 	str_heal_arbitration_1_type		string	outer	${Ini[INI_CHAR,HEALS,DIVINE_1].Arg[2,|]}
		/declare 	str_heal_arbitration_1_minHP	int		outer	${Ini[INI_CHAR,HEALS,DIVINE_1].Arg[4,|]}
		/if (${use_heal_arbitration_1}) /varcalc int_heal_arbitrationCount ${int_heal_arbitrationCount}+1
		
		/if (${Ini[INI_CHAR,HEALS,DIVINE_2].NotEqual[NULL]} && ${bln_heal_useGroup} ) {
			/varset  	int_heal_arbitrationCount	2
			/declare	use_heal_arbitration_2			bool	outer	${Ini[INI_CHAR,HEALS,DIVINE_2].Arg[3,|]} 
			/declare	str_heal_arbitration_2 			string	outer	${Ini[INI_CHAR,HEALS,DIVINE_2].Arg[1,|]}
			/declare 	str_heal_arbitration_2_type		string	outer	${Ini[INI_CHAR,HEALS,DIVINE_2].Arg[2,|]}
			/declare 	str_heal_arbitration_2_minHP	int		outer	${Ini[INI_CHAR,HEALS,DIVINE_2].Arg[4,|]}
			/if (${use_heal_arbitration_2}) /varcalc int_heal_arbitrationCount ${int_heal_arbitrationCount}+1
		}
		/if (${use_heal_arbitration_1} || ${use_heal_arbitration_2}) /varset use_heal_arbitration TRUE
	}


	:Summary
	| SUMMARY
	/echo ---------------
	/echo \ayHeal Settings: \awMode:\ag${int_heal_mode}\aw || UseGroup:\ag${use_heal_group}\aw || useHoT:\ag${use_heal_HoT}
	/echo 					\awAnnounce:\ag${bln_heal_announce}\aw || Interrupt:\ag${bln_heal_interrupt}\aw || SaveSelf:\ag${int_heal_saveSelf}
	/echo ---------------	 
	/echo \ayHeal Spells: \awNormal:\ag${str_heal_normal}\aw  || Efficient:\ag${str_heal_frantic_efficient}\aw || CH:\ag${str_heal_complete}
	/echo				  \awFrantic:\ag${str_heal_frantic}\aw || Frantic Efficient:\ag${str_heal_frantic_efficient}\aw || HOT:\ag${str_heal_HoT}
	/echo                 \awGroup:\ag${str_heal_group}\aw || GroupHoT:\ag${str_heal_groupHoT}\aw || Rez:\ag${str_rezSpell}
	/echo ---------------
	/echo \ayHeal Usages:  \awNormal:\ag${use_heal_normal}\aw || Frantic:\ag${use_heal_frantic}\aw || HoT:\ag${use_heal_HoT}\aw || Group:\ag${use_heal_group}\aw || GroupHoT:\ag${use_heal_groupHoT}\aw || AutoRez:\ag${use_heal_ressurection}\aw
	/echo ---------------
	/echo \ayHeal Ranges: \awPatch:\ag${int_range_normal_efficient}\aw || Normal:\ag${int_range_normal}\aw || Group:\ag${int_range_group}\aw
	/echo  			Frantic:\ag${int_range_frantic}\aw || FranticEfficient:\ag${int_range_frantic_efficient} 
	/echo 				HoT:\ag${int_range_HoT}\aw || GroupHoT:\ag${int_range_groupHoT}\aw || Rez:\ag${int_range_rez}
	/echo ---------------
	/echo \ayDivine Arbitration: \awCount:\ag${int_heal_arbitrationCount}\aw
	/echo ---------------
/return 