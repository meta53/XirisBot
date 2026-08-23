|**
----------------------------
xiris_events_raid_dodh_demiplane.inc
----------------------------
Event Handlers for the DODH raids in Demiplane of Blood
Server contract: EQXiris content/dodh/quests/dreadspire encounter Lua
______________________________________________
REQUIRED PLUGINS
	mq2nav
______________________________________________
REVISION HISTORY
	12.04.11    xiris  INITIAL REVISION
	08.23.26    xiris  Updated for current xirisbot event, movement, and state APIs
	08.23.26    xiris  Aligned handlers with current xirisbot event structure
	08.23.26    xiris  Restored all Performer medleys and Hatchet trap luring
	08.23.26    xiris  Added support for the new event structure
______________________________________________
**|
|-- DEMI events

|-- Hatchet The Torturer
#EVENT HatchetDuck			"Duck before Hatchet's axe reaches you!"
#EVENT HatchetClose			"Hide between Hatchet's legs!"
#EVENT HatchetFar			"Get as far away from Hatchet as possible!"
#EVENT HatchetRun			"Run from Hatchet's charge!"
#EVENT HatchetAttackMiss	"Hatchet's attack misses you."
#EVENT HatchetChargeFail	"Hatchet catches you and unleashes Minotaur Gore!"
#EVENT HatchetEvadeFail		"You fail to evade Hatchet's Decapitating Chop!"

|-- Tris Wallow III
#EVENT TrisStare			"Turn away from Tris before her gaze takes hold!"
#EVENT TrisStareEnd			"You avert your eyes before Tris can seize your mind."
#EVENT TrisHuman			"#*#Hair, fangs, and claws retract as Tris collapses in her human form.#*#"
#EVENT TrisWere				"#*#My beauty gives way to the beast once more!#*#"

|-- Performer
#EVENT FailStats    "[MQ2] stat#*#"

#EVENT PerformerBegin   "#*#Hold the hall for eighteen minutes, and answer my musicians when they beckon!#*#"
#EVENT PerformerWave    "#*#A bell sounds through the hall. Wave #1# approaches!#*#"
#EVENT PerformerBeckon  "#1# beckons for #2# to approach and complete the song."
#EVENT PerformerRetry   "Turn attack off and target #1# before completing the lyric."
#EVENT PerformerSuccess "#1# completes #2#'s lyric perfectly."
#EVENT PerformerFailed  "A missed lyric wounds The Performer."
#EVENT PerformerComplete "#*#The final chord rings out. The Performer and his ensemble survive the night!#*#"
#EVENT PerformerReset   "#*#The music dies. The ensemble will regroup before another attempt.#*#"

#EVENT AelfricTouched   "#*#Aelfric the Flautist sings, 'Touched tenderly.'#*#"
#EVENT AelfricWhere     "#*#Aelfric the Flautist sings, 'Where will you be?'#*#"
#EVENT AelfricDreaming  "#*#Aelfric the Flautist sings, 'Dreaming with me.'#*#"
#EVENT AelfricPlease    "#*#Aelfric the Flautist sings, 'Please,'#*#"
#EVENT AelfricEverybody "#*#Aelfric the Flautist sings, 'everybody, hear the music.'#*#"
#EVENT BrittonSplit     "#*#Britton Harmony sings, 'When she and I split ways,'#*#"
#EVENT BrittonEndDays   "#*#Britton Harmony sings, 'it felt like the end of my days.'#*#"
#EVENT BrittonSuddenly  "#*#Britton Harmony sings, 'Until I suddenly,'#*#"
#EVENT BrittonRealized  "#*#Britton Harmony sings, 'suddenly realized'#*#"
#EVENT BrittonAlone     "#*#Britton Harmony sings, 'this life was better off alone.'#*#"
#EVENT BrittonSolitude  "#*#Britton Harmony sings, 'Solitude was the best gift you ever gave me.'#*#"
#EVENT RandalNilipus    "#*#Randal Reedsea sings, 'Ol' Nilipus hailed from Misty Thicket.'#*#"
#EVENT RandalJumjum     "#*#Randal Reedsea sings, 'Where'er he smelled Jumjum he'd pick it.'#*#"
#EVENT RandalCross      "#*#Randal Reedsea sings, 'The halflings grew cross'#*#"
#EVENT RandalProfits    "#*#Randal Reedsea sings, 'when their profits were lost,'#*#"
#EVENT RandalBrownie    "#*#Randal Reedsea sings, 'screamin', 'Where is that brownie?  I'll kick it!''#*#"
#EVENT SethNight        "#*#Seth Kimble sings, 'Another night, in eternal darkness'#*#"
#EVENT SethTime         "#*#Seth Kimble sings, 'Time bleeds like a wound that's lost all meaning'#*#"
#EVENT SethWinter       "#*#Seth Kimble sings, 'It's a long winter in the swirling chaotic void'#*#"
#EVENT SethTorture      "#*#Seth Kimble sings, 'This is my torture'#*#"
#EVENT SethPain         "#*#Seth Kimble sings, 'my pain and suffering!'#*#"
#EVENT SethPinch        "#*#Seth Kimble sings, 'Pinch me'#*#"


