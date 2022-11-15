|**
----------------------------
EVENTS_OOW_MPG_SPECIALIZATION.INC
----------------------------
This was written ground up for usage with xiris's bots
NOTE: this requires MQ2NAV
______________________________________________
REQUIRED PLUGINS
	mq2nav
	mq2bandolier
______________________________________________
______________________________________________
REVISION HISTORY
	04.26.22    xiris	INITIAL REVISION
______________________________________________
**|	
|-- Muramite Proving Grounds events
|-- Mastery of Specialization

#event RIGIDITY 	"#*#Your movements become rigid#*#"
#event MISFORTUNE 	"#*#You feel very unlucky#*#"
#event IMPRECISION  "#*#You are affected by imprecision#*#"
#event IMPURITY     "#*#You lose your coordination#*#"

Sub InitEvents_OOW_MPG_Specialization
	/cecho \ag Initializing MPG Specialization
	/declare str_currentMode string outer NORMAL
	/call RegisterRaidEvents Events_OOW_MPG_Specialization 1
	/if (${str_classArchetype.NotEqual[PRIEST]} && ${str_classArchetype.NotEqual[CASTER]}) /call toggleMQ2Melee FALSE
/return

Sub Events_OOW_MPG_Specialization
	|**
	/doevents RIGIDITY
	/doevents MISFORTUNE
	/doevents IMPRECISION
	/doevents IMPURITY
	**|
	|- safety catch if we have the debuff but the events didnt catch it, manually check our debuffs!
	/call chkCurrentSpecialization
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/call chkCombatAbilities
	}
	
	
/return

Sub EVENT_RIGIDITY 
	/varset str_currentMode RIGIDITY
	/call handleSpecializationChange
/return

Sub EVENT_MISFORTUNE
	/varset str_currentMode MISFORTUNE
	/call handleSpecializationChange
/return

Sub EVENT_IMPRECISION 
	/varset str_currentMode IMPRECISION
	/call handleSpecializationChange
/return

Sub EVENT_IMPURITY
	/varset str_currentMode IMPURITY
	/call handleSpecializationChange
/return


Sub chkCurrentSpecialization
	/declare hasChanged bool local FALSE
	| rigidity check
	/if (${Me.Buff[Rigidity].ID} && ${str_currentMode.NotEqual[RIGIDITY]}) {
		/varset str_currentMode RIGIDITY
		/varset hasChanged TRUE
	}

	| misfortune check
	/if (${Me.Buff[Curse of Misfortune].ID} && ${str_currentMode.NotEqual[MISFORTUNE]}) {
		/varset str_currentMode MISFORTUNE
		/varset hasChanged TRUE
	}
	| imprecision check
	/if (${Me.Buff[Imprecision].ID} && ${str_currentMode.NotEqual[IMPRECISION]}) {
		/varset str_currentMode IMPRECISION
		/varset hasChanged TRUE
	}
	| impurity check
	/if (${Me.Buff[Impurity].ID} && ${str_currentMode.NotEqual[IMPURITY]}) {
		/varset str_currentMode IMPURITY
		/varset hasChanged TRUE
	}

	| no debuff check
	/if (!${Me.Buff[Rigidity].ID} && !${Me.Buff[Curse of Misfortune].ID} && !${Me.Buff[Imprecision].ID} && !${Me.Buff[Impurity].ID} && ${str_currentMode.NotEqual[NORMAL]}) {
		/varset str_currentMode NORMAL
		/varset hasChanged TRUE
	}

	/if (${hasChanged}) /call handleSpecializationChange
/return

|-- handle the current specialization if we need to change things;
|   try not to call this unless the current check said we needed to change;
Sub handleSpecializationChange
	| switch on current Mode
	/if (${str_currentMode.Equal[RIGIDITY]}) {
		/echo RIGIDITY	
		| RIGIDITY: Don't use blunt weapons or cold based spells (casting time will be slowed);
		/if (${str_classArchetype.Equal[CASTER]}) {
			/call SetResistTypes Fire,Magic,Poison,Disease
		} else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
			/call doBandolier NOBLUNT
			/if (${Me.Class.ShortName.Equal[MNK]}) /melee flyingkick=on	
			/call EngageTarget
		}
	} else /if (${str_currentMode.Equal[MISFORTUNE]}) {
		/echo MISFORTUNE
		| CURSE OF MISFORTUNE: Don't use slashing weapons and or magic based spell  (MOB becomes 100% resistant to them);
		/if (${str_classArchetype.Equal[CASTER]}) {
			/call SetResistTypes Fire,Cold,Poison,Disease
		} else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
			/call doBandolier NOSLASHING
			/if (${Me.Class.ShortName.Equal[MNK]}) /melee flyingkick=on	
			/call EngageTarget
		}
	} else /if (${str_currentMode.Equal[IMPRECISION]}) {
		/echo IMPRECISION
		| IMPRECISION: Don't use piercing weapons, back stab or fire based spells (damage will be decreased);
		/if (${str_classArchetype.Equal[CASTER]}) {
			/call SetResistTypes Magic,Cold,Poison,Disease
		} else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
			/call doBandolier NOPIERCING
			/if (${Me.Class.ShortName.Equal[MNK]}) /melee flyingkick=on	
			/call EngageTarget
		}
	} else /if (${str_currentMode.Equal[IMPURITY]}) {
		/echo IMPURITY
		| IMPURITY: Don't use hand to hand, flying kick, poison based spells or disease based spells (there is a significant mana increase to cast spells);
		/if (${str_classArchetype.Equal[CASTER]}) {
			/call SetResistTypes Fire,Magic,Cold
		} else /if (${Me.Class.ShortName.Equal[MNK]} || ${Me.Class.ShortName.Equal[BST]} ) {
			/call doBandolier NOH2H
			/if (${Me.Class.ShortName.Equal[MNK]}) /melee flyingkick=off	
			/call EngageTarget
		} else {
			/call doBandolier default
		}
	} else /if (${str_currentMode.Equal[NORMAL]}) {
		/echo NORMAL
		| NORMAL: Use whatever you want!
		/if (${str_classArchetype.Equal[CASTER]}) {
			/call SetResistTypes Fire,Cold,Poison,Disease
		} else /if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
			/call doBandolier default
			/if (${Me.Class.ShortName.Equal[MNK]}) /melee flyingkick=on	
			/call EngageTarget
		}
	}
	/if (${str_classArchetype.Equal[MELEE_END]} || ${str_classArchetype.Equal[MELEE_MANA]} || ${str_classArchetype.Equal[TANK]}) {
		/face fast
		/attack on
	}
/return