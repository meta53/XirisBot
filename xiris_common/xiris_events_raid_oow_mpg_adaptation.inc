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
    07.15.22    xiris   First attempt revealed some bad code, fixing
    07.15.22    xiris   Second attempt. Don't really need this code, since it was almost flawless manually!
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Adaptation


Sub InitEvents_OOW_MPG_Adaptation
	/cecho \ag Initializing MPG Adaptation
    /declare str_bandolier                  string  outer default
    /declare adaptationPowerDebuffTimer     timer   outer 0
    /declare adaptationIceDebuffTimer       timer   outer 0
    /declare adaptationFireDebuffTimer      timer   outer 0
    /declare bln_phaseChanged               bool    outer FALSE
    /declare int_currentPhase               int     outer 0
    |/call RegisterRaidEvents Events_OOW_MPG_Adaptation 0
/return

Sub Events_OOW_MPG_Adaptation
    | Check state
    /call chkAdaptation

	|-- Killing Events
    /if (${Me.Class.ShortName.Equal[ENC]}) {
        /call AdaptationENC
    } else /if (${Me.Class.ShortName.Equal[SHM]}) {
        /call AdaptationSHM
    } else /if (${Me.Class.ShortName.Equal[PAL]}) {
        /call AdaptationPAL
    } else /if (${Me.Class.ShortName.Equal[PAL]}) {
        /call AdaptationSHD
    } else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]}) {
        /call AdaptationMELEE
    } else /if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[DRU]}) {
        /call AdaptationCASTER
    }
/return

Sub AdaptationENC
    | Construct of Power (5 dimunitives)
    /declare i int local
    /if (${SpawnCount[npc dimunitive]}) {
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}
            /call CastDebuffsOnTarget ${Target.ID} TRUE
            /if (!${Target.Buff[${str_mezSingle}].ID}) {
                /call MQ2Cast "${str_mezSingle}" ${int_mezSingleGem}
            }
        /next i
        /varset adaptationPowerDebuffTimer 30s
    }

    | Construct of Fire
    /if (${SpawnCount[npc lick of flame]}) {
        /if (${adaptationIceDebuffTimer}) /return
        /call TrueTarget ${NearestSpawn[${i}, npc lick of flame].ID}
        /call CastDebuffsOnTarget ${Target.ID} TRUE
        /varset adaptationFireDebuffTimer 1m
    }

    | Construct of Ice
    /if (${SpawnCount[npc mote of frost]}) {
        /if (${adaptationIceDebuffTimer}) /return
        /call TrueTarget ${NearestSpawn[${i}, npc mote of frost].ID}
        /call CastDebuffsOnTarget ${Target.ID} TRUE
        /varset adaptationIceDebuffTimer 1m
    }
/return 


Sub AdaptationPAL
    |My job as a paldin is to offtank things with stun!
    | Construct of Power (5 dimunitives) -note, since SK will be going 1-5, Pal needs to go 5-1
    /declare i int local
    /if (${SpawnCount[npc dimunitive]}) {
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}
            /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
                /if (!${Target.Buff["Euphoria"].ID}) {
                    /if (${Me.SpellReady[${spell_hate}]}) /call MQ2Cast "${spell_hate}" ${spell_hateGem}
                    /rsay KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}
                }
            }
        /next i
    }
/return

Sub AdaptationSHD 

    |My job as a shadowknight is to offtank things and kill them with my group!
    | Construct of Power (5 dimunitives)
    /declare i int local
    /if (${SpawnCount[npc dimunitive]}) {
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}
            /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
                /if (!${Target.Buff["Euphoria"].ID} && ${Me.SpellReady[${spell_hate}]} ) {
                    /call MQ2Cast "${spell_hate}" ${spell_hateGem}
                } 
            }
        /next i
    }

    | Construct of Fire
    /if (${SpawnCount[npc lick of flame]}) {
        /call TrueTarget ${NearestSpawn[${i}, npc lick of flame].ID}
        /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
            /if (${Me.SpellReady[${spell_hate}]} ) {
                /call MQ2Cast "${spell_hate}" ${spell_hateGem}
            } 
        } else {
            /if (!${Melee.Attack}) {
                /killthis 
                /rsay KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}
            }
        }
    }

    | Construct of Ice
    /if (${SpawnCount[npc mote of frost]}) {
        /call TrueTarget ${NearestSpawn[${i}, npc mote of frost].ID}
        /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
            /if (${Me.SpellReady[${spell_hate}]} ) {
                /call MQ2Cast "${spell_hate}" ${spell_hateGem}
            } 
        } else {
            /if (!${Melee.Attack}) {
                /killthis 
                /rsay KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}
            }
        }
    }

