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
**|


|**
 ----------------------------
 Pet Buff/Existence Checking
 ----------------------------
 Spawn a pet and buff it
 Called from utility sub
 Note: if this is the first time called, it will check immediately, afterwards it sets a 30s: timer_petBuffRecheck
 ----------------------------
**|

Sub chkPet
	
	|--Destroy your familiar if it is up. Hate them
	/if (${bln_engaged} && !${bln_hanvarEngaged}) /return
	/if (${Me.Pet.ID} && ${Me.Pet.Name.Find[familiar]}) /pet get lost
	/if (${Me.Invis} || ${Me.Moving} || ${timer_petBuffRecheck}) /return

	/declare hadPetOnInitialize bool local ${Bool[${Me.Pet.ID}]}
	|--Spawn your pet
	/call chkPetSpawn
	/call RunRaidEvents	
	
	|--Buff your pet
	/if (${Me.Class.ShortName.NotEqual[SHM]} &&  ${Bool[${Me.Pet.ID}]}) /call chkPetBuffs

	|--Weaponize your pet if its a new spawn
	/if (${Me.Class.ShortName.Equal[MAG]} && !${hadPetOnInitialize}) /call chkPetWeaps
	
	|--Shrink your pet
	/if (!${bln_petShrunk}) /call ShrinkPet
	
/return

Sub chkPetBuffs
	/if (${bln_engaged}) /return
	/if (${bln_petUseBuffs} && ${Me.Pet.ID} && !${timer_petBuffRecheck} && ${Me.Pet.Distance} < 100) {
		/declare i int local
		/for i 1 to ${int_petBuffTotal}  
			/if (!${Me.PetBuff[${petBuff_${i}}]}) {
				/call chkSpellMemorized "${petBuff_${i}}" TRUE ${petBuff_${i}_Gem}
				/call MQ2Cast "${petBuff_${i}}" 
			}
		/next i
		/memspellset ${str_spellset}
	}
	/if (${Target.ID}==${Me.Pet.ID}) /squelch /target clear
	/if (!${timer_petBuffRecheck})	 /varset timer_petBuffRecheck 30s

	
/return

|**
 ----------------------------
 Pet Buff/Existence Checking
 ----------------------------
 Spawn a pet no buffs for shaman pets!
 ----------------------------
**|

Sub chkPetSpawn

	|--Destroy your familiar if it is up. Hates them precious
	/if (${bln_engaged}) /return
	/if (${Me.Pet.ID} && ${Me.Pet.Name.Find[familiar]}) /pet get lost
	/if (${Me.Invis} || ${Me.Moving} || ${timer_petBuffRecheck}) /return

	|--Spawn your pet
	/if (!${Me.Pet.ID} && ${bln_petUse}) {
		/if ((${str_petReagent.Equal[NULL]}) || (${str_petReagent.NotEqual[NULL]} && ${FindItemCount[${str_petReagent}]} > 1)) {
			/if (!${Me.Standing}) /stand on
			/echo spawning pet ${str_petSpell} ${str_petGem}
			/if (${Me.AltAbility[Suspend Minion]}) /alt activate 176
			/delay 5s !${Me.Casting.ID}
			/delay 3s ${Me.Pet.ID}
			/if (!${Me.Pet.ID}) {
				/call chkSpellMemorized "${str_petSpell}" TRUE ${str_petGem}
				/call MQ2Cast "${str_petSpell}" ${str_petGem} 5s
				:castWait
					/delay 10
					/if (${Me.Casting.ID}) /goto :castWait
			} 
			/delay 5s
			
			/if (${Me.Pet.ID} && ${Me.AltAbility[Pet Discipline]}) /pet hold on
			/varset bln_petShrunk FALSE
			/delay 8s
		} else {
			/echo Cannot Summon Pet: Out of ${str_petReagent}
			/varset bln_petUse FALSE
		}
		
	}
	/if (${Target.ID}==${Me.Pet.ID}) /squelch /target clear
/return


Sub chkPetWeaps
	/if (${bln_engaged} || !${Me.Pet.ID} || ${bln_petWeaponized}) /goto :return

	|-- load 'petWeap' spell set
	/delay 3s
	/memspellset petweap
	/delay 5s

	|-- Belt
	/call SummonItemForPet "Summoned: ${str_petBelt}" "Summon ${str_petBelt}" ${Me.Pet.ID}
	|-- Weapon 1
	/call SummonItemForPet "Summoned: ${str_petWeap1}" "Summon ${str_petWeap1}" ${Me.Pet.ID}
	|-- Weapon 2
	/call SummonItemForPet "Summoned: ${str_petWeap2}" "Summon ${str_petWeap2}" ${Me.Pet.ID}
	
	/memspellset ${str_spellset}
	/autoinv
	/varset bln_petWeaponized TRUE

	:return
/return

Sub WeaponizePet(int _petID)

/return

