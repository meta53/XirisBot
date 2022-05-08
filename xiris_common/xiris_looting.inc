#EVENT NotYourKill		"Someone is already looting that corpse."
#EVENT NotYourKill		"You may not loot this corpse at this time."
#Event LootTooFar		"Corpse too far away"

#EVENT LOOT_REFRESH		"#*#lootrefresh#*#"

#EVENT LOOT_ON 			"<#1#> looton #2#"
#EVENT LOOT_ON 			"[#1#(msg)] looton #2#"
#EVENT LOOT_ON 			"#1# tells you, 'looton #2#'"
#EVENT LOOT_ON 			"#1# tells the group, 'looton #2#'"
#EVENT LOOT_ON 			"#1# you tell your party, 'looton #2#'"

#EVENT LOOT_OFF 		"<#1#> lootoff#*#"
#EVENT LOOT_OFF 		"[#1#(msg)] lootoff#*#"
#EVENT LOOT_OFF 		"#1# tells you, 'lootoff#*#'"
#EVENT LOOT_OFF 		"#1# tells the group, 'lootoff#*#'"
#EVENT LOOT_OFF 		"#1# you tell your party, 'lootoff#*#'"

#EVENT SELL_VENDOR 		"<#1#> selltovendor#*#"
#EVENT SELL_VENDOR 		"[#1#(msg)] selltovendor#*#"
#EVENT SELL_VENDOR 		"#1# tells you, 'selltovendor#*#'"
#EVENT SELL_VENDOR 		"#1# tells the group, 'selltovendor#*#'"
#EVENT SELL_VENDOR 		"#1# you tell your party, 'selltovendor#*#'"

Sub InitEvents_Looting
	/call RegisterBackgroundEvents Events_Looting 7
/return

Sub Events_Looting
	/doevents LOOT_ON
	/doevents SELL_VENDOR

	|- Only need to run these events if you are looting
	/if (${bln_lootAuto}) {	
		/doevents LOOT_REFRESH
		/doevents LOOT_OFF
	}
/return

Sub EVENT_LOOT_ON(_line, _chatSender, _eventParams)
	/call EnableLooting ${_eventParams}
	/doevents flush LOOT_ON
/return 

Sub EnableLooting(string _eventParams)
	/echo _eventParams: ${_eventParams}
	/call ParseLootINI 
	| Announce
	/bc ${Me.Name} is now Looting
	/hidecorpse looted
	/varset bln_lootAuto TRUE
	/varset bln_onlyQuest FALSE
	/if (!${Plugin[MQ2Nav].Name.Length}) /plugin mq2nav
/return

Sub EVENT_LOOT_OFF
	/call DisableLooting
	/doevents flush LOOT_OFF
/return

Sub DisableLooting
	| Annnounce
	/bc ${Me.Name} is no longer looting
	/hidecorpse npc
	/varset bln_lootAuto FALSE
/return 

Sub EVENT_LOOT_REFRESH
	/call RefreshLoot 
	/doevents flush LOOT_REFRESH
/return

Sub RefreshLoot
	/bc ${Me.Name} is refreshing the loot ini file
	/call ParseLootINI xiris_common/xiris_loot.ini
/return

Sub EVENT_SELL_VENDOR
	/call SellVendor
	/doevents flush SELL_VENDOR
/return


