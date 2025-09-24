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
	11.12.23	xiris 	Added shake down method/event for raid cash management
**|


#EVENT NotYourKill		"Someone is already looting that corpse."
#EVENT NotYourKill		"You may not loot this corpse at this time."
#Event LootTooFar		"Corpse too far away"

#EVENT LOOT_REFRESH		"#*#lootrefresh#*#"

#EVENT LOOT_ON			"#1# tells the group#*#'looton #2#'"
#EVENT LOOT_ON			"#1# tells you#*#'looton #2#'"
#EVENT LOOT_ON			"#1# tells the raid#*#looton #2#'"
#EVENT LOOT_ON 			"[ #1# #*#]#*#looton #2#"

#EVENT LOOT_OFF			"#1# tells the group#*#'lootoff#*#'"
#EVENT LOOT_OFF			"#1# tells you#*#'lootoff#*#'"
#EVENT LOOT_OFF			"#1# tells the raid#*#lootoff#*#'"
#EVENT LOOT_OFF 		"[ #1# #*#]#*#lootoff#*#"

#EVENT SELL_VENDOR		"#1# tells the group#*#'selltovendor#*#'"
#EVENT SELL_VENDOR		"#1# tells you#*#'selltovendor#*#'"
#EVENT SELL_VENDOR		"#1# tells the raid#*#selltovendor#*#'"
#EVENT SELL_VENDOR 		"[ #1# #*#]#*#selltovendor#*#"

#EVENT HIDE_CORPSES		"#1# tells the group#*#'DoHideCorpses#*#'"
#EVENT HIDE_CORPSES		"#1# tells you#*#'DoHideCorpses#*#'"
#EVENT HIDE_CORPSES		"#1# tells the raid#*#DoHideCorpses#*#'"
#EVENT HIDE_CORPSES 	"[ #1# #*#]#*#DoHideCorpses#*#"

#EVENT HIDE_LOOTED		"#1# tells the group#*#'DoHideLooted#*#'"
#EVENT HIDE_LOOTED		"#1# tells you#*#'DoHideLooted#*#'"
#EVENT HIDE_LOOTED		"#1# tells the raid#*#DoHideLooted#*#'"
#EVENT HIDE_LOOTED 		"[ #1# #*#]#*#DoHideLooted#*#"

#EVENT LOOT_SELF  		"#*#LootYourself#*#"

#EVENT SHAKE_DOWN_RAIDFOR_MONEY "#*#ShakeDown#*#"

#EVENT GIMME_MONEY		"#1# tells you#*#'GimmeMoney #2#'"
#EVENT GIMME_MONEY		"#1# tells you#*#'GimmeMoney #2#'"
#EVENT GIMME_MONEY		"#1# tells the raid#*#GimmeMoney #2#'"
#EVENT GIMME_MONEY 		"[ #1# #*#]#*#GimmeMoney #2#"

#EVENT HANDIN_ITEM		"#1# HandInItem #2#"

Sub InitEvents_Looting
	/call RegisterBackgroundEvents Events_Looting 9
/return

Sub Events_Looting
	/doevents LOOT_ON
	/doevents SELL_VENDOR
	/doevents HIDE_CORPSES
	/doevents HIDE_LOOTED
	/doevents LOOT_SELF
	/doevents SHAKE_DOWN_RAIDFOR_MONEY
	/doevents GIMME_MONEY
	/doevents HANDIN_ITEM
	|- Only need to run these events if you are looting
	/if (${bln_looting}) {	
		/doevents LOOT_REFRESH
		/doevents LOOT_OFF
	}
/return

Sub EVENT_LOOT_ON(string _line, string _chatSender, string _eventParams)
	/call EnableLooting "${_eventParams}"
	
/return 

Sub EVENT_LOOT_SELF
	/if (${bln_naked}) {
		/call LootSelf
	}
/return

