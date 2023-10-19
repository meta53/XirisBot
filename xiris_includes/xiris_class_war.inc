
|**
----------------------------
xiris_class_war.inc
----------------------------
Allows for Warrior specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Warrior Class Events
______________________________________________
**|	


Sub initEvents_Class_WAR
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_WAR
/return

Sub xbot_initialize_class_WAR
    /call initEvents_Class_WAR
/return
