|**
----------------------------
XIRIS_FOLLOW.INC
----------------------------
This was written ground up for usage with xiris's bots
Allows to bln_following via stick or nav
______________________________________________
REQUIRED PLUGINS
    
______________________________________________
REVISION HISTORY
	04.05.22    xiris   INITIAL REVISION
	06.13.22	xiris   Updated events (minor)
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only	
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

#EVENT MoveHere		"#1# tells you#*#'MoveTo #2#'"
#EVENT MoveHere		"#1# tells the group#*#'MoveTo #2#'"
#EVENT MoveHere		"#1# tells the raid#*#'MoveTo #2#'"
#EVENT MoveHere 	"[ #1# #*#]#*#MoveTo #2#"

#EVENT NavigateWP	"#1# tells you#*#'NavToWP #2#'"
#EVENT NavigateWP	"#1# tells the group#*#'NavToWP #2#'"
#EVENT NavigateWP	"#1# tells the raid#*#'NavToWP #2#'"
#EVENT NavigateWP 	"[ #1# #*#]#*#NavToWP #2#"

#EVENT CreateCamp	"#1# tells you#*#'CreateCamp #2#'"
#EVENT CreateCamp	"#1# tells the group#*#'CreateCamp #2#'"
#EVENT CreateCamp	"#1# tells the raid#*#'CreateCamp #2#'"
#EVENT CreateCamp 	"[ #1# #*#]#*#CreateCamp #2#"


Sub InitEvents_Movement
	/call RegisterBackgroundEvents Events_Movement 5
/return

Sub Events_Movement
	/doevents NavigateWP
	/doevents Follow
	/doevents Stop
	/doevents MoveHere
	/doevents CreateCamp
/return


Sub xbot_initialize_movement
	/cecho \aoxbot_initialize_movement
	/declare bln_following 			bool 	outer FALSE
	/declare int_maxFollowDistance	int 	outer 1000
	/declare str_followTarget		string  outer 
	/call InitEvents_Movement
/return



SUB EVENT_Follow(_line, _chatSender, _eventParams)
	/if (${_chatSender.Equal[${Me.CleanName}]}) /return
	/declare followType 	string local ${If[${_eventParams.Find[/type|NAV]},NAV,STICK]}
	/call verifyEvent "${_chatSender}" "${_eventParams}"
	/if (${Bool[${Macro.Return.Arg[2,_]}]} && ${Bool[${Macro.Return.Arg[3,_]}]} && ${Bool[${Macro.Return.Arg[4,_]}]}) {
		/call StartFollowing ${_chatSender} ${followType}
	}
	/doevents flush Follow
/return


SUB EVENT_Stop(_line, _chatSender, _eventParams)
	| -Check if I'm bln_following.
	/if (${bln_following}) /call StopFollowing
	/doevents flush Stop
/return


SUB EVENT_NavigateWP(_line, _chatSender, _eventParams)
	/cecho \ayNavigate to waypoint: \ag${_eventParams}
	/declare wp string local ${_eventParams.Arg[1]}
	/nav wp ${wp}
	/doevents flush NavigateWP
/return


SUB EVENT_CreateCamp(_line, _chatSender, _eventParams)
	/cecho \ayCreating Camp: \ag${_eventParams}
	/declare cmp string local ${_eventParams.Arg[1]}
	/nav recordwaypoint ${cmp}
	/varset int_campY ${Me.Y}
	/varset int_campX ${Me.X}
	/varset int_campZ ${Me.Z}
	/doevents flush CreateCamp
/return


Sub StartFollowing(string _leader, string _followType)
	/varset bln_following TRUE
	/cecho \ao Following \awLeader: \ag${_leader} \awFollowType: \ag${_followType}
	
	| -Check Range
	/if (${Spawn[=${_leader}].Distance} > ${int_maxFollowDistance}) {
		/docommand /${cmdChannel} [${str_followTarget}] is too far away.
	} else {
		/call Disengage
		| -Check LoS.
		/if (${_followType.Equal[NAV]} || (${_followType.NotEqual[NAV]} && ${Spawn[=${_leader}].LineOfSight})) {
			/call AquireFollow ${_followType} ${_leader}
		} else {
			/docommand /${cmdChannel} _followType != NAV || STICK cannot see [${_leader}]
		}
	} 
/return

Sub StopFollowing
	/varset bln_following 	 FALSE
	/squelch /stick off
	/if (${Navigation.Active}) 		/squelch /nav stop
	/if (${Me.Class.ShortName.Equal[ROG]}) {
			/squelch /melee hide=1
			/squelch /melee sneak=1
	}
/return


