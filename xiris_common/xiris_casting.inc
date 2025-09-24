|**
----------------------------
xiris_casting.inc
----------------------------
SPELL/CASTING HANDLERS. This library supplements/abstracts mq2cast functionality
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	Added QuickNukes and AENukes
	03.31.22	xiris	Updated the named checking as well as normalizing all calls to MQ2Cast
	04.16.22	xiris	Added chkStun and support for Stun Locking
	05.08.22	xiris	Added support for bards in Nuke/DOT
	06.13.22	xiris   Removed resist events as the xiris_spell_routines.inc handles that.
	12.31.22	xiris	Changed Bards medley calls to use new method in spell_routines.inc
**|



|NECRO/DOT/DEBUFF class stuff
#EVENT DotOff1				"#*#Your |${DOT_1}| spell has worn off#*#"
#EVENT DotOff2				"#*#Your |${DOT_2}| spell has worn off#*#"
#EVENT DotOff3				"#*#Your |${DOT_3}| spell has worn off#*#"
#EVENT DotOff4				"#*#Your |${DOT_4}| spell has worn off#*#"
#EVENT DotOff5				"#*#Your |${DOT_5}| spell has worn off#*#"
#EVENT DotOff6				"#*#Your |${DOT_6}| spell has worn off#*#"
#EVENT DotOff7				"#*#Your |${DOT_7}| spell has worn off#*#"
#EVENT DotOff8				"#*#Your |${DOT_8}| spell has worn off#*#"
#EVENT DotOff9				"#*#Your |${DOT_9}| spell has worn off#*#"
#EVENT DotOff10				"#*#Your |${DOT_10}| spell has worn off#*#"
#EVENT DotOff11				"#*#Your |${DOT_11}| spell has worn off#*#"
#EVENT DotOff12				"#*#Your |${DOT_12}| spell has worn off#*#"

#EVENT Slow					"#1#is hindered by a shackle of spirit#*#"
#EVENT SlowResist			"#*#Your target resisted the |${str_slowSpell}|#*#"
#EVENT Snareon				"#*#damage by your |${str_snareSpell}|#*#"
#EVENT Snareon				"#*#engulfed by darkness#*#"
#EVENT Snareon				"#*#engulfed in an embracing darkness#*#"
#EVENT Snareon				"#*#is shackled to the ground#*#"
#EVENT Snareon				"#*#has been ensnared#*#"
#EVENT Snareon				"#*#movements slow as their feet are covered in tangling weeds#*#"
#EVENT Snareon				"#*#has been entangled#*#"
#EVENT Snareon				"#*#has been entrapped in living earth#*#"
#EVENT Snareon				"#*##covered in coruscating black shadows#*#"
#EVENT SnareResist			"#*#Your target resisted the |${Snare}|#*#"
#EVENT SnareWornOff			"Your |${str_snareSpell}| spell has worn off#*#"
#EVENT NoSnare         		"#*#is immune to changes in its#*#"
#EVENT NoSnareHold     		"#*#Your spell did not take hold.#*#"

|SPELL Checking stuff
#EVENT ChkHasSpell           "#*#ChkHasSpell #2#"

|RESIST TYPE stuff
#EVENT SetResistType     	"#1# tells the group#*#'ResistTypes #2#'"
#EVENT SetResistType		"#1# tells you#*#'ResistTypes #2#'"
#EVENT SetResistType		"#1# tells the raid#*#ResistTypes #2#'"
#EVENT SetResistType 		"[ #1# #*#]#*#ResistTypes #2#"

|SETTING Triggers
#EVENT SetUseFastOnly     	"#1# tells the group#*#'UseFastOnly #2#'"
#EVENT SetUseFastOnly		"#1# tells you#*#'UseFastOnly #2#'"
#EVENT SetUseFastOnly		"#1# tells the raid#*#UseFastOnly #2#'"
#EVENT SetUseFastOnly 		"[ #1# #*#]#*#UseFastOnly #2#"


Sub InitEvents_Casting
	/if (${str_classArchetype.Equal[CASTER]} || ${str_classArchetype.Equal[PRIEST]}) {
		/call RegisterBackgroundEvents Events_Casting_Background 2
	}
	
	/if (${Me.Class.ShortName.Equal[RNG]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[NEC]}) {
		/call RegisterRaidEvents Events_Casting_Snare 3	
	}

/return

Sub Events_Casting_Background
	/doevents SetResistType
	/doevents SetUseFastOnly
	/doevents ChkHasSpell
/return

Sub Events_Casting_Snare
	/doevents Snareon
	/doevents SnareResist
	/doevents SnareWornOff
/return

Sub Events_Casting_DOT

/return 

|**
 ----------------------------
 Direct Damage  
 ----------------------------
 Loops through the nuke list and applies
 ----------------------------
**|