|-------------------------------------------------------------------------------- 
|SUB: Checking available corpses 
|-------------------------------------------------------------------------------- 
Sub chkLoot
	 /if (${bln_lootAuto} && ${SpawnCount[npc radius ${int_sitRadius} zradius 15]} == 0 && ${SpawnCount[npc corpse radius 85 noalert 5]} > 0 ) {
		|-- check to see if I even have room to loot
		/if (${Me.FreeInventory} <= 2) {
			/g out of bagspace
			/rs out of bagspace
			/call DisableLooting
			/return
		}
		|/echo count: ${SpawnCount[npc corpse radius ${int_lootRadius} noalert 5]}
		/declare i int local
		:lootCheck
		/for i 1 to ${SpawnCount[npc corpse radius ${int_lootRadius} noalert 5]}
			/if (${SpawnCount[npc radius ${int_sitRadius} zradius 15]} > 0) /return
			/call LootTarget ${NearestSpawn[${i},corpse radius ${int_lootRadius} noalert 5].ID}
			/delay 5
		/next i
		
		/if (${SpawnCount[npc corpse radius 85 noalert 5]} > 0 ) /goto :lootCheck
		/squelch /alert clear 5
    }
	
/return


Sub EVENT_NotYourKill
	/echo NotYourKill: skipping loot;
	/declare corpseName string 	local ${Spawn[id ${Target.ID}]}
	/echo ${corpseName} ${Target.ID}
	|/alert add 5 "${corpseName}" 
/return LOOT_NO_RIGHTS


Sub EVENT_LootTooFar
	/echo LootTooFar: corpse too far away;
/return LOOT_TOO_FAR




Sub xbot_initialize_looting
	/cecho \aoxbot_initialize_looting
	/call InitEvents_Looting 

	/declare bln_lootAuto			bool	outer 	${Ini[botINI,Settings,lootUse,FALSE]}
	/declare bln_onlyQuest			bool 	outer	FALSE
	/declare bln_lootAttuneable		bool	outer 	TRUE
	/declare int_lootRadius			int 	outer 	100

	/declare list_lootedCorpses		string	outer
	/declare list_lootItemsForClass string  outer

	/declare lootINIFile			string  outer	xiris_common/xiris_loot.ini


	|-- turn on looting by default if the Bot's INI is set to true (unlikely)
	|/if (${bln_lootAuto}) /call EnableLooting
/return
	


|-------------------------------------------------------------------------------- 
|SUB: Parsing the loot ini file (xiris_common/xiris_loot.ini)
|-------------------------------------------------------------------------------- 
Sub ParseLootINI 
	/cecho \agParseLootINI \aw${lootINIFile}

	|-- determine the count of the 3 major type QUEST|TRADE|VENDOR
	/declare int_total  int local ${Ini[${lootINIFile},Settings,Total]}
	/declare int_vendor	int	local ${Ini[${lootINIFile},Settings,Vendor]}
	/declare int_trade  int local ${Ini[${lootINIFile},Settings,Trade]}
	/declare int_quest	int	local ${Ini[${lootINIFile},Settings,Quest]}
	
	|-- kill the arrays if they already exist so we can rebuild them
	/if (${Defined[loot_ALL]})		/deletevar loot_ALL
	/if (${Defined[loot_VENDOR]}) 	/deletevar loot_VENDOR
	/if (${Defined[loot_TRADE]}) 	/deletevar loot_TRADE
	/if (${Defined[loot_QUEST]}) 	/deletevar loot_QUEST
	
	/echo total: ${int_total} vendor: ${int_vendor} trade: ${int_trade} quest: ${int_quest}

	|-- create new 2D arrays 
	|	[1] item
	|	[2] TYPE 
	|	[3] Count|* 
	|	[4] Class|*
	|   [5] CharExclusion|Xiris, etc or..
	|   [6] CharOnly|Xiris, etc or...
	/declare loot_ALL[${int_total},4]		string  outer
	/declare loot_VENDOR[${int_vendor},4]	string	outer
	/declare loot_TRADE[${int_trade},4]		string	outer
	/declare loot_QUEST[${int_quest},4]		string	outer
	
	/declare n int local 1
	/declare item_loot string local
	/declare vc int local 0
	/declare qc int local 0
	/declare tc int local 0
	
	
	|-- populate the 3 loot array types
	
	/for n 1 to ${int_total}
		/varset item_loot ${Ini[${lootINIFile},Loot,Loot${n}]}
		/if (${item_loot.Arg[2,|].Equal[VENDOR]}) {
			/varcalc vc ${vc}+1
			/varset loot_VENDOR[${vc},1] ${item_loot.Arg[1,|]} 
			/varset loot_VENDOR[${vc},2] ${item_loot.Arg[2,|]}
			/varset loot_VENDOR[${vc},3] ${item_loot.Arg[3,|]} 
			/varset loot_VENDOR[${vc},4] ${item_loot.Arg[4,|]} 
		} else /if (${item_loot.Arg[2,|].Equal[TRADE]}) {
			/varcalc tc ${tc}+1
			/varset loot_TRADE[${tc},1] ${item_loot.Arg[1,|]} 
			/varset loot_TRADE[${tc},2] ${item_loot.Arg[2,|]}
			/varset loot_TRADE[${tc},3] ${item_loot.Arg[3,|]} 
			/varset loot_TRADE[${tc},4] ${item_loot.Arg[4,|]} 
		} else /if (${item_loot.Arg[2,|].Equal[QUEST]} && !${item_loot.Arg[5,|].Find[${Me.Name}]}) {
			/varcalc qc ${qc}+1
			/varset loot_QUEST[${qc},1] ${item_loot.Arg[1,|]} 
			/varset loot_QUEST[${qc},2] ${item_loot.Arg[2,|]}
			/varset loot_QUEST[${qc},3] ${item_loot.Arg[3,|]} 
			/varset loot_QUEST[${qc},4] ${item_loot.Arg[4,|]} 
		}

		/varset loot_ALL[${n},1] ${item_loot.Arg[1,|]} 
		/varset loot_ALL[${n},2] ${item_loot.Arg[2,|]}
		/varset loot_ALL[${n},3] ${item_loot.Arg[3,|]} 
		/varset loot_ALL[${n},4] ${item_loot.Arg[4,|]} 
	/next n

	/echo loot_ALL: ${loot_ALL.Size}

