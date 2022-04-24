|**
----------------------------
EVENTS_RAID_GOD.INC
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
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
#event UquaKey                  "The #1# Key must unlock the door to the next room."

| IKK4
#event KeeperPosition			"#*#KeeperPosition#*#"

| INKTUTA
|** Positions For Noqufiel:
    ncf - noq cleric fake (left)
	ncr - noq cleric real (right)
	ntf	- noq tank fake (left)
	ntr - noq tank real (right)
	nw1 - noq wiz 1
	nw2 - noq wiz 2
**|

#event NoqCurse					"[#1#(msg)] NoqCurse,#2#"
#event InkCurse					"#*#The thoughts of a cursed trusik#*#"
#event InkCurseSafe				"#*#inkstop#*#"
#event InkCurseSafe				"#*#inksafe#*#"
#event CC1						"#*#inkcc1#*#"
#event CC2						"#*#inkcc2#*#"
#event CC3						"#*#inkcc3#*#"
#event CC4						"#*#inkcc4#*#"
#event CC5						"#*#inkcc5#*#"
#event CC6						"#*#inkcc6#*#"

| TACVI
#event VORRAMP					"#*#body bulges with strength as he enters a blind rage#*#"
#event VORBLUR					"#*#Vor mem-blurs#*#"
#event PIRIKACTIVE              "#*#Come you fools!#*#"
#event BALANCE					"#*#Kvxe enters a state of battle#*#"
#event BALANCERAPE				"#*#RapeHealOn#*#"
#event BALANCERAPE				"#*#FullHeal#*#"
#event BALANCERAPE				"#*#Kvxe enters a state of seething rage#*#"
#event BALANCERAPEOFF			"#*#RapeHealOff#*#"
#event BALANCEOFF			 	"#*#your failure to balance the scales#*#"
#event BALANCEOFF			 	"#*#body trembles as he fails to gather power#*#"
#event BALANCEOFF			 	"#*#BalanceOFF#*#"

Sub InitEvents_GoD_Ikkinz
	/call RegisterBackgroundEvents Events_GoD_Ikkinz 18
/return

Sub InitEvents_GoD_Uqua
	/call RegisterBackgroundEvents Events_GoD_Uqua 18
/return

Sub InitEvents_GoD_Inktuta
	/call RegisterBackgroundEvents Events_GoD_Inktuta 9
	/declare inkRunning				bool 	outer   FALSE
/return

Sub InitEvents_GoD_Tacvi
	/call RegisterBackgroundEvents Events_GoD_Tacvi 18
/return

Sub Events_GoD_Ikkinz
	/doevents KeeperPosition
/return

Sub Events_GoD_Uqua
	/doevents UquaKey
/return

Sub Events_GoD_Inktuta
	/doevents NoqCurse
	/doevents InkCurse
	/doevents InkCurseSafe
	/doevents CC1
	/doevents CC2
	/doevents CC3
	/doevents CC4
	/doevents CC5
	/doevents CC6
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
/return


|-------------------------------------------------------------------------------- 
|SUB: Event_UquaKey
|--------------------------------------------------------------------------------
Sub Event_UquaKey(string line,string key)
  /rs ${key}
/return
|--------------------------------------------------------------------------------



Sub Event_InkCurse
	/if (${SpawnCount[npc cursecaller]}==0) /return
	|/melee reset
	/call meleeReset
	/varset inkRunning TRUE
	/call InkCurseRun
/return

Sub Event_InkCurseSafe
	/if (!${inkRunning}) /return
	/rs ${Me.Name} is safe, returning.
	/varset inkRunning FALSE
	/maxfps bg 25
	/play off
	/alert clear 2
/return

Sub InkCurseDead
	/rs ${Me.Name} is dead, resetting.
	/varset inkRunning FALSE
	/play off
	/delay 30
    /keypress back
/return

Sub Event_NoqCurse(ChatType,Sender,ChatText,_curseID)
	/echo Event_NoqCurse ${ChatText}
	/declare curseID int local ${ChatText.Arg[1]} 
	/echo curseID: ${curseID}
	/melee reset
	|/call meleeReset
	/varset inkRunning TRUE
	/call InkNoqRun ${curseID}