/return

Sub AdaptationSHM 
    | Construct of Power (5 dimunitives)
    /if (${SpawnCount[npc dimunitive]}) {
        /if (${adaptationPowerDebuffTimer}) /return
        /declare i int local
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}    
            /call CastDebuffsOnTarget ${Target.ID} FALSE
            
        /next i
        /varset adaptationPowerDebuffTimer 30s
    }

     | Construct of Fire
     /if (${SpawnCount[npc lick of flame]}) {
        /if (${adaptationIceDebuffTimer}) /return
        /call TrueTarget ${NearestSpawn[${i}, npc lick of flame].ID}
        /call CastDebuffsOnTarget ${Target.ID} TRUE
        /varset adaptationFireDebuffTimer 45s
     }

     | Construct of Ice
     /if (${SpawnCount[npc mote of frost]}) {
        /if (${adaptationIceDebuffTimer}) /return
        /call TrueTarget ${NearestSpawn[${i}, npc mote of frost].ID}
        /call CastDebuffsOnTarget ${Target.ID} TRUE
        /varset adaptationIceDebuffTimer 45s
     }



/return


|**
Offtank Group is 2 cleric, 2 knights, 1 shaman, 1 Chanter
**|

Sub chkAdaptation 
    | Construct of Pain (Dragorn)
    | * Withering Heat (Single Target 1500DD)
    | * Lacerating Aura (DS, must be removed)
    /if (${Bool[${Target.Name.Find[Construct of Pain]}]}) {
        /if (${int_currentPhase} != 1) {
            /varset bln_phaseChanged TRUE 
            /varset int_currentPhase 1
        }
    
    | Construct of Power (Golem)
    | * 5x "a dimunitive construct"  
    | * BLUNT ONLY 
    } else /if (${Bool[${Target.Name.Find[Construct of Power]}]}) {
        /if (${int_currentPhase} != 2) {
            /varset bln_phaseChanged TRUE 
            /varset int_currentPhase 2
        }

    | Construct of Fire 
    | * Flickering Fire (DS & Spell DS, must be removed)
    | * Metacrystalline Polyphase (DOT/Drain, 36 curse counters)
    | * 1x "a lick of flame"
    } else /if (${Bool[${Target.Name.Find[Construct of Fire]}]}) {
        /if (${int_currentPhase} != 3) {
            /varset bln_phaseChanged TRUE 
            /varset int_currentPhase 3
        }


    | Construct of Brutality
    | * AE RAMP
    | * SLASH ONLY
    } else /if (${Bool[${Target.Name.Find[Construct of Brutality]}]}) {
        /if (${int_currentPhase} != 4) {
            /varset bln_phaseChanged TRUE 
            /varset int_currentPhase 4
        }


    | Construct of Ice
    | * Breath of the Gelidran (2000DD, Snare)
    | * Glint of Ice (50% Spell Reflect, attempt to remove)
    | * 1x "a mote of frost"
    } else /if (${Bool[${Target.Name.Find[Construct of Ice]}]}) {
        /if (${int_currentPhase} != 5) {
            /varset bln_phaseChanged TRUE 
            /varset int_currentPhase 5
        }
    }
    :return
    /if (${bln_phaseChanged}) /call handlePhaseChange
/return 

Sub handlePhaseChange 
    /varset bln_phaseChanged FALSE
    /if (${int_currentPhase} == 1) {
        | PAIN
        /if (${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[CASTER]})  /call TrueTarget ${NearestSpawn[${i}, construct].ID}
        /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
        /if (${Me.Name.Equal[Xanshia]}) {
            /call TrueTarget ${NearestSpawn[${i}, construct].ID}
            /call CastDebuffsOnTarget ${Target.ID} TRUE
        } 
    } else /if (${int_currentPhase} == 2) {
        | POWER
        /if (${Me.Class.ShortName.Equal[SHD]} || ${Me.Class.ShortName.Equal[PAL]}) /return
        /if (${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[CASTER]})  /call TrueTarget ${NearestSpawn[${i}, construct].ID}
    } else /if (${int_currentPhase} == 3) {
        | FIRE
        /if (${Me.Class.ShortName.Equal[SHD]} || ${Me.Class.ShortName.Equal[PAL]}) /return
        /if (${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[CASTER]})  /call TrueTarget ${NearestSpawn[${i}, construct].ID}
        /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
        /if (${Me.Name.Equal[Xanshia]}) {
            /call TrueTarget ${NearestSpawn[${i}, construct].ID}
            /call CastDebuffsOnTarget ${Target.ID} TRUE
        } 
    } else /if (${int_currentPhase} == 4) {
        | BRUTALITY
        /if (${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[CASTER]})  /call TrueTarget ${NearestSpawn[${i}, construct].ID}
    } else /if (${int_currentPhase} == 5) {
        | ICE
        /if (${Me.Class.ShortName.Equal[SHD]} || ${Me.Class.ShortName.Equal[PAL]}) /return
        /if (${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[CASTER]})  /call TrueTarget ${NearestSpawn[${i}, construct].ID}
    }

