
|**
----------------------------
xiris_class_pal.inc
----------------------------
Allows for Paladin specific events
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
    10.19.23    xiris   Added Paladin Class Events
______________________________________________
**|	

#EVENT STUNMOB "<#1#> stunThis #2#"
#EVENT STUNMOB "[#1#(msg)] stunThis #2#"
#EVENT STUNMOB "#1# tells you,#*#'stunThis #2#'"
#EVENT STUNMOB "#1# tells the group#*#'stunThis #2#'"
#EVENT STUNMOB "#1#tells the raid,#*#'stunThis #2#'"

#EVENT STUNLOCK "<#1#> stunLock #2#"
#EVENT STUNLOCK "[#1#(msg)] stunLock #2#"
#EVENT STUNLOCK "#1# tells you,#*#'stunLock #2#'"
#EVENT STUNLOCK "#1# tells the group#*#'stunLock #2#'"
#EVENT STUNLOCK "#1#tells the raid,#*#'stunLock #2#'"

#EVENT STUNSTOP "<#1#> stunStop#*#"
#EVENT STUNSTOP "[#1#(msg)] stunStop#*#"
#EVENT STUNSTOP "#1# tells you,#*#'stunStop#*#'"
#EVENT STUNSTOP "#1# tells the group#*#'stunStop#*#'"
#EVENT STUNSTOP "#1#tells the raid,#*#'stunStop#*#'"

Sub initEvents_Class_PAL
    /call RegisterClassEvents Events_Class_${Me.Class.ShortName}
/return 

Sub Events_Class_PAL
	/doevents STUNMOB
	/doevents STUNLOCK
	/doevents STUNSTOP
/return

Sub xbot_initialize_class_PAL
    /call initEvents_Class_PAL
/return

Sub EVENT_STUNMOB(string _line, string _sender, string _eventParams)
	/echo \ayI should stun this mob
	/call TrueTarget(${_eventParams.Arg[1, ]})
	/call chkStun TRUE
	/doevents flush STUNMOB
/return

Sub EVENT_STUNLOCK(string _line, string _sender, string _eventParams)
	/echo \aoI am Stun Locking this mob
	/call TrueTarget(${_eventParams.Arg[1, ]})
	/call chkStunLock
	/doevents flush STUNLOCK
/return

Sub EVENT_STUNSTOP(string _line, string _sender, string _eventParams)
	/echo \agI am no lnger Stun Locking the mob
	/call chkStunLockStop
	/doevents flush STUNLOCKSTOP
/return


Sub chkStunLock
    /echo \ar Hey asshole, implement this!
/return

Sub chkStunLockStop 

/return