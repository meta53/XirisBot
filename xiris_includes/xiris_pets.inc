|**
----------------------------
xiris_pets.inc
----------------------------
This include contains logic for pet spawning/buffing/etc

Supported by all classes based on INI settings

______________________________________________
REVISION HISTORY
    08.01.19    xiris 	INITIAL REVISION
	06.29.22	xiris 	Changed the chkPet to only chkPetWeapons if its a new pet
	08.04.23	xiris 	Rewrote the INI parsing routines, and changed how pets are summoned. Should be faster now.
**|



Sub initEvents_Pets
/return 

Sub Events_Pets
/return 

|**
 ----------------------------
 Pet Routines Initialization
 ----------------------------
 Sets default variables, and initializes the event listeners
 ----------------------------
**|
Sub xbot_initialize_pets
	/if (!${Select[${Me.Class.ShortName},NEC,MAG,BST,SHM]}) /return
	/echo \n \apInitialize: \aoPets \aw-------------------------

	/call processPets
	/call initEvents_Pets
/return

Sub processPets

	/declare bln_pets_enabled	bool	outer	FALSE
	/declare bln_pet_heals		bool	outer	FALSE
	/declare bln_pet_buffs 		bool	outer	FALSE
	/declare bln_pet_shrink		bool	outer	FALSE
	/declare bln_pet_items 		bool	outer	FALSE

	/call processPetSettings
	/if (!${bln_pets_enabled})	/return

	/call processPetSummons
	/call processPetBuffs
	/call processPetHeals
	/call processPetShrink
	/call processPetItems

/return 


Sub processPetSettings 
	/echo \awprocessPets
	| There is only 1 pet ever defined so we don't have to worry about loops
	|; _enabled|_reagent|_usePetHold
	|settings=TRUE|NULL|TRUE|
	/if (${Ini[INI_CHAR,PET,settings].Arg[1,|]})  /varset bln_pets_enabled	TRUE
	/if (!${bln_pets_enabled}) /return

	/declare str_pet_reagent	string	outer	NULL 
	/declare bln_pet_hold 		bool	outer 	FALSE

	/if (${Ini[INI_CHAR,PET,settings].Arg[2,|].NotEqual[NULL]})	/varset str_pet_reagent	${Ini[INI_CHAR,PET,settings].Arg[2,|]}
    /if (${Ini[INI_CHAR,PET,settings].Arg[3,|].NotEqual[NULL]})	/varset bln_pet_hold	${Ini[INI_CHAR,PET,settings].Arg[3,|]}

	/echo \a-w    Settings Processed: \awEnabled:\ag${bln_pets_enabled} \awReagent:\ag${str_pet_reagent} \awHold:\ag${bln_pet_hold}

/return 

Sub processPetSummons
	/echo \awprocessPetSummons
	|; _pet|_enabled|_gem|_startHP|_keepAlive|_reagent
	|PET="Child of Water"|TRUE|3|99|TRUE|NULL|

	/declare bln_petSummoned	bool	outer	FALSE

	/declare PET_1				string	outer	${Ini[INI_CHAR,PET,PET].Arg[1,|]}
	/declare PET_1_enabled			bool	outer	${Ini[INI_CHAR,PET,PET].Arg[2,|]}
	/declare PET_1_Gem			int		outer	${Ini[INI_CHAR,PET,PET].Arg[3,|]}
	/declare PET_1_startHP		int		outer	${Ini[INI_CHAR,PET,PET].Arg[4,|]}
	/declare PET_1_KeepAlive	bool	outer	${Ini[INI_CHAR,PET,PET].Arg[5,|]}
	/declare PET_1_Reagent		string	outer	${Ini[INI_CHAR,PET,PET].Arg[6,|]}	
 
	/echo \ao        PET[\ag1\ao]: \awPet:\ag${PET_1} \awUse:\ag${PET_1_enabled} \awGem:\ag${PET_1_Gem} \awStartHP:\ag${PET_1_startHP} \awKeepAlive:\ag${PET_1_KeepAlive}
/return

