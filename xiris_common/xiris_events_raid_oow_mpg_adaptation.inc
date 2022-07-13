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
    /declare str_bandolier                  string  outer default
    /declare adaptationPowerDebuffTimer     timer   outer 0
    /declare adaptationIceDebuffTimer       timer   outer 0
    /declare adaptationFireDebuffTimer      timer   outer 0
    
    /call RegisterRaidEvents Events_OOW_MPG_Adaptation 0
/return

Sub Events_OOW_MPG_Adaptation
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
    } else /if (${str_classArchetype.Equal[CASTER]} || ${Me.Class.ShortName.Equal[DRU]}) /call AdaptationCASTER
/return

Sub AdaptationENC
    | Construct of Power (5 dimunitives)
    /declare i int local
    /if (${SpawnCount[npc dimunitive]}) {
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}
            /call CastDebuffsOnTarget ${Target.ID} TRUE
            /if (!${Target.Buff[${str_mezSingle}]}) {
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

   
    | Find any mote of frost if I don't have a pet and charm it (well, if we were in DODH)
    |**
     /if (!${bln_activeCharming}) /varset bln_activeCharming TRUE
    /if (!${Me.Pet.ID} && ${SpawnCount[npc mote]}) {
        | Find mote
        :moteFind
        /call TrueTarget ${Spawn[npc mote].ID}
        | Determine LOS
        :moteLineOfSight
        /if (!${Target.LineOfSight} || ${Target.Distance3D} > 150) {
            | Move to it if no LOS
            /if (!${Navigation.Active}) /nav target
            /delay 10
            /goto :moteLineOfSight
        }

        | Charm it!
        :moteCharm
        /if (${Navigation.Active}) /nav stop
        /call CharmNPC ${Spawn[npc mote].ID}
        | Charm Event will send pet to attack int_targetID (ie: /call EngageTarget)
    } else {
      |  /cecho \ag Either I have a pet, or there is no mote to charm!
    }**|
/return 


Sub AdaptationPAL
    |My job as a paldin is to offtank things with stun!
    | Construct of Power (5 dimunitives) -note, since SK will be going 1-5, Pal needs to go 5-1
    /declare i int local
    /if (${SpawnCount[npc dimunitive]}) {
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}
            /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
                /if (!${Target.Buff["Euphoria"].ID} && ${Me.SpellReady[${spell_hate}]} ) {
                    /call MQ2Cast "${spell_hate}" ${spell_hateGem}
                } else {
                    /killthis 
                    /delay 30
                    /melee reset
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
                } else {
                    /killthis 
                    /delay 30
                    /melee reset
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
                /gsay KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}
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
                /gsay KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}
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


|**
Offtank Group is 2 cleric, 2 knights, 1 shaman, 1 Chanter
**|

Sub AdaptationMELEE
    /if (${Bool[${Target.Name.Find[Construct of Pain]}]} && ${str_bandolier.NotEqual[PIERCE_ONLY]}) {
        | Construct of Pain is weak vs piercing, and flurries
         /if (${str_bandolier.NotEqual[PIERCE_ONLY]}) {
             /varset str_bandolier PIERCE_ONLY
             /varset bln_bandoChanged TRUE
        }
        /varset str_bandolier BLUNT_ONLY
        /varset bln_bandoChanged TRUE
        | Check to see if target has Spell DS
        /if (${Target.Buff[Lacerating Aura]}) {
            /cecho \arConstruct of Pain has SpellDS:Lacerating Aura!
            /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
            /delay 10
        } else {
            /if (!${Melee.Attack}) /killthis
        }
    } else /if  (${Bool[${Target.CleanName.Find[Construct of Fire]}]}) {
        | Construct of Fire is weak vs spells, aand has a ds that must be removed before attacking
        /if (${str_bandolier.NotEqual[default]}) {
            /varset str_bandolier default
            /varset bln_bandoChanged TRUE
        }
        :ConstructOfFire_DSCheck
        | Check to see if target has DS
        /if (${Target.Buff[Flickering Fire]}) {
            /echo waiting 5s for Flickering Fire to be gone
            /if (${Melee.Attack}) /melee reset
            /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
            /delay 10
            /goto :ConstructOfFire_DSCheck
        } else {
            /if (!${Melee.Attack}) /killthis
        }

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Ice]}]}) {
        | Construct of Ice reflects spells and has a and add Mote of Frost (charmable!)
        /if (${str_bandolier.NotEqual[default]}) {
             /varset str_bandolier default
             /varset bln_bandoChanged TRUE
        }
        | Check to see if target has Reflect
        /if (${Target.Buff[Glint of Ice]}) {
            /cecho \atConstruct of Ice has Reflection!
            /if (${Me.Class.ShortName.Equal[RNG]}) /call CastDebuffsOnTarget ${Target.ID} TRUE
        }

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Power]}]} && ${str_bandolier.NotEqual[BLUNT_ONLY]}) {
        | Construct of Power is weak vs slash, and needs to be at max melee (ae ramp)
        /varset str_bandolier BLUNT_ONLY
        /varset bln_bandoChanged TRUE

    } else /if  (${Bool[${Target.CleanName.Find[Construct of Brutality]}]} && ${str_bandolier.NotEqual[SLASH_ONLY]}) {
        | Construct of Brutality is weak vs Slashing, and needs to be at max melee (ae ramp)
        /varset str_bandolier SLASH_ONLY
        /varset bln_bandoChanged TRUE
        /varset int_maxStickDistance 55
        /varset int_currentStickDistance 35
    }

    :return 
    /if (${bln_bandoChanged}) {
        /call doBandolier ${str_bandolier}
    } 
/return


Sub AdaptationCASTER
    | Construct of Fire
    :AdapationCasterLoop 
    /if  (${SpawnCount[npc Construct of Fire]}) {
        /call TrueTarget (${NearestSpawn[npc Construct of Fire].ID}
        | Construct of Fire is weak vs spells but has spell DS
        /if (${Target.Buff[Flickering Fire]}) {
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
        /if (${Target.Buff[Glint of Ice]}) {
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
        /if (${Target.Buff[Lacerating Aura]}) {
            | Construct has Reflection on it, do not cast
            /call ResetTarget
        } else {
            /call EngageTarget ${Target.ID}
            /if (${bln_BURNMode}) /varset bln_BURNMode FALSE.
        }
    }
    :return
/return
