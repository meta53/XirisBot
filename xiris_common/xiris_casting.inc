|**
----------------------------
SPELL/CASTING HANDLERS
----------------------------
This library supplements/abstracts mq2cast functionality
______________________________________________
REVISION HISTORY
    11.09.17    xiris	INITIAL REVISION - Handling of clickies and other things
	08.02.18	xiris	Added QuickNukes and AENukes
	03.31.22	xiris	Updated the named checking as well as normalizing all calls to MQ2Cast
	04.16.22	xiris	Added chkStun and support for Stun Locking
	05.08.22	xiris	Added support for bards in Nuke/DOT
	06.13.22	xiris   Removed resist events as the xiris_spell_routines.inc handles that.
**|



|NECRO/DOT/DEBUFF class stuff
#event DotOff1				"#*#Your |${DOT_1}| spell has worn off#*#"
#event DotOff2				"#*#Your |${DOT_2}| spell has worn off#*#"
#event DotOff3				"#*#Your |${DOT_3}| spell has worn off#*#"
#event DotOff4				"#*#Your |${DOT_4}| spell has worn off#*#"
#event DotOff5				"#*#Your |${DOT_5}| spell has worn off#*#"
#event DotOff6				"#*#Your |${DOT_6}| spell has worn off#*#"
#event DotOff7				"#*#Your |${DOT_7}| spell has worn off#*#"
#event DotOff8				"#*#Your |${DOT_8}| spell has worn off#*#"
#event DotOff9				"#*#Your |${DOT_9}| spell has worn off#*#"
#event DotOff10				"#*#Your |${DOT_10}| spell has worn off#*#"
#event DotOff11				"#*#Your |${DOT_11}| spell has worn off#*#"
#event DotOff12				"#*#Your |${DOT_12}| spell has worn off#*#"

#event Slow					"#1#is hindered by a shackle of spirit#*#"
#event SlowResist			"#*#Your target resisted the |${str_slowSpell}|#*#"
#event Snareon				"#*#damage by your |${str_snareSpell}|#*#"
#event Snareon				"#*#engulfed by darkness#*#"
#event Snareon				"#*#engulfed in an embracing darkness#*#"
#event Snareon				"#*#is shackled to the ground#*#"
#event Snareon				"#*#has been ensnared#*#"
#event Snareon				"#*#movements slow as their feet are covered in tangling weeds#*#"
#event Snareon				"#*#has been entangled#*#"
#event Snareon				"#*#has been entrapped in living earth#*#"
#event Snareon				"#*##covered in coruscating black shadows#*#"
#event SnareResist			"#*#Your target resisted the |${Snare}|#*#"
#event SnareWornOff			"Your |${str_snareSpell}| spell has worn off#*#"
#event NoSnare         		"#*#is immune to changes in its#*#"
#event NoSnareHold     		"#*#Your spell did not take hold.#*#"

|SPELL Checking stuff
#event ChkHasSpell           "#*#ChkHasSpell #2#"

|RESIST TYPE stuff
#event SetResistType     	"#*#ResistTypes #1#"

|SETTING Triggers
#event SetUseFastOnly		"#*#UseFastOnly #1#"