Sub processPetBuffs
	/echo \awprocessPetBuffs

    /declare raw_petBuffs[5]			string 	local NULL
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL


	|Pet buffs
    /declare timer_check_petBuffs           timer   outer   0
    /declare int_petBuffs_total             int     outer   0
	/declare bln_petBuffs_enabled			bool	outer	${bln_pets_enabled}
	/declare bln_petBuffed					bool	outer	FALSE

    |; _buff|_enabled|_gem
	|BUFF_1="Elemental Fury"|TRUE|7|
	/for i 1 to ${raw_petBuffs.Size}
        | if the value of the entry ends with a quoted string, we need to wrap it all in quotes again.
        /varset test ${Ini[INI_CHAR,PET,BUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_petBuffs_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare PETBUFF_${i}		string	outer   ${test.Arg[1,|]}
		/declare PETBUFF_${i}_enabled	bool	outer   ${test.Arg[2,|]}
		/declare PETBUFF_${i}_gem	int		outer   ${test.Arg[3,|]}
        /echo \ao        PETBUFF[\ag${i}\ao]: \awBuff:\ag${PETBUFF_${i}} \awUse:\ag${PETBUFF_${i}_enabled} \awGem:\ag${PETBUFF_${i}_gem}
	/next i

	| No Buffs? Disable buffing
	/if (${int_petBuffs_total} == 0) /varset bln_petBuffs_enabled FALSE

/return 

Sub processPetHeals
	/echo \awprocessPetHeals

    /declare raw_petHeals[5]			string 	local NULL
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL

	|Pet Heals
    /declare timer_check_petHeal            timer   outer   0
    /declare int_petHeal_total              int     outer   0
	/declare bln_petHeals_enabled			bool	outer	${PET_1_KeepAlive}

	/if (${Ini[INI_CHAR,PET,PETHEAL_1].Arg[1,|].Equal[NULL]})  /multiline ;  /echo \a-w    Settings Processed: \awEnabled:\ag${bln_petHeals_enabled} ; /return
	

    |; _heal|_enabled|_gem|_minHP|_minMana
	|HEAL_1="Renewal Of Jerikor"|TRUE|50|40|
	/for i 1 to ${raw_petHeals.Size}
        | if the value of the entry ends with a quoted string, we need to wrap it all in quotes again.
        /varset test ${Ini[INI_CHAR,PET,HEAL_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_petHeal_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare PETHEAL_${i}			string	outer   ${test.Arg[1,|]}
		/declare PETHEAL_${i}_enabled		bool	outer   ${test.Arg[2,|]}
		/declare PETHEAL_${i}_gem		int		outer   ${test.Arg[3,|]}
		/declare PETHEAL_${i}_MinHP		int		outer   ${test.Arg[4,|]}
		/declare PETHEAL_${i}_minMana	int		outer   ${test.Arg[5,|]}
        /echo \ao        PETHEAL[\ag${i}\ao]: \awBuff:\ag${PETHEAL_${i}} \awUse:\ag${PETHEAL_${i}_enabled} \awGem:\ag${PETHEAL_${i}_gem} \awMinHP:\ag${PETHEAL_${i}_MinHP} \awMinMana:\ag${PETHEAL_${i}_minMana}
	/next i
/return 


Sub processPetShrink
	/echo \awprocessPetShrink
	| There is only 1 pet shrinker ever defined so we don't have to worry about loops
	|; _shrink|_enabled|_type|

	/declare bln_petShrink_enabled 		bool	outer	FALSE
	/declare int_petShrink_total 		int		outer	0
	/declare bln_petShrunk				bool	outer	FALSE

	/if (${Ini[INI_CHAR,PET,SHRINK_1].Arg[1,|].Equal[NULL]})  /multiline ;  /echo \a-w    Settings Processed: \awEnabled:\ag${bln_petShrink_enabled} ; /return

	/varset  bln_petShrink_enabled		TRUE
	/varset  int_petShrink_total 		1
	
	/declare PETSHRINK_1		string	outer	${Ini[INI_CHAR,PET,SHRINK_1].Arg[1,|]}
	/declare PETSHRINK_1_enabled	bool	outer	${Ini[INI_CHAR,PET,SHRINK_1].Arg[2,|]}
	/declare PETSHRINK_1_Type	int		outer	${Ini[INI_CHAR,PET,SHRINK_1].Arg[3,|]}

	/echo \ao        PETSHRINK[\ag1\ao]: \awShrink:\ag${PETSHRINK_1} \awUse:\ag${PETSHRINK_1_enabled} \awType:\ag${PETSHRINK_1_Type} \
/return 

Sub processPetItems
	/echo \awprocessPetItems

    /declare raw_petItems[5]			string 	local NULL
	/declare i						    int 	local 0
	/declare sz 					    int     local 0
    /declare test                       string  local NULL

	|Pet Items
    /declare timer_check_petItems           timer   outer   0
    /declare int_petItem_total              int     outer   0
	/declare bln_petItems_enabled			bool	outer	TRUE
	/declare bln_petItemized				bool 	outer 	FALSE

	/varset test ${Ini[INI_CHAR,PET,ITEM_1]}
	/if (${test.Arg[1,|].Equal[NULL]}) /multiline ; /varset bln_petItems_enabled FALSE ; /return

    |; _item|_type|_enabled|_gem|_summoner|_slot
	|ITEM_1="Summoned: Crystal Belt"|SPELL|TRUE|1|"Summon Crystal Belt"|WORN
	/for i 1 to ${raw_petItems.Size}
        | if the value of the entry ends with a quoted string, we need to wrap it all in quotes again.
        /varset test ${Ini[INI_CHAR,PET,ITEM_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_petItem_total ${sz}
            /return
        } 
        | Not null, set the values for the buffs
		/varcalc sz	${sz}+1
		/declare PETITEM_${i}			string	outer   ${test.Arg[1,|]}
		/declare PETITEM_${i}_type		bool	outer   ${test.Arg[2,|]}
		/declare PETITEM_${i}_enabled		bool	outer   ${test.Arg[3,|]}
		/declare PETITEM_${i}_gem		int		outer   ${test.Arg[4,|]}
		/declare PETITEM_${i}_Summoner	string	outer   ${test.Arg[5,|]}
		/declare PETITEM_${i}_Slot	    string	outer   ${test.Arg[6,|]}
        /echo \ao        PETITEM[\ag${i}\ao]: \awBuff:\ag${PETITEM_${i}} \awType:\ag${PETITEM_${i}_type}  \awUse:\ag${PETITEM_${i}_enabled} \awGem:\ag${PETITEM_${i}_gem} \awSummoner:\ag${PETITEM_${i}_Summoner}} \awSlot:\ag${PETITEM_${i}_Slot} 
	/next i
/return

Sub chkPet
	/if (!${bln_pets_enabled}) /return
	| This method calls the other methods based on bools when we set this up
	/if (${bln_engaged}) {
		/if (${bln_petHeals_enabled})	/call chkPetHeal
	} else {
		
		| Call the base determination/summon method
		/call chkPetStatus

		/if (${bln_petBuffs_enabled} && (!${bln_petBuffed} || !${timer_check_petBuffs}))		/call chkPetBuffs 
		/if (${bln_petShrink_enabled} && !${bln_petShrunk})		/call chkPetShrink
		/if (${bln_petItems_enabled} && !${bln_petItemized})	/call chkPetItems
	}
/return

Sub chkPetStatus
	/if (!${Pet.ID}) {
		| Check Reagent Count (if required)
		/if (${PET_1_Reagent.NotEqual[NULL]} && !${FindItemCount[${PET_1_Reagent}]}) {
			/echo \arCannot Summon Pet \ao Out of ${PET_1_Reagent}
			/varset bln_pets_enabled FALSE
			/return
		}
		| Summon Pet
		/call chkSpellMemorized "${PET_1}" TRUE ${PET_1_Gem}
		/call MQ2Cast "${PET_1}" ${PET_1_Gem} 50
		/delay 90 !${Me.Casting.ID}

		| Set status for new pet
		/varset bln_petBuffed	FALSE
		/varset bln_petShrunk	FALSE
		/varset bln_petItemized FALSE 

		| If we have pet hold enabled, use it
		/if (${bln_pet_hold} && ${Me.AltAbility[Pet Discipline]} && !${Pet.Hold}) /squelch /pet hold
	}
/return 

Sub chkPetAttack
	/if (!${bln_engaged} || !${Target.PctHPs} ) /return
	/if (${Pet.ID} && !${Pet.Combat} && ${Target.ID}==${int_targetID} && ${Target.Distance}<=100) /call chkPetAttack
/return

Sub chkPetBuffs
	/if (!${Pet.ID}) /return
	/declare i int local 0
	/for i 1 to ${int_petBuffs_total}
		| If no buff, or buff duration is under a minute, cast
		/if (!${Pet.Buff[${PETBUFF_${i}}]} || (${Pet.Buff[${PETBUFF_${i}}]} && ${Pet.BuffDuration[${PETBUFF_${i}}]} < 60000)) {
			/call chkSpellMemorized "${PETBUFF_${i}}" TRUE ${PETBUFF_${i}_gem}
			/call MQ2Cast "${PETBUFF_${i}}" ${PETBUFF_${i}_gem} 50
		}
	/next i
	
	/varset  bln_petBuffed  TRUE
	/varset  timer_check_petBuffs 3m
/return 

Sub chkPetHeal 
	/if (!${Pet.ID}) /return
/return 

Sub chkPetShrink 
	/if (!${Pet.ID}) /return
/return 

Sub chkPetItems
	/if (!${Pet.ID}) /return
	/if (!${bln_petItemized}) {
		| Count the pet weapons so we don't have to cast them if we dont need to
		/declare weaponCount int local 2
		/if (!${Pet.Equipment[offhand]}) /varcalc weaponCount ${weaponCount} - 1
		/if (!${Pet.Equipment[primary]}) /varcalc weaponCount ${weaponCount} - 1
		/if (${weaponCount} == 2) /return

		| Else just blast the pet items all on pet cause its a PITA to calculate need
		/declare i int local 0
		/for i 1 to ${int_petItem_total}
			/echo \a-wItem \ag${PETITEM_${i}} \a-wSummoner \ag${PETITEM_${i}_Summoner} 
			/call chkSpellMemorized "${PETITEM_${i}_Summoner}" TRUE ${PETITEM_${i}_gem}
			| Summon items and hand them to pet.
			/call MQ2Cast "${PETITEM_${i}_Summoner}" ${PETITEM_${i}_gem} 5s
			/delay 50 !${Me.Casting.ID}
			/if (${Pet.Distance} > 20 && ${Pet.Distance} < 100) /nav id ${Pet.ID}
			/delay 300 !${Navigation.Active}
			
			/if (${Pet.Distance} > 30) {
				/echo \aoCannot Reach Pet to Itemize!
				/varset bln_petItemized	TRUE
				/return
			} else {
				:checkCursor
				/if (!${Cursor.ID}) {
					| nothing on cursor, we need to find in packs
					/declare j int local
					/declare e int local 
					/declare retryTimer timer local 
					/declare petItem string local
					/declare packItem string local 
					/for j 1 to 8 
						/if (${Bool[${Me.Inventory[pack${j}]}]}) {
							| Look at each bag slot for item
							/for e 1 to ${Me.Inventory[pack${j}].Container}
								/varset packItem ${Me.Inventory[pack${j}].Item[${e}]}
								/itemnotify in pack${j} ${e} leftmouseup
								/delay 10
								/goto :give_itemToPet
							:next_item_in_pack
							/delay 5
							/next e
						}
					:next_pack
					/next i	
				}

				:give_itemToPet
				/call TrueTarget ${Pet.ID}
				/face fast
				/if (${Cursor.ID}) {
					/echo \a-wHanding \at${Cursor.Name} \a-wto Pet
					/click left target
					/delay 10
					/if (${Window[GiveWnd].Open}) /notify GiveWnd GVW_Give_Button leftmouseup 
					/delay 10
				}
			}
		/next i
	}
/return