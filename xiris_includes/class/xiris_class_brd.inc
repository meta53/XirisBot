
|**
----------------------------
xiris_class_brd.inc
----------------------------
Allows for Bard specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Bard Class Events
______________________________________________
**|	
#EVENT SING				"#1# tells the group#*#'SingSongs'"
#EVENT SING				"#1# tells you#*#'SingSongs'"
#EVENT SING				"#1# tells the raid#*#'SingSongs'"
#EVENT SING 			"[ #1# #*#]#*#SingSongs"

#EVENT HUSH				"#1# tells the group#*#'HUSH#*#'"
#EVENT HUSH				"#1# tells you#*#'HUSH#*#'"
#EVENT HUSH				"#1# tells the raid#*#'HUSH#*#'"
#EVENT HUSH 			"[ #1# #*#]#*#HUSH#*#"


Sub initEvents_Class_BRD
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_BRD
    /doevents SING 
    /doevents HUSH
/return

Sub xbot_initialize_class_BRD

    /declare str_currentMedley string outer MELEE
    /call SetupSongs
    /call SingSongs ${str_currentMedley}
    /call initEvents_Class_BRD
/return

Sub EVENT_Hush(string _line, string _sender, string _eventParams)
    /doevents flush HUSH
	/medley stop
	
/return

Sub EVENT_Sing(string _line, string _sender, string _eventParams)	
    /doevents flush SING
	/call SingSongs ${str_currentMedley}
	
/return

Sub SingSongs(string _type)
    /echo \awSinging \ag${_type} \awmemming set
    /medley stop
    /varset str_currentMedley ${_type}
	/call SetSpellSet ${_type}
	/delay 30
	/medley ${_type}
/return



Sub SetupSongs
	| Set Bard songs based on raid type and group leader
	| Note: TODO: Set this as an INI driven setup
    /declare i int local 0

    /for i 0 to ${Group.GroupSize}
        | Plate classes get a TANK singing bard
        /if (${Select[${Group.Member[${i}].Class.ShortName},WAR,PAL,SHD]}) {
            /varset str_currentMedley TANK
            /return
        } 
    /next i
    /for i 0 to ${Group.GroupSize}
        | Melee classes get a MELEE singing bard
        /if (${Select[${Group.Member[${i}].Class.ShortName},ROG,BER,MNK]}) {
            /varset str_currentMedley MELEE
            /return
        }
    /next i
    /for i 0 to ${Group.GroupSize}
        | Casters.. I guess.
        /if (${Select[${Group.Member[${i}].Class.ShortName},NEC,WIZ,MAG]}) {
            /varset str_currentMedley CASTER
            /return
        }
    /next i
/return