|**
----------------------------
xiris_buffing_lines.inc version 01.17.22
----------------------------
Methods to 'buff beg' from an appropriate class.
______________________________________________
REVISION HISTORY
	01.17.22	xiris	INITIAL REVISION
______________________________________________

Example: Macro runner running bot_pull.mac routine 
Calls internal method -> chkBuffs | Which calls library methods:

->chkHasBuffLine AEGO ${Me.Name}
	Returns FALSE
/if FALSE {
	/call composeGroupList |
	Returns Xiris|Xaelia|Xaexa|Xirael

	/bct ClericName requestBuffLine AEGO GROUP ${groupList}
}

This will request the buffline Aegolism from a cleric.
This works for several buff lines
[AEGO],[SYMBOL],[NINE],[DRU_DS],[FOCUS],[STR],[STA],[AGI]
[SHM_HASTE],[ENC_HASTE],[CRACK],[RNG_ATK],[RNG_AC]


**|

#event requestBuffLine "[#1#(msg)] requestBuffLine #2#,#3#,#4#"

Sub Event_requestBuffLine(_string _text, string _sender, string _buffLine, string _targetType, string _groupList)
	/cecho \ao rqstBuffLine
	/cecho \ao _text:\ag${_text}
	/cecho \ao _sender:\ag${_sender}
	/cecho \ao _buffLine:\ag${_buffLine}
	/cecho \ao _targetType:\ag${_targetType}
	/cecho \ao _groupList:\ag${_groupList}ye
	/call requestBuffLine ${_buffLine} ${_targetType} ${_groupList}
/return

|-- Method for getting a buff for SINGLE or GROUP
Sub requestBuffLine(string _buffLine, string _targetType, string _groupList)

	/cecho \aorequestBuffLine\ax ${_buffLine}, ${_targetType}, ${_groupList}
	/declare hasBuffCount int local 0
	/declare missingBuffCount int local 0
	/declare whoHasBuff	string local
	/declare whoMissingBuff string local
	/declare buffMissing			string	local
	/declare singleTargetMember string local

	
	|-- check to see how many have the buffline

	/call chkGroupHasBuffLine ${_buffLine} ${_groupList}
	
	/echo \ay ${Macro.Return}
	
	/varset hasBuffCount ${Macro.Return.Arg[1,~]}
	/varset missingBuffCount ${Macro.Return.Arg[2,~]}
	/varset whoHasBuff ${Macro.Return.Arg[3,~]}
	/varset whoMissingBuff ${Macro.Return.Arg[4,~]}
	/varset singleTargetMember ${_groupList.Arg[1,|]}
	
	|-- the split between casting single targets, and group is ~ 3 toons
	|Sub castBuffLine(string _buffLine, string _spellType, int _targetID) 
	/if (${missingBuffCount} > 2) {
		/echo More than 2 people are missing the buff (${whoMissingBuff})
		/call castBuffLine ${_buffLine} GROUP ${Spawn[pc ${requester}].ID}
	} else /if (${missingBuffCount} > 0) {
		/echo Less than 3 people are missing the buff (${whoMissingBuff})
		/declare arrMissing[${missingBuffCount}]
		/declare i int local 0
		/for i 1 to ${missingBuffCount}
			/cecho \ay${Spawn[pc ${whoMissingBuff.Arg[${i},,]}].ID}
			/call castBuffLine ${_buffLine} SINGLE ${Spawn[pc ${whoMissingBuff.Arg[${i},,]}].ID}
		/next i
		
		
	} else {
		/echo No one is missing the buff, fool.
	}
	
/return


