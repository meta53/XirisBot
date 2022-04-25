|**
----------------------------
EVENTS_OOW_MPG.INC updated version 04.03.22 
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	04.03.22    xiris  INITIAL REVISION
______________________________________________
**|	
|-- Muramite Proving Grounds events

|-- Mastery of Foresight
#event ForesightNorth		"#*#dramatically. The north side looks safe#*#"
#event ForesightSouth		"#*#dramatically. The south side looks safe#*#"
#event ForesightEast		"#*#dramatically. The east side looks safe.#*#"
#event ForesightWest		"#*#dramatically. The west side looks safe.#*#"
#event ForesightCenter		"#*#dramatically."


#event ForesightAESafe		"#*#You escape the blast unscathed#*#"
#event ForesightAESafe		"#*#The air around you ignites#*#"


#event ForesightReflect		"#*#developing an anti-magic aura#*#"


#event ForesightPlatform	"#*#You should find higher ground#*#"
#event ForesightPlatformSafe  "#*#You escape the blast unscathed#*#"
#event ForesightPlatformSafe "#*#The air around you ignites#*#"

#event ForesightAOEDPS		"#*#Doing enough damage to him might interrupt the process#*#"
#event ForesightDS			"#*#before you is sprouting shard spikes#*#"
#event ForesightDS			"#*#you are pierced by thorns"

#event ForesightKyvMove		"#*#You should move#*#"
#event ForesightKyvMoveSafe "#*#You are struck by a stray arrow#*#"
#event ForesightKyvMoveSafe "#*#An arrow narrowly misses you#*#"

#event ForesightKyvStill	"#*#You should stand still#*#"
#event ForesightKyvStillSafe "#*#You are struck by a stray arrow#*#"
#event ForesightKyvStillSafe "#*#An arrow narrowly misses you#*#"

#event ForesightKyvDuck		"#*#You should duck#*#"
#event ForesightKyvDuckSafe "#*#Your head is struck#*#"
#event ForesightKyvDuckSafe "#*#An arrow narrowly misses you#*#"

#event ForesightWeapon		"#*#you should remove it#*#"
#event ForesightWeaponSafe  "#*#Your weaponry cools down#*#"
#event ForesightWeaponSafe  "#*#searing your hands#*#"

#event ForesightRing		"#*#bite into your fingers#*#"
#event ForesightRingSafe	"#*#You regain the use of your fingers#*#"
#event ForesightRingSafe	"#*#Your rings clamp down#*#"

#event ForesightAEClose		"#*#before you is preparing to cast a devastating close-range spell#*#"
#event ForesightAECloseSafe	"#*#You escape the blast unscathed#*#"
#event ForesightAECloseSafe	"#*#The air around you ignites#*#"

#event D1 					"#*#MoveD1#*#"
#event D2					"#*#MoveD2#*#"
#event D3 					"#*#MoveD3#*#"
#event D4 					"#*#MoveD4#*#"
#event D5 					"#*#MoveD5#*#"
#event D6 					"#*#MoveD6#*#"

Sub InitEvents_OOW_MPG_Foresight
	/cecho \ag Initializing MPG Foresight
	/call RegisterRaidEvents Events_OOW_MPG_Foresight 24
	/declare IsMovingNorth			bool	outer	FALSE
	/declare IsMovingSouth			bool	outer	FALSE
	/declare IsMovingEast			bool	outer	FALSE
	/declare IsMovingWest			bool	outer	FALSE
	/declare IsMovingCenter			bool	outer	FALSE
	
	/declare IsMovingAnywhere		bool	outer	FALSE
	/declare IsMovingToPlatform		bool	outer	FALSE
	/declare IsMovingOutOfMeleeAE	bool	outer	FALSE
	/declare IsHoldingStill			bool	outer	FALSE
	/declare IsDucking				bool	outer	FALSE
	/declare IsAvoidingDS			bool	outer	FALSE
	/declare IsAvoidingReflect		bool	outer	FALSE
	/declare IsFocusingDPS			bool	outer	FALSE
	/declare HasRemovedWeapon		bool	outer	FALSE
	/declare MainHandWeapon			string	outer	NULL
	/declare WeaponSlot				int		outer	1
	/declare HasRemovedRings		bool	outer	FALSE
	/declare Ring_1_Name			string	outer	NULL
	/declare Ring_1_Slot			int		outer	1
	/declare Ring_2_Name			string	outer	NULL
	/declare Ring_2_Slot			int		outer	1
