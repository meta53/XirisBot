|**
----------------------------
xiris_burns.inc
----------------------------
Burn Mode subs. AA/CLICK/BURN items/AA/disc handlers
______________________________________________
REVISION HISTORY
	12.27.22	xiris	INITIAL REVISION - Moved from xiris_common.inc
    12.31.22    xiris   Moved AA, Clicky, Burns to this file, and changed their initialization
                        Requires ini file changes in BURN/CLICKY/AA
                        Changed Bards medley calls to use new method in spell_routines.inc
	07.24.23	xiris	V2.0 Rewrote majority of code to handle new INI implementations					

**|    


#EVENT Burn_ON_ALL      "#*#BurnOnAll#*#"
#EVENT Burn_ON_1        "#*#BurnOn1#*#"
#EVENT Burn_ON_2        "#*#BurnOn2#*#"
#EVENT Burn_OFF			"#*#BurnOff#*#"


Sub initEvents_Burning
    /call RegisterRaidEvents Events_Burning 4
/return 

Sub Events_Burning
	/doevents Burn_ON_ALL
	/doevents Burn_ON_1
	/doevents Burn_ON_2
    /doevents Burn_OFF
    /if (${bln_burning} && ${bln_engaged} &&  ${bln_killing} && ${int_burn_total}) /call chkBurns
    /if (${Select[${Me.Class.ShortName},MNK,PAL,RNG,SHD,WAR]} && !${timer_cryCheck} && !${bln_tanking}) /call chkCryActive
/return


Sub EVENT_Burn_ON_ALL
    /if (!${bln_engaged} && !${bln_killing}) /return
    /varset bln_burning     TRUE
    /doevents flush Burn_ON_ALL
/return 

Sub EVENT_Burn_ON_1
    /if (!${bln_engaged} || ${int_burn_key} != 1 || !${bln_killing}) /return
    /varset bln_burning     TRUE
    /varset timer_cryCheck  0
    /doevents flush Burn_ON_1
/return 

Sub EVENT_Burn_ON_2
    /if (!${bln_engaged} || ${int_burn_key} != 2 || !${bln_killing}) /return
    /varset bln_burning     TRUE
    /varset timer_cryCheck  0
    /doevents flush Burn_ON_2
/return

Sub EVENT_Burn_OFF
    /varset bln_burning     FALSE
    /varset timer_cryCheck  300
    /doevents flush Burn_OFF
/return

Sub xbot_initialize_burning
    /echo \n \apInitialize: \aoBurning \aw-------------------------

    /call processBurns
    /call initEvents_Burning
/return


