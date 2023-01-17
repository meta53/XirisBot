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
**|



|-- Settings and Triggers
#event GroupHeal  		"#*#DoGroupHeal#*#"
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
#event CanRez     		"This corpse can be resurrected."
#event CannotRez  		"This corpse cannot be resurrected."
#event NotOnline  		"You told #1#, '#2# is not online at this time'"
#event NotOnline  		"Your tell to #1# has been queued"

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
#Event FireTotem		"#1# tells you#*#'FireTotem #2#'"
#EVENT FireTotem		"#1# tells the group#*#'FireTotem #2#'"
#EVENT FireTotem		"#1# tells the raid#*#'FireTotem #2#'"
#EVENT FireTotem 		"[ #1# #*#]#*#FireTotem #2#"


#EVENT CheckHoTOnTarget "#*#CheckHoTOnTarget#*#"


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
	/doevents ChangeHPTank
	/doevents ChangeHPSelf
	/doevents ChangeHPGroup
	/doevents HealMode
	/doevents HealType
	/doevents InterruptON
	/doevents InterruptOFF
	/doevents FireTotem
	/doevents CheckHoTOnTarget
	/doevents FocusHEALMT_ON
	/doevents FocusHEALMT_OFF
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
		/varset	int_point_tankPatch ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tankPatch}
	}
	/doevents flush ChangeHP
/return

Sub EVENT_ChangeHPTank(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agTank HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_tankPatch ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Spawn[id ${int_MTID}].CleanName} <<  @ ${int_point_tankPatch}

	}
/return 

Sub EVENT_ChangeHPSelf(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agSelf HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_selfPatch ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> ${Me.CleanName} <<  @ ${int_point_selfPatch}

	}
/return 

Sub EVENT_ChangeHPGroup(_line, _chatSender, _eventParams)
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo \agGroup HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_grpPatch ${HP}
		/dgt [${Me.Class.ShortName}] ${Me.Name} ${str_action} >> GROUP <<  @ ${int_point_grpPatch}
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
		/varset str_spellset default
		/varset str_healMode CHCHAIN
		/call InterruptOFF
	} else /if (${_strMode.Find[EFFICIENT]}) {
		/echo \a-wChanging healing \aoMODE \a-wto \agEFFICIENT
		/varset str_spellset efficient
		/varset use_heal_HoT FALSE
		/varset str_healMode EFFICIENT
		/call InterruptON
	} else {
		/echo \a-wChanging healing \aoMODE \a-wto \agDEFAULT
		/varset str_spellset default
		/varset use_heal_HoT TRUE
		/varset str_healMode DEFAULT
		/call InterruptOFF
	}
	/memspellset ${str_spellset}
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
	/if (${int_healMode}==0) /echo \a-wChanging healing \aoTYPE \a-wto \ag0-Self (only)
	/if (${int_healMode}==1) /echo \a-wChanging healing \aoTYPE \a-wto \ag1-MT (and self)
	/if (${int_healMode}==2) /echo \a-wChanging healing \aoTYPE \a-wto \ag2-Group (and MT, includes self)
	/if (${int_healMode}==3) /echo \a-wChanging healing \aoTYPE \a-wto \ag3-GroupOnly (includes self)
/return


Sub EVENT_FireTotem(_line, _chatSender, _eventParams)
	/if (!${bln_hasTotem}) /return
	/call PlaceTotem ${Int[${_eventParams}]}
/return

Sub PlaceTotem(int _totemKey)
	/echo PlaceTotem ${_totemKey} ? ${int_totemKey}
	/if (${_totemKey} == ${int_totemKey} || ${_totemKey} == 0) {
		/call MQ2Cast "${str_totemName}" alt
		/rs Totem: ${str_totemName} Placed
	} 

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
----------------------------  
**|	
Sub setHealingSpell(int _hp)
	/if (!${Bool[${_hp}]}) /return
	/if (${str_healMode.Equal[NORMAL]}) {
		/if (${_hp} < ${int_point_stopHeal}) {
			/if (${_hp} <= ${int_currentFPoint}) {
				/varset str_currentHeal	${str_heal_frantic}
			} else {
				/varset str_currentHeal ${str_heal_normal}
			}
		}	
	} else /if (${str_healMode.Equal[EFFICIENT]}) {
		/if (${_hp} < ${int_point_calcStopHeal}) {
			/if (${_hp} <= ${int_currentFPoint}) {
				/varset str_currentHeal	${str_heal_frantic_efficient}
			} else {
				/varset str_currentHeal ${str_heal_normal_efficient}
			}
		}	
	} else /if (${str_healMode.Equal[CHCHAIN]}) {
		|- Note, if we are in CHCHAIN mode we are not really going to use this  method as we will bypass all 
		|  normal healing routes
	} 
	/echo \aysetHealingSpell ${_class} ${_hp} ${str_currentHeal}