Sub LootSelf 
	/echo \ao${Me.Name} is looting self
	/if (${Me.Class.ShortName.Equal[MNK]} || ${Me.Class.ShortName.Equal[NEC]} ) {
		/multiline ; /stand ; /delay 2s; /tar ${Me.Name}'s ; /corpse ; /loot ; /delay 2s ; /notify LootWnd LW_LootAllButton leftmouseup ; /delay 10s ; /memspellset default;
	} else {
		/multiline ; /delay 2s ; /target ${Me.Name}'s ; /corpse ; /loot ; /delay 2s ; /notify LootWnd LW_LootAllButton leftmouseup ; /delay 10s ; /memspellset default; /mqpause off
	}
/return

Sub EnableLooting(string _eventParams)
	| Announce
	/hidecorpse looted
	/varset bln_onlyQuest FALSE
	/if (${_eventParams.Arg[1, ].Equal[QUEST]}) /varset bln_onlyQuest TRUE
	/if (${_eventParams.Arg[2, ].Equal[RS]}) 	/varset str_lootChannel rs
	/if (${_eventParams.Arg[2, ].Equal[DGT]}) 	/varset str_lootChannel dgt
	/if (${_eventParams.Arg[2, ].Equal[G]}) 	/varset str_lootChannel gsay
	/varset bln_looting TRUE
	/if (${bln_onlyQuest}) {
		/docommand /${str_lootChannel} ${Me.Name} is now Looting QUEST items
	} else {
		/docommand /${str_lootChannel} ${Me.Name} is now Looting ALL items
	}
	/call ParseLootINI 
	/if (!${Plugin[MQ2Nav].Name.Length}) /plugin mq2nav
/return

Sub EVENT_LOOT_OFF
	/call DisableLooting
/return

Sub DisableLooting
	| Annnounce
	/docommand /${str_lootChannel} ${Me.Name} is no longer Looting
	/hidecorpse npc
	/varset bln_looting FALSE
/return 

Sub EVENT_LOOT_REFRESH
	/call RefreshLoot 
/return

Sub RefreshLoot
	| Announce
	/docommand /${str_lootChannel} ${Me.Name} is refreshing the loot ini file
	/call ParseLootINI xiris_common/xiris_loot_list.ini
/return

Sub EVENT_SELL_VENDOR
	/call SellVendor
/return


Sub EVENT_HIDE_CORPSES
	/hidecorpse npc
/return 

Sub EVENT_HIDE_LOOTED 
	/hidecorpse looted
/return

Sub EVENT_ITEM_LORE
	/hidecorpse looted
/return


Sub EVENT_NotYourKill
	/echo NotYourKill: skipping loot;
	/declare corpseName string 	local ${Spawn[id ${cache_targetID}]}
	/echo ${corpseName} ${cache_targetID}
/return LOOT_NO_RIGHTS


Sub EVENT_LootTooFar
	/echo LootTooFar: corpse too far away;
/return LOOT_TOO_FAR

Sub EVENT_LootTooFar
	/echo LootTooFar: corpse too far away;
/return LOOT_TOO_FAR




|-------------------------------------------------------------------------------- 
|SUB: Shake down the assholes in your raid for cash!
|-------------------------------------------------------------------------------- 
Sub Event_SHAKE_DOWN_RAIDFOR_MONEY(string _line, string _chatSender, string _eventParams)
	/if (${bln_engaged}) /return
	/declare i int local 0
	/declare pp int local 0
	/if (${Raid.Members} == 0) /return
	/declare startPlat = ${Me.Platinum}
	/declare endPlat = ${Me.Platinum}
	/declare totalPlat = 0
	/for i 2 to ${Raid.Members}
		/if (${Raid.Member[${i}].Name.Equal[${Me.Name}]}) /goto :next
		/dquery ${Raid.Member[${i}]} -q "Me.Platinum" -o pp
		/delay 20 ${DanNet.Query.Received}
		/echo \ao(${i}) \ag ${Raid.Member[${i}]}  \aw${pp}
		/if (${pp} <= 1000) /goto :next
		/dt ${Raid.Member[${i}]} GimmeMoney VIG
		/delay 10s
	:next
	/next i
	/varset endPlat ${Me.Platinum}
	/varcalc totalPlat ${endPlat} - ${startPlat}
	/docommand /${announceChannel} Raid has given me ${totalPlat}! I now have ${Me.Platinum}!
