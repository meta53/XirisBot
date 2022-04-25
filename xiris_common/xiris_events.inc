|**
----------------------------
XIRIS_EVENTS.INC
----------------------------
This was written ground up for usage with xiris's bots
Allows for aggregation of event handlers, and the kicking off of special cases
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
	04.05.22    xiris   INITIAL REVISION
    04.25.22    xiris   Added Hatred and Corruption
______________________________________________
**|	

#include xiris_common/xiris_events_raid_god.inc 
#include xiris_common/xiris_events_raid_dodh_demi.inc 
#include xiris_common/xiris_events_raid_oow_mpg_foresight.inc 
#include xiris_common/xiris_events_raid_oow_mpg_hatred.inc 
#include xiris_common/xiris_events_raid_oow_mpg_corruption.inc 

Sub xbot_initialize_events
    |--Array
    /declare BackgroundEventsRaw[100]   string outer NULL
    /declare BackgroundEvents[0]        string outer NULL
    /declare RaidEventsRaw[25]          string outer NULL
    /declare RaidEvents[0]              string outer NULL
    /declare ClassEventsRaw[10]         string outer NULL
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
    /if (${Zone.ShortName.Equal[chamberse]})    /call InitEvents_OOW_MPG_Corruption
    /if (${Zone.ShortName.Equal[anguish]})      /call InitEvents_OOW_Anguish

    |-- Dragons of Norrath
    /if (${Zone.ShortName.Equal[thenest]})      /call InitEvents_DoN_Vishimtar

    |-- Depths of Darkhollow
    /if (${Zone.ShortName.Equal[dreadspire]})   /call InitEvents_DoDH_Demiplane

/return


Sub RegisterBackgroundEvents(string _EventSet, int _EventCount)
    |/cecho \agRegisterBackgroundEvents(\ay${_EventSet}:${_EventCount})
    | find the first index in BackgroundEvents thats empty
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${BackgroundEventsRaw.Size}
        /if (${BackgroundEventsRaw[${i}].Equal[NULL]}) {
            /echo inserting ${_EventSet} @ index: ${i}
            /varset BackgroundEventsRaw[${i}] ${_EventSet}
            |/cecho \agBackgroundEvents[${i}]: ${BackgroundEventsRaw[${i}]} 
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
   | /cecho \ayTrimBackgroundEvents ${_count}
    
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

      |/for i 1 to ${BackgroundEvents.Size}
        |/cecho  \agBackgroundEvents[${i}] ${BackgroundEvents[${i}]}
    |/next i

/return


Sub RegisterRaidEvents(string _EventSet, int _EventCount)
    |/cecho \agRegisterRaidEvents(\ay${_EventSet}:${_EventCount})
    | find the first index in RaidEventsRaw thats empty
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${RaidEventsRaw.Size}
        /if (${RaidEventsRaw[${i}].Equal[NULL]}) {
            /echo inserting ${_EventSet} @ index: ${i}
            /varset RaidEventsRaw[${i}] ${_EventSet}
            |/cecho \agRaidEventsRaw[${i}]: ${RaidEventsRaw[${i}]} 
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
    |/cecho \ayTrimRaidEvents ${_count}
    
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

      |/for i 1 to ${RaidEvents.Size}
     |   /cecho  \agRaidEvents[${i}] ${RaidEvents[${i}]}
    |/next i

/return

Sub RegisterClassEvents(string _EventSet, int _EventCount)
    /if (${_EventCount} == 0) /return
    |/cecho \agRegisterClassEvents(\ay${_EventSet}:${_EventCount})
    | find the first index in ClassEventsRaw thats empty
    /declare i int local -1
    /declare bound int local 0
    /for i 1 to ${ClassEventsRaw.Size}
        /if (${ClassEventsRaw[${i}].Equal[NULL]}) {
            /echo inserting ${_EventSet} @ index: ${i}
            /varset ClassEventsRaw[${i}] ${_EventSet}
           | /cecho \agClassEvents[${i}]: ${ClassEventsRaw[${i}]} 
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
    |/cecho \ayTrimClassEvents ${_count}
    
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

      |/for i 1 to ${ClassEvents.Size}
      |  /cecho  \agClassEvents[${i}] ${ClassEvents[${i}]}
    |/next i

/return


Sub RunBackgroundEvents
    /declare i int local 1
    /for i 1 to ${BackgroundEvents.Size}
        |/echo calling ${BackgroundEvents[${i}]}
        /call ${BackgroundEvents[${i}]}
    /next i
/return

Sub RunRaidEvents
    /if (${RaidEvents.Size} == 0) /return
    /declare i int local 1
    /for i 1 to ${RaidEvents.Size}
        |/echo calling ${RaidEvents[${i}]}
        /call ${RaidEvents[${i}]}
    /next i
/return

Sub RunClassEvents(string _EventSet)
    /if (${ClassEvents.Size} == 0) /return
    /declare i int local 1
    /for i 1 to ${ClassEvents.Size}
        |/echo calling ${ClassEvents[${i}]}
        /call ${ClassEvents[${i}]}
    /next i
/return


Sub RunAllEvents
    /call RunBackgroundEvents
    /call RunRaidEvents
    /call RunClassEvents
/return