/return

|-------------------------------------------------------------------------------- 
|SUB: NPC Corpse looting
|   - Validates against the parsed loot ini
|	- Loots if count < max, my class is a looter on the list
|-------------------------------------------------------------------------------- 
Sub LootTarget(int _corpseID)
	|/echo lootTarget
	/call TrueTarget ${_corpseID}
	
	|-corpse validation
	/if (${Target.Distance} > ${int_lootRadius} || ${Target.DistanceZ} > ${int_lootRadius}) {
		/echo Corpse too far, returning
		/goto :return
	}
	|-corpse movement
	/declare nav_count int local 0
	:corpse_nav
		/varcalc nav_count ${nav_count}+1
		/delay 5
		/if (${nav_count} > 30) {
			/echo Nav count too high returning
			/goto :return
		}
		/if ((${Target.Distance} > 10) && !${Navigation.Active}) /navigate target
		/if ((${Target.Distance} < 10) && !${Navigation.Active}) /goto :corpse_loot
		/if (${Navigation.Active}) /goto :corpse_nav

	|-corpse looting
	:corpse_loot
		/if (${Navigation.Active}) /navigate stop
		/loot
		/delay 10
		|-corpse loot attempt/validation
		/doevents LootTooFar
		/if (${Macro.Return.Equal[LOOT_TOO_FAR]}) /goto :corpse_nav
		/doevents NotYourKill
		/delay 20 ${Window[LootWnd].Open} || ${Macro.Return.Equal[SKIP_LOOT]}
		/if (${Macro.Return.Equal[LOOT_NO_RIGHTS]}) /goto :return
		/delay 10
		/if (!${Corpse.Items}) {
			/cecho \arEmpty Corpse
			|/squelch /target clear
			/notify LootWnd DoneButton leftmouseup
			/keypress esc
			/goto :return
		}

		|-corpse looting
		/declare n 			int 	local
		/declare i 			int 	local
		/declare keep  		bool 	local 	TRUE
		/declare lootName 	string 	local 
		/declare lootType 	string 	local
		/declare lootCount	int		local	1000
		/declare lootClass 	bool 	local 	FALSE

		/for n 1 to ${Corpse.Items}
			|-check to see if I want the item
			| [item][1] Name [2] type|* [3] count|* [4] class|*
			/if (!${bln_onlyQuest}) {
				/echo Slot ${n}: ${Corpse.Item[${n}].Name}
				/for i 1 to ${loot_ALL.Size}
					|/echo ${i} ${loot_ALL[${i},1]}
					/if (${loot_ALL[${i},1].Equal[${Corpse.Item[${n}].Name}]}) {

						/varset lootName  ${Corpse.Item[${n}].Name}
						/varset lootType  ${loot_ALL[${i},2]}

						/echo Item: ${Corpse.Item[${n}].Name} Type: ${lootType}

						|- count to keep
						/if (${loot_ALL[${i},3].NotEqual[*]}) /varset lootCount ${loot_ALL[${i},3]}

						|- classes to loot
						/if (${loot_ALL[${i},4].Find[${Me.Class.ShortName}]} ||${loot_ALL[${i},4].Equal[*]} ) /varset lootClass TRUE

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

					|- count to keep
					/if (${loot_QUEST[${i},3].NotEqual[*]}) /varset lootCount ${loot_QUEST[${i},3]}

					|- classes to loot
					/if (${loot_QUEST[${i},4].Find[${Me.Class.ShortName}]} ||${loot_ALL[${i},4].Equal[*]} ) /varset lootClass TRUE

					/goto :loot_validation
				} 
				/next i
			}
			/goto :nextItem
			


			|-check to see if item is lore
			:loot_validation
			/cecho Count in Inventory:\ay${FindItemCount[${lootName}]} Maximum: \ay${lootCount} Class Lootable?: \ay${lootClass}
			/if (${Corpse.Item[${n}].Lore} && ${FindItemCount[${lootName}]} == 1) {
				/cecho \arItem is lore, and we have more than 1
				/varset keep FALSE
			}
			/if (${FindItemCount[${lootName}]} > ${lootCount}) {
				/cecho \arWe already have more than we need of this item
				/beep
				/varset keep FALSE
			} 
			/if (!${lootClass}) {
				/cecho \arMy class cant keep this
				/varset keep FALSE
			} 
			:loot_gank
			
			/if (${keep}) {
				/cecho \agKeeping ${lootName} ${lootType} ${FindItemCount[${lootName}]} of ${lootCount}
				/rs Keeping ${lootName} ${lootType} ${FindItemCount[${lootName}]} of ${lootCount}
				/itemnotify loot${n} leftmouseup
				/delay 5 ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
				/if (${Cursor.ID}) {
					/delay 1
					/autoinv
				}
			}
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