Sub InitEvents_Casting
	/call RegisterBackgroundEvents Events_Casting_Background 2
	
	/if (${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[ENC]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[NEC]}) {
		/call RegisterClassEvents Events_Casting_DOT 12
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
	/if (!${bln_engaged}) 		/return
	/if (${bln_useFastOnly})	/return
	/if (${NUKE_Total} == 0) /return 
	/if (${AENUKE_Active}) 		/return
	/if (${NUKE_DelayTimer} && ${NUKE_HasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return

	|-- press pet attack for funsies
	/if (${Me.Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

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
			|-- skip if not ready
			/if (!${Me.SpellReady[${NUKE_${i}_Spell}]}) /goto :NextNuke
			|-- skip if disabled
			/if (!${NUKE_${i}_Use}) /goto :NextNuke

			|-- Check the timer on this nuke
			|/if (${NUKE_${i}_Timer} <= 0) 
			/if (${NUKE_${i}_Timer} > 0 && ${NUKE_${i}_HasCastOnce}) {
				/if (${i}<${NUKE_Total}) { 
					/cecho \arTimer not ready for \ay${NUKE_${i}_Spell} :: \ay${NUKE_${i}_Timer}
					/goto :NextNuke
				} else {
					/return
				}
			}

			
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
			/if (!${isNamed} && ${NUKE_${i}_NamedOnly}) {
				/echo Skipping Nuke ${NUKE_${i}_Spell} is named only
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
			/if (!${Me.SpellReady[${NUKE_${i}_Spell}]} || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :NukeCastWait
			}

		
			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${NUKE_${i}_StartHP} && ${Target.PctHPs}>=${NUKE_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${NUKE_${i}_Spell}].Mana} && ${Target.Distance}<=${Spell[${NUKE_${i}_Spell}].Range} && ${Target.LineOfSight}) {
					/cecho \agTimer ready for \ay${NUKE_${i}_Spell} :: \ay${NUKE_${i}_Timer} \awSpellReady?\ag${Me.SpellReady[${NUKE_${i}_Spell}]} ${Target.PctHPs}<=${NUKE_${i}_StartHP} && ${Target.PctHPs}>=${NUKE_${i}_StopHP})	
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${NUKE_${i}_Spell}"
						/cecho \ag ${NUKE_${i}_Spell}  : DELAYING for \ay${NUKE_${i}_Timer.OriginalValue}s \agrefresh	
					} else {
						/call MQ2Cast "${NUKE_${i}_Spell}"	
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${NUKE_${i}_Spell} ${Macro.Return} : DELAYING for \ay${NUKE_${i}_Timer.OriginalValue}s \agrefresh	
						}
					}

					/if (${NUKE_${i}_Delay} > 0) {
						/varset NUKE_${i}_Timer	${NUKE_${i}_Delay}
					} else {
						/varset NUKE_${i}_Timer	${NUKE_${i}_Timer.OriginalValue}
					}
					
					/varset NUKE_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextNuke
			
		/next i
	}
	/varset NUKE_HasCastOnce TRUE
	/varset NUKE_DelayTimer ${NUKE_DelayTimer.OriginalValue}
/return

Sub chkQNuke
	/if (!${bln_engaged}) 		 /return
	/if (${QNUKE_Total} == 0) /return 
	/if (${AENUKE_Active}) 		 /return
	/if (${QNUKE_DelayTimer} && ${QNUKE_HasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- press pet attack for funsies
	/if (${Me.Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

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
			|-- skip if not ready
			/if (!${Me.SpellReady[${QNUKE_${i}_Spell}]}) /goto :NextQuickNuke
			|-- skip if disabled
			/if (!${QNUKE_${i}_Use}) /goto :NextQuickNuke

			|-- Check the timer on this nuke
			/if (${QNUKE_${i}_Timer} == 0) /cecho \agTimer ready for \ay${QNUKE_${i}_Spell} :: \ay${QNUKE_${i}_Timer}
			/if (${QNUKE_${i}_Timer} > 0 && ${QNUKE_${i}_HasCastOnce}) {
				/if (${i}<${QNUKE_Total}) { 
					/cecho \arTimer not ready for \ay${QNUKE_${i}_Spell} :: \ay${QNUKE_${i}_Timer}
					/goto :NextQuickNuke
				} else {
					/return
				}
			}
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
	
			/if (!${isNamed} && ${QNUKE_${i}_NamedOnly}) {
				/echo Skipping QuickNuke ${QNUKE_${i}_Spell} is named only
				/goto :NextQuickNuke
			}

			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${QNUKE_${i}_Resist}]}]} || ${QNUKE_${i}_Resist.Equal[Unresistable]}) {
				|/echo QuickNuke ${QNUKE_${i}_Resist} matches resist
			} else {
				/echo Skipping QuickNuke ${QNUKE_${i}_Resist} doesnt match resist
				/goto :NextQuickNuke
			}
			
			/call RunAllEvents
			
			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping		
			/call chkSpellMemorized "${QNUKE_${i}_Spell}" FALSE ${QNUKE_${i}_Gem}
			:QnukeNukeCastWait
			/if (!${Me.SpellReady[${QNUKE_${i}_Spell}]} || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :QnukeNukeCastWait
			}
			
			
			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${QNUKE_${i}_StartHP} && ${Target.PctHPs}>=${QNUKE_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${QNUKE_${i}_Spell}].Mana} && ${Target.Distance}<=${Spell[${QNUKE_${i}_Spell}].Range} && ${Target.LineOfSight}) {
					
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${QNUKE_${i}_Spell}"
						/cecho \ag ${QNUKE_${i}_Spell}  : DELAYING for \ay${QNUKE_${i}_Timer.OriginalValue}s \agrefresh	
					} else {
						/call MQ2Cast "${QNUKE_${i}_Spell}"	
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${QNUKE_${i}_Spell} ${Macro.Return} : DELAYING for \ay${QNUKE_${i}_Timer.OriginalValue}s \agrefresh	
						}
					}

					/if (${QNUKE_${i}_Delay} > 0) {
						/varset QNUKE_${i}_Timer	${QNUKE_${i}_Delay}
					} else {
						/varset QNUKE_${i}_Timer	${QNUKE_${i}_Timer.OriginalValue}
					}

					/varset QNUKE_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextQuickNuke
		/call RunAllEvents
			
		/next i
	}
	/varset QNUKE_HasCastOnce TRUE
	/varset QNUKE_DelayTimer ${QNUKE_DelayTimer.OriginalValue}
/return

|**
 ----------------------------
 AE Direct Damage  
 ----------------------------
 Loops through the AE nuke list and applies, doesn't do much checking because
 this is usually triggered via an event
 ----------------------------
