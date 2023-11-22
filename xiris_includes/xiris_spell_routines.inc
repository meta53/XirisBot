|**
----------------------------
xiris_spell_routines.inc
----------------------------
This file is similar to the normal Spell_Routines.inc by Rusty~ and edited by TreeHuggingDruid
This file modified the default handlers and includes a handler for MQ2Medley (bards)
This macro uses the MQ2Cast Plugin by s0rcier
______________________________________________
REQUIRED PLUGINS
    MQ2Cast
______________________________________________
REVISION HISTORY
	04.06.22	xiris	Changed name to xiris_spell_routines.inc to avoid conflics with mmoloader updates
						Incorprated some changes from Devestator's version
	04.24.22	xiris	Incorporated some of the methods form mmobug's version written by Pete Sampras
	12.31.22	xiris	Added MQ2Medley which allows bards to cast like MQ2Cast does
______________________________________________

  Cast: the main subroutine that casts spells or items for you 
     Usage: 
        /call MQ2Cast ["spell name"|"item name"|"AA name"|"AA#"] [[item|slotname]|AA|gem#] [give up time][s|m] [custom subroutine name] [-targetid|###] [-maxtries|#] [-recast|#] [-setin|setname] [-bandolier|setname]
     Examples: 
 
     To cast Howl of Tashan and mem it in slot 3 if not memmed:
       /call MQ2Cast "Howl of Tashan" gem3
 
     To cast Arcane Rune and keep trying for 7 seconds, in case of interrupts.
       /call MQ2Cast "Arcane Rune" gem5 7s

     To click Grim Aura earring that's in a bag:
       /call MQ2Cast "Shrunken Goblin Skull Earring" item

     To click Grim Aura earring that's in a bag:
       /call MQ2Cast "Shrunken Goblin Skull Earring" slotname

     To use AA ability Eldritch Rune:
       /call MQ2Cast "Eldritch Rune" alt
         or
       /call MQ2Cast "173" alt
 
     To call a subroutine that interrupts CH if target gets healed before it lands:
       /call MQ2Cast "Complete Healing" gem1 0 CheckHP
     Then in your macro have somewhere:
       Sub CheckHP
          /if ( ${Target.PctHPs}>=80 ) /call Interrupt
       /return

		3000 = 3 sec
		Sub CheckHP
		   /if ( ${Me.Casting.Name.Equal[Complete Healing]} && ${Target.PctHPs} < 40 && ${Cast.Timing} > 3000 ) ) /call Interrupt
		/return
 
 Returns these values: ${Macro.Return} and ${Cast.Result} and ${castReturn}
+----------------------+----------------------------------------------------------------------+ 
| CAST_CANCELLED       | Spell was cancelled by ducking (either manually or because mob died) | 
| CAST_CANNOTSEE       | You can't see your target                                            | 
| CAST_IMMUNE          | Target is immune to this spell                                       | 
| CAST_INTERRUPTED     | Casting was interrupted and exceeded the given time limit            | 
| CAST_INVIS           | You were invis, and noInvis is set to true                           | 
| CAST_NOTARGET        | You don't have a target selected for this spell                      | 
| CAST_NOTMEMMED       | Spell is not memmed and you gem to mem was not specified             | 
| CAST_NOTREADY        | AA ability or spell is not ready yet                                 | 
| CAST_OUTOFMANA       | You don't have enough mana for this spell!                           | 
| CAST_OUTOFRANGE      | Target is out of range                                               | 
| CAST_OUTOFREAGENTS   | You are out of required reagents for this spell                      | 
| CAST_RESIST          | Your spell was resisted!                                             | 
| CAST_SUCCESS         | Your spell was cast successfully! (yay)                              | 
| CAST_UNKNOWN         | Spell/Item/Ability was not found                                     | 
| CAST_COLLAPSE        | Gate Colapsed                                                        | 
| CAST_TAKEHOLD        | Spell not hold                                                       | 
| CAST_FIZZLE          | Spell Fizzle                                                         | 
| CAST_INVISIBLE       | NOT Casting Invis                 									  |
| CAST_RECOVER		   | Spell not Recovered yet!											  |
| CAST_STUNNED		   | Stunned 															  |
| CAST_STANDING		   | Not Standing 														  |
| CAST_DISTRACTED      | To Distracted ( spell book open )    								  |
| CAST_NOCHARM		   | This NPC cannot be charmed											  |
| CAST_NOTARGET		   | You must first select a target for this spell!	                      |
+----------------------+----------------------------------------------------------------------+

