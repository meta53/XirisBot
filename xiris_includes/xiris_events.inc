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
    09.01.23    xiris   Added Anguish
    10.19.23    xiris   Added Class Events
______________________________________________
**|	

#include xiris_includes/events/xiris_events_raid_god.inc 
#include xiris_includes/events/xiris_events_raid_dodh_demiplane.inc 
#include xiris_includes/events/xiris_events_raid_epic.inc 
#include xiris_includes/events/xiris_events_raid_oow_anguish.inc
#include xiris_includes/events/xiris_events_raid_oow_mpg_adaptation.inc
#include xiris_includes/events/xiris_events_raid_oow_mpg_corruption.inc 
#include xiris_includes/events/xiris_events_raid_oow_mpg_endurance.inc
#include xiris_includes/events/xiris_events_raid_oow_mpg_foresight.inc 
#include xiris_includes/events/xiris_events_raid_oow_mpg_hatred.inc 
#include xiris_includes/events/xiris_events_raid_oow_mpg_specialization.inc



Sub xbot_initialize_events
    /echo \n \apInitialize: \aoEvents \aw-------------------------
    
    |--Timers for Event Types
    /declare timer_event_background     timer  outer 30
    /declare timer_event_raid           timer  outer 10
    /declare timer_event_class          timer  outer 10
    /declare timer_event_priority       timer  outer 0


    |--Array
    /declare BackgroundEventsRaw[100]   string outer NULL
    /declare BackgroundEvents[0]        string outer NULL
    /declare RaidEventsRaw[100]         string outer NULL
    /declare RaidEvents[0]              string outer NULL
    /declare ClassEventsRaw[50]         string outer NULL
    /declare ClassEvents[0]             string outer NULL
    /declare PriorityEventsRaw[10]      string outer NULL
    /declare PriorityEvents[0]          string outer NULL

    |--Check for Special Events

    |- Gates of Discord
    /if (${Zone.ShortName.Equal[uqua]})         /call initEvents_GoD_Uqua
    /if (${Zone.ShortName.Equal[ikkinz]})       /call initEvents_GoD_Uqua
    /if (${Zone.ShortName.Equal[inktuta]})      /call initEvents_GoD_Inktuta
    /if (${Zone.ShortName.Equal[tacvi]})        /call initEvents_GoD_Tacvi

    |- Omens of War
    /if (${Zone.ShortName.Equal[chambersa]})    /call initEvents_OOW_MPG_Hatred
    /if (${Zone.ShortName.Equal[chambersb]})    /call initEvents_OOW_MPG_Endurance
    /if (${Zone.ShortName.Equal[chambersc]})    /call initEvents_OOW_MPG_Foresight
    /if (${Zone.ShortName.Equal[chambersd]})    /call initEvents_OOW_MPG_Specialization
    /if (${Zone.ShortName.Equal[chamberse]})    /call initEvents_OOW_MPG_Adaptation
    /if (${Zone.ShortName.Equal[chambersf]})    /call initEvents_OOW_MPG_Corruption
    /if (${Zone.ShortName.Equal[thundercrest]} || ${Zone.ShortName.Equal[anguish]})      /call initEvents_OOW_Anguish

    |- Epic 1.5 Fights
    /if (${Zone.ShortName.Equal[gfaydark]})     /call initEvents_Epic
    
    |- Dragons of Norrath
    /if (${Zone.ShortName.Equal[thenest]})      /call initEvents_DoN_Vishimtar

    |- Depths of Darkhollow
    /if (${Zone.ShortName.Equal[dreadspire]})   /call initEvents_DoDH_Demiplane

   

/return

Sub AddEventToExistingSet(string _EventSet, string _EventName)
    /declare ExistingSet string local ${_EventSet}
    /declare TempSet[100]   string outer NULL

    |insert event at end
    /declare i int local 1
    /for i 1 to ${ExistingSet.Size}
        /varset TempSet[${i}] ${_EventSet[${i}]}
    /next i
   

/return