|--Mayong
#EVENT MayongSlash	 "#*#The Master raises his sword! All but his foremost foe must break line of sight or flee the platform.#*#"
Sub InitEvents_DoDH_Demiplane
	/echo \agInitializing Demiplane of Blood raid events
	/declare bln_demiHatchetDuck		bool	outer FALSE
	/declare bln_demiHatchetClose	bool	outer FALSE
	/declare bln_demiHatchetFar		bool	outer FALSE
	/declare bln_demiHatchetRun		bool	outer FALSE
	/declare bln_demiTrisStare		bool	outer FALSE
	/declare bln_demiTrisHuman		bool	outer FALSE
	/declare bln_demiPerformerActive	bool	outer FALSE
	/declare bln_demiPerformerPending bool	outer FALSE

	/declare int_demiSavedTargetID	int		outer 0
	/declare int_demiHatchetID		int		outer 0
	/declare int_demiHatchetTrapID	int		outer 0
	/declare flt_demiReturnX		float	outer 0
	/declare flt_demiReturnY		float	outer 0
	/declare flt_demiReturnZ		float	outer 0
	/declare flt_demiHatchetX		float	outer 0
	/declare flt_demiHatchetY		float	outer 0
	/declare flt_demiHatchetZ		float	outer 0
	/declare flt_demiHatchetTrapX	float	outer 0
	/declare flt_demiHatchetTrapY	float	outer 0
	/declare flt_demiHatchetTrapZ	float	outer 0
	/declare timer_demiHatchetRun	timer	outer 0
	/declare timer_demiPerformerAnswer timer	outer 0
	/declare timer_demiPerformerRetry timer	outer 0
	/declare str_demiPerformerMusician string	outer NULL
	/declare str_demiPerformerAnswer	string	outer NULL
	/declare int_demiPerformerWave	int		outer 0
	/declare int_demiPerformerPrompts int		outer 0
	/declare int_demiPerformerRetries int		outer 0
	/declare int_demiPerformerSuccess int		outer 0
	/declare int_demiPerformerFailed	int		outer 0

	/call RegisterRaidEvents Events_DODH_Demiplane 43
/return


