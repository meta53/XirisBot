|**
----------------------------
xiris_dannet.inc
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
    06.22.22    xiris	INITIAL REVISION
    06.29.22	xiris 	Removed test harness methods
                        Changed var names in /dannet  for clarity

**|
#bind DanNet /dannet

Sub xbot_initialize_dannet 
    /declare dName string outer NULL
    /declare dDelim string outer |
    /declare dPeers string outer ${DanNet.Peers}
/return 

Sub Bind_DanNet(_dName, _dQuery)
    /echo \aw/dannet \ayName:\ag${_dName} \ayQuery: \ag"${_dQuery}"
    /if (${_dName.Equal[null]} || !${_dName.Length}) {
        /dnet info
        /return
    } else {
        | Check for Name in DanNet List. Note, FULL NAMES are off
        /varset dName ${_dName.Lower}
        | Can you find the name in the list?
        
        /if (${dPeers.Find[${dName}${dDelim}]}) {
            /echo Found Peer ${dName}
        } else {
            /echo Peer ${dName} NOT Found in List.
            /return FALSE
        }
    }
    /declare dResponse string outer NULL
    
    /if (${_dQuery.Equal[null]} || !${_dQuery.Length}) {
        /echo Query Information Required. Please Try Again.
        /return
    } else {
        /varset dResponse @null@
        /dquery ${dName} -q "${_dQuery}" -o dResponse
        /delay 20 ${DanNet[${dName}].Query["${_dQuery}"].Received}
    }

    /if (${Bool[${dResponse}]}) {
        /echo \awQuery \ay(${_dQuery}) \awto Peer \ag${dName} \awReturned \ag${dResponse}
    } else {
        /echo \awQuery \ay(${_dQuery}) \awto Peer \ag${dName} \arFailed or returned NO Data.
    }
   
/return ${dResponse}