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
**|



|NECRO/DOT/DEBUFF class stuff
#event DotResist1			"#*#Your target resisted the |${DOT_1}|#*#"
#event DotResist2			"#*#Your target resisted the |${DOT_2}|#*#"
#event DotResist3			"#*#Your target resisted the |${DOT_3}|#*#"
#event DotResist4			"#*#Your target resisted the |${DOT_4}|#*#"
#event DotResist5			"#*#Your target resisted the |${DOT_5}|#*#"
#event DotResist6			"#*#Your target resisted the |${DOT_6}|#*#"
#event DotResist7			"#*#Your target resisted the |${DOT_7}|#*#"
#event DotResist8			"#*#Your target resisted the |${DOT_8}|#*#"
#event DotResist9			"#*#Your target resisted the |${DOT_9}|#*#"
#event DotResist10			"#*#Your target resisted the |${DOT_10}|#*#"
#event DotResist11			"#*#Your target resisted the |${DOT_11}|#*#"
#event DotResist12			"#*#Your target resisted the |${DOT_12}|#*#"
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

#event InDoors				"#*#You can only cast this spell in the outdoors#*#"
#event InDoors				"#*#This spell does not work here#*#"
#event InDoors				"#*#You are already on a mount#*#"
#event InDoors				"#*#You can not summon a mount here#*#"

|NUKE class stuff
#event SpellResist			"#*#Your target resisted the #*#"
#event SpellResist			"#*#Your target resisted the #*#"

|RESIST TYPE stuff
#event SetResistType     	"#*#ResistTypes #1#"

|SETTING Triggers
#event SetUseFastOnly		"#*#UseFastOnly #1#"

Sub InitEvents_Casting
	/call RegisterBackgroundEvents Events_Casting_Background 2
	
	/if (${Me.Class.ShortName.Equal[SHM]} || ${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[NEC]}) {
		/call RegisterClassEvents Events_Casting_DOT 24
		/call RegisterRaidEvents Events_Casting_SlowSnare 8	
		
	}

/return

Sub Events_Casting_Background
	/doevents SetResistType
	/doevents SetUseFastOnly
	/doevents InDoors
/return

Sub Events_Casting_SlowSnare
	/doevents Slow
	/doevents SlowResist
	/doevents Snareon
	/doevents SnareResist
	/doevents SnareWornOff
	/doevents SpellResist
/return

Sub Events_Casting_DOT
	/doevents DotResist1
	/doevents DotResist2
	/doevents DotResist3
	/doevents DotResist4
	/doevents DotResist5
	/doevents DotResist6
	/doevents DotResist7
	/doevents DotResist8
	/doevents DotResist9
	/doevents DotResist10
	/doevents DotResist11
	/doevents DotResist12
	/doevents DotOff1
	/doevents DotOff2
	/doevents DotOff3
	/doevents DotOff4
	/doevents DotOff5
	/doevents DotOff6
	/doevents DotOff7
	/doevents DotOff8
	/doevents DotOff9
	/doevents DotOff10
	/doevents DotOff11
	/doevents DotOff12
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
	/if (${int_nukeTotal} == 0) /return 
	/if (${AENuke_Active}) 		/return
	/if (${timer_nukeDelay} && ${nuke_hasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return

	|-- press pet attack for funsies
	/if (${Me.Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP and my mana check
	/if (${isNamed}  && (${Target.PctHPs} < ${int_namedNukeStopHP}))	/return
	/if (${isNamed}  && (${Target.PctHPs} > ${int_namedNukeStartHP}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${int_nukeStopHP}))			/return
	/if (!${isNamed} && (${Target.PctHPs} > ${int_nukeStartHP}))		/return

	/if (${isNamed}  && (${Me.PctMana} < ${int_namedNukeMinMana}))		/return
	/if (!${isNamed} && (${Me.PctMana} < ${int_nukeMinMana}))			/return

	:fd_check
	/if (${Me.Feigning}) /stand
	
	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {
		/declare i 			int 	local
		/declare gemStr 	string 	local 	
		/declare gem		string  local
		
		/for i 1 to ${int_nukeTotal}  
			:NukeLoopStart
			|-- skip if disabled
			/if (!${Nuke_${i}_Use}) /goto :NextNuke

			|-- Check the timer on this nuke
			/if (${Nuke_${i}_Timer} <= 0) /cecho \agTimer ready for \ay${Nuke_${i}} :: \ay${Nuke_${i}_Timer}
			/if (${Nuke_${i}_Timer} > 0 && ${Nuke_${i}_HasCastOnce}) {
				/if (${i}<${int_nukeTotal}) { 
					/cecho \arTimer not ready for \ay${Nuke_${i}} :: \ay${Nuke_${i}_Timer}
					/goto :NextNuke
				} else {
					/return
				}
			}

			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
			/if (!${isNamed} && ${Nuke_${i}_NamedOnly}) {
				/echo Skipping Nuke ${Nuke_${i}} is named only
				/goto :NextNuke
			}

			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${Nuke_${i}_Resist}]}]}) {
				|/echo Nuke ${Nuke_${i}_Resist} matches resist
			} else {
				/echo Skipping Nuke ${Nuke_${i}_Resist} doesnt match resist
				/goto :NextNuke
			}
			
			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping		
			/call chkSpellMemorized "${Nuke_${i}}" TRUE ${Nuke_${i}_Gem}
			:NukeCastWait
			/if (!${Me.SpellReady[${Nuke_${i}}]} || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :NukeCastWait
			}

			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${Nuke_${i}_StartHP} && ${Target.PctHPs}>=${Nuke_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${Nuke_${i}}].Mana} && ${Target.Distance}<=${Spell[${Nuke_${i}}].Range} && ${Target.LineOfSight}) {
	
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${Nuke_${i}}"
						/cecho \ag ${Nuke_${i}}  : DELAYING for \ay${Nuke_${i}_Timer.OriginalValue}s \agrefresh	
					} else {
						/call MQ2Cast "${Nuke_${i}}"	
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${Nuke_${i}} ${Macro.Return} : DELAYING for \ay${Nuke_${i}_Timer.OriginalValue}s \agrefresh	
						}
					}

					/if (${Nuke_${i}_Delay} > 0) {
						/varset Nuke_${i}_Timer	${Nuke_${i}_Delay}
					} else {
						/varset Nuke_${i}_Timer	${Nuke_${i}_Timer.OriginalValue}
					}
					
					/varset Nuke_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextNuke
			
		/next i
	}
	/varset nuke_hasCastOnce TRUE
	/varset timer_nukeDelay ${timer_nukeDelay.OriginalValue}