|-- Determine if the group has the buff line we want to check
|   Pass the group members names in | seperated string, and determine how many of her groupmates have the buff
Sub chkGroupHasBuffLine(string _buffLine, string _groupMembers)
	/declare whoHasBuff			string	local
	/declare whoMissingBuff		string	local
	/declare list_groupMembers	list 	local
	/declare list_buffedMembers list	local
	/declare list_unbuffedMembers list local
	/declare delim				string	local
	/declare li 				listiterator local
	/declare found				bool	local 	FALSE
	/declare 	r 				string	local
	/declare result string local
	
	/varset delim ${list_groupMembers.Delimiter[|]}
	
	/if (${list_groupMembers.Append[${_groupMembers}]}) {
		/echo Splitting the group list into individual names
		/vardata li list_groupMembers.First
		/while (!${li.IsEnd}) {
			/echo Character: ${li.Value}
			/call chkHasBuffLine ${_buffLine} ${li.Value}
			/varset found ${Macro.Return}
			/if (${found}) { 
				/varset result ${list_buffedMembers.Append[${li.Value}]}
			} else {
				/varset result ${list_unbuffedMembers.Append[${li.Value}]}
			}
			
			/varset found FALSE
			/varset r ${li.Advance}
		}
		
	}
	
	|-- compose a string list of who is BUFFED (macros cant return anything but standard primitives)
	/vardata li list_buffedMembers.First
	/if (${list_buffedMembers.Count} == 1) {
		/varset whoHasBuff ${list_buffedMembers.Item[0]}
	} else {
		/while (!${li.IsEnd}) {
			/varset whoHasBuff ${whoHasBuff}${li.Value},
			/varset r ${li.Advance}
		}
	}

	|-- compose a string list of who is UNBUFFED
	/vardata li list_unbuffedMembers.First
	/if (${list_unbuffedMembers.Count} == 1) {
		/varset whoMissingBuff ${list_unbuffedMembers.Item[0]}
	} else {
		/while (!${li.IsEnd}) {
			/varset whoMissingBuff ${whoMissingBuff}${li.Value},
			/varset r ${li.Advance}
		}
	}
	
	|-- compose the return string and send it ~ delimited 
	:return

	/cecho \ar Checked ${_groupMembers} for ${_buffLine} 
	/cecho \ar Found: ${list_buffedMembers.Count} (${whoHasBuff})
	/cecho \ar Missing: ${list_unbuffedMembers.Count} (${whoMissingBuff})

/return ${list_buffedMembers.Count}~${list_unbuffedMembers.Count}~${whoHasBuff}~${whoMissingBuff}

|-- Determine if character has the buff line we want to check
Sub chkHasBuffLine(string _buffLine, string _targetName)
	/declare 	hasBuff			bool		 local	FALSE
	/declare 	buffDuration	int			 local	0
	/declare	slot			int			 local	0
	/declare 	list_buffs 		list 		 local
	/declare 	delim 			string 		 local
	/declare 	li 				listiterator local
	/declare 	r 				string	local
	
	
	|-- Netbots returns the list of buffs as a space(Fuck) seperated list
	/varset 	delim ${list_buffs.Delimiter[ ]}
	
	/echo Raw NetBot data on ${_targetName}: ${NetBots[${_targetName}].Buff}
	/if (${list_buffs.Append[${NetBots[${_targetName}].Buff}]}) {
		/echo Splitting into list for [${_targetName}]'s Buffs succeeded. Count: ${list_buffs.Count}
		/vardata li list_buffs.First
		/while (!${li.IsEnd}) {
			/echo Spell ID: ${li.Value} Spell Name: ${Spell[${li.Value}]}
			/varset r ${li.Advance}
		}
	}
	/cecho \apSetting line to ${_buffLine}
	|-- Set the reference to the requested line
	/if (${_buffLine.Equal[AEGO]}) 		/vardata li list_aegoLine.First
	/if (${_buffLine.Equal[SYMBOL]}) 	/vardata li list_symbolLine.First
	/if (${_buffLine.Equal[NINE]}) 		/vardata li list_nineLine.First
	/if (${_buffLine.Equal[DRU_DS]})	/vardata li list_druDSLine.First
	/if (${_buffLine.Equal[FOCUS]}) 	/vardata li list_focusLine.First
	/if (${_buffLine.Equal[STR]}) 		/vardata li list_strLine.First
	/if (${_buffLine.Equal[STA]}) 		/vardata li list_staLine.First
	/if (${_buffLine.Equal[AGI]}) 		/vardata li list_agiLine.First
	/if (${_buffLine.Equal[SHM_HASTE]}) /vardata li list_shmHasteLine.First
	/if (${_buffLine.Equal[ENC_HASTE]}) /vardata li list_encHasteLine.First
	/if (${_buffLine.Equal[CRACK]})		/vardata li list_encCrackLine.First
	/if (${_buffLine.Equal[RNG_ATK]}) 	/vardata li list_atkLine.First
	/if (${_buffLine.Equal[RNG_AC]}) 	/vardata li list_acLine.First


	| Run selected list throught the interator
	|-- split the targetsbuff list from Netbots into an array, and compare values with the values of the selected line
	/while (!${li.IsEnd}) {
		/echo Checking if li_charBuffs.Contains ${Spell[${li.Value}]}
		/if (${list_buffs.Contains[${li.Value}]}) {
			/varset slot ${list_buffs.Index[${li.Value}]}
			/varset hasBuff TRUE
			/varcalc buffDuration (${NetBots[${_targetName}].Buff[1].DurationValue1} )
			/echo FOUND Buff: ${Spell[${li.Value}]} ID: ${li.Value} SLOT: ${slot} DURATION: ${buffDuration}
			/goto :return
		} 
		/varset r ${li.Advance}
	}

	:return
