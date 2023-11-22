|**
----------------------------
xiris_common.inc
----------------------------
This includes the healing setup (INI READ)
and sub routines as necessary
______________________________________________
REVISION HISTORY
    03.24.16    xiris	INITIAL REVISION - Consolidation of several macro embedded routines
	10.15.16	xiris	Cleaned up (removed CH) and added group healing event
	01.13.22	xiris	Added Rez routines
	04.25.22	xiris 	Added healing potion routines
	05.08.22	xiris	Integrated AERez.mac
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
						Updated the rez portion to work with new dannet handlers
	10.13.22	xiris 	Update sub healCast to ignore hp checks on group heals			
	12.31.22	xiris	Worked on Divine Arbitration handler, should be cleaner
	01.31.23	xiris 	Replaced chkGroup healing loops to use the MQ2WorstHurt plugin	
	02.02.23	xiris	Fixed long standing bug: chkHeal_MT would always call chkMTAlive, with targetting to TRUE, which would break cleric debuffing since it 
						would always target the MT before trying to cast debuff
	08.11.23	xiris	Rewrite for 2.0 INIs and performance issues
	11.22.23	xiris	Reworked Divine Arbitration
**|



#EVENT GroupHeal  		"#*#DoGroupHeal#*#"
#EVENT InterruptON 		"#*#InterruptON#*#"
#EVENT InterruptOFF 	"#*#InterruptOFF#*#"

|-- Hitpoint Change Events
#EVENT ChangeHP			"#1# tells you#*#'ChangeHP #2#'"
#EVENT ChangeHP			"#1# tells the group#*#'ChangeHP #2#'"
#EVENT ChangeHP			"#1# tells the raid#*#'ChangeHP #2#'"
#EVENT ChangeHP 		"[ #1# #*#]#*#ChangeHP #2#"

#EVENT ChangeHPTank		"#1# tells you#*#'changeHPTank #2#'"
#EVENT ChangeHPTank		"#1# tells the group#*#'changeHPTank #2#'"
#EVENT ChangeHPTank		"#1# tells the raid#*#'changeHPTank #2#'"
#EVENT ChangeHPTank 	"[ #1# #*#]#*#changeHPTank #2#"

#EVENT ChangeHPSelf		"#1# tells you#*#'changeHPSelf #2#'"
#EVENT ChangeHPSelf		"#1# tells the group#*#'changeHPSelf #2#'"
#EVENT ChangeHPSelf		"#1# tells the raid#*#'changeHPSelf #2#'"
#EVENT ChangeHPSelf 	"[ #1# #*#]#*#changeHPSelf #2#"

#EVENT ChangeHPGroup	"#1# tells you#*#'changeHPGroup #2#'"
#EVENT ChangeHPGroup	"#1# tells the group#*#'changeHPGroup #2#'"
#EVENT ChangeHPGroup	"#1# tells the raid#*#'changeHPGroup #2#'"
#EVENT ChangeHPGroup 	"[ #1# #*#]#*#changeHPGroup #2#"

#EVENT HealPoints		"#1# tells you#*#'SetHealPOINTS #2#'"
#EVENT HealPoints		"#1# tells the group#*#'SetHealPOINTS #2#'"
#EVENT HealPoints		"#1# tells the raid#*#'SetHealPOINTS #2#'"
#EVENT HealPoints		"[ #1# #*#]#*#SetHealPOINTS #2#"

|-- Heal type events
#EVENT HealType			"#1# tells you#*#'HealType #2#'"
#EVENT HealType			"#1# tells the group#*#'HealType #2#'"
#EVENT HealType			"#1# tells the raid#*#'HealType #2#'"
#EVENT HealType 		"[ #1# #*#]#*#HealType #2#"

#EVENT HealMode			"#1# tells you#*#'HealMode #2#'"
#EVENT HealMode			"#1# tells the group#*#'HealMode #2#'"
#EVENT HealMode			"#1# tells the raid#*#'HealMode #2#'"
#EVENT HealMode 		"[ #1# #*#]#*#HealMode #2#"

|-- Rez Events
#EVENT CANNOTREZ		"This corpse cannot be resurrected."
#EVENT NOTONLINE		"You told #1#, '#2# is not online at this time'"
#EVENT NOTONLINE  		"Your tell to #1# has been queued"
#EVENT CombatRezON		"#*#CombatRezON#*#"
#EVENT CombatRezOFF		"#*#CombatRezOFF#*#"

|-- Divine Intervention Events
#EVENT DivineInterventionRequest	"#1# tells you#*#'dime#*#'"
#EVENT DivineInterventionRequest	"#1# tells the group#*#'dime#*#'"
#EVENT DivineInterventionRequest	"#1# tells the raid#*#'dime#*#'"
#EVENT DivineInterventionRequest	"[ #1# #*#]#*#dime#*#"

|-- CHEvents should only be listened for in the class handlers (cleric, druid)
#EVENT CHStart			"#1# tells you#*#'CHStart #2#'"
#EVENT CHStart			"#1# tells the group#*#'CHStart #2#'"
#EVENT CHStart			"#1# tells the raid#*#'CHStart #2#'"
#EVENT CHStart			"[ #1# #*#]#*#CHStart #2#"

#EVENT CHStop			"#1# tells you#*#'CHStop #2#'"
#EVENT CHStop			"#1# tells the group#*#'CHStop #2#'"
#EVENT CHStop			"#1# tells the raid#*#'CHStop #2#'"
#EVENT CHStop			"[ #1# #*#]#*#CHStop #2#"

#EVENT CHPause			"#1# tells you#*#'CHPause #2#'"
#EVENT CHPause			"#1# tells the group#*#'CHPause #2#'"
#EVENT CHPause			"#1# tells the raid#*#'CHPause #2#'"
#EVENT CHPause			"[ #1# #*#]#*#CHPause #2#"

#EVENT CHResume			"#1# tells you#*#'CHResume#*#'"
#EVENT CHResume			"#1# tells the group#*#'CHResume#*#'"
#EVENT CHResume			"#1# tells the raid#*#'CHResume#*#'"
#EVENT CHResume 		"[ #1# #*#]#*#CHResume#*#"

#EVENT CHSwitch			"#1# tells you#*#'CHSwitch #2#'"
#EVENT CHSwitch			"#1# tells the group#*#'CHSwitch #2#'"
#EVENT CHSwitch			"#1# tells the raid#*#'CHSwitch #2#'"
#EVENT CHSwitch 		"[ #1# #*#]#*#CHSwitch #2#"


|-- Regen Events (Celestial Regen [CLR], Spirit of Wood [DRU], Ancestral Aid [SHM] )
#EVENT FireRegen		"#1# tells you#*#'FireRegen #2#'"
#EVENT FireRegen		"#1# tells the group#*#'FireRegen #2#'"
#EVENT FireRegen		"#1# tells the raid#*#'FireRegen #2#'"
#EVENT FireRegen 		"[ #1# #*#]#*#FireRegen #2#"

|-- Mana Regen Events (Paragon of Spirit [BST])
#EVENT FireManaRegen	"#1# tells you#*#'FireManaRegen #2#'"
#EVENT FireManaRegen	"#1# tells the group#*#'FireManaRegen #2#'"
#EVENT FireManaRegen	"#1# tells the raid#*#'FireManaRegen #2#'"
#EVENT FireManaRegen	"[ #1# #*#]#*#FireManaRegen #2#"

