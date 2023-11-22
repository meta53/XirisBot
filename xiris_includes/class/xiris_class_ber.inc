
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

#EVENT  WARCRYON_1        "#1# tells the group#*#'CryOn1#*#'"
#EVENT  WARCRYON_1        "#1# tells you#*#'CryOn1#*#'"
#EVENT  WARCRYON_1        "#1# tells the raid#*#CryOn1#*#'"
#EVENT  WARCRYON_1        "[ #1# #*#]#*#CryOn1#*#"

#EVENT  WARCRYON_2        "#1# tells the group#*#'CryOn2#*#'"
#EVENT  WARCRYON_2        "#1# tells you#*#'CryOn2#*#'"
#EVENT  WARCRYON_2        "#1# tells the raid#*#CryOn2#*#'"
#EVENT  WARCRYON_2        "[ #1# #*#]#*#CryOn2#*#"

#EVENT  FIRE_WARCRY        "#1# tells the group#*#'FireWarCry #2#'"
#EVENT  FIRE_WARCRY        "#1# tells you#*#'FireWarCry #2#'"
#EVENT  FIRE_WARCRY        "#1# tells the raid#*#FireWarCry #2#'"
#EVENT  FIRE_WARCRY        "[ #1# #*#]#*#FireWarCry #2#"


Sub initEvents_Class_BER
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_BER
    /doevents FIRE_WARCRY
	/doevents WARCRYON_1
	/doevents WARCRYON_2
/return

Sub xbot_initialize_class_BER
    /call initEvents_Class_BER
/return


Sub EVENT_WARCRYON_1
	/doevents flush WARCRYON_1
	/if (${BURN_Group}==1) {
		/echo \agEnabling \arWarCry \agMode on CoHort \ao1
		/call FireWarCry
	}
/return 

Sub EVENT_WARCRYON_2
	/doevents flush WARCRYON_2
	/if (${BURN_Group}==2) {
		/echo \agEnabling \arWarCry \agMode on CoHort \ao1
		/call FireWarCry
	}
/return 


Sub EVENT_FIRE_WARCRY(_line, _chatSender, _eventParams)
	/shout I am Firing Warcry on ${Spawn[id ${_eventParams}].Name} (${_eventParams});
	/call TrueTarget ${_eventParams}
	/call FireWarCry
/return 


Sub FireWarCry
	/if (${Me.CombatAbilityReady[Ancient: Cry of Chaos]}) {
		/echo \agFIRING WARCRY ${Time.Time24} ${Target.Name}
 		/doability "Ancient: Cry of Chaos"
	}
/return 