/return 


Sub AdaptationMELEE
    /if (${Bool[${Target.Name.Find[Construct of Pain]}]}) {
        | Check to see if target has Spell DS
        /if (${Target.Buff[Lacerating Aura].ID}) {
            /cecho \arConstruct of Pain has SpellDS:Lacerating Aura!
            /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
            /delay 10
        } else {
            /if (!${Melee.Attack}) /killthis
        }
    } else /if  (${Bool[${Target.CleanName.Find[Construct of Fire]}]}) {
        | Construct of Fire is weak vs spells, aand has a ds that must be removed before attacking
        :ConstructOfFire_DSCheck
        | Check to see if target has DS
        /if (${Target.Buff[Flickering Fire].ID}) {
            /echo waiting 5s for Flickering Fire to be gone
            /if (${Melee.Attack}) /melee reset
            /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
            /delay 10
            /goto :ConstructOfFire_DSCheck
        } else {
            /if (!${Melee.Attack}) /killthis
        }

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Ice]}]}) {
        | Check to see if target has Reflect
        /if (${Target.Buff[Glint of Ice].ID}) {
            /cecho \atConstruct of Ice has Reflection!
            /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
            /if (!${Melee.Attack}) /killthis
        }

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Power]}]}) {

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Brutality]}]}) {
        | Construct of Brutality is weak vs SLASHING, and needs to be at max melee (ae ramp)
        /varset int_maxStickDistance 55
        /varset int_currentStickDistance 35
    }

    :return 

/return


Sub AdaptationCASTER
    | Construct of Fire
    :AdapationCasterLoop 
    /if  (${SpawnCount[npc Construct of Fire]}) {
        /call TrueTarget (${NearestSpawn[npc Construct of Fire].ID}
        | Construct of Fire is weak vs spells but has spell DS
        /if (${Target.Buff[Flickering Fire].ID}) {
            | Construct has Reflection on it, kill lick of flame instead
             /if (${SpawnCount[npc lick of flame]}) {
                 /call TrueTarget ${NearestSpawn[${i}, npc lick of flame].ID}
                 /call EngageTarget ${Target.ID}
            } else {
                /call ResetTarget
            }
        } else {
            /call EngageTarget ${Target.ID}
            /if (!${bln_BURNMode}) /varset bln_BURNMode TRUE.
        }
    } else  /if  (${SpawnCount[npc Construct of Ice]}) {
        /call TrueTarget (${NearestSpawn[npc Construct of Ice].ID}
        | Construct of Ice has a reflection buff
        /if (${Target.Buff[Glint of Ice].ID}) {
            | Construct has Reflection on it, kill mote of frost instead
             /if (${SpawnCount[npc mote of frost]}) {
                 /call TrueTarget ${NearestSpawn[${i}, npc mote of frost].ID}
                 /call EngageTarget ${Target.ID}
            } else {
                /call ResetTarget
            }
        } else {
            /call EngageTarget ${Target.ID}
            /if (${bln_BURNMode}) /varset bln_BURNMode FALSE.
        }
    } else /if  (${SpawnCount[npc Construct of Pain]}) {
        /call TrueTarget (${NearestSpawn[npc Construct of Pain].ID}
        | Construct of Pain has a reflection buff
        /if (${Target.Buff[Lacerating Aura].ID}) {
            | Construct has Reflection on it, do not cast
            /call ResetTarget
        } else {
            /call EngageTarget ${Target.ID}
            /if (${bln_BURNMode}) /varset bln_BURNMode FALSE.
        }
    }
    :return
/return