Sub initEvents_Healing
	/if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]} ) /call RegisterBackgroundEvents	Events_Healing_Background
	/if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]} ) /call RegisterRaidEvents 		Events_Healing_Raid
	/if (${Me.Class.ShortName.Equal[CLR]})							/call RegisterRaidEvents		Events_Healing_Raid_ClericsOnly 
/return 


Sub Events_Healing_Raid 
	/if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]} && (${bln_engaged} || ${bln_killing})) /call chkHeals
/return

Sub Events_Healing_Raid_ClericsOnly
	/doevents DivineInterventionRequest
	/doevents CombatRezON
	/doevents CombatRezOFF
	/doevents CHStart
	/doevents CHStop
	/doevents CHPause
	/doevents CHResume
	/doevents CHSwitch
	/doevents HealType
	/doevents HealMode
/return 

Sub Events_Healing_Background
    /if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]} && (!${bln_engaged} && !${bln_killing} && !${timer_check_heals})) /call chkHeals
	/doevents GroupHeal
	/doevents InterruptON
	/doevents InterruptOFF
	/doevents ChangeHPTank
	/doevents ChangeHPSelf
	/doevents ChangeHPGroup
	/doevents HealPoints
	/doevents FireRegen 
	/doevents FireRegenMana
	/if (!${bln_engaged} && !${bln_killing} && ${int_mana_regen_total}) /call chkManaRegen
/return

Sub EVENT_InterruptON
	/doevents flush InterruptON
	/call setHealInterrupt TRUE
/return

Sub EVENT_InterruptOFF
	/doevents flush InterruptOFF
	/call setHealInterrupt FALSE
/return

Sub EVENT_GroupHeal
	/doevents flush GroupHeal
	/delay 40 !${Me.Casting.ID}
	/call MQ2Cast "${str_heal_group}"
/return

Sub EVENT_ChangeHPTank(string _line, string _sender, string _eventParams)
	/doevents flush ChangeHPTank
	/if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]}) {
		/echo \agTank HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_tank_normal ${HP}
	}
/return 

Sub EVENT_ChangeHPSelf(string _line, string _sender, string _eventParams)
	/doevents flush ChangeHPSelf
	/if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]}) {
		/echo \agSelf HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_self_normal ${HP}
	}
/return 

Sub EVENT_ChangeHPGroup(string _line, string _sender, string _eventParams)
	/doevents flush ChangeHPGroup
	/if (${Select[${Me.Class.ShortName},BST,CLR,DRU,PAL,SHM,RNG]}) {
		/echo \agGroup HP (HealPoint) Change called! \awChanging to: \ay${_eventParams}
		/declare HP int local ${_eventParams}
		/varset	int_point_group_normal ${HP}
	}
/return

Sub xbot_initialize_healing(int _tankHealPointOverride)
    /echo \n \apInitialize: \aoHealing \aw-------------------------

    /if (${Select[${Me.Class.ShortName},BER,BRD,MNK,ROG,WAR,ENC,MAG,NEC,WIZ]}) /return
	/call processHealDefaults ${_tankHealPointOverride}
    /call processHealSpellSettings
	/call processHPRegenSettings
	/call processManaRegenSettings
    
    /call initEvents_Healing
/return 

Sub processHealDefaults(int _tankHealPointOverride) 
	/echo \awprocessHealDefaults

	| [Heal_Settings]
	/declare int_heal_mode					int		outer	${Ini[INI_HEAL,Heal_Settings,int_heal_mode,2]}
	/declare bln_heal_interrupt				bool	outer	${Ini[INI_HEAL,Heal_Settings,bln_heal_interrupt,TRUE]}
	/declare bln_heal_useGroup				bool 	outer 	${Ini[INI_HEAL,Heal_Settings,bln_heal_useGroup,TRUE]}
	/declare bln_heal_useHoT				bool	outer	${Ini[INI_HEAL,Heal_Settings,bln_heal_useHoT,TRUE]}
	/declare str_heal_HoT_List				string 	outer  	${Ini[INI_HEAL,Heal_Settings,str_heal_HoT_List,NULL]}
	/declare bln_heal_announce				bool 	outer	${Ini[INI_HEAL,Heal_Settings,bln_heal_announce,TRUE]}
	/declare str_heal_announce				string 	outer 	${Ini[INI_HEAL,Heal_Settings,str_heal_announce,TRUE]}
	/declare bln_heal_useDivineArb			bool	outer 	${Ini[INI_HEAL,Heal_Settings,bln_heal_useDivineArb,FALSE]}
	/declare bln_heal_interruptToSaveSelf	bool	outer	${Ini[INI_HEAL,Heal_Settings,bln_heal_interruptToSaveSelf,TRUE]}
	/declare int_heal_defaultGem			int     outer 	${Ini[INI_HEAL,Heal_Settings,int_heal_defaultGem,2]}
	/declare int_heal_major					int  	outer 	0
	/declare int_heal_minor					int 	outer 	0
	/declare timer_check_heals				timer 	outer   50

	| [Heal_Spells]
	/declare int_heal_interrupt				int 	outer  	0
	/declare str_heal_current				string  outer   NULL
	/declare bln_heal_useEfficient			bool    outer   FALSE
	/declare str_heal_normal				string	outer	NULL
	/declare bln_heal_normal				bool 	outer 	TRUE
	/declare str_heal_normal_efficient		string	outer	NULL
	/declare str_heal_frantic				string	outer	NULL
	/declare bln_heal_frantic				bool 	outer 	TRUE
	/declare str_heal_frantic_efficient 	string	outer	NULL
	/declare str_heal_HoT					string	outer	NULL
	/declare bln_heal_HoT					bool 	outer 	${bln_heal_useHoT}
	/declare str_heal_group					string	outer	NULL
	/declare bln_heal_group 				bool 	outer 	${bln_heal_useGroup}

	| [Complete Heal]
	/declare str_heal_complete				string	outer	NULL
	/declare bln_heal_complete				bool 	outer 	TRUE
	/declare bln_heal_complete_member		bool 	outer   TRUE
	/declare int_heal_complete_chain		int     outer   0
	/declare int_heal_complete_target		int  	outer 	0
	/declare bln_heal_complete_paused		bool  	outer 	FALSE
	/declare timer_heal_complete_pause		timer   outer   0

	| Can you believe that you cannot get the HoT amount from any TLO? I've looked.
	/declare int_heal_HoT_perTick			int 	outer	100
	/if (${str_heal_HoT.Find[Pious Elixir]}) 		/varset int_heal_HoT_perTick 1170
	/if (${str_heal_HoT.Find[Holy Elixir]}) 		/varset int_heal_HoT_perTick 900
	/if (${str_heal_HoT.Find[Spiritual Serenity]}) 	/varset int_heal_HoT_perTick 820

	| [Heal_Ranges]
	| Heal ranges are declare dand set in the processHealSpellSettings sub

	| [Heal_Points]
	/declare int_point_self_normal		int		outer	${Ini[INI_HEAL,Heal_Points,int_point_self_normal,75 ]}
	/declare int_point_self_frantic		int		outer	${Ini[INI_HEAL,Heal_Points,int_point_self_frantic,45]}
    /declare int_point_self_HoT			int		outer	${Ini[INI_HEAL,Heal_Points,int_point_self_HoT,95]}
	/declare int_point_tank_normal		int		outer	${If[${_tankHealPointOverride}, ${_tankHealPointOverride}, ${Ini[INI_HEAL,Heal_Points,int_point_tank_normal,75 ]}]}
	/declare int_point_tank_frantic		int		outer	${Ini[INI_HEAL,Heal_Points,int_point_tank_frantic,45]}
	/declare int_point_tank_HoT			int		outer	${Ini[INI_HEAL,Heal_Points,int_point_tank_HoT,95]}
	/declare int_point_group_HoT		int		outer	${Ini[INI_HEAL,Heal_Points,int_point_group_HoT,95]}
	/declare int_point_group_normal		int		outer	${Ini[INI_HEAL,Heal_Points,int_point_group_normal,70]}
	/declare int_point_group_frantic	int		outer	${Ini[INI_HEAL,Heal_Points,int_point_group_frantic,45]}
	/declare int_point_stopHeal			int		outer	${Ini[INI_HEAL,HealHeal_Points_Settings,int_point_stopHeal,96]}

	| [Ressurection]
	/declare bln_heal_ressurection		bool 	outer 	FALSE
	/declare bln_combatRez				bool 	outer 	FALSE
	/declare timer_heal_ressurection	timer	outer	300
	/declare int_heal_range_rez			int 	outer 	100
	/declare bln_heal_rez_skip			bool 	outer 	FALSE

	| [HP_REGEN]
	/declare bln_heal_regen				bool 	outer   FALSE

	| [MANA_REGEN]
	/declare bln_heal_regen_mana		bool 	outer   FALSE

