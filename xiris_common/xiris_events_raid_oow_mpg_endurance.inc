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
	/cecho \ag Initializing MPG Endurance
    /call RegisterRaidEvents Events_OOW_MPG_Endurance 1
/return

Sub Events_OOW_MPG_Endurance
    /if (${Me.Class.ShortName.Equal[ENC]}) /call chkEnduranceMez
    /if (${Me.Class.ShortName.Equal[WAR]}) /call chkEnduranceAdds
    /if (${Me.Class.ShortName.Equal[PAL]}) /call chkEnduranceAdds
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
        /squelch /melee aggro=1
        /squelch /melee taunt=1
        /declare i int local 1

        | Frantic Discordling
        /if (${${SpawnCount[npc frantic]}}) {
            /for i 1 to ${SpawnCount[npc frantic]}
                /call TrueTarget ${NearestSpawn[${i}, npc frantic].ID}
                /say Aggro Frantic ${Target.ID}
                :aggroFrantic
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]})) {
                    /killthis
                    /delay 10
                    /goto :aggroFrantic
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
                    /killthis
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
                    /killthis
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
                    /killthis
                    /delay 10
                    /goto :aggroChampion
                }
            :nextChampion
            /next i
        }

    } else {
        /squelch /melee aggro=0
        /squelch /melee taunt=0
    }
/return