/return ${hasBuff}

|-- Cast a specific buffline on the target (will cast highest available either group or single target)
Sub castBuffLine(string _buffLine, string _spellType, int _targetID)  
	/echo castBuffLine(${_buffLine},${_spellType},${_targetID})
	/declare 	spellName	string			local
	/declare	groupSpell	bool			local	FALSE
	/declare	bookSlot	int				local	0
	/declare	highestCastable	string		local	""
	/declare	canCast		bool			local	FALSE
	/declare 	li 			listiterator 	local
	/declare 	r 			string			local
	
	/if (${_spellType.Equal[GROUP]}) /varset groupSpell TRUE
	
	|-- find which spell in the line I can cast
	/cecho \aiSetting line to \ar${_buffLine}
	|-- Set the reference to the requested line
	/if (${_buffLine.Equal[AEGO]}) 		/vardata li list_aegoLine.First
	/if (${_buffLine.Equal[SYMBOL]}) 	/vardata li list_symbolLine.First
	/if (${_buffLine.Equal[NINE]}) 		/vardata li list_nineLine.First
	/if (${_buffLine.Equal[DRU_DS]})	/vardata li list_druDSLine.First
	/if (${_buffLine.Equal[FOCUS]}) 	/vardata li list_focusLine.First
	/if (${_buffLine.Equal[STR]}) 		/vardata li list_strLine.First
	/if (${_buffLine.Equal[STA]}) 		/vardata li list_staLine.First
	/if (${_buffLine.Equal[AGI]}) 		/vardata li list_agiLine.First
	/if (${_buffLine.Equal[SHM_HASTE]}) /vardata li list_shmHasteLine.First
	/if (${_buffLine.Equal[ENC_HASTE]}) /vardata li list_encHasteLine.First
	/if (${_buffLine.Equal[CRACK]})		/vardata li list_encCrackLine.First
	/if (${_buffLine.Equal[RNG_ATK]}) 	/vardata li list_atkLine.First
	/if (${_buffLine.Equal[RNG_AC]}) 	/vardata li list_acLine.First
	
	/while (!${li.IsEnd}) {
		|- list is in ID form, for this we need to derive the name
		/varset spellName ${Spell[${li.Value}].Name}
		/varset bookSlot ${Me.Book[${spellName}]}
		|/echo ${spellName} ${bookSlot} ${Spell[${spellName}].TargetType}
		/if (${bookSlot}) {
			/if (!${groupSpell} && ${Spell[${spellName}].TargetType.Equal[Single]}) {
				| we want the single target version
				|/cecho Looking for my highest level \ay${_spellType} \axtarget version of ${_buffLine}
				/varset highestCastable ${li.Value}
				|/echo I can cast ${spellName}(${li.Value})
			} else /if (${groupSpell} && (${Spell[${spellName}].TargetType.Equal[Group v2]} || ${Spell[${spellName}].TargetType.Equal[Group v1]})) {
				| we want the group version
				|/cecho Looking for my highest level \ay${_spellType} \axversion of ${_buffLine}
				/varset highestCastable ${li.Value}
				|/echo I can cast ${spellName}(${li.Value})
			}
		
			
		}
		/varset r ${li.Advance}
	}
	/cecho \ap ${Me.Name}'s Highest Castable \ay${_spellType} \ar${_buffLine}\ap is ${Spell[${highestCastable}].Name} (${highestCastable})
	