Sub chkNuke
	/if (!${bln_engaged} || !${Target.ID}) 		/return
	/if (${bln_useFastOnly})	/return
	/if (${NUKE_Total} == 0)    /return 
	/if (${AENUKE_Active}) 		/return
	/if (${NUKE_DelayTimer} && ${NUKE_HasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return

	|-- press pet attack for funsies
	/if (${Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP and my mana check
	/if (${isNamed}  && (${Target.PctHPs} < ${NUKE_StopHPIfNamed}))	/return
	/if (${isNamed}  && (${Target.PctHPs} > ${NUKE_StartHPIfNamed}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${NUKE_StopHP}))			/return
	/if (!${isNamed} && (${Target.PctHPs} > ${NUKE_StartHP}))		/return

	/if (${isNamed}  && (${Me.PctMana} < ${NUKE_Mana_NamedMinimum}))		/return
	/if (!${isNamed} && (${Me.PctMana} < ${NUKE_Mana_Minimum}))			/return

	:fd_check
	/if (${Me.Feigning}) /stand
	
	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {
		/declare i 			int 	local
		/declare gemStr 	string 	local 	
		/declare gem		string  local
		
		/for i 1 to ${NUKE_Total}  
			:NukeLoopStart
			/if ((${Spawn[id ${int_targetID}].Name.Find[_corpse]}) && ${bln_engaged})  /return
			|-- skip if not ready
			/if (!${Me.SpellReady[${NUKE_${i}_Spell}]}) /goto :NextNuke
			|-- skip if disabled
			/if (!${NUKE_${i}_Use}) /goto :NextNuke

			|-- Check the timer on this nuke
			|/if (${NUKE_${i}_RecastTimer} <= 0) 
			/if (${NUKE_${i}_RecastTimer} > 0 && ${NUKE_${i}_HasCastOnce}) {
				/if (${i}<${NUKE_Total}) { 
					/echo \arTimer not ready for \ay${NUKE_${i}_Spell} :: \ay${NUKE_${i}_RecastTimer}
					/goto :NextNuke
				} else {
					/return
				}
			}

			
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
			/if (!${isNamed} && ${NUKE_${i}_NamedOnly}) {
				|/echo Skipping Nuke ${NUKE_${i}_Spell} is named only
				/goto :NextNuke
			}

			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${NUKE_${i}_Resist}]}]} || ${NUKE_${i}_Resist.Equal[Unresistable]}) {
				|/echo Nuke ${NUKE_${i}_Resist} matches resist
			} else {
				/echo Skipping Nuke ${NUKE_${i}_Resist} doesnt match resist
				/goto :NextNuke
			}
			
			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping		
			/call chkSpellMemorized "${NUKE_${i}_Spell}" TRUE ${NUKE_${i}_Gem}
			:NukeCastWait
			/delay 5s (${Me.SpellReady[${NUKE_${i}_Spell}]} && (!${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}))

			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${NUKE_${i}_StartHP} && ${Target.PctHPs}>=${NUKE_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${NUKE_${i}_Spell}].Mana} && ${Target.Distance}<=${Spell[${NUKE_${i}_Spell}].Range} && ${Target.LineOfSight}) {
					/varset int_temp_stopCastPoint ${NUKE_${i}_StopHP}
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/call MQ2Medley SONG "${NUKE_${i}}" ${int_targetID} TRUE
						/echo \ag ${NUKE_${i}_Spell}  : DELAYING for \ay${NUKE_${i}_DelayTimer.OriginalValue}s \agrefresh	
					} else {
						/call MQ2Cast "${NUKE_${i}_Spell}" ${NUKE_${i}_Gem} 1s chkCastTarget
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/echo \ag ${NUKE_${i}_Spell} ${Macro.Return} : DELAYING for \ay${NUKE_${i}_DelayTimer.OriginalValue}s \agrefresh	
						}
					}
					/varset NUKE_DelayTimer	${NUKE_DelayTimer.OriginalValue}
					/varset NUKE_${i}_RecastTimer	${NUKE_${i}_RecastTimer.OriginalValue}
					/varset NUKE_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextNuke
		/call RunRaidEvents
		/call RunRapidEvents	
		/next i
	}
	/varset NUKE_HasCastOnce TRUE
/return

Sub chkQNuke
	/if (!${bln_engaged} || !${Target.ID}) 		/return
	/if (${QNUKE_Total} == 0) /return 
	/if (${AENUKE_Active}) 		 /return
	/if (${QNUKE_DelayTimer} && ${QNUKE_HasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- press pet attack for funsies
	/if (${Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP and my mana check
	/if (${isNamed}  && (${Target.PctHPs} < ${QNUKE_StopHPIfNamed}))	/return
	/if (${isNamed}  && (${Target.PctHPs} > ${QNUKE_StartHPIfNamed}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${QNUKE_StopHP}))		/return
	/if (!${isNamed} && (${Target.PctHPs} > ${QNUKE_StartHP}))		/return

	/if (${isNamed}  && (${Me.PctMana} < ${QNUKE_Mana_NamedMinimum}))		/return
	/if (!${isNamed} && (${Me.PctMana} < ${QNUKE_Mana_Minimum}))			/return


	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {

		/declare i 			int 	local
		/declare gemStr 	string 	local 	
		/declare gem		string  local
		
		/for i 1 to ${QNUKE_Total}  
			:QNLoopStart
			/if ((${Spawn[id ${int_targetID}].Name.Find[_corpse]}) && ${bln_engaged})  /return
			|-- skip if not ready
			/if (!${Me.SpellReady[${QNUKE_${i}_Spell}]}) /goto :NextQuickNuke
			|-- skip if disabled
			/if (!${QNUKE_${i}_Use}) /goto :NextQuickNuke

			|-- Check the timer on this nuke
			/if (${QNUKE_${i}_RecastTimer} > 0 && ${QNUKE_${i}_HasCastOnce}) {
				/if (${i}<${QNUKE_Total}) { 
					/echo \arTimer not ready for \ay${QNUKE_${i}_Spell} :: \ay${QNUKE_${i}_RecastTimer}
					/goto :NextQuickNuke
				} else {
					/return
				}
			}
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
	
			/if (!${isNamed} && ${QNUKE_${i}_NamedOnly}) {
				/echo Skipping QNUKE ${QNUKE_${i}_Spell} is named only
				/goto :NextQuickNuke
			}

			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${QNUKE_${i}_Resist}]}]} || ${QNUKE_${i}_Resist.Equal[Unresistable]}) {
				|/echo QNUKE ${QNUKE_${i}_Resist} matches resist
			} else {
				/echo Skipping QNUKE ${QNUKE_${i}_Resist} doesnt match resist
				/goto :NextQuickNuke
			}
			
			/call RunAllEvents
			
			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping		
			/call chkSpellMemorized "${QNUKE_${i}_Spell}" FALSE ${QNUKE_${i}_Gem}
			:QnukeNukeCastWait
			/delay 5s (${Me.SpellReady[${QNUKE_${i}_Spell}]} && (!${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}))
			
			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${QNUKE_${i}_StartHP} && ${Target.PctHPs}>=${QNUKE_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${QNUKE_${i}_Spell}].Mana} && ${Target.Distance}<=${Spell[${QNUKE_${i}_Spell}].Range} && ${Target.LineOfSight}) {
					/varset int_temp_stopCastPoint ${QNUKE_${i}_StopHP}
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/call MQ2Medley SONG "${QNUKE_${i}}" ${int_targetID} TRUE
						/echo \ag ${QNUKE_${i}_Spell}  : DELAYING for \ay${QNUKE_${i}_DelayTimer.OriginalValue}s \agrefresh	
					} else {
						/call MQ2Cast "${QNUKE_${i}_Spell}" ${QNUKE_${i}_Gem} 1s chkCastTarget
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/echo \ag ${QNUKE_${i}_Spell} ${Macro.Return} : DELAYING for \ay${QNUKE_${i}_DelayTimer.OriginalValue}s \agrefresh	
						}
					}
					/varset QNUKE_DelayTimer		${QNUKE_DelayTimer.OriginalValue}
					/varset QNUKE_${i}_RecastTimer	${QNUKE_${i}_RecastTimer.OriginalValue}
					/varset QNUKE_${i}_HasCastOnce TRUE
					/varset QNUKE_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextQuickNuke
		/call RunRaidEvents
		/call RunRapidEvents	
		/next i
	}
	/varset QNUKE_HasCastOnce TRUE