/return

Sub Events_OOW_MPG_Foresight
	|/doevents ForesightNorth
	|/doevents ForesightSouth
	|/doevents ForesightEast
	|/doevents ForesightWest
	|/doevents ForesightCenter
	|/doevents ForesightPlatform
	|/doevents ForesightPlatformSafe


	/doevents ForesightKyvMove
	/doevents ForesightKyvMoveSafe
	/doevents ForesightKyvStill
	/doevents ForesightKyvStillSafe
	/doevents ForesightKyvDuck
	/doevents ForesightKyvDuckSafe
	/doevents ForesightKyvSafe

	|/doevents ForesightWeapon
	|/doevents ForesightWeaponSafe

	/doevents ForesightRing
	|/doevents ForesightRingSafe

	|/doevents ForesightAEClose
	|/doevents ForesightAECloseSafe

	|--events that you need to just wait out
	/doevents ForesightReflect
	|/doevents ForesightAEMaxDPS
	/doevents ForesightDS
	

	|-- Killing Events
	/doevents D1
	/doevents D2
	/doevents D3
	/doevents D4
	/doevents D5
	/doevents D6
/return

Sub InitEvents_MPG_Hate
/return

Sub InitEvents_MPG_Endurance
/return

Sub InitEvents_MPG_Specialization
/return

Sub InitEvents_MPG_Adaptation
/return

Sub InitEvents_MPG_Corruption
/return

Sub ForesightNavigation(string _wp, string _watch, string _event)
	/say Navigating to ${_wp}
	/declare atk bool local FALSE
	/declare tid int local ${Target.ID}
	/cecho \agForesightNavigation: SafeWP: ${_wp}, VariableWatch: ${_watch}, SafeEvent: ${_event}
	
	|- record the origin waypoint
	/nav recordwaypoint origin

	|- turn off melee & movement
	/if (${Melee.Attack})   /varset atk TRUE
	/if (${Navigation.Active}) /nav stop
	/if (${Melee.Sticking})    /stick off
	/melee reset

	|- set the watch variable to TRUE
	/varset ${_watch} TRUE
	|- nav to the safe spot
	/nav wp ${_wp}

	|- movement & event waiting loop
	:movementDelay
	/if (${Navigation.Active} || ${${_watch}}) {
		/echo Watching variable: ${_watch} ${${_watch}} from event ${_event}
		/delay 20
		/doevents ${_event}
		/doevents flush ${_event}
		/goto :movementDelay
	}


	/echo Safe event fired, returning to origin
	/varset ${_watch} FALSE
	/navigate wp origin
	/say Navigating to origin
	/call TrueTarget ${tid}
	:returnDelay
	/if (${Target.Distance} >= 40) {
		/delay 10
		/goto :returnDelay
	}
	/nav stop
	/if (${atk}) /killthis
	
/return

Sub EVENT_ForesightNorth
	/doevents flush ForesightNorth
	|/call ForesightNavigation SafeNorth IsMovingNorth ForesightAESafe
/return

Sub EVENT_ForesightSouth
	/doevents flush ForesightSouth
	|/call ForesightNavigation SafeSouth IsMovingSouth ForesightAESafe
/return

Sub EVENT_ForesightEast
	/doevents flush ForesightEast
	|/call ForesightNavigation SafeEast IsMovingEast ForesightAESafe
/return

