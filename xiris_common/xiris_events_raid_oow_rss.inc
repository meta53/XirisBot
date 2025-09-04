|**
----------------------------
xiris_events_raid_oow_anguish.inc
----------------------------
Event Handlers for the OOW RSS raid(s).
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	04.01.24    xiris	INITIAL REVISION
______________________________________________
**|	

| King Gelaqua
#EVENT KingSetup    "#*#KingSetup#*#"
#EVENT KingStart    "#*#KingStart#*#"
#EVENT KingVictim   "#1##*#KingVictim|#2#|#*#"
#EVENT KingSafe     "#*#KingSafe#*#"

Sub InitEvents_OOW_RSS 
	/echo \ag Initializing Riftseeker's Sanctum
    /declare    bln_kingEngaged     bool    outer   FALSE
    /declare    bln_queenEngaged    bool    outer   FALSE 

    | King
    /declare    bln_kingRunning     bool    outer   FALSE 
    /declare    bln_kingSafe        bool    outer   FALSE
    /declare    timer_bombCheck     timer   outer   0

    | Queen 
    /declare    bln_queenRunning    bool    outer   FALSE 
    /declare    bln_queenSafe       bool    outer   FALSE

    /call RegisterRaidEvents    EVENTS_OOW_Riftseekers 1

/return


Sub EVENTS_OOW_Riftseekers 
    /if (${Zone.ShortName.NotEqual[riftseekers]} && ${Zone.ShortName.NotEqual[arena]} && ${Zone.ShortName.NotEqual[guildhall]}) /return
    /if (${Select[${Me.Class.ShortName},WAR,SHD,PAL]} && ${bln_engaged}) /call chkDivineIntervention

    | King Gelaqua Evetns
    /if (${SpawnCount[npc King]} && ${Spawn[npc King].Distance3D} < 400) {
        /call chkKingEvents
    }

/return 


Sub EVENT_KingSetup(_line, _chatSender, _eventParams)
/return 

Sub EVENT_KingStart(_line, _chatSender, _eventParams)
/return 

Sub EVENT_KingVictim(_line, _chatSender, _eventParams) 
    /call KingVictim ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
/return 

Sub EVENT_KingSafe(_line, _chatSender, _eventParams)
/return

Sub chkKingEvents
    /doevents KingSetup
    /doevents KingStart 
    /if (!${timer_bombCheck} && !${bln_tanking}) /call KingBombCheck
    /doevents KingSafe
/return 

Sub KingSetup
/return 

Sub KingStart
    /varset bln_kingEngaged TRUE
/return

Sub KingBombCheck
    /declare sTID   int local ${Target.ID}
    /declare i      int local 0
    | Loop through all the spawns of icy orbs and see if they are targetting me
    /for i 0 to ${SpawnCount[npc "icy orb"]}
       /target ID ${NearestSpawn[${i}, npc "icy orb"].ID}
       /if (${Me.TargetOfTarget.ID} == ${Me.ID} && !${bln_kingRunning}) {
            /varset bln_kingRunning TRUE 
            /goto :kingRunning
       }    
    /next i
    /varset bln_kingRunning FALSE

    :kingRunning
    /if (${bln_kingRunning}) /call KingRunning ${Me.TargetOfTarget.ID}
    /target id ${sTID}
/return 



Sub KingRunning(int _bombID)
    /nav recordwaypoint bombStart
    :bombCheck
    /if (!${SpawnCount[id ${_bombID}]})         /goto :bombReset 
    /if (${Target.ID} !=${_bombID})             /target id ${_bombID}
    /if (${Me.TargetOfTarget.ID} != ${Me.ID})   /goto :bombReset

    /nav wp br1
    :br1 
        /if (!${bln_running}) /return
        /if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call KingDeath
        /if (${Me.TargetOfTarget.ID} != ${Me.ID}) /goto :bombReset
        /if (${Navigation.Active}) /goto :br1 

    /nav wp br2
    :br2
        /if (!${bln_running}) /return
        /if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call KingDeath
        /if (${Me.TargetOfTarget.ID} != ${Me.ID}) /goto :bombReset
        /if (${Navigation.Active}) /goto :br2 

    /nav wp br3
    :br3
        /if (!${bln_running}) /return
        /if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call KingDeath
        /if (${Me.TargetOfTarget.ID} != ${Me.ID}) /goto :bombReset
        /if (${Navigation.Active}) /goto :br3

    /nav wp br4
    :br4
        /if (!${bln_running}) /return
        /if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /call KingDeath
        /if (${Me.TargetOfTarget.ID} != ${Me.ID}) /goto :bombReset
        /if (${Navigation.Active}) /goto :br4

    /nav wp br1
    /goto :br1
    
    :bombReset 
    /if (${Navigation.Active}) /nav stop
    /nav wp bombStart
    /call KingSafe
/return 

Sub KingSafe
    /varset bln_kingRunning FALSE
    /if (${Select[${Me.Class.ShortName},${MeleeClasses}]} && ${bln_engaged}) {
        /call EngageTarget ${int_targetID}
    }
/return


Sub KingDeath 
    :kingDead
    /call RunAllEvents
    /if (${Zone.ShortName.Equal[${Me.BoundLocation[0]}]}) /goto :kingDead
/return




| Unsure if this will be needed
Sub KingVictim(int _victimID, int _bombID)
    /if (${bln_kingRunning}) /return
    /declare i int local 0
    /for i 0 to ${SpawnCount[npc "icy orb"]}
        /if (${NearestSpawn[${i}, npc "icy orb"].ID})

    /next i
/return 
