|**
----------------------------
xiris_events_raid_god.inc
----------------------------
Event Handlers for the Gates of Discord raids.
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	04.03.22    xiris	INITIAL REVISION
	04.10.22	xiris	Tweaked Pirik Event for better mana preservation
______________________________________________
**|	
|-- Gates of Discord events

| UQUA
#EVENT UquaKey                  "The #1# Key must unlock the door to the next room."

| IKK4
#EVENT KeeperPosition			"#*#KeeperPosition#*#"

| INKTUTA
|** Positions For Noqufiel:
    ncf - noq cleric fake (left)
	ncr - noq cleric real (right)
	ntf	- noq tank fake (left)
	ntr - noq tank real (right)
	nw1 - noq wiz 1
	nw2 - noq wiz 2
**|

#EVENT NoqCurse 				"[ #1# #*#]#*#NoqCurse,#2#"
#EVENT NoqSafe  				"#*#noqsafe#*#"
#EVENT InkCurse					"#*#The thoughts of a cursed trusik#*#"
#EVENT InkCurseSafe				"#*#inkstop#*#"
#EVENT InkCurseSafe				"#*#inksafe#*#"
#EVENT CC1						"#*#inkcc1#*#"
#EVENT CC2						"#*#inkcc2#*#"
#EVENT CC3						"#*#inkcc3#*#"
#EVENT CC4						"#*#inkcc4#*#"
#EVENT CC5						"#*#inkcc5#*#"
#EVENT CC6						"#*#inkcc6#*#"

| TACVI
#EVENT VORRAMP					"#*#body bulges with strength as he enters a blind rage#*#"
#EVENT VORBLUR					"#*#Vor mem-blurs#*#"
#EVENT PIRIKACTIVE              "#*#Come you fools!#*#"
#EVENT BALANCE					"#*#Kvxe enters a state of battle#*#"
#EVENT BALANCERAPE				"#*#RapeHealOn#*#"
#EVENT BALANCERAPE				"#*#FullHeal#*#"
#EVENT BALANCERAPE				"#*#Kvxe enters a state of seething rage#*#"
#EVENT BALANCERAPEOFF			"#*#RapeHealOff#*#"
#EVENT BALANCEOFF			 	"#*#your failure to balance the scales#*#"
#EVENT BALANCEOFF			 	"#*#body trembles as he fails to gather power#*#"
#EVENT BALANCEOFF			 	"#*#BalanceOFF#*#"
#EVENT PIRIKDEAD				"#*#two heads face each other just before it falls to the floor#*#"

Sub InitEvents_GoD_Ikkinz
	/call RegisterRaidEvents Events_GoD_Ikkinz 18
/return

Sub InitEvents_GoD_Uqua
	/call RegisterRaidEvents Events_GoD_Uqua 18
/return

Sub InitEvents_GoD_Inktuta
	/call RegisterRaidEvents Events_GoD_Inktuta 9
	/declare inkRunning				bool 	outer   FALSE
/return

Sub InitEvents_GoD_Tacvi
	/call RegisterRaidEvents Events_GoD_Tacvi 9
	/declare balanceRapeHeal bool outer FALSE
	/declare bln_balancing bool outer FALSE
/return

Sub Events_GoD_Ikkinz
	/doevents KeeperPosition
/return

Sub Events_GoD_Uqua
	/doevents UquaKey
/return

Sub Events_GoD_Inktuta
	| Cursebearers

	/doevents CC1
	/doevents CC2
	/doevents CC3
	/doevents CC4
	/doevents CC5
	/doevents CC6
	
	/if (${Bool[${SpawnCount[npc Cursecaller]}]}) {
		/doevents InkCurse
		/doevents InkCurseSafe
	}		

	| Noqufiel
	/if (${Bool[${SpawnCount[npc Noqufiel]}]} && !${Bool[${SpawnCount[npc Cursecaller]}]}) {
		/doevents NoqCurse
		/doevents NoqSafe
		/if (${Me.Class.ShortName.Equal[WIZ]} || ${Me.Class.ShortName.Equal[MAG]}) /call chkNoqCursebearers
	}
	
/return


Sub Events_GoD_Tacvi
	/doevents VORRAMP
	/doevents VORBLUR
	/doevents PIRIKACTIVE
	/doevents BALANCE
	/doevents BALANCERUNE
	/doevents BALANCERAPE
	/doevents BALANCERAPEOFF
	/doevents BALANCEOFF
	/doevents PIRIKDEAD
/return


|-------------------------------------------------------------------------------- 
|SUB: Event_UquaKey
|--------------------------------------------------------------------------------
Sub Event_UquaKey(string line,string key)
  /docommand /${announceChannel} ${key}
  /doevents flush UquaKey