SUB AquireFollow(string _type, string _leader)
	/if (${SpawnCount[=${_leader}]} && ${SpawnCount[=${_leader} radius ${int_maxFollowDistance}]} && ${_leader.NotEqual[${Me.CleanName}]}) {
		
		| store the follow target in case we need it in another sub (unlikely)
		/varset str_followTarget		${_leader}
		/if (${Stick.Active}) 			/squelch /stick off
		/if (${Navigation.Active}) 		/squelch /nav stop
		/if (${Me.Class.ShortName.Equal[ROG]}) {
			/if (${Melee.Silent}) {
				/melee sneak=0
				/doability sneak
			}
			/if (${Melee.Hidden}) {
				/melee hide=0
				/doability hide
			}
		}

		| If the str_followTarget declares that you need to use MQ2Nav...
		|-- We do not need to use LOS for MQ2Nav nor do we need to use target
		/if (${_type.Equal[NAV]}) {
			/squelch /nav spawn ${Spawn[pc =${_leader}]}
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
			/squelch /nav spawn ${Spawn[pc =${_leader}]}
			/delay 10
		}
		/goto :movingLoop
	}
/return





SUB EVENT_MoveHere(_line, _chatSender, _eventParams)

	| -Verify the event
	/if (${Defined[_chatSender]}) /call verifyEvent "${_chatSender}" "${_line}"
	|    |-----------Valid User----------|    |--------Included Bot-----------|    |-----------In Zone-------------|    |------------In Range------------|
	/if (${Bool[${Macro.Return.Arg[2,_]}]} && ${Bool[${Macro.Return.Arg[3,_]}]} && ${Bool[${Macro.Return.Arg[4,_]}]} && ${Bool[${Macro.Return.Arg[5,_]}]}) {
	
		| -Check if the _chatSender is me.
		/if (${_chatSender.NotEqual[${Me}]}) {
			| -Set move_destination.
			/declare moveTo_Name string local
			/declare move_destination string local ${If[${_eventParams.Arg[1, ].Find[/]} || ${_eventParams.Arg[1, ].Equal[me]} || ${_eventParams.Arg[1, ].Equal[here]},${_chatSender},${_eventParams.Arg[1, ]}]}
			
			| -Change move_destination to a spawnID
			/if (!${Int[${move_destination}]}) {
				/if (${SpawnCount[pc =${move_destination}]}) {
					/varset move_destination ${Spawn[pc =${move_destination}].ID}
				} else /if (${SpawnCount[pet =${move_destination}]}) {
					/varset move_destination ${Spawn[pet =${move_destination}].ID}
				} else /if (${SpawnCount[npc =${move_destination}]}) {
					/varset move_destination ${Spawn[npc =${move_destination}].ID}
				}
			}
			
			| Convert SpawnID to Loc
			/varset moveTo_Name ${Spawn[id ${move_destination}].CleanName}
			/declare y int local ${Spawn[id ${move_destination}].Y}
			/declare x int local ${Spawn[id ${move_destination}].X}
			/declare z int local ${Spawn[id ${move_destination}].Z}
			/varset move_destination ${y},${x},${z}

			| -Stop assisting so I don't try to stick to the AssistTarget.	
			/if (${Me.Combat}) /call Event_BackOff
			
			/declare retryTimer timer local 150
			
			:retry_MoveTo
			/call MoveTo -Loc|${move_destination} uw
			
			| -Check if I have moved to the Target.
			/call Triangulate_Distance ${move_destination} ${Me.Loc.Replace[ ,]}
			/if (${Macro.Return} > 10) {
				/if  (${retryTimer}) {
					/goto :retry_MoveTo
				}
			} 
			| -If I'm bln_following, reaquire follow.
			/if (${bln_following}) {
				/if (${moveTo_Name.Equal[${str_followTarget}]}) {
					/call AquireFollow
				} else {
					/call EVENT_Stop "<${Me.CleanName}> Stop" ${Me.CleanName}
				}
			}
		}
	}
	/doevents flush MoveHere
/return

Sub MoveToPosition(int _y, int _x, int _z, int _adjustment, bool _warp)
	/cecho \ayMoveToPosition: \ag${_y},${_x},${_z} \aw@\ag${_adjustment}
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
	/if (${_warp}) /warp loc ${_y} ${_x} ${_z}
	/deletevar timer_movement
/return 



|----------------------------------------------------------------------------------------------------|
|- MoveTo																							-|
|----------------------------------------------------------------------------------------------------|
|- Attempts to utilize MQ2MoveUtils /moveto function, to move to a given destination.				-|
|----------------------------------------------------------------------------------------------------|
|- @param1 destination: Destinations can be provided as an ID, or loc, using the bln_following syntax:	-|
|-																									-|
|- Move to a Spawn ID use, -SpawnID|#																-|
|-		Ex.  /call MoveTo -SpawnID|${Target.ID}														-|
|-																									-|
|- Move to a Location use, -Loc|Y,X																	-|
|-		Ex.  /call MoveTo -Loc|${Target.Y},${Target.X}												-|
|----------------------------------------------------------------------------------------------------|
SUB MoveTo(destinationInfo, int distCheck)
	/declare X_Loc  float local
	/declare Y_Loc  float local
	/declare Z_Loc  float local
	/declare dist2D	float local 0
	/declare dist3D float local 0
	/declare distZ  float local 0
	/declare zDir   string local UP

	
	/if (!${Defined[distCheck]}) /declare distCheck int local 10
	/varset distCheck 10