/return

Sub Event_GIMME_MONEY(string _line, string _chatSender, string _eventParams)
	/call TransferCash ${_chatSender} ${_eventParams}
/return 

Sub TransferCash(string _recipient, string _cashArgs)
	/if (${bln_engaged}) /return
	/echo \aoTransferCash \awTo=>\ag${_recipient} \awType=>\ag${_cashArgs}
	
	/declare myPlat		int	local 	${Me.Platinum}
	/declare givenPlat	int	local	0
	/declare moveCount	int local 	0
	| to select all plat  						/nomodkey /shiftkey /notify InventoryWindow IW_Money0 leftmouseup
	| to select some plat						/nomodkey /notify InventoryWindow IW_Money0 leftmouseup
	| to check quanity window open 				/if ${Window[QuantityWnd].Open}
	| to check what is in quantity window 		${Window[QuantityWnd].Child[QTYW_SliderInput].Text}
	| to change what is in the quantity window 	/notify QuantityWnd QTYW_slider newvalue ${newAmount}
	| to hit accept on quantity window 			/notify QuantityWnd QTYW_Accept_Button leftmouseup

	/if (${_cashArgs.Equal[ALL]}) {
		/varset givenPlat ${myPlat}
	} else {
		/if (${_cashArgs.Equal[VIG]}) {
			/echo vig!
			| VIG means... well its a joke, but leave self with 1000pp
			/varcalc givenPlat ${myPlat} - 1000
			/echo givenPlat ${givenPlat}
			/if (${givenPlat} <= 0) {
				/docommand /${announceChannel}  I don't have the VIG! 1000pp minimum remainder < (${myPlat}pp)
				/return
			}
		} else {
			/varset givenPlat ${_cashArgs}
		}
		| Can I afford this?
		/if (${givenPlat} > ${myPlat}) {
			/docommand /${announceChannel}  I don't have ${givenPlat}pp (${myPlat}pp)
			/return
		}
		
	}

	/echo \aoGiving \ag${givenPlat}pp \aoto \ag ${_recipient}
	/docommand /${announceChannel} Giving ${givenPlat}pp to ${_recipient}

	| Move to recipient
	/call TrueTarget ${Spawn[pc ${_recipient}].ID}
	/if (${Navigation.PathExists[spawn ${_recipient}]}) {
		/nav recordwaypoint navStart
		/nav spawn ${_recipient}

		:navRecipient
		/if (${moveCount} > 10) {
			/docommand /${announceChannel} Cannot move to ${_recipient}
			/return
		}
		/if (${Navigation.Active}) {
			/delay 10
			/varcalc moveCount ${moveCount}+1
			/goto :navRecipient
		}
	}
	/delay 5

	| Open Inventory and grab plat
	/if (!${Window[InventoryWindow].Open}) /nomodkey /keypress inventory 
	/delay 5

	| Select the amount of platinum
	/nomodkey /notify InventoryWindow IW_Money0 leftmouseup
	/delay 3
	/if (!${Window[QuantityWnd].Open}) {
		/docommand /${announceChannel} Failed to open QuanitityWnd
		/return
	} else {
		/notify QuantityWnd QTYW_slider newvalue ${givenPlat}
		/delay 2
		/notify QuantityWnd QTYW_Accept_Button leftmouseup
		/delay 2
		/click left target
		/delay 10
		/if (${Window[TradeWnd].Open}) /nomodkey /notify TradeWnd TRDW_Trade_Button leftmouseup
		|/docommand /${announceChannel} I have given ${_recipient} ${givenPlat}pp
		/delay 40
		/nav wp navStart
	}

/return

