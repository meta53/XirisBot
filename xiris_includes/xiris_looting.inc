|**
----------------------------
xiris_looting.inc
----------------------------
This includes the looting setup (INI READ)
and sub routines as necessary
______________________________________________
REVISION HISTORY
    03.24.16    xiris	INITIAL REVISION
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only
	07.29.22	xiris 	Updated to allow for 'beeping' if the item is lore and the looter already has it (useful for manual loots like mirrored mask)
**|




#EVENT Loot_Refresh     "#1# tells the group#*#'lootrefresh#*#'"
#EVENT Loot_Refresh		"#1# tells you#*#'lootrefresh#*#'"
#EVENT Loot_Refresh		"#1# tells the raid#*#lootrefresh#*#'"
#EVENT Loot_Refresh 	"[ #1# #*#]#*#lootrefresh#*#"

#EVENT Loot_ON			"#1# tells the group#*#'looton #2#'"
#EVENT Loot_ON			"#1# tells you#*#'looton #2#'"
#EVENT Loot_ON			"#1# tells the raid#*#looton #2#'"
#EVENT Loot_ON 			"[ #1# #*#]#*#looton #2#"

#EVENT Loot_OFF			"#1# tells the group#*#'lootoff#*#'"
#EVENT Loot_OFF			"#1# tells you#*#'lootoff#*#'"
#EVENT Loot_OFF			"#1# tells the raid#*#lootoff#*#'"
#EVENT Loot_OFF 		"[ #1# #*#]#*#lootoff#*#"

#EVENT Loot_Self        "#1# tells the group#*#'lootself#*#'"
#EVENT Loot_Self        "#1# tells you#*#'lootself#*#'"
#EVENT Loot_Self        "#1# tells the raid#*#lootself#*#'"
#EVENT Loot_Self 		"[ #1# #*#]#*#lootself#*#"

|-- UI Events
#EVENT NotYourKill		"Someone is already looting that corpse."
#EVENT NotYourKill		"You may not loot this corpse at this time."
#Event LootTooFar		"Corpse too far away"

Sub initEvents_Looting
    /call RegisterBackgroundEvents Events_Looting_Background
/return 

Sub Events_Looting_Background
    /if (!${bln_engaged} && ${bln_looting}) /call chkLoot
    /if (${bln_looting}) {
        /doevents Loot_OFF
        /doevents Loot_Refresh
    }
    /doevents Loot_ON
	/doevents Loot_Self
/return


Sub EVENT_Loot_ON(string _line, string _sender, string _eventParams)
    /echo loot on ${_eventParams}
    /doevents flush Loot_ON
    /call setLootConstraint ${_eventParams}
    /call setLootState TRUE
/return

Sub EVENT_Loot_OFF(string _line, string _sender, string _eventParams)
    /doevents flush Loot_OFF
    /call setLootState FALSE
/return 

Sub EVENT_Loot_Refresh(string _line, string _sender, string _eventParams)
    /doevents flush Loot_Refresh
    /call processLootList
/return

Sub EVENT_Loot_Self(string _line, string _sender, string _eventParams)
    /doevents flush Loot_Self
    /if (${bln_naked}) /call lootSelf
/return

Sub EVENT_NotYourKill
	/echo NotYourKill: skipping loot;
	/declare corpseName string 	local ${Spawn[id ${Target.ID}]}
	/echo ${corpseName} ${Target.ID}
/return LOOT_NO_RIGHTS

Sub EVENT_LootTooFar
	/echo LootTooFar: corpse too far away;
/return LOOT_TOO_FAR

Sub xbot_initialize_looting
    /echo \n \apInitialize: \aoLooting \aw-------------------------

    /declare    bln_looting         bool    outer   FALSE 
    /declare    str_lootChannel     string  outer   dgt
    /declare    str_lootConstraint  string  outer   ALL
    /declare    bln_lootAttuneable  bool    outer   TRUE 
    /declare    int_lootRadius      int     outer   100
    /declare    timer_check_loot    timer   outer   100