Sub SellVendor
	/call ParseLootINI
	/echo Told to sell to vendor.
	|target merchant
	/tar npc merchant
	/nav target
	:merchant_run
		/delay 5
		/if (${Navigation.Active}) /goto  :merchant_run
	
	:merchant_open
		/click right target
		/delay 1
		/if (${Cursor.ID}) /autoinv
	:check_inventory
	/call openBags
	/declare i int local
	/for i 1 to ${loot_VENDOR.Size}
		/if (${FindItemCount[${loot_VENDOR[${i},1]}]}) {
			/call Sell "${loot_VENDOR[${i},1]}"
			/delay 10
		}

	:next_item
	/next i
/return

Sub Sell(string _item)
	| find the item
	/declare i int local
	/declare e int local 
	/declare retryTimer timer local 
	/declare packItem string local 

	/for i 1 to 8 
		/if (${Bool[${Me.Inventory[pack${i}]}]}) {
			


			| Look at each bag slot for items to sell
			/for e 1 to ${Me.Inventory[pack${i}].Container}
				| Get the loot setting for the item in pack slot 'i' slot 'e'
				/varset packItem ${Me.Inventory[pack${i}].Item[${e}]}
				/echo packItem: ${packItem}
				/if (${packItem.NotEqual[${_item}]}) {
					/echo ${packItem} is not the same as ${_item} next!
					/goto :next_item_in_pack
				}
				| Select the item to sell
				/varset retryTimer 30

				:retrySelect_Pack
				/echo |- sellItems -| :retrySelect_Pack
				/itemnotify in pack${i} ${e} leftmouseup
				/delay 10
				/delay ${retryTimer} ${Window[MerchantWnd].Child[MW_SelectedItemLabel].Text.Equal[${Me.Inventory[pack${i}].Item[${e}]}]}

				| If the item was not selected
				/if (!${Window[MerchantWnd].Child[MW_SelectedItemLabel].Text.Equal[${Me.Inventory[pack${i}].Item[${e}]}]}) {
					
					| If I have time, retry to select the item
					/if (${retryTimer}) {
						/goto :retrySelect_Pack
					} else {
						/echo ERROR: Failed to select [${Me.Inventory[pack${i}].Item[${e}]}], skipping.
					}
				} else {
					| Sell the item
					/varset retryTimer 30
					/echo Selling [${Me.Inventory[pack${i}].Item[${e}]}].
					
					:SellItem_Pack
					/if (${Debug} || ${Debug_Sell}) /echo |- sellItems -| :SellItem_Pack
					/notify MerchantWnd MW_Sell_Button leftmouseup
					/delay 5
					/delay ${retryTimer} ${Window[QuantityWnd].Open} || !${Bool[${Me.Inventory[pack${i}].Item[${e}]}]}
	
					| Close the quantity window
					/if (${Window[QuantityWnd].Open}) {
						/notify QuantityWnd QTYW_Accept_Button leftmouseup
						/delay ${retryTimer} !${Window[QuantityWnd].Open}
						/delay ${retryTimer} !${Bool[${Me.Inventory[pack${i}]}]}
					}
	
					| If the item is still in my inventory
					/if (${Bool[${Me.Inventory[pack${i}].Item[${e}]}]} && ${retryTimer}) {
						/goto :SellItem
					} else {
						/echo ERROR: Failed to sell [${Me.Inventory[pack${i}].Item[${e}]}], skipping.
					}
				}
			:next_item_in_pack
			/delay 5
			/next e
		}
	:next_pack
	/next i