|-------------------------------------------------------------------------------- 
|SUB: Checking available corpses 
|-------------------------------------------------------------------------------- 
Sub chkLoot
	 /if (${bln_looting} && !${timer_check_loot} && ${SpawnCount[npc radius ${int_sitRadius} zradius 15]} == 0 && ${SpawnCount[npc corpse radius 85]} > 0 ) {
		|-- check to see if I even have room to loot
		/if (${Me.FreeInventory} <= 2) {
			/docommand /${str_lootChannel} out of bagspace
			/call DisableLooting
			/return
		}
		/squelch /hidecorpse looted
		|/echo count: ${SpawnCount[npc corpse radius ${int_lootRadius}]}
		/nav recordwaypoint lootStart
		/declare i int local
		:lootCheck
		/for i 1 to ${SpawnCount[npc corpse radius ${int_lootRadius}]}
			/if (${SpawnCount[npc radius ${int_sitRadius} zradius 15]} > 0) /return
			/call LootTarget ${NearestSpawn[${i},corpse radius ${int_lootRadius}].ID}
			/delay 5
		/next i
		
		/if (${SpawnCount[npc corpse radius 85]} > 0 ) /goto :lootCheck
		/varset timer_check_loot 10s
		| return to where I stated looting
		/nav wp lootStart
    }
	
/return


|-------------------------------------------------------------------------------- 
|SUB: Parsing the loot ini file (xiris_common/xiris_loot_list.ini)
|-------------------------------------------------------------------------------- 
Sub ParseLootINI 
	/echo \agParseLootINI \aw${lootINIFile}

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
		/varset item_loot ${Ini[${lootINIFile},Loot,Loot${n}]}
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