/return

Sub processLootList
	/echo \awprocessLootList \awINI_LOOT

	|-- determine the count of the 3 major type QUEST|TRADE|VENDOR
	/declare int_total  int local ${Ini[INI_LOOT,SETTINGS,Total]}
	/declare int_vendor	int	local ${Ini[INI_LOOT,SETTINGS,Vendor]}
	/declare int_trade  int local ${Ini[INI_LOOT,SETTINGS,Trade]}
	/declare int_quest	int	local ${Ini[INI_LOOT,SETTINGS,Quest]}
	
	|-- kill the arrays if they already exist so we can rebuild them
	/if (${Defined[loot_ALL]})		/deletevar loot_ALL
	/if (${Defined[loot_VENDOR]}) 	/deletevar loot_VENDOR
	/if (${Defined[loot_TRADE]}) 	/deletevar loot_TRADE
	/if (${Defined[loot_QUEST]}) 	/deletevar loot_QUEST
	
	/echo \awTotal Loot: \ag${int_total} \ayvendor: \ag${int_vendor} \aotrade: \ag${int_trade} \arquest: \ag${int_quest}

	|-- create new 2D arrays 
	|	[1] item
	|	[2] TYPE 
	|	[3] Count|* 
	|	[4] Class|*
	|   [5] CharExclusion|Xiris, etc or..
	|   [6] CharOnly|Xiris, etc or...
	|   [7] Beep|T,F
	/declare loot_ALL[${int_total},7]		string  outer
	/declare loot_VENDOR[${int_vendor},7]	string	outer
	/declare loot_TRADE[${int_trade},7]		string	outer
	/declare loot_QUEST[${int_quest},7]		string	outer
	
	/declare n int local 1
	/declare item_loot string local
	/declare vc int local 0
	/declare qc int local 0
	/declare tc int local 0
	
	
	|-- populate the 3 loot array types
	
	/for n 1 to ${int_total}
		/varset item_loot ${Ini[INI_LOOT,Loot,Loot${n}]}
		/if (${item_loot.Arg[2,|].Equal[VENDOR]}) {
			/varcalc vc ${vc}+1
			/varset loot_VENDOR[${vc},1] ${item_loot.Arg[1,|]} 
			/varset loot_VENDOR[${vc},2] ${item_loot.Arg[2,|]}
			/varset loot_VENDOR[${vc},3] ${item_loot.Arg[3,|]} 
			/varset loot_VENDOR[${vc},4] ${item_loot.Arg[4,|]} 
			/varset loot_VENDOR[${vc},5] ${item_loot.Arg[5,|]} 
			/varset loot_VENDOR[${vc},6] ${item_loot.Arg[6,|]} 
			/varset loot_VENDOR[${vc},7] ${item_loot.Arg[7,|]}
		} else /if (${item_loot.Arg[2,|].Equal[TRADE]}) {
			/varcalc tc ${tc}+1
			/varset loot_TRADE[${tc},1] ${item_loot.Arg[1,|]} 
			/varset loot_TRADE[${tc},2] ${item_loot.Arg[2,|]}
			/varset loot_TRADE[${tc},3] ${item_loot.Arg[3,|]} 
			/varset loot_TRADE[${tc},4] ${item_loot.Arg[4,|]} 
			/varset loot_TRADE[${tc},5] ${item_loot.Arg[5,|]} 
			/varset loot_TRADE[${tc},6] ${item_loot.Arg[6,|]} 
			/varset loot_TRADE[${tc},7] ${item_loot.Arg[7,|]} 
		} else /if (${item_loot.Arg[2,|].Equal[QUEST]} && !${item_loot.Arg[5,|].Find[${Me.Name}]}) {
			/varcalc qc ${qc}+1
			/varset loot_QUEST[${qc},1] ${item_loot.Arg[1,|]} 
			/varset loot_QUEST[${qc},2] ${item_loot.Arg[2,|]}
			/varset loot_QUEST[${qc},3] ${item_loot.Arg[3,|]} 
			/varset loot_QUEST[${qc},4] ${item_loot.Arg[4,|]} 
			/varset loot_QUEST[${qc},5] ${item_loot.Arg[5,|]} 
			/varset loot_QUEST[${qc},6] ${item_loot.Arg[6,|]} 
			/varset loot_QUEST[${qc},7] ${item_loot.Arg[7,|]} 
		}

		/varset loot_ALL[${n},1] ${item_loot.Arg[1,|]} 
		/varset loot_ALL[${n},2] ${item_loot.Arg[2,|]}
		/varset loot_ALL[${n},3] ${item_loot.Arg[3,|]} 
		/varset loot_ALL[${n},4] ${item_loot.Arg[4,|]} 
		/varset loot_ALL[${n},5] ${item_loot.Arg[5,|]} 
		/varset loot_ALL[${n},6] ${item_loot.Arg[6,|]} 
		/varset loot_ALL[${n},7] ${item_loot.Arg[7,|]} 
	/next n