/return
|--------------------------------------------------------------------------------



Sub Event_InkCurse
	/docommand /${announceChannel} ${Me.Name} is Running.
	/call MeleeReset
	/varset inkRunning TRUE
	/call InkCurseRun
/return

Sub Event_InkCurseSafe
	/if (!${inkRunning}) /return
	/docommand /${announceChannel} ${Me.Name} is safe, returning.
	/varset inkRunning FALSE
	/maxfps bg 20
	/play off
/return

Sub InkCurseDead
	/docommand /${announceChannel} ${Me.Name} is dead, resetting.
	/varset inkRunning FALSE
	/play off
	/maxfps bg 20
	/delay 30
    /keypress back
/return

|----------------------------
| Noqufiel
|----------------------------

Sub Event_NoqCurse(_line, _chatSender, _eventParams)
	
	/if (${bln_tanking}) {
		/docommand /${announceChannel} I NEED TO RUN BUT TANKING!
		/return
	} 
	/echo Event_NoqCurse ${_eventParams}
	/declare curseID int local ${_eventParams.Arg[1]} 
	/echo curseID: ${curseID}
	/squelch /attack off 
	/squelch /stick off
	/call InkNoqRun ${curseID}
	/doevents flush NoqCurse
/return

Sub InkNoqRun(int _curseID)
	/if (${bln_tanking}) {
		/docommand /${announceChannel} I NEED TO RUN BUT TANKING!
		/return
	} 
	/docommand /${announceChannel} Running Noqufiel ${_curseID}
	/doevents flush
	/stick off
	/call Disengage
	|-- find which cursebearer has you on aggro;
	/call TrueTarget ${_curseID}
	/delay 10
	:checkCurseTarget
	/if (${Me.TargetOfTarget.Name.Equal[NULL]}) /goto :checkCurseTarget
	
	/echo Target ${_curseID} is targetting ${Me.TargetOfTarget.Name} ? ${Me.TargetOfTarget.Name.Equal[${Me.Name}]}
	
	|-- start run logic
	/varset inkRunning TRUE
	
	|-- make sure that fucker still has you on aggro, 
	|   if not, end this and continue what you were doing
	
	/if (${Me.TargetOfTarget.Name.Equal[${Me.Name}]}) {
		/say Target ${_curseID} is targetting me
	}
	
	| DOUBLE CHECK THIS RUN LOOP  TODO MAKE IT WORK LIKE CURSECALLERS
	:startLoop
	/if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call InkDeath
	/if (${Me.TargetOfTarget.Name.Equal[${Me.Name}]}) {
		/if (${Target.Distance} < 50) {
			/nav wp noqRun
			:startLoopRun 
			/delay 10
			/if (${Navigation.Active}) /goto :startLoopRun
			/goto :farLoop
		}
		/goto :startLoop
	} 	
	
	:farLoop
	/if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call InkDeath
	/if (${Me.TargetOfTarget.Name.Equal[${Me.Name}]} || !${Target.Name.Find[_corpse]}) {
		/if (${Target.Distance} < 50) {
			/varset inkRunning TRUE
			/call InkCurseRun
		}
	}
	|-- no longer targeted or you dont have a target. safe now

	/if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call InkDeath
	|-- insert some return logic here	
	/varset inkRunning FALSE
	/nav spawn pc ${str_MTName}
	|-- clear events queue so we dont get into an echo chamber
	/doevents flush
/return

| Method for the nukers to check for Cursebearers during the Noqufiel events
Sub chkNoqCursebearers
	/declare i 		int 	local 
	/declare victim string 	local 

	:curseBearerCheck
	/if (${SpawnCount[npc cursebearer]}) {
		/varset bln_engaged TRUE
		/varset QNUKE_DelayTimer 0
		/varset QNUKE_StopHPIfNamed 0
		/varset QNUKE_StartHPIfNamed 100
		/varset QNUKE_StopHP 0
		/varset QNUKE_StartHP 100
		/varset QNUKE_Mana_NamedMinimum 0
		/varset QNUKE_Mana_Minimum 0

		/for i 1 to ${SpawnCount[npc cursebearer]}
			/call TrueTarget ${NearestSpawn[${i},npc cursebearer].ID}
			| get the victim
			:getVictim
			/if (${Target.ID} && ${Target.Type.Equal[NPC]}) {
				/varset victim ${Me.TargetOfTarget.Name}
				/if (${victim.Equal[${Me.Name}]}) {
					/call InkNoqRun ${Target.ID}
				} else /if (${victim.NotEqual[NULL]}) {
					/squelch /dt ${victim} NoqCurse, ${Target.ID}
				} else {
					/delay 1
					/goto :getVictim
				}
			}
		/next i
	}

	/doevents NoqCurse

	/call TrueTarget ${NearestSpawn[1,npc cursebearer].ID}
	:curseBearerNuke 
		/if (${Target.ID} && ${Target.Type.Equal[NPC]}) {
			/call chkQNuke
			/goto :curseBearerNuke
		}


	:curseBearerPosition
	/if (${inkRunning}) /return 
	/declare campDist int local
	/varcalc campDist ${Math.Distance[${Me.Y},${Me.X}:${int_campY},${int_campX}]}
	/if (${campDist}>10) {
		/nav loc ${int_campX} ${int_campY} ${int_campZ}
	}