|-------------------------------------------------------------------------------- 
|SUB: NPC Corpse looting
|   - Validates against the parsed loot ini
|	- Loots if count < max, my class is a looter on the list
|-------------------------------------------------------------------------------- 
Sub LootTarget(int _corpseID)
	/echo \awLootTarget: \ag${_corpseID}
	/call TrueTarget ${_corpseID}

	
	/declare nav_count int local 0
	|-corpse validation
	:corpse_validation
	/if (${Target.Distance} > ${int_lootRadius} || ${Target.DistanceZ} > ${int_lootRadius}) {
		/echo Corpse too far, returning
		/goto :return
	}
	|-corpse movement
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
			/echo \arEmpty Corpse
			|/squelch /target clear
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
				/docommand /${str_lootChannel} ${Target.Name}(${cache_targetID}) has ${lootName} and I cannot loot it  ${reason}. (${cache_targetID})
				/goto :loot_gank
			}

			/if (${bln_includedByClass} && ${bln_includedByCharName}) {
				| My character can loot this provisionally. More checks to follow
				/echo \awI may be able to keep this item

				| Check to see if its lore and I already have one
				/if (${Corpse.Item[${n}].Lore} && ${FindItemCount[${lootName}]} >= ${int_numberToLoot}) {
					/echo \arItem is lore, and we have ${int_numberToLoot}
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
				/varcalc fCount ${FindItemCount[=${lootName}]} + 1
				/echo \agKeeping ${lootName} ${lootType} ${fCount} of ${int_numberToLoot}
				/docommand /${str_lootChannel} Keeping ${lootName} ${lootType} ${fCount} of ${int_numberToLoot} (${cache_targetID})
				/itemnotify loot${n} leftmouseup
				/delay 5 ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
				/if (${Cursor.ID}) {
					/delay 1
					/autoinv
				}
			} else {
				/docommand /${str_lootChannel} ${Target.Name}(${cache_targetID}) has ${lootName} and I cannot loot it  ${reason}. (${cache_targetID})
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
	/call OpenInventoryBags
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
		/if (${Me.Inventory[pack${i}]}) {
			


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
					/notify MerchantWnd MW_Sell_Button leftmouseup
					/delay 5
					/delay ${retryTimer} ${Window[QuantityWnd].Open} || !${Me.Inventory[pack${i}].Item[${e}]}
	
					| Close the quantity window
					/if (${Window[QuantityWnd].Open}) {
						/notify QuantityWnd QTYW_Accept_Button leftmouseup
						/delay ${retryTimer} !${Window[QuantityWnd].Open}
						/delay ${retryTimer} !${Me.Inventory[pack${i}]}
					}
	
					| If the item is still in my inventory
					/if (${Me.Inventory[pack${i}].Item[${e}]} && ${retryTimer}) {
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
SUB OpenInventoryBags
	| Open all bags in inventory.
	/declare i int local
	/declare retryTimer timer local
	/declare miscTimer timer local
	
	/for i 1 to 8
		
		/varset miscTimer 30
		/if (${Me.Inventory[pack${i}].Container}) {
			
			:openBag
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
/RETURN


|----------------------------------------|
|- Hands in Item X to NPC Y.			-|
|----------------------------------------|
| example : /dgt handinitem Stained Stone Chalice|Maroley
|----------------------------------------|

Sub EVENT_HANDIN_ITEM(_line, _chatSender, string _eventParams)
    /echo Handing In Item line:${_line}| sender:${_chatSender}| params:${_eventParams}
    /declare itemName   string   outer ${_eventParams.Arg[1,|]}
    /declare recipient string    outer ${_eventParams.Arg[2,|]}
    /docommand /${announceChannel} ${_eventParams} 
    /docommand /${announceChannel}  Giving ${itemName} to ${recipient}
    /if (${FindItemCount[=${itemName}]} <1) {
            /docommand /${announceChannel} cannot find ${itemName}
            /end
        } else {
            /call GiveItem
        }
/return 

Sub GiveItem
    /target npc ${recipient}
    
    /delay 2
    | Open Inventory to grab muffins
    /if (!${cache_targetID}) /return
    /if (${Target.Distance3D} > 400) {
        /docommand /${announceChannel}  ${recipient} is too far away!
        /end
    }
	/keypress open_inv_bags
	/delay 5

    /declare givenCount int local 0
    /declare packSlot   int local ${FindItem[=${itemName}].ItemSlot}
    /varcalc packSlot ${packSlot}
    /echo packSlot: ${packSlot}

    | item notify uses old pack numbers 1-8
    /declare notifyPackSlot int local 0
    /varcalc notifyPackSlot ${packSlot} - 22
    /echo \awnotifyPackSlot: \ag${notifyPackSlot}

    /declare notifyItemSlot int local ${FindItem[=${itemName}].ItemSlot2}
    /varcalc notifyItemSlot ${notifyItemSlot} + 1
    /echo \awnotifyItemSlot: \ag${notifyItemSlot}

    :moveLoop 
    /if (${Target.Distance3D} < 400) {
        | If you do not have mq2nav you could /stick here
        /nav target

        :distCheck 
        /if (${Target.Distance3D} > 10) {
            /delay 10
            /goto :distCheck
        }
    }

    :giveLoop
    /keypress esc 
    /delay 10
    /hail
    /target ${recipient}
    
    /delay 10
    /nomodkey /ctrl /itemnotify in pack${notifyPackSlot} ${notifyItemSlot} leftmouseup

    /delay 2
    /hail
    /click left target 

    /delay 2 

    /if (${Window[TradeWnd].Open}) /nomodkey /notify TradeWnd TRDW_Trade_Button leftmouseup
    /varcalc givenCount ${givenCount}+1
    /delay 5
   
    /echo trading
    /nomodkey /notify TradeWnd TRDW_Trade_Button leftmouseup
    /delay 10
    /autoinv
    /end
/return 



Sub xbot_initialize_looting

	/declare bln_looting			bool	outer 	${Ini[INI_CHAR,DEFAULT_SETTINGS,lootUse,FALSE]}
	/declare str_lootChannel		string  outer   1
	/declare bln_onlyQuest			bool 	outer	FALSE
	/declare bln_lootAttuneable		bool	outer 	TRUE
	/declare int_lootRadius			int 	outer 	100

	/declare list_lootedCorpses		string	outer
	/declare list_lootItemsForClass string  outer
	/declare timer_check_loot		timer   outer   0

	/declare lootINIFile			string  outer	xiris_common/xiris_loot_list.ini

	|-- turn on looting by default if the Bot's INI is set to true (unlikely)
	|/if (${bln_looting}) /call EnableLooting

	/echo \aoxbot_initialize_looting
	/call InitEvents_Looting 
/return
	