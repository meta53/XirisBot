|**
----------------------------
xiris_movement.inc
----------------------------
This was written ground up for usage with xiris's bots
Allows to bln_following via stick or nav
______________________________________________
REQUIRED PLUGINS
    mq2nav
	mq2moveutils
______________________________________________
REVISION HISTORY
	04.05.22    xiris   INITIAL REVISION
	06.13.22	xiris   Updated events (minor)
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
	08.01.23	xiris	Cleaned up unused events and subs
______________________________________________
**|	

#EVENT Follow		"#1# tells you#*#'FollowMe #2#'"
#EVENT Follow		"#1# tells the group#*#'FollowMe #2#'"
#EVENT Follow		"#1# tells the raid#*#'FollowMe #2#'"
#EVENT Follow 		"[ #1# #*#]#*#FollowMe #2#"

#EVENT Stop			"#1# tells you#*#'NavStop#*#'"
#EVENT Stop			"#1# tells the group#*#'NavStop#*#'"
#EVENT Stop			"#1# tells the raid#*#'NavStop#*#'"
#EVENT Stop 		"[ #1# #*#]#*#NavStop#*#"

#EVENT NavigateWP	"#1# tells you#*#'NavToWP #2#'"
#EVENT NavigateWP	"#1# tells the group#*#'NavToWP #2#'"
#EVENT NavigateWP	"#1# tells the raid#*#'NavToWP #2#'"
#EVENT NavigateWP 	"[ #1# #*#]#*#NavToWP #2#"


Sub initEvents_Movement
	/call RegisterBackgroundEvents Events_Movement_Background 5
/return

Sub Events_Movement_Background
	/doevents NavigateWP
	/doevents Follow
	/doevents Stop
/return


Sub xbot_initialize_movement
	/echo \n \apInitialize: \aoMovement \aw-------------------------
	/declare bln_following 			bool 	outer FALSE
	/declare int_maxFollowDistance	int 	outer 1000
	/declare str_followTarget		string  outer 
	/call initEvents_Movement
/return

SUB EVENT_Follow(string _line, string _sender, string _eventParams)
	/doevents flush Follow
	/echo followin ${_sender} ${_eventParams}
	/if (${_sender.Equal[${Me.CleanName}]}) /return
	/declare followType 	string 	local 	${If[${_eventParams.Find[/type|NAV]},NAV,STICK]}
	/call verifyEvent "${_sender}" "${_eventParams}"
	/if (${Bool[${Macro.Return.Arg[2,_]}]} && ${Bool[${Macro.Return.Arg[3,_]}]} && ${Bool[${Macro.Return.Arg[4,_]}]}) {
		/call StartFollowing ${_sender} ${followType}
	}
	
/return


SUB EVENT_Stop(string _line, string _sender, string _eventParams)
	| -Check if I'm bln_following.
	/if (${bln_following}) /call StopFollowing
	/doevents flush Stop
/return


SUB EVENT_NavigateWP(string _line, string _sender, string _eventParams)
	/echo \ayNavigate to waypoint: \ag${_eventParams}
	/declare wp string local ${_eventParams.Arg[1]}
	/nav wp ${wp}
	/doevents flush NavigateWP
/return


Sub StartFollowing(string _leader, string _followType)
	/varset bln_following TRUE
	/echo \ao Following \awLeader: \ag${_leader} \awFollowType: \ag${_followType}
	/if (!${Me.Standing}) /stand on
	| -Check Range
	/if (${Spawn[=${_leader}].Distance} > ${int_maxFollowDistance}) {
		/docommand /${str_command_channel} [${str_followTarget}] is too far away.
	} else {
		/echo \ao StartFollowing, so call disengage
		/call Disengage
		| -Check LoS.
		/if (${_followType.Equal[NAV]} || (${_followType.NotEqual[NAV]} && ${Spawn[=${_leader}].LineOfSight})) {
			/call AquireFollow ${_followType} ${_leader}
		} else {
			/docommand /${str_command_channel} _followType != NAV || STICK cannot see [${_leader}]
		}
	} 
/return

Sub StopFollowing
	/varset bln_following 	 FALSE
	/squelch /stick off
	/if (${Navigation.Active}) 		/squelch /nav stop
/return


SUB AquireFollow(string _type, string _leader)
	/if (${SpawnCount[=${_leader}]} && ${SpawnCount[=${_leader} radius ${int_maxFollowDistance}]} && ${_leader.NotEqual[${Me.CleanName}]}) {
		
		| store the follow target in case we need it in another sub (unlikely)
		/varset str_followTarget		${_leader}
		/if (${Stick.Active}) 			/squelch /stick off
		/if (${Navigation.Active}) 		/squelch /nav stop
		

		| If the str_followTarget declares that you need to use MQ2Nav...
		|-- We do not need to use LOS for MQ2Nav nor do we need to use target
		/if (${_type.Equal[NAV]}) {
			/nav spawn ${Spawn[pc =${_leader}]}
		} else {
			/if (${Spawn[pc ${_leader}].LineOfSight}) {
				/call TrueTarget ${Spawn[pc ${_leader}].ID}
				/if (${Me.Underwater}) {
					/squelch /stick hold behind 0 uw
				} else {
					/squelch /stick hold behind 10 uw
				}
			}
		}
	}
	
	/declare distCheck int local 20
	:movingLoop
	|-- Check all Events
	/call RunAllEvents
	/if (${bln_following}) {
		/if (${_type.Equal[NAV]} && !${Navigation.Active} && (${Spawn[pc =${_leader}].Distance3D} > ${distCheck} )) {
			/nav spawn ${Spawn[pc =${_leader}]}
			/delay 10
		}
		/goto :movingLoop
	}