/return
|----------------------------
| Cursebearers
|----------------------------
Sub InkCurseRun
    /docommand /${announceChannel} Running Cursecallers
	/nav wp cc1
    /delay 3s
	/maxfps bg 30
	:start
		/if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call InkDeath
		/if (${AdvPath.State} == 0) /play cursecallers
	:movement
		/if (!${Me.Inventory[17].ID} && !${Me.Inventory[18].ID}) /call InkDeath
		/if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call InkDeath
		/if (${AdvPath.State} == 2) {
			/delay 10
			/doevents InkCurseSafe
			/doevents NoqSafe
			/if (!${inkRunning}) {
				/play off
				/doevents flush
				/return
			}
			/goto :movement
		}
		/goto :start
	
/return

Sub InkDeath
	/docommand /${announceChannel} ${Me.Name} is dead, Resetting.
	/varset inkRunning FALSE
	/play off
	/delay 30
    /keypress back
	:InkDeathWait
	/doevents flush
	/if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) {
		/delay 5s
		/goto :InkDeathWait
	}

/return

Sub Event_CC1 
  /if (!${inkRunning}) /nav wp cc1
  /doevents flush CC1
/return

Sub Event_CC2 
  /if (!${inkRunning}) /nav wp cc2
  /doevents flush CC2
/return

Sub Event_CC3 
  /if (!${inkRunning}) /nav wp cc3
  /doevents flush CC3
/return

Sub Event_CC4 
  /if (!${inkRunning}) /nav wp cc4
  /doevents flush CC4
/return

Sub Event_CC5 
  /if (!${inkRunning}) /nav wp cc5
  /doevents flush CC5
/return

Sub Event_CC6 
  /if (!${inkRunning}) /nav wp cc6
  /doevents flush CC6
/return

|--Ikkinz4
Sub Event_KeeperPosition
	/if (${str_classArchetype.Equal[MELEE_END]} ||  ${str_classArchetype.Equal[TANK]} ||  ${str_classArchetype.Equal[MELEE_MANA]}) {
		/echo Keeper punted me, re-positioning.
		/nav target
	} else /if (${str_classArchetype.Equal[CASTER]} ||  ${str_classArchetype.Equal[PRIEST]}) {
		/if (${Cast.Status.NotEqual[C]}) {
			/echo Keeper punted me, re-positioning.
			/nav wp kclr
		}
	}
	/doevents flush KeeperPosition
/return


Sub EVENT_VORRAMP
	/echo ah shit, yihst vor is about to rape my face
	/if (${Me.Class.ShortName.Equal[CLR]}) /call MQ2Cast "${str_heal_group}" gem1 5s
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call FireRiposteDisc
	} else {
		/return
	}
	/doevents flush VORRAMP
/return

Sub EVENT_VORBLUR
	/if (!${bln_engaged}) /return
	/if (${str_classArchetype.Equal[CASTER]} && ${Me.Casting.ID}) /call Interrupt
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]} && ${bln_engaged}) {
		/if (!${bln_riposting}) {
			/squelch /attack off 
			/squelch /stick off
			/delay 30
			/attack on
		}
	}
	/doevents flush VORBLUR
/return 


SUB EVENT_PIRIKACTIVE
	/if (${Me.Class.ShortName.Equal[ENC]}) {
		/if (!${bln_runeActive}) /call RuneSpam ${Spawn[pc id ${int_MTID}]}
		/return
	}

	/if (${Me.Class.ShortName.Equal[CLR]}) {
		/call InterruptOFF
	}
	/doevents flush PIRIKACTIVE
/return

