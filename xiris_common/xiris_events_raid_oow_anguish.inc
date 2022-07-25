|**
----------------------------
EVENTS_OOW_ANGUISH.INC
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	07.20.22    xiris	INITIAL REVISION
______________________________________________
**|	

|-- Citadel of Anguish
|   Ture
#EVENT TureRampOn       "#*#roars with fury#*#"
#EVENT TureRampOff      "#*#calms and regains#*#"
#EVENT TureWanton		"#*#dissolves into a landscape#*#"

|	Warden Hanvar
#EVENT HanvarStart		"#*#hanvarstart#*#"

|	Arch Magus Vangl
#EVENT MarkedForDeath	"#*#You have been marked for death#*#"

|   OMM
#Event GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#Event GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#Event GazeDone			"#*#Mata Muram's spell has been reflected#*#"




Sub InitEvents_OOW_Anguish
	/cecho \ag Initializing Anguish
	/call RegisterRaidEvents Events_OOW_Anguish 5
/return

Sub Events_OOW_Anguish 
	/doevents TureRampOn
	/doevents TureRampOff
	/doevents HanvarStart
	/doevents MarkedForDeath
	/doevents GazeYou
	/doevents GazeBurst
	/doevents GazeDone
/return 


Sub Event_TureRampOn
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/squelch /melee reset
		/squelch /melee reset
		/squelch /attack off
		/stick off
		/nav wp tureSafe
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]}) {
		/pet hold on
		/pet back off
	}
/return 


Sub Event_TureRampOff 
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/target npc Ture
		/squelch /killthis
		/nav wp tureDPS
		:ture_run 
		/if (${Target.Distance} < 40) /nav stop
		/if (${Navigation.Active}) /goto :ture_run
	} else /if (${Me.Class.ShortName.Equal[NEC]} || ${Me.Class.ShortName.Equal[MAG]}) {
		/pet attack
	}
/return 

Sub Event_TureWanton 

/return

Sub Event_MarkedForDeath
	/if (${Me.Class.ShortName.Equal[SHM]}) /call cureSelf disease 14
	/if (${Me.Class.ShortName.Equal[CLR]}) /call cureSelf disease 14
	/if (${Me.Class.ShortName.Equal[DRU]}) /call cureSelf disease 14
	/if (${Me.Class.ShortName.Equal[PAL]}) /call cureSelf disease 14
/return 


Sub Event_HanvarStart
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call TrueTarget ${Spawn[npc hanvar]}
		/nav target
	}
/return 

Sub Event_GazeYou 
/return 

Sub Event_GazeBurst
/return 

Sub Event_GazeDone
/return