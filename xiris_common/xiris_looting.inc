#EVENT NotYourKill	"Someone is already looting that corpse."
#EVENT NotYourKill	"You may not loot this corpse at this time."
#EVENT CorpseEmpty	"You must first target a corpse to loot!"
#Event LootTooFar	"Corpse too far away"
#EVENT EnableLoot	"#*#looton#*#"
#EVENT DisableLoot	"#*#lootoff#*#"
#EVENT RefreshINI	"#*#lootrefresh#*#"
#EVENT LootYourself	"#*#lootyourself#*#"

Sub InitEvents_Looting
	/call RegisterBackgroundEvents Events_Looting 7
/return

Sub Events_Looting
	/doevents EnableLoot

	|- Only need to run these events if you are looting
	/if (${bln_lootAuto}) {
		/doevents NotYourKill
		/doevents CorpseEmpty
		/doevents LootTooFar	
		/doevents DisableLoot
		/doevents RefreshINI
		/doevents LOOTSELF
	}
/return

Sub EVENT_LootYourself
	/delay 1s
	/hidecorpse looted
	/tar ${Me.Name}'s
	/delay 1s
	/corpse
	/lootall
	/delay 10s
	/doevents flush LootYourself
/return

Sub EVENT_EnableLoot
	/call EnableLooting
	/doevents flush EnableLoot
/return

Sub EnableLooting 
	/call getLootINI xiris_common/xiris_loot.ini Loot
	/if (!${Defined[RV_LootArray]}) /echo No Loot Array Created...
	/bc ${Me.Name} is now Looting
	/hidecorpse looted
	/varset bln_lootAuto TRUE
	/if (!${Plugin[MQ2Nav].Name.Length}) /plugin mq2nav
/return


Sub EVENT_DisableLoot
	/call DisableLooting
/return

Sub DisableLooting
	/bc ${Me.Name} is no longer looting
	/hidecorpse npc
	/varset bln_lootAuto FALSE
/return 

Sub EVENT_RefreshINI
	/bc ${Me.Name} is refreshing the loot ini file
	/call getLootINI xiris_common/xiris_loot.ini Loot
/return

|-------------------------------------------------------------------------------- 
|SUB: Checking available corpses 
|-------------------------------------------------------------------------------- 
Sub chkLoot
	 /if (${bln_lootAuto} && ${SpawnCount[npc radius ${int_sitRadius} zradius 15]} == 0 && ${SpawnCount[npc corpse radius 85 noalert 5]} > 0 ) {
		|-- check to see if I even have room to loot
		/if (${Me.FreeInventory} <= 2) {
			/g out of bagspace
			/call DisableLooting
			/return
		}
		|/echo count: ${SpawnCount[npc corpse radius ${int_lootRadius} noalert 5]}
		/declare i int local
		:lootCheck
		/for i 1 to ${SpawnCount[npc corpse radius ${int_lootRadius} noalert 5]}
			/if (${SpawnCount[npc radius ${int_sitRadius} zradius 15]} > 0) /return
			/call doLootTGT ${NearestSpawn[${i},corpse radius ${int_lootRadius} noalert 5].ID}
			/if (${Macro.Return.Equal[JUST_LOOTED]}) {
				/delay 5
			}
		/next i
		
		/if (${SpawnCount[npc corpse radius 85 noalert 5]} > 0 ) /goto :lootCheck
		/squelch /alert clear 5
    }
	
	
/return

Sub EVENT_CorpseEmpty 
	/echo CorpseEmpty: Ignoring, because it... disapeared.
/return SKIP_LOOT


Sub EVENT_NotYourKill
	/echo NotYourKill: skipping loot;
	/declare corpseName string 	local ${Spawn[id ${Target.ID}]}
	/echo ${corpseName} ${Target.ID}
	|/alert add 5 "${corpseName}" 
/return SKIP_LOOT


Sub EVENT_LootTooFar
	/echo LootTooFar: corpse too far away;
/return MOVE_LOOT