Sub processBurns
    /echo \awprocessBurns
    
    |; _enabled|_key|_cry2hander
	| settings=TRUE|1|FALSE
    /declare bln_burns_enabled   bool    outer   TRUE
    /declare int_burn_key       int     outer   1
    /declare int_burn_total     int     outer   0
    /declare bln_burning        bool    outer   FALSE
    /declare bln_cry2Hander     bool    outer   TRUE
    /declare bln_cryRunning     bool    outer   FALSE
    /declare timer_cryCheck     timer   outer   0

    /if (!${Ini[INI_CHAR,BURNS,settings].Arg[1,|]})  /varset bln_burns_enabled    FALSE
    /if (!${Ini[INI_CHAR,BURNS,settings].Arg[2,|]})  /varset int_burn_key       1
    /if (!${Ini[INI_CHAR,BURNS,settings].Arg[3,|]})  /varset bln_cry2Hander     FALSE

    /echo \a-w    Settings Processed: \awEnabled:\ag${bln_burns_enabled} \awKey:\ag${int_burn_key}
    |;_burn|_type[AA|ITEM|SPELL|DISC]|_enabled|_afterIndex|_MGB[TRUE|ATTEMPT|FALSE]
    /declare raw_burn[20]       string  local   NULL
    /declare i                  int     local   0
    /declare sz                 int     local   0
    /declare test               string  local   NULL

    /for i 1 to ${raw_burn.Size}
        /varset test ${Ini[INI_CHAR,BURNS,BURN_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_burn_total ${sz}
            /return
        }
        | Not null, set the values for the burns
        /varcalc sz ${sz}+1
        /declare BURN_${i}          string  outer   ${Ini[INI_CHAR,BURNS,BURN_${i}].Arg[1,|]}
        /declare BURN_${i}_type     string  outer   ${Ini[INI_CHAR,BURNS,BURN_${i}].Arg[2,|]}
        /declare BURN_${i}_enabled      bool    outer   ${Ini[INI_CHAR,BURNS,BURN_${i}].Arg[3,|]}       
        /declare BURN_${i}_after    int     outer   ${Ini[INI_CHAR,BURNS,BURN_${i}].Arg[4,|]}
        /declare BURN_${i}_MGB      string  outer   ${Ini[INI_CHAR,BURNS,BURN_${i}].Arg[5,|]}
         /echo \ao        BURN[\ag${i}\ao]: \awName:\ag${BURN_${i}} \awType:\ag${BURN_${i}_type} \awEnabled:\ag${BURN_${i}_enabled} \awAfter:\ag${BURN_${i}_after}
    /next i
/return



| When the toon has warcry on, then we need to make sure
| we are using 2handers on MNK,PAL,RNG,SHD
|**
[INVENTORY]
	leftfinger="Ring of Disdain"
	rightfinger="Ring of Ire Intent"

	; = _mainHand|_offHand|_range
	SET_default="Notched Blade of Bloodletting"|"Mace of Tortured Nightmares"|NULL
	SET_slashing="Notched Blade of Bloodletting"|"Blade of Natural Turmoil"|NULL
    SET_2handed="Staff of the Worldwalker"|NULL|NULL
**|
Sub chkCryActive
    /if (${Me.Song[Ancient: Cry of Chaos].ID} && !${bln_cryRunning}) {
		/call LoadWeaponset 2HANDER
		/varset bln_cryRunning TRUE
        /varset timer_cryCheck 300
	} else /if (!${Me.Song[Ancient: Cry of Chaos].ID} && ${bln_cryRunning}) {
		/call LoadWeaponset DEFAULT
		/varset bln_cryRunning FALSE
        /varset timer_cryCheck 50
	}
/return

|**
 ----------------------------
 Check BURN routine
 ----------------------------
 Fires the burns if an event has set bln_burning TRUE
 ----------------------------
**|
Sub chkBurns
    /if (!${Target.ID} || ${Target.Type.NotEqual[NPC]} || !${bln_engaged} || !${bln_killing} || !${bln_burning} || !${int_burn_total} ||  (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) || ${Me.Invis} || ${Me.Feigning}) /return

    /declare i  int local   0

    /for i 1 to ${int_burn_total}
        /if (!${BURN_${i}_enabled}) /goto :nextBurn
        /call attemptBurn ${BURN_${i}_type} ${BURN_${i}} ${BURN_${i}_after} ${BURN_${i}_MGB}
    :nextBurn
    /next i
/return 


Sub attemptBurn(string _type, string _burn, int _after, string _mgb)
    /if (${_after} > 0) {
        | Checking to see if the previous ability was fired
        /if (${BURN_${_after}_type.Equal[AA]}   &&  ${Me.AltAbilityReady[${BURN_${_after}}]})    /return 
        /if (${BURN_${_after}_type.Equal[DISC]} &&  ${Me.CombatAbilityReady[${BURN_${_after}}]}) /return
    } else {
        | No prior requirements | Check for MGB (Bards cant MGB so ignore them)
        /if (${_mgb.Equal[ATTEMPT]}) {
            /if (${Me.AltAbilityReady[Mass Group Buff]}) /call MQ2Cast "Mass Group Buff" ALT
        } else /if  (${_mgb.Equal[TRUE]}) {
            /if (${Me.AltAbilityReady[Mass Group Buff]}) {
                /call  MQ2Cast "Mass Group Buff" ALT
            } else {
                /return
            }
        } 
    }
    | Valid burn, cast it
    /if (${Me.Class.ShortName.Equal[BRD]}) {
        /if (${_type.Equal[AA]} && ${Me.AltAbilityReady[${_burn}]})           /call MQ2Medley "${_burn}" ALT  ${int_targetID} TRUE
        /if (${_type.Equal[DISC]} && ${Me.CombatAbilityReady[${_burn}]})      /doability      "${_burn}"
        /if (${_type.Equal[ITEM]} && !${FindItem[=${_burn}].TimerReady})    /call MQ2Medley "${_burn}" ITEM ${int_targetID} TRUE
        /if (${_type.Equal[SPELL]})                                         /call MQ2Medley "${_burn}" SONG
    } else {
        /if (${_type.Equal[AA]} && ${Me.AltAbilityReady[${_burn}]})           /call MQ2Cast   "${_burn}" ALT
        /if (${_type.Equal[DISC]} && ${Me.CombatAbilityReady[${_burn}]})      /doability      "${_burn}"
        /if (${_type.Equal[ITEM]} && !${FindItem[=${_burn}].TimerReady})    /call MQ2Cast   "${_burn}" ITEM
        /if (${_type.Equal[SPELL]})                                         /call MQ2Cast   "${_burn}"
    }
/return