/return


|----------------------------------------|
|- Opens all containers in inventory.	-|
|----------------------------------------|
SUB openBags
/if (${Debug} || ${Debug_Sell}) /echo |- openBags ==>

	| Open all bags in inventory.
	/declare i int local
	/declare retryTimer timer local
	/declare miscTimer timer local
	
	/for i 1 to 8
		
		/varset miscTimer 30
		/if (${Me.Inventory[pack${i}].Container}) {
			
			:openBag
			/if (${Debug} || ${Debug_Sell}) /echo |- openBags -| :openBag
		
			| Open container
			/if (!${Window[pack${i}].Open}) {
				/itemnotify pack${i} rightmouseup
				/delay 5 ${Window[pack${i}].Open} || ${Window[TradeSkillWnd].Open}
				/if (!${Window[pack${i}].Open} && !${Window[TradeSkillWnd].Open} && ${miscTimer}) /goto :openBag
				
				| Open tradeskill container.
				/if (${Window[TradeSkillWnd].Open}) {
					:openExperimentWnd
					/notify TradeSkillWnd ExperimentButton leftmouseup
					/delay 5 !${Window[TradeSkillWnd].Open}
					/if (${Window[TradeSkillWnd].Open} && ${miscTimer}) /goto :openExperimentWnd
				}
			}
		}
	
	/next i
	
/if (${Debug} || ${Debug_Sell}) /echo <== openBags -|
/RETURN