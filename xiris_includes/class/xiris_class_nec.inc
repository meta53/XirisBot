
|**
----------------------------
xiris_class_enc.inc
----------------------------
Allows for Necromancer specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Necromancer Class Events
______________________________________________
**|	

Sub initEvents_Class_NEC
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_NEC
/return

Sub xbot_initialize_class_NEC
    /call initEvents_Class_NEC
/return
