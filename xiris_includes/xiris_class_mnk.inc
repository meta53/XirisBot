
|**
----------------------------
xiris_class_mnk.inc
----------------------------
Allows for Monkey specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Monk Class Events
______________________________________________
**|	


Sub initEvents_Class_MNK
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_MNK
/return

Sub xbot_initialize_class_MNK
    /call initEvents_Class_MNK
/return
