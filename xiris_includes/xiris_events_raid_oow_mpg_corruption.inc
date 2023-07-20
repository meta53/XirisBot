|**
----------------------------
xiris_events_raid_mpg_corruption.inc
----------------------------
Event Handlers for the MPG: Trial of Corruption raid.
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	04.25.22    xiris	INITIAL REVISION
	08.14.22	xiris 	Added OT routines that prioritize ROG/ENC/MNK/WAR
    09.18.22    xiris   Commented out Mez because you... can't
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Corruption



Sub initEvents_OOW_MPG_Corruption
	/echo \ag Initializing MPG Corruption
	/call RegisterRaidEvents Events_OOW_MPG_Corruption 1
/return

Sub Events_OOW_MPG_Corruption
	|/if (${Me.Class.ShortName.Equal[ENC]}) /call chkCorruptionMez
	/if (${Me.Class.ShortName.Equal[WAR]}) /call chkCorruptionAdds
    /if (${Me.Class.ShortName.Equal[PAL]}) /call chkCorruptionAdds
	/if (${Me.Class.ShortName.Equal[SHD]}) /call chkCorruptionAdds
/return

Sub chkCorruptionMez
    /if (!${bln_engaged}) /return
	/declare i int local 1
	| Mez Wizards
	/if (${SpawnCount[npc WIZ]}) {
        /for i 1 to ${SpawnCount[npc WIZ]}
            /call TrueTarget ${NearestSpawn[${i}, npc WIZ].ID}
            /if (!${Target.Buff[${str_mezSingle}].ID}) {
				/say Mezzing ${Target.Name}
                /call MQ2Cast "${str_mezSingle}" ${int_mezSingleGem}
				/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
					/docommand /${announceChannel} ${Target.Name} MEZZED
				}
            }
        :nextwizard
        /next i
	}

	| Mez Mages
	/if (${SpawnCount[npc MAG]}) {
        /for i 1 to ${SpawnCount[npc MAG]}
            /call TrueTarget ${NearestSpawn[${i}, npc MAG].ID}
            /if (!${Target.Buff[${str_mezSingle}].ID}) {
				/say Mezzing ${Target.Name}
                /call MQ2Cast "${str_mezSingle}" ${int_mezSingleGem}
				/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
					/docommand /${announceChannel} ${Target.Name} MEZZED
				}
            }
        :nextmage
        /next i
	}

	| Mez Necro
	/if (${SpawnCount[npc NEC]}) {
        /for i 1 to ${SpawnCount[npc NEC]}
            /call TrueTarget ${NearestSpawn[${i}, npc NEC].ID}
            /if (!${Target.Buff[${str_mezSingle}].ID}) {
				/say Mezzing ${Target.Name}
                /call MQ2Cast "${str_mezSingle}" ${int_mezSingleGem}
				/if (${Macro.Return.Equal[CAST_SUCCESS]}) {
					/docommand /${announceChannel} ${Target.Name} MEZZED
				}
            }
        :nextnecro
        /next i
	}
/return

Sub chkCorruptionAdds
    /if (!${bln_engaged}) /return
	/if (${SpawnCount[npc]} > 2) {
		/squelch /varset bln_tanking TRUE
        /squelch /varset bln_taunting TRUE
        /declare i int local 1

		| ROG
        /if (${${SpawnCount[npc ROG]}}) {
            /for i 1 to ${SpawnCount[npc ROG]}
                /call TrueTarget ${NearestSpawn[${i}, npc ROG].ID}
                /say Aggroing Rogue ${Target.Name} (${Target.ID})
                :aggroRogue
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < 150) {
                    /attack on
                    /delay 10
                    /goto :aggroRogue
                }
            :nextRogue
            /next i
        }
		| WAR
        /if (${${SpawnCount[npc WAR]}}) {
            /for i 1 to ${SpawnCount[npc WAR]}
				/if (${Bool[${NearestSpawn[${i}, npc WAR].Name.Find[Huhn-Abram]}]}) /goto :nextWarrior
                /call TrueTarget ${NearestSpawn[${i}, npc WAR].ID}
                /say Aggroing Warrior ${Target.Name} (${Target.ID})
                :aggroWarrior
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < 150) {
                    /attack on
                    /delay 10
                    /goto :aggroWarrior
                }
            :nextWarrior
            /next i
        }
		| ENC
        /if (${${SpawnCount[npc ENC]}}) {
            /for i 1 to ${SpawnCount[npc ENC]}
                /call TrueTarget ${NearestSpawn[${i}, npc ENC].ID}
                /say Aggroing Enchanter ${Target.Name} (${Target.ID})
                :aggroEnchanter
                /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < 150) {
                    /attack on
                    /delay 10
                    /goto :aggroEnchanter
                }
            :nextEnchanter
            /next i
        }

	} else {
        /squelch /varset bln_tanking    FALSE
        /squelch /varset bln_taunting   FALSE
    }

/return


