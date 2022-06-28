|**
----------------------------
CHARM CASTING HANDLERS
----------------------------
This includes the charm setup (see xiris_charm.ini) which has mobs to force 
charm on check (ie: Specialization mote of frost, OMM lieutenants, etc)
______________________________________________
REVISION HISTORY
    06.24.22    xiris	INITIAL REVISION
**|



#EVENT  CharmON         "#*#CharmON#*#"
#EVENT  CharmOFF        "#*#CharmOFF#*#"
#EVENT  CharmNPCByID    "#*#CharmNPCID #1#"
#EVENT  CharmNPCByName  "#*#CharmNPCName #1#"

Sub InitEvents_Charm
    /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Me.Class.ShortName.NotEqual[ENC]}) /return
    /call RegisterBackgroundEvents Events_Charm 4
/return 

Sub Events_Charm 
    /doevents CharmON 
    /doevents CharmOFF
    /doevents CharmNPCByID
    /doevents CharmNPCByName
/return

Sub EVENT_CharmON 

/return 

Sub EVENT_CharmOFF 

/return 

Sub EVENT_CharmNPCByID(_line, _eventParams)
    /echo \ayCharming NPC by ID \ag${_eventParams}
/return

Sub EVENT_CharmNPCByName(_line, _eventParams)
    /echo \ayCharming NPC by Name \ag${_eventParams}
/return 

Sub CharmNPC(int _id)
    /if (${Me.Pet.ID}) {
        /cecho \arCannot Charm ${_id}: Already have pet!
        /return
    }
    /call TrueTarget ${_id}

/return