/return

|**
 ----------------------------
 AE Direct Damage  
 ----------------------------
 Loops through the AE nuke list and applies, doesn't do much checking because
 this is usually triggered via an EVENT
 ----------------------------
**|
Sub chkAENuke
	/if (!${AENUKE_Active}) /return
	/if (${AENUKE_DelayTimer} && ${AENUKE_HasCastOnce})	 	/return 


	/declare i int local
	:AENukeLoop
	/for i 1 to ${AENUKE_Total}
		|-- AE Nuke is active, let's light them up!
		|/docommand /${announceChannel} Lighting them up with ${AENUKE_Total} AE Nukes! ${AENUKE_${i}_Spell}
		:AENukeStart 
		|-- skip if not ready
		/if (!${Me.SpellReady[${AENUKE_${i}_Spell}]}) /goto :NextAENuke

		/if (${AENUKE_${i}_RecastTimer} > 0 && ${AENUKE_${i}_HasCastOnce}) {
			/if (${i}<${AENUKE_Total}) { 
				/echo \arTimer not ready for \ay${AENUKE_${i}_Spell} :: \ay${AENUKE_${i}_DelayTimer}
				/goto :NextAENuke
			}
		}
		/call chkSpellMemorized "${AENUKE_${i}_Spell}" TRUE ${AENUKE_${i}_Gem}
		:AENukeCastWait
		/delay 5s (${Me.SpellReady[${AENUKE_${i}_Spell}]} && (!${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}))

		/call MQ2Cast "${AENUKE_${i}_Spell}"	
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
			/echo \ag ${AENUKE_${i}_Spell} ${Macro.Return} : DELAYING for \ay${AENUKE_${i}_DelayTimer.OriginalValue}s \agrefresh	
		}
		/varset AENUKE_DelayTimer	${AENUKE_DelayTimer.OriginalValue}
		/varset AENUKE_${i}_RecastTimer	${AENUKE_${i}_RecastTimer.OriginalValue}
		/varset AENUKE_${i}_HasCastOnce TRUE
	:NextAENuke
	/call RunRaidEvents
	/call RunRapidEvents	
	/next i
	/if (${AENUKE_Active}) /goto :AENukeLoop
	:Return
/return	



|**
 ----------------------------
 Check Stun  
 ----------------------------
 If we are supposed to be stunning this NPC
 ----------------------------
**|
Sub chkStun(bool _memorize)
	/if (!${bln_engaged} || !${Target.ID}) 		/return
	/if (${STUN_Total} == 0)  /return 
	/if (${STUN_DelayTimer} && ${STUN_HasCastOnce})	 	/return 

	/if (${Me.PctMana} < ${STUN_Mana_Minimum}) 	/return
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {

		/declare i 			int 	local
		/declare gemStr 	string 	local 	
		/declare gem		string  local
		
		/for i 1 to ${STUN_Total}  
			:StunLoopStart
			/if ((${Spawn[id ${int_targetID}].Name.Find[_corpse]}) && ${bln_engaged})  /return
			|-- skip if not ready
			/if (!${Me.SpellReady[${STUN_${i}_Spell}]}) /goto :NextStun
			|-- skip if disabled
			/if (!${STUN_${i}_Use}) /goto :NextStun

			
			/call RunAllEvents	
			/call chkSpellMemorized "${STUN_${i}_Spell}" ${_memorize} ${STUN_${i}_Gem}

			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${STUN_${i}_StartHP} && ${Target.PctHPs}>=${STUN_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${STUN_${i}_Spell}].Mana} && ${Target.Distance}<=${Spell[${STUN_${i}_Spell}].Range} && ${Target.LineOfSight}) {
					/varset int_temp_stopCastPoint ${STUN_${i}_StopHP}
					/call MQ2Cast "${STUN_${i}_Spell}" ${STUN_${i}_Gem} 5s chkCastTarget
					/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
						/echo \ag ${STUN_${i}_Spell} ${Cast.Result}
						/varset STUN_${i}_HasCastOnce TRUE
					}
				}
			}
		:NextStun
		/call RunRaidEvents
		/call RunRapidEvents	
		/next i
	}
	/varset STUN_HasCastOnce TRUE
	/varset STUN_DelayTimer ${STUN_DelayTimer.OriginalValue}
/return



|**
 ----------------------------
 Check Stun Locking
 ----------------------------
 If we are supposed to be stunning this NPC
 ----------------------------
**|
Sub chkStunLock
	/if (!${bln_engaged} || ${Me.Invis}) /return 
	/varset STUN_UseLocking TRUE
	:stunLock
		/call chkStun TRUE
		/call RunRaidEvents
		/call RunRapidEvents
		/delay 10
		/if (${STUN_UseLocking}) /goto :stunLock
:return
/return

Sub chkStunLockStop
	/varset STUN_UseLocking FALSE
/return




|**
 ----------------------------
 Damage Over Time
 ----------------------------
 This is the routine where the majority of the DPS will occur.
 Modified to now have a seperate fast DOT component call
 Modified to allow for bards
 ----------------------------
**|