**|	
#Event BeginCast	"#*#You begin casting#*#"
#Event Collapse		"#*#Your gate is too unstable, and collapses.#*#"
#Event FDFail 		"#1# has fallen to the ground.#*#"
#Event Fizzle 		"#*#Your spell fizzles#*#"
#Event Immune 		"#*#Your target is immune to changes in its attack speed#*#"
#Event Immune 		"#*#Your target is immune to changes in its run speed#*#"
#Event Immune 		"#*#Your target cannot be mesmerized#*#"
#Event Interrupt 	"#*#Your casting has been interrupted#*#"
#Event Interrupt 	"#*#Your spell is interrupted#*#"
#Event Invulnerable "#*#You can't cast spells while invulnerable#*#"
#Event Invulnerable "#*#You cannot cast spells while invulnerable#*#"
#Event Invulnerable "#*#You *CANNOT* cast spells while invulnerable#*#"
#Event NoHold 		"#*#Your spell did not take hold#*#"
#Event NoHold 		"#*#Your spell would not have taken hold#*#"
#Event NoHold 		"#*#You must first target a group member#*#"
#Event NoHold 		"#*#Your spell is too powerful for your intended target#*#"
#Event NoLOS 		"#*#You cannot see your target.#*#"
#Event NoMount 		"#*#You can not summon a mount here.#*#"
#Event NoTarget 	"#*#You must first select a target#*#"
#Event NotReady 	"#*#Spell recast time not yet met.#*#"
#Event OutOfMana 	"#*#Insufficient Mana to cast this spell!#*#"
#Event OutOfRange 	"#*#Your target is out of range, get closer!#*#"
#Event OutOfRReagents "#*#You are missing some required components#*#"
#Event Recover 		"#*#You haven't recovered yet...#*#"
#Event Recover 		"#*#Spell recovery time not yet met#*#"
#Event Resisted 	"#*#Your target resisted the #1# spell#*#"
#Event Resisted2 	"#*#You resist the #1# spell#*#"
#Event Standing 	"#*#You must be standing to cast a spell#*#"
#Event Stunned 		"#*#You are stunned#*#"
#Event Stunned 		"#*#You can't cast spells while stunned!#*#"
#Event Stunned 		"#*#You *CANNOT* cast spells, you have been silenced!#*#"
#Event NoCharm      "#*#This NPC Cannot be charmed#*#"


|- note these are not registered wtih the xiris_events handler because they are only used in this include
Sub Events_SpellRoutines
	/doevents Recover
	/doevents BeginCast
	/doevents Fizzle
	/doevents Interrupt
	/doevents Standing
	/doevents FDFail
	/doevents OutOfRange
	/doevents OutOfMana
	/doevents OutOfRReagents
	/doevents NoLOS
	/doevents Resisted2
	/doevents Resisted
	/doevents Immune
	/doevents Stunned
	/doevents Collapse
	/doevents NoTarget
	/doevents NotReady
	/doevents NoHold
	/doevents NoCharm
/return



Sub xbot_initialize_spellRoutines
	/echo \apInitialize: \aoSpell Routines \aw-------------------------
	|-- Default values for xiris_spell_routines.inc
	/declare noInvis 			bool 	outer 	TRUE
	/declare noFeigning 		bool 	outer 	TRUE
	/declare castReturn			string  outer
	/if ( !${Defined[castReturn]} )  /declare castReturn string outer NULL
	/if ( !${Defined[defaultGem]} )  /declare defaultGem string outer gem9

	|-- Verify MQ2Cast Plugin
	/if ( !${Plugin[MQ2Cast].Name.Length} ) {
		/squelch /plugin MQ2Cast noauto
		/if ( !${Plugin[MQ2Cast].Name.Length} ) {
			/echo You must have MQ2Cast plugin to use this macro!!
			/beep
			/endmacro
		}
	}
	|-- Verify MQ2Exchange Plugin
	/if ( !${Plugin[MQ2Exchange].Name.Length} ) {
		/squelch /plugin MQ2Exchange noauto
		/if ( !${Plugin[MQ2Exchange].Name.Length} ) {
			/echo You must have MQ2Exchange plugin to use this macro!!
			/beep
			/endmacro
		}
	}
	
/return


| Note the _medley must be a string of the song name, aa name, or item name
| Rewriting this nonsense
| "[Harmony of Sound|Pendant of Discord|Blade Dance]",[SONG|ITEM|AA],214,TRUE 
Sub MQ2Medley(string _medley, string _type, int _targetID, bool _interrupt)

	/declare str_bardCasting string local queue "${_medley}"
	/if (${_interrupt}) /varset str_bardCasting ${str_bardCasting} -interrupt

    |/echo \aoMQ2Medley \awToken \ag${_medley}\aw, Type\ag${_type}\aw, Target\ao${_targetID}\aw, Interrupt\ao${_interrupt}  
	/echo \at/medley \ay${str_bardCasting} \at(slot:\ay${FindItem[${_medley}].InvSlot}\at)

	| if the item is not ready 
	/if (${_type.Equal[ITEM]}) {
		/if (${FindItem[${_medley}].TimerReady} > 0) /return 'CLICK_NOTREADY'
	}
	/medley ${str_bardCasting}
