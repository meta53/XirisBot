
|**
----------------------------
xiris_class_shd.inc
----------------------------
Allows for ShadowKnight specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added ShadowKnight Class Events
______________________________________________
**|	


Sub initEvents_Class_SHD
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_SHD
/return

Sub xbot_initialize_class_SHD
    /call initEvents_Class_SHD
/return
