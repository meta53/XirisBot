
|**
----------------------------
xiris_class_rog.inc
----------------------------
Allows for Rogue specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Rogue Class Events
______________________________________________
**|	


Sub initEvents_Class_ROG
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_ROG
/return

Sub xbot_initialize_class_ROG
    /call initEvents_Class_ROG
/return