
|**
----------------------------
xiris_class_bst.inc
----------------------------
Allows for Beastlord specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Beastlord Class Events
______________________________________________
**|	

#EVENT PARAGON				"#1# tells the group#*#'Paragon'"
#EVENT PARAGON				"#1# tells you#*#'Paragon'"
#EVENT PARAGON				"#1# tells the raid#*#'Paragon'"
#EVENT PARAGON 			"[ #1# #*#]#*#Paragon"


Sub initEvents_Class_BST
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_BST
    /doevents PARAGON 
/return

Sub xbot_initialize_class_BST
    /call initEvents_Class_BST
/return

Sub EVENT_Paragon(string _line, string _sender, string _eventParams)

/return