/return

Sub chkQNuke
	/if (!${bln_engaged}) 		 /return
	|/if (!${bln_useFastOnly}) 	 /return
	/if (${int_QnukeTotal} == 0) /return 
	/if (${AENuke_Active}) 		 /return
	/if (${timer_QnukeDelay} && ${qnuke_hasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- press pet attack for funsies
	/if (${Me.Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP and my mana check
	/if (${isNamed}  && (${Target.PctHPs} < ${int_namedQNukeStopHP}))	/return
	/if (${isNamed}  && (${Target.PctHPs} > ${int_namedQNukeStartHP}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${int_QnukeStopHP}))		/return
	/if (!${isNamed} && (${Target.PctHPs} > ${int_QnukeStartHP}))		/return

	/if (${isNamed}  && (${Me.PctMana} < ${int_namedQNukeMinMana}))		/return
	/if (!${isNamed} && (${Me.PctMana} < ${int_QnukeMinMana}))			/return


	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {

		/declare i 			int 	local
		/declare gemStr 	string 	local 	
		/declare gem		string  local
		
		/for i 1 to ${int_QnukeTotal}  
			:QNLoopStart
			|-- skip if disabled
			/if (!${QNuke_${i}_Use}) /goto :NextQuickNuke

			|-- Check the timer on this nuke
			/if (${QNuke_${i}_Timer} <= 0) /cecho \agTimer ready for \ay${QNuke_${i}} :: \ay${QNuke_${i}_Timer}
			/if (${QNuke_${i}_Timer} > 0 && ${QNuke_${i}_HasCastOnce}) {
				/if (${i}<${int_qnukeTotal}) { 
					/cecho \arTimer not ready for \ay${QNuke_${i}} :: \ay${QNuke_${i}_Timer}
					/goto :NextQuickNuke
				} else {
					/return
				}
			}
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
	
			/if (!${isNamed} && ${QNuke_${i}_NamedOnly}) {
				/echo Skipping QuickNuke ${QNuke_${i}} is named only
				/goto :NextQuickNuke
			}

			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${QNuke_${i}_Resist}]}]}) {
				|/echo QuickNuke ${QNuke_${i}_Resist} matches resist
			} else {
				/echo Skipping QuickNuke ${QNuke_${i}_Resist} doesnt match resist
				/goto :NextQuickNuke
			}
			
			/call RunAllEvents
			
			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping		
			/call chkSpellMemorized "${QNuke_${i}}" FALSE ${QNuke_${i}_Gem}
			:QnukeNukeCastWait
			/if (!${Me.SpellReady[${QNuke_${i}}]} || (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :QnukeNukeCastWait
			}
			
			
			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${QNuke_${i}_StartHP} && ${Target.PctHPs}>=${QNuke_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${QNuke_${i}}].Mana} && ${Target.Distance}<=${Spell[${QNuke_${i}}].Range} && ${Target.LineOfSight}) {
					
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${QNuke_${i}}"
						/cecho \ag ${QNuke_${i}}  : DELAYING for \ay${QNuke_${i}_Timer.OriginalValue}s \agrefresh	
					} else {
						/call MQ2Cast "${QNuke_${i}}"	
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${QNuke_${i}} ${Macro.Return} : DELAYING for \ay${QNuke_${i}_Timer.OriginalValue}s \agrefresh	
						}
					}

					/if (${QNuke_${i}_Delay} > 0) {
						/varset QNuke_${i}_Timer	${QNuke_${i}_Delay}
					} else {
						/varset QNuke_${i}_Timer	${QNuke_${i}_Timer.OriginalValue}
					}

					/varset QNuke_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextQuickNuke
		/call RunAllEvents
			
		/next i
	}
	/varset qnuke_hasCastOnce TRUE
	/varset timer_QnukeDelay ${timer_QnukeDelay.OriginalValue}
