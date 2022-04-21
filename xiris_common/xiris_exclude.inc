|**
	EXCLUSION LIST for SHAMAN/ENC/ETC that auto target mobs
	/if (!${ExcludeList.Find[${Spawn[${mobID}]}].Name}) /continue
**|



Sub setExclusions
	/if (!${Defined[ExcludeList]}) /declare ExcludeList string outer
	/varset ExcludeList ${Ini[${excludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
	/if (${ExcludeList.Equal[NULL]}) {
		/ini "${excludeINI}" "Excludes.${Zone.ShortName}" "Exclude" "|"
		/varset ExcludeList ${Ini[${excludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
		/echo Zone Exclusions: ${ExcludeList}
	} else {
		/varset ExcludeList ${Ini[${excludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
		/echo Zone Exclusions: ${ExcludeList}
	}
/return 

Sub xbot_initialize_exclusions
	/cecho \ao xbot_initialize_exclusions
	|-- [Settings] : Mob Exclusion List
	/declare    excludeINI 				string  outer   xiris_common/xiris_exclude.ini
	/call setExclusions
	
/return