/return

Sub processHealSpellSettings
	/echo \awprocessHealSpellSettings

	/declare i                          int     local	0
    /declare sz                         int     local	0 
    /declare test                       string  local	NULL

	|; _mode|_useGroup|_useHoT|_announce|_autointerrupt|_defaultGem|_interruptToSaveSelf
	| settings=2|TRUE|TRUE|TRUE|TRUE|2|TRUE|
	/varset test ${Ini[INI_CHAR,HEALS,settings]}
	/if (${test.NotEqual[NULL]}) {
	/varset int_heal_mode 					${test.Arg[1,|]}
	/varset bln_heal_useGroup				${test.Arg[2,|]}
	/varset bln_heal_useHoT					${test.Arg[3,|]}
	/varset bln_heal_announce				${test.Arg[4,|]}
	/varset bln_heal_interrupt				${test.Arg[5,|]}
	/varset int_heal_defaultGem				${test.Arg[6,|]}
	/varset bln_heal_interruptToSaveSelf	${test.Arg[7,|]}

	 /echo \a-w    Settings Processed: \awMode:\ag${int_heal_mode} \awAllowGroup:\ag${bln_heal_useGroup} \awAllowHoT:\ag${bln_heal_useHoT} \awAnnounce:\ag${bln_heal_announce} \awDefaultGem:\ag${int_heal_defaultGem} \awAutoInterrupt:\ag${bln_heal_interrupt} \awInterruptToSaveSelf:\ag${bln_heal_interruptToSaveSelf}
	}

	|; _heal|_hp_self|_hp_tank|_hp_group
	| HEAL_normal="Ancient: Hallowed Light"|75|90|70|
	:heals
	|-----------
	| Normal Heal
	|-----------
	/varset     test  						${Ini[INI_CHAR,HEALS,HEAL_normal,NULL]}
	/varset		bln_heal_normal				${test}
	/if (${test.NotEqual[NULL]}) {
		/varset str_heal_normal				${test.Arg[1,|]}
		/varset int_point_self_normal		${test.Arg[2,|]}
		/varset int_point_tank_normal		${test.Arg[3,|]}
		/varset int_point_group_normal		${test.Arg[4,|]}
		/declare int_heal_range_normal		int	outer	${Spell[${str_heal_normal}].MyRange}
	} 

	| Normal Heal Efficient
	/varset     test  						${Ini[INI_CHAR,HEALS,HEAL_normal_efficient,NULL]}
	/if (${test.NotEqual[NULL]}) {
		/varset str_heal_normal_efficient				${test.Arg[1,|]}
		/declare int_heal_range_normal_efficient		int	outer	${Spell[${str_heal_normal_efficient}].MyRange}
	} 			

	|-----------
	| Frantic Heal
	|-----------
	/varset     test  						${Ini[INI_CHAR,HEALS,HEAL_frantic,NULL]}
	/varset		bln_heal_frantic			${test}
	/if (${test.NotEqual[NULL]}) {
		/varset str_heal_frantic			${test.Arg[1,|]}
		/varset int_point_self_frantic		${test.Arg[2,|]}
		/varset int_point_tank_frantic		${test.Arg[3,|]}
		/varset int_point_group_frantic		${test.Arg[4,|]}
		/declare int_heal_range_frantic		int	outer	${Spell[${str_heal_frantic}].MyRange}
	}

	| Frantic Heal Efficient
	/varset     test  						${Ini[INI_CHAR,HEALS,HEAL_frantic_efficient,NULL]}
	/if (${test.NotEqual[NULL]}) {
		/varset str_heal_frantic_efficient			${test.Arg[1,|]}	
		/declare int_heal_range_frantic_efficient	int	outer	${Spell[${str_heal_frantic_efficient}].MyRange}
	}			

	|-----------
	| HoT
	|-----------
	/varset     test  						${Ini[INI_CHAR,HEALS,HEAL_HoT,NULL]}
	/varset		bln_heal_HoT				${test}
	/if (${test.NotEqual[NULL]}) {
		/varset str_heal_HoT				${test.Arg[1,|]}	
		/varset int_point_self_HoT			${test.Arg[2,|]}
		/varset int_point_tank_HoT			${test.Arg[3,|]}
		/varset int_point_group_HoT			${test.Arg[4,|]}
		/declare int_heal_range_HoT			int	outer	${Spell[${str_heal_HoT}].MyRange}
	}
	|-----------
	| Group
	|-----------
	/varset     test  						${Ini[INI_CHAR,HEALS,HEAL_group,NULL]}
	/varset		bln_heal_group				${test}
	/if (${test.NotEqual[NULL]}) 			{
		/varset str_heal_group				${test.Arg[1,|]}
		/declare int_range_group			int	outer	${Spell[${str_heal_group}].MyRange}
	}	

	|-----------
	| Complete
	|-----------
	/varset     test						${Ini[INI_CHAR,HEALS,HEAL_complete,NULL]}
	/varset		bln_heal_complete			${test}
	/if (${test.NotEqual[NULL]}) 			/varset str_heal_complete	${test.Arg[1,|]}
	
	/echo \ao        Spells: \awNormal:\ag(${bln_heal_normal}) ${str_heal_normal} \awFrantic:\ag(${bln_heal_frantic}) ${str_heal_frantic} \awHoT:\ag(${bln_heal_HoT})${str_heal_HoT} \awGroup:\ag(${bln_heal_group}) ${str_heal_group} \awComplete:\ag(${bln_heal_complete}) ${str_heal_complete} 

	/echo \ao        Points: \awNormal:\ag${int_point_self_normal}\a-g|\ag${int_point_tank_normal}\a-g|\ag${int_point_group_normal} \awFrantic:\ag${int_point_self_frantic}\a-g|\ag${int_point_tank_frantic}\a-g|\ag${int_point_group_frantic} \awHoT:\ag${int_point_self_HoT}\a-g|\ag${int_point_tank_HoT}\a-g|\ag${int_point_group_HoT}

	|-----------
	| Divine Arbitration
	|-----------
	|; _divineArbitration|_type|_enabled|_MinHP
	:divineArb
	/varset sz 0
	/if  (${Me.Class.ShortName.Equal[CLR]}) {
		     
		/declare int_DivineArbitration_total int outer 0
		/declare raw_da[3]     string  local NULL
		/varset test ${Ini[INI_CHAR,HEALS,DIVINE_1]}
		/if (${test.NotEqual[NULL]}) /varset bln_heal_useDivineArb	${test.Arg[1,|]}
		/echo \a-w   Settings Processed: \awEnabled:\ag${bln_heal_useDivineArb} 

		/for i 1 to ${raw_da.Size}
			/varset test ${Ini[INI_CHAR,HEALS,DIVINE_${i}]}
			/if (${test.Arg[1,|].Equal[NULL]}) {
				/varset int_DivineArbitration_total ${sz}
				/return
			}
			| Not null, set the values for the debuffs to be cured
			/varcalc sz ${sz}+1
			/declare DIVINE_${i}			string	outer	${Ini[INI_CHAR,HEALS,DIVINE_${i}].Arg[1,|]}
			/declare DIVINE_${i}_type		string	outer	${Ini[INI_CHAR,HEALS,DIVINE_${i}].Arg[2,|]}
			/declare DIVINE_${i}_enabled	string	outer	${Ini[INI_CHAR,HEALS,DIVINE_${i}].Arg[3,|]}
			/declare DIVINE_${i}_MinHP		string	outer	${Ini[INI_CHAR,HEALS,DIVINE_${i}].Arg[4,|]}
			/declare DIVINE_${i}_Ready 		bool    outer   FALSE
			/echo \ao        DIVINE_[\ag${i}\ao]: \awDA:\ag${DIVINE_${i}} \awType:\ag${DIVINE_${i}_type} \awUse:\ag${DIVINE_${i}_enabled} \awminHP:\ag${DIVINE_${i}_MinHP}
 
		/next i
	}

	|-----------
	| Ressurection
	| Currently we only support Cleric Rez, since anything else, and we are obviously fucked
	|-----------
	/varset     test						${Ini[INI_CHAR,HEALS,REZ,NULL]}
	/varset		bln_heal_ressurection		${test}
	
	/if (${test.NotEqual[NULL]}) {
		/varset REZ							${test.Arg[1,|]}
		/varset REZ_Type					${test.Arg[2,|]}
		/varset REZ_enabled						${test.Arg[3,|]}
	}			