Sub Events_DODH_Demiplane
	|-- Hatchet The Torturer
	/doevents HatchetDuck			"Duck before Hatchet's axe reaches you!"
	/doevents HatchetClose			"Hide between Hatchet's legs!"
	/doevents HatchetFar			"Get as far away from Hatchet as possible!"
	/doevents HatchetRun			"Run from Hatchet's charge!"
	/doevents HatchetAttackMiss	"Hatchet's attack misses you."
	/doevents HatchetChargeFail	"Hatchet catches you and unleashes Minotaur Gore!"
	/doevents HatchetEvadeFail	"You fail to evade Hatchet's Decapitating Chop!"

	|-- Tris Wallow III
	/doevents TrisStare			"Turn away from Tris before her gaze takes hold!"
	/doevents TrisStareEnd		"You avert your eyes before Tris can seize your mind."
	/doevents TrisHuman			"#*#Hair, fangs, and claws retract as Tris collapses in her human form.#*#"
	/doevents TrisWere			"#*#My beauty gives way to the beast once more!#*#"

	|-- Performer
	/doevents FailStats    "[MQ2] stat#*#"
	/doevents PerformerBegin   "#*#Hold the hall for eighteen minutes, and answer my musicians when they beckon!#*#"
	/doevents PerformerWave    "#*#A bell sounds through the hall. Wave #1# approaches!#*#"
	/doevents PerformerBeckon  "#1# beckons for #2# to approach and complete the song."
	/doevents PerformerRetry   "Turn attack off and target #1# before completing the lyric."
	/doevents PerformerSuccess "#1# completes #2#'s lyric perfectly."
	/doevents PerformerFailed  "A missed lyric wounds The Performer."
	/doevents PerformerComplete "#*#The final chord rings out. The Performer and his ensemble survive the night!#*#"
	/doevents PerformerReset   "#*#The music dies. The ensemble will regroup before another attempt.#*#"
	/doevents AelfricTouched   "#*#Aelfric the Flautist sings, 'Touched tenderly.'#*#"
	/doevents AelfricWhere     "#*#Aelfric the Flautist sings, 'Where will you be?'#*#"
	/doevents AelfricDreaming  "#*#Aelfric the Flautist sings, 'Dreaming with me.'#*#"
	/doevents AelfricPlease    "#*#Aelfric the Flautist sings, 'Please,'#*#"
	/doevents AelfricEverybody "#*#Aelfric the Flautist sings, 'everybody, hear the music.'#*#"
	/doevents BrittonSplit     "#*#Britton Harmony sings, 'When she and I split ways,'#*#"
	/doevents BrittonEndDays   "#*#Britton Harmony sings, 'it felt like the end of my days.'#*#"
	/doevents BrittonSuddenly  "#*#Britton Harmony sings, 'Until I suddenly,'#*#"
	/doevents BrittonRealized  "#*#Britton Harmony sings, 'suddenly realized'#*#"
	/doevents BrittonAlone     "#*#Britton Harmony sings, 'this life was better off alone.'#*#"
	/doevents BrittonSolitude  "#*#Britton Harmony sings, 'Solitude was the best gift you ever gave me.'#*#"
	/doevents RandalNilipus    "#*#Randal Reedsea sings, 'Ol' Nilipus hailed from Misty Thicket.'#*#"
	/doevents RandalJumjum     "#*#Randal Reedsea sings, 'Where'er he smelled Jumjum he'd pick it.'#*#"
	/doevents RandalCross      "#*#Randal Reedsea sings, 'The halflings grew cross'#*#"
	/doevents RandalProfits    "#*#Randal Reedsea sings, 'when their profits were lost,'#*#"
	/doevents RandalBrownie    "#*#Randal Reedsea sings, 'screamin', 'Where is that brownie?  I'll kick it!''#*#"
	/doevents SethNight        "#*#Seth Kimble sings, 'Another night, in eternal darkness'#*#"
	/doevents SethTime         "#*#Seth Kimble sings, 'Time bleeds like a wound that's lost all meaning'#*#"
	/doevents SethWinter       "#*#Seth Kimble sings, 'It's a long winter in the swirling chaotic void'#*#"
	/doevents SethTorture      "#*#Seth Kimble sings, 'This is my torture'#*#"
	/doevents SethPain         "#*#Seth Kimble sings, 'my pain and suffering!'#*#"
	/doevents SethPinch        "#*#Seth Kimble sings, 'Pinch me'#*#"
	|--Mayong
	/doevents MayongSlash	 "#*#The Master raises his sword! All but his foremost foe must break line of sight or flee the platform.#*#"

	| Tris's human phase needs repeated casts, but event handlers should not
	| monopolize the macro thread with an unbounded /goto loop.
	/if (${bln_demiTrisHuman}) /call HandleDemiTrisHuman
	/if (${bln_demiHatchetRun}) /call HandleDemiHatchetRun
	/if (${bln_demiPerformerPending}) /call HandleDemiPerformerPending