/return

Sub InkNoqRun(int curseID)
	/rs Running Noqufiel ${curseID}
	/doevents flush
	/declare startX int local ${Me.X}
	/declare startY int local ${Me.Y}
	/declare startZ int local ${Me.Z}
	
	|-- find which cursebearer has you on aggro;
	/target id ${curseID}
	/delay 10
	:checkCurseTarget
	/if (${Me.TargetOfTarget.Name.Equal[NULL]}) /goto :checkCurseTarget
	
	/echo Target ${curseID} is targetting ${Me.TargetOfTarget.Name} ? ${Me.TargetOfTarget.Name.Equal[${Me.Name}]}
	
	|-- start run logic
	/varset inkRunning TRUE
	
	|-- make sure that fucker still has you on aggro, 
	|   if not, end this and continue what you were doing
	
	/if (${Me.TargetOfTarget.Name.Equal[${Me.Name}]}) {
		/say Target ${curseID} is targetting me
	}
	
	:startLoop
	/if (${Zone.ShortName.Equal[GuildLobby]}) /call InkDeath
	/if (${Me.TargetOfTarget.Name.Equal[${Me.Name}]}) {
		/if (${Target.Distance} < 50) {
			/nav wp noqRun
			/delay 10
			/goto :farLoop
		}
		/goto :startLoop
	} 	
	
	:farLoop
	/if (${Zone.ShortName.Equal[GuildLobby]}) /call InkDeath
	/if (${Me.TargetOfTarget.Name.Equal[${Me.Name}]}) {
		/if (${Target.Distance} < 50) {
			/nav loc ${startY} ${startX} ${startZ}
			/delay 10
			/goto :startLoop
		}
		/goto :farLoop
	}
	|-- no longer targeted or you dont have a target. safe now
	/rs ${Me.Name} is safe, returning.
	/if (${Zone.ShortName.Equal[GuildLobby]}) /call InkDeath
	|-- insert some return logic here	
	/varset inkRunning FALSE
	/nav loc ${startY} ${startX} ${startZ}
	|-- clear events queue so we dont get into an echo chamber
	/doevents flush
	/return
/return

Sub InkCurseRun
    /rs Running Cursecallers
	/nav wp cursedentrance
    /delay 3s
	/maxfps bg 30
	:start
		/if (${Zone.ShortName.Equal[GuildLobby]}) {
			/varset inkRunning FALSE
			/play off
			/doevents flush
			/return
		}
		/if (${AdvPath.State} == 0) {
			/play cursecallers
		}
	:movement
		/if (!${Me.Inventory[17].ID} && !${Me.Inventory[18].ID}) /call InkDeath
		/if (${Zone.ShortName.Equal[GuildLobby]}) /call InkDeath
		/if (${AdvPath.State} == 2) {
			/delay 1s
			/doevents InkCurseSafe
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
	/rs ${Me.Name} is dead, resetting.
	
	/varset inkRunning FALSE
	/play off
	/delay 30
    /keypress back
	:InkDeathWait
	/doevents flush
	
	/if (${Zone.ShortName.Equal[GuildLobby]}) {
		/delay 5s
		/goto :InkDeathWait
	}

/return

Sub Event_CC1 
  /if (!${inkRunning}) /nav wp cc1
/return

Sub Event_CC2 
  /if (!${inkRunning}) /nav wp cc2
/return

Sub Event_CC3 
  /if (!${inkRunning}) /nav wp cc3
/return

Sub Event_CC4 
  /if (!${inkRunning}) /nav wp cc4
/return

Sub Event_CC5 
  /if (!${inkRunning}) /nav wp cc5
/return

Sub Event_CC6 
  /if (!${inkRunning}) /nav wp cc6
/return

|--Ikkinz4
Sub Event_KeeperPosition
	/if (${str_classArchetype.Equal[MELEE_END]} ||  ${str_classArchetype.Equal[TANK]} ||  ${str_classArchetype.Equal[MELEE_MANA]}) {
		/echo Keeper punted me, re-positioning.
		/nav Target
		|/stick 10 behind
	} else /if (${str_classArchetype.Equal[CASTER]} ||  ${str_classArchetype.Equal[PRIEST]}) {
		/if (${Cast.Status.NotEqual[C]}) {
			/echo Keeper punted me, re-positioning.
			/nav wp kclr
		}
	}
/return


Sub EVENT_VORRAMP
	/echo ah shit, yihst vor is about to rape my face
	/if (${Me.Class.ShortName.Equal[CLR]}) /call MQ2Cast "${str_heal_group}" gem1 5s
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call FireRiposteDisc
	} else {
		/return
	}