Sub RegisterBackgroundEvents(string _EventSet)
    /echo RegisterBackGroundEvents ${_EventSet}
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${BackgroundEventsRaw.Size}
        /if (${BackgroundEventsRaw[${i}].Equal[NULL]}) {
            /varset BackgroundEventsRaw[${i}] ${_EventSet}
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


Sub RegisterRaidEvents(string _EventSet)
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

Sub RegisterClassEvents(string _EventSet)
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

Sub RegisterPriorityEvents(string _EventSet)
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${PriorityEventsRaw.Size}
        /if (${PriorityEventsRaw[${i}].Equal[NULL]}) {
            /varset PriorityEventsRaw[${i}] ${_EventSet}
            /goto :postInsert
        }
    /next i

    :postInsert
     /for i 1 to ${PriorityEventsRaw.Size}
        /if (${PriorityEventsRaw[${i}].NotEqual[NULL]}) {
            /varcalc bound ${bound} + 1
        }
    /next i

    :return
    /call TrimPriorityEvents ${bound}
/return

Sub TrimPriorityEvents(int _count)
    /declare i int local -1
    /if (${Defined[ClassEvents]}) /deletevar PriorityEvents
    /declare PriorityEvents[${_count}] string outer

    /declare incr int local 1
    /for i 1 to ${PriorityEventsRaw.Size}
        /if (${PriorityEventsRaw[${i}].NotEqual[NULL]}) {
            /varset PriorityEvents[${incr}] ${PriorityEventsRaw[${i}]}
            /varcalc incr ${incr} + 1
        }
    /next i
/return


Sub RunBackgroundEvents
    |DEBUG< Turn Events off
    |/echo RunBackgroundEvents

    /if (${BackgroundEvents.Size} == 0 || ${timer_event_background}) /return
    /declare i int local 1
    /for i 1 to ${BackgroundEvents.Size}
        /if (${BackgroundEvents[${i}].Equal[NULL]})  /goto :nextBGEvent
        /call ${BackgroundEvents[${i}]}
    :nextBGEvent
    /next i
    | Timer for background events always the original value (3s)
    /varset timer_event_background ${timer_event_background.OriginalValue}
/return

Sub RunRaidEvents
    |DEBUG< Turn Events off
    |/echo RunRaidEvents
 
    /if (${RaidEvents.Size} == 0 || ${timer_event_raid}) /return
    /declare j int local 1
    /for j 1 to ${RaidEvents.Size}
        /if (${RaidEvents[${j}].Equal[NULL]})  /goto :nextRaidEvent
        /call ${RaidEvents[${j}]}
    :nextRaidEvent    
    /next j
    | Timer for raid events to 0 if killing
    /if (${bln_engaged} || ${bln_killing}) {
        /varset timer_event_raid 0
    } else {
        /varset timer_event_raid ${timer_event_raid.OriginalValue}
    }
/return

Sub RunClassEvents
    |DEBUG< Turn Events off
    |/echo RunClassEvents

    /if (${ClassEvents.Size} == 0 || ${timer_event_class}) /return
    /declare k int local 1
    /for k 1 to ${ClassEvents.Size}
        /if (${ClassEvents[${k}].Equal[NULL]})  /goto :nextClassEvent
        /call ${ClassEvents[${k}]}
    :nextClassEvent
    /next k
    | Timer for class events to 0 if killing
    /if (${bln_engaged} || ${bln_killing}) {
        /varset timer_event_class 0
    } else {
        /varset timer_event_class ${timer_event_class.OriginalValue}
    }
    
/return

Sub RunPriorityEvents
    |DEBUG< Turn Events off
    |/echo RunPriorityEvents ${PriorityEvents.Size}
    
    /if (${PriorityEvents.Size} == 0 || ${timer_event_priority}) /return
    /declare k int local 1
    /for k 1 to ${PriorityEvents.Size}
        |/echo priority ${PriorityEvents[${k}]}
        /if (${PriorityEvents[${k}].Equal[NULL]})  /goto :nextRunPriorityEvents
        /call ${PriorityEvents[${k}]}
    :nextRunPriorityEvents
    /next k
    | Timer for priority events always 0
    /varset timer_event_priority ${timer_event_priority.OriginalValue}
/return


Sub RunAllEvents
    |DEBUG< Turn Events off
    |/echo RunAllEvents
    
    /call RunBackgroundEvents
    /call RunRaidEvents
    /call RunClassEvents
    /call RunPriorityEvents
/return