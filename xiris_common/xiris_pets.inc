|**
----------------------------
Pet HANDLERS updated version 08.01.19 
----------------------------
This include contains logic for pet spawning/buffing/etc

Supported by all classes based on INI settings

______________________________________________
REVISION HISTORY
    08.01.19    xiris 	INITIAL REVISION

	
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
	/if (${bln_engaged}) /return
	/if (${Me.Pet.ID} && ${Me.Pet.Name.Find[familiar]}) /pet get lost
	/if (${Me.Invis} || ${Me.Moving} || ${timer_petBuffRecheck}) /return

	|--Spawn your pet
	/call chkPetSpawn
	/call RunRaidEvents	
	
	|--Buff your pet
	/if (${Me.Class.ShortName.NotEqual[SHM]}) 	/call chkPetBuffs

	|--Weaponize your pet
	/if (${Me.Class.ShortName.Equal[MAG]}) 		/call chkPetWeaps
	
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
	}
	/if (${Target.ID}==${Me.Pet.ID}) /squelch /target clear
	/if (!${timer_petBuffRecheck})	 /varset timer_petBuffRecheck 30s
	/memspellset default
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
	/if (${bln_engaged}) /return
	/if (!${Me.Pet.ID}) /return
	/if (${bln_petWeaponized}) /return
	|-- load 'petWeap' spell set
	/delay 3s
	/echo checking Pet weapons
	/echo weaponized? ${bln_petWeaponized}
	/memspellset petweap
	/delay 5s
	/target ${Me.Pet}
	/face fast
	/if (${Target.Distance} > 20 ) {
		/stick 10
		/delay 5s
	}
	/delay 10
	/if (${Target.Distance} > 40) /goto :return
	/stick 10
	/delay 10

	/echo trying ${str_petBelt}
	/if (${FindItemCount[=Summoned: ${str_petBelt}]}==0) {
		/echo Belt: ${str_petBelt}
		/call chkSpellMemorized "${str_petBelt}" TRUE
		/call MQ2Cast "${str_petBelt}"
		/target ${Me.Pet}
		/if (${Cursor.ID}) /click left target
		/delay 10
		/if (${Window[GiveWnd].Open}) /notify GiveWnd GVW_Give_Button leftmouseup 
	}
	/delay 2s
	/echo trying ${str_petWeap1}
	/if (${FindItemCount[=Summoned: ${str_petWeap1}]}==0) {
		/echo Weapon 1: ${str_petWeap1}
		/call chkSpellMemorized "${str_petWeap1}" TRUE
		/call MQ2Cast "${str_petWeap1}"
		/target ${Me.Pet}
		/if (${Cursor.ID}) /click left target
		/delay 10
		/if (${Window[GiveWnd].Open}) /notify GiveWnd GVW_Give_Button leftmouseup 
	}
	/delay 2s
	/echo trying ${str_petWeap2}
	/if (${FindItemCount[=Summoned: ${str_petWeap2}]}==0) {
		/echo Weapon 2: ${str_petWeap2}
		/call chkSpellMemorized "${str_petWeap2}" TRUE
		/call MQ2Cast "${str_petWeap2}"
		/target ${Me.Pet}
		/if (${Cursor.ID}) /click left target
		/delay 10
		/if (${Window[GiveWnd].Open}) /notify GiveWnd GVW_Give_Button leftmouseup 
	}

	:return
	/melee reset
	/varset bln_petWeaponized TRUE
	/memspellset default
	/autoinv
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

|**
 ----------------------------
 Pet Attack  
 ----------------------------
 Sends the pet in
 ----------------------------
**|

Sub chkPetAttack
	/if (!${bln_engaged}) /return
	/if (${bln_petAttacking}) /return
	/if (${Target.PctHPs} == 0) /return
	/if (${Me.Pet.ID} && !${Me.Pet.Combat} && ${Target.ID}==${int_targetID} && ${Target.PctHPs}<${int_petAttackHP} && ${Target.Distance}<=100) {
		/echo chkPetAttack: Siccing Pet
		/varset bln_petAttacking TRUE
		/pet attack
	}
/return



Sub xbot_initialize_pets
	/if (${str_classArchetype.Equal[MELEE_END]}) /return
	/if (${Me.Class.ShortName.Equal[WAR]}) /return
	/echo xbot_initialize_pets
	
	|--Pet Information
	/declare str_petReagent				string  outer	${Ini[botINI,Pet,str_petReagent,NULL]}
	/declare bln_petUse					bool	outer	${Ini[botINI,Pet,bln_petUse,TRUE]}
	/declare bln_petAttacking			bool	outer 	${Me.Pet.Combat}
	/declare int_petAttackHP			int		outer	${Ini[botINI,Pet,int_petAttackHP,95]}
	/declare bln_petDiscipline			bool	outer	${Ini[botINI,Pet,bln_petDiscipline,TRUE]}
	
	/declare str_petSpell				string	outer	${Ini[botINI,Pet,str_petSpell,Child Of Bertoxxulous]}
	/declare str_petGem					string	outer	${Ini[botINI,Pet,str_petGem,gem11]}
	
	/declare bln_petUseBuffs			bool	outer	${Ini[botINI,Pet,bln_petUseBuffs,FALSE]}
	/declare int_petBuffTotal			int		outer   ${Ini[botINI,DamageOverTime,int_petBuffTotal,0]}
	/declare timer_petBuffRecheck		timer	outer	0

	/declare pb							int		local
	|--Pet Shrinker
	/declare str_petShrinker			string	outer  	${Ini[botINI,Pet,str_petShrinker,NULL]}
	/declare bln_petShrunk				bool	outer	FALSE
	
	|--Pet Buffs
	/for pb 1 to ${int_petBuffTotal} 
		/declare petBuff_${pb} 				string	outer	${Ini[botINI,Pet,petBuff_${pb}]}
		/declare petBuff_${pb}_Gem 			string	outer	${Ini[botINI,Pet,petBuff_${pb}_Gem]}
	/next pb
	
	|--Pet Weapons
	/declare str_petBelt				string	outer 	${Ini[botINI,Pet,str_petBelt,Girdle of Magi`Kot]}
	/declare str_petWeap1				string  outer   ${Ini[botINI,Pet,str_petWeap1,Blade of the Kedge]}
	/declare str_petWeap2				string  outer   ${Ini[botINI,Pet,str_petWeap1,Fist of Ixiblat]}
	/declare bln_petWeaponized			bool	outer	FALSE
	
	
/return