|---------------------------------------------------------Set MoveTo variables per destinationType.
	/if (${destinationInfo.Find[-SpawnID]}) {
		/call TrueTarget ${destinationInfo.Arg[2,|]}
		/varset Y_Loc ${Target.Y}
		/varset X_Loc ${Target.X}
		/varset Z_Loc ${Target.Z}
		
	} else /if (${destinationInfo.Find[-Loc]}) {
		/varset Y_Loc ${destinationInfo.Arg[2,|].Arg[1,,]}
		/varset X_Loc ${destinationInfo.Arg[2,|].Arg[2,,]}
		/varset Z_Loc ${destinationInfo.Arg[2,|].Arg[3,,]}

	} else {
		/echo [${destinationInfo}] does not specify a valid destinationType.  Use -SpawnID|#, or, -Loc|Y,X,Z to note your intended destinationType.
	}
	
	|-- Calculate the start distance
	/varcalc dist2D ${Math.Distance[${Me.Y},${Me.X}:${Y_Loc},${X_Loc}]}
	/varcalc dist3D ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${Y_Loc},${X_Loc},${Z_Loc}]}
	|-- Check if target is too far.
	
	/if (${Int[${dist3D}]} > ${int_maxFollowDistance}) {
		/echo [${Spawn[id ${destination}].CleanName}] is too far away.
	} else {

	|-- Check if target is already close enough
		/if (${Int[${dist3D}]} > 10) {
	
		| Declare a timer based on how far you have to move (1s for every 50 units + 3).
			/declare retryTimer timer local ${Math.Calc[${Macro.Return} / 50 + 3].Int}s
			
		| Engage MQ2MoveUtils.
			:retry_MoveToLoc
			/if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /interrupt
			/moveto loc ${Y_Loc} ${X_Loc} ${Z_Loc} uw
			/delay 10 ${Me.Moving} || ${MoveTo.Moving}
			
			:movingTo_Loop
			/call RunRaidEvents	
			|-- Calculate the current distance
			/varcalc dist2D ${Math.Distance[${Me.Y},${Me.X}:${Y_Loc},${X_Loc}]}
			/varcalc dist3D ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${Y_Loc},${X_Loc},${Z_Loc}]}
			|/echo dist now: 2D:${dist2D} 3D:${dist3D}
			/if (${MoveTo.Moving}) {
				/if (${dist3D} > ${distCheck} && ${retryTimer}) {
					/if (${dist2D} > ${distCheck} && ${retryTimer}) {
						|/echo not there yet, returning to loop
						/goto :movingTo_Loop
					} else {
						|/echo dist3D is still far, but 2d is close, I am above/below
						/squelch  /stick off
						/squelch  /moveto off
						/goto :zwarp
					}
				} else {
					/squelch /stick off
				}
			} else {
				|/echo not moving ${dist3D} > ${Math.Calc[${distCheck} + ${Math.Calc[${distCheck} / 4].Int}].Int}
				/if (${dist3D} > ${Math.Calc[${distCheck} + ${Math.Calc[${distCheck} / 4].Int}].Int}) {
					|/echo checking retry timer
					/if (${retryTimer}) {
						|/echo (${dist2D} <= ${distCheck}) && ${retryTimer})
						/if ((${dist2D} <= ${distCheck}) && ${retryTimer}) {
							|/echo dist3D is still far, but 2d is close, I am above/below
							/squelch  /stick off
							/squelch  /moveto off
							/goto :zwarp
						} else {
							|/echo going to retry_MoveToLoc
							/goto :retry_MoveToLoc
						}
					}
				} else {
					/squelch /stick off
					/delay 30 !${Me.Moving}
				}
			}
		}
	}
	:zwarp	
	/squelch  /stick off
	/squelch  /moveto off
	/declare amt float local 
	/if (${Z_Loc}> ${Me.Z}) {
		/varcalc amt ${dist3D}
	} else {
		/varcalc amt ${dist3D}*-1
	}
	/zwarp ${amt}
	/delay 30 !${Me.Moving}
	:return
/return


|----------------------------------------------------------------------------|
|- Triangulates distance between to points using the Pythagoras theorem.	-|
|----------------------------------------------------------------------------|
|- @param loc#1: Location 'A'												-|
|- @param loc#2: Location 'B'												-|
|----------------------------------------------------------------------------|
SUB Triangulate_Distance(string loc#1, string loc#2)
/return ${Math.Sqrt[${Math.Calc[${loc#2.Arg[2,,]}-${loc#1.Arg[2,,]}]}*${Math.Calc[${loc#2.Arg[2,,]}-${loc#1.Arg[2,,]}]}+${Math.Calc[${loc#2.Arg[1,,]}-${loc#1.Arg[1,,]}]}*${Math.Calc[${loc#2.Arg[1,,]}-${loc#1.Arg[1,,]}]}]}

|-- Determine the distance X,Y,Z instead of a single planar X,Y
|-- Formula is AB = SQRT((x2-x1)^2+(y2-y1)^2+(z2-z1)^2)
SUB Triangulate_Distance_3D(string loc#1, string loc#2)
/return