|-------------------------------------------------------------------------------- 
|SUB: Looting a corpse ID
|-------------------------------------------------------------------------------- 
Sub doLootTGT(int corpseID)
	/declare corpseName 		string 	local ${Spawn[id ${corpseID}]}
	/declare lootSetting 		string 	local
	/declare linkList 			string 	local	
	/declare lootTotal 			int 	local -1
	/declare lootTotal_Timer 	timer 	local 15
	/declare j 					int 	local 1
	/declare lootItem			int		local 1
	
	| Check to see if corpseID belongs to an NPC (NPC corpses have _'s in the name).
	
	
		| Target the corpse, and move to it
		/call TrueTarget ${corpseID}
		/delay 10
		:move_Loot
		/if (${Target.Distance} > ${int_lootRadius}) /goto :skip_Loot
		/if ((${Target.Distance} > 10) && !${Navigation.Active}) {
			/navigate target
			/delay 10
		} 
		
		/if (${Navigation.Active}) {
			/delay 20
			/goto :move_Loot
		}
		
		| If corpseID is in range, loot it.
		/if (${Spawn[id ${corpseID}].Distance} < 10 && ${Spawn[id ${corpseID}].DistanceZ} < 10) {
			/loot
			
			| Check to see if the corpse is too far
			/doevents LootTooFar
			/delay 5
			/if (${Macro.Return.Equal[MOVE_LOOT]}) /goto :move_Loot
			
			
			| Check to see if the corpse is lootable
			/doevents NotYourKill
			/delay 20 ${Window[LootWnd].Open} || ${Macro.Return.Equal[SKIP_LOOT]}
			/if (${Macro.Return.Equal[SKIP_LOOT]}) /goto :skip_Loot
			
			
			| Count items on the corpse, loop until no new items are found.
			/if (!${Corpse.Items}) { 
				/echo Empty Corpse
				/target clear
				/notify LootWnd DoneButton leftmouseup 
				/return	
			}
			
			:autoxplootlag
			/if (${lootTotal}!=${Corpse.Items}) {
				/varset lootTotal ${Corpse.Items}
				/delay 5
				/if (${lootTotal_Timer}) /goto :autoxplootlag
			}
			
			| If there are items to loot, do it
			/if (${lootTotal}) {
				
				/for j 1 to ${lootTotal}
					
					/for lootItem 1 to ${RV_LootArray.Size} 
						
						|-- we want to look on the corpse and find the items we want if they exist  
						|/echo ${lootItem}. ${Corpse.Item[${j}].Name} ==? ${RV_LootArray[${lootItem}]}
						
						/if (${Corpse.Item[${j}].Name.Equal[${RV_LootArray[${lootItem}]}]}) {
						
							|-- If item is lore, check if/hidec it is in inventory
							/if (${Corpse.Item[${j}].Lore} && ${FindItemCount[${Corpse.Item[${j}].Name}]} < 1) {
								/g ${Corpse.Item[${j}].Name} is LORE item - I am looting this.
								/itemnotify loot${j} leftmouseup
								/delay 5 ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
								/if (${Cursor.ID}) {
									/delay 1
									/autoinv
								}
							} else /if (!${Corpse.Item[${j}].Lore}) {
								/g Keeping a ${Corpse.Item[${j}].Name}
								/itemnotify loot${j} leftmouseup
								/delay 5 ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
								/if (${Cursor.ID}) {
									/delay 1
									/autoinv
								}
	
							} else {
								/g ${Corpse.Item[${j}].Name} is LORE item, I already had it. (${Target.ID})
								/beep
								/beep
							}
						} 
						/if (${Corpse.Item[${j}].Attuneable} && ${bln_lootAttuneable} && ${Corpse.Item[${j}].Lore}) {
							/if (&& ${FindItemCount[${Corpse.Item[${j}].Name}]} < 1) {
								/g Keeping a ${Corpse.Item[${j}].Name}
								/itemnotify loot${j} leftmouseup
								/delay 5 ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
								/if (${Cursor.ID}) {
									/delay 1
									/autoinv
								}
							} else {
								/g ${Corpse.Item[${j}].Name} is LORE item, I already had it. (${Target.ID})
								/beep
								/beep
								/goto :nextItem
							}
							
						}
						
					/next lootItem
				:nextItem
				/next j
				/delay 1
				/notify LootWnd DoneButton leftmouseup

			}
		}
	/return JUST_LOOTED
	
	:SKIP_LOOT
	/target clear
	| Click 'Done' and return to group.
	/if (${Corpse.Open}) /notify LootWnd DoneButton leftmouseup 
/return


|-------------------------------------------------------------------------------- 
|SUB: Reading from an INI File for looting (PullLoot.ini)
|-------------------------------------------------------------------------------- 
Sub getLootINI(FileName) 
    /echo getLootINI FileName:${FileName}
	/echo Attempting to Read Information from ${FileName}... 
	/echo count ${Ini[${FileName},Settings,Count]}
	/declare nValues     int local  1 
	/declare nArray      int local  0 
	/declare KeySet      string local  ${Ini[${FileName}]} 

	| Count the entities enumerated under KeyOne
	:CounterLoop 
		/if (${Ini[${FileName},Settings,Count]}) { 
			/varcalc nValues ${Ini[${FileName},Settings,Count]}
			/echo setting up array with Count ${nValues}
			/goto :MakeArray 
		} 
		/varcalc nValues ${nValues}+1 
		/goto :CounterLoop 
		
	| Make the array	
	:MakeArray 
		/echo ${nValues} array created;
		/if (!${nValues}) /return 
		/if (${FileName.Equal[${FileName}]}&&${nValues}>0 && !${Defined[RV_LootArray]}) { 
			/echo Declaring Loot Array... 
			/declare RV_LootArray[${nValues}]  string outer 
		} 
		
		/if (${FileName.Equal[${FileName}]}&&${nValues}>0 && ${Defined[RV_LootArray]}) {
			/deletevar RV_LootArray
			/declare RV_LootArray[${nValues}] string outer
		}
		/for nArray 1 to ${nValues} 
			/if (${FileName.Equal[${FileName}]}) { 
				/varset RV_LootArray[${nArray}] ${Ini[${FileName},Loot,Loot${nArray}]} 
				/echo RV_LOOTArray[${nArray}] ${Ini[${FileName},Loot,Loot${nArray}]}
			} 
		/next nArray 
		/echo Information Read Successfully from ${FileName}... 
/return 

Sub xbot_initialize_looting
	/cecho \aoxbot_initialize_looting
	/call InitEvents_Looting 

	/declare bln_lootAuto			bool	outer ${Ini[botINI,Settings,lootUse,FALSE]}
	/declare bln_lootAttuneable		bool	outer 	TRUE
	/declare int_lootRadius			int 	outer 	100
	/declare list_lootedCorpses		string	outer
	/if (${bln_lootAuto}) /call EnableLooting
/return
	
	