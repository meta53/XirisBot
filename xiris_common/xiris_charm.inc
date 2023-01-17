|**
----------------------------
xiris_charm.inc
----------------------------
This includes the charm setup (see xiris_charm.ini) which has mobs to force 
charm on check (ie: Specialization mote of frost, OMM lieutenants, etc)
______________________________________________
REVISION HISTORY
    06.24.22    xiris	INITIAL REVISION
    06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
**|



#EVENT  CharmON         "#*#CharmON#*#"
#EVENT  CharmOFF        "#*#CharmOFF#*#"
#EVENT  CharmNPCByID    "[ #1# #*#]#*#CharmNPCByID #2#"
#EVENT  CharmNPCByName  "[ #1# #*#]#*#CharmNPCByName #2#"

Sub InitEvents_Charm
    /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Me.Class.ShortName.NotEqual[ENC]} && ${Me.Class.ShortName.NotEqual[NEC]}) /return
    /call RegisterBackgroundEvents Events_Charm 4
/return 

Sub Events_Charm 
    /doevents CharmON 
    /doevents CharmOFF
    /doevents CharmNPCByID
    /doevents CharmNPCByName
/return

Sub EVENT_CharmON 
    /varset bln_activeCharming TRUE
/return 

Sub EVENT_CharmOFF 
    /varset bln_activeCharming FALSE
/return 

Sub EVENT_CharmNPCByID(_line, _chatSender, _eventParams)
    /echo \ayCharming NPC by ID \ag${_eventParams}
    /call CharmNPC ${_eventParams}
/return

Sub EVENT_CharmNPCByName(_line, _chatSender,_eventParams)
    /echo \ayCharming NPC by Name \ag${_eventParams}
/return 

Sub CharmNPC(int _id)
    /if (!${bln_activeCharming}) /return
    /if (${Me.Pet.ID}) {
        /echo \arCannot Charm ${_id}: Already have pet!
        /goto :Engage
    }
    /declare castAttempt int local 1
    /call chkSpellMemorized "${str_charm}" TRUE ${int_charmGem}
    /echo calling TrueTarget ${_id}
    /call TrueTarget ${_id}
    :castAttempt
    /varcalc castAttempt ${castAttempt}+1
    /call MQ2Cast "${str_charm}" ${int_charmGem}
    /if (${Macro.Return.Equal[CAST_NOCHARM]}) {
        /rsay ${Target.Name} CANNOT be charmed!
        /gsay ${Target.Name} CANNOT be charmed!
        /goto :return
    } else /if (${Macro.Return.Equal[CAST_RESIST]} && ${castAttempt} < 4) {
        /goto :castAttempt
    } else /if (${Macro.Return.Equal[CAST_SUCCESS]} && ${Me.Pet.ID}) {
        :Engage
        /delay 10
        /echo \awEngaging Target(\ag${int_targetID}\aw) with Charmed Pet \ag${Me.Pet.Name}! 
        /call EngageTarget ${int_targetID}
    }
    :return
/return

Sub xbot_initialize_charming
    /echo \ao xbot_initialize_charming
    /call InitEvents_Charm

    |-- Charm Spell(s)
    /declare bln_doCharm            bool    outer   ${Ini[INI_CHAR,Charm,bln_doCharm,FALSE]}
    /declare bln_activeCharming     bool    outer   FALSE

    /declare str_charm              string  outer   ${Ini[INI_CHAR,Charm,Charm_Spell,NULL]}
    /declare int_charmGem           int     outer   ${Ini[INI_CHAR,Charm,Charm_Gem,7]}    
    /declare int_charmMinLevel      int     outer   ${Ini[INI_CHAR,Charm,Charm_MinLevel,46]}
    /declare int_charmMaxLevel      int     outer   ${Ini[INI_CHAR,Charm,Charm_MaxLevel,69]}
/return 