SUB EVENT_BALANCE
	
	/declare bpct	int	local	100
	/varcalc bpct ${Spawn[npc Kvxe].PctHPs}+1
	/if (${str_classArchetype.Equal[PRIEST]}) {
		/echo PRIEST CLASS handling

		/if (${bpct} < 45) {
			/call ChangeHealMode NORMAL
			/call InterruptOFF
		} else {
			/call ChangeHealMode EFFICIENT
			/call InterruptON
		}
		/if (${bln_chChainMember}) /call heal_chPause 400
		/if (${Me.Class.ShortName.Equal[DRU]} || ${Me.Class.ShortName.Equal[SHM]}) /call Disengage
		/goto :return
	}
	/keypress esc
	|- determine percentage to run to based on Kvxe's Hitpoints
	/varset bln_balancing TRUE
	/if (${str_classArchetype.Equal[CASTER]}) {
		
		/echo CASTER CLASS checking
		/varset bln_engaged FALSE
		/call Disengage
		/pet back off
		/if (${Me.Class.ShortName.Equal[ENC]}) /call RuneSpamOff

		| 03/24/2023 - XIRIS CREW DPS is very high, so high that we don't need casters helping, as it will just fuck them up.
		/goto :return

		|-pre 70% dont help with anything
		/if (${bpct} >= 75) {
			/call BalancerReset
			/goto :return
		}

		|-post 70% have the casters help with defense
		/if (${bpct} < 75 && ${bpct} >=65) {
			|-- Balance of Defense	
			/call BalancerKillCaster "Balance of Defense" ${Spawn[npc radius 500 Defense].ID} ${bpct}
			/call BalancerReset
			
		} 
		|-post 60% have the casters help with rage
		/if (${bpct} < 65 && ${bpct} >=55) {
			|-- Balance of Defense	
			/call BalancerKillCaster "Balance of Rage" 		${Spawn[npc radius 500 Rage].ID} ${bpct}
			/call BalancerKillCaster "Balance of Defense" 	${Spawn[npc radius 500 Defense].ID} ${bpct}
			/call BalancerReset
		} 

		|-post 50% have the casters help with fury
		/if (${bpct} < 55 && ${bpct} >=45) {
			|-- Balance of Defense	
			/call BalancerKillCaster "Balance of Fury" 		${Spawn[npc radius 500 Fury].ID} ${bpct}
			/call BalancerKillCaster "Balance of Rage" 		${Spawn[npc radius 500 Rage].ID} ${bpct}
			/call BalancerKillCaster "Balance of Defense" 	${Spawn[npc radius 500 Defense].ID} ${bpct}
			/call BalancerReset
		} 

		|-post 40% have the casters help with speed;
		/if (${bpct} < 45) {
			|-- Balance of Speed
			/call BalancerKillCaster "Balance of Speed" 	${Spawn[npc radius 500 Speed].ID} ${bpct}
			/call BalancerKillCaster "Balance of Fury" 		${Spawn[npc radius 500 Fury].ID} ${bpct}
			/call BalancerKillCaster "Balance of Rage" 		${Spawn[npc radius 500 Rage].ID} ${bpct}
			/call BalancerKillCaster "Balance of Defense" 	${Spawn[npc radius 500 Defense].ID} ${bpct}
			/call BalancerReset
		} 
	}

	|-- Melee Classes
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		|-- Balance of Speed	
		/squelch /attack off 
		/squelch /stick off
		/call BalancerKillMelee "Balance of Speed" ${Spawn[npc radius 500 Speed].ID} ${bpct}

		|-- Balance of Fury	
		/squelch /attack off 
		/squelch /stick off
		/call BalancerKillMelee "Balance of Fury" ${Spawn[npc radius 500 Fury].ID} ${bpct}

		|-- Balance of Rage
		/squelch /attack off 
		/squelch /stick off
		/call BalancerKillMelee "Balance of Rage" ${Spawn[npc radius 500 Rage].ID} ${bpct}
			
		|-- Balance of Defense	
		/squelch /attack off 
		/squelch /stick off
		/call BalancerKillMelee "Balance of Defense" ${Spawn[npc radius 500 Defense].ID} ${bpct}

		|-- return
		/call BalancerReset
	} 
:return
/doevents flush BALANCE
/return	

