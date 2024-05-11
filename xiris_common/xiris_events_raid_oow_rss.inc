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



Sub chkKingEvents
    /doevents KingSetup
    /doevents KingStart 
    /doevents KingVictim 
    /doevents KingSafe
/return 