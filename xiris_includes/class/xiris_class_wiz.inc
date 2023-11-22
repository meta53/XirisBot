
|**
----------------------------
xiris_class_wiz.inc
----------------------------
Allows for Wizard specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Wizard Class Events
______________________________________________
**|	
#EVENT MANABURN	"#*#manaburnON"
#EVENT FIRETL 	"#*#firetl#*#"

Sub initEvents_Class_WIZ
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_WIZ
	/doevents MANABURN
	/doevents FIRETL
/return

Sub xbot_initialize_class_WIZ
    /call initEvents_Class_WIZ
/return

Sub EVENT_MANABURN
	/echo MBURN CALLED
	/call MQ2Cast "Mana Burn" alt
	/doevents flush
/return

SUB EVENT_FIRETL
	/doevents flush
	/dgt Firing TL ${Time.Time24}
	/call MQ2Cast "${str_tlSpell}" ${str_tlGem}
	/delay 1m
	/memspellset ${str_casting_spellSet}
/return