/return 'CAST_SUCCESS'


Sub MQ2Cast(string spellName,string spellType,timer giveUpTimer,string mySub,string MQ2Feature1,string MQ2Feature2,string MQ2Feature3,string MQ2Feature4,string MQ2Feature5,string MQ2Feature6,string MQ2Feature7,string MQ2Feature8)
	/echo \aoMQ2Cast \ag${spellName} \aw ${spellType} \a-w${giveUpTimer}
	/if ( !${Defined[spellType]} )   /declare spellType   string local NULL
	/if ( !${Defined[giveUpTimer]} ) /declare giveUpTimer timer  local 0
	/if ( !${Defined[MQ2Feature1]} ) /declare MQ2Feature1 string local
	/if ( !${Defined[MQ2Feature2]} ) /declare MQ2Feature2 string local
	/if ( !${Defined[MQ2Feature3]} ) /declare MQ2Feature3 string local
	/if ( !${Defined[MQ2Feature4]} ) /declare MQ2Feature4 string local
	/if ( !${Defined[MQ2Feature5]} ) /declare MQ2Feature5 string local
	/if ( !${Defined[MQ2Feature6]} ) /declare MQ2Feature6 string local
	/if ( !${Defined[MQ2Feature7]} ) /declare MQ2Feature7 string local
	/if ( !${Defined[MQ2Feature8]} ) /declare MQ2Feature8 string local
	
	/if ( !${Defined[interruptFlag]} ) {
		/declare interruptFlag bool outer FALSE	
	} else {
		/varset interruptFlag FALSE
	}

	/if ( ${mySub.Find[-targetid|]} || ${mySub.Find[-maxtries|]} || ${mySub.Find[-kill]} || ${mySub.Find[-recast|]} || ${mySub.Find[-setin]} || ${mySub.Find[-setout]} || ${mySub.Find[-bandolier|]} ) {
		/varset MQ2Feature8 ${MQ2Feature7}
		/varset MQ2Feature7 ${MQ2Feature6}
		/varset MQ2Feature6 ${MQ2Feature5}
		/varset MQ2Feature5 ${MQ2Feature4}
		/varset MQ2Feature4 ${MQ2Feature3}
		/varset MQ2Feature3 ${MQ2Feature2}
		/varset MQ2Feature2 ${MQ2Feature1}
		/varset MQ2Feature1 ${mySub}
	}
	
:wait_for_stop
	/if ( ${Me.Casting.ID} ) {
		/goto :wait_for_stop
	} else /if ( ${Corpse.Open} ) {
		/notify LootWnd DoneButton leftmouseup
		/goto :wait_for_stop
	}
	/if ( !${noFeigning} && ${Me.Feigning} ) /stand
		
	/if ( ${Window[SpellBookWnd].Open} ) /keypress spellbook
	/declare SaveItem string local NULL
	/declare SaveItemSlot string local NULL

	/if ( ${spellName.Find[|]} ) {
		/varset spellType ${spellName.Right[${spellName.Length[]}]}
		/varset spellName ${spellName.Left[${spellName.Find[|]}]}
	}

