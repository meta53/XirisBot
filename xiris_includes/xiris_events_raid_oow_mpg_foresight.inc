|**
----------------------------
xiris_events_raid_mpg_foresight.inc
----------------------------
Event Handlers for the MPG: Trial of Foresight raid.
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	04.03.22    xiris	INITIAL REVISION
	04.25.22	xiris	Updated D1-D6 movement to make sure toons stand and fixed distance checking
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Foresight
#EVENT ForesightNorth		"#*#dramatically. The north side looks safe#*#"
#EVENT ForesightSouth		"#*#dramatically. The south side looks safe#*#"
#EVENT ForesightEast		"#*#dramatically. The east side looks safe.#*#"
#EVENT ForesightWest		"#*#dramatically. The west side looks safe.#*#"
#EVENT ForesightCenter		"#*#dramatically."


#EVENT ForesightAESafe		"#*#You escape the blast unscathed#*#"
#EVENT ForesightAESafe		"#*#The air around you ignites#*#"


#EVENT ForesightReflect		"#*#developing an anti-magic aura#*#"


#EVENT ForesightPlatform	"#*#You should find higher ground#*#"
#EVENT ForesightPlatformSafe  "#*#You escape the blast unscathed#*#"
#EVENT ForesightPlatformSafe "#*#The air around you ignites#*#"

#EVENT ForesightAOEDPS		"#*#Doing enough damage to him might interrupt the process#*#"
#EVENT ForesightDS			"#*#before you is sprouting shard spikes#*#"
#EVENT ForesightDS			"#*#you are pierced by thorns"

#EVENT ForesightKyvMove		"#*#You should move#*#"
#EVENT ForesightKyvMoveSafe "#*#You are struck by a stray arrow#*#"
#EVENT ForesightKyvMoveSafe "#*#An arrow narrowly misses you#*#"

#EVENT ForesightKyvStill	"#*#You should stand still#*#"
#EVENT ForesightKyvStillSafe "#*#You are struck by a stray arrow#*#"
#EVENT ForesightKyvStillSafe "#*#An arrow narrowly misses you#*#"

#EVENT ForesightKyvDuck		"#*#You should duck#*#"
#EVENT ForesightKyvDuckSafe "#*#Your head is struck#*#"
#EVENT ForesightKyvDuckSafe "#*#An arrow narrowly misses you#*#"

#EVENT ForesightWeapon		"#*#you should remove it#*#"
#EVENT ForesightWeaponSafe  "#*#Your weaponry cools down#*#"
#EVENT ForesightWeaponSafe  "#*#searing your hands#*#"

#EVENT ForesightRing		"#*#bite into your fingers#*#"
#EVENT ForesightRingSafe	"#*#You regain the use of your fingers#*#"
#EVENT ForesightRingSafe	"#*#Your rings clamp down#*#"

#EVENT ForesightAEClose		"#*#before you is preparing to cast a devastating close-range spell#*#"
#EVENT ForesightAECloseSafe	"#*#You escape the blast unscathed#*#"
#EVENT ForesightAECloseSafe	"#*#The air around you ignites#*#"

#EVENT D1 					"#*#MD1#*#"
#EVENT D2					"#*#MD2#*#"
#EVENT D3 					"#*#MD3#*#"
#EVENT D4 					"#*#MD4#*#"
#EVENT D5 					"#*#MD5#*#"
#EVENT D6 					"#*#MD6#*#"

Sub initEvents_OOW_MPG_Foresight
	/echo \ag Initializing MPG Foresight
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


	|--Waypoints
	|  Pulled from MQ2Nav		   X	   Y	  Z
	/declare D1	string 	outer   155.939 152.306 55.822 
	/declare D2	string 	outer  -255.035 149.536 55.822 
	/declare D3	string 	outer  -333.305 219.533 55.822 
	/declare D4	string 	outer  -329.153 323.222 55.822 
	/declare D5	string 	outer  -256.868 400.682 55.822 
	/declare D6	string 	outer  -156.151 399.493 55.822 

	/declare D1X int outer ${D1.Arg[1, ]}
	/declare D1Y int outer ${D1.Arg[2, ]}
	/declare D1Z int outer ${D1.Arg[3, ]}

	/declare D2X int outer ${D2.Arg[1, ]}
	/declare D2Y int outer ${D2.Arg[2, ]}
	/declare D2Z int outer ${D2.Arg[3, ]}
	
	/declare D3X int outer ${D3.Arg[1, ]}
	/declare D3Y int outer ${D3.Arg[2, ]}
	/declare D3Z int outer ${D3.Arg[3, ]}
	
	/declare D4X int outer ${D4.Arg[1, ]}
	/declare D4Y int outer ${D4.Arg[2, ]}
	/declare D4Z int outer ${D4.Arg[3, ]}
	
	/declare D5X int outer ${D5.Arg[1, ]}
	/declare D5Y int outer ${D5.Arg[2, ]}
	/declare D5Z int outer ${D5.Arg[3, ]}
	
	/declare D6X int outer ${D6.Arg[1, ]}
	/declare D6Y int outer ${D6.Arg[2, ]}
	/declare D6Z int outer ${D6.Arg[3, ]}
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