/return


|**
 ----------------------------
 Check Health Subroutines
 ----------------------------
 These three subroutines are called in order from the Class Macro MainLoop
 checksMT	(and heals)
 checksSELF	(and heals)
 checksGRP	(and heals)
 Healing type changes the index of int_healMode
 0-Self (only) | 1-MT (and self) | 2-Group (and MT, includes self) | 3-GroupOnly (includes self) | 
 ----------------------------
**|

Sub chkHeal_MT
	/if (${int_healMode}== 0 || ${int_healMode} == 3) /return
	/call chkMTAlive TRUE
	/if ((${Spawn[id ${int_MTID}].PctHPs} < ${int_point_tankPatch}) && (${Spawn[id ${int_MTID}].Distance} < ${int_range_normal})) /call healMAINTANK
/return


Sub chkHeal_Self
	/if (${Me.PctHPs}<${int_point_selfPatch}) /call healSELF
/return


Sub chkHeal_Group
	/if (${int_healMode}>=2) /call healGRP
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
	/varcalc int_point_calcStopHeal ${Math.Calc[${int_point_tankPatch}*1.1]}
	/call chkMTAlive TRUE
	/if (${Spawn[id ${int_MTID}].PctHPs} <= 0) /return
	/call setHealingSpell ${Target.PctHPs}
	/echo \aghealMAINTANK:\ao${str_MTName}\aw(\ay${int_MTID}\aw) \aw=>\ag${Target.Name}  \ao${str_currentHeal}
	/call healCast ${int_MTID} ${Target.Name} "${str_currentHeal}" FALSE
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /call TrueTarget ${int_targetID}
/return


Sub healMAINTANKOLD
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_tankPatch}*1.1]}	
	/if (${Target.ID} != ${int_MTID}) /call TrueTarget ${int_MTID}
	/echo healMAINTANK (healing.inc): ${int_MTID} ${Target.Name} "${str_heal_normal}"
	/call healCast ${int_MTID} ${Target.Name} "${str_heal_normal}" FALSE
	/varset int_point_calcStopHeal	${int_point_tankPatch}
/return

|**
 ----------------------------
 HealSelf
 ----------------------------
 Healing Logic for the ranger
 Determines which spell to cast, then casts it.
 ----------------------------
**|

|**
 ----------------------------
 HealSelf
 ----------------------------
 Healing Logic for the cleric
 Determines which spell to cast, then casts it.
 This routine is modified from the common one in xiris_healing.inc
 ----------------------------
**|

Sub healSELF
	/varcalc 	int_point_calcStopHeal ${Math.Calc[${int_point_selfPatch}*1.05]}
	/if (${Target.ID} != ${Me.ID}) /call TrueTarget ${Me.ID}
	/call setHealingSpell ${Me.PctHPs}
	/echo \aghealSELF:\ao${str_Me.Me.Name}\aw(\ay${Me.ID}\aw) \aw=>\ag${Target.Name}  \ao${str_currentHeal}
	/call healCast ${Me.ID} ${Me.Name} "${str_currentHeal}" FALSE
	/if (${Me.Class.ShortName.Equal[CLR]}) /call TrueTarget ${int_MTID}
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /call TrueTarget ${int_targetID}
/return

Sub healSELFOLD
	/varcalc 	int_point_calcStopHeal ${Math.Calc[${int_point_selfPatch}*1.1]}
	/if (${Target.ID} != ${Me.ID}) /call TrueTarget ${Me.ID}
	/echo healSELF (healing.inc): ${Me.ID} ${Target.Name} "${str_heal_normal}"
	/call healCast ${Me.ID} ${Me.Name} "${str_heal_normal}" FALSE
	/varset int_point_calcStopHeal	${int_point_selfPatch}
