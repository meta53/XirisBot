|**
----------------------------
EVENTS_OOW_MPG_ADAPTATION.INC
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	06.27.22    xiris	INITIAL REVISION
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Adaptation



Sub InitEvents_OOW_MPG_Adaptation
	/cecho \ag Initializing MPG Adaptation
    /declare str_bandolier string outer default
/return

Sub Events_OOW_MPG_Adaptation
	|-- Killing Events
    /if (${Me.Class.ShortName.Equal[ENC]}) {
        /call AdaptationENC
    } else /if (${Me.Class.ShortName.Equal[PAL]}) {
        /call AdaptationPAL
    } else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
        /call AdaptationMELEE
    } else /if (${str_classArchetype.Equal[CASTER]}) /call AdaptationCASTER
/return

Sub AdaptationENC
    | Find any mote of frost if I don't have a pet and charm it
    /if (!${Me.Pet.ID}) {
        | Find mote

        | Determine LOS

        | Move to it if no LOS

        |
    } else {
        /if (${Bool[${Me.Pet.Name.Find[mote]}]}) {
            | call pet attack
        } else {
            /pet get lost
        }
    }
/return 


Sub AdaptationPAL

/return

Sub AdaptationSHD 

/return

|**
Offtank Group is 2 cleric, 2 knights, 1 shaman, 1 Chanter
**|

Sub AdaptationMELEE
    /if (${Bool[${Target.Name.Find[Construct of Pain]}]} && ${str_bandolier.NotEqual[BLUNT_ONLY]}) {
        | Construct of Pain is weak vs blunt, and flurries
        /varset str_bandolier BLUNT_ONLY
        /varset bln_bandoChanged TRUE
        /goto :return

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Fire]}]}) {
        | Construct of Pain is weak vs spells, aand has a ds that must be removed before attacking
        /if (${str_bandolier.NotEqual[default]}) {
            /varset str_bandolier default
            /varset bln_bandoChanged TRUE
        }
        | Check to see if target has DS
        /if (${Target.Buff[]})
        
        /goto :return

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Ice]}]} && ${str_bandolier.NotEqual[default]}) {
        | Construct of Ice reflects spells and has a and add Mote of Frost (charmable!)
        /varset str_bandolier default
        /varset bln_bandoChanged TRUE
        /goto :return

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Power]}]} && ${str_bandolier.NotEqual[SLASH_ONLY]}) {
        | Construct of Power is weak vs slash, and needs to be at max melee (ae ramp)
        /varset str_bandolier SLASH_ONLY
        /varset bln_bandoChanged TRUE
        /varset int_maxStickDistance 55
        /varset int_currentStickDistance 35
        /goto :return

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Brutality]}]} && ${str_bandolier.NotEqual[PIERCE_ONLY]}) {
        | Construct of Brutality is weak vs Piercing, and needs to be at max melee (ae ramp)
        /varset str_bandolier default
        /varset bln_bandoChanged TRUE
        /varset int_maxStickDistance 55
        /varset int_currentStickDistance 35
        /goto :return
    }

    :return 
    /if (${bln_bandoChanged}) /equipset ${str_bandolier}
    /if (${Bool[${Target.CleanName.Find[Construct of Fire]}]})
/return


Sub AdaptationCASTER

/return