/return
|--TRIS EVENTS
Sub Event_TrisStare
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/varset bln_demiTrisStare TRUE
	/varset int_demiSavedTargetID ${Target.ID}
	/call Disengage
	/if (${Navigation.Active}) /nav stop
	/squelch /stick off
	/target clear
	/target npc "Tris Wallow III"
	/delay 5 ${Target.ID}
	/if (!${Target.ID}) /return
	/face fast
	/delay 2
	/face away
/return

Sub Event_TrisStareEnd
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/varset bln_demiTrisStare FALSE
	/call RestoreDemiTarget
/return

Sub Event_TrisHuman
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/echo \aoTris entered her human phase
	/varset bln_demiTrisHuman TRUE
/return

Sub Event_TrisWere
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/echo \aoTris returned to werewolf form
	/varset bln_demiTrisHuman FALSE
	/if (${Target.CleanName.Equal[Tris Wallow III]} && ${Target.PctHPs} < 8) /call Disengage
	/call RestoreDemiTarget
/return

Sub HandleDemiTrisHuman
	/if (${cache_zoneShort.NotEqual[dreadspire]} || !${bln_demiTrisHuman}) /return
	/if (${Me.Class.ShortName.NotEqual[CLR]} && ${Me.Class.ShortName.NotEqual[SHM]}) /return
	/declare str_demiCastSpell string local NULL
	/declare str_demiCastGem string local gem5
	/target clear
	/target npc "Tris Wallow III"
	/delay 3 ${Target.ID}
	/if (!${Target.ID} || ${Target.Race.Equal[Werewolf]}) {
		/varset bln_demiTrisHuman FALSE
		/return
	}
	/if (${Target.PctHPs} >= 99) {
		| EQXiris advances the wounded phase through Tris's exact say-link phrase.
		/say remove greater curse
		/return
	} else /if (${Target.PctHPs} < 99 && ${str_heal_normal_efficient.NotEqual[NULL]}) {
		/varset str_demiCastSpell ${str_heal_normal_efficient}
		/if (${Me.Class.ShortName.Equal[CLR]}) /varset str_demiCastGem gem1
	}
	/if (${str_demiCastSpell.Equal[NULL]}) /return
	/if (${Me.Gem[${str_demiCastSpell}]}) /varset str_demiCastGem gem${Me.Gem[${str_demiCastSpell}]}
	/call MQ2Cast "${str_demiCastSpell}" ${str_demiCastGem} 1s
/return


|--HATCHET EVENTS
Sub Event_HatchetDuck
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/varset bln_demiHatchetDuck TRUE
	/call Disengage
	/if (${Navigation.Active}) /nav stop
	/squelch /stick off
	/if (!${Me.Ducking}) /keypress duck
	/delay 1s
	/if (!${Me.Ducking}) /keypress duck
/return
Sub Event_HatchetClose
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/dgt Hatchet needs me under his legs.
	/varset bln_demiHatchetClose TRUE
	/call SaveDemiPosition
	/call Disengage
	/target clear
	/target npc "Hatchet the Torturer"
	/delay 5 ${Target.ID}
	/if (${Target.ID}) /nav target
/return

Sub Event_HatchetFar
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/dgt Hatchet has me running away
	/varset bln_demiHatchetFar TRUE
	/call SaveDemiPosition
	/call Disengage
	/target clear
	/target npc "Hatchet the Torturer"
	/delay 5 ${Target.ID}
	/if (${Target.ID}) /face fast
	/keypress back hold