Sub SummonItemForPet(string _itemName, string _spellName, int _petID)
	/echo \ag Summoning Item \ay ${_itemName} \ag with \ay${_spellName}\ag for \aw${Spawn[id ${_petID}].CleanName}(${_petID})
	/autoinv
	:summon_item
	/if (!${FindItemCount[=${petItem}]}) {
		| Pet item isn't in inventory, summon it
		/call chkSpellMemorized "${_spellName}" TRUE
		/call MQ2Cast "${_spellName}"
		/delay 10
	}

	/call TrueTarget ${_petID}

	:moveToPet
	/if (${Target.Distance} > 10) {
		/if (${Target.Distance} < 100) {
			/if (!${Navigation.Active}) /navigate target
		} else {
			/echo \arPet is too far away! Abort!
			/goto :return
		}
		/delay 10
		/goto :moveToPet
	} else {
		/if (${Navigation.Active}) /navigate stop
	}

	:checkCursor
	/if (!${Cursor.ID}) {
		| nothing on cursor, we need to find in packs
		/declare i int local
		/declare e int local 
		/declare retryTimer timer local 
		/declare petItem string local
		/declare packItem string local 

		/for i 1 to 8 
			/if (${Bool[${Me.Inventory[pack${i}]}]}) {
				| Look at each bag slot for item
				/for e 1 to ${Me.Inventory[pack${i}].Container}
					/varset packItem ${Me.Inventory[pack${i}].Item[${e}]}
					/itemnotify in pack${i} ${e} leftmouseup
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
	/if (${Cursor.ID}) {
		/echo Handing ${Cursor.Name} to Pet
		/click left target
		/delay 10
		/if (${Window[GiveWnd].Open}) /notify GiveWnd GVW_Give_Button leftmouseup 
		/delay 10
	}
/return


Sub ShrinkPet
	/if (!${Me.Pet.ID}) /return
	/if (${bln_petShrunk}) /return
	/if (${str_petShrinker.Equal[NULL]}) /return
	/declare sc int local 1
	/declare castArg string local
	/declare buffName 	string 	local
	/varset buffName  ${str_petShrinker.Token[1,|]}
	/varset castArg   ${str_petShrinker.Token[2,|]}

	:shrink
		/echo Shrinking: #${sc}: ${castArg}
		/if (${sc}>2) /goto :return
		/varcalc sc ${sc}+1
		/call MQ2Cast ${castArg}
		/goto :shrink
:return
	/varset bln_petShrunk TRUE

/return

Sub chkPetHeal

/return

|**
 ----------------------------
 Pet Attack  
 ----------------------------
 Sends the pet in
 ----------------------------
**|

Sub chkPetAttack
	/if (!${bln_engaged} || ${bln_petAttacking} || !${Bool[${Target.PctHPs}]} ) /return
	/if (${Me.Pet.ID} && !${Me.Pet.Combat} && ${Target.ID}==${int_targetID} && ${Target.PctHPs}<${int_petAttackHP} && ${Target.Distance}<=100) {
		/echo \agSiccing Pet on \ay${Target.Name}
		/varset bln_petAttacking TRUE
		/pet attack
	}
/return



Sub xbot_initialize_pets
	/if (${str_classArchetype.Equal[MELEE_END]}) /return
	/if (${Me.Class.ShortName.Equal[WAR]}) /return
	
	/echo \ao xbot_initialize_pets
	
	|--Pet Information
	/declare str_petReagent				string  outer	${Ini[INI_CHAR,Pet,str_petReagent,NULL]}
	/declare bln_petUse					bool	outer	${Ini[INI_CHAR,Pet,bln_petUse,TRUE]}
	/declare bln_petAttacking			bool	outer 	${Me.Pet.Combat}
	/declare int_petAttackHP			int		outer	${Ini[INI_CHAR,Pet,int_petAttackHP,99]}
	/declare bln_petDiscipline			bool	outer	${Ini[INI_CHAR,Pet,bln_petDiscipline,TRUE]}
	
	/declare str_petSpell				string	outer	${Ini[INI_CHAR,Pet,str_petSpell,Dark Assassin]}
	/declare str_petGem					string	outer	${Ini[INI_CHAR,Pet,str_petGem,gem11]}
	
	/declare bln_petUseBuffs			bool	outer	${Ini[INI_CHAR,Pet,bln_petUseBuffs,FALSE]}
	/declare int_petBuffTotal			int		outer   ${Ini[INI_CHAR,DOT,int_petBuffTotal,0]}
	/declare timer_petBuffRecheck		timer	outer	0

	/declare pb							int		local
	|--Pet Shrinker
	/declare str_petShrinker			string	outer  	${Ini[INI_CHAR,Pet,str_petShrinker,NULL]}
	/declare bln_petShrunk				bool	outer	FALSE
	
	|--Pet Buffs
	/for pb 1 to ${int_petBuffTotal} 
		/declare petBuff_${pb} 				string	outer	${Ini[INI_CHAR,Pet,petBuff_${pb}]}
		/declare petBuff_${pb}_Gem 			string	outer	${Ini[INI_CHAR,Pet,petBuff_${pb}_Gem]}
	/next pb
	
	|--Pet Weapons
	/declare str_petBelt				string	outer 	${Ini[INI_CHAR,Pet,str_petBelt,Girdle of Magi`Kot]}
	/declare str_petWeap1				string  outer   ${Ini[INI_CHAR,Pet,str_petWeap1,Blade of the Kedge]}
	/declare str_petWeap2				string  outer   ${Ini[INI_CHAR,Pet,str_petWeap2,Fist of Ixiblat]}
	/declare bln_petWeaponized			bool	outer	FALSE
	
	
/return