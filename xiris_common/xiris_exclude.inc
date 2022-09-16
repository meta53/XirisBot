|**
	EXCLUSION LIST for SHAMAN/ENC/ETC that auto target mobs
	/if (!${ExcludeList.Find[${Spawn[${mobID}]}].Name}) /continue
**|



Sub setExclusions
	| Load Global Exclusions
	/declare ExcludeGlobal string  local ${Ini[${ExcludeINI},"Excludes.global","Exclude"]}
	/cecho \ayGlobal Exclusions: \ag${ExcludeGlobal}

	
	
	| Add zone specific Exclusions
	| Not found by zone so add it.
	/declare ExcludeZone string  local ${Ini[${ExcludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
	/if (${ExcludeZone.Equal[NULL]}) {
		/ini "${ExcludeZone}" "Excludes.${Zone.ShortName}" "Exclude" "|"
		/varset ExcludeZone ${Ini[${ExcludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
		/cecho \ayZone Exclusions: ${ExcludeZone}
	} else {
		/varset ExcludeZone ${Ini[${ExcludeINI},"Excludes.${Zone.ShortName}","Exclude"]}
		/cecho \ayZone Exclusions: ${ExcludeZone}
	}

	/varset ExcludeList ${ExcludeGlobal}|${ExcludeZone}
	/cecho \agExlcusionList: \aw${ExcludeList}
	
/return 

Sub xbot_initialize_exclusions
	/cecho \ao xbot_initialize_exclusions
	|-- [Settings] : Mob Exclusion List
	/declare 	ExcludeList string  outer   NULL
	/declare    ExcludeINI	string  outer   xiris_common/xiris_exclude.ini
	/call setExclusions
	
/return