Sub ForesightNavigation(string _wp, string _watch, string _event)
	/say Navigating to ${_wp}
	/declare atk bool local FALSE
	/declare tid int local ${Target.ID}
	/echo \agForesightNavigation: SafeWP: ${_wp}, VariableWatch: ${_watch}, SafeEvent: ${_event}
	
	|- record the origin waypoint
	/nav recordwaypoint origin

	|- turn off melee & movement
	
	/if (${Me.Combat})   /varset atk TRUE

	/call Disengage
	/stick off

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
	/if (${atk}) /attack on
	
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
	/if (${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) {
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
	/if (${Me.Combat})   	/varset atk TRUE
	/call Disengage
	/if (${Navigation.Active}) 	/nav stop
	/stick off


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
	/if (${atk}) /call EngageTarget ${tid}
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
	/if (${Me.Combat})   	/varset atk TRUE
	/call Disengage
	/if (${Navigation.Active}) 	/nav stop
	/stick off
	/if (${Me.Sitting}) 	 	/stand on

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
	/if (${atk})  /call EngageTarget ${tid}
/return

Sub EVENT_ForesightKyvDuckSafe
	/varset IsDucking FALSE
	/if (${Me.Ducking}) /keypress duck
	/if (${Me.State.NotEqual[STAND]}) /stand
/return


Sub EVENT_ForesightAEClose
	/say AEClose to Dragorn!
	/doevents flush ForesightAEClose

	| The dragorns will cast spell 5693 Flashfire 5000 AE DD
	/if (${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) {
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
	
	/if (${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) {
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
	/docommand /${str_announce_channel} !!Evading DamageShield!!
	/doevents flush ForesightDS

	| The dragorns will cast spell: 5698 Lacerating Aura 350pt DS
	/if (${str_class_type.Equal[PRIEST]} || ${str_class_type.Equal[CASTER]}) {
		| casters and healers can continue whatever they are doing
		/return
	} else {
		/declare atk bool local FALSE
		/declare tid int local ${Target.ID}
		/declare wait int local 1
		| Melee need to stop attacking
		|- turn off melee & movement
		/if (${Me.Combat})   /varset atk TRUE
		/if (${Navigation.Active}) /nav stop
		/stick off
		
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
	/if (${atk}) /attack on
/return

Sub EVENT_ForesightWeapon
	/say Removing Weapon
	/doevents flush ForesightWeapon
	/delay 10
	| The Kyv will cast spell 2315 Searing Heat 2000 DD
	| save the weapon and slot information (well, its allways primary weapon) and unequip
	/if (${Me.Inventory[mainhand].Name.Equal[NULL]}) /return
	/echo \ayRemoving \ar${MainHandWeapon} \ayfrom mainhand
	/unequip mainhand
	|- set the watch variable to TRUE
	/varset HasRemovedWeapon TRUE
/return

Sub EVENT_ForesightWeaponSafe
	/delay 10
	/echo \agSafe event fired, returning \ar${MainHandWeapon} \agto mainhand
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
	/echo \agForesightKyvSafe \ay: IsHoldingStill:${IsHoldingStill} IsDucking:${IsDucking} IsMovingAnywhere:${IsMovingAnywhere}
	/varset IsHoldingStill 		FALSE
	/varset IsDucking			FALSE
	/varset IsMovingAnywhere	FALSE
	/echo \ag: IsHoldingStill:${IsHoldingStill} IsDucking:${IsDucking} IsMovingAnywhere:${IsMovingAnywhere}
	/doevents flush ForesightKyvSafe
/return


Sub EVENT_ForesightReflect
	| The dragorns will cast spell: 5699
	/doevents flush ForesightReflect
	/if (${str_class_type.Equal[CASTER]}) {
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
	| moving to Destination Y,X,Z will movement loop until Distance between me, and destination < 50
	/declare dist float local 100
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${D1Y},${D1X},${D1Z}]}
		/if (${dist} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_class_type.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Flamecaster"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Flamecaster"].ID}
		} 
/return

Sub EVENT_D2
	/doevents flush D2
	/if (${Navigation.Active}) /nav stop
	/nav wp D2
	| moving to Destination Y,X,Z will movement loop until Distance between me, and destination < 50
	/declare dist float local 100
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${D2Y},${D2X},${D2Z}]}
		/if (${dist} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_class_type.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Spellscribe"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Spellscribe"].ID}
		} 
/return

Sub EVENT_D3
	/doevents flush D3
	/if (${Navigation.Active}) /nav stop
	/nav wp D3
	| moving to Destination Y,X,Z will movement loop until Distance between me, and destination < 50
	/declare dist float local 100
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${D3Y},${D3X},${D3Z}]}
		/if (${dist} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_class_type.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Adept"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Adept"].ID}
		} 
/return

Sub EVENT_D4
	/doevents flush D4
	/if (${Navigation.Active}) /nav stop
	/nav wp D4
	| moving to Destination Y,X,Z will movement loop until Distance between me, and destination < 50
	/declare dist float local 100
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${D4Y},${D4X},${D4Z}]}
		/if (${dist} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_class_type.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Incinerator"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Incinerator"].ID}
		} 
/return

Sub EVENT_D5
	/doevents flush D5
	/if (${Navigation.Active}) /nav stop
	/nav wp D5
	| moving to Destination Y,X,Z will movement loop until Distance between me, and destination < 50
	/declare dist float local 100
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${D5Y},${D5X},${D5Z}]}
		/if (${dist} < 50) /nav stop
		/if (${str_class_type.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Acolyte"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Acolyte"].ID}
		} 
/return

Sub EVENT_D6
	/doevents flush D6
	/if (${Navigation.Active}) /nav stop
	/nav wp D6
	| moving to Destination Y,X,Z will movement loop until Distance between me, and destination < 50
	/declare dist float local 100
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${D6Y},${D6X},${D6Z}]}
		/if (${dist} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_class_type.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Annihilator"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Annihilator"].ID}
		} 
/return