:cast_spell
	/if ( ${FindItem[=${spellName}].ID} ) {
		/if (${FindItem[=${spellName}].TimerReady}) {
			/varset castReturn CAST_NOTREADY
			/goto :CastEnd
		}

		/if ( ${Cursor.ID} && ${Me.FreeBuffSlots} ) /autoinventory
		/if ( ${FindItem[${spellName}].InvSlot.ID} && ${FindItem[${spellName}].InvSlot.ID} > 21 ) {			
			/if ( ${FindItem[${spellName}].InvSlot.ID} > 30 && ${FindItem[${spellName}].EffectType.Equal[Click Inventory]} || ${FindItem[${spellName}].EffectType.Equal[Click Unknown]} ) {
				/if ( !${InvSlot[pack8].Item.Container} && ${InvSlot[pack8].Item.ID} != ${FindItem[${spellName}].ID} ) /varset spellType pack8
			}
		}
	} else /if ( ${Me.AltAbility[${spellName}].ID} ) {
		/varset spellType ALT
	} else /if ( ${Spell[${spellName}].ID} ) {
		/if ( !${spellType.Find[gem]} ) /varset spellType ${defaultGem}
	}
	
	/if ( ${noInvis} ) {
		/casting "${spellName}" ${spellType} "-invis" "${MQ2Feature1}" "${MQ2Feature2}" "${MQ2Feature3}" "${MQ2Feature4}" "${MQ2Feature5}" "${MQ2Feature6}" "${MQ2Feature7}" "${MQ2Feature8}"
		/varset castReturn ${Cast.Result}
		/call Events_SpellRoutines
	} else {
		/casting "${spellName}" ${spellType} "${MQ2Feature1}" "${MQ2Feature2}" "${MQ2Feature3}" "${MQ2Feature4}" "${MQ2Feature5}" "${MQ2Feature6}" "${MQ2Feature7}" "${MQ2Feature8}"
		/varset castReturn ${Cast.Result}
		/call Events_SpellRoutines
	}
	:cast_spell_pending
	/if ( ${Cast.Status.Find[C]} ) {
		/if (${Cast.Result.Equal[CAST_OUTOFREAGENTS]}) 	/goto :CastEnd
		/if (${Cast.Result.Equal[CAST_NOTARGET]}) 		/goto :CastEnd
		/if (${Cast.Result.Equal[CAST_NOTREADY]}) 		/goto :CastEnd
		/if (${Cast.Result.Equal[CAST_NOCHARM]})		/goto :CastEnd
		/if ( !${interruptFlag} && ${mySub.Length} ) 	/call ${mySub}
		
		/goto :cast_spell_pending
	} else /if ( ${Cast.Result.Equal[CAST_RESIST]} ) {
		/if ( ( ${FindItem[${spellName}].ID} && ${giveUpTimer} && !${FindItem[${spellName}].Timer} ) || ( ${Spell[${spellName}].ID} && ${giveUpTimer} > ${Spell[${spellName}].RecoveryTime} ) ) /goto :cast_spell
	} else /if ( ${Select[${Cast.Result},CAST_FIZZLE,CAST_STUNNED,CAST_INTERRUPTED]} ) {
		/if ( !${interruptFlag} && ( ${FindItem[${spellName}].ID} && ${giveUpTimer} && !${FindItem[${spellName}].Timer} ) || ( ${Me.AltAbility[${spellName}].ID} && ${Me.AltAbilityReady[${spellName}]} ) || ( ${Spell[${spellName}].ID} && ${giveUpTimer} > ${Spell[${spellName}].RecoveryTime} ) ) /goto :cast_spell
	}
	:CastEnd
	/delay 50 (!${Me.Casting.ID})
/return ${castReturn}


Sub Interrupt
	|/echo \awInterrupting Cast! \ao${Time.Time24}
	/if (!${Defined[interruptFlag]}) /declare interruptFlag bool outer TRUE
	/stopcast
/return


Sub Event_Fizzle
	/if (${Defined[castReturn]}) /varset castReturn CAST_FIZZLE
/return

Sub Event_Immune
	/if (${Defined[castReturn]}) /varset castReturn CAST_IMMUNE
/return

Sub Event_Interrupt
	/if (${Defined[castReturn]}) /varset castReturn CAST_INTERRUPTED
/return

Sub Event_Invulnerable
	/if (${Defined[castReturn]}) /varset castReturn CAST_INVULNERABLE
/return

Sub Event_NoHold
	/if (${Defined[spellNotHold]}) /varset spellNotHold 1
	/if (${Defined[castReturn]}) /varset castReturn CAST_NOTHOLD
/return

Sub Event_NoLOS
	/if (${Defined[castReturn]}) /varset castReturn CAST_CANNOTSEE
/return

Sub Event_NoMount
	/if (${Defined[castReturn]}) /varset castReturn CAST_NOMOUNT
/return

Sub Event_NoTarget
	/if (${Defined[castReturn]}) /varset castReturn CAST_NOTARGET
/return

Sub Event_NotReady
	/if (${Defined[castReturn]}) /varset castReturn CAST_NOTREADY
/return

Sub Event_OutOfMana
	/if (${Defined[castReturn]}) /varset castReturn CAST_OUTOFMANA
/return

Sub Event_OutOfRange
	/if (${Defined[castReturn]}) /varset castReturn CAST_OUTOFRANGE
/return

Sub Event_OutOfRReagents
	/if (${Defined[castReturn]}) /varset castReturn CAST_OUTOFREAGENTS
/return

Sub Event_Recover
	/if (${Defined[castReturn]}) /varset castReturn CAST_RECOVER
/return

Sub Event_NoCharm
	/if (${Defined[castReturn]}) /varset castReturn CAST_NOCHARM
/return