/return

Sub Event_HatchetRun
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/call SaveDemiPosition
	/call Disengage
	/if (${Navigation.Active}) /nav stop
	/target clear
	/target npc "Hatchet the Torturer"
	/delay 5 ${Target.ID}
	/if (!${Target.ID}) /return
	/varset int_demiHatchetID ${Target.ID}
	/varset flt_demiHatchetX ${Target.X}
	/varset flt_demiHatchetY ${Target.Y}
	/varset flt_demiHatchetZ ${Target.Z}
	/varset bln_demiHatchetRun TRUE
	/varset timer_demiHatchetRun 0
	/call HandleDemiHatchetRun
/return

Sub Event_HatchetAttackMiss
	/call FinishDemiHatchetResponse
/return

Sub Event_HatchetChargeFail
	/call FinishDemiHatchetResponse
/return

Sub Event_HatchetEvadeFail
	/call FinishDemiHatchetResponse
/return

Sub FinishDemiHatchetResponse
	/declare bln_demiMoved bool local ${If[${bln_demiHatchetClose} || ${bln_demiHatchetFar} || ${bln_demiHatchetRun},TRUE,FALSE]}
	/if (${Me.Ducking}) /keypress duck
	/keypress back
	/varset bln_demiHatchetDuck FALSE
	/varset bln_demiHatchetClose FALSE
	/varset bln_demiHatchetFar FALSE
	/varset bln_demiHatchetRun FALSE
	/varset timer_demiHatchetRun 0
	/varset int_demiHatchetTrapID 0
	/if (${Navigation.Active}) /nav stop
	/if (${bln_demiMoved}) {
		/call ReturnToDemiPosition
	} else {
		/call RestoreDemiTarget
	}
/return

Sub HandleDemiHatchetRun
	/if (!${bln_demiHatchetRun} || ${timer_demiHatchetRun}) /return
	/declare flt_demiHatchetDistance float local 0
	/declare flt_demiTrapDistance float local 0
	/declare flt_demiEscapeX float local 0
	/declare flt_demiEscapeY float local 0
	/if (!${int_demiHatchetID} || !${Spawn[id ${int_demiHatchetID}].ID}) {
		/varset int_demiHatchetID ${NearestSpawn[npc =Hatchet the Torturer].ID}
		/if (!${int_demiHatchetID}) /return
	}

	| Keep a current fix on Hatchet as he chases us, then extend the vector
	| from Hatchet through our position by another 125 units.
	/varset flt_demiHatchetX ${Spawn[id ${int_demiHatchetID}].X}
	/varset flt_demiHatchetY ${Spawn[id ${int_demiHatchetID}].Y}
	/varset flt_demiHatchetZ ${Spawn[id ${int_demiHatchetID}].Z}
	/varset flt_demiHatchetDistance ${Math.Distance[${Me.Y},${Me.X}:${flt_demiHatchetY},${flt_demiHatchetX}]}
	/if (${flt_demiHatchetDistance} < 5) {
		/call TrueTarget ${int_demiHatchetID}
		/face fast
		/keypress back hold
		/varset timer_demiHatchetRun 1s
		/return
	}

	/keypress back

	| EQXiris spawns NPC type 2100250 as "a_rusted_floor_trap" at lure distance
	| from Hatchet at each HP gate. Route beyond it so the chase line crosses
	| the trap, then use the trap's scripted phrase once Hatchet is over it.
	/varset int_demiHatchetTrapID ${NearestSpawn[npc =a rusted floor trap].ID}
	/if (${int_demiHatchetTrapID} && ${Spawn[id ${int_demiHatchetTrapID}].Distance3D} < 350) {
		/varset flt_demiHatchetTrapX ${Spawn[id ${int_demiHatchetTrapID}].X}
		/varset flt_demiHatchetTrapY ${Spawn[id ${int_demiHatchetTrapID}].Y}
		/varset flt_demiHatchetTrapZ ${Spawn[id ${int_demiHatchetTrapID}].Z}
		/varset flt_demiTrapDistance ${Math.Distance[${flt_demiHatchetTrapY},${flt_demiHatchetTrapX}:${flt_demiHatchetY},${flt_demiHatchetX}]}
		/if (${flt_demiTrapDistance} <= 18) {
			/if (${Navigation.Active}) /nav stop
			/target id ${int_demiHatchetTrapID}
			/delay 3 ${Target.ID} == ${int_demiHatchetTrapID}
			/if (${Target.ID} == ${int_demiHatchetTrapID}) /say spring the trap
			/varset timer_demiHatchetRun 1s
			/return
		}
		/if (${flt_demiTrapDistance} >= 5) {
			/varset flt_demiEscapeX ${Math.Calc[${flt_demiHatchetTrapX}+((${flt_demiHatchetTrapX}-${flt_demiHatchetX})/${flt_demiTrapDistance}*150)]}
			/varset flt_demiEscapeY ${Math.Calc[${flt_demiHatchetTrapY}+((${flt_demiHatchetTrapY}-${flt_demiHatchetY})/${flt_demiTrapDistance}*150)]}
			/if (${Navigation.Active}) /nav stop
			/nav loc ${flt_demiEscapeY} ${flt_demiEscapeX} ${flt_demiHatchetTrapZ}
			/varset timer_demiHatchetRun 1s
			/return
		}
	}

	| No usable trap is up, so continue extending the line directly away from
	| Hatchet until a trap appears or the charge ends.
	/varset flt_demiEscapeX ${Math.Calc[${Me.X}+((${Me.X}-${flt_demiHatchetX})/${flt_demiHatchetDistance}*125)]}
	/varset flt_demiEscapeY ${Math.Calc[${Me.Y}+((${Me.Y}-${flt_demiHatchetY})/${flt_demiHatchetDistance}*125)]}
	/if (${Navigation.Active}) /nav stop
	/nav loc ${flt_demiEscapeY} ${flt_demiEscapeX} ${Me.Z}
	/varset timer_demiHatchetRun 1s