Sub EVENT_ForesightWest
	/doevents flush ForesightWest
	|/call ForesightNavigation SafeEast IsMovingWest ForesightAESafe
/return

Sub EVENT_ForesightCenter
	/doevents flush ForesightCenter
	|/call ForesightNavigation SafeCenter IsMovingCenter ForesightAESafe
/return



Sub EVENT_ForesightAESafe
	/if (${IsMovingNorth}) 		/varset IsMovingNorth 	FALSE
	/if (${IsMovingSouth}) 		/varset IsMovingSouth 	FALSE
	/if (${IsMovingEast}) 		/varset IsMovingEast 	FALSE
	/if (${IsMovingWest}) 		/varset IsMovingWest 	FALSE
	/if (${IsMovingCenter}) 	/varset IsMovingCenter 	FALSE
	/if (${IsMovingAnywhere}) 	/varset IsMovingAnywhere FALSE	
	/doevents flush EVENT_ForesightAESafe
/return


Sub EVENT_ForesightKyvMove
	/say Kyv Moving
	/doevents flush ForesightKyvMove
	| The Kyv will cast spell 5694 Headshot 10000 DD, Feign Death
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) {
		| casters and healers need to move to SafeCenterCaster
		|/echo KyvMove
		|/call ForesightNavigation D4 IsMovingAnywhere ForesightKyvSafe
		/keypress forward hold
		/delay 20
		/keypress back
		/varset IsMovingAnywhere FALSE
	} else {
		| Melee need to move to SafeCenterDPS
		|/call ForesightNavigation D4 IsMovingAnywhere ForesightKyvSafe
	}
/return

Sub EVENT_ForesightKyvMoveSafe 
	/varset IsMovingAnywhere FALSE
/return 



Sub EVENT_ForesightKyvStill
	/say Kyv Holding Still
	/doevents flush ForesightKyvStill
	| The Kyv will cast spell 5694 Headshot 10000 DD, Feign Death
	|- set initial states (to restore at end)
	/declare atk bool local FALSE
	/declare tid int local ${Target.ID}
	/declare kyvstill timer local 30s
	/if (${Melee.Attack})   /varset atk TRUE
	/if (${Navigation.Active}) /nav stop
	/if (${Melee.Sticking})    /stick off
	/melee reset

	|- set the watch variable to TRUE
	/varset IsHoldingStill TRUE
	
	:stillnessDelay
	/if (${IsHoldingStill}) {
		/echo Watching variable: IsHoldingStill ${IsHoldingStill} from event ForesightKyvSafe
		/delay 20
		/doevents ForesightKyvStillSafe
		/doevents flush ForesightKyvStillSafe
		/if (!${kyvstill}) /goto :return
		/goto :stillnessDelay
	}
	:return
	/varset IsHoldingStill FALSE
	/if (${Me.Ducking}) /keypress duck
	/if (${Me.State.NotEqual[STAND]}) /stand
	/call TrueTarget ${tid}
	/if (${atk}) /killthis
/return

Sub EVENT_ForesightKyvStillSafe
	/varset IsHoldingStill FALSE
	/if (${Me.Ducking}) /keypress duck
	/if (${Me.State.NotEqual[STAND]}) /stand
/return

Sub EVENT_ForesightKyvDuck
	/say Kyv Ducking
	/doevents flush ForesightKyvDuck

	| The Kyv will cast spell 5696 Kyv Arrow 7500 DD
	|- set initial states (to restore at end)
	/declare atk bool local FALSE
	/declare tid int local ${Target.ID}
	/declare kyvduck timer local 30s
	/if (${Melee.Attack})   /varset atk TRUE
	/if (${Navigation.Active}) /nav stop
	/if (${Melee.Sticking})    /stick off
	/melee reset

	/if (${Me.Sitting})  /stand on
	/if (${Me.Standing}) {
		/keypress duck
	}
	|- set the watch variable to TRUE
	/varset IsDucking TRUE

	:duckDelay
	/if (${IsDucking}) {
		|/echo ${Me.Ducking} Watching variable: IsDucking ${IsDucking} from event ForesightKyvSafe
		/delay 20
		/doevents ForesightKyvDuckSafe
		/doevents flush ForesightKyvDuckSafe
		/if (!${kyvduck}) /goto :return
		/goto :duckDelay
	}
	:return
	/varset IsDucking FALSE
	/if (${Me.Ducking}) /keypress duck
	/if (${Me.State.NotEqual[STAND]}) /stand
	/call TrueTarget ${tid}
	/if (${atk}) /killthis
