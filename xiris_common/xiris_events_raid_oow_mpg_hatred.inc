|**
----------------------------
EVENTS_OOW_MPG_HATRED.INC
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	04.25.22    xiris	INITIAL REVISION
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Hatred

#event D1 					"#*#MD1#*#"
#event D2					"#*#MD2#*#"
#event D3 					"#*#MD3#*#"
#event D4 					"#*#MD4#*#"
#event D5 					"#*#MD5#*#"
#event D6 					"#*#MD6#*#"

Sub InitEvents_OOW_MPG_Hatred
	/cecho \ag Initializing MPG Hatred
    /call RegisterRaidEvents Events_OOW_MPG_Hatred 6
/return

Sub Events_OOW_MPG_Hatred
	|-- Killing Events
	/doevents D1
	/doevents D2
	/doevents D3
	/doevents D4
	/doevents D5
	/doevents D6
/return

Sub EVENT_D1
	/doevents flush D1
	/if (${Navigation.Active}) /nav stop
	/nav wp D1
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Flamecaster"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Flamecaster"].ID}
		} 
/return

Sub EVENT_D2
	/doevents flush D2
	/if (${Navigation.Active}) /nav stop
	/nav wp D2
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Spellscribe"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Spellscribe"].ID}
		} 
/return

Sub EVENT_D3
	/doevents flush D3
	/if (${Navigation.Active}) /nav stop
	/nav wp D3
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/if (${Target.Distance} < 30) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Adept"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Adept"].ID}
		} 
/return

Sub EVENT_D4
	/doevents flush D4
	/if (${Navigation.Active}) /nav stop
	/nav wp D4
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Incinerator"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Incinerator"].ID}
		} 
/return

Sub EVENT_D5
	/doevents flush D5
	/if (${Navigation.Active}) /nav stop
	/nav wp D5
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Acolyte"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Acolyte"].ID}
		} 
/return

Sub EVENT_D6
	/doevents flush D6
	/if (${Navigation.Active}) /nav stop
	/nav wp D6
	/if (!${Me.Standing}) /stand on
	:movement
		/delay 20
		/if (${Target.Distance} < 50) /nav stop
		/if (${Navigation.Active})  /goto :movement
		/if (${str_classArchetype.NotEqual[PRIEST]}) {
			/call TrueTarget ${Spawn[NPC "Dragorn Annihilator"].ID}
			/call EngageTarget  ${Spawn[NPC "Dragorn Annihilator"].ID}
		} 
/return