Sub BalancerKillMelee(string _targetName, int _targetID, int _balanceTo)
	|-- Balance of _targetName
	/keypress esc
	/g BalancerKillMelee ${_targetName}, ${_targetID} =>${_balanceTo}
	/call TrueTarget ${_targetID}
	/nav target
	:BalancerMeleeMove
		/echo BalancerMeleeMove
		/delay 10
		/if (${Target.Distance} < 25) /goto :BalancerMeleeKill
		/if (${Navigation.Active}) /goto :BalancerMeleeMove
	:BalancerMeleeKill
		/echo BalancerKillMelee ${_targetName}, ${_targetID} =>${_balanceTo}
		/varset bln_balancing TRUE
		/nav stop
		/call EngageTarget ${Target.ID}
	:BalancerMeleeLoop
		/if (${Target.PctHPs} < ${_balanceTo}) /goto :return
		/doevents GETCLOSER
		/doevents RESTICK
		/doevents BALANCEOFF
		/doevents BurnOn
		/doevents BurnOn1
		/doevents BurnOn2
		/doevents BurnOnAll
		/doevents BurnOff
		/call chkBurn
		/if (!${bln_balancing}) /goto :return
		/if (${Target.PctHPs} > ${_balanceTo}) /goto :BalancerMeleeLoop
	:return
	/echo BalancerKillMelee return
	/call Disengage
	/varset bln_engaged FALSE
/return 

Sub BalancerKillCaster(string _targetName, int _targetID, int _balanceTo)
	|-- Balance of _targetName
	/call TrueTarget ${_targetID}
	/if (${QNUKE_Total} > 0 && ${Me.Class.ShortName.Equal[WIZ]}) {
		/echo setting fast only
		/varset bln_useFastOnly TRUE
	}
	/g BalancerKillCaster ${_targetName}, ${_targetID} =>${_balanceTo}
	:BalancerCasterKill
		/echo BalancerKillCaster ${_targetName}, ${_targetID} =>${_balanceTo}
		/varset bln_balancing TRUE
		/call EngageTarget ${_targetID}
	:BalancerCasterLoop
		/if (${Target.PctHPs} < ${_balanceTo}) /goto :return
		|/echo BalancerCasterLoop
		/doevents BALANCEOFF
		/doevents BurnOn
		/doevents BurnOn1
		/doevents BurnOn2
		/doevents BurnOnAll
		/doevents BurnOff
		/call chkBurn
		/if (!${bln_balancing}) /goto :return
		/if (${Target.PctHPs} > ${_balanceTo}) /goto :BalancerCasterLoop
	:return
	/echo BalancerKillCaster return
	/pet hold on
	/pet back off
	/call Disengage
	/varset bln_engaged FALSE
/return

Sub EVENT_BALANCEOFF
	/call BalancerReset
	/doevents flush BALANCEOFF
/return

Sub BalancerReset
	/echo BALANCE OFF ${Target.Name}
	/varset bln_balancing FALSE
	
	/if (${str_classArchetype.Equal[PRIEST]}) /return

	/doevents flush
	/call Disengage

	|-- Kvxe
	/if (${str_classArchetype.NotEqual[CASTER]}) {
		/target npc Kvxe
		/delay 5
		/nav target

		:moveKvxe
			/delay 10
			/if (${Target.Distance} < 35) /goto :returnBalance
			/if (${Navigation.Active}) /goto :moveKvxe

		:returnBalance
		/if (${Navigation.Active}) /nav stop
		/stick ${stickCmd} ${int_currentStickDistance}
		/delay 10
		/call Disengage
		
		/if ((${Me.Class.ShortName.Equal[WAR]} || ${Me.Class.ShortName.Equal[SHD]} || ${Me.Class.ShortName.Equal[PAL]}) && ${int_MTID} != ${Me.ID}) /varset bln_tanking FALSE
	}
	/call RunRaidEvents	
/return



Sub EVENT_BALANCERAPE
 	/varset  balanceRapeHeal TRUE
	|- rune spam
	/if (${Me.Class.ShortName.Equal[ENC]}) /call RuneSpam ${Spawn[pc id ${int_MTID}]}

	|- exit if not priest
	/if (${str_classArchetype.NotEqual[PRIEST]}) /return
	/if (${bln_chChainMember}) {
		/echo part of a CHCHain, probably not doing this;
		/return
	}
 
	/echo Turning Spam Healing on.
	:balanceRapeHeal
		/doevents BALANCERAPEOFF
		/if (!${balanceRapeHeal}) /return
		/if (${bln_annouce}) /${cmdChannel} ${str_currentHeal} > ${Target.Name} hp:${Target.PctHPs} m:${Me.PctMana} d:${Target.Distance}
		/call MQ2Cast "${str_heal_normal}" gem2 1s
		/if (!${SpawnCount[npc kvxe]}) /return
	/goto :balanceRapeHeal
	/doevents flush BALANCERAPE
/return
	
Sub EVENT_BALANCERAPEOFF
	/echo Turning Spam Healing off.
	/varset balanceRapeHeal FALSE
	/doevents flush BALANCERAPEOFF
/return	


	

	
	
	
	
	
	
	
	
	
	
	