/return

Sub EVENT_ForesightKyvDuckSafe
	/varset IsDucking FALSE
	/if (${Me.Ducking}) /keypress duck
	/if (${Me.State.NotEqual[STAND]}) /stand
/return


Sub EVENT_ForesightAEClose
	/say AEClose to Dragorn!
	/doevents flush EVENT_ForesightAEClose

	| The dragorns will cast spell 5693 Flashfire 5000 AE DD
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) {
		| casters and healers need to move to SafeCenterCaster
		/call ForesightNavigation SafeCenterCaster IsMovingAnywhere ForesightAECloseSafe
	} else {
		| Melee need to move to SafeCenterDPS
		/call ForesightNavigation SafeCenterDPS IsMovingAnywhere ForesightAECloseSafe
	}
/return

Sub EVENT_ForesightAECloseSafe
	/varset IsMovingAnywhere FALSE
/return



Sub EVENT_ForesightPlatform
	/say Need to get on the platform!
	/doevents flush ForesightPlatform

	| The hazard will cast spell: 5697 Scalding Steam 5500 DD AE & 950/tick DOT
	
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) {
		| casters and healers can stay on the platform where they are
		/call ForesightNavigation SafeCenterCaster IsMovingAnywhere ForesightPlatformSafe
	} else {
		| Melee need to move to the closest platform
		/call ForesightNavigation SafeCenterDPS IsMovingAnywhere ForesightPlatformSafe
	}
/return

Sub EVENT_ForesightPlatformSafe
	/varset IsMovingAnywhere FALSE
/return 

Sub EVENT_ForesightDS
	/rs !!Evading DamageShield!!
	/doevents flush ForesightDS

	| The dragorns will cast spell: 5698 Lacerating Aura 350pt DS
	/if (${str_classArchetype.Equal[PRIEST]} || ${str_classArchetype.Equal[CASTER]}) {
		| casters and healers can continue whatever they are doing
		/return
	} else {
		/declare atk bool local FALSE
		/declare tid int local ${Target.ID}
		/declare wait int local 1
		| Melee need to stop attacking
		|- turn off melee & movement
		/if (${Melee.Attack})   /varset atk TRUE
		/if (${Navigation.Active}) /nav stop
		/if (${Melee.Sticking})    /stick off
		
		|- movement & event waiting loop
		:DSDelay
		/varcalc wait ${wait}+1
		/if (${wait} < 11) {
			/echo Waiting for DS to fade (no emote)
			/delay 3
			/goto :DSDelay
		}
	}
	/echo Safe event fired, returning to origin
	/varset ${_watch} FALSE
	/call TrueTarget ${tid}
	:returnDelay
	/if (${Target.Distance} >= 40) {
		/delay 10
		/goto :returnDelay
	}
	/nav stop
	/if (${atk}) /killthis
/return

Sub EVENT_ForesightWeapon
	/say Removing Weapon
	/doevents flush ForesightWeapon
	/delay 10
	| The Kyv will cast spell 2315 Searing Heat 2000 DD
	| save the weapon and slot information (well, its allways primary weapon) and unequip
	/if (${Me.Inventory[mainhand].Name.Equal[NULL]}) /return
	/cecho \ayRemoving \ar${MainHandWeapon} \ayfrom mainhand
	/unequip mainhand
	|- set the watch variable to TRUE
	/varset HasRemovedWeapon TRUE