/return

Sub SaveDemiPosition
	/varset int_demiSavedTargetID ${Target.ID}
	/varset flt_demiReturnX ${Me.X}
	/varset flt_demiReturnY ${Me.Y}
	/varset flt_demiReturnZ ${Me.Z}
/return

Sub ReturnToDemiPosition
	/if (${Navigation.Active}) /nav stop
	/nav loc ${flt_demiReturnY} ${flt_demiReturnX} ${flt_demiReturnZ}
	/call RestoreDemiTarget
/return

Sub RestoreDemiTarget
	/if (${int_demiSavedTargetID} && ${Spawn[id ${int_demiSavedTargetID}].ID}) {
		/call TrueTarget ${int_demiSavedTargetID}
	} else /if (${int_targetID} && ${Spawn[id ${int_targetID}].ID}) {
		/call TrueTarget ${int_targetID}
	}
/return
|--PERFORMER EVENTS
Sub Event_PerformerBegin
	/varset bln_demiPerformerActive TRUE
	/varset int_demiPerformerWave 1
	/varset int_demiPerformerPrompts 0
	/varset int_demiPerformerRetries 0
	/varset int_demiPerformerSuccess 0
	/varset int_demiPerformerFailed 0
	/varset bln_demiPerformerPending FALSE
	/varset timer_demiPerformerAnswer 0
/return

Sub Event_PerformerWave(string _line, int _wave)
	/varset bln_demiPerformerActive TRUE
	/varset int_demiPerformerWave ${_wave}
/return

Sub Event_PerformerBeckon(string _line, string _musician, string _client)
	/if (${_client.NotEqual[${Me.CleanName}]}) /return
	/popup ${_musician} selected me for the next lyric.
	/varset str_demiPerformerMusician ${_musician}
/return

