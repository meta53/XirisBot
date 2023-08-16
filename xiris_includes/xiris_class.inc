|**
----------------------------
xiris_class.inc
----------------------------
This includes the small amount of very class specific things (like bards)
______________________________________________
REVISION HISTORY
    06.24.22    xiris	INITIAL REVISION
    06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
    03.08.23	xiris	Removed Trigger parsing, now uses set method chkCharmTrigger which has everything enumerated
						based on a key _TYPE in the ini. This is because parsing the trigger is a PITA, and didn't work well
**|

| BARD EVENTS
#EVENT Sing				"#1# tells the group#*#'SingSongs'"
#EVENT Sing				"#1# tells you#*#'SingSongs'"
#EVENT Sing				"#1# tells the raid#*#'SingSongs'"
#EVENT Sing 			"[ #1# #*#]#*#SingSongs"

#EVENT Hush				"#1# tells the group#*#'Hush#*#'"
#EVENT Hush				"#1# tells you#*#'Hush#*#'"
#EVENT Hush				"#1# tells the raid#*#'Hush#*#'"
#EVENT Hush 			"[ #1# #*#]#*#Hush#*#"

Sub initEvents_Class
    /call RegisterBackgroundEvents Events_Class
/return 

Sub Events_Class
    /if (${Me.Class.ShortName.Equal[BRD]}) {
        /doevents Sing 
        /doevents Hush
    }
    
    /doevents CharmNPCByID
    /doevents CharmNPCByName
/return

Sub xbot_initialize_classes
    /if (${Me.Class.ShortName.Equal[BRD]}) {
        /declare str_currentMedley string outer MELEE
        /call SetupSongs
        
        /call SingSongs ${str_currentMedley}
    }
/return

|--BARD SPECIFIC THINGS
Sub EVENT_Hush(string _line, string _sender, string _eventParams)
    /doevents flush Hush
	/medley stop
	
/return

Sub EVENT_Sing(string _line, string _sender, string _eventParams)	
    /doevents flush Sing
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