Sub chkDOT
	/if (!${bln_engaged} || !${Target.ID}) 		/return
	/if (${bln_useFastOnly}) 	/return
	/if (${DOT_Total} == 0)		/return 
	/if (${AENUKE_Active}) 		/return
	/if (${DOT_DelayTimer} && ${DOT_HasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- press pet attack for funsies
	/if (${Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP and my mana check
	/if (${isNamed}  && (${Target.PctHPs} < ${DOT_StopHPIfNamed}))		/return
	/if (${isNamed}  && (${Target.PctHPs} > ${DOT_StartHPIfNamed}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${DOT_StopHP}))			/return
	/if (!${isNamed} && (${Target.PctHPs} > ${DOT_StartHP}))			/return

	/if (${isNamed}  && (${Me.PctMana} < ${DOT_Mana_NamedMinimum}))		/return
	/if (!${isNamed} && (${Me.PctMana} < ${DOT_Mana_Minimum}))			/return
	
	:fd_check
	/if (${Me.Feigning}) /stand

	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {
		/declare i 				int 	local
		/declare dotDuration 	int 	local 	0
		/declare gemStr 		string 	local 	
		/declare gem			string  local
		
		/for i 1 to ${DOT_Total}  
			:DotLoopStart
			/if ((${Spawn[id ${int_targetID}].Name.Find[_corpse]}) && ${bln_engaged})  /return
			|-- skip if not ready
			/if (!${Me.SpellReady[${DOT_${i}_Spell}]}) /goto :NextDot
			|-- skip if disabled
			/if (!${DOT_${i}_Use}) /goto :NextDot

			|-- Check the timer on this dot
			/if (${DOT_${i}_RecastTimer} <= 0) /echo \agTimer ready for \ay${DOT_${i}_Spell} :: \ay${DOT_${i}_RecastTimer}
			/if (${DOT_${i}_RecastTimer} > 0 && ${DOT_${i}_HasCastOnce}) {
				/if (${i}<${DOT_Total}) { 
					/echo \arTimer not ready for \ay${DOT_${i}_Spell} :: \ay${DOT_${i}_RecastTimer}
					/goto :NextDot
				} else {
					/return
				}
			}
			
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
			/if (!${isNamed} && ${DOT_${i}_NamedOnly}) {
				/echo Skipping Dot ${DOT_${i}_Spell} is named only
				/goto :NextDot
			}
			
			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${DOT_${i}_Resist}]}]} || ${DOT_${i}_Resist.Equal[Unresistable]}) {
				|/echo DOT ${DOT_${i}_Resist} matches resist
			} else {
				/echo Skipping DOT ${DOT_${i}_Resist} doesnt match resist
				/goto :NextDot
			}
			
			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping
			/call chkSpellMemorized "${DOT_${i}_Spell}" FALSE ${DOT_${i}_Gem}
			
			:DotReadyWait
			/delay 5s (${Me.SpellReady[${DOT_${i}_Spell}]} && (!${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}))

			|-- All this ready, check for LOS/Distance/Mana
			|-- Think about adding the named stop point here at some point in future
			/if (${Target.PctHPs}<=${DOT_${i}_StartHP} && ${Target.PctHPs}>=${DOT_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${DOT_${i}_Spell}].Mana} && (${Me.SpellReady[${DOT_${i}_Spell}]} || ${Me.Class.ShortName.Equal[BRD]}) && ${Target.Distance}<=${Spell[${DOT_${i}_Spell}].Range} && ${Target.LineOfSight} && (${DOT_${i}_RecastTimer} <= 0)) {
					/varset dotDuration ${Spell[${DOT_${i}_Spell}].Duration.TotalSeconds}
					/varset int_temp_stopCastPoint ${DOT_${i}_StopHP}
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/call MQ2Medley SONG "${DOT_${i}}" ${int_targetID} TRUE
						/echo \ag ${DOT_${i}_Spell}  : DELAYING for \ay${dotDuration}s \agrefresh	
					} else {
						/call MQ2Cast "${DOT_${i}_Spell}" ${DOT_${i}_Gem} 1s chkCastTarget
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/echo \ag ${DOT_${i}_Spell} ${Macro.Return} : DELAYING for \ay${dotDuration}s \agrefresh	
						}
					}
					/varset DOT_DelayTimer ${DOT_DelayTimer.OriginalValue}
					/varset DOT_${i}_RecastTimer	${dotDuration}s
					/varset DOT_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextDot
		/call RunRaidEvents
		/call RunRapidEvents	
		/next i
	}
	/varset DOT_HasCastOnce TRUE
	
/return


|**
 ----------------------------
 Snaring  
 ----------------------------
 Snare Spell is declared in the INI
 Note apparently I have this disabled: TODO, actually use this again
 ----------------------------