/return

Sub composeGroupList(string _delim) 
	/declare groupList string local
	/declare i int local
	/for i 0 to ${Group}
		/varset groupList  ${groupList}${_delim}${Group.Member[${i}].Name}
	/next i
/return ${groupList}


Sub xbot_initialize_bufflines
	/declare delim string local
	/declare result string local
	/declare li	listiterator local
	/return
	
	|-- Aegolism Spell IDs
	|	Virtue|Hand of Virtue|Conviction|Hand of Conviction
	/declare str_aegoLine		string	outer	3467|3479|5257|5278
	/declare list_aegoLine		list	outer
	/varset	 delim	${list_aegoLine.Delimiter[|]}
	/if (${list_aegoLine.Append[${str_aegoLine}]}) {
		|/echo Splitting into list for Cleric Aegolism Line suceeded. Count: ${list_aegoLine.Count}
	}
	
	|-- Symbol Spell IDs
	|   Symbol of Marzin|Marzin's Mark|Symbol of Kazad|Kazad's Mark|Symbol of Balikor|Balikor's Mark
	/declare str_symbolLine		string	outer	1535|2893|3466|3047|5252|5277
	/declare list_symbolLine 	list	outer
	/varset	 delim	${list_symbolLine.Delimiter[|]}
	/if (${list_symbolLine.Append[${str_symbolLine}]}) {
		|/echo Splitting into list for Cleric Symbol Line suceeded. Count: ${list_symbolLine.Count}
	}
	
	|-- Nine Spell IDs
	|	Protection of the Nine|Blessing of the Nine|Steeloak Skin|Blessing of Steeloak
	/declare str_nineLine 		string	outer 	3234|3451|5352|5466
	/declare list_nineLine		list	outer
	/varset	 delim	${list_nineLine.Delimiter[|]}
	/if (${list_nineLine.Append[${str_nineLine}]}) {
		|/echo Splitting into list for Druid Nine Line suceeded. Count: ${list_nineLine.Count}
	}
	
	|-- Druid DS Spell IDs
	|	Shield of Bracken|Legacy of Bracken|Nettle Shield|Legacy of Nettles
	/declare str_druDSLine 		string	outer 	3448|3295|5258|5365
	/declare list_druDSLine		list	outer
	/varset	 delim	${list_druDSLine.Delimiter[|]}
	/if (${list_druDSLine.Append[${str_druDSLine}]}) {
		|/echo Splitting into list for Druid DS Line suceeded. Count: ${list_druDSLine.Count}
	}
	
	|-- Focus Spell IDs
	|	Focus of Soul|Focus of the Seventh|Wunshi's Focusing|Talisman of Wunshi
	/declare str_focusLine		string	outer	3235|3397|5396|5415
	/declare list_focusLine		list	outer
	/varset	 delim	${list_focusLine.Delimiter[|]}
	/if (${list_focusLine.Append[${str_focusLine}]}) {
		|/echo Splitting into list for Shaman Focus Line suceeded. Count: ${list_focusLine.Count}
	}
	
	|-- Shaman Strength IDs
	|	Strength of the Diaku|Talisman of the Diaku|Spirit of Might|Talisman of Might
	/declare str_strLine 		string	outer 	3388|3392|5404|5409
	/declare list_strLine		list	outer
	/varset	 delim	${list_strLine.Delimiter[|]}
	/if (${list_strLine.Append[${str_strLine}]}) {
		|/echo Splitting into list for Shaman Strength Line suceeded. Count: ${list_strLine.Count}
	}
	
	|-- Shaman Stamina IDs
	|	Endurance of the Boar|Talisman of the Boar|Spirit of Fortitude|Talisman of Fortitude
	/declare str_staLine		string	outer	3382|3389|5398|5405
	/declare list_staLine		list	outer
	/varset	 delim	${list_staLine.Delimiter[|]}
	/if (${list_staLine.Append[${str_staLine}]}) {
		|/echo Splitting into list for Shaman Stamina Line suceeded. Count: ${list_staLine.Count}
	}
	
	|-- Shaman Agility IDs
	|	Agility of the Wrulan|Talisman of the Wrulan|Spirit of Sense|Talisman of Sense
	/declare str_agiLine		string	outer	3378|3383|5390|5399
	/declare list_agiLine		list	outer
	/varset	 delim	${list_agiLine.Delimiter[|]}
	/if (${list_agiLine.Append[${str_agiLine}]}) {
		|/echo Splitting into list for Shaman Agility Line suceeded. Count: ${list_agiLine.Count}
	}
	
	|-- Shaman Haste IDs
	| Swift Like the Wind|Talisman of Celerity
	/declare str_shmHasteLine	string	outer	0172|3391
	/declare list_shmHasteLine	list	outer
	/varset	 delim	${list_shmHasteLine.Delimiter[|]}
	/if (${list_shmHasteLine.Append[${str_shmHasteLine}]}) {
		|/echo Splitting into list for Shaman Haste Line suceeded. Count: ${list_shmHasteLine.Count}
	}
	
	|-- Enchanter Haste IDs
	| Wonderous Rapidity|Speed of the Brood|Visions of Grandeur|Speed of Vallon|Vallon's Quickening|Speed of Salik|Hastening of Salik
	/declare str_encHasteLine	string	outer	1709|2895|1710|3240|3178|5507|5521
	/declare list_encHasteLine 	list	outer
	/varset	 delim	${list_encHasteLine.Delimiter[|]}
	/if (${list_encHasteLine.Append[${str_encHasteLine}]}) {
		|/echo Splitting into list for Enchanter Haste Line suceeded. Count: ${list_encHasteLine.Count}
	}
	
	|-- Enchanter Haste IDs
	| Wonderous Rapidity|Speed of the Brood|Visions of Grandeur|Speed of Vallon|Vallon's Quickening|Speed of Salik|Hastening of Salik
	/declare str_encCrackLine	string	outer	1709|2895|1710|3240|3178|5507|5521
	/declare list_encCrackLine 	list	outer
	/varset	 delim	${list_encCrackLine.Delimiter[|]}
	/if (${list_encCrackLine.Append[${str_encCrackLine}]}) {
		|/echo Splitting into list for Enchanter Crack Line suceeded. Count: ${list_encCrackLine.Count}
	}
	
	|-- Ranger ATK
	| Mark of the Predator|Spirit of the Predator|Howl of the Predator
	/declare str_rngATK			string	outer	2598|3417|5312
	/declare list_rngATK		list	outer
	/varset	 delim	${list_rngATK.Delimiter[|]}
	/if (${list_rngATK.Append[${str_rngATK}]}) {
		|/echo Splitting into list for Ranger ATK Line suceeded. Count: ${list_rngATK.Count}
	}	
	
	|-- Ranger AC
	| Strength of Tunare|Strength of the Hunter|
	/declare str_rngAC		string	outer	3487|5306
	/declare list_rngAC		list	outer
	/varset	 delim	${list_rngAC.Delimiter[|]}
	/if (${list_rngAC.Append[${str_rngAC}]}) {
		|/echo Splitting into list for Ranger AC Line suceeded. Count: ${list_rngAC.Count}
	}	
	
	
/return