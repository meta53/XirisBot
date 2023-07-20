|**
----------------------------
xiris_events_raid_mpg_hatred.inc
----------------------------
Event Handlers for the MPG: Trial of Hatred raid.
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

#EVENT D1 					"#*#MD1#*#"
#EVENT D2					"#*#MD2#*#"
#EVENT D3 					"#*#MD3#*#"
#EVENT D4 					"#*#MD4#*#"
#EVENT D5 					"#*#MD5#*#"
#EVENT D6 					"#*#MD6#*#"

Sub initEvents_OOW_MPG_Hatred
	/echo \ag Initializing MPG Hatred
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
/return

Sub EVENT_D1
	/doevents flush D1
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop
	/nav wp D1
	/call MoveToPosition ${D1Y} ${D1X} ${D1Z} 30 TRUE
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}	
/return

Sub EVENT_D2
	/doevents flush D2
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop
	/nav wp D2
	/call MoveToPosition ${D2Y} ${D2X} ${D2Z} 30 TRUE
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}	
/return

Sub EVENT_D3
	/doevents flush D3
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop
	/nav wp D3
	/call MoveToPosition ${D3Y} ${D3X} ${D3Z} 30 TRUE
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}	
/return

Sub EVENT_D4
	/doevents flush D4
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop
	/nav wp D4
	/call MoveToPosition ${D4Y} ${D4X} ${D4Z} 30 TRUE
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}	
/return

Sub EVENT_D5
	/doevents flush D5
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop
	/nav wp D5
	/call MoveToPosition ${D5Y} ${D5X} ${D5Z} 30 TRUE
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}			
/return

Sub EVENT_D6
	/doevents flush D6
	/if (!${Me.Standing}) /stand on
	/if (${Navigation.Active}) /nav stop
	/nav wp D6
	/call MoveToPosition ${D6Y} ${D6X} ${D6Z} 30 TRUE
	/if (${str_classArchetype.NotEqual[PRIEST]}) /call TrueTarget ${NearestSpawn[NPC Voracious].ID}			
/return