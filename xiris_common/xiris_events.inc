|**
----------------------------
xiris_events.inc
----------------------------
Allows for aggregation of event handlers, and the kicking off of special cases
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
	04.03.22    xiris   INITIAL REVISION
    04.05.22    xiris   Added Foresight
    04.25.22    xiris   Added Hatred and Corruption
    04.26.22    xiris   Added Specialization
    06.27.22    xiris   Added Adaptation
    07.16.22    xiris   Added Endurance
______________________________________________
**|	

#include xiris_common/xiris_events_raid_god.inc 
#include xiris_common/xiris_events_raid_dodh_demiplane.inc 
#include xiris_common/xiris_events_raid_epic.inc 
#include xiris_common/xiris_events_raid_oow_anguish.inc
#include xiris_common/xiris_events_raid_oow_mpg_adaptation.inc
#include xiris_common/xiris_events_raid_oow_mpg_corruption.inc 
#include xiris_common/xiris_events_raid_oow_mpg_endurance.inc
#include xiris_common/xiris_events_raid_oow_mpg_foresight.inc 
#include xiris_common/xiris_events_raid_oow_mpg_hatred.inc 
#include xiris_common/xiris_events_raid_oow_mpg_specialization.inc



Sub xbot_initialize_events
    |--Array
    /declare BackgroundEventsRaw[100]   string outer NULL
    /declare BackgroundEvents[0]        string outer NULL
    /declare RaidEventsRaw[100]         string outer NULL
    /declare RaidEvents[0]              string outer NULL
    /declare ClassEventsRaw[50]         string outer NULL
    /declare ClassEvents[0]             string outer NULL

    |--Check for Special Events

    |- Gates of Discord
    /if (${Zone.ShortName.Equal[uqua]})         /call InitEvents_GoD_Uqua
    /if (${Zone.ShortName.Equal[ikkinz]})       /call InitEvents_GoD_Uqua
    /if (${Zone.ShortName.Equal[inktuta]})      /call InitEvents_GoD_Inktuta
    /if (${Zone.ShortName.Equal[tacvi]})        /call InitEvents_GoD_Tacvi

    |-- Omens of War
    /if (${Zone.ShortName.Equal[chambersa]})    /call InitEvents_OOW_MPG_Hatred
    /if (${Zone.ShortName.Equal[chambersb]})    /call InitEvents_OOW_MPG_Endurance
    /if (${Zone.ShortName.Equal[chambersc]})    /call InitEvents_OOW_MPG_Foresight
    /if (${Zone.ShortName.Equal[chambersd]})    /call InitEvents_OOW_MPG_Specialization
    /if (${Zone.ShortName.Equal[chamberse]})    /call InitEvents_OOW_MPG_Adaptation
    /if (${Zone.ShortName.Equal[chambersf]})    /call InitEvents_OOW_MPG_Corruption
    /if (${Zone.ShortName.Equal[arena]} || ${Zone.ShortName.Equal[anguish]})      /call InitEvents_OOW_Anguish

    |-- Epic 1.5 Fights
    /if (${Zone.ShortName.Equal[gfaydark]})     /call InitEvents_Epic
    
    |-- Dragons of Norrath
    /if (${Zone.ShortName.Equal[thenest]})      /call InitEvents_DoN_Vishimtar

    |-- Depths of Darkhollow
    /if (${Zone.ShortName.Equal[dreadspire]})   /call InitEvents_DoDH_Demiplane

    /echo \aoxbot_initialize_events

/return


Sub RegisterBackgroundEvents(string _EventSet, int _EventCount)
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${BackgroundEventsRaw.Size}
        /if (${BackgroundEventsRaw[${i}].Equal[NULL]}) {
            /varset BackgroundEventsRaw[${i}] ${_EventSet}
            /call ${_EventSet}
            /goto :postInsert
        }
    /next i
    :postInsert
     /for i 1 to ${BackgroundEventsRaw.Size}
        /if (${BackgroundEventsRaw[${i}].NotEqual[NULL]}) {
            /varcalc bound ${bound} + 1
        }
    /next i
    :return
    /call TrimBackgroundEvents ${bound}