/return

|**
 ----------------------------
 AE Direct Damage  
 ----------------------------
 Loops through the AE nuke list and applies
 ----------------------------
**|
Sub chkAENuke
	/if (!${AENuke_Active}) /return
	|-- AE Nuke is active, let's light them up!
	/echo Lighting them up with AE Nuke ${AENuke_1}
	/call chkSpellMemorized "${AENuke_1}" TRUE
	:AECastWait
		/if (${Me.Casting.ID}) {
			/delay 2
			/goto :AECastWait
		}
		/if (!${Me.SpellReady[${AENuke_1}]}) {
			/delay 2
			/goto :AECastWait
		}
	/call MQ2Cast "${AENuke_1}"
	/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
		/cecho \ag ${AENuke_1} ${Cast.Result}
	}	
/return

|**
 ----------------------------
 Snaring  
 ----------------------------
 Snare Spell is declared in the INI
 ----------------------------
**|
Sub chkSnare
	/if (!${bln_engaged} || ${Me.Invis}||${Me.Moving}|| ${Me.Song[${GoM}].ID}||${list_immuneSnare.Find[${Spawn[${Target.CleanName}]}]}||${list_ignore.Find[${Spawn[${Target.CleanName}]}]}) /return
	/if (${Target.PctHPs}<=${int_snareHP} && ${Target.ID} && ${Target.Type.Equal[NPC]} && !${bln_snareActive} && ${bln_snareUse} && ${SpawnCount[npc radius 50 zradius 10]}<=${int_snareLocalNPC}) {
		/call MQ2Cast "${str_snareSpell}" ${str_snareGem} 
	}
	
	/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
		/varset bln_snareActive TRUE
	}
	
	/if (${Cast.Result.Equal[CAST_IMMUNE]}) {
		/ini "${IniFile}" "SnareImmunes.${Zone.ShortName}" "SnareImmune" "${list_immuneSnare}${Target.CleanName}|"
		/varset list_immuneSnare ${Ini[${ignoreINI},"SnareImmunes.${Zone.ShortName}","SnareImmune"]}
		/varset bln_snareActive TRUE
	}
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
	/if (${int_stunTotal} == 0)  /return 
	/if (${timer_stunDelay} && ${stun_hasCastOnce})	 	/return 

	/if (${Me.PctMana} < ${int_stunMinMana}) 	/return
	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {

		/declare i 			int 	local
		/declare gemStr 	string 	local 	
		/declare gem		string  local
		
		/for i 1 to ${int_stunTotal}  
			:StunLoopStart
			|-- skip if disabled
			/if (!${Stun_${i}_Use}) /goto :NextStun

			|-- Check to see if this stun fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${Stun_${i}_Resist}]}]}) {
				|/echo Stun ${Stun_${i}_Resist} matches resist
			} else {
				/echo Skipping Stun ${Stun_${i}_Resist} doesnt match resist
				/goto :NextStun
			}
			
			/call RunAllEvents	
			/call chkSpellMemorized "${Stun_${i}}" ${_memorize} ${Stun_${i}_Gem}

			|-- All this ready, check for LOS/Distance/Mana
			/if (${Target.PctHPs}<=${Stun_${i}_StartHP} && ${Target.PctHPs}>=${Stun_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${Stun_${i}}].Mana} && ${Target.Distance}<=${Spell[${Stun_${i}}].Range} && ${Target.LineOfSight}) {
					/call MQ2Cast "${Stun_${i}}" ${Stun_${i}_Gem} 5s
					/if (${Cast.Result.Equal[CAST_SUCCESS]}) {
						/cecho \ag ${Stun_${i}} ${Cast.Result}
						/varset Stun_${i}_HasCastOnce TRUE
					}
				}
			}
		:NextStun
		/call RunAllEvents
			
		/next i
	}
	/varset stun_hasCastOnce TRUE
	/varset timer_stunDelay ${timer_stunDelay.OriginalValue}
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
	/varset bln_stunLocking TRUE
	/call chkStun TRUE
	:stunLock
		/call chkStun FALSE
		/call RunAllEvents
		/if (${bln_stunLocking}) /goto :stunLock
:return
/return

