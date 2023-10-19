
|**
----------------------------
xiris_class_dru.inc
----------------------------
Allows for Druid specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Druid Class Events
______________________________________________
**|	


Sub initEvents_Class_DRU
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_DRU
/return

Sub xbot_initialize_class_DRU
/return
