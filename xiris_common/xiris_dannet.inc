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


Sub xbot_initialize_dannet 
    /declare dName string outer NULL
    /declare dDelim string outer |
    /declare dPeers string outer ${DanNet.Peers}
/return 

Sub Bind_DanNet(DName, DQuery)
    /if (${DName.Equal[null]} || !${DName.Length}) {
        /dnet info
        /return
    } else {
        | Check for Name in DanNet List.
        /if (${DName.Find[_]}) {
            /varset dName ${DName.Lower}
        } else {
            /varset dName ${EverQuest.Server.Lower}_${DName.Lower}
        }
        | Can you find the name in the list?
        
        /if (${dPeers.Find[${dName}${dDelim}]}) {
            /echo Found Peer ${dName}
        } else {
            /echo Peer ${dName} NOT Found in List.
            /return FALSE
        }
    }
    
    /if (${DQuery.Equal[null]} || !${DQuery.Length}) {
        /echo Query Information Required. Please Try Again.
        /return
    } else {
        /varset dResponse @null@
        /dquery ${dName} -q "${DQuery}" -o dResponse
        /delay 20 ${DanNet[${dName}].Query["${DQuery}"].Received}
    }
    
    /if (${Bool[${dResponse}]}) {
        /echo Query (${DQuery}) to Peer ${dName} Returned ${dResponse}
    } else {
        /echo Query (${DQuery}) to Peer ${dName} Failed or returned NO Data.
    }
   
/return ${dResponse}

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
            /varset dName ${P1.Lower}
        } else {
            /varset dName ${EverQuest.Server.Lower}_${P1.Lower}
        }
        | Can you find the name in the list?
        /varset dPeers ${DanNet.Peers}
        /if (!${dPeers.Find[${dName}${dDelim}]}) {
            /echo Invalid ${dName}. Valid Names are: ${dPeers}.
            /return
        } else {
            /varset TName ${dName}
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
            /varset dPeers ${DanNet.Peers}
            /varset idx1 1
            /while (${Bool[${dPeers.Arg[${idx1},|]}]}) {
                /varset TName ${dPeers.Arg[${idx1},|]}
                /if (${TName.NotEqual[${MyPeerName}]}) {
                    /varset idx2 1
                    /echo ---------------------------------------- ${idx1} ----------------------------------------
                    /while (${idx2} < 4) {
                        /varset dResponse @null@
                        /varset BTime ${MacroQuest.Running}
                        /dquery ${TName} -q PingData1 -o dResponse
                        /delay 20 ${DanNet[${TName}].Query[PingData1].Received}
                        /varset ETime ${MacroQuest.Running}
                        /if (!${dResponse.Length}) {
                            /echo ${idx2} - No Information Received From ${TName} Time: ${Math.Calc[${ETime}-${BTime}]} 
                        } else {
                            /echo ${idx2} - Reply from ${TName} Received Length: ${dResponse.Length} Time: ${Math.Calc[${ETime}-${BTime}]} 
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
                /varset dResponse @null@
                /varset BTime ${MacroQuest.Running}
                /dquery ${TName} -q PingData1 -o dResponse
                /delay 20 ${DanNet[${TName}].Query[PingData1].Received}
                /varset ETime ${MacroQuest.Running}
                |/echo Response: ${dResponse} ${TName} 
                /if (!${dResponse.Length}) {
                    /echo ${idx1} - No Information Received From ${TName} Time: ${Math.Calc[${ETime}-${BTime}]} 
                } else {
                    /echo ${idx1} - Reply from ${TName} Received Length: ${dResponse.Length} Time: ${Math.Calc[${ETime}-${BTime}]} 
                }
                /varcalc idx1 ${idx1}+1
            }
            /varset DTestActive 0
        }
    }
    /varset DTestActive 0
/return