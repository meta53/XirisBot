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
	07.07.24	xiris	Changed pet buffing/itemizing to be more modern
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
	/if (${Pet.ID} && ${Me.Pet.Name.Find[familiar]}) /pet get lost
	/if (${Me.Invis} || ${Me.Moving} || ${timer_petBuffRecheck}) /return

	/declare hadPetOnInitialize bool local ${Pet.ID}
	|--Spawn your pet
	/call chkPetSpawn
	/call RunRaidEvents	
	
	|--Buff your pet
	/if (${Me.Class.ShortName.NotEqual[SHM]} && ${Pet.ID}) /call chkPetBuffs

	|--Weaponize your pet if its a new spawn
	/if (${Me.Class.ShortName.Equal[MAG]} && !${hadPetOnInitialize}) /call chkPetEquipment
	
	|--Shrink your pet
	/if (!${bln_petShrunk}) /call ShrinkPet
	
/return

|**
 ----------------------------
 Pet Buff/Existence Checking
 ----------------------------
 Check your pet for buffs
 ----------------------------
**|

Sub chkPetBuffs
	/if (${bln_engaged} || !${PETBUFF_Total}) /return
	/if (${bln_petUseBuffs} && ${Pet.ID} && !${timer_petBuffRecheck} && ${Me.Pet.Distance} < 100) {
		/declare i	 int local
		/declare st string local
		/for i 1 to ${PETBUFF_Total}  
	|/echo \ayPet Buffs: \awChecking \aw"${PETBUFF_${i}}"  \agHas:\aw${Me.PetBuff[${PETBUFF_${i}}]}
			/if (!${Me.PetBuff[${PETBUFF_${i}}]}) {
				/varset st ${PETBUFF_${i}_Type}
				/if (${st.Equal[ITEM]} && ${FindItem[${PETBUFF_${i}_Summoner}].TimerReady} == 0) {
					/echo \aw     Casting: \aw${PETBUFF_${i}_Summoner}
					/call MQ2Cast "${PETBUFF_${i}_Summoner}" item
					|/delay 10s ${Cursor.ID}	
				} else /if (${st.Equal[SPELL]}) {
					/echo \aw     Casting: \aw${PETBUFF_${i}}
					/call MQ2Cast  "${PETBUFF_${i}_Summoner}"
					|/delay 10s ${Cursor.ID}
				} 
			}
		/next i
		/memspellset ${str_spellset}
	}
	/if (${Target.ID}==${Pet.ID}) /squelch /target clear
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
	/if (${Pet.ID} && ${Me.Pet.Name.Find[familiar]}) /pet get lost
	/if (${Me.Invis} || ${Me.Moving} || ${timer_petBuffRecheck}) /return

	|--Spawn your pet
	/if (!${Pet.ID} && ${bln_petUse}) {
		/if ((${str_petReagent.Equal[NULL]}) || (${str_petReagent.NotEqual[NULL]} && ${FindItemCount[${str_petReagent}]} > 1)) {
			/if (!${Me.Standing}) /stand on
			/echo \aw     Spawning pet \ag${str_petSpell} \ayGem${str_petGem}
			/if (${Me.AltAbility[Suspend Minion]}) /alt activate 176
			/delay 5s !${Me.Casting.ID}
			/delay 3s ${Pet.ID}
			/if (!${Pet.ID}) {
				/call chkSpellMemorized "${str_petSpell}" TRUE ${str_petGem}
				/call MQ2Cast "${str_petSpell}" ${str_petGem} 5s
				:castWait
					/delay 10
					/if (${Me.Casting.ID}) /goto :castWait
			} 
			/delay 5s ${Pet.ID}
			
			/if (${Pet.ID} && ${Me.AltAbility[Pet Discipline]}) /pet hold on
			/varset bln_petShrunk FALSE
		} else {
			/echo Cannot Summon Pet: Out of ${str_petReagent}
			/varset bln_petUse FALSE
		}
		
	}
	/if (${Target.ID}==${Pet.ID}) /squelch /target clear
/return


Sub chkPetEquipment
	/if (${bln_engaged} || !${Pet.ID} || ${bln_petItemized}) /goto :return
	/echo \ag    Weaponizing Pet => \aw${PETITEM_Total}

	|-- load 'petWeap' spell set
	/delay 3s
	/memspellset petweap
	/delay 5s

	/declare i int local 0
	/for i 1 to ${PETITEM_Total}
		/echo "${PETITEM_${i}}" "${PETITEM_${i}_Type}" "${PETITEM_${i}_Summoner}"  
		/if (${PETITEM_${i}_Use})	/call SummonItemForPet "${PETITEM_${i}}" "${PETITEM_${i}_Type}" "${PETITEM_${i}_Summoner}"  
	/next i

	| |-- Belt
	| /call SummonItemForPet "Summoned: ${str_petBelt}" "Summon ${str_petBelt}" ${Pet.ID}
	| |-- Weapon 1
	| /call SummonItemForPet "Summoned: ${str_petWeap1}" "Summon ${str_petWeap1}" ${Pet.ID}
	| |-- Weapon 2
	| /call SummonItemForPet "Summoned: ${str_petWeap2}" "Summon ${str_petWeap2}" ${Pet.ID}
	
	/memspellset ${str_spellset}
	/autoinv
	/varset bln_petItemized TRUE
	/echo \agPet has been weaponized!
	:return
/return