/return 


Sub chkLoot
    /if (${bln_looting} && !${timer_check_loot} && ${SpawnCount[npc radius ${int_sitRadius} zradius 15]} == 0 && ${SpawnCount[npc corpse radius 85]} > 0 ) {
		|-- check to see if I even have room to loot
		/if (${Me.FreeInventory} <= 2) {
			/docommand /${str_lootChannel} out of bagspace
			/call setLootState FALSE
			/return
		}
		/squelch /hidecorpse looted
		/nav recordwaypoint lootStart
		/declare i int local
		:lootCheck
		/for i 1 to ${SpawnCount[npc corpse radius ${int_lootRadius}]}
			/if (${SpawnCount[npc radius ${int_sitRadius} zradius 15]} > 0) /return
			/call lootTarget ${NearestSpawn[${i},corpse radius ${int_lootRadius}].ID}
			/delay 10
		/next i
		
		/if (${SpawnCount[npc corpse radius ${int_lootRadius}]} > 0 ) /goto :lootCheck
		/varset timer_check_loot 100
		| return to where I stated looting
		/nav wp lootStart
    }
/return


Sub setLootAnnouce(string _channel)
    /varset str_lootChannel ${_channel}
/return 

Sub setLootConstraint(string _constraint)
    /varset str_lootConstraint ${_constraint}
/return

Sub setLootState(bool _enabled)
    /varset bln_looting ${_enabled}
    /if (${_enabled}) /call processLootList
    /docommand /${str_lootChannel} ${Me.Name} is now Looting ${str_lootConstraint} items
    
/return 

Sub lootSelf 
    /if (!${Me.Standing}) /stand  
    /multiline /delay 20 ; /tar ${Me.Name}'s ; /corpse ; /loot ; /delay 2s ; /notify LootWnd LW_LootAllButton leftmouseup ; /delay 100 ; 
    /if (${Select[${Me.Class.ShortName},BRD,BST,CLR,DRU,ENC,MAG,NEC,PAL,SHM,SHD]}) /memspellset ${str_casting_spellSet}
/return