**|
Sub chkAENuke
	/if (!${AENUKE_Active}) /return
	|-- AE Nuke is active, let's light them up!
	/rs Lighting them up with ${int_AENukeTotal} AE Nukes!

	/declare i int local
	:AENukeLoop
	/for i 1 to ${int_AENukeTotal}
		:AENukeStart 
		|-- skip if not ready
		/if (!${Me.SpellReady[${AENUKE_${i}_Spell}]}) /goto :NextAENuke

		/if (${AENUKE_${i}_Timer} == 0) /cecho \agTimer ready for \ay${AENUKE_${i}_Spell} :: \ay${AENUKE_${i}_Timer}
		/if (${AENUKE_${i}_Timer} > 0 && ${AENUKE_${i}_HasCastOnce}) {
			/if (${i}<${int_AENukeTotal}) { 
				/cecho \arTimer not ready for \ay${AENUKE_${i}_Spell} :: \ay${AENUKE_${i}_Timer}
				/goto :NextAENuke
			}
		}
		/call chkSpellMemorized "${AENUKE_${i}_Spell}" TRUE ${AENUKE_${i}_Gem}
		:AENukeCastWait
		/if (!${Me.SpellReady[${AENUKE_${i}_Spell}]} || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
			/delay 2
			/goto :AENukeCastWait
		}

		/call MQ2Cast "${AENUKE_${i}_Spell}"	
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
			/cecho \ag ${AENUKE_${i}_Spell} ${Macro.Return} : DELAYING for \ay${AENUKE_${i}_Timer.OriginalValue}s \agrefresh	
		}

		/if (${AENUKE_${i}_Delay} > 0) {
			/varset AENUKE_${i}_Timer	${AENUKE_${i}_Delay}
		} else {
			/varset AENUKE_${i}_Timer	${AENUKE_${i}_Timer.OriginalValue}
		}
		/varset AENUKE_${i}_HasCastOnce TRUE
	:NextAENuke
	/call RunAllEvents
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
	/if (!${bln_engaged}) 		 /return
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
			|-- skip if not ready
			/if (!${Me.SpellReady[${STUN_${i}_Spell}]}) /goto :NextStun
			|-- skip if disabled
			/if (!${STUN_${i}_Use}) /goto :NextStun

			
			/call RunAllEvents	
			/call chkSpellMemorized "${STUN_${i}_Spell}" ${_memorize} ${STUN_${i}_Gem}

			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${STUN_${i}_StartHP} && ${Target.PctHPs}>=${STUN_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${STUN_${i}_Spell}].Mana} && ${Target.Distance}<=${Spell[${STUN_${i}_Spell}].Range} && ${Target.LineOfSight}) {
					/call MQ2Cast "${STUN_${i}_Spell}" ${STUN_${i}_Gem} 5s
					/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
						/cecho \ag ${STUN_${i}_Spell} ${Cast.Result}
						/varset STUN_${i}_HasCastOnce TRUE
					}
				}
			}
		:NextStun
		/call RunAllEvents
			
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
		/call RunAllEvents
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

	/if (${bln_useFastOnly}) 	/return
	/if (${AENUKE_Active}) 		/return
	/if (!${bln_engaged}) 		/return
	/if (${DOT_Total} == 0)	/return 
	/if (${DOT_DelayTimer} && ${DOT_HasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- press pet attack for funsies
	/if (${Me.Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

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
			|-- skip if not ready
			/if (!${Me.SpellReady[${DOT_${i}_Spell}]}) /goto :NextDot
			|-- skip if disabled
			/if (!${DOT_${i}_Use}) /goto :NextDot

			|-- Check the timer on this dot
			/if (${DOT_${i}_RecastTimer} <= 0) /cecho \agTimer ready for \ay${DOT_${i}_Spell} :: \ay${DOT_${i}_RecastTimer}
			/if (${DOT_${i}_RecastTimer} > 0 && ${DOT_${i}_HasCastOnce}) {
				/if (${i}<${DOT_Total}) { 
					/cecho \arTimer not ready for \ay${DOT_${i}_Spell} :: \ay${DOT_${i}_RecastTimer}
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
			
			/call RunAllEvents
			

			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping
			/call chkSpellMemorized "${DOT_${i}_Spell}" FALSE ${DOT_${i}_Gem}
			:DotReadyWait
			/if ((!${Me.SpellReady[${DOT_${i}_Spell}]} || (${Me.Casting.ID}) && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :DotReadyWait
			}

			|-- All this ready, check for LOS/Distance/Mana
			|-- Think about adding the named stop point here at some point in future
			/if (${Target.PctHPs}<=${DOT_${i}_StartHP} && ${Target.PctHPs}>=${DOT_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${DOT_${i}_Spell}].Mana} && (${Me.SpellReady[${DOT_${i}_Spell}]} || ${Me.Class.ShortName.Equal[BRD]}) && ${Target.Distance}<=${Spell[${DOT_${i}_Spell}].Range} && ${Target.LineOfSight} && (${DOT_${i}_RecastTimer} <= 0)) {
					/varcalc dotDuration ${Spell[${DOT_${i}_Spell}].Duration} * 6
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${DOT_${i}_Spell}"
						/cecho \ag ${DOT_${i}_Spell}  : DELAYING for \ay${dotDuration}s \agrefresh	
					} else {
						/call MQ2Cast "${DOT_${i}_Spell}"	
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${DOT_${i}_Spell} ${Macro.Return} : DELAYING for \ay${dotDuration}s \agrefresh	
						}
					}
					/varset DOT_${i}_RecastTimer	${dotDuration}s
					/varset DOT_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextDot
		/call RunAllEvents
			
		/next i
	}
	/varset DOT_HasCastOnce TRUE
	/varset DOT_DelayTimer ${DOT_DelayTimer.OriginalValue}
/return


|**
 ----------------------------
 Snaring  
 ----------------------------
 Snare Spell is declared in the INI
 ----------------------------
**|
Sub chkSnare
	/if (${Me.Invis}||${Me.Moving}|| ${Me.Song[${GoM}].ID}||${list_immuneSnare.Find[${Spawn[${Target.CleanName}]}]}||${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	/if (${Target.PctHPs}<=${int_snareHP} && ${Target.ID} && ${Target.Type.Equal[NPC]} && !${bln_snareActive} && ${bln_snareUse} && ${SpawnCount[npc radius 50 zradius 10]}<=${int_snareLocalNPC}) {
		/call chkSpellMemorized "${str_snareSpell}" TRUE ${str_snareGem} 
		/call MQ2Cast "${str_snareSpell}"
	}
	
	/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
		/cecho \ag ${str_snareSpell} ${Cast.Result}
		/varset bln_snareActive TRUE
	}
	
	/if (${Cast.Result.Equal[CAST_IMMUNE]}) {
		/cecho \ar ${str_snareSpell} ${Cast.Result}
		/ini "${IniFile}" "SnareImmunes.${Zone.ShortName}" "SnareImmune" "${list_immuneSnare}${Target.CleanName}|"
		/varset list_immuneSnare ${Ini[${ignoreINI},"SnareImmunes.${Zone.ShortName}","SnareImmune"]}
		/varset bln_snareActive TRUE
	}
	/call RunAllEvents
			
/return




|- Method will memorize a spell, to the optional attributed gem, and if wait is true, will wait until its ready to return
|- note will call utility to string 'gem' from the gem # if found
Sub chkSpellMemorized(string _spell, bool _wait, int _gem)
	/if (${_spell.Equal[NULL]}) /goto :return
	/if (${Me.Gem[${_spell}]}) /goto :return
	/echo chkSpellMemorized: ${_spell} (${Me.Gem[${_spell}]}) 
	/declare intGem int local 9
	/if (${_gem} > 0) {
		/echo integer
		/varset intGem ${_gem}
		/goto :mem
	} else /if (${_gem} == 0) {
		/goto :mem
	}
	/if (${_gem.Length} == 0) {
		| gem is undefined so default 9
		/varset intGem  9
	} else {
		/if (${_gem.Find[gem]}) {
			| gem was given as a string
			/call StripGem ${_gem}
			/varset intGem ${Macro.Return}
		} else {
			| gem was given as an integer
			/varset intGem ${_gem}
		}
	}
	:mem
		/echo memorizing "${_spell}" ${intGem} 
		/memorize "${_spell}" ${intGem} 
		/if (!${_wait}) /goto :return
		/declare waitCount int local 0
		:memwait
			/delay 10
			/varcalc waitCount ${waitCount}+1
			/if (${Me.SpellReady[${_spell}]}) /goto :return
			/if (${waitCount} > 10) {
				/rs cannot memorize ${_spell}! Update INI
				/goto :return
			}
			/goto :memwait
:nullError 
/rs There was a null error trying to memorize a spell; Check your scope!
:return
/return 

Sub Event_ChkHasSpell(_line, _sender, _eventParams)
	/echo line: ${_line} sender: ${_sender} params:${_eventParams}
	/doevents flush ChkHasSpell 
	/declare clz   string local ${_eventParams.Arg[1,|]}
	/declare sName string local ${_eventParams.Arg[2,|]}
	/call chkHasSpell ${clz} "${sName}"
/return

Sub chkHasSpell(string _clz, string _spell)
	/echo checking spell _clz: ${_clz} _spell:${_spell}
	/if (${Me.Class.ShortName.Equal[${_clz}]}) {
		/if (${Bool[${Me.Spell[${_spell}].ID}]}) {
			/rs ${_spell} Found.
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
	/if (!${Bool[${NUKE_Total}]}) /goto :return 
	/varset NUKE_HasCastOnce  FALSE
	/for i 1 to ${NUKE_Total}
		/varset NUKE_${i}_HasCastOnce FALSE
	/next i

	:qnuke
	/if (!${Bool[${QNUKE_Total}]}) /goto :return 
	/varset QNUKE_HasCastOnce  FALSE
	/for i 1 to ${QNUKE_Total}
		/varset QNUKE_${i}_HasCastOnce FALSE
	/next i

	:stuns
	/if (!${Bool[${STUN_Total}]}) /goto :return 
	/varset STUN_HasCastOnce  FALSE
	/for i 1 to ${STUN_Total}
		/varset STUN_${i}_HasCastOnce FALSE
	/next i

	:fastDoT
	/if (!${Bool[${QDOT_Total}]}) /goto :return 
	/varset QDOT_HasCastOnce  FALSE
	/for i 1 to ${QDOT_Total}
		/varset QDOT_${i}_HasCastOnce FALSE
	/next i

	:DoT
	/if (!${Bool[${DOT_Total}]}) /goto :return 
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
	/doevents flush MezImmune
/return

|-- SNARE IMMUNE-|
Sub Event_SnareImmune
	/varset bln_snareActive	TRUE
	/${cmdChannel} snareimmunemob ${Target.CleanName}
	/doevents flush SnareImmune
/return

Sub Event_Snareon
	/varset bln_snareActive	TRUE
	/if (${bln_snareUse}) /${cmdChannel} snared: ${Target.CleanName}
	/doevents flush Snareon
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
	/doevents flush SnareResist
/return

Sub Event_SnareWornOff
	/varset bln_snareActive FALSE
	/doevents flush SnareWornOff
/return

Sub EVENT_SetUseFastOnly(_chatText, _use)
	/if (${QNUKE_Total} > 0) {
		/echo setting fast only:${_use}
		/varset bln_useFastOnly ${_use}
	} else {
		/varset bln_useFastOnly FALSE
	}
	/doevents flush SetUseFastOnly
/return 



|-------------------------------------------------------------------------------- 
|EVENTS: RESISTS, ETC
| Set the resist type for this specific NPC (usually for Anguish)
| CSV value Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
|--------------------------------------------------------------------------------

Sub EVENT_SetResistType(_chatText, _strResists) 
	/cecho \awSetting resist types: \ag${_strResists}
	/if (${_strResists.Equal[ALL]}) /varset _strResists ${str_defaultResistTypes}
	/call SetResistTypes ${_strResists}
	/doevents flush SetResistType
/return 

Sub SetResistTypes(string _typesCSV)
	/varset str_useResistTypes ${_typesCSV}
/return

Sub GetResistTypes
	| Lookup the resist type for this particular NPC - note only certain raid mobs are enumerated in the resistTypes INI

/return


| Create the ability to pass a CSV|CSV list of new spells to cast for a type
| for specific events that we need to change the spell lineup for like Hanvar
|  example: /call setSpellLineUp DOT 2  "Blood of Thule"|TRUE|8|2|TRUE|100|0|Poison|FALSE|0,"Chaos Venom"|TRUE|9|2|TRUE|100|0|Poison|FALSE|0
|  prefix count line,line
Sub setSpellLineUp(string _type, int _count, string _blob)
	
	/call clearSpellLineUp ${_type}

	/declare s int local 1
	/for s 1 to ${_count}
		/call addSpellToLineup ${_type} ${s} "${_blob.Arg[${s},,]}"
	/next s
/return

Sub addSpellToLineup(string _type, int _index, string _line)
	/cecho \awAdding Spell to Lineup: \aoType:\ag${_type} \aoIndex:\ag${_index} \aoArgs:\ag${_line}
	/declare propertyList string local ${${_type}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/declare property string local
	/declare i int local 1
	/declare j int local 1

	| Add the new spells by type 

	/cecho \awCreating New \ag ${_type} \awset: ${_type}_${_index}  == ${_line.Token[1,|]}
	
	/for j 2 to ${propertyCount}
		/varset property ${propertyList.Token[${j},|]} 
		|/echo prop: ${property} line: ${_line.Arg[${j},|]}
		/declare ${_type}_${_index}${property} string outer ${_line.Token[${j},|]}
		/echo ${_type}_${_index}${property}=${_line.Arg[${j},|]}
	/next j


/return


Sub clearSpellLineUp(string _prefix){
	/declare i int local 
	/declare j int local
	
	/cecho \aoClearSpellLineup \ay${_prefix}
	/cecho \awTemplate: ${${_prefix}_Template}
	
	/declare propertyList string local ${${_prefix}_Template}
	/declare propertyCount int local ${propertyList.Count[|]}
	/declare property string local 
	/declare TypeTotal int local ${${_prefix}_Total}

	/echo ${_prefix}_Total: ${TypeTotal}

	/for i 1 to ${TypeTotal}
		/echo Deleting set: [${_prefix}_${i}=${${_prefix}_${i}_Spell}]
		
		/for j 1 to ${propertyCount}
			| property of type, eg: DOT_1_Use
			|                        prefix_index_property
			/varset property ${propertyList.Token[${j},|]}
			/echo prop "${property}" Deleting key: ${_prefix}_${i}${property} value: ${${_prefix}_${i}${property}}
			/deletevar ${_prefix}_${i}${property}
		/next j
	/next i
/return



Sub xbot_initialize_casting
	/if (${str_classArchetype.Equal[MELEE_END]} || ${Me.Class.ShortName.Equal[WAR]}) /return

	/cecho \aoxbot_initialize_casting

	/declare	str_spellset				string  outer 	default
	|-- Misc (resist types, fast only, etc
	/declare str_defaultResistTypes	string	outer	Chromatic,Corruption,Cold,Disease,Fire,Magic,Poison,Prismatic
	/declare str_useResistTypes		string	outer	${str_defaultResistTypes}
	/if (${Zone.ShortName.Equal[pofire]}) /varset str_useResistTypes Cold,Magic,Poison,Disease
	
	/declare bln_useFastOnly	bool	outer	FALSE
	/declare int_concCount		int     outer 	0

	|-- Templates
	/declare DOT_Template				string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare NUKE_Template 				string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare QNUKE_Template 			string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	/declare AENUKE_Template	 		string outer  	_Spell|_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	
	|-- Nukes
	/declare NUKE_Total					int		outer	${Ini[botINI,DirectDamage,NUKE_Total,0]}	
	/declare NUKE_DelayTimer			timer	outer	${Ini[botINI,DirectDamage,NUKE_DelayTimer,1s]}
	/declare NUKE_HasCastOnce			bool	outer   FALSE
	/declare NUKE_Mana_Minimum			int		outer	${Ini[botINI,DirectDamage,NUKE_Mana_Minimum,10]}
	/declare NUKE_Mana_NamedMinimum		int		outer	${Ini[botINI,DamageOverTime,NUKE_Mana_NamedMinimum,10]} 
	/declare NUKE_StartHP				int		outer	${Ini[botINI,DirectDamage,NUKE_StartHP,98]}
	/declare NUKE_StopHP				int		outer	${Ini[botINI,DirectDamage,NUKE_StopHP,5]}
	/declare NUKE_StartHPIfNamed		int 	outer 	${Ini[botINI,DirectDamage,NUKE_StartHPIfNamed,100]}
	/declare NUKE_StopHPIfNamed			int 	outer 	${Ini[botINI,DirectDamage,NUKE_StopHPIfNamed,0]}

	/declare nc 						int		local
	/for nc 1 to ${NUKE_Total} 
		/declare NUKE_${nc}_Spell			string	outer	${Ini[botINI,DirectDamage,NUKE_${nc}]}
		/declare NUKE_${nc}_Use 			bool	outer	${Ini[botINI,DirectDamage,NUKE_${nc}_Use]}
		/declare NUKE_${nc}_Gem 			string	outer	${Ini[botINI,DirectDamage,NUKE_${nc}_Gem]}
		/declare NUKE_${nc}_StartHP			int		outer	${Ini[botINI,DirectDamage,NUKE_${nc}_StartHP]}
		/declare NUKE_${nc}_StopHP			int		outer	${Ini[botINI,DirectDamage,NUKE_${nc}_StopHP]}
		/declare NUKE_${nc}_Resist      	string  outer   ${Spell[${NUKE_${nc}_Spell}].ResistType} 
		/declare NUKE_${nc}_NamedOnly		string  outer   ${Ini[botINI,DirectDamage,NUKE_${nc}_NamedOnly]}
		/declare NUKE_${nc}_Timer			timer   outer   ${NUKE_DelayTimer}
		/declare NUKE_${nc}_HasCastOnce		bool	outer   FALSE
		/declare NUKE_${nc}_Delay   		int     outer   ${Ini[botINI,DirectDamage,NUKE_${nc}_Delay,0]}
	/next nc

	|--AE Nuke
	/declare AENUKE_Total 				int		outer	${Ini[botINI,AENuke,int_AENUKE_Total,0]}	
	/declare AENUKE_DelayTimer			timer	outer	${Ini[botINI,AENuke,AENUKE_DelayTimer,0]}

	/declare ae 						int		local
	/for ae 1 to ${AENUKE_Total} 
		/declare AENUKE_${ae}_Spell			string	outer	${Ini[botINI,AENuke,AENUKE_${ae}]}
		/declare AENUKE_${ae}_Use 			bool	outer	${Ini[botINI,AENuke,AENUKE_${ae}_Use]}
		/declare AENUKE_${ae}_Gem 			string	outer	${Ini[botINI,AENuke,AENUKE_${ae}_Gem]}
		/declare AENUKE_${ae}_StartHP		int		outer	${Ini[botINI,AENuke,AENUKE_${ae}_StartHP,0]}
		/declare AENUKE_${ae}_StopHP		int		outer	${Ini[botINI,AENuke,AENUKE_${ae}_StopHP,0]}
		/declare AENUKE_${ae}_Resist    	string  outer   ${Spell[${AENUKE_${ae}_Spell}].ResistType} 
		/declare AENUKE_${ae}_NamedOnly 	string  outer   ${Ini[botINI,AENuke,AENUKE_${ae}_NamedOnly,FALSE]}
		/declare AENUKE_${ae}_Timer			timer   outer   ${AENUKE_DelayTimer}
		/declare AENUKE_${ae}_HasCastOnce	bool	outer   FALSE
		/declare AENUKE_${ae}_Delay   		int     outer   ${Ini[botINI,AENuke,AENUKE_${ae}_Delay,0]}
	/next ae


	|--Quick Nuke
	/declare QNUKE_Total				int		outer	${Ini[botINI,QuickNuke,QNUKE_Total,0]}	
	/declare QNUKE_TotalIfNamed			int 	outer   ${Ini[botINI,QuickNuke,QNUKE_Total,${QNUKE_Total}]}	
	/declare QNUKE_DelayTimer			timer	outer	${Ini[botINI,QuickNuke,QNUKE_DelayTimer,1s]}
	/declare QNUKE_HasCastOnce			bool	outer   FALSE
	/declare QNUKE_Mana_Minimum			int		outer	${Ini[botINI,QuickNuke,QNUKE_Mana_Minimum,10]}
	/declare QNUKE_Mana_NamedMinimum	int		outer	${Ini[botINI,QuickNuke,QNUKE_Mana_NamedMinimum,10]} 
	/declare QNUKE_StartHP				int		outer	${Ini[botINI,QuickNuke,QNUKE_StartHP,98]}
	/declare QNUKE_StopHP				int		outer	${Ini[botINI,QuickNuke,QNUKE_StopHP,0]}
	/declare QNUKE_StartHPIfNamed		int 	outer 	${Ini[botINI,QuickNuke,QNUKE_StartHPIfNamed,100]}
	/declare QNUKE_StopHPIfNamed		int 	outer 	${Ini[botINI,QuickNuke,QNUKE_StopHPIfNamed,0]}

	/declare qn 						int		local
	/for qn 1 to ${QNUKE_Total} 
		/declare QNUKE_${qn}_Spell 			string	outer	${Ini[botINI,QuickNuke,QNUKE_${qn}]}
		/declare QNUKE_${qn}_Use 			bool	outer	${Ini[botNI,QuickNuke,QNUKE_${qn}_Use,TRUE]}
		/declare QNUKE_${qn}_Gem 			string	outer	${Ini[botINI,QuickNuke,QNUKE_${qn}_Gem]}
		/declare QNUKE_${qn}_StartHP		int		outer	${Ini[botINI,QuickNuke,QNUKE_${qn}_StartHP]}
		/declare QNUKE_${qn}_StopHP			int		outer	${Ini[botINI,QuickNuke,QNUKE_${qn}_StopHP]}
		/declare QNUKE_${qn}_Resist     	string  outer   ${Spell[${QNUKE_${qn}_Spell}].ResistType} 
		/declare QNUKE_${qn}_NamedOnly 		string	outer	${Ini[botINI,QuickNuke,QNUKE_${qn}_NamedOnly]}
		/declare QNUKE_${qn}_Timer			timer   outer   ${QNUKE_DelayTimer}
		/declare QNUKE_${qn}_HasCastOnce	bool	outer   FALSE
		/declare QNUKE_${qn}_Delay   		int     outer   ${Ini[botINI,QuickNuke,QNUKE_${qn}_Delay,0]}
	/next qn	

	|--Stuns
	/declare STUN_Total					int		outer	${Ini[botINI,Stun,STUN_Total,0]}	
	/declare STUN_DelayTimer			timer	outer	${Ini[botINI,Stun,STUN_DelayTimer,1s]}
	/declare STUN_HasCastOnce			bool	outer   FALSE
	/declare STUN_Mana_Minimum			int		outer	${Ini[botINI,Stun,STUN_Mana_Minimum,0]}
	/declare STUN_UseLocking			bool	outer	FALSE

	/declare st 						int		local
	/for st 1 to ${STUN_Total} 
		/declare STUN_${st}_Spell 			string	outer	${Ini[botINI,Stun,STUN_${st}]}
		/declare STUN_${st}_Use 			bool	outer	${Ini[botNI,Stun,STUN_${st}_Use,TRUE]}
		/declare STUN_${st}_Gem 			string	outer	${Ini[botINI,Stun,STUN_${st}_Gem]}
		/declare STUN_${st}_StartHP			int		outer	${Ini[botINI,Stun,STUN_${st}_StartHP]}
		/declare STUN_${st}_StopHP			int		outer	${Ini[botINI,Stun,STUN_${st}_StopHP]}
		/declare STUN_${st}_Resist 			string  outer   ${Spell[${STUN_${st}_Spell}].ResistType} 
		/declare STUN_${st}_Timer			timer   outer   ${STUN_DelayTimer}
		/declare STUN_${st}_HasCastOnce		bool	outer   FALSE
	/next st	
	
	|-- DOTs MOAR DOTS!
	|--FAST Damage Over Time
	/declare QDOT_Total					int		outer	${Ini[botINI,FastDOT,QDOT_Total,0]}
	/declare QDOT_TotalIfNamed			int		outer	${Ini[botINI,FastDOT,QDOT_TotalIfNamed,${QDOT_Total}]}
	/declare QDOT_DelayTimer			timer	outer	${Ini[botINI,FastDOT,QDOT_DelayTimer,1s]}
	/declare QDOT_HasCastOnce			bool	outer   FALSE
	/declare QDOT_Mana_Minimum			int		outer	${Ini[botINI,FastDOT,QDOT_Mana_Minimum,10]} 
	/declare QDOT_Mana_NamedMinimum		int		outer	${Ini[botINI,FastDOT,QDOT_Mana_NamedMinimum,1]}
	/declare QDOT_StartHP				int		outer	${Ini[botINI,FastDOT,QDOT_StartHP,94]}
	/declare QDOT_StopHP				int		outer	${Ini[botINI,FastDOT,QDOT_StopHP,10]}
	/declare QDOT_StartHPIfNamed		int 	outer 	${Ini[botINI,FastDOT,QDOT_StartHPIfNamed,100]}
	/declare QDOT_StopHPIfNamed			int 	outer 	${Ini[botINI,FastDOT,QDOT_StopHPIfNamed,5]}

	/declare fa							int		local
	/for fa 1 to ${QDOT_Total} 
		/declare QDOT_${fa}_Spell 			string	outer	${Ini[botINI,FastDOT,QDOT_${fa}]}
		/declare QDOT_${fa}_Use 			bool	outer	${Ini[botINI,FastDOT,QDOT_${fa}_Use]}
		/declare QDOT_${fa}_Gem 			string	outer	${Ini[botINI,FastDOT,QDOT_${fa}_Gem]}
		/declare QDOT_${fa}_MaxTries 		int		outer	${Ini[botINI,FastDOT,QDOT_${fa}_MaxTries]}
		/declare QDOT_${fa}_NamedOnly 		int		outer	${Ini[botINI,FastDOT,QDOT_${fa}_NamedOnly]}
		/declare QDOT_${fa}_StartHP			int		outer	${Ini[botINI,FastDOT,QDOT_${fa}_StartHP,${QDOT_StartHP}]}
		/declare QDOT_${fa}_StopHP			int		outer	${Ini[botINI,FastDOT,QDOT_${fa}_StopHP,${QDOT_StopHP}]}
		/declare QDOT_${fa}_Resist      	string  outer   ${Spell[${QDOT_${fa}_Spell}].ResistType} 
		/declare QDOT_${fa}_HasCastOnce		bool	outer   FALSE
		/declare QDOT_${fa}_RecastTimer		timer   outer   0
	/next fa	
	
	|--Damage Over Time
	/declare DOT_Total					int		outer	${Ini[botINI,DamageOverTime,DOT_Total,0]}
	/declare DOT_TotalIfNamed			int		outer	${Ini[botINI,DamageOverTime,DOT_TotalIfNamed,${DOT_Total}]}
	/declare DOT_DelayTimer				timer	outer	${Ini[botINI,DamageOverTime,DOT_DelayTimer,0]}
	/declare DOT_HasCastOnce			bool	outer   FALSE
	/declare DOT_Mana_Minimum			int		outer	${Ini[botINI,DamageOverTime,DOT_Mana_Minimum,10]} 
	/declare DOT_Mana_NamedMinimum		int		outer	${Ini[botINI,DamageOverTime,DOT_Mana_NamedMinimum,1]}
	/declare DOT_StartHP				int		outer	${Ini[botINI,DamageOverTime,DOT_StartHP,98]}
	/declare DOT_StopHP					int		outer	${Ini[botINI,DamageOverTime,DOT_StopHP,10]}
	/declare DOT_StartHPIfNamed			int 	outer 	${Ini[botINI,DamageOverTime,DOT_StartHPIfNamed,100]}
	/declare DOT_StopHPIfNamed			int 	outer 	${Ini[botINI,DamageOverTime,DOT_StopHPIfNamed,0]}

	/declare dc							int		local
	/for dc 1 to ${DOT_Total} 
		/declare DOT_${dc}_Spell 			string	outer	${Ini[botINI,DamageOverTime,DOT_${dc}]}
		/declare DOT_${dc}_Use 				bool	outer	${Ini[botINI,DamageOverTime,DOT_${dc}_Use]}
		/declare DOT_${dc}_Gem 				string	outer	${Ini[botINI,DamageOverTime,DOT_${dc}_Gem]}
		/declare DOT_${dc}_MaxTries 		int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_MaxTries]}
		/declare DOT_${dc}_NamedOnly 		int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_NamedOnly]}
		/declare DOT_${dc}_StartHP			int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_StartHP,${DOT_StartHP}]}
		/declare DOT_${dc}_StopHP			int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_StopHP,${DOT_StopHP}]}
		/declare DOT_${dc}_Resist     		string  outer   ${Spell[${DOT_${dc}_Spell}].ResistType} 
		/declare DOT_${dc}_HasCastOnce		bool	outer   FALSE
		/declare DOT_${dc}_RecastTimer		timer   outer   0
		
	/next dc

	/echo dot_1 defined? ${Defined[DOT_1]}
	
	/call InitEvents_Casting
	/cecho \awNukes: \ag${NUKE_Total} \awAENukes: \ag${AENUKE_Total} \awQNuke: \ag${QNUKE_Total} \awStun: \ag${STUN_Total} \awDOT: \ag${DOT_Total} 

	|**
	|								_Spell      |_Use|_Gem|_MaxTries|_NamedOnly|_StartHP|_StopHP|_Resist|_HasCastOnce|_RecastTimer
	|									1		   2  3 4   5   6  7   8      9   
	/call setSpellLineUp DOT 2  "Blood of Thule"|TRUE|8|2|TRUE|100|0|Poison|FALSE|0,"Chaos Venom"|TRUE|9|2|TRUE|100|0|Poison|FALSE|0
	**|
/return
	
	
	
	