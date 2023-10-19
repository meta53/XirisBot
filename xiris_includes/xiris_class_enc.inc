
|**
----------------------------
xiris_class_enc.inc
----------------------------
Allows for Enchanter specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Enchanter Class Events
______________________________________________
**|	

#EVENT RUNEON "<#1#> RuneOn#*#"
#EVENT RUNEON "[#1#(msg)] RuneOn#*#"
#EVENT RUNEON "#1# tells you,#*#'RuneOn#*#'"
#EVENT RUNEON "#1# tells the group#*#'RuneOn#*#'"
#EVENT RUNEON "#1#tells the raid,#*#'RuneOn#*#'"

#EVENT RUNEONName "<#1#> RuneOn #2#"
#EVENT RUNEONName "[#1#(msg)] RuneOn #2#"
#EVENT RUNEONName "#1# tells you,#*#'RuneOn #2#'"
#EVENT RUNEONName "#1# tells the group#*#'RuneOn #2#'"
#EVENT RUNEONName "#1#tells the raid,#*#'RuneOn #2#'"

#EVENT RUNEOFF "<#1#> RuneOff#*#"
#EVENT RUNEOFF "[#1#(msg)] RuneOff#*#"
#EVENT RUNEOFF "#1# tells you,#*#'RuneOff#*#'"
#EVENT RUNEOFF "#1# tells the group#*#'RuneOff#*#'"
#EVENT RUNEOFF "#1#tells the raid,#*#'RuneOff#*#'"



Sub initEvents_Class_ENC
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_ENC
	/doevents RUNEON
	/doevents RUNEONName
	/doevents RUNEOFF
/return

Sub xbot_initialize_class_ENC
    /call initEvents_Class_ENC
/return

|**
 ----------------------------
 RUNIC Subs
 ----------------------------
 Handles the chain runing of target (usually MT)
 ----------------------------
**|



Sub EVENT_RUNEON(string _line, string _sender, string _eventParams)
	/varset bln_runeActive TRUE
	/call RuneSpam ${_sender}
/return

Sub EVENT_RUNEONName(string _line, string _sender, string _eventParams)
	/varset bln_runeActive TRUE
	/call RuneSpam ${_eventParams}
/return

Sub RuneSpam(_name)
	/echo Runing ${_name}:${str_rune} ${str_runeGem}
	/call chkSpellMemorized "${str_rune}" TRUE ${str_runeGem}
	:runeLoop
		/call TrueTarget ${Spawn[pc ${_name}].ID}
		/delay 30s ${Me.SpellReady[${str_rune}]}
		/call MQ2Cast "${str_rune}" ${str_runeGem} 5s
		/call RunAllEvents
		/if (${bln_runeActive}) /goto :runeLoop
/return

Sub EVENT_RUNEOFF
	/call RuneSpamOff
/return
	
Sub RuneSpamOff
	/echo --turning rune off--
	/varset bln_runeActive FALSE
/return