Sub lootTarget(int _id)
	/echo \awLootTarget: \ag${_id}
	/call TrueTarget ${_id}

	/declare nav_count int local 0
	|-corpse validation
	:corpse_validation
	/if (${Target.Distance} > ${int_lootRadius} || ${Target.DistanceZ} > ${int_lootRadius}) {
		/echo Corpse too far, returning
		/goto :return
	}
	|-corpse movement
    :corpse_nav_start 
    /varset nav_count 0
	:corpse_nav
		/varcalc nav_count ${nav_count}+1
		/delay 5
		/if (${nav_count} > 30 || (${Target.Distance} > ${int_lootRadius})) {
			/echo Nav count too high or, this ended up being too far returning
			/goto :return
		}
		/if ((${Target.Distance} > 10) && !${Navigation.Active}) /navigate target
		/if ((${Target.Distance} < 10) && !${Navigation.Active}) /goto :corpse_loot
		/if (${Navigation.Active}) /goto :corpse_validation

	|-corpse looting
	:corpse_loot
		/if (${Navigation.Active}) /navigate stop
        /delay 5
		/loot
		/delay 10
		|-corpse loot attempt/validation
		/doevents LootTooFar
		/if (${Macro.Return.Equal[LOOT_TOO_FAR]}) /goto :corpse_nav_start
		/doevents NotYourKill
		/delay 20 ${Window[LootWnd].Open} || ${Macro.Return.Equal[SKIP_LOOT]}
		/if (${Macro.Return.Equal[LOOT_NO_RIGHTS]}) /goto :return
		/delay 10
		/if (!${Corpse.Items}) {
			/echo \arEmpty Corpse
			/notify LootWnd DoneButton leftmouseup
			/keypress esc
			/goto :return
		}

		|-corpse looting
		/declare n 						int 	local
		/declare i 						int 	local
		/declare keep  					bool 	local 	TRUE
		/declare lootName 				string 	local 
		/declare lootType 				string 	local
		/declare int_numberToLoot		int		local	1000
		/declare bln_includedByClass	bool 	local 	TRUE    | my class can loot
		/declare bln_excludedByCharName	bool    local   TRUE	| do not loot on this character [if found, dont loot]
		/declare bln_includedByCharName	bool	local	FALSE	| loot only on this character
		/declare bln_useBeep			bool    local   FALSE 	| should I beep if I can't loot this item?
		/declare reason					string  local   <no reason>
		/declare fCount 				int  	local   0

		/for n 1 to ${Corpse.Items}
			|-check to see if I want the item
			| [item][1] Name [2] type|* [3] count|* [4] class|*
			/varset bln_includedByClass 	FALSE
			/varset bln_excludedByCharName 	FALSE 
			/varset bln_includedByCharName	TRUE
			/echo Slot ${n}: ${Corpse.Item[${n}].Name}
			/if (!${bln_onlyQuest}) {
				/for i 1 to ${loot_ALL.Size[1]}
					|/echo ${i} ${loot_ALL[${i},1]}
					/if (${loot_ALL[${i},1].Equal[${Corpse.Item[${n}].Name}]}) {

						/varset lootName  ${Corpse.Item[${n}].Name}
						/varset lootType  ${loot_ALL[${i},2]}
						/varset bln_useBeep  ${If[${loot_ALL[${i},7].Find[T]}, TRUE,FALSE]}
						/echo Item: ${lootName} Type: ${lootType} Beep: ${bln_useBeep}

						|- count to keep
						/if (${loot_ALL[${i},3].NotEqual[*]}) /varset int_numberToLoot ${loot_ALL[${i},3]}

						|- classes to loot
						/if (${loot_ALL[${i},4].Find[${Me.Class.ShortName}]} || ${loot_ALL[${i},4].Equal[*]} ) /varset bln_includedByClass TRUE

						|- characters that shouldn't loot
						/if (${loot_ALL[${i},5].Find[${Me.Name}]}) /varset bln_excludedByCharName TRUE

						|- characters that should loot
						/if (!${bln_excludedByCharName} && (${loot_ALL[${i},6].Find[${Me.Name}]} || ${loot_ALL[${i},6].Equal[*]})) /varset bln_includedByCharName TRUE

						/goto :loot_validation
					} 
				/next i
			} else {
				|----------------------
				|  Only items flagged QUEST in the INI will be validated as loot
 				|----------------------
				/for i 1 to ${loot_QUEST.Size}
					/if (${loot_QUEST[${i},1].Equal[${Corpse.Item[${n}].Name}]}) {

						/varset lootName  ${Corpse.Item[${n}].Name}
						/varset lootType  ${loot_QUEST[${i},2]}
						/varset bln_useBeep  ${If[${loot_QUEST[${i},7].Find[T]}, TRUE,FALSE]}
						/echo Item: ${lootName} Type: ${lootType} Beep: ${bln_useBeep}

						|- count to keep
						/if (${loot_QUEST[${i},3].NotEqual[*]}) /varset int_numberToLoot ${loot_QUEST[${i},3]}

						|- classes to loot
						/if (${loot_QUEST[${i},4].Find[${Me.Class.ShortName}]} ||${loot_QUEST[${i},4].Equal[*]} ) /varset bln_includedByClass TRUE

						|- characters that shouldn't loot
						/if (${loot_QUEST[${i},5].Find[${Me.Name}]}) /varset bln_excludedByCharName TRUE

						|- characters that should loot
						/if (!${bln_excludedByCharName} && (${loot_QUEST[${i},6].Find[${Me.Name}]} || ${loot_QUEST[${i},6].Equal[*]})) /varset bln_includedByCharName TRUE
						
						/goto :loot_validation
					} 
				/next i
			}
			/goto :nextItem
			


			|-check to see if item is lore
			:loot_validation
			/echo ${lootName} \awCount in Inventory: \ay${FindItemCount[${lootName}]} \awMaximum: \ay${int_numberToLoot} \awClass Lootable?: \ay${bln_includedByClass} \awExcluded by Name: \ay${bln_excludedByCharName} \awLoot By Me|*|Only? \ay${bln_includedByCharName} \awBeep? \ay${bln_useBeep}
			
			|- My character name is in the exclusion list
			/if (${bln_excludedByCharName} || !${bln_includedByClass}) {
				/echo \arI shouldn't keep this (ex name: ${bln_excludedByCharName}, class: !${bln_includedByClass})
				/varset keep FALSE
				/varset reason EXCLUDED CHARACTER||CLASS
				/goto :loot_gank
			}

			/if (${bln_includedByClass} && ${bln_includedByCharName}) {
				| My character can loot this provisionally. More checks to follow
				/echo \awI may be able to keep this item

				| Check to see if its lore and I already have one
				/if (${Corpse.Item[${n}].Lore} && ${FindItemCount[${lootName}]} == 1) {
					/echo \arItem is lore, and we have 1
					/varset keep FALSE
					/varset reason LORE
					/goto :loot_gank
				} else /if (${FindItemCount[${lootName}]} >= ${int_numberToLoot}) {
					/echo \arI already have more than what I need of this item. (${FindItemCount[${lootName}]} >= ${int_numberToLoot})
					/varset keep FALSE
					/varset reason LOOTCOUNT >= ${int_numberToLoot}
					/goto :loot_gank
				} else {
					/echo \agI should keep this. All checks passed.
					/varset keep TRUE
					/goto :loot_gank
				}
			}


		

			:loot_gank
			/if (${keep}) {
				/varcalc fCount ${FindItemCount[${lootName}]} + 1
				/echo \agKeeping ${lootName} ${lootType} ${fCount} of ${int_numberToLoot}
				/docommand /${str_lootChannel} Keeping ${lootName} ${lootType} ${fCount} of ${int_numberToLoot}
				/itemnotify loot${n} leftmouseup
				/delay 5 ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
				/if (${Cursor.ID}) {
					/delay 1
					/autoinv
				}
			} else {
				/docommand /${str_lootChannel} ${Target.Name}(${Target.ID}) has ${lootName} and I cannot loot it  ${reason}.
			}
			/if (${bln_useBeep}) /beep
			:nextItem
			/next n

	:return
	|/target clear
	/if (${Corpse.Open}) /notify LootWnd DoneButton leftmouseup 
	/keypress esc
	/if (${Cursor.ID}) {
		/delay 1
		/autoinv
	}
/return    