/return


Sub MoveToPosition(int _y, int _x, int _z, int _adjustment, bool _warp)
	/echo \ayMoveToPosition: \ag${_y},${_x},${_z} \aw@\ag${_adjustment}
	/declare timer_movement timer local 30s
	/declare dist int local ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${_y},${_x},${_z}]}
	:movement 
		/delay 10
		/varcalc dist ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${_y},${_x},${_z}]}
		/echo ${timer_movement} ${dist}
		/if (${dist} < ${_adjustment}) /goto :final
		/if (!${timer_movement}) /goto :final
		/if (${Navigation.Active})  /goto :movement
	:final
	/nav stop
	/if (${_warp} && ${gbl_bln_useWarp}) /warp loc ${_y} ${_x} ${_z}
	/deletevar timer_movement
/return 

|#TODO #5

SUB verifyEvent(sring _sender, string _eventParams)
	/declare userValidated 	bool local FALSE
	/declare includeBot 	bool local TRUE
	/declare inZone 		bool local FALSE
	/declare inRange 		bool local FALSE
	/declare inAssistRange 	bool local FALSE
	/declare sender         string local ${_sender}

	| Set sender by parsing the _sender
	/if (${_sender.Left[1].Compare[ ]}<0) /varset sender ${_sender.Right[-2].Left[-1]}
	/if (${_sender.Equal[You]}) /varset sender ${Me.CleanName}
	/if (${_sender.Equal[MQ2]}) /varset sender ${Me.CleanName}

	| Note if this is sent by dannet as a /dgt (instead of box chat /bc)
	| Then the sender will be like xiris (all)
 	/if (${_sender.Find[(]})  {
		/varset sender ${_sender.Arg[1,(]}
	}

	| -Validate _sender.
	/varset userValidated TRUE
	
	| -Check inZone
	/if (${SpawnCount[pc =${sender}]}) /varset inZone TRUE
	
	| -Check inRange
	/if (${SpawnCount[pc =${sender} radius ${int_maxFollowDistance}]}) /varset inRange TRUE
	|/echo \awVerifyEvent Sender:\ag${sender} \awParams: \ag${_eventParams}

	| -Check for /only|
	/if (${_eventParams.Find[/only|]}) {
		/varset includeBot FALSE
		/call argueString only| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot TRUE
	}
		
	| -Check for /exclude|
	/if (${_eventParams.Find[/exclude|]}) {
		/call argueString exclude| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot FALSE
	}
	
	| -Check for /not|
	/if (${_eventParams.Find[/not|]}) {
		/call argueString not| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot FALSE
	}
	
	| -Check for /include|
	/if (${_eventParams.Find[/include|]}) {
		/call argueString include| "${_eventParams}"
		/call check_selectedBots "${Macro.Return}" "${sender}"
		/if (${Macro.Return}) /varset includeBot TRUE
	}
	/echo \ayReturning Sender:\ag${sender} \awValid?\ag${userValidated} \awIncluded?\ag${includeBot} \awInZone?\ag${inZone} \awInRange?\ag${inRange}
/return ${sender}_${userValidated}_${includeBot}_${inZone}_${inRange}

| See if the bot is in the return from the arguestring parser
SUB check_selectedBots(_selectedBots, _sender)
	/declare botSelected bool local FALSE
	/if (${_selectedBots.Find[ ]}) /varset _selectedBots ${_selectedBots.Replace[ ,,]}
	/if (${Select[priests,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${PriestClasses}]} || ${Select[healers,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${PriestClasses}]} || ${Select[ranged,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${RangedClasses}]} || ${Select[casters,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${CasterClasses}]} || ${Select[melee,${_selectedBots}]} && ${Select[${Me.Class.ShortName},${MeleeClasses}]} || ${Select[me,${_selectedBots}]} && ${_sender.Equal[${Me.CleanName}]} || ${Select[${Me.Class},${_selectedBots}]} || ${Select[${Me.Class.ShortName},${_selectedBots}]} || ${Select[${Me.CleanName},${_selectedBots}]} || ${Select[all,${_selectedBots}]} || ${Select[group,${_selectedBots}]} && ${Me.CleanName.Equal[${_sender}]} || ${Select[group,${_selectedBots}]} || ${Select[zone,${_selectedBots}]} && ${SpawnCount[pc ${_sender}]}) /varset botSelected TRUE
	
/return ${botSelected}


| Evaluate and Argument string in E3 Format eg: \only|CLR,DRU,SHM
SUB argueString(WhatToFind, givenData)
	|/echo \aoEvaluating Arguments \aw==> \ag${WhatToFind}  \aw=> \ay${givenData}
	/declare i 			int 	local 1
	/declare foundData 	string 	local NULL
	
	:nextArg
	/if (${givenData.Find[/]}) {
		/if (${givenData.Arg[${i},/].Find[${WhatToFind}]}) {
			/varset foundData ${givenData.Arg[${i},/].Arg[2,|]}		
		} else {
			/varcalc i ${i} + 1
			/if (${givenData.Arg[${i},/].Length}) /goto :nextArg
		}
	} else {
		/if (${givenData.Arg[${i}, ].Find[${WhatToFind}]}) {
			/varset foundData ${givenData.Arg[${i}, ].Arg[2,|]}
		} else {
			/varcalc i ${i} + 1
			/if (${givenData.Arg[${i}, ].Length}) /goto :nextArg
		}
	}

/return ${foundData}