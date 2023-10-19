
|**
----------------------------
xiris_class_ber.inc
----------------------------
Allows for Berserker specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Berserker Class Events
______________________________________________
**|	


Sub initEvents_Class_BER
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_BER
/return

Sub xbot_initialize_class_BER
    /call initEvents_Class_BER
/return
