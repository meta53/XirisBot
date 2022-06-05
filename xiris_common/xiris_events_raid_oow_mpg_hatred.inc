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
#event SetupHatred			"#*#SETUP#*#"

Sub InitEvents_OOW_MPG_Hatred
	/cecho \ag Initializing MPG Hatred
    /call RegisterRaidEvents Events_OOW_MPG_Hatred 6

	|--Waypoints
	|  Pulled from MQ2Nav		   X	   Y	  Z
	/declare D1	string 	outer  -155.939 152.306 55.822 
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

Sub Events_OOW_MPG_Hatred
	|-- Killing Events
	/doevents D1
	/doevents D2
	/doevents D3
	/doevents D4
	/doevents D5
	/doevents D6
	/doevents SetupHatred
/return


Sub EVENT_SetupHatred 

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
	:final
	/warp loc ${D1Y} ${D1X} ${D1Z}
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}
	
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
	:final
	/warp loc ${D2Y} ${D2X} ${D2Z}
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}
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
	:final	
	/warp loc ${D3Y} ${D3X} ${D3Z}
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}	
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
	
	/warp loc ${D4Y} ${D4X} ${D4Z}
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}	
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
		/if (${Navigation.Active})  /goto :movement
	:final
	/warp loc ${D5Y} ${D5X} ${D5Z}
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}			
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
	:final
	/warp loc ${D6Y} ${D6X} ${D6Z}
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}			
/return