/return



Sub processHPRegenSettings
	/echo \awprocessHPRegenSettings
	/declare i                          int     local	0
    /declare sz                         int     local	0 
    /declare test                       string  local	NULL

	|; _enabled
	|settings=TRUE
	/varset test ${Ini[INI_CHAR,HP_REGEN,settings]}
	/if (${test.NotEqual[NULL]}) /varset bln_heal_regen	${test.Arg[1,|]}
	/echo \a-w    Settings Processed: \awEnabled:\ag${bln_heal_regen}     
	|;_regen|_type[AA|ITEM|SPELL]|_enabled|_key|_MGB[TRUE|ATTEMPT|FALSE]
	| REGEN_1="Celestial Regeneration"|AA|TRUE|4|ATTEMPT|
    
	/declare int_heal_regen_total 		int outer 0
	/declare raw_regen[10]     			string  local NULL
	/for i 1 to ${raw_regen.Size}
		/varset test ${Ini[INI_CHAR,HP_REGEN,REGEN_${i}]}
		/if (${test.Arg[1,|].Equal[NULL]}) {
			/varset int_heal_regen_total ${sz}
			/return
		}
		| Not null, set the values for the debuffs to be cured
		/varcalc sz ${sz}+1
		/declare REGEN_HP_${i}			string	outer	${Ini[INI_CHAR,HP_REGEN,REGEN_${i}].Arg[1,|]}
		/declare REGEN_HP_${i}_type		string	outer	${Ini[INI_CHAR,HP_REGEN,REGEN_${i}].Arg[2,|]}
		/declare REGEN_HP_${i}_enabled		string	outer	${Ini[INI_CHAR,HP_REGEN,REGEN_${i}].Arg[3,|]}
		/declare REGEN_HP_${i}_Key		int		outer	${Ini[INI_CHAR,HP_REGEN,REGEN_${i}].Arg[4,|]}
		/declare REGEN_HP_${i}_MGB		bool	outer	${Ini[INI_CHAR,HP_REGEN,REGEN_${i}].Arg[5,|]}
		/echo \ao        REGEN_[\ag${i}\ao]: \awRegen:\ag${REGEN_HP_${i}} \awType:\ag${REGEN_HP_${i}_type} \awUse:\ag${REGEN_HP_${i}_enabled} \awKey:\ag${REGEN_HP_${i}_Key} \awMGB:\ag${REGEN_HP_${i}_MGB} 

	/next i
	|
	| Global Heal INI values, use this to cross reference array index when calling fireRegen[i]
	| Each cleric that has CR should be enumerated in xiris_settings_healing.ini
	| Note this has been generally superceeded by the individual regens having different keys
	| [Celestial_Regeneration] DEPRECATED/UNUSED FOR NOW
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return
	/declare int_heal_celestial_total							int 	outer   ${Ini[INI_HEAL,Celestial_Regeneration,int_totalCR,16]}
	/declare int_heal_celestial_duration 						int  	outer 	${Ini[INI_HEAL,Celestial_Regeneration,int_durationCR,420]}
	/declare int_heal_celestial_index							int 	outer 	0
	/declare arr_heal_celestial[${int_heal_celestial_total}]	string  outer   NULL
	/declare i int local 0
	/for i 1 to ${arr_heal_celestial.Size}
		/varset arr_heal_celestial[${i}]	${Ini[INI_HEAL,Celestial_Regeneration,cr_${i},NULL]}
		/if (${Me.CleanName.Equal[${arr_heal_celestial[${i}]}]}) /varset int_heal_celestial_index ${i}
	/next i
	
/return 


Sub processManaRegenSettings
	/echo \awprocessManaRegenSettings
	/declare i                          int     local	0
    /declare sz                         int     local	0 
    /declare test                       string  local	NULL

	|; _enabled
	|;_regen|_type[AA|ITEM|SPELL]|_enabled|_key|_MGB[TRUE|ATTEMPT|FALSE]
	|settings=TRUE
	/varset test ${Ini[INI_CHAR,MANA_REGEN,settings]}
	/if (${test.NotEqual[NULL]}) /varset bln_heal_regen_mana	${test.Arg[1,|]}
	/echo \a-w    Settings Processed: \awEnabled:\ag${bln_heal_regen_mana}     
	|;_regen|_type[AA|ITEM|SPELL]|_enabled|_key|_MGB[TRUE|ATTEMPT|FALSE]
	| REGEN_1="Celestial Regeneration"|AA|TRUE|4|ATTEMPT|
    
	/declare int_mana_regen_total 		int outer 0
	/declare raw_regen[10]     			string  local NULL
	/for i 1 to ${raw_regen.Size}
		/varset test ${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}]}
		/if (${test.Arg[1,|].Equal[NULL]}) {
			/varset int_mana_regen_total ${sz}
			/return
		}
		| Not null, set the values for the debuffs to be cured
		/varcalc sz ${sz}+1
		/declare REGEN_MANA_${i}			string	outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[1,|]}
		/declare REGEN_MANA_${i}_targets	string	outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[2,|]}
		/declare REGEN_MANA_${i}_Combat		string	outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[3,|]}
		/declare REGEN_MANA_${i}_type		string	outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[4,|]}
		/declare REGEN_MANA_${i}_enabled		string	outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[5,|]}
		/declare REGEN_MANA_${i}_gem		int		outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[6,|]}
		/declare REGEN_MANA_${i}_minMana	int		outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[7,|]}
		/declare REGEN_MANA_${i}_MinHP		int		outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[8,|]}
		/declare REGEN_MANA_${i}_MGB		bool	outer	${Ini[INI_CHAR,MANA_REGEN,REGEN_${i}].Arg[9,|]}
		/echo \ao        REGEN_MANA_[\ag${i}\ao]: \awRegen:\ag${REGEN_MANA_${i}} \awType:\ag${REGEN_MANA_${i}_type} \awUse:\ag${REGEN_MANA_${i}_enabled} \awGem:\ag${REGEN_MANA_${i}_gem} \awMinMana:\ag${REGEN_MANA_${i}_minMana} \awMinHP:\ag${REGEN_MANA_${i}_MinHP} \awMGB:\ag${REGEN_MANA_${i}_MGB} 

	/next i