/return

|**
 ----------------------------
 HealGroup
 ----------------------------
 Healing Logic for the Group
 Determines which spell to cast, then casts it.
 	int_point_grpHot=95 
	int_point_grpPatch=65
	int_point_grpFrantic=40
	arr_groupHP[${j},1] ${Group.Member[${i}].PctHPs}
	arr_groupHP[${j},2] ${Group.Member[${i}].Distance3D}
	arr_groupHP[${j},3] ${Group.Member[${i}].ID}
	arr_groupHP[${j},4] ${i}  - The index in the group of the member, so we can quickly set the name even for out of zone chars
 ----------------------------
**|
Sub healGRP
	/declare 	i 			 	int local	0
	/declare 	int_inRange		int local 	0
	/varcalc	int_point_calcStopHeal ${Math.Calc[${int_point_grpPatch}*1.1]}
	/declare 	memName 		string local 

	/declare    franticRange    int local ${int_range_frantic}
	/declare    normalRange  	int local ${int_range_normal}
	/declare 	perTick			int local 0

	/if (${str_healMode.Equal[EFFICIENT]}) {
		/varset franticRange	${int_range_frantic_efficient}
		/varset normalRange 	${int_range_normal_efficient}
	}

	:determineGroupDamage
	| Method updates the group array, in order of HP, and counts the major/minor hurt
	/call updateGroupHP

	| No one is hurt, return
	|/echo determineGroupDamage healGRP major: ${int_majorHurt} minor: ${int_minorHurt}
	/if (${int_majorHurt} == 0 && ${int_minorHurt} == 0) /goto :return

	| Determine if we should use group heal
	| Check the range of the majorhurt
	/if (${int_majorHurt} >=3 && (${Me.Class.ShortName.Equal[CLR]} || ${Me.Class.ShortName.Equal[PAL]}) && ${use_heal_group} && (${Me.CurrentMana} > ${Spell[${str_heal_group}].Mana})) {
		/call getGroupInRange ${int_range_group}
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
		/call getGroupInRange ${int_range_groupHoT}
		/varset int_inRange ${Int[${Macro.Return}]}
		/if (${int_inRange}>=3 ) {
			/echo  \apGROUP_HOT \ap${str_heal_groupHoT} \awon \ay${Me.Name}
			/call healCast ${Me.ID} ${Me.Name} "${str_heal_groupHoT}" TRUE
			/if (${Macro.Return.Equal["CAST_SUCCESS"]}) /varset timer_groupHoT 40s
			/goto :return
		} else {
			/echo \apGROUP_HOT not fired not enough in range \aw(\ay${int_inRange} < 3\aw)
			/goto :CastSingleTargetHeals
		}
	} 

	:CastSingleTargetHeals
	| Note the worst hurt will be the first in the list, and then run the heal checks again
	| This is so we heal 1 person, check tank, self, heal 1 person, check tank, self.
	| The only time we skip to another person in group is if they are out of frantic range.
	/call updateGroupHP
	|/echo CastSingleTargetHeals healGRP major: ${int_majorHurt} minor: ${int_minorHurt}
	/for i 1 to 6 
		| Not present or No Data? Then the toon is not in zone!
		/varset memName ${Group.Member[${arr_groupHP[${i},4]}]}
		|----------Not Found------------------No HP Left----------------------------Out of Range---------------------No ID-----------------------------
		/if (!${SpawnCount[pc ${memName}]} || ${arr_groupHP[${i},1]} == 0 || ${arr_groupHP[${i},2]} >= 250 || ${arr_groupHP[${i},3]} == 0) /goto :next_groupMember

		| No Heal? Since the lowest HP is always first, if we get to a index where there is no heal at all, we can skip the rest
		/if (${use_heal_HoT} && (${arr_groupHP[${i},1]} >= ${int_point_grpHot}) ) /goto :return
		/if (!${use_heal_HoT} && (${arr_groupHP[${i},1]} >= ${int_point_grpPatch})) /goto :return

		| Frantic? 
		| This spell has the longest range, so if we can't reach them here, we need to goto next group member
		/if (${arr_groupHP[${i},1]} <= ${int_point_grpFrantic}) {
			/if (${arr_groupHP[${i},2]} >= ${franticRange}) {
				/echo \ar${memName} is out of FRANTIC range or DEAD
				/goto :next_groupMember
			} else {
				/varset str_currentHeal ${str_heal_frantic}
				/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_frantic_efficient}
				/echo  \aoFRANTIC \ag(${str_healMode})  \ar${str_currentHeal} \awon \ag ${memName}\aw(\ay${arr_groupHP[${i},3]}\aw)
				/if ((${Me.CurrentMana} < ${Spell[${str_currentHeal}].Mana})) /goto :return
				/call healCast ${arr_groupHP[${i},3]} ${Spawn[pc id ${arr_groupHP[${i},3]}].Name} "${str_currentHeal}" FALSE
				/goto :return	
			}
		} else /if (${arr_groupHP[${i},1]} <= ${int_point_grpPatch} && ${arr_groupHP[${i},2]} <= ${int_range_normal}) {
			/varset str_currentHeal ${str_heal_normal}
			/if (${str_healMode.Equal[EFFICIENT]}) /varset str_currentHeal ${str_heal_normal_efficient}
			/echo \aoNORMAL \ag(${str_healMode})   \ay${str_currentHeal} \awon \ag ${memName}\aw(\ay${arr_groupHP[${i},3]}\aw)
			/if ((${Me.CurrentMana} < ${Spell[${str_currentHeal}].Mana})) /goto :return
			/call healCast ${arr_groupHP[${i},3]} ${Spawn[pc id ${arr_groupHP[${i},3]}].Name} "${str_currentHeal}" FALSE
			/goto :return
		} else {
			/if (!${use_heal_HoT}) /goto :next_groupMember
			/dquery ${Spawn[pc id ${arr_groupHP[${i},3]}].Name} -q "Me.SPA[100]"  -o perTick
			/delay 120 ${DanNet.Q.Received}
			/echo \ag${Spawn[pc id ${arr_groupHP[${i},3]}].Name} \awhas \ay${perTick} \awHoT happening
			/if (${arr_groupHP[${i},1]} <= ${int_point_grpHot} && ${arr_groupHP[${i},2]} <= ${int_range_HoT} && ${perTick} < ${int_heal_HoT_perTick}) {
				/echo  \aoHoT \ag(${str_healMode})   \ay${str_currentHeal} \awon \ag ${memName}\aw(\ay${arr_groupHP[${i},3]}\aw)
				/if ((${Me.CurrentMana} < ${Spell[${str_heal_HoT}].Mana})) /goto :return
				/call healCast ${arr_groupHP[${i},3]} ${Spawn[pc id ${arr_groupHP[${i},3]}].Name} "${str_heal_HoT}" TRUE
				/goto :return
			}
		}
	:next_groupMember
	/next i

	|:debug_group_hp
	|/for i 1 to 6
	|	/echo \ag${arr_hotTimerKeys[${i}]} hp:${arr_groupHP[${i},1]} range: ${arr_groupHP[${i},2]} id:${arr_groupHP[${i},3]} 
	|	/echo \ao  timer?${timer_HoT_${arr_groupHP[${i},3]}}   ==> ${${arr_hotTimerKeys[${i}]}}
	|/next i
	
	:return
