
|**
----------------------------
xiris_class_mag.inc
----------------------------
Allows for Magician specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Magician Class Events
______________________________________________
**|	




#EVENT DS 		"<#1#> DSMe#*#"
#EVENT DS 		"[#1#(msg)] DSMe#*#"
#EVENT DS		"#1# tells you,#*#'DSMe#*#'"
#EVENT DS		"#1# tells the group#*#'DSMe#*#'"
#EVENT DS		"#1#tells the raid,#*#'DSMe#*#'"




Sub initEvents_Class_MAG
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_MAG
	/doevents DS
/return

Sub xbot_initialize_class_MAG
    /call initEvents_Class_MAG
/return
