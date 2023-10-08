|**
----------------------------
xiris_events_raid_mpg_adaptation.inc
----------------------------
Event Handlers for the MPG: Trial of Adaptation raid.
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
|-- Mastery of Destruction


#EVENT DestructionActive "#*#Destruction begins to glow#*#"

Sub InitEvents_OOW_MPG_Adaptation
	/echo \ag Initializing MPG Adaptation
    /declare str_bandolier                  string  outer default
    /declare adaptationPowerDebuffTimer     timer   outer 0
    /declare adaptationIceDebuffTimer       timer   outer 0
    /declare adaptationFireDebuffTimer      timer   outer 0
    /declare bln_phaseChanged               bool    outer FALSE
    /declare int_currentPhase               int     outer 0
    /declare glowTimer                      timer  outer 30s
    /call RegisterRaidEvents Events_OOW_MPG_Adaptation 0
/return

Sub Events_OOW_MPG_Adaptation
    /if (${SpawnCount[npc Master of Destruction]}) /doevents DestructionActive
	|/if (${Me.Class.ShortName.Equal[ENC]}) /call chkAdaptationMez
	/if (${Me.Class.ShortName.Equal[WAR]}) /call chkCorruptionAdds
    /if (${Me.Class.ShortName.Equal[PAL]}) /call chkCorruptionAdds
	/if (${Me.Class.ShortName.Equal[SHD]}) /call chkCorruptionAdds
/return

Sub EVENT_DestructionActive 
    /doevents flush DestructionActive
    /if (${glowTimer}) /return
    /echo \arDESTRUCTION Glow Triggered! ${Time.Time24}
    /gsay DESTRUCTION Glow Triggered! ${Time.Time24}
    /varset glowTimer 30s
/return 

Sub chkAdaptationMez
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
/return 


Sub chkAdaptationAdds
    |My job as a paldin is to offtank things with stun!
    | Construct of Power (5 dimunitives) -note, since SK will be going 1-5, Pal needs to go 5-1
    /declare i int local

    | Dimunitive Construct
    /if (${${SpawnCount[npc dimunitive]}}) {
        /for i 1 to ${SpawnCount[npc dimunitive]}
            /call TrueTarget ${NearestSpawn[${i}, npc dimunitive].ID}
            /say Aggroing dimunitive ${Target.Name} (${Target.ID})
            :aggroDimunitive
            /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < 150) {
                /attack on
                /delay 10
                /goto :aggroDimunitive
            }
        :nextDimunitive
        /next i
    }
    | Lick of Flame
    /if (${${SpawnCount[npc lick of flame]}}) {
        /for i 1 to ${SpawnCount[npc lick of flame]}
            /call TrueTarget ${NearestSpawn[${i}, npc lick of flame].ID}
            /say Aggroing lick of flame ${Target.Name} (${Target.ID})
            :aggroLick
            /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < 150) {
                /attack on
                /delay 10
                /goto :aggroLick
            }
        :nextLick
        /next i
    }
    
    | Mote of Frost
    /if (${${SpawnCount[npc mote of frost]}}) {
        /for i 1 to ${SpawnCount[npc mote of frost]}
            /call TrueTarget ${NearestSpawn[${i}, npc mote of frost].ID}
            /say Aggroing mote of frost ${Target.Name} (${Target.ID})
            :aggroMote
            /if ((${Me.TargetOfTarget.ID} != ${Me.ID}) && (${Me.TargetOfTarget.Class.ShortName.NotEqual[WAR]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[PAL]} || ${Me.TargetOfTarget.Class.ShortName.NotEqual[SHD]}) && ${Target.Distance3D} < 150) {
                /attack on
                /delay 10
                /goto :aggroMote
            }
        :nextMote
        /next i
    }

/return