Sub chkStunLockStop
	/varset bln_stunLocking FALSE
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
	/if (${AENuke_Active}) 		/return
	/if (!${bln_engaged}) 		/return
	/if (${int_dotTotal} == 0)	/return 
	/if (${timer_dotDelay} && ${dot_hasCastOnce})	 	/return 

	/if ((${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Moving} || ${Me.Invis} || ${ExcludeList.Find[${Spawn[${Target.CleanName}]}]}) /return
	
	|-- press pet attack for funsies
	/if (${Me.Pet.ID} && ${bln_petUse} && !${Me.Pet.Combat}) /call chkPetAttack

	|-- named check
	:named_check
	/declare isNamed bool local FALSE
	/call TargetNamedCheck ${Target.ID}
	/varset isNamed ${Macro.Return}

	|-- target HP and my mana check
	/if (${isNamed}  && (${Target.PctHPs} < ${int_namedDotStopHP}))		/return
	/if (${isNamed}  && (${Target.PctHPs} > ${int_namedDotStartHP}))	/return
	/if (!${isNamed} && (${Target.PctHPs} < ${int_dotStopHP}))			/return
	/if (!${isNamed} && (${Target.PctHPs} > ${int_dotStartHP}))			/return

	/if (${isNamed}  && (${Me.PctMana} < ${int_namedDotMinMana}))		/return
	/if (!${isNamed} && (${Me.PctMana} < ${int_dotMinMana}))			/return
	
	:fd_check
	/if (${Me.Feigning}) /stand

	|-- Final Check before casting
	/if (${Target.ID} && ${Target.Type.Equal[NPC]} && ${Target.LineOfSight}) {
		/declare i 				int 	local
		/declare dotDuration 	int 	local 	0
		/declare gemStr 		string 	local 	
		/declare gem			string  local
		
		/for i 1 to ${int_dotTotal}  
			:DotLoopStart
			|-- skip if disabled
			/if (!${DOT_${i}_Use}) /goto :NextDot

			|-- Check the timer on this dot
			/if (${DOT_${i}_Timer} <= 0) /cecho \agTimer ready for \ay${DOT_${i}} :: \ay${DOT_${i}_Timer}
			/if (${DOT_${i}_Timer} > 0 && ${DOT_${i}_HasCastOnce}) {
				/if (${i}<${int_dotTotal}) { 
					/cecho \arTimer not ready for \ay${DOT_${i}} :: \ay${DOT_${i}_Timer}
					/goto :NextDot
				} else {
					/return
				}
			}
			
			|-- Named Target check
			|-- if we only cast this spell on named, and this is NOT a named target, skip
			/if (!${isNamed} && ${DOT_${i}_NamedOnly}) {
				/echo Skipping Dot ${DOT_${i}} is named only
				/goto :NextDot
			}
			
			|-- Check to see if this nuke fits current criteria (allowable Resist type}
			/if (${Bool[${str_useResistTypes.Find[${DOT_${i}_Resist}]}]}) {
				|/echo DOT ${DOT_${i}_Resist} matches resist
			} else {
				/echo Skipping DOT ${DOT_${i}_Resist} doesnt match resist
				/goto :NextDot
			}
			
			/call RunAllEvents
			

			|-- Ready Wait loop waits for the DOT to be ready to cast
			|   Used due to lag on raids sometimes lagging gems
			|   Additionally spell might not be memmed due to swapping
			/call chkSpellMemorized "${DOT_${i}}" FALSE ${DOT_${i}_Gem}
			:DotReadyWait
			/if ((!${Me.SpellReady[${DOT_${i}}]} || (${Me.Casting.ID}) && ${Me.Class.ShortName.NotEqual[BRD]})) {
				/delay 2
				/goto :DotReadyWait
			}

			|-- All this ready, check for LOS/Distance/Mana
			|-- Think about adding the named stop point here at some point in future
			/if (${Target.PctHPs}<=${DOT_${i}_StartHP} && ${Target.PctHPs}>=${DOT_${i}_StopHP}) {
				/if (${Me.CurrentMana}>=${Spell[${DOT_${i}}].Mana} && (${Me.SpellReady[${DOT_${i}}]} || ${Me.Class.ShortName.Equal[BRD]}) && ${Target.Distance}<=${Spell[${DOT_${i}}].Range} && ${Target.LineOfSight} && (${DOT_${i}_Timer} <= 0)) {
					/varcalc dotDuration ${Spell[${DOT_${i}}].Duration} * 6
					/if (${Me.Class.ShortName.Equal[BRD]}) {
						/medley queue "${DOT_${i}}"
						/cecho \ag ${DOT_${i}}  : DELAYING for \ay${dotDuration}s \agrefresh	
					} else {
						/call MQ2Cast "${DOT_${i}}"	
						/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
							/cecho \ag ${DOT_${i}} ${Macro.Return} : DELAYING for \ay${dotDuration}s \agrefresh	
						}
					}
					/varset DOT_${i}_Timer	${dotDuration}s
					/varset DOT_${i}_HasCastOnce TRUE
					/varcalc int_concCount ${int_concCount}+1
				}
			}
		:NextDot
		/call RunAllEvents
			
		/next i
	}
	/varset dot_hasCastOnce TRUE
	/varset timer_dotDelay ${timer_dotDelay.OriginalValue}
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
:return
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
	/varset nuke_hasCastOnce  FALSE
	/for i 1 to ${int_nukeTotal}
		/varset Nuke_${i}_HasCastOnce FALSE
	/next i

	:qnuke
	/varset qnuke_hasCastOnce  FALSE
	/for i 1 to ${int_QnukeTotal}
		/varset QNuke_${i}_HasCastOnce FALSE
	/next i

	:stuns
	/varset stun_hasCastOnce  FALSE
	/for i 1 to ${int_stunTotal}
		/varset Stun_${i}_HasCastOnce FALSE
	/next i

	:fastDoT
	/varset fast_hasCastOnce  FALSE
	/for i 1 to ${int_fastTotal}
		/varset FAST_${i}_HasCastOnce FALSE
	/next i

	:DoT
	/varset dot_hasCastOnce  FALSE
	/for i 1 to ${int_dotTotal}
		/varset DOT_${i}_HasCastOnce FALSE
		/varset DOT_${i}_Active FALSE
		/varset DOT_${i}_Resisted 0
	/next i

	/echo Casting: Reset all timers

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

