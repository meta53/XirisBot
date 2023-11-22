
|**
----------------------------
xiris_class_shm.inc
----------------------------
Allows for Shaman specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Shaman Class Events
______________________________________________
**|	


Sub initEvents_Class_SHM
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_SHM
/return

Sub xbot_initialize_class_SHM
    /call initEvents_Class_SHM
/return