Sub Event_PerformerRetry(string _line, string _musician)
	/if (!${bln_demiPerformerPending}) /return
	/if (${_musician.NotEqual[${str_demiPerformerMusician}]}) /return
	/varcalc int_demiPerformerRetries ${int_demiPerformerRetries}+1
	/varset timer_demiPerformerRetry 0
	/call SubmitDemiPerformerAnswer
/return

Sub Event_AelfricTouched
	/call BeginDemiPerformerResponse "Aelfric the Flautist" "Where will you be?"
/return

Sub Event_AelfricWhere
	/call BeginDemiPerformerResponse "Aelfric the Flautist" "Dreaming with me"
/return

Sub Event_AelfricDreaming
	/call BeginDemiPerformerResponse "Aelfric the Flautist" "Please,"
/return

Sub Event_AelfricPlease
	/call BeginDemiPerformerResponse "Aelfric the Flautist" "everybody, hear the music."
/return

Sub Event_AelfricEverybody
	/call BeginDemiPerformerResponse "Aelfric the Flautist" "Touched tenderly"
/return

Sub Event_BrittonSplit
	/call BeginDemiPerformerResponse "Britton Harmony" "it felt like the end of my days."
/return

Sub Event_BrittonEndDays
	/call BeginDemiPerformerResponse "Britton Harmony" "Until I suddenly,"
/return

Sub Event_BrittonSuddenly
	/call BeginDemiPerformerResponse "Britton Harmony" "suddenly realized"
/return

Sub Event_BrittonRealized
	/call BeginDemiPerformerResponse "Britton Harmony" "this life was better off alone."
/return

Sub Event_BrittonAlone
	/call BeginDemiPerformerResponse "Britton Harmony" "Solitude was the best gift you ever gave me."
/return

Sub Event_BrittonSolitude
	/call BeginDemiPerformerResponse "Britton Harmony" "When she and I split ways,"
/return

Sub Event_RandalNilipus
	/call BeginDemiPerformerResponse "Randal Reedsea" "Where'er he smelled Jumjum he'd pick it."
/return

Sub Event_RandalJumjum
	/call BeginDemiPerformerResponse "Randal Reedsea" "The halflings grew cross."
/return

Sub Event_RandalCross
	/call BeginDemiPerformerResponse "Randal Reedsea" "when their profits were lost,"
/return

Sub Event_RandalProfits
	/call BeginDemiPerformerResponse "Randal Reedsea" "screamin', where is that brownie? I'll kick it!"
/return

Sub Event_RandalBrownie
	/call BeginDemiPerformerResponse "Randal Reedsea" "Ol' Nilipus hailed from Misty Thicket."
/return

Sub Event_SethNight
	/call BeginDemiPerformerResponse "Seth Kimble" "Time bleeds like a wound that's lost all meaning."
/return

Sub Event_SethTime
	/call BeginDemiPerformerResponse "Seth Kimble" "It's a long winter in the swirling chaotic void"
/return

Sub Event_SethWinter
	/call BeginDemiPerformerResponse "Seth Kimble" "This is my torture,"
/return

Sub Event_SethTorture
	/call BeginDemiPerformerResponse "Seth Kimble" "my pain and suffering!"
/return

Sub Event_SethPain
	/call BeginDemiPerformerResponse "Seth Kimble" "Pinch me, O' Death. . ."
/return

Sub Event_SethPinch
	/call BeginDemiPerformerResponse "Seth Kimble" "Another night, in eternal darkness."
/return

Sub BeginDemiPerformerResponse(string _musician, string _answer)
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	/if (${bln_demiPerformerPending} && ${str_demiPerformerMusician.Equal[${_musician}]}) /return
	/varset int_demiSavedTargetID ${Target.ID}
	/varset str_demiPerformerMusician ${_musician}
	/varset str_demiPerformerAnswer ${_answer}
	/varset bln_demiPerformerPending TRUE
	/varset timer_demiPerformerAnswer 16s
	/varset timer_demiPerformerRetry 0
	/varcalc int_demiPerformerPrompts ${int_demiPerformerPrompts}+1
	/call SubmitDemiPerformerAnswer