|-- SLOW IMMUNE
Sub Event_SlowImmune
	/varset bln_slowed 	TRUE
	/${cmdChannel} slowimmunemob ${Target.CleanName}
	/doevents flush SlowImmune
/return


Sub Event_InDoors
	/varset bln_outside	FALSE
	/doevents flush Indoors
/return

Sub Event_Slow(string Line, string MobName)
	/varset bln_slowed	TRUE
	/if (${bln_slowUse}) /${cmdChannel} slowed: ${Target.CleanName}
	/doevents flush Slow
/return

Sub Event_SlowResist
	/varcalc int_slowResisted	${int_slowResisted}+1
	/if (${int_slowResisted} > ${int_slowMaxTries}) /varset bln_slowed 	TRUE
	/doevents flush SlowResist
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
	/if (${int_QnukeTotal} > 0) {
		/echo setting fast only:${_use}
		/varset bln_useFastOnly ${_use}
	} else {
		/varset bln_useFastOnly FALSE
	}
	/doevents flush SetUseFastOnly
/return 



|-------------------------------------------------------------------------------- 
|EVENTS: RESISTS, ETC
| Handle up to 12 DOTS, etc. Note, unlikely to actually have that
| many memorized at once, but still...
|--------------------------------------------------------------------------------

Sub EVENT_SetResistType(_chatText, _strResists) 
	/echo setting resist type: ${_strResists}
	/varset str_useResistTypes ${_strResists}
	/doevents flush SetResistType
/return 

Sub Event_DotResist1
	/varcalc DOT_1_Resisted		${DOT_1_Resisted}+1
	/varset  DOT_1_Active		FALSE
	/doevents flush DotResist1
/return

Sub Event_DotResist2
	/varcalc DOT_2_Resisted		${DOT_2_Resisted}+1
	/varset  DOT_2_Active		FALSE
	/doevents flush DotResist2
/return

Sub Event_DotResist3
	/varcalc DOT_3_Resisted		${DOT_3_Resisted}+1
	/varset  DOT_3_Active		FALSE
	/doevents flush DotResist3
/return

Sub Event_DotResist4
	/varcalc DOT_4_Resisted		${DOT_4_Resisted}+1
	/varset  DOT_4_Active		FALSE
	/doevents flush DotResist4
/return

Sub Event_DotResist5
	/varcalc DOT_5_Resisted		${DOT_5_Resisted}+1
	/varset  DOT_5_Active		FALSE
	/doevents flush DotResist5
/return

Sub Event_DotResist6
	/varcalc DOT_6_Resisted		${DOT_6_Resisted}+1
	/varset  DOT_6_Active		FALSE
	/doevents flush DotResist6
/return

Sub Event_DotResist7
	/varcalc DOT_7_Resisted		${DOT_7_Resisted}+1
	/varset  DOT_7_Active		FALSE
	/doevents flush DotResist7
/return

Sub Event_DotResist8
	/varcalc DOT_8_Resisted		${DOT_8_Resisted}+1
	/varset  DOT_8_Active		FALSE
	/doevents flush DotResist8
/return

Sub Event_DotResist9
	/varcalc DOT_9_Resisted		${DOT_9_Resisted}+1
	/varset  DOT_9_Active		FALSE
	/doevents flush DotResist9
/return

Sub Event_DotResist10
	/varcalc DOT_10_Resisted	${DOT_10_Resisted}+1
	/varset  DOT_10_Active		FALSE
	/doevents flush DotResist10
/return

Sub Event_DotResist11
	/varcalc DOT_11_Resisted	${DOT_11_Resisted}+1
	/varset  DOT_11_Active		FALSE
	/doevents flush DotResist11
/return

Sub Event_DotResist12
	/varcalc DOT_12_Resisted	${DOT_12_Resisted}+1
	/varset  DOT_12_Active		FALSE
	/doevents flush DotResist12
/return

Sub Event_DotOff1
	/varset	 DOT_1_Active		FALSE
	/varset  DOT_1_Timer		0
	/doevents flush DotOff1
/return

Sub Event_DotOff2
	/varset	 DOT_2_Active		FALSE
	/varset  DOT_2_Timer		0
	/doevents flush DotOff2