**|
Sub chkSnare
	/if (${Me.Invis}||${Me.Moving}|| ${Me.Song[${GoM}].ID}||${list_immuneSnare.Find[${Spawn[${Target.CleanName}]}]}||${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	/if (${Target.PctHPs}<=${int_snareHP} && ${Target.ID} && ${Target.Type.Equal[NPC]} && !${bln_snareActive} && ${bln_snareUse} && ${SpawnCount[npc radius 50 zradius 10]}<=${int_snareLocalNPC}) {
		/call chkSpellMemorized "${str_snareSpell}" TRUE ${str_snareGem} 
		/varset int_temp_stopCastPoint 10
		/call MQ2Cast "${str_snareSpell}" gem5 1s chkCastTarget
	}
	
	/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
		/echo \ag ${str_snareSpell} ${Cast.Result}
		/varset bln_snareActive TRUE
	}
	
	/if (${Cast.Result.Equal[CAST_IMMUNE]}) {
		/echo \ar ${str_snareSpell} ${Cast.Result}
		/ini "${IniFile}" "SnareImmunes.${cache_zoneShort}" "SnareImmune" "${list_immuneSnare}${Target.CleanName}|"
		/varset list_immuneSnare ${Ini[${ignoreINI},"SnareImmunes.${cache_zoneShort}","SnareImmune"]}
		/varset bln_snareActive TRUE
	}
	/call RunRaidEvents
	/call RunRapidEvents	
/return




|- Method will memorize a spell, to the optional attributed gem, and if wait is true, will wait until its ready to return
|- note will call utility to string 'gem' from the gem # if found
|- Method will memorize a spell, to the optional attributed gem, and if wait is true, will wait until its ready to return
|- note will call utility to string 'gem' from the gem # if found
Sub chkSpellMemorized(string _spell, bool _wait, string _gem)
	/if (${_spell.Equal[NULL]}) /goto :nullSpell
	/if (${Me.Gem[${_spell}]})  /return

    /declare int_gem int local 9
    /if (${_gem.NotEqual[NULL]}) {
         /if (${_gem.Length} > 1) {
            | this has to be a string, strip "gem" from it
            /varset int_gem ${_gem.Right[1]}
         }
    } else {
        | this has to be an integer
        /varset int_gem ${_gem}
    } 
   
	:mem
	    /echo \aochkSpellMemorized\ag "${_spell}" \awgem:\ao${int_gem} \awwait?:\ao${_wait}
		/memspell ${int_gem} "${_spell}" 
		/if (!${_wait}) /return
		/declare waitCount int local 0
		:memwait
            /echo Waiting for ${_spell}
			/delay 60 !${Me.SpellReady[${_spell}]}
            /echo Spell Memorized
			/if (${Me.SpellReady[${_spell}]}) {
                /return
            } else {
                |/docommand /${announceChannel} There was a timeout error trying to memorize "${_spell}"
                /return
            }
		
:nullSpell 
	/docommand /${announceChannel} Ther was an error trying to memorize a NULL spell!;
/return 

Sub Event_ChkHasSpell(_line, _sender, _eventParams)
	/echo line: ${_line} sender: ${_sender} params:${_eventParams}
	/declare clz   string local ${_eventParams.Arg[1,|]}
	/declare sName string local ${_eventParams.Arg[2,|]}
	/call chkHasSpell ${clz} "${sName}"
/return

Sub chkHasSpell(string _clz, string _spell)
	/echo checking spell _clz: ${_clz} _spell:${_spell}
	/if (${Me.Class.ShortName.Equal[${_clz}]}) {
		/if (${Me.Spell[${_spell}].ID}) {
			/docommand /${announceChannel} ${_spell} Found.
		}
	}
/return

Sub StripGem(string _gem) 
	/declare l int local ${_gem.Length}
	/declare r int local ${Math.Calc[${l} - 3]} 
	/declare g int local ${_gem.Right[${r}]}
/return ${g}

|-- Reset the timers on everything DOT/DD/STUN
Sub ResetCastTimers
	/if (${str_classArchetype.Equal[MELEE_END]}) /return
	/if (${Me.Class.ShortName.Equal[WAR]}) /return

	/declare i int local
	:nuke
	/if (!${NUKE_Total}) /goto :return 
	/varset NUKE_HasCastOnce  FALSE
	/for i 1 to ${NUKE_Total}
		/varset NUKE_${i}_HasCastOnce FALSE
	/next i

	:qnuke
	/if (!${QNUKE_Total}) /goto :return 
	/varset QNUKE_HasCastOnce  FALSE
	/for i 1 to ${QNUKE_Total}
		/varset QNUKE_${i}_HasCastOnce FALSE
	/next i

	:stuns
	/if (!${STUN_Total}) /goto :return 
	/varset STUN_HasCastOnce  FALSE
	/for i 1 to ${STUN_Total}
		/varset STUN_${i}_HasCastOnce FALSE
	/next i

	:QDOT
	/if (!${QDOT_Total}) /goto :return 
	/varset QDOT_HasCastOnce  FALSE
	/for i 1 to ${QDOT_Total}
		/varset QDOT_${i}_HasCastOnce FALSE
	/next i

	:DoT
	/if (!${DOT_Total}) /goto :return 
	/varset DOT_HasCastOnce  FALSE
	/for i 1 to ${DOT_Total}
		/varset DOT_${i}_HasCastOnce FALSE
	/next i

	/echo Casting: Reset all timers
	:return
/return 


|-- MEZ IMMUNE
Sub Event_MezImmune
	/${cmdChannel} mezimmunemob ${Target.CleanName}
/return

|-- SNARE IMMUNE-|
Sub Event_SnareImmune
	/varset bln_snareActive	TRUE
	/${cmdChannel} snareimmunemob ${Target.CleanName}
/return

Sub Event_Snareon
	/varset bln_snareActive	TRUE
	/if (${bln_snareUse}) /${cmdChannel} snared: ${Target.CleanName}
/return

Sub Event_SnareResist
	/if (${int_snareCounter} > 2) {
		/varset bln_snareActive TRUE
		/if (${bln_snareUse}) /${cmdChannel} snareimmunemob Giving Up
		/varset int_snareCounter 0
		/return
	} else {
		/varset bln_snareActive FALSE
		/varcalc int_snareCounter ${int_snareCounter}+1
		/if (${bln_snareUse}) /${cmdChannel} snare resisted LTE 3: Trying Again
	}
/return

Sub Event_SnareWornOff
	/varset bln_snareActive FALSE
/return

Sub EVENT_SetUseFastOnly(_line, _chatSender, _eventParams)
	/if (${QNUKE_Total} > 0) {
		/echo \aoSetting fast only:\ag${_eventParams}
		/varset bln_useFastOnly ${_eventParams}
	} else {
		/echo \aoSetting fast only:\ag${_eventParams}
		/varset bln_useFastOnly FALSE
	}
/return 


|-------------------------------------------------------------------------------- 
|EVENTS: RESISTS, ETC
| Set the resist type for this specific NPC (usually for Anguish)
| CSV value Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
|--------------------------------------------------------------------------------

Sub EVENT_SetResistType(_line, _chatSender, _eventParams) 
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return
	/declare _strResists string local ${_eventParams}
	/echo \awSetting resist types: \ag${_eventParams}
	
	/if (${_strResists.Equal[ALL]}) /varset _strResists ${str_defaultResistTypes}
	/call SetResistTypes ${_strResists}
/return 

Sub SetResistTypes(string _typesCSV)
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return
	/varset str_useResistTypes ${_typesCSV}
/return

Sub GetResistTypes
	| Lookup the resist type for this particular NPC - note only certain raid mobs are enumerated in the resistTypes INI

/return

Sub SetSpellSet(string _type)
	/memspellset ${_type}
	/varset str_spellset ${_type}
/return 

| Create the ability to pass a CSV|CSV list of new spells to cast for a type
| for specific events that we need to change the spell lineup for like Hanvar
|  example: /call SetSpellLineUp DOT 2  "Blood of Thule"|TRUE|8|2|TRUE|100|0|Poison|FALSE|0,"Chaos Venom"|TRUE|9|2|TRUE|100|0|Poison|FALSE|0
|  prefix count line,line
Sub SetSpellLineUp(string _type, int _count, string _blob)
	
	/call ClearSpellLineUp ${_type}

	/declare s int local 1
	/for s 1 to ${_count}
		/call AddSpellToLineup ${_type} ${s} "${_blob.Arg[${s},,]}"
	/next s
/return

Sub AddSpellToLineup(string _type, int _index, string _line)
	/echo \awAdding Spell to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
	/declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/varcalc propertyCount ${propertyCount}+1
	/declare property string local
	/declare i int local 1
	/declare j int local 1

	| Add the new spells by type 
	|_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/echo \awCreating New \ag ${_type} \awset: ${_type}_${_index}  == ${_line.Token[1,|]}
	/declare ${_type}_${_index}_Spell string outer  ${_line.Token[1,|]}
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		|/echo ${property}
		/if (${property.Find[Timer]}) {
			/declare ${_type}_${_index}${property} timer outer ${_line.Token[${j},|]}s
		|	/echo \ao${_type}_${_index}${property} \awis a timer \ar${${_type}_${_index}${property}}
		} else {
			/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
		}
		
	/next j
/return


Sub ClearSpellLineUp(string _prefix)
	/declare i int local 
	/declare j int local

	/declare propertyList string local ${${_prefix}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/declare property string local 
	/declare TypeTotal int local ${${_prefix}_Total}
	/if (!${TypeTotal}) /return
	/for i 1 to ${TypeTotal}
		/for j 1 to ${propertyCount}
			/varset property ${propertyList.Token[${j},|]}
			/deletevar ${_prefix}_${i}${property}
		/next j
	/next i
/return


| Reloads entire spell lineup from INI
Sub CreateCastingLineup 
	/declare i 							int		local

	|-- Nukes
	/declare NUKE_Total					int		outer	${Ini[INI_CHAR,NUKE,NUKE_Total,0]}	
	/declare NUKE_DelayTimer			timer	outer	${Ini[INI_CHAR,NUKE,NUKE_DelayTimer,1s]}
	/declare NUKE_HasCastOnce			bool	outer   FALSE
	/declare NUKE_Mana_Minimum			int		outer	${Ini[INI_CHAR,NUKE,NUKE_Mana_Minimum,10]}
	/declare NUKE_Mana_NamedMinimum		int		outer	${Ini[INI_CHAR,DOT,NUKE_Mana_NamedMinimum,10]} 
	/declare NUKE_StartHP				int		outer	${Ini[INI_CHAR,NUKE,NUKE_StartHP,98]}
	/declare NUKE_StopHP				int		outer	${Ini[INI_CHAR,NUKE,NUKE_StopHP,5]}
	/declare NUKE_StartHPIfNamed		int 	outer 	${Ini[INI_CHAR,NUKE,NUKE_StartHPIfNamed,100]}
	/declare NUKE_StopHPIfNamed			int 	outer 	${Ini[INI_CHAR,NUKE,NUKE_StopHPIfNamed,0]}
	/if (${NUKE_Total}) {
		/for i 1 to ${NUKE_Total} 
			/call AddSpellToLineup NUKE ${i} ${Ini[INI_CHAR,NUKE,NUKE_${i}]}
		/next i
	}

	|--AE Nuke
	/declare AENUKE_Active				bool	outer   FALSE
	/declare AENUKE_Total 				int		outer	${Ini[INI_CHAR,AENUKE,AENUKE_Total,0]}	
	/declare AENUKE_DelayTimer			timer	outer	${Ini[INI_CHAR,AENUKE,AENUKE_DelayTimer,0]}
	/declare AENUKE_Mana_Minimum		int		outer	${Ini[INI_CHAR,AENUKE,AENUKE_Mana_Minimum,10]}
	/declare AENUKE_StartHP				int		outer	${Ini[INI_CHAR,AENUKE,AENUKE_StartHP,98]}
	/declare AENUKE_StopHP				int		outer	${Ini[INI_CHAR,AENUKE,AENUKE_StopHP,0]}
	/if (${AENUKE_Total}) {
		/for i 1 to ${AENUKE_Total} 
			/call AddSpellToLineup AENUKE ${i} ${Ini[INI_CHAR,AENUKE,AENUKE_${i}]}
		/next i
	}

	|--Quick Nuke
	/declare QNUKE_Total				int		outer	${Ini[INI_CHAR,QNUKE,QNUKE_Total,0]}	
	/declare QNUKE_TotalIfNamed			int 	outer   ${Ini[INI_CHAR,QNUKE,QNUKE_Total,${QNUKE_Total}]}	
	/declare QNUKE_DelayTimer			timer	outer	${Ini[INI_CHAR,QNUKE,QNUKE_DelayTimer,1s]}
	/declare QNUKE_HasCastOnce			bool	outer   FALSE
	/declare QNUKE_Mana_Minimum			int		outer	${Ini[INI_CHAR,QNUKE,QNUKE_Mana_Minimum,10]}
	/declare QNUKE_Mana_NamedMinimum	int		outer	${Ini[INI_CHAR,QNUKE,QNUKE_Mana_NamedMinimum,10]} 
	/declare QNUKE_StartHP				int		outer	${Ini[INI_CHAR,QNUKE,QNUKE_StartHP,98]}
	/declare QNUKE_StopHP				int		outer	${Ini[INI_CHAR,QNUKE,QNUKE_StopHP,0]}
	/declare QNUKE_StartHPIfNamed		int 	outer 	${Ini[INI_CHAR,QNUKE,QNUKE_StartHPIfNamed,100]}
	/declare QNUKE_StopHPIfNamed		int 	outer 	${Ini[INI_CHAR,QNUKE,QNUKE_StopHPIfNamed,0]}
	/if (${QNUKE_Total}) {
		/for i 1 to ${QNUKE_Total} 
			/call AddSpellToLineup QNUKE ${i} ${Ini[INI_CHAR,QNUKE,QNUKE_${i}]}
		/next i	
	}

	|--Stuns
	/declare STUN_Total					int		outer	${Ini[INI_CHAR,STUN,STUN_Total,0]}	
	/declare STUN_DelayTimer			timer	outer	${Ini[INI_CHAR,STUN,STUN_DelayTimer,1s]}
	/declare STUN_HasCastOnce			bool	outer   FALSE
	/declare STUN_Mana_Minimum			int		outer	${Ini[INI_CHAR,STUN,STUN_Mana_Minimum,0]}
	/declare STUN_UseLocking			bool	outer	FALSE
	/if (${STUN_Total}) {
		/for i 1 to ${STUN_Total} 
			/call AddSpellToLineup STUN ${i} ${Ini[INI_CHAR,STUN,STUN_${i}]}
		/next i	
	}

	|-- DOTs MOAR DOTS!
	|--FAST Damage Over Time (note, not really useful on EMU due to era)
	/declare QDOT_Total					int		outer	${Ini[INI_CHAR,QDOT,QDOT_Total,0]}
	/declare QDOT_TotalIfNamed			int		outer	${Ini[INI_CHAR,QDOT,QDOT_TotalIfNamed,${QDOT_Total}]}
	/declare QDOT_DelayTimer			timer	outer	${Ini[INI_CHAR,QDOT,QDOT_DelayTimer,1s]}
	/declare QDOT_HasCastOnce			bool	outer   FALSE
	/declare QDOT_Mana_Minimum			int		outer	${Ini[INI_CHAR,QDOT,QDOT_Mana_Minimum,10]} 
	/declare QDOT_Mana_NamedMinimum		int		outer	${Ini[INI_CHAR,QDOT,QDOT_Mana_NamedMinimum,1]}
	/declare QDOT_StartHP				int		outer	${Ini[INI_CHAR,QDOT,QDOT_StartHP,94]}
	/declare QDOT_StopHP				int		outer	${Ini[INI_CHAR,QDOT,QDOT_StopHP,10]}
	/declare QDOT_StartHPIfNamed		int 	outer 	${Ini[INI_CHAR,QDOT,QDOT_StartHPIfNamed,100]}
	/declare QDOT_StopHPIfNamed			int 	outer 	${Ini[INI_CHAR,QDOT,QDOT_StopHPIfNamed,5]}
	/if (${QDOT_Total}) {
		/for i 1 to ${QDOT_Total} 
			/call AddSpellToLineup QDOT ${i} ${Ini[INI_CHAR,QDOT,QDOT_${i}]}
		/next i	
	}

	|--Damage Over Time
	/declare DOT_Total					int		outer	${Ini[INI_CHAR,DOT,DOT_Total,0]}
	/declare DOT_TotalIfNamed			int		outer	${Ini[INI_CHAR,DOT,DOT_TotalIfNamed,${DOT_Total}]}
	/declare DOT_DelayTimer				timer	outer	${Ini[INI_CHAR,DOT,DOT_DelayTimer,0]}
	/declare DOT_HasCastOnce			bool	outer   FALSE
	/declare DOT_Mana_Minimum			int		outer	${Ini[INI_CHAR,DOT,DOT_Mana_Minimum,10]} 
	/declare DOT_Mana_NamedMinimum		int		outer	${Ini[INI_CHAR,DOT,DOT_Mana_NamedMinimum,1]}
	/declare DOT_StartHP				int		outer	${Ini[INI_CHAR,DOT,DOT_StartHP,98]}
	/declare DOT_StopHP					int		outer	${Ini[INI_CHAR,DOT,DOT_StopHP,10]}
	/declare DOT_StartHPIfNamed			int 	outer 	${Ini[INI_CHAR,DOT,DOT_StartHPIfNamed,100]}
	/declare DOT_StopHPIfNamed			int 	outer 	${Ini[INI_CHAR,DOT,DOT_StopHPIfNamed,0]}
	/if (${DOT_Total}) {	
		/for i 1 to ${DOT_Total} 
			/call AddSpellToLineup DOT ${i} ${Ini[INI_CHAR,DOT,DOT_${i}]}
		/next i
	}
/return


|**
 ----------------------------
 Check Cast Target Subroutines
 ----------------------------
 This is called (looping) while a spell is being cast
 This will verify if the target is still alive while casting nukes, etc 
 allowing ducking out if npc is dead
 ----------------------------
**|
Sub chkCastTarget
	|/echo \awchkCastTarget corpse? \ag${Target.Name.Find[s_corpse]}  \awstop? \ag${int_temp_stopCastPoint}
	| I'm no longer casting, or no longer need to stop it ie, target is MT
	/call RunRapidEvents
	/if (!${Me.Casting.ID}) /goto :return

	| If the Target is Dead... stop
	/if (${Target.Name.Find[s_corpse]}) /goto :interrupt

	| If the Target has a Stop Cast HP setting (like... say balancer)
	/if (${Target.PctHPs} <= ${int_temp_stopCastPoint}) /goto :interrupt

	/goto :return

	:interrupt
	/stopcast

    :return
/return

|**
 ----------------------------
 Check Defensive Spells vs Target (JOLT)
 ----------------------------
 This method checks the target status (named, etc), and loops through the defensive spell types and children
 and calls casts if named and use conditions are met. Note: these can be AA/DISC/SPELL jolts
 ----------------------------
**|
Sub chkDefensiveSpells
    /if (!${Target.ID} || !${bln_engaged} || ${Target.Type.NotEqual[NPC]} || !${bln_cast_JOLT_enabled}) /return
    /declare i      int     local 0
    /declare j      int     local 0
    /declare dtype   string  local NULL
    /declare key    string  local NULL

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}
    
	:jolt_check
    /for i 1 to ${int_cast_JOLT_total}
        /varset dtype    JOLT

        | Check Settings for this type
        /if (!${bln_cast_${dtype}_enabled}) /continue
        /if (${timer_cast_${dtype}} || (${Me.PctMana} < ${int_cast_${dtype}_minMana} ||  ${Target.PctHPs} >= ${int_cast_${dtype}_startHP})) /continue

        | Check Each entry for this typoe
        /for j 1 to ${int_cast_${dtype}_total}
            /varset key   ${dtype}_${j}

            | Check Triggers for this entry
            /if ((${Target.PctHPs} <= ${${key}_stopHP} && ${Target.PctHPs} >= ${${key}_startHP}) && !${${key}_enabled} || (!${isNamed} && ${${key}_namedOnly}) || ${${key}_timer}) /continue
            /call attemptCastingDefensiveSpell ${key} ${key}
			/call RunAllEvents
            /varset timer_cast_${key} ${timer_cast_${key}.OriginalValue}
        /next j
    /next i
    /varset timer_cast_${dtype} ${timer_cast_${dtype}.OriginalValue}
/return

| Defensive spells can be ALT/DISC/SPELL
Sub attemptCastingDefensiveSpell(string _key) 
    /declare _type string local ${${_key}_type}

   /if (${Me.Class.ShortName.Equal[BRD]}) {
        /if (${_type.Equal[AA]}     &&   ${Me.AltAbilityReady[${_key}]})    /call MQ2Medley "${_key}" ALT  ${int_targetID} TRUE
        /if (${_type.Equal[DISC]}   && ${Me.CombatAbilityReady[${_key}]})   /doability      "${_key}"
        /if (${_type.Equal[ITEM]}   && !${FindItem[=${_key}].TimerReady})   /call MQ2Medley "${_key}" ITEM ${int_targetID} TRUE
        /if (${_type.Equal[SPELL]})                                         /call MQ2Medley "${_key}" SONG
    } else {
        /if (${_type.Equal[AA]}     && ${Me.AltAbilityReady[${_key}]})      /call MQ2Cast   "${_key}" ALT
        /if (${_type.Equal[DISC]}   && ${Me.CombatAbilityReady[${_key}]})   /doability      "${_key}"
        /if (${_type.Equal[ITEM]}   && !${FindItem[=${_key}].TimerReady})   /call MQ2Cast   "${_key}" ITEM
        /if (${_type.Equal[SPELL]})                                         /call MQ2Cast   "${_key}"
    }
    /varset ${_key}_hasCastOnce TRUE
    /varset ${_key}_timer       ${${_key}_delay} 
/return

| Note defensive settings include AA|DISC|SPELL just like other AA/DISCS
Sub processDefensiveSettings
    /echo \awprocessDefensiveSettings \ay

    /declare bln_cast_JOLT_enabled          bool    outer   FALSE
    /declare int_cast_JOLT_total            int     outer   0
    /declare timer_cast_JOLT                timer   outer   0
    /declare int_cast_JOLT_minMana          int     outer   10
    /declare int_cast_JOLT_startHP          int     outer   99
    /declare int_cast_JOLT_stopHP           int     outer   0  
    

     |; _enabled|_delay|_minMana|_startHP|_stopHP
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[1,|]})  /varset bln_cast_JOLT_enabled    ${Ini[INI_CHAR,JOLT,settings].Arg[1,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[1,|]})  /varset timer_cast_JOLT          ${Ini[INI_CHAR,JOLT,settings].Arg[2,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[3,|]})  /varset int_cast_JOLT_minMana    ${Ini[INI_CHAR,JOLT,settings].Arg[3,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[4,|]})  /varset int_cast_JOLT_startHP    ${Ini[INI_CHAR,JOLT,settings].Arg[4,|]}
    /if (${Ini[INI_CHAR,JOLT,settings].Arg[5,|]})  /varset int_cast_JOLT_stopHP     ${Ini[INI_CHAR,JOLT,settings].Arg[5,|]}

        /echo \a-w    Settings Processed: \awJOLT Enabled:\ag${bln_cast_JOLT_enabled} \awTotal:\ag${int_cast_JOLT_total} \awMinMana:\ag${int_cast_JOLT_minMana} \awStart:\ag${int_cast_JOLT_startHP} \awStop:\ag${int_cast_JOLT_stopHP}
    /if (!${bln_cast_JOLT_enabled}) {    
        /varset int_cast_JOLT_total 0
        /echo \a-w        JOLT Processed: Count:\ao[\ag${int_cast_JOLT_total}\ao] \awNo JOLT Spells
        /return
    }

   | ; _jolt|_enabled|_gem|_maxTries|_namedOnly|_startHP|_stopHP|_hasCastOnce|_delay
    /declare raw_jolt[12]		    string 	local NULL
    /declare i						int 	local 0
    /declare j                      int     local 0
	/declare sz 					int     local 0
    /declare test                   string  local   NULL

    /for i 1 to ${raw_jolt.Size}
        /varset test ${Ini[INI_CHAR,JOLT,JOLT_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset  int_cast_JOLT_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
        /declare JOLT_${i}		        string	outer   ${test.Arg[1,|]}
		/declare JOLT_${i}_type		    string	outer   ${test.Arg[2,|]}
        /declare JOLT_${i}_enabled	    string  outer   ${test.Arg[3,|]}
		/declare JOLT_${i}_gem	        string  outer   ${test.Arg[4,|]}
		/declare JOLT_${i}_maxTries	    string  outer   ${test.Arg[5,|]}
        /declare JOLT_${i}_namedOnly	string  outer   ${test.Arg[6,|]}
        /declare JOLT_${i}_startHP	    string  outer   ${test.Arg[7,|]}
        /declare JOLT_${i}_stopHP	    string  outer   ${test.Arg[8,|]}
        /declare JOLT_${i}_hasCastOnce  string  outer   ${test.Arg[9,|]}
        /declare JOLT_${i}_delay	    string  outer   ${test.Arg[10,|]}
        /declare JOLT_${i}_timer        timer   outer   ${JOLT${i}_delay}	
        /echo \ao        JOLT[\ag${i}\ao]:\ag${JOLT_${i}} \awType:\ag${JOLT_${i}_type} \awUse:\ag${JOLT_${i}_enabled} 
    /next i


/return 

Sub xbot_initialize_casting
	/echo \aoxbot_initialize_casting

	|- [DEFAULT_SETTINGS] : Casting 
	|-- Initial Spellset
	/declare	str_spellset				string  outer 	default

	|-- Misc (resist types, fast only, etc)
	/declare str_defaultResistTypes	string	outer	Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
	/declare str_useResistTypes		string	outer	${str_defaultResistTypes}
	/if (${Zone.ShortName.Equal[pofire]}) /call SetResistTypes Cold,Magic,Poison,Disease
	/declare bln_useFastOnly			bool	outer	FALSE
	/declare int_concCount				int     outer 	0
	/declare int_temp_stopCastPoint		int 	outer   0

	
	|-- Setup defensive AA/SPELL/DISC
	/call processDefensiveSettings

	|-- Return out if not a caster
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return

	|-- Templates
	/declare DOT_Template				string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare QDOT_Template				string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare NUKE_Template 				string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare QNUKE_Template 			string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare AENUKE_Template	 		string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare STUN_Template				string outer	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer	
	/declare JOLT_Template				string outer	_jolt|_type|_enabled|_gem|_maxTries|_namedOnly|_startHP|_stopHP|_hasCastOnce|_delay

	|-- Load the spells set in the INI file
	/call CreateCastingLineup
	
	
	/call InitEvents_Casting
	/echo \awNukes: \ag${NUKE_Total} \awAENukes: \ag${AENUKE_Total} \awQNuke: \ag${QNUKE_Total} \awStun: \ag${STUN_Total} \awDOT: \ag${DOT_Total} \awJOLT: \ag${int_cast_JOLT_total} 


/return
	
	
	
	