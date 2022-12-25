|**
	EXCLUSION LIST for SHAMAN/ENC/ETC that auto target mobs
	/if (!${ExcludeList.Find[${Spawn[${mobID}]}].Name}) /continue
**|

#EVENT REFRESHEXCLUSIONS 	"#1# tells the group#*#'RefreshExclusions#*#'"
#EVENT REFRESHEXCLUSIONS	"#1# tells you#*#'RefreshExclusions#*#'"
#EVENT REFRESHEXCLUSIONS	"#1# tells the raid#*#RefreshExclusions#*#'"
#EVENT REFRESHEXCLUSIONS	"[ #1# #*#]#*#RefreshExclusions#*#"

#EVENT ADDEXCLUSIONS 		"#1# tells the group#*#'AddExclusions #2#'"
#EVENT ADDEXCLUSIONS		"#1# tells you#*#'AddExclusions #2#'"
#EVENT ADDEXCLUSIONS		"#1# tells the raid#*#AddExclusions #2#'"
#EVENT ADDEXCLUSIONS	 	"[ #1# #*#]#*#AddExclusions #2#"

Sub InitEvents_Exclusions
	/call RegisterBackgroundEvents Events_Exclusions 1
/return 

Sub Events_Exclusions 
	/doevents REFRESHEXCLUSIONS
	/doevents ADDEXCLUSION
/return

Sub EVENT_REFRESHEXCLUSIONS(_line, _chatSender, _eventParams)
	/call RefreshExclusions
/return

Sub EVENT_ADDEXCLUSION(_line, _chatSender, _eventParams)
	/call AddExclusions ${_eventParams}
/return

Sub RefreshExclusions
	/call setExclusions
/return

Sub AddExclusions(string _list)
	/varset ExcludeList ${ExcludeList}|${_list}
/return

Sub setExclusions
	| Clear Exclusion List
	/varset ExcludeList 

	| Load Global Exclusions
	/declare ExcludeGlobal string  local ${Ini[${ExcludeINI},"Excludes.global","Exclude"]}
	/cecho \ayGlobal Exclusions: \ag${ExcludeGlobal}
	/call AddExclusions "${ExcludeGlobal}"
	
	| Add zone specific Exclusions
	| If Zone entry not found, add it.
	/declare ExcludeZone string  local ${Ini[${ExcludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
	/if (${ExcludeZone.Equal[NULL]}) {
		/ini "${ExcludeZone}" "Excludes.${Zone.ShortName}" "Exclude" "|"
		/varset ExcludeZone ${Ini[${ExcludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
		/cecho \ayZone Exclusions: ${ExcludeZone}
	} else /if (${ExcludeZone.Equal[|]}) {
		/varset ExcludeZone 
	}
	/cecho \ayZone Exclusions: ${ExcludeZone}
	/call AddExclusions "${ExcludeZone}"

	/cecho \agExclusionList: \aw${ExcludeList}
/return 

Sub xbot_initialize_exclusions
	/cecho \ao xbot_initialize_exclusions
	|-- [Settings] : Mob Exclusion List
	/declare 	ExcludeList string  outer   
	/declare    ExcludeINI	string  outer   xiris_common/xiris_exclude.ini
	/call InitEvents_Exclusions
	/call setExclusions
/return