/return

Sub TrimBackgroundEvents(int _count)
    /declare i int local -1
    /if (${Defined[BackgroundEvents]}) /deletevar BackgroundEvents
    /declare BackgroundEvents[${_count}] string outer
    /declare incr int local 1
    /for i 1 to ${BackgroundEventsRaw.Size}
        /if (${BackgroundEventsRaw[${i}].NotEqual[NULL]}) {
            /varset BackgroundEvents[${incr}] ${BackgroundEventsRaw[${i}]}
            /varcalc incr ${incr} + 1
        }
    /next i
/return


Sub RegisterRaidEvents(string _EventSet, int _EventCount)
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${RaidEventsRaw.Size}
        /if (${RaidEventsRaw[${i}].Equal[NULL]}) {
            /varset RaidEventsRaw[${i}] ${_EventSet}
            /goto :postInsert
        }
    /next i
    :postInsert
     /for i 1 to ${RaidEventsRaw.Size}
        /if (${RaidEventsRaw[${i}].NotEqual[NULL]}) {
            /varcalc bound ${bound} + 1
        }
    /next i
    :return
    /call TrimRaidEvents ${bound}
/return

Sub TrimRaidEvents(int _count)
    /declare i int local -1
    /if (${Defined[ClassEvents]}) /deletevar RaidEvents
    /declare RaidEvents[${_count}] string outer
    /declare incr int local 1
    /for i 1 to ${RaidEventsRaw.Size}
        /if (${RaidEventsRaw[${i}].NotEqual[NULL]}) {
            /varset RaidEvents[${incr}] ${RaidEventsRaw[${i}]}
            /varcalc incr ${incr} + 1
        }
    /next i
/return

Sub RegisterClassEvents(string _EventSet, int _EventCount)
    /if (${_EventCount} == 0) /return
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${ClassEventsRaw.Size}
        /if (${ClassEventsRaw[${i}].Equal[NULL]}) {
            /varset ClassEventsRaw[${i}] ${_EventSet}
            /goto :postInsert
        }
    /next i

    :postInsert
     /for i 1 to ${ClassEventsRaw.Size}
        /if (${ClassEventsRaw[${i}].NotEqual[NULL]}) {
            /varcalc bound ${bound} + 1
        }
    /next i

    :return
    /call TrimClassEvents ${bound}
/return

Sub TrimClassEvents(int _count)
    /declare i int local -1
    /if (${Defined[ClassEvents]}) /deletevar ClassEvents
    /declare ClassEvents[${_count}] string outer

    /declare incr int local 1
    /for i 1 to ${ClassEventsRaw.Size}
        /if (${ClassEventsRaw[${i}].NotEqual[NULL]}) {
            /varset ClassEvents[${incr}] ${ClassEventsRaw[${i}]}
            /varcalc incr ${incr} + 1
        }
    /next i
/return


Sub RunBackgroundEvents
    /if (${BackgroundEvents.Size} == 0) /return
    /declare i int local 1
    /for i 1 to ${BackgroundEvents.Size}
        /if (${BackgroundEvents[${i}].Equal[NULL]})  /goto :nextBGEvent
        /call ${BackgroundEvents[${i}]}
    :nextBGEvent
    /next i
/return

Sub RunRaidEvents
    /if (${RaidEvents.Size} == 0) /return
    /declare j int local 1
    /for j 1 to ${RaidEvents.Size}
        /if (${RaidEvents[${j}].Equal[NULL]})  /goto :nextRaidEvent
        /call ${RaidEvents[${j}]}
    :nextRaidEvent    
    /next j
/return

Sub RunClassEvents
    /if (${ClassEvents.Size} == 0) /return
    /declare k int local 1
    /for k 1 to ${ClassEvents.Size}
        /if (${ClassEvents[${k}].Equal[NULL]})  /goto :nextClassEvent
        /call ${ClassEvents[${k}]}
    :nextClassEvent
    /next k
/return


Sub RunAllEvents
    /call RunBackgroundEvents
    /call RunRaidEvents
    /call RunClassEvents
/return