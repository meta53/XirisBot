|**
----------------------------
xiris_clicks.inc
----------------------------
ITEM CLICKING HANDLERS. This library performs item clicks. 
______________________________________________
REVISION HISTORY
    07.28.23    xiris	INITIAL REVISION - Moved from xiris_burn.inc
**|


Sub initEvents_Clicking
    /call RegisterRaidEvents Events_Clicking 1
/return 

Sub Events_Clicking
    /if (!${timer_check_clicks}) /call chkClicks
/return 

Sub xbot_initialize_clicking
    /echo \n \apInitialize: \aoClicking \aw-------------------------

    /call processClicks
    
    /call initEvents_Clicking
/return 

Sub processClicks
    /echo \awprocessItems

    |; _enabled|_delay|_startHP|_stopHP
	| settings=TRUE|1**|
    /declare bln_clicksEnabled      bool    outer   TRUE
    /declare int_click_delay        int     outer   0
    /declare int_click_start        int     outer   0
    /declare int_click_stop         int     outer   0
    /declare int_click_total        int     outer   0
    /declare timer_check_clicks     timer   outer   50

    /if (!${Ini[INI_CHAR,CLICKS,settings].Arg[1,|]})  /varset bln_clicksEnabled   FALSE
    /if (!${Ini[INI_CHAR,CLICKS,settings].Arg[2,|]})  /varset int_click_delay     0
    /if (!${Ini[INI_CHAR,CLICKS,settings].Arg[3,|]})  /varset int_click_start     98
    /if (!${Ini[INI_CHAR,CLICKS,settings].Arg[4,|]})  /varset int_click_stop      0

    /echo \a-w    Settings Processed: \awEnabled:\ag${bln_clicksEnabled} \awDelay:\ag${int_click_delay} \awStart:\ag${int_click_start} \awStop:\ag${int_click_stop}
    |;_item|_type|_effect[OFFENSE|DEFENSE|HEAL]|_use|_namedOnly|_startHP|_stopHP|_trigger
    | Note: Trigger is globally set to true for now. In future it will support the _effect type 
    /declare raw_click[20]       string  local   NULL
    /declare i                  int     local   0
    /declare sz                 int     local   0
    /declare test               string  local   NULL

    /for i 1 to ${raw_click.Size}
        /varset test ${Ini[INI_CHAR,CLICKS,CLICK_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_click_total ${sz}
            /return
        }
        | Not null, set the values for the clicks
        /varcalc sz ${sz}+1
        /declare CLICK_${i}             string  outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[1,|]}
        /declare CLICK_${i}_Type        string  outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[2,|]}
        /declare CLICK_${i}_Effect      string  outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[3,|]}       
        /declare CLICK_${i}_Use         bool    outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[4,|]}
        /declare CLICK_${i}_NamedOnly   bool    outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[5,|]}
        /declare CLICK_${i}_Start       int     outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[6,|]}
        /declare CLICK_${i}_Stop        int     outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[7,|]}
        /declare CLICK_${i}_Trigger     bool    outer   ${Ini[INI_CHAR,CLICKS,CLICK_${i}].Arg[8,|]}
         /echo \ao        CLICK[\ag${i}\ao]: \awName:\ag${CLICK_${i}} \awType:\ag${CLICK_${i}_Type} \awEffect:\ag${CLICK_${i}_Effect} \awEnabled:\ag${CLICK_${i}_Use} \awNamedOnly:\ag${CLICK_${i}_NamedOnly} \awStart:\ag${CLICK_${i}_Start} \awStop:\ag${CLICK_${i}_Stop} \awTrigger:\ag${CLICK_${i}_Trigger}
    /next i
/return


|**
 ----------------------------
 Check CLICK routine
 ----------------------------
 Fires the clicky if an event has set bln_burning TRUE
 ----------------------------
**|

Sub chkClicks 
    /if (!${int_click_total} || (${Me.Casting.ID} && ${Me.ClassName.NotEqual[BRD]}) || ${Me.Invis} || ${Me.Feigning} || ${timer_clickDelay}) /return

    | Global [CLICKY] settings check
    /if (${Target.PctHps} > ${int_click_start} || ${Target.PctHPs} < ${int_click_stop}) /return

    | Named Check
    /declare savedTargetID ${Target.ID}
    /call TrueTarget ${int_targetID}
    /call TargetNamedCheck
    /declare targetIsNamed  bool    outer   ${Macro.Return}

    /declare i int local 0
    /for i 1 to ${int_click_total}
        | If the timer isn't ready, skip
		/if (${FindItem[${CLICKY_${i}}].TimerReady} > 0) /goto :nextClick
        | Handle click types
        /if (${CLICK_${i}_Type.Equal[OFFENSE]}) {
            /if (!${bln_engaged} || (${CLICK_${i}_NamedOnly} && !${targetIsNamed}) || (${Target.PctHps} > ${CLICK_${i}_Start} || ${Target.PctHPs} < ${CLICK_${i}_Stop}))  /goto :nextClick
        } else /if (${CLICK_${i}_Type.Equal[DEFENSE]})
            /if (!${bln_engaged} || (${CLICK_${i}_NamedOnly} && !${targetIsNamed}))  /goto :nextClick 
        } else /if (${CLICK_${i}_Type.Equal[HEAL]})
            /call TrueTarget ${Me.ID}
            /if ( (${Target.PctHps} > ${CLICK_${i}_Start} || ${Target.PctHPs} < ${CLICK_${i}_Stop})) /return 
        }
        | Handle the trigger (not currently implemented, all are set to TRUE)
        /if (!${CLICK_${i}_Trigger}) /goto :nextClick

        :castClick
        /if (${Me.Class.ShortName.Equal[BRD]}) {
            /call MQ2Medley "${CLICKY_${i}}" ITEM ${Target.ID} TRUE
        } else {
            /call MQ2Cast   "${CLICKY_${i}}" ITEM
        }
    :nextClick
    /next i
    /varset timer_check_clicks ${timer_check_clicks.OriginalValue}
/return