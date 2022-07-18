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
#EVENT TureRampOn       "#*#Ture roars with fury#*#"
#EVENT TureRampOff      "#*#Ture calms and regains#*#"

|   OMM
#Event GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#Event GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#Event GazeDone			"#*#Mata Muram's spell has been reflected#*#"

Sub InitEvents_OOW_Anguish
	/cecho \ag Initializing Anguish
/return

Sub InitEvents_OOW_Anguish
	|-- Killing Events
	/call RegisterRaidEvents Events_OOW_Anguish 5
/return

Sub Events_OOW_Anguish 
	/doevents TureRampOn
	/doevents TureRampOff
	/doevents GazeYou
	/doevents GazeBurst
	/doevents GazeDone
/return 