/return

|-- Method to update the group HP tracker ordered
Sub updateGroupHP
	/declare 	i 			 		int local	0
	/declare    j                   int local   0
	/declare    s					int local   0
	/declare 	hp1 				int local 	0
	/declare 	hp2 				int local 	0
	/declare 	arr_temp[1,4]		int local	100

	/varset int_minorHurt 0
	/varset int_majorHurt 0
	/for i 0 to 5
		/varcalc j ${i}+1
		/if (${Group.Member[${i}].Present} && (${Group.Member[${i}].PctHPs}< 100) && (${Group.Member[${i}].PctHPs}>=${int_point_grpPatch})) /varcalc int_minorHurt ${int_minorHurt}+1 
		/if (${Group.Member[${i}].Present} && (${Group.Member[${i}].PctHPs}< 100) && (${Group.Member[${i}].PctHPs}< ${int_point_grpPatch})) /varcalc int_majorHurt ${int_majorHurt}+1 
		/if (${Group.Member[${i}].Present}) {
			/varset arr_groupHP[${j},1] ${Group.Member[${i}].PctHPs}
			/varset arr_groupHP[${j},2] ${Group.Member[${i}].Distance3D}
			/varset arr_groupHP[${j},3] ${Group.Member[${i}].ID}
			/varset arr_groupHP[${j},4] ${i}
		} else {
			/varset arr_groupHP[${j},1] 100
			/varset arr_groupHP[${j},2] 1000
			/varset arr_groupHP[${j},3] 0
			/varset arr_groupHP[${j},4] ${i}
		}
		
	/next i

	|--- sorting arr_groupHP
	/for i 1 to 6
		/varcalc s ${i}+1
		/if (${s} > 6)  /goto :return
		/for j ${s} to 6
			/varset hp1 ${arr_groupHP[${i},1]}
			/varset hp2 ${arr_groupHP[${j},1]}
			/if (${hp1} > ${hp2}) {
				/varset arr_temp[1,1] ${arr_groupHP[${i},1]}
				/varset arr_temp[1,2] ${arr_groupHP[${i},2]}
				/varset arr_temp[1,3] ${arr_groupHP[${i},3]}
				/varset arr_temp[1,4] ${arr_groupHP[${i},4]}
				/varset arr_groupHP[${i},1] ${arr_groupHP[${j},1]}
				/varset arr_groupHP[${i},2] ${arr_groupHP[${j},2]}
				/varset arr_groupHP[${i},3] ${arr_groupHP[${j},3]}
				/varset arr_groupHP[${i},4] ${arr_groupHP[${j},4]}
				/varset arr_groupHP[${j},1] ${arr_temp[1,1]}
				/varset arr_groupHP[${j},2] ${arr_temp[1,2]}
				/varset arr_groupHP[${j},3] ${arr_temp[1,3]}
				/varset arr_groupHP[${i},4] ${arr_groupHP[${j},4]}
			}
		/next j 
	/next i
	
	:printOrdered
	/echo \ay1. --------------Ordered Major Hurt---------
	/echo \agint_majorHurt: \ar${int_majorHurt} \agint_minorHurt: \ay${int_minorHurt}
	/declare memName string local
	/for i 1 to 6
		/varset memName ${Group.Member[${arr_groupHP[${i},4]}]}
		/echo \ag${i}:${arr_groupHP[${i},1]} d:${arr_groupHP[${i},2]}  \ao${memName} \aw(\ag${arr_groupHP[${i},3]}\aw)
	/next i
	:return