/return

Sub EVENT_VORBLUR
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/if (!${bln_riposting}) {
			/melee reset
			/attack off
			/delay 30
			/killthis
			/attack on
		}
	}
/return 


SUB EVENT_PIRIKACTIVE
	/if (${Me.Class.ShortName.Equal[ENC]}) {
		/if (!${bln_runeActive}) /call RuneSpam ${Spawn[pc id ${int_MTID}]}
		/return
	}

	/if (${Me.Class.ShortName.Equal[CLR]}) {
		/call InterruptOFF
	}
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
		/return
	}
	
	|- determine percentage to run to based on Kvxe's Hitpoints
	/if (!${Defined[bln_balancing]}) /declare bln_balancing bool outer TRUE
	/varset bln_balancing TRUE
	/if (${str_classArchetype.Equal[CASTER]}) {
		/echo CASTER CLASS checking
		/varset bln_engaged FALSE
		/call targetReset
		/pet back off
		/if (${Me.Class.ShortName.Equal[ENC]}) /call RuneSpamOff

		|-pre 70% dont help with anything
		/if (${bpct} >= 75) {
			/call BalancerReset
			/return
		}

		|-post 70% have the casters help with defense
		/if (${bpct} < 75 && ${bpct} >=65) {
			|-- Balance of Defense	
			/call BalancerKillCaster "Balance of Defense" ${Spawn[npc radius 5000 Defense].ID} ${bpct}
			
		} 
		|-post 60% have the casters help with rage
		/if (${bpct} < 65 && ${bpct} >=55) {
			|-- Balance of Defense	
			/call BalancerKillCaster "Balance of Rage" 		${Spawn[npc radius 5000 Rage].ID} ${bpct}
			/call BalancerKillCaster "Balance of Defense" 	${Spawn[npc radius 5000 Defense].ID} ${bpct}
		} 

		|-post 50% have the casters help with fury
		/if (${bpct} < 55 && ${bpct} >=45) {
			|-- Balance of Defense	
			/call BalancerKillCaster "Balance of Fury" 		${Spawn[npc radius 5000 Fury].ID} ${bpct}
			/call BalancerKillCaster "Balance of Rage" 		${Spawn[npc radius 500 Rage].ID} ${bpct}
			/call BalancerKillCaster "Balance of Defense" 	${Spawn[npc radius 5000 Defense].ID} ${bpct}
		} 

		|-post 40% have the casters help with speed;
		/if (${bpct} < 45) {
			|-- Balance of Speed
			/call BalancerKillCaster "Balance of Speed" 	${Spawn[npc radius 5000 Speed].ID} ${bpct}
			/call BalancerKillCaster "Balance of Fury" 		${Spawn[npc radius 5000 Fury].ID} ${bpct}
			/call BalancerKillCaster "Balance of Rage" 		${Spawn[npc radius 5000 Rage].ID} ${bpct}
			/call BalancerKillCaster "Balance of Defense" 	${Spawn[npc radius 5000 Defense].ID} ${bpct}
		} 
	}

	|-- Melee Classes
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		|-- warrior needs to bounce around aggro on balancers
		/if (${Me.Class.ShortName.Equal[WAR]}) /melee aggro=on
	
		|-- Balance of Speed	
		/call BalancerKillMelee "Balance of Speed" ${Spawn[npc radius 500 Speed].ID} ${bpct}

		|-- Balance of Fury	
		/call BalancerKillMelee "Balance of Fury" ${Spawn[npc radius 500 Fury].ID} ${bpct}

		|-- Balance of Rage
		/call BalancerKillMelee "Balance of Rage" ${Spawn[npc radius 500 Rage].ID} ${bpct}
			
		|-- Balance of Defense	
		/call BalancerKillMelee "Balance of Defense" ${Spawn[npc radius 500 Defense].ID} ${bpct}

		|-- return
		/call BalancerReset
	} 
