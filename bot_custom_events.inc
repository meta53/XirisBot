|
| bot_custom_events.inc
|
| Add your custom code here for event responses.
|
| Sub SetupCustomEvents MUST EXIST


|------------------
| SetupCustomEvents
| This sub will be called once during program startup.  Use it to setup timers and such.
|------------------
Sub SetupCustomEvents
    | hateplaneb
    /if (${Zone.ID}==76) /declare InnyDoomTimer timer global 5s
/return

|------------
| Event_Timer
| Built in function that will trigger any time a timer expires.
| Use this to call special handling routines as needed based on what timer expired.
|------------
Sub Event_Timer(Timer,OriginalValue)
    /if (${String[${Timer}].Equal[InnyDoomTimer]}) /call InnyDoomTimer ${OriginalValue}
/return

|
|### hateplaneb Inny Doom
|

#event InnyDoom "#*#You feel a sense of doom upon you.#*#"

Sub Event_InnyDoom
    /if (${str_announce_channel.NotEqual[NULL]}) /docommand /${str_announce_channel} Inny wants to DT me!
    /while ( ${Me.Song[Doom].ID} && ${Me.State.NotEqual[HOVER]} ) {
        /bc -- Need Cure for Doom --
        /delay 2s ${Spawn[Cleansing Aura].ID}
        /doevents
        /moveto id ${Spawn[Cleansing Aura].ID} loose
	    /delay 2s ${Math.Distance[${Me.Y},${Me.X}:${YLOC},${XLOC}]}<10
	}
/return

Sub InnyDoomTimer(OriginalValue)
    /if (${Zone.ID}==76) {
        /call InnyDoomAura
    }
    /varset InnyDoomTimer ${OriginalValue}
/return

Sub InnyDoomAura
    | Calls the Aura location for the raid, fixed approximate X locations are:
    |   X=104 == door
    |   X=125 == Corner
    |   X=-4 == Spawn
    
	/if (${Zone.ID}==76 && ${Spawn[Cleansing Aura].ID}) {
		/declare X int local
		/varset X ${Spawn[Cleansing Aura].X}
		/if (${X} < 0) {
			/rsay Aura at SPAWN
		} else /if ( ${X} < 112 ) {
			/rsay Aura at DOOR
		} else {
			/rsay Aura at CORNER
		}
	} else {
        /bc Can't find cleansing aura!
    }
/return