/return



Sub EVENT_DebugCheckHeal(string _line, string _sender, string _eventParams)
	/echo check Heal Fired ${_eventParams}
	/call setHealingSpell ${_eventParams} self 100 Xanshia ${Me.ID} TRUE
	/echo ${Macro.Return} ${str_heal_current} ${int_heal_interrupt}
	/doevents flush
/return


|**
 ----------------------------
 Check Health Subroutines
 ----------------------------
 These three subroutines are called in order from the Class Macro MainLoop
 checksMT	(and heals)
 checksSELF	(and heals)
 checksGRP	(and heals)
 Healing type changes the index of int_heal_mode
 0-Self (only) | 1-MT (and self) | 2-Group (and MT, includes self) | 3-GroupOnly (includes self) 
 Note: Lazy healing is not currently utilized, but could in the future.
 ----------------------------
**|

Sub chkHeals
	/declare lazy bool local ${If[${bln_engaged},FALSE,TRUE]} 
	/if (${bln_killing} || ${bln_engaged}) /varset lazy FALSE
	/echo \aochkHeals \a-wMT: \ap${If[(${int_heal_mode} && ${int_heal_mode} < 3),TRUE,FALSE]} \a-wSelf:\ap${If[!${int_heal_mode},TRUE,FALSE]} \a-wGroup: \ap${bln_heal_useGroup} \a-wLazy:\ap${lazy} \a-wEngaged?:\ap ${bln_engaged} ${If[${bln_engaged},,\a-wTimer?: \ap${timer_check_heals.OriginalValue}]}
	/if (${Select[${int_heal_mode},1,2]})							/call chkHeal_MT    ${lazy}
 	/if (!${int_heal_mode}) 										/call chkHeal_Self  ${lazy}
	/if (${bln_heal_useGroup} && ${Select[${int_heal_mode},2,3]})	/call chkHeal_Group ${lazy}
	/varset timer_check_heals ${timer_check_heals.OriginalValue}

/return 

Sub chkHeal_MT(bool _lazyMT)
    /echo \awchkHeal_MT(Lazy:\ao${_lazyMT}\aw)
	/declare eff string  local	${If[${bln_heal_useEfficient},_efficient,]} 
	/echo \aweff:\ag${eff} \aw(hp:\ag${Spawn[id ${int_MTID}].PctHPs} \aw< limit\ag${Math.Clamp[${Math.Calc[${int_point_tank_HoT}*1.025]},0,100]} \aw&& Dist:(\ag${Spawn[id ${int_MTID}].Distance3D} \aw<= \ag${int_heal_range_normal${eff}}\aw))
	/if (${Spawn[id ${int_MTID}].PctHPs} < ${Math.Clamp[${Math.Calc[${int_point_tank_HoT}*1.025]},0,100]} && (${Spawn[id ${int_MTID}].Distance3D} <= ${int_heal_range_normal${eff}})) {
		/call chkMTAlive TRUE 
		/call setHealingSpell ${Spawn[id ${int_MTID}].PctHPs} tank ${Spawn[id ${int_MTID}].Distance3D} ${Spawn[id ${int_MTID}].CleanName} ${int_MTID} TRUE
	}
/return

Sub chkHeal_Self(bool _lazySelf)
	/echo \awchkHeal_Self(Lazy:\ao${_lazySelf}\aw)
	/if (${Me.PctHPs} < ${Math.Clamp[${Math.Calc[${int_point_self_HoT}*1.025]},0,100]}) /call setHealingSpell ${Me.PctHPs} self 0 ${Me.CleanName} ${Me.ID} TRUE
/return 

Sub chkHeal_Group(bool _lazyGroup)
    /echo \awchkHeal_Group(Lazy:\ao${_lazyGroup}\aw)
	/if (${Spawn[${WorstHurt[group,1,100,FALSE]}].PctHPs} > ${Math.Clamp[${Math.Calc[${int_point_group_HoT}*1.025]},0,100]} ) /return

	| Determine the counts of major and minor hurt, using the Group.Injured TLO
	/varset int_heal_major ${Group.Injured[${int_point_group_normal}]}
	/varset int_heal_minor ${Math.Calc[${Group.Injured[${int_point_group_normal}]} - ${Group.Injured[${int_point_group_HoT}]}]}

	| Determine if we should use a group heal
	/declare int_inRange int local 0
	/if (${int_heal_major} >=3 && ${Select[${Me.Class.ShortName},CLR,PAL]} && ${bln_heal_group} && (${Me.CurrentMana} > ${Spell[${str_heal_group}].Mana})) {
		/call updateGroupRangeCount ${int_range_group}
		/varset int_inRange ${Int[${Macro.Return}]}
		/if (${int_inRange}>=3) /multiline ; /call castHealV2 ${Me.ID} ${Me.Name} "${str_heal_group}" TRUE FALSE ; /return
	} 
	| Single Target Heals
	/if (${Spawn[${WorstHurt[group,1,100,FALSE]}].Distance3D} > ${int_heal_range_frantic}) /return
	/call setHealingSpell ${Spawn[${WorstHurt[group,1,100,FALSE]}].PctHPs} group ${Spawn[${WorstHurt[group,1,100,FALSE]}].Distance3D} ${Spawn[${WorstHurt[group,1,100,FALSE]}].CleanName} ${Spawn[${WorstHurt[group,1,100,FALSE]}].ID} TRUE
/return

Sub updateGroupRangeCount(int _range) 
	/declare inRange int local 0 
	/declare k int local 0
	/declare member string local
	/for k 0 to 5
		/if (${Group.Member[${k}].Present} && ${Group.Member[${k}].Distance3D} <= ${_range}) /varcalc inRange ${inRange}+1
	/next k
/return ${inRange}


