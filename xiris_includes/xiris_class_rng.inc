
|**
----------------------------
xiris_class_rng.inc
----------------------------
Allows for Ranger specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Ranger Class Events
______________________________________________
**|	


Sub initEvents_Class_RNG
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_RNG
/return

Sub xbot_initialize_class_RNG
    /call initEvents_Class_RNG
/return