:return
/return	

Sub BalancerKillMelee(string _targetName, int _targetID, int _balanceTo)
	|-- Balance of _targetName
	/g BalancerKillMelee ${_targetName}, ${_targetID} =>${_balanceTo}
	/call TrueTarget ${_targetID}
	/melee on
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
		/call engageTarget ${Target.ID}
	:BalancerMeleeLoop
		|/echo BalancerMeleeLoop
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
	/melee reset
	/call targetReset
	/varset bln_engaged FALSE
/return 

Sub BalancerKillCaster(string _targetName, int _targetID, int _balanceTo)
	|-- Balance of _targetName
	/call TrueTarget ${_targetID}
	/if (${int_QnukeTotal} > 0 && ${Me.Class.ShortName.Equal[WIZ]}) {
		/echo setting fast only
		/varset bln_useFastOnly TRUE
	}
	/g BalancerKillCaster ${_targetName}, ${_targetID} =>${_balanceTo}
	:BalancerCasterKill
		/echo BalancerKillCaster ${_targetName}, ${_targetID} =>${_balanceTo}
		/varset bln_balancing TRUE
		/call engageTarget ${_targetID}
		/if (${Me.Class.ShortName.Equal[WIZ]}) /twist 3 4
		/if (${Me.Class.ShortName.Equal[NEC]}) /twist 7
		/if (${Me.Class.ShortName.Equal[MAG]}) /twist 4
		/if (${Me.Class.ShortName.Equal[ENC]}) /twist 4
	:BalancerCasterLoop
		|/echo BalancerCasterLoop
		/doevents BALANCEOFF
		/doevents BurnOn
		/doevents BurnOn1
		/doevents BurnOn2
		/doevents BurnOnAll
		/doevents BurnOff
		/if (!${bln_balancing}) /goto :return
		/if (${Target.PctHPs} > ${_balanceTo}) /goto :BalancerCasterLoop
	:return
	/if (${Twist.Twisting}) /twist off
	/call targetReset
	/varset bln_engaged FALSE
/return

Sub EVENT_BALANCEOFF
	/call BalancerReset
/return

Sub BalancerReset
	/echo BALANCE OFF ${Target.Name}
	/if (${Twist.Twisting}) /twist off
	/if (${Defined[bln_balancing]}) /varset bln_balancing FALSE
	/if (${str_classArchetype.Equal[PRIEST]}) /return
	
	/call targetReset
	/doevents flush
	/melee reset

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
		/melee reset
		/if (${Me.Class.ShortName.Equal[WAR]} && ${int_MTID} != ${Me.ID}) /melee aggro=off
	}
	/call RunRaidEvents	
/return



Sub EVENT_BALANCERAPE
	|- rune spam
	/if (${Me.Class.ShortName.Equal[ENC]}) /call RuneSpam ${Spawn[pc id ${int_MTID}]}

	|- exit if not priest
	/if (${str_classArchetype.NotEqual[PRIEST]}) /return
	/if (${bln_chChainMember}) {
		/echo part of a CHCHain, probably not doing this;
		/return
	}

	/if (!${Defined[balanceRapeHeal]}) 	   /declare balanceRapeHeal bool outer TRUE
	/echo Turning Spam Healing on.
	:balanceRapeHeal
		/call RunRaidEvents	
		/if (!${balanceRapeHeal}) /return
		/if (${bln_annouce}) /${cmdChannel} ${str_currentHeal} > ${Target.Name} hp:${Target.PctHPs} m:${Me.PctMana} d:${Target.Distance}
		/call MQ2Cast "${str_heal_normal}" gem2 1s
		|/if (${Me.Class.ShortName.Equal[CLR]}) /call chkDivineIntervention
	/goto :balanceRapeHeal
/return
	
Sub EVENT_BALANCERAPEOFF
	/echo Turning Spam Healing off.
	/if (${str_classArchetype.NotEqual[PRIEST]}) /return
	/if (${Defined[balanceRapeHeal]}) /varset balanceRapeHeal FALSE
/return	


	

	
	
	
	
	
	
	
	
	
	
	