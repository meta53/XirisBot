|**
----------------------------
EVENTS_OOW_MPG_CORRUPTION.INC
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

|-- Citadel of Anguish
|   Ture
#EVENT TureRampOn       "#*#Ture roars with fury#*#"
#EVENT TureRampOff      "#*#Ture calms and regains#*#"

|   OMM
#Event GazeYou			"#*#gaze of deadly power focusing on you#*#" 
#Event GazeBurst		"#*#Mata Muram fixes his gaze#*#"
#Event GazeDone			"#*#Mata Muram's spell has been reflected#*#"

Sub InitEvents_OOW_MPG_Corruption
	/cecho \ag Initializing MPG Corruption
/return

Sub Events_OOW_MPG_Corruption
	|-- Killing Events

/return