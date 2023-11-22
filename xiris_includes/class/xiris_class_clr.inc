
|**
----------------------------
xiris_class_clr.inc
----------------------------
Allows for Cleric specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Cleric Class Events
______________________________________________
**|	
#EVENT BattleCleric					"#*#BattleOn#*#"
#EVENT CelestialRegeneration		"#*#celregen#*#"
#EVENT AutoRezON					"#*#AutoRezON#*#"
#EVENT AutoRezOFF					"#*#AutoRezOFF#*#"
#EVENT CombatRezON					"#*#CombatRezON#*#"
#EVENT CombatRezOFF					"#*#CombatRezOFF#*#"

|-- Divine Intervention Events
#EVENT DivineInterventionRequest	"#1# tells you#*#'dime#*#'"
#EVENT DivineInterventionRequest	"#1# tells the group#*#'dime#*#'"
#EVENT DivineInterventionRequest	"#1# tells the raid#*#'dime#*#'"
#EVENT DivineInterventionRequest	"[ #1# #*#]#*#dime#*#"


Sub initEvents_Class_CLR
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_CLR
	/doevents BattleCleric
	/doevents CelestialRegeneration
	/doevents AutoRezON
	/doevents AutoRezOFF
	/doevents CombatRezON
	/doevents CombatRezOFF
	/doevents DivineInterventionRequest
/return

Sub xbot_initialize_class_CLR
    /call initEvents_Class_CLR
/return


Sub EVENT_DivineInterventionRequest(string _line, string _sender, string _eventParams) 
	/if (${Bool[${Me.Spell[Divine Intervention].ID}]}) {
		/if (${Me.SpellReady[Divine Intervention]}) {
			/declare tid int local ${Spawn[pc ${_sender}].ID}
			/if (${Target.ID} != ${tid}) {
				/call TrueTarget ${tid}
			}
			/echo \agDivine Intervention\aw on \ay${Target.Name}(${tid})
			/call MQ2Cast "Divine Intervention" gem6 8s
			/docommand /${str_announce_channel} DI >> ${Target.Name}
		} else {
			/docommand /${str_announce_channel} DI is NOT ready, ask someone else!
		}
	}
	/doevents flush DivineInterventionRequest
/return



Sub Event_AutoRezOn
	/call AutoRezON
	/doevents flush AutoRezON
/return

Sub AutoRezON
	/dgt ${Me.Name} is now auto rezzing out of combat.
	/varset bln_autoRez TRUE
/return 

Sub Event_AutoRezOff
	/call AutoRezOFF
	/doevents flush AutoRezOFF
/return 

Sub AutoRezOFF
	/dgt ${Me.Name} is no auto rezzing out of combat.
	/varset bln_autoRez FALSE
/return


Sub Event_CombatRezON
	/call CombatRezON
	/doevents flush CombatRezON
/return

Sub CombatRezON 
	/dgt ${Me.Name} is now auto rezzing during combat.
	/varset bln_autoRez TRUE
	/varset bln_combatRez TRUE
/return 

Sub Event_CombatRezOFF
	/call CombatRezOFF
	/doevents flush CombatRezOFF
/return 

Sub CombatRezOFF
	/dgt ${Me.Name} is no longer auto rezzing during combat.
	/varset bln_autoRez FALSE
	/varset bln_combatRez FALSE
/return