/return

Sub getGroupInRange(int _range) 
	/declare inRange int local 0 
	/declare k int local 0
	/declare memNameIR string local
	/for k 1 to 6
		/varset memNameIR ${Group.Member[${arr_groupHP[${k},4]}]}
		/if (${arr_groupHP[${k},2]} <= ${_range} && ${SpawnCount[pc ${memNameIR}]}) /varcalc inRange ${inRange}+1
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
Sub healCast(int _id, string _name, string _spell, bool _isGroup)
	/if (${_name.Equal[NULL]} || ${_id} == 0) {
		/echo \arHEAL NAME is NULL || ID is \ao${_id}\ar something is fucked;
		/return
	}
	/echo \awhealCast \ayTarget:\ag${_name}(${_id}) \aydist: \ag${Target.Distance} \aycHP: \ag${Spawn[pc ${_name}].PctHPs} \aycSH: \ag${int_point_calcStopHeal} \ayspell: \ag"${_spell}"
	/if (${_isGroup} || (${Spawn[pc ${_name}].PctHPs} < ${int_point_calcStopHeal} && (${Me.CurrentMana} > ${Spell[${_spell}].Mana}))) {
		/if (${Target.ID} != ${_id}) /call TrueTarget ${_id}
		/call chkSpellMemorized "${_spell}" TRUE 2
		/call MQ2Cast "${_spell}" gem2 1s chkHP
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
	/if (${Me.Class.ShortName.NotEqual[CLR]} || !${use_heal_divineArb} || !${bln_DARBReady}) /goto :return

	:divine
	/declare gmember int local 0
	/for gmember 0 to ${Group.Members}
		/if (${Group.Member[${gmember}].PctHPs}<=${int_point_divineArb}) {
			| add a check for member in group here
			/dgt ${Group.Member[${gmember}].Name} triggered Divine Arbitration!
			/call Interrupt
			/call healDivineArbitration
			/goto :return
		}
	/next gmember

	:interrupt
	/call Interrupt

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
Sub chkRez
	/if (!${bln_autoRez}) /return
	/if (!${bln_combatRez} && ${bln_engaged}) /return

	| find all corpses in range	
	/if (${SpawnCount[corpse]}) /squelch /hidecorpse npc
	/declare int_corpses int local ${SpawnCount[corpse radius ${int_range_rez}]}
	/if (!${int_corpses}) /return

	| if corpses, then aerez
	/call AERez ${int_corpses}
/return

|-- TODO: Rework this. It's buggy
Sub AERez(int _corpseCount)

	
	/declare arr_corpses[${_corpseCount}] int local 
	/declare str_corpse		string local

	/declare i 				int local
	/declare x				int local
	/declare id 			int local
	/declare charID int local

	/for i 1 to ${_corpseCount}
		/varset arr_corpses[${i}] ${NearestSpawn[${i},corpse radius ${int_range_rez}].ID}
	/next i
	
	:consider 
	/for i 1 to ${_corpseCount}
		/varset id ${arr_corpses[${i}]}
		/varset bln_skipCorpse FALSE

		| get the name from corpse	
		/varset str_corpse ${Spawn[id ${id}].CleanName.Token[1,']}
		/if (!${Bool[${str_corpse}]}) /goto :next_corpse

		| check corpse name 
		/dquery ${str_corpse} -q "Me.ID"
		/varset charID ${DanNet.Q}
		| make sure the target is actually a corpse, and online
		/if (${Spawn[id ${id}].CleanName.Find[corpse]} == NULL || !${charID}) /varset bln_skipCorpse TRUE
		/if (${bln_skipCorpse}) /goto :next_corpse


		| check ignore list
		/if (${int_rezIgnore}) {
			/for x 1 to ${int_rezIgnore}
				/if (${arr_corpses[${x}]} == ${arr_rezIgnore[${x}]}) {
					/echo corpse #{x} ${arr_corpses[${x}]} == ${arr_rezIgnore[${x}]}
					/varset bln_skipCorpse TRUE
				}
			/next x
		}
		/echo bln_skipCorrpse? ${bln_skipCorpse}
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
	/dex  ${Spawn[id ${_id}].CleanName.Arg[1,']} /consent ${Me.Name}
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
	/tell ${_name} Checking to see if you are online for rez....
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
		/varset arrTemp[${x}] ${arr_rezIgnore[${x}]}
	/next x
	/varset int_rezIgnore ${Math.Calc[${int_rezIgnore}+1]}
	/varset arrTemp[${int_rezIgnore}] ${_id}
	| delete old ignore list and copy temp list to new ignore list
	/deletevar arr_rezIgnore
	/declare arr_rezIgnore[${int_rezIgnore}] int outer
	/for x 1 to ${int_rezIgnore}
		/varset arr_rezIgnore[${x}] ${arrTemp[${x}]}
	/next x
/return

Sub CleanRezIgnoreList
	/if (!${Bool[${int_rezIgnore}]}) /return
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
	/if (${found} != ${int_rezIgnore} && ${int_rezIgnore} != -1) {
		/deletevar arr_rezIgnore
		/varset int_rezIgnore ${found}
		/declare arr_rezIgnore[${int_rezIgnore}] int outer
		/for x 1 to ${int_rezIgnore}
			/varset arr_rezIgnore[${x}] ${arrTemp[${x}]}
		/next x
	}
/return


Sub EVENT_InterruptON
	/call InterruptON
/return

Sub EVENT_InterruptOFF
	/call InterruptOFF
/return

Sub InterruptON
	/echo \ayAutoInterrupt: \agON
	/varset bln_autointerrupt TRUE
/return

Sub InterruptOFF 
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


	/declare 	healINI				string  outer	xiris_common/xiris_healing.ini
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

	/echo ---------------
	/echo \ayHeal Spells: \awNormal:\ag${str_heal_normal}\aw  || Efficient:\ag${str_heal_frantic_efficient}\aw || CH:\ag${str_heal_complete}
	/echo				 \awFrantic:\ag${str_heal_frantic}\aw || Frantic Efficient:\ag${str_heal_frantic_efficient}\aw || HOT:\ag${str_heal_HoT}
	/echo               \awGroup:\ag${str_heal_group}\aw || GroupHoT:\ag${str_heal_groupHoT}\aw || Rez:\ag${str_rezSpell}
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

	
	/echo \ayHeal Ranges: \awPatch:\ag${int_range_normal_efficient}\aw || Normal:\ag${int_range_normal}\aw || Group:\ag${int_range_group}\aw
	/echo  			Frantic:\ag${int_range_frantic}\aw || FranticEfficient:\ag${int_range_frantic_efficient} 
	/echo 				HoT:\ag${int_range_HoT}\aw || GroupHoT:\ag${int_range_groupHoT}\aw || Rez:\ag${int_range_rez}
	/echo ---------------
	|-- Heal Points
		|-- SELF (Defaults)
		/declare 	int_def_selfPatch		int     outer   ${Ini[${healINI},Heal_Points,int_point_selfPatch]}
		/declare 	int_def_selfFrantic		int     outer   ${Ini[${healINI},Heal_Points,int_point_selfFrantic]}
		/declare 	int_def_selfHot			int     outer   ${Ini[${healINI},Heal_Points,int_point_selfHot]}

		|-- SELF (Overrides)
		/declare	int_point_selfPatch		int 	outer 	${Ini[INI_CHAR,Heal_Points,int_point_selfPatch,${int_def_selfPatch}]}
		/declare	int_point_selfFrantic	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_selfFrantic,${int_def_selfFrantic}]}
		/declare	int_point_selfHot		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_selfHot,${int_def_selfHot}]}
	
		|-- TANK (Defaults)
		/declare 	int_def_tankPatch		int     outer   ${Ini[${healINI},Heal_Points,int_point_tankPatch]}
		/declare 	int_def_tankFrantic		int     outer   ${Ini[${healINI},Heal_Points,int_point_tankFrantic]}
		/declare 	int_def_tankHot			int     outer   ${Ini[${healINI},Heal_Points,int_point_tankHot]}

		|-- TANK (Overrides)
		/declare	int_point_tankPatch		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_tankPatch,${int_def_tankPatch}]}
		/if (${Bool[${_healPoint}]}) {
			/echo \agHealpoint setting overriding defaults: ${_healPoint}	
			/varset	int_point_tankPatch	${_healPoint}
		} else {
			/echo No healpoint defined using INI values
		}
		/declare	int_point_tankFrantic	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_tankFrantic,${int_def_tankFrantic}]}
		/declare	int_point_tankHot		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_tankHot,${int_def_tankHot}]}
	
		|-- Rest of Group (Defaults)
		/declare 	int_def_grpPatch		int     outer   ${Ini[${healINI},Heal_Points,int_point_grpPatch]}
		/declare 	int_def_grpFrantic		int     outer   ${Ini[${healINI},Heal_Points,int_point_grpFrantic]}
		/declare 	int_def_grpHot			int     outer   ${Ini[${healINI},Heal_Points,int_point_grpHot]}

		|-- Rest of Group (Overrides)
		/declare	int_point_grpPatch		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_grpPatch,${int_def_grpPatch}]}	
		/declare	int_point_grpFrantic	int		outer	${Ini[INI_CHAR,Heal_Points,int_point_grpFrantic,${int_def_grpFrantic}]}
		/declare	int_point_grpHot		int		outer	${Ini[INI_CHAR,Heal_Points,int_point_grpHot,${int_def_grpHot}]}	

		|-- Divine Arbitration && Misc
		/declare    int_point_divineArb     int		outer	${Ini[INI_CHAR,Heal_Points,int_point_divineArb,20]}
		/declare    int_point_stopHeal    	int		outer   ${Ini[INI_CHAR,Heal_Points,int_point_stopHeal,90]}	
		/declare	int_point_calcStopHeal	int		outer	100
		/declare    use_interrupt_selfHeal  bool	outer   TRUE
		/declare    int_point_interrupt		int		outer   ${Ini[INI_CHAR,Heal_Points,int_point_interrupt,50]}
	
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

	|-- Totems
	/declare    bln_hasTotem		 	bool    outer  FALSE
	/if (${str_classArchetype.Equal[PRIEST]}) /varset bln_hasTotem TRUE
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
	


	

	/call InitEvents_Healing
	/echo \ao xbot_initialize_healing ${int_point_tankPatch}
/return