| Based on conditions, determine which healing spell to use
Sub setHealingSpell(int _hp, string _targetType, int _range, string _targetName, string _targetID, bool _castOnReady)
	/echo \aosetHealingSpell \ag${_hp} ${_targetType} ${_range} ${_targetName} ${_targetID} ${_castOnReady}
	/declare pt			int		local	${Math.Clamp[${Math.Calc[${int_point_${_targetType}_HoT}*1.025]},0,100]}
	/if (!${_hp} || ${Me.PctMana} == 0 || (${_hp} > ${pt})) /return NOHEAL

	/if (!${Defined[timer_HoT_${_targetID}]}) /declare timer_HoT_${_targetID} timer outer 0
	/declare eff 		string  local	${If[${bln_heal_useEfficient},_efficient,]}
	| FRANTIC?
	/if (${_hp} <= ${int_point_${_targetType}_frantic} && ${bln_heal_frantic} && (${_range} <= ${int_heal_range_frantic${eff}})) {
		| DIVINE ARBITRATION
		/call chkDivinearbitrationReady
		/if (${bln_heal_useDivineArb} && ${Macro.Return}) {
			/call castDivineArbitration ${Macro.Return}
			/return CASTOK
		}
	
		| FRANTIC
		/varset		str_heal_current	${str_heal_frantic${eff}}
		/varcalc	int_heal_interrupt	${Math.Clamp[${Math.Calc[${int_point_${_targetType}_frantic}*1.1]},0,100]}
		/if (${_castOnReady}) /call castHealV2 ${_targetID} ${_targetName} "${str_heal_current}" FALSE FALSE
		/return CASTOK
	}
	| NORMAL?
	/if (${_hp} <= ${int_point_${_targetType}_normal} && ${bln_heal_normal} && (${_range} <= ${int_heal_range_normal${eff}})) {
		/varset		str_heal_current	${str_heal_normal${eff}}
		/varcalc	int_heal_interrupt	${Math.Clamp[${Math.Calc[${int_point_${_targetType}_normal}*1.1]},0,100]}
		/if (${_castOnReady}) /call castHealV2 ${_targetID} ${_targetName} "${str_heal_current}" FALSE FALSE
		/return CASTOK
	}
	| HoT?
	/if (${_hp} <= ${int_point_${_targetType}_HoT} && ${bln_heal_HoT} && !${timer_HoT_${_targetID}} && (${Me.CurrentMana} > ${Spell[${str_heal_HoT}].Mana}) ) {
		| Determine if target already has HoT, it's per tick value, and how many ticks left
		/declare ptick int local 0
		/declare pduration int local 0
		/dquery ${_targetName} -q "Me.SPA[100]" -o ptick -t 20
		/dquery ${_targetName} -q "Me.Song[${str_heal_HoT}].Duration.TotalSeconds" -o pduration -t 20
		/if (${ptick} > ${int_heal_HoT_perTick} || ${pduration} > 12) /return NOHEAL
		/varset		str_heal_current	${str_heal_HoT}
		/varcalc	int_heal_interrupt	${Math.Clamp[${Math.Calc[${int_point_${_targetType}_HoT}*1.025]},0,100]}
		/if (${_castOnReady}) /call castHealV2 ${_targetID} ${_targetName} "${str_heal_current}" FALSE TRUE
		/return CASTOK
	}
/return 


|-------------------------------------------------------------------------------- 
|SUB: Casting the Heal Spell, will call castHealLoop while casting (for divine arb, ducking out, etc)
|--------------------------------------------------------------------------------
Sub castHealV2(int _id, string _name, string _spell, bool _isGroup, bool _isHoT) 
	/if (${_name.Equal[NULL]} || ${_id} == 0)   /multiline ;  /echo \arERROR in castHealV2: \awName is null, or ID == 0; /return
	/echo \aoHealCast \ay:IsGroup:\ag${_isGroup} \ayTarget:\ag${_name}(${_id}) \aydist:\ag${Target.Distance} \aytargetHP:\ag${Spawn[pc ${_name}].PctHPs}\ao < \ayStopHP:\ag${int_heal_interrupt} \ayspell:\ag"${_spell}" \ayMyMana:\ag ${Me.CurrentMana}\ao > \ag${Spell[${_spell}].Mana}
	/g HealCast Target:${_name}(${_id}) dist:${Target.Distance} targetHP:${Spawn[pc ${_name}].PctHPs} < StopHP:${int_heal_interrupt} spell:"${_spell}" MyMana:${Me.CurrentMana} SpellCost:${Spell[${_spell}].Mana}
	/call TrueTarget ${_id}
	/call chkSpellMemorized "${_spell}" TRUE ${int_heal_defaultGem}
	/if (${_isGroup}) {
		| Don't check health on group spells or you will constantly duck out if cleric doesnt need to be healed!
		/call MQ2Cast "${_spell}" gem${int_heal_defaultGem}
	} else {
		/call MQ2Cast "${_spell}" gem${int_heal_defaultGem} castingHealLoop
	}
	
	/delay 5
	/if (!${Defined[timer_HoT_${_id}]}) /declare timer_HoT_${_id} timer outer 30s
	/if (${_isHoT} && ${Macro.Return.Equal[CAST_SUCCESS]}) /varset timer_HoT_${_id} 30s
	/if (${Macro.Return.Equal[CAST_SUCCESS]} && ${bln_announce}) /docommand /${str_announce_channel} ${_spell} > ${Target.Name} hp:${Target.PctHPs} m:${Me.PctMana} d:${Target.Distance}
/return ${Macro.Return}

| Casting Heal Loop. Will determine if we need to duck out, and also check priority events while doing so (mark of death!)
Sub castingHealLoop
	/if (${Target.Name.Find[s_corpse]}) /multiline ; /stopcast ; /return
	/if (${bln_heal_interrupt} && ${Target.PctHPs} > ${int_heal_interrupt}) /multiline ; /stopcast ; /return
	/if (${bln_heal_interruptToSaveSelf} && ${Me.PctHPs} < ${int_point_self_frantic}) /multiline ; /stopcast ; /call chkHeal_Self FALSE ; /return
	/if (${Me.Class.ShortName.NotEqual[CLR]}) /return

	:divineArbitration
	/call chkDivinearbitrationReady
	/if (${bln_heal_useDivineArb} && ${Spawn[${WorstHurt[group,1,100,FALSE]}].Type.NotEqual[Corpse]} && (${Spawn[${WorstHurt[group,1,100,FALSE]}].PctHPs} < ${DIVINE_1_MinHP}) && ${Macro.Return}) /multiline ; /stopcast ; /call castDivineArbitration ${Macro.Return} ; /return
/return

Sub chkDivinearbitrationReady
	/declare ready 	int local 0
	/declare i 		int local 0
	/for i 1 to ${int_DivineArbitration_total}
		/varset ${DIVINE_${i}_Ready} FALSE
		/if (${DIVINE_${i}_type.Equal[ITEM]} && ${DIVINE_${i}_enabled} && !${FindItem[=${DIVINE_${i}}].TimerReady}) {
			/varset ${DIVINE_${i}_Ready} TRUE
			/varset ready ${i}
		} 
		/if (${DIVINE_${i}_type.Equal[AA]} && ${DIVINE_${i}_enabled} && ${Me.AltAbilityReady[${DIVINE_${i}}]}) {
			/varset ${DIVINE_${i}_Ready} TRUE
			/varset ready ${i}
		}     
	/next i
/return ${ready}

Sub castDivineArbitration(int _index)
	/call MQ2Cast "${DIVINE_${_index}}" ${If[${DIVINE_${_index}_type.Equal[AA]},AA,]}
/return



|-------------------------------------------------------------------------------- 
|SUB: Check Resurrection - Clerics only
|--------------------------------------------------------------------------------
Sub chkRessurection
	/if (!${bln_heal_ressurection} || (${bln_engaged} && !${bln_combatRez}) || (!${bln_engaged} && ${timer_heal_ressurection}) || !${SpawnCount[corpse radius ${int_heal_range_rez}]}) /return 
	/if (${SpawnCount[corpse]}) /squelch /hidecorpse npc
	/call RessurectCorpses
	/varset timer_heal_ressurection ${timer_heal_ressurection.OriginalValue}
/return 