/return

Sub SubmitDemiPerformerAnswer
	/if (!${bln_demiPerformerPending} || ${timer_demiPerformerRetry}) /return
	/call Disengage
	/autofire off
	/if (${Navigation.Active}) /nav stop
	/squelch /stick off
	/target clear
	/target npc "${str_demiPerformerMusician}"
	/delay 10 ${Target.ID}
	/if (!${Target.ID}) {
		/echo \arCould not target ${str_demiPerformerMusician}; retrying before the lyric deadline.
		/varset timer_demiPerformerRetry 1s
		/return
	}
	/say ${str_demiPerformerAnswer}
	/varset timer_demiPerformerRetry 2s
/return

Sub HandleDemiPerformerPending
	/if (!${bln_demiPerformerPending}) /return
	/if (!${timer_demiPerformerAnswer}) {
		/varcalc int_demiPerformerFailed ${int_demiPerformerFailed}+1
		/call ClearDemiPerformerPending TRUE
		/return
	}
	/if (!${timer_demiPerformerRetry}) /call SubmitDemiPerformerAnswer
/return

Sub Event_PerformerSuccess(string _line, string _player, string _musician)
	/if (${_player.NotEqual[${Me.CleanName}]}) /return
	/varcalc int_demiPerformerSuccess ${int_demiPerformerSuccess}+1
	/popup Correct lyric for ${_musician}! (${int_demiPerformerSuccess}/${int_demiPerformerPrompts})
	/call ClearDemiPerformerPending TRUE
/return

Sub Event_PerformerFailed
	/if (!${bln_demiPerformerPending}) /return
	/varcalc int_demiPerformerFailed ${int_demiPerformerFailed}+1
	/popup MISSED lyric! (${int_demiPerformerFailed})
	/call ClearDemiPerformerPending TRUE
/return

Sub Event_PerformerComplete
	/varset bln_demiPerformerActive FALSE
	/call ClearDemiPerformerPending TRUE
/return

Sub Event_PerformerReset
	/varset bln_demiPerformerActive FALSE
	/varset int_demiPerformerWave 0
	/call ClearDemiPerformerPending TRUE
/return

Sub ClearDemiPerformerPending(bool _restoreTarget)
	/varset bln_demiPerformerPending FALSE
	/varset timer_demiPerformerAnswer 0
	/varset timer_demiPerformerRetry 0
	/varset str_demiPerformerMusician NULL
	/varset str_demiPerformerAnswer NULL
	/if (${_restoreTarget}) /call RestoreDemiTarget
/return

Sub Event_FailStats
	/echo Performer active: ${bln_demiPerformerActive}; wave: ${int_demiPerformerWave}/16
	/echo Personal prompts: ${int_demiPerformerPrompts}; correct: ${int_demiPerformerSuccess}; missed: ${int_demiPerformerFailed}; validation retries: ${int_demiPerformerRetries}
	/echo Pending lyric: ${bln_demiPerformerPending}; musician: ${str_demiPerformerMusician}; answer deadline: ${timer_demiPerformerAnswer}
/return

|--MAYONG EVENTS
Sub Event_MayongSlash
	/if (${cache_zoneShort.NotEqual[dreadspire]}) /return
	| EQXiris fires the sword strike after five seconds against every client
	| except Mayong's foremost foe. The configured MT holds position.
	/if (${Me.ID} == ${int_MTID}) /return
	/call SaveDemiPosition
	/call Disengage
	/if (${Navigation.Active}) /nav stop
	/if (${Target.CleanName.NotEqual[Mayong Mistmoore]}) {
		/target clear
		/target npc "Mayong Mistmoore"
		/delay 5 ${Target.ID}
	}
	/if (${Target.ID}) /face fast
	/keypress back hold
	/delay 6s
	/keypress back
	/call ReturnToDemiPosition
/return