Sub SummonItemForPet(string _item, string _type, string _summoner)
	/if (${SpawnCount[npc radius ${int_campRadius}]} > 0 && ${Zone.ShortName.NotEqual[thundercrest]}  && ${Zone.ShortName.NotEqual[guildhall]}  && ${Zone.ShortName.NotEqual[guildlobby]}) {
		/echo \a-rReturning due to spawn
		/return
	} 

	| Look at the pet item index, and summon item, handing it to pet as we go
	/echo \ag    Summoning Item \ay ${_item} \ag type \ay${_type}\ag with \ay${_summoner}\ag \ag  for \aw${Spawn[id ${Pet.ID}].CleanName}(${Pet.ID})
	/autoinv

	/if (${_type.Equal[ITEM]}) {
		/call MQ2Cast "${_summoner}" item
		|/delay 10s ${Cursor.ID}	
	} else /if (${_type.Equal[SPELL]}) {
		/call MQ2Cast "${_summoner}" 
		|/delay 10s ${Cursor.ID}
	}

	:moveToPet
	/call TrueTarget ${Pet.ID}
	/call RunAllEvents

	/if (${Cursor.ID}) {
		/if (${Target.Distance} > 10) {
			/if (${Target.Distance} < 100) {
				/if (!${Navigation.Active}) /navigate spawn id ${Pet.ID}
				/delay 10
				/goto :moveToPet
			} else {
				/echo \arPet is too far away! Abort!
				/return
			}
		}
	} else {
		/echo \aw Summoning Item Failed, returning
		/return
	}

	:give_itemToPet
	/if (${Cursor.ID}) {
		/echo Handing ${Cursor.Name} to Pet
		/click left target
		/delay 10
		/if (${Window[GiveWnd].Open}) /notify GiveWnd GVW_Give_Button leftmouseup 
		/delay 10
	}
:return
/return


Sub ShrinkPet
	/if (!${Pet.ID}) /return
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
	/if (!${bln_engaged} || !${Target.PctHPs} ) /return
	/if (${Pet.ID} && !${Me.Pet.Combat} && ${Target.ID}==${int_targetID} && ${Target.PctHPs}<${int_petAttackHP} && ${Target.Distance}<=100) 	/squelch /pet attack
/return


Sub processPetItems
	/declare PETITEM_Total				int		outer	${Ini[INI_CHAR,PET,PETITEM_Total,0]}	

	/if (${PETITEM_Total}) {
		/declare i int local 0
		/declare test string local NULL
		/echo \ay  Pet Items:
		/for i 1 to ${PETITEM_Total}
			/varset test ${Ini[INI_CHAR,PET,PETITEM_${i}]}
			/echo \ag    Pet Item: \aw${test}
			/declare PETITEM_${i} 					string	outer	${test.Arg[1,|]}
			|/echo \aw    _Item:\ag${PETITEM_${i}} 

			/declare PETITEM_${i}_Use 				bool	outer	${test.Arg[2,|]}
			|/echo \aw    _Use:\ag${PETITEM_${i}_Use}

			/declare PETITEM_${i}_Type 				string	outer	${test.Arg[3,|]}
			|/echo \aw    _Type:\ag${PETITEM_${i}_Type}

			/declare PETITEM_${i}_Summoner			string	outer	${test.Arg[4,|]}
			|/echo \aw    _Summoner:\ag${PETITEM_${i}_Summoner}
		/next i
	}
/return 


Sub processPetBuffs
	/declare PETBUFF_Total				int		outer	${Ini[INI_CHAR,PET,PETBUFF_Total,0]}	

	/if (${PETBUFF_Total}) {
		/declare i int local 0
		/declare test string local NULL

		/for i 1 to ${PETBUFF_Total}
			/varset test ${Ini[INI_CHAR,PET,PETBUFF_${i}]}
			/declare PETBUFF_${i} 					string	outer	${test.Arg[1,|]}
			/declare PETBUFF_${i}_Use 				bool	outer	${test.Arg[2,|]}
			/declare PETBUFF_${i}_Type 				string	outer	${test.Arg[3,|]}
			/declare PETBUFF_${i}_Summoner			string	outer	${test.Arg[4,|]}
		/next i
	}
/return

Sub xbot_initialize_pets
	/if (${str_classArchetype.Equal[MELEE_END]}) /return
	/if (${Me.Class.ShortName.Equal[WAR]}) /return
	
	/echo \ao xbot_initialize_pets
	
	|--Pet Information
	/declare str_petReagent				string  outer	${Ini[INI_CHAR,Pet,str_petReagent,NULL]}
	/declare bln_petUse					bool	outer	${Ini[INI_CHAR,Pet,bln_petUse,TRUE]}
	/declare int_petAttackHP			int		outer	${Ini[INI_CHAR,Pet,int_petAttackHP,99]}
	/declare bln_petDiscipline			bool	outer	${Ini[INI_CHAR,Pet,bln_petDiscipline,TRUE]}
	
	/declare str_petSpell				string	outer	${Ini[INI_CHAR,Pet,str_petSpell,Dark Assassin]}
	/declare str_petGem					string	outer	${Ini[INI_CHAR,Pet,str_petGem,gem11]}
	
	/declare bln_petUseBuffs			bool	outer	${Ini[INI_CHAR,Pet,bln_petUseBuffs,FALSE]}
	/declare timer_petBuffRecheck		timer	outer	0

	|--Pet Shrinker
	/declare str_petShrinker			string	outer  	${Ini[INI_CHAR,Pet,str_petShrinker,NULL]}
	/declare bln_petShrunk				bool	outer	FALSE
	
	|--Pet Buffs
	/call processPetBuffs

	
	|--Pet Items
	/declare bln_petItemized			bool	outer	FALSE
	/call processPetItems

	
	
/return