Sub RessurectCorpses
	/declare i 		int		local 0
	/declare id		int     local 0
	
	/for i 1 to ${SpawnCount[corpse radius ${int_heal_range_rez}]}
		/squelch /hidecorpse npc
		/dquery ${NearestSpawn[${i},corpse radius ${int_heal_range_rez}].CleanName} -q ${Me.ID} -o id -t 20
		/if (!${id}) /continue
		/call TrueTarget ${id}
		/consider
		/delay 5
		/varset bln_heal_rez_skip FALSE
		/doevents CANNOTREZ 
		/if (${bln_heal_rez_skip}) /continue 
		/call castRessurection ${id}
		/varset id 0
	/next i	
/return

| Event only ever used by the rez routine;
Sub EVENT_CANNOTREZ
	/doevents flush CANNOTREZ
	/if (${Target.ID}) /varset bln_heal_rez_skip TRUE
/return

Sub castRessurection(int _id)
	/dex  ${Spawn[id ${_id}].CleanName.Arg[1,']} /consent ${Me.Name}
	/delay 5
	/corpse 
	/if (${REZ_Type.Equal[ITEM]} && ${FindItem[${REZ}].ID}) {
		/call MQ2Cast "${REZ}"  ITEM
		/if (${Macro.Return.Equal[CAST_SUCCESS]}) /docommand /${str_announce_channel} Rezzed ${Target.Name}
	}
/return 

|-------------------------------------------------------------------------------- 
|SUB: Complete Heal Chain - Clerics Only
|--------------------------------------------------------------------------------
Sub EVENT_CHStart(string _line, string _sender, string _eventParams)
	/doevents flush CHStart
	/call getCHPosition ${_eventParams.Arg[5]}
	/echo ${Macro.Return}
	/if (${Macro.Return}) /call heal_chStart "${_eventParams}" ${Macro.Return}
/return 

Sub EVENT_CHStop(string _line, string _sender, string _eventParams)
	/doevents flush CHStop 
	/if (!${bln_heal_complete_member}) /return 
	/call heal_chStop ${_eventParams}
/return

Sub EVENT_CHPause(string _line, string _sender, string _eventParams)
	/doevents flush CHPause
	/call heal_chPause ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
/return 

Sub EVENT_CHResume(string _line, string _sender, string _eventParams)
	/doevents flush CHResume
	/call heal_chResume ${_eventParams}
/return 

Sub EVENT_CHSwitch(string _line, string _sender, string _eventParams)
	/doevents flush CHSwitch
	/call heal_chSwitch ${_eventParams.Arg[1]} ${_eventParams.Arg[2]}
/return 



| gets the CH postion in the complete heal order string
Sub getCHPosition(string _order)
	/declare i int local 0
	/declare p int local 0
	/echo ${Math.Calc[${_order.Count[,]}+1]} count
	/for i 1 to ${Math.Calc[${_order.Count[,]}+1]}
		/echo ${i} ${_order.Arg[${i},,]}
		/if (${_order.Arg[${i},,].Equal[${Me.CleanName}]}) /return ${i}
	/next i
/return 0

Sub heal_chStart(string _settings, int _position)
	/echo heal_chStart (${_settings}) :: ${_position}
	/call ChangeHealMode CHCHAIN
	/varset bln_heal_complete_member TRUE
	/varset _position ${Math.Calc[${_position} - 1 ]}

	/varset  int_heal_complete_chain	${_settings.Arg[1]}
	/declare t1		string	local		${Spawn[pc ${_settings.Arg[2]}].ID}
	/echo t1: ${t1} ${Spawn[pc ${_settings.Arg[2]}]}
	/declare t2		string	local 		${Spawn[pc ${_settings.Arg[3]}].ID}
	/echo t1: ${t2} ${Spawn[pc ${_settings.Arg[2]}]}
	/declare delay	timer	local		0
	/declare sd 	int   	local 		${Math.Calc[${_settings.Arg[4]} * ${_position} + 1]}

	/call chkSpellMemorized "${str_heal_complete}"	TRUE 9

	:chStart
	/varset  int_heal_complete_target	${t1}
	/echo \a-wStarting Complete Heal Chain \ao[\ag\ag${int_heal_complete_chain} \ao @ \ag${sd}\a-gms\ao] on \ag${t1}
	/varset  delay ${sd}
	/echo delay ${delay}
	:chWaitLoop
	/if (${delay}) {
		 /call RunAllEvents
		 /goto :chWaitLoop
	}
	
	:chLoop
	| Check properties that might have changed during the chWaitLoop EVENT checking
	| No longer a chain member, target switched, t1 died, t2 died, oom
	/echo \ao chLoop member? \ag${bln_heal_complete_member} \ao t1\ag ${t1}
	/if (!${bln_heal_complete_member}) /return
	/if (${Spawn[id ${t1}].ID} && !${t1.NotEqual[${t2}]}) /varset t1 ${t2}
	/echo count ${t1} !${Spawn[id ${t1}].ID} || ${Math.Calc[${Spell[${str_heal_complete}].Mana}+20]} > ${Me.CurrentMana}
	/if (!${Spawn[id ${t1}].ID}) {
		/echo ch failed due to no ${t1} spawn ${Spawn[id ${t1}].ID}
		/return
	} 

	/if (${Math.Calc[${Spell[${str_heal_complete}].Mana}+20]} > ${Me.CurrentMana}) {
		/echo failed due to lack of mana
		/return

	}

		| Loop is valid, running
		/call RunAllEvents
		/if (${Spawn[id ${t1}].Distance3D} > ${Spell[${str_heal_complete}].MyRange}) /goto :chCastingLoop
		/if (${timer_heal_complete_pause}) /goto :chPauseLoop
		/call TrueTarget ${t1}

		:chCast
		/docommand /${str_announce_channel} [${int_heal_complete_chain}]:[${_position}] CHC->${Spawn[id ${t1}].CleanName} hp:${Target.PctHPs} d:${Target.Distance3D} m:${Me.PctMana}
		/call MQ2Cast "${str_heal_complete}" RunPriorityEvents
		/if (${Macro.Return.Equal[CAST_FIZZLE]}) /goto :chCast
		/call RunAllEvents
		/varset delay ${sd}
		/goto :chWaitLoop
	
	:chPauseLoop 
		/call RunAllEvents
		/if (!${timer_heal_complete_pause}) /goto :chLoop
	/goto :chPauseLoop 
/return 

Sub heal_chStop(int _chainID)
	/if (${int_heal_complete_chain} != ${_chainID} || !${bln_heal_complete_member}) /return 
	/echo \a-wStopping Complete Heal Chain \ao[\ag${int_heal_complete_chain}\ao]
	/if (${Me.Casting.ID}) 							/stopcast
	/varset bln_heal_complete_member 				FALSE
	/call ChangeHealMode ${str_casting_spellSet_saved}
/return 

Sub heal_chPause(int _chainID, int _ms)
	/if (${_chainID} != ${int_heal_complete_chain} || !${bln_heal_complete_member}) /return
	/echo \a-wPausing Complete Heal Chain \ao[\ag${int_heal_complete_chain}\ao]
	/if (${Me.Casting.ID}) 							/stopcast
	/varset timer_heal_complete_pause ${ms}
/return 

Sub heal_chResume(int _chainID)
	/if (${_chainID} == ${int_heal_complete_chain} && ${bln_heal_complete_member}) {
		/echo \a-wResuming Complete Heal Chain \ao[\ag${int_heal_complete_chain}\ao]
		/varset timer_heal_complete_pause 0
	}
/return 

Sub heal_chSwitch(int _chainID, string _newT1)
	/if (${_chainID} == ${int_heal_complete_chain} && ${bln_heal_complete_member}) {
		/echo \a-wSwitching Complete Heal Chain \ao[\ag${int_heal_complete_chain} \ao- \ag ${_newT1}\ao]
		/if (${Me.Casting.ID}) /stopcast
		/varset int_heal_complete_target ${Spawn[pc ${_newT1}].ID}
 	}
/return

|**
 ----------------------------
 HealType
 ----------------------------
 Healing type changes the index of int_heal_mode
 0-Self (only) | 1-MT (and self) | 2-Group (and MT, includes self) | 3-GroupOnly (includes self) | 
 ----------------------------
**|
Sub EVENT_HealType(string _line, string _sender, string _eventParams)
	/doevents flush HealType
	/call ChangeHealType ${_eventParams}
	
/return

Sub ChangeHealType(int _type) 
	/varset int_heal_mode ${_type}
	/if (${int_heal_mode}==0) /echo \a-wChanging healing \aoTYPE \a-wto \ag0-Self
	/if (${int_heal_mode}==1) /echo \a-wChanging healing \aoTYPE \a-wto \ag1-MT (and self)
	/if (${int_heal_mode}==2) /echo \a-wChanging healing \aoTYPE \a-wto \ag2-Group (and MT, includes self)
	/if (${int_heal_mode}==3) /echo \a-wChanging healing \aoTYPE \a-wto \ag3-GroupOnly (includes self)
/return

|**
 ----------------------------
 HealMode
 ----------------------------
 We need to have several different healing modes based on what encounter we want setup
 Example, Zun`Muram Kvxe Pirik needs a very efficient heal mode from the spam healers for 
 the first 70% of the event, so for clerics that would be Ethereal Lights and CHChain
 this will be handled in the setHealingSpell sub.
 NORMAL|EFFICIENT|CHCHAIN
 ----------------------------
**|
Sub EVENT_HealMode(string _line, string _sender, string _eventParams)
	/doevents flush HealMode
	/call ChangeHealMode ${_eventParams}
	
/return

Sub ChangeHealMode(string _strMode)
	|- Mem the appropriate spell set
	/echo \a-wChanging healing \aoMODE \a-wto \ag${_strMode}
	/varset str_casting_spellSet_saved ${str_casting_spellSet}
	/if (${_strMode.Find[CHCHAIN]}) {
		/varset str_casting_spellSet	default
		/varset bln_heal_useHoT 		FALSE
		/varset bln_heal_useEfficient	FALSE
		/call   setHealInterrupt 		FALSE
	} else /if (${_strMode.Find[EFFICIENT]}) {
		/varset str_casting_spellSet 	efficient
		/varset bln_heal_useHoT 		FALSE
		/varset bln_heal_useEfficient 	TRUE
		/call   setHealInterrupt 		TRUE
	} else {
		/varset str_casting_spellSet 	default
		/varset bln_heal_useHoT 		FALSE
		/call   setHealInterrupt 		TRUE
	}
	/memspellset ${str_casting_spellSet}
/return

Sub setHealInterrupt(bool _interrupt)
	/varset bln_heal_interrupt ${_interrupt}
/return

|**
 ----------------------------
 Divine Intervention
 ----------------------------
 Divine Intervention Methods for CLR only
 ----------------------------
**|

Sub EVENT_DivineInterventionRequest(string _line, string _sender, string _eventParams)
	/doevents flush DivineInterventionRequest
	/if (!${bln_heal_useDivineIntervention}) /return 
	/call castDivineIntervention ${_sender}
/return


Sub castDivineIntervention(string _targetName) 
	/if (!${FindItemCount[Emerald]}) {
		/docommand /${str_announce_channel} Cannot DI ${_targetName} out of Emeralds
	}
	/if (${Me.SpellReady[Divine Intervention]}) {
		/call TrueTarget ${Spawn[pc ${_targetName}].ID}
		/call MQ2Cast "Divine Intervention"
		/docommand /${str_announce_channel} Casting DI ON ${_targetName}
	} else {
		/docommand /${str_announce_channel} Cannot DI ${_targetName} spell not Ready
	}
/return

|**
 ----------------------------
 Regeneration Settings
 ----------------------------
 TODO #7
 ManaRegen SELF  is generally self regens that are instant, like Cannibalization
 ManaRegen GROUP is generally group regens that are duration, like Paragon of Spirit
 HPRegen   is generally assumed GROUP healing things like totems or celestial healing type AA
 ----------------------------
**|

Sub EVENT_FireRegen(string _line, string _sender, string _eventParams)
	/doevents flush FireRegen
	/call castHPRegen ${_eventParams}
/return


Sub castHPRegen(int _key) 
	/declare i int local 0
	/for i 1 to ${int_heal_regen_total}
		/if (${REGEN_HP_${i}_Key} == ${_key} ) {
			/call attemptRegen ${REGEN_HP_${i}_type} ${REGEN_HP_${i}} ${REGEN_HP_${i}_MGB}
		}
	/next i
/return

|-- Mana Regen out of combat (cannibalization)
Sub chkManaRegen
	/declare i int local 0
	/for i 1 to ${int_mana_regen_total}
		/if (${REGEN_MANA_${i}_targets.Equal[SELF]} && ${Me.PctMana} <= ${REGEN_MANA_${i}_minMana} && ${Me.PctHPs} >= ${REGEN_MANA_${i}_MinHP}) {
			/call attemptRegen ${REGEN_MANA_${i}_type} ${REGEN_MANA_${i}} ${REGEN_MANA_${i}_MGB}
		}
	/next i
/return 

|-- Forced Group Mana Regen (paragon)
Sub EVENT_FireManaRegen(string _line, string _sender, string _eventParams)
	/doevents flush FireManaRegen
	/call castManaRegen ${_eventParams}
/return

Sub castManaRegen(int _key)
	/declare i int local 0
	/for i 1 to ${int_mana_regen_total}
		/if (${REGEN_MANA_${i}_Key} == ${_key} && ${REGEN_MANA_${i}_targets.Equal[GROUP]} ) {
			/call attemptRegen ${REGEN_MANA_${i}_type} ${REGEN_MANA_${i}} ${REGEN_MANA_${i}_MGB}
		}
	/next i
/return

| Regen attempt routine, works for both hp and mana
Sub attemptRegen(string _type, string _regen, string _mgb)
	/if (${_mgb.Equal[ATTEMPT]}) {
		/if (${Me.AltAbilityReady[Mass Group Buff]}) /call MQ2Cast "Mass Group Buff" ALT
	} else /if  (${_mgb.Equal[TRUE]}) {
		/if (${Me.AltAbilityReady[Mass Group Buff]}) {
			/call  MQ2Cast "Mass Group Buff" ALT
		} else {
			/return
		}
	} 

	| Cast the Regen
	| Note bards are excluded because they have no AE regen ability
	/if (${_type.Equal[AA]} && ${Me.AltAbilityReady[${_AA}]})           /call MQ2Cast   "${_regen}" ALT
	/if (${_type.Equal[DISC]} && ${Me.CombatAbilityReady[${_AA}]})      /doability      "${_regen}"
	/if (${_type.Equal[ITEM]} && !${FindItem[=${_burn}].TimerReady})    /call MQ2Cast   "${_regen}" ITEM
	/if (${_type.Equal[SPELL]})                                         /call MQ2Cast   "${_regen}"
    
/return 