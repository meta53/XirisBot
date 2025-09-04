|**
----------------------------
xiris_events_raid_mpg_endurance.inc
----------------------------
Event Handlers for the MPG: Trial of Endurance raid.
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	07.16.22    xiris	INITIAL REVISION
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Endurance



Sub InitEvents_OOW_MPG_Endurance
	/echo \ag Initializing MPG Endurance
    /call RegisterRaidEvents Events_OOW_MPG_Endurance 1
/return

Sub Events_OOW_MPG_Endurance
    /if (${Me.Name.Equal[Xiris]} && ${SpawnCount[npc Ansdaicher]}) /call chkAncients 
    /if (${Me.Class.ShortName.Equal[ENC]}) /call chkEnduranceMez
    /if (${Me.Class.ShortName.Equal[PAL]}) /call chkEnduranceAdds
    /if (${Me.Class.ShortName.Equal[WAR]} && ${Me.Name.NotEqual[Xiris]}) /call chkEnduranceAdds
/return 

Sub chkAncients
    /echo chkAncients
    
    :checkAnsdaicher
    /call TrueTarget ${Spawn[npc Ansdaicher]}
    /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
        /call chk_warProvoke Ancient: Chaos Cry
        /call chk_warProvoke Bazu Bellow
        /call chk_taunt
    }

    :checkZellucheraz
    /call TrueTarget ${Spawn[npc Zellucheraz]}
    /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
        /call chk_warProvoke Ancient: Chaos Cry
        /call chk_warProvoke Bazu Bellow
        /call chk_taunt  
    }

    /goto :checkAnsdaicher


/return

Sub chkEnduranceMez
    /if (${SpawnCount[npc defender]}) {
        /declare i int local 1
        /for i 1 to ${SpawnCount[npc defender]}
            /call TrueTarget ${NearestSpawn[${i}, npc defender].ID}

            /if (!${Target.Buff[${str_mezSingle}].ID}) {
                /call MQ2Cast "${str_mezSingle}" ${int_mezSingleGem}
            }
        :nextdefender
        /next i
    }
    :return
/return

Sub chkEnduranceAdds
    /if (${SpawnCount[npc]} > 2) {
        /squelch /varset bln_tanking TRUE
        /squelch /varset bln_taunting TRUE
        /declare i int local 1

        | Frantic Discordling
        /if (${${SpawnCount[npc frantic]}}) {
            /for i 1 to ${SpawnCount[npc frantic]}
                /call TrueTarget ${NearestSpawn[${i}, npc frantic].ID}
                /say Aggro Frantic ${Target.ID}
                :aggroFrantic
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]})) {
                    /attack on
                    /delay 10
                    /goto :aggroFrantic
                } else /if (${Me.TargetOfTarget.ID} != ${Me.ID}) {
                    | We want to immediately call kill on Frantic Discordlings (they rape)
                    /rs KillMob ${Target.ID} "${Target.Name}" ${Time.Time24}
                }
            :nextFrantic
            /next i
        }

        | Dragorn Antagonist 
        /if (${${SpawnCount[npc antagonist]}}) {
            /for i 1 to ${SpawnCount[npc antagonist]}
                /call TrueTarget ${NearestSpawn[${i}, npc antagonist].ID}
                :aggroAntagonist
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]})) {
                    /attack on
                    /delay 10
                    /goto :aggroAntagonist
                }
            :nextAntagonist
            /next i
        }
        
        | Muramite Sentinel 
        /if (${${SpawnCount[npc sentinel]}}) {
            /for i 1 to ${SpawnCount[npc sentinel]}
                /call TrueTarget ${NearestSpawn[${i}, npc sentinel].ID}
                :aggroSentinel
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]})) {
                    /attack on
                    /delay 10
                    /goto :aggroSentinel
                }
            :nextSentinel
            /next i
        }

        | Dragorn Champion 
        /if (${${SpawnCount[npc champion]}}) {
            /for i 1 to ${SpawnCount[npc champion]}
                /call TrueTarget ${NearestSpawn[${i}, npc champion].ID}
                :aggroChampion
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]})) {
                    /attack on
                    /delay 10
                    /goto :aggroChampion
                }
            :nextChampion
            /next i
        }

    } else {
        /squelch /varset bln_tanking    FALSE
        /squelch /varset bln_taunting   FALSE
    }
/return