/return

Sub Event_DotOff3
	/varset	 DOT_3_Active		FALSE
	/varset  DOT_3_Timer		0
	/doevents flush DotOff3
/return

Sub Event_DotOff4
	/varset	 DOT_4_Active		FALSE
	/varset  DOT_4_Timer		0
	/doevents flush DotOff4
/return

Sub Event_DotOff5
	/varset	 DOT_5_Active		FALSE
	/varset  DOT_5_Timer		0
	/doevents flush DotOff5
/return

Sub Event_DotOff6
	/varset	 DOT_6_Active		FALSE
	/varset  DOT_6_Timer		0
	/doevents flush DotOff6
/return

Sub Event_DotOff7
	/varset	 DOT_7_Active		FALSE
	/varset  DOT_7_Timer		0
	/doevents flush DotOff7
/return

Sub Event_DotOff8
	/varset	 DOT_8_Active		FALSE
	/varset  DOT_8_Timer		0
	/doevents flush DotOff8
/return

Sub Event_DotOff9
	/varset	 DOT_9_Active		FALSE
	/varset  DOT_9_Timer		0
	/doevents flush DotOff9
/return

Sub Event_DotOff10
	/varset	 DOT_10_Active		FALSE
	/varset  DOT_10_Timer		0
	/doevents flush DotOff10
/return

Sub Event_DotOff11
	/varset	 DOT_11_Active		FALSE
	/varset  DOT_11_Timer		0
	/doevents flush DotOff11
/return

Sub Event_DotOff12
	/varset	 DOT_12_Active		FALSE
	/varset  DOT_12_Timer		0
	/doevents flush DotOff12
/return

