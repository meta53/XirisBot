|**
----------------------------
MQ2DANNET Utilities
----------------------------
This file includes various utilities to help querying members with mq2dannet

| - Use the following format for sending query.
| - /dannet PeerName Query
| - Examples:
| -     /dannet
| -        Will issue the /dnet info command.
| -     /dannet Xanshia Me.ID
| -        Will Send a Query to character named Xanshia and return his Spawn ID
| -     /dannet Xanshia Me.AltAbilityReady[MGB]
| -        Will Send a Query to character named Xanshia and return if MGB is ready
______________________________________________
REVISION HISTORY
    06.22.2022    xiris	INITIAL REVISION
**|

#bind DanNet /dannet
#bind DTestSet /dantest

Sub Bind_DanNet(DName, DQuery)
    /if (${DName.Equal[null]} || !${DName.Length}) {
        /dnet info
        /return
    } else {
        | Check for Name in DanNet List.
        /if (${DName.Find[_]}) {
            /varset Name1 ${DName.Lower}
        } else {
            /varset Name1 ${EverQuest.Server.Lower}_${DName.Lower}
        }
        | Can you find the name in the list?
        /varset PeerList1 ${DanNet.Peers}
        /if (${PeerList1.Find[${Name1}${DanDelim}]}) {
            /echo Found Peer ${Name1}
        } else {
            /echo Peer ${Name1} NOT Found in List.
            /return FALSE
        }
    }
    
    /if (${DQuery.Equal[null]} || !${DQuery.Length}) {
        /echo Query Information Required. Please Try Again.
        /return
    } else {
        /varset Response1 @null@
        /dquery ${Name1} -q "${DQuery}" -o Response1
        /delay 20 ${DanNet[${TName}].Query["${DQuery}"].Received}
    }
    
    /if (${Bool[${Response1}]}) {
        /echo Query (${DQuery}) to Peer ${Name1} Returned ${Response1}
    } else {
        /echo Query (${DQuery}) to Peer ${Name1} Failed or returned NO Data.
    }
   
/return ${Response1}

Sub Bind_DTestSet(DAction, P1, P2)
    | Actions: Ping 
    | Ping Param P1: Peer Name, all
    | Ping Param P2: Not Used.
    
    /declare TargetName string local
    
    /if (${DTestActive}) {
        /echo DanTest Still Active. Aborting Active Test.
        /varset DTestActive 0
        /return
    }
    /if (${DAction.Equal[null]} || !${DAction.Length} || ${DAction.NotEqual[ping]}) {
        /echo Invalid Action. Valid Actions are: Ping
        /return
    } else {
        /varset DTestAction ${DAction}
    }
    /if (${P1.Equal[null]} || !${P1.Length}) {
        /echo Invalid Paramater for Paramater 1. Paramater Required.
        /return
    }
    /if (${P1.Equal[all]}) {
        /varset TName all
    } else {
        | Check for Name in DanNet List.
        /if (${P1.Find[_]}) {
            /varset Name1 ${P1.Lower}
        } else {
            /varset Name1 ${EverQuest.Server.Lower}_${P1.Lower}
        }
        | Can you find the name in the list?
        /varset PeerList1 ${DanNet.Peers}
        /if (!${PeerList1.Find[${Name1}${DanDelim}]}) {
            /echo Invalid ${Name1}. Valid Names are: ${PeerList1}.
            /return
        } else {
            /varset TName ${Name1}
        }
    }
    /varset DTestActive 1
/return

Sub DTestRun
    /declare idx1      int    local 0
    /declare idx2      int    local 0
    /declare BTime     int    local 0
    /declare ETime     int    local 0
    
    /if (!${DTestActive}) /return
    
    /if (${DTestAction.Equal[ping]}) {
        /if (${TName.Equal[all]}) {
            /varset PeerList1 ${DanNet.Peers}
            /varset idx1 1
            /while (${Bool[${PeerList1.Arg[${idx1},|]}]}) {
                /varset TName ${PeerList1.Arg[${idx1},|]}
                /if (${TName.NotEqual[${MyPeerName}]}) {
                    /varset idx2 1
                    /echo ---------------------------------------- ${idx1} ----------------------------------------
                    /while (${idx2} < 4) {
                        /varset Response1 @null@
                        /varset BTime ${MacroQuest.Running}
                        /dquery ${TName} -q PingData1 -o Response1
                        /delay 20 ${DanNet[${TName}].Query[PingData1].Received}
                        /varset ETime ${MacroQuest.Running}
                        /if (!${Response1.Length}) {
                            /echo ${idx2} - No Information Received From ${TName} Time: ${Math.Calc[${ETime}-${BTime}]} 
                        } else {
                            /echo ${idx2} - Reply from ${TName} Received Length: ${Response1.Length} Time: ${Math.Calc[${ETime}-${BTime}]} 
                        }
                        /varcalc idx2 ${idx2}+1
                    }
                }
                /varcalc idx1 ${idx1}+1
            }
            /varset DTestActive 0
        } else {
            /varset idx1 1
            /while (${idx1} < 4) {
                /varset Response1 @null@
                /varset BTime ${MacroQuest.Running}
                /dquery ${TName} -q PingData1 -o Response1
                /delay 20 ${DanNet[${TName}].Query[PingData1].Received}
                /varset ETime ${MacroQuest.Running}
                |/echo Response: ${Response1} ${TName} 
                /if (!${Response1.Length}) {
                    /echo ${idx1} - No Information Received From ${TName} Time: ${Math.Calc[${ETime}-${BTime}]} 
                } else {
                    /echo ${idx1} - Reply from ${TName} Received Length: ${Response1.Length} Time: ${Math.Calc[${ETime}-${BTime}]} 
                }
                /varcalc idx1 ${idx1}+1
            }
            /varset DTestActive 0
        }
    }
    /varset DTestActive 0
/return