/return

Sub EVENT_ForesightWeaponSafe
	/delay 10
	/cecho \agSafe event fired, returning \ar${MainHandWeapon} \agto mainhand
	/exchange "${MainHandWeapon}" mainhand
	/varset HasRemovedWeapon	FALSE
	/doevents flush ForesightWeaponSafe
/return


Sub EVENT_ForesightRing
	/say Removing Rings
	/doevents flush ForesightRing
	| The Hazard will cast spell 5695 Crippling Clamp, 1200 DD, 25% slow, 60 disease counters
	| save the ring information and unequip
	/call RemoveRingset
	|- set the watch variable to TRUE
	/varset HasRemovedRings TRUE
/return

Sub EVENT_ForesightRingSafe
	/call LoadRingset
	/varset HasRemovedRings		FALSE
	/doevents flush ForesightRingSafe
/return


Sub EVENT_ForesightKyvSafe
	/cecho \agForesightKyvSafe \ay: IsHoldingStill:${IsHoldingStill} IsDucking:${IsDucking} IsMovingAnywhere:${IsMovingAnywhere}
	/varset IsHoldingStill 		FALSE
	/varset IsDucking			FALSE
	/varset IsMovingAnywhere	FALSE
	/cecho \ag: IsHoldingStill:${IsHoldingStill} IsDucking:${IsDucking} IsMovingAnywhere:${IsMovingAnywhere}
	/doevents flush ForesightKyvSafe
/return


Sub EVENT_ForesightReflect
	| The dragorns will cast spell: 5699
	/doevents flush ForesightReflect
	/if (${str_classArchetype.Equal[CASTER]}) {
		/declare wait int local 0
		/say stopping for Reflection
		| casters need to stop casting on target
		/declare tid int local ${Target.ID}
		
		:ReflectDelay
		/varcalc wait ${wait}+1
		/doevents flush ForesightReflect
		/if (${wait} < 11) {
			/echo Waiting for DS to fade (no emote)
			/delay 10
			/goto :ReflectDelay
		} else {
			/goto :return
		}
		:return
	} 
/return

Sub EVENT_ForesightAEMaxDPS
	/doevents flush ForesightAEMaxDPS
	| do we want to do anything on this? likely not.
/return



Sub EVENT_D1
	/doevents flush D1
	/if (${Navigation.Active}) /nav stop
	/nav wp D1
	:movement
		/delay 20
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Flamecaster"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Flamecaster"].ID}
			/if (${Target.Distance} < 50) /nav stop
		} 

/return

Sub EVENT_D2
	/doevents flush D2
	/if (${Navigation.Active}) /nav stop
	/nav wp D2
	:movement
		/delay 20
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Spellscribe"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Spellscribe"].ID}
			/if (${Target.Distance} < 50) /nav stop
		} 
/return

Sub EVENT_D3
	/doevents flush D3
	/if (${Navigation.Active}) /nav stop
	/nav wp D3
	:movement
		/delay 20
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Adept"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Adept"].ID}
			/if (${Target.Distance} < 50) /nav stop
		} 
/return

Sub EVENT_D4
	/doevents flush D4
	/if (${Navigation.Active}) /nav stop
	/nav wp D4
	:movement
		/delay 20
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Incinerator"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Incinerator"].ID}
			/if (${Target.Distance} < 50) /nav stop
		} 
/return

Sub EVENT_D5
	/doevents flush D5
	/if (${Navigation.Active}) /nav stop
	/nav wp D5
	:movement
		/delay 20
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Acolyte"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Acolyte"].ID}
			/if (${Target.Distance} < 50) /nav stop
		} 
/return

Sub EVENT_D6
	/doevents flush D6
	/if (${Navigation.Active}) /nav stop
	/nav wp D6
	:movement
		/delay 20
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Annihilator"].ID}
			/if (${Target.Distance} < 50) /nav stop
		} 
/return