Sub xbot_initialize_casting
	/if (${str_classArchetype.Equal[MELEE_END]}) /return
	/if (${Me.Class.ShortName.Equal[WAR]}) /return
	
	|-- Misc (resist types, fast only, etc
	/declare str_useResistTypes	string	outer 	Fire,Magic,Cold,Poison,Disease
	/if (${Zone.ShortName.Equal[pofire]}) /varset str_useResistTypes Cold,Magic,Poison,Disease
	
	/declare bln_useFastOnly	bool	outer	FALSE
	/declare int_concCount		int     outer 	0
	
	|-- Nukes
	/declare int_nukeTotal				int		outer	${Ini[botINI,DirectDamage,int_nukeTotal,0]}	
	/declare timer_nukeDelay			timer	outer	${Ini[botINI,DirectDamage,timer_nukeDelay,1s]}
	/declare nuke_hasCastOnce			bool	outer   FALSE
	/declare int_nukeMinMana			int		outer	${Ini[botINI,DirectDamage,int_nukeMinMana,10]}
	/declare int_namedNukeMinMana		int		outer	${Ini[botINI,DamageOverTime,int_namedNukeMinMana,10]} 
	/declare int_nukeStartHP			int		outer	${Ini[botINI,DirectDamage,int_nukeStartHP,98]}
	/declare int_nukeStopHP				int		outer	${Ini[botINI,DirectDamage,int_nukeStopHP,5]}
	/declare int_namedNukeStopHP		int 	outer 	${Ini[botINI,DamageOverTime,int_namedNukeStopHP,0]}
	
	/echo int_nukeTotal: ${int_nukeTotal}
	/declare nc 						int		local
	/for nc 1 to ${int_nukeTotal} 
		/declare Nuke_${nc} 			string	outer	${Ini[botINI,DirectDamage,Nuke_${nc}]}
 
		/declare Nuke_${nc}_Use 		bool	outer	${Ini[botINI,DirectDamage,Nuke_${nc}_Use]}
		/declare Nuke_${nc}_Gem 		string	outer	${Ini[botINI,DirectDamage,Nuke_${nc}_Gem]}
		/declare Nuke_${nc}_StartHP		int		outer	${Ini[botINI,DirectDamage,Nuke_${nc}_StartHP]}
		/declare Nuke_${nc}_StopHP		int		outer	${Ini[botINI,DirectDamage,Nuke_${nc}_StopHP]}
		/declare Nuke_${nc}_Resist      string  outer   ${Spell[${Nuke_${nc}}].ResistType} 
		/declare Nuke_${nc}_NamedOnly   string  outer   ${Ini[botINI,DirectDamage,Nuke_${nc}_NamedOnly]}
		/declare Nuke_${nc}_Timer		timer   outer   ${timer_nukeDelay}
		/declare Nuke_${nc}_HasCastOnce	bool	outer   FALSE
		/declare Nuke_${nc}_Delay   	int     outer   ${Ini[botINI,DirectDamage,Nuke_${nc}_NamedOnly,0]}
		
		/echo Nuke_${nc} Use:  ${Nuke_${nc}_Use} || Gem: ${Nuke_${nc}_Gem} || Start: ${Nuke_${nc}_StartHP}
		/echo Stop: ${Nuke_${nc}_StopHP} || Resist: ${Nuke_${nc}_Resist} || Named: ${Nuke_${nc}_NamedOnly} || Timer: ${Nuke_${nc}_Timer}
	/next nc

	|--AE Nuke
	/declare AENuke_Active				bool	outer	FALSE
	/declare AENuke_1					string	outer 	${Ini[botINI,AENuke,AENuke_1,NULL]}
	/declare AENuke_1_Use 				bool	outer	${Ini[botINI,AENuke,AENuke_1_Use,FALSE]}
	/declare AENuke_1_Gem 				string	outer	${Ini[botINI,AENuke,AENuke_1_Gem]}
	/declare AENuke_1_StartHP			int		outer	${Ini[botINI,AENuke,AENuke_1_StartHP]}
	/declare AENuke_1_StopHP			int		outer	${Ini[botINI,AENuke,AENuke_1_StopHP]}
	/declare AENuke_1_NamedOnly			bool	outer   ${Ini[botINI,AENuke,AENuke_1_NamedOnly]}
	
	|--Quick Nuke
	/declare int_QnukeTotal				int		outer	${Ini[botINI,QuickNuke,int_QnukeTotal,0]}	
	/declare timer_QnukeDelay			timer	outer	${Ini[botINI,QuickNuke,timer_QnukeDelay,1s]}
	/declare qnuke_hasCastOnce			bool	outer   FALSE
	/declare int_QnukeMinMana			int		outer	${Ini[botINI,QuickNuke,int_QnukeMinMana,10]}
	/declare int_namedQNukeMinMana		int		outer	${Ini[botINI,DamageOverTime,int_namedQNukeMinMana,10]} 
	/declare int_QnukeStartHP			int		outer	${Ini[botINI,QuickNuke,int_QnukeStartHP,98]}
	/declare int_QnukeStopHP			int		outer	${Ini[botINI,QuickNuke,int_QnukeStopHP,0]}
	/declare int_namedQNukeStopHP		int 	outer 	${Ini[botINI,DamageOverTime,int_namedQNukeStopHP,0]}

	/declare qn 						int		local
	/for qn 1 to ${int_QnukeTotal} 
		/declare QNuke_${qn} 			string	outer	${Ini[botINI,QuickNuke,QNuke_${qn}]}
		/declare QNuke_${qn}_Use 		bool	outer	${Ini[botNI,QuickNuke,QNuke_${qn}_Use,TRUE]}
		/declare QNuke_${qn}_Gem 		string	outer	${Ini[botINI,QuickNuke,QNuke_${qn}_Gem]}
		/declare QNuke_${qn}_StartHP	int		outer	${Ini[botINI,QuickNuke,QNuke_${qn}_StartHP]}
		/declare QNuke_${qn}_StopHP		int		outer	${Ini[botINI,QuickNuke,QNuke_${qn}_StopHP]}
		/declare QNuke_${qn}_Resist     string  outer   ${Spell[${QNuke_${qn}}].ResistType} 
		/declare QNuke_${qn}_NamedOnly 	string	outer	${Ini[botINI,QuickNuke,QNuke_${qn}_NamedOnly]}
		/declare QNuke_${qn}_Timer		timer   outer   ${timer_QnukeDelay}
		/declare QNuke_${qn}_HasCastOnce	bool	outer   FALSE
	/next qn	

	|--Stuns
	/declare int_stunTotal				int		outer	${Ini[botINI,Stun,int_stunTotal,0]}	
	/declare timer_stunDelay			timer	outer	${Ini[botINI,Stun,timer_stunDelay,1s]}
	/declare stun_hasCastOnce			bool	outer   FALSE
	/declare int_stunMinMana			int		outer	${Ini[botINI,Stun,int_stunMinMana,0]}
	/declare bln_stunLocking			bool	outer	FALSE


	/declare st 						int		local
	/for st 1 to ${int_stunTotal} 
		/declare Stun_${st} 			string	outer	${Ini[botINI,Stun,Stun_${st}]}
		/declare Stun_${st}_Use 		bool	outer	${Ini[botNI,Stun,Stun_${st}_Use,TRUE]}
		/declare Stun_${st}_Gem 		string	outer	${Ini[botINI,Stun,Stun_${st}_Gem]}
		/declare Stun_${st}_StartHP		int		outer	${Ini[botINI,Stun,Stun_${st}_StartHP]}
		/declare Stun_${st}_StopHP		int		outer	${Ini[botINI,Stun,Stun_${st}_StopHP]}
		/declare Stun_${st}_Resist 		string  outer   ${Spell[${Stun_${st}}].ResistType} 
		/declare Stun_${st}_Timer		timer   outer   ${timer_stunDelay}
		/declare Stun_${st}_HasCastOnce	bool	outer   FALSE
	/next st	
	
	|-- DOTs MOAR DOTS!
	|--FAST Damage Over Time
	/declare int_fastTotal				int		outer	${Ini[botINI,FastDOT,int_fastTotal,0]}
	/declare timer_fastDelay			timer	outer	${Ini[botINI,FastDOT,timer_fastDelay,1s]}
	/declare fast_hasCastOnce			bool	outer   FALSE
	/declare int_fastMinMana			int		outer	${Ini[botINI,DamageOverTime,int_fastMinMana,10]} 
	/declare int_namedFastMinMana		int		outer	${Ini[botINI,DamageOverTime,int_namedFastMinMana,1]}
	/declare int_fastStartHP			int		outer	${Ini[botINI,FastDOT,int_fastStartHP,94]}
	/declare int_fastStopHP				int		outer	${Ini[botINI,FastDOT,int_fastStopHP,10]}
	/declare int_namedFastStopHP		int 	outer 	${Ini[botINI,DamageOverTime,int_namedFastStopHP,5]}
	/declare bln_fastTrigger			bool 	outer	FALSE
	/declare int_fastTrigger			int		outer	${Ini[botINI,FastDOT,int_fastTrigger,3]}

	/declare fa							int		local
	/for fa 1 to ${int_fastTotal} 
		/declare FAST_${fa} 			string	outer	${Ini[botINI,FastDOT,FAST_${fa}]}
		/declare FAST_${fa}_Use 		bool	outer	${Ini[botINI,FastDOT,FAST_${fa}_Use]}
		/declare FAST_${fa}_Gem 		string	outer	${Ini[botINI,FastDOT,FAST_${fa}_Gem]}
		/declare FAST_${fa}_MaxTries 	int		outer	${Ini[botINI,FastDOT,FAST_${fa}_MaxTries]}
		/declare FAST_${fa}_NamedOnly 	int		outer	${Ini[botINI,FastDOT,FAST_${fa}_NamedOnly]}
		/declare FAST_${fa}_StartHP		int		outer	${Ini[botINI,DamageOverTime,FAST_${fa}_StartHP,${int_fastStartHP}]}
		/declare FAST_${fa}_StopHP		int		outer	${Ini[botINI,DamageOverTime,FAST_${fa}_StopHP,${int_fastStopHP}]}
		/declare FAST_${fa}_Resisted	int		outer 	0
		/declare FAST_${fa}_Resist      string  outer   ${Spell[${FAST_${fa}}].ResistType} 
		/declare FAST_${fa}_Active		bool	outer 	FALSE
		/declare FAST_${fa}_Timer		timer   outer   ${timer_fastDelay}
		/declare FAST_${fa}_HasCastOnce	bool	outer   FALSE
	/next fa	
	
	|--Damage Over Time
	/declare int_dotTotal				int		outer	${Ini[botINI,DamageOverTime,int_dotTotal,0]}
	/declare timer_dotDelay				timer	outer	${Ini[botINI,DamageOverTime,timer_dotDelay,1s]}
	/declare dot_hasCastOnce			bool	outer   FALSE
	/declare int_dotMinMana				int		outer	${Ini[botINI,DamageOverTime,int_dotMinMana,10]} 
	/declare int_namedDotMinMana		int		outer	${Ini[botINI,DamageOverTime,int_namedDotMinMana,1]}
	/declare int_dotStartHP				int		outer	${Ini[botINI,DamageOverTime,int_dotStartHP,98]}
	/declare int_dotStopHP				int		outer	${Ini[botINI,DamageOverTime,int_dotStopHP,10]}
	/declare int_namedDotStopHP			int 	outer 	${Ini[botINI,DamageOverTime,int_namedDotStopHP,5]}
	/declare int_dotStopSeconds			int		outer 	${Ini[botINI,DamageOverTime,int_dotStopSeconds,15]}
	
	/declare dc							int		local
	/for dc 1 to ${int_dotTotal} 
		/if (${Defined[DOT_${dc}]}) /deletevar DOT_${dc}
		/declare DOT_${dc} 				string	outer	${Ini[botINI,DamageOverTime,DOT_${dc}]}
		/declare DOT_${dc}_Use 			bool	outer	${Ini[botINI,DamageOverTime,DOT_${dc}_Use]}
		/declare DOT_${dc}_Gem 			string	outer	${Ini[botINI,DamageOverTime,DOT_${dc}_Gem]}
		/declare DOT_${dc}_MaxTries 	int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_MaxTries]}
		/declare DOT_${dc}_NamedOnly 	int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_NamedOnly]}
		/declare DOT_${dc}_StartHP		int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_StartHP,${int_dotStartHP}]}
		/declare DOT_${dc}_StopHP		int		outer	${Ini[botINI,DamageOverTime,DOT_${dc}_StopHP,${int_dotStopHP}]}
		/declare DOT_${dc}_Resisted		int		outer 	0
		/declare DOT_${dc}_Resist     	string  outer   ${Spell[${DOT_${dc}}].ResistType} 
		/declare DOT_${dc}_Active		bool	outer 	FALSE
		/declare DOT_${dc}_Timer		timer   outer   ${timer_dotDelay}
		/declare DOT_${dc}_HasCastOnce	bool	outer   FALSE
	/next dc

	/cecho \aoxbot_initialize_casting
	/call InitEvents_Casting

/return
	
	
	
	