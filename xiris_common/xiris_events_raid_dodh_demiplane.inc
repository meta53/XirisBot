|**
----------------------------
xiris_events_raid_dodh_demiplane.inc
----------------------------
Event Handlers for the DODH raids in Demiplane of Blood
______________________________________________
REQUIRED PLUGINS
    mq2mmoreachit
    mq2mmowarp
______________________________________________
REVISION HISTORY
	12.04.11    xiris  INITIAL REVISION
______________________________________________
**|	
|-- DEMI events

|-- Hatchet The Torturer
#EVENT HatchetDuck			"#*#levels his axe at your upper#*#"
#EVENT HatchetStand			"#*#passes harmlessly overhead#*#"
#EVENT HatchetClose			"#*#lifts his axe high#*#"
#EVENT HatchetCloseEnd		"#*#darting between#*#"
#EVENT HatchetFar			"#*#hefts a weighted throwing#*#"
#EVENT HatchetFarEnd		"#*#outdistance the reach of the#*#"
#EVENT HatchetRun			"#*#about to charge directly at you#*#"
#EVENT HatchetRunEnd		"#*#stops to catch his breath#*#"
#EVENT HatchetBugged		"You are too distracted to use a skill"

|-- Tris Wallow III
#EVENT TrisStare			"#*#stares deeply into your eyes#*#"
#EVENT TrisStareEnd			"#*#averted your eyes from#*#"
#EVENT TrisHuman			"#*#claws retract as Tris reverts#*#"
#EVENT TrisWere				"#*#screams as her flesh peels#*#"

|-- Performer
#EVENT FailStats    "[MQ2] stat#*#"

#EVENT Flawless      "#*#Your performance is flawless.  The skeletal musician nods their head approvingly.#*#"
#EVENT Failed        "The Performer says 'Come on, #1#, stay with us.  I'm counting on you to help perform this medley.  No more mistakes!'"


#EVENT Aelfric_0    "#*#Aelfric recites a line of his song and beckons for you to approach him and sing the next line.#*#"
#EVENT Aelfric_1    "#*#Touched tenderly.#*#"
#EVENT Aelfric_2    "#*#Where will you be?#*#"
#EVENT Aelfric_3    "#*#Dreaming with me.#*#"
#EVENT Aelfric_4    "#*#Please,#*#"
#EVENT Aelfric_5    "#*#everybody, hear the music.#*#"


#EVENT Britton_0    "#*#Britton recites a line of her song and beckons for you to approach her and sing the next line.#*#"
#EVENT Britton_1    "#*#When she and I split ways,#*#"
#EVENT Britton_2    "#*#it felt like the end of my days.#*#"
#EVENT Britton_3    "#*#Until I suddenly,#*#"
#EVENT Britton_4    "#*#suddenly realized#*#"
#EVENT Britton_5    "#*#this life was better off alone.#*#"
#EVENT Britton_6    "#*#Solitude was the best gift you ever gave me.#*#"


#EVENT Randal_0     "#*#Randal recites a line of his song and beckons for you to approach him and sing the next line.#*#"
#EVENT Randal_1     "#*#Ol' Nilipus hailed from Misty Thicket.#*#"
#EVENT Randal_2     "#*#Where'er he smelled Jumjum he'd pick it.#*#"
#EVENT Randal_3     "#*#The halflings grew cross#*#"
#EVENT Randal_4     "#*#when their profits were lost,#*#"
#EVENT Randal_5     "#*#screamin', 'Where is that brownie?  I'll kick it!'#*#"


#EVENT Seth_0        "#*#Seth recites a line of his song and beckons for you to approach him and sing the next line#*#"
#EVENT Seth_1        "#*#Another night, in eternal darkness#*#"
#EVENT Seth_2        "#*#Time bleeds like a wound that's lost all meaning#*#"
#EVENT Seth_3        "#*#It's a long winter in the swirling chaotic void#*#"
#EVENT Seth_4        "#*#This is my torture#*#"
#EVENT Seth_5        "#*#my pain and suffering!#*#"
#EVENT Seth_6        "#*#Pinch me#*#"


|--Mayong
#EVENT MayongSlash	 "#*#The Master raises his sword#*#"





Sub InitEvents_DoDH_Demiplane
	/declare sEventWait	bool	outer	FALSE
	/declare hDuck		bool	outer 	FALSE
	/declare hClose		bool	outer	FALSE
	/declare hFar		bool	outer	FALSE
	/declare hRun		bool	outer	FALSE
	
	/declare tStare		bool	outer	FALSE
	/declare tHuman		bool	outer	FALSE

    /declare pTrigger	bool	outer	FALSE

    /declare TriggerNPC			string		outer
    /declare TriggerRespond		string		outer
    /declare Flawless			int			outer    0
    /declare Failed[54,2]		string		outer    NA	
    
    /declare mSlash		bool	outer	FALSE

	/call RegisterRaidEvents Events_DODH_Demiplane 42
/return


Sub Events_DODH_Demiplane 
	|-- Hatchet The Torturer
	/doevents HatchetDuck		"#*#levels his axe at your upper#*#"
	/doevents HatchetStand		"#*#passes harmlessly overhead#*#"
	/doevents HatchetClose		"#*#lifts his axe high#*#"
	/doevents HatchetCloseEnd	"#*#darting between#*#"
	/doevents HatchetFar		"#*#hefts a weighted throwing#*#"
	/doevents HatchetFarEnd		"#*#outdistance the reach of the#*#"
	/doevents HatchetRun		"#*#about to charge directly at you#*#"
	/doevents HatchetRunEnd		"#*#stops to catch his breath#*#"
	/doevents HatchetBugged		"You are too distracted to use a skill"

	|-- Tris Wallow III
	/doevents TrisStare			"#*#stares deeply into your eyes#*#"
	/doevents TrisStareEnd		"#*#averted your eyes from#*#"
	/doevents TrisHuman			"#*#claws retract as Tris reverts#*#"
	/doevents TrisWere			"#*#screams as her flesh peels#*#"

	|-- Performer
	/doevents FailStats    "[MQ2] stat#*#"
	/doevents Flawless     "#*#Your performance is flawless.  The skeletal musician nods their head approvingly.#*#"
	/doevents Failed       "The Performer says 'Come on, #1#, stay with us.  I'm counting on you to help perform this medley.  No more mistakes!'"

	/doevents Aelfric_0    "#*#Aelfric recites a line of his song and beckons for you to approach him and sing the next line.#*#"
	/doevents Aelfric_1    "#*#Touched tenderly.#*#"
	/doevents Aelfric_2    "#*#Where will you be?#*#"
	/doevents Aelfric_3    "#*#Dreaming with me.#*#"
	/doevents Aelfric_4    "#*#Please,#*#"
	/doevents Aelfric_5    "#*#everybody, hear the music.#*#"

	/doevents Britton_0    "#*#Britton recites a line of her song and beckons for you to approach her and sing the next line.#*#"
	/doevents Britton_1    "#*#When she and I split ways,#*#"
	/doevents Britton_2    "#*#it felt like the end of my days.#*#"
	/doevents Britton_3    "#*#Until I suddenly,#*#"
	/doevents Britton_4    "#*#suddenly realized#*#"
	/doevents Britton_5    "#*#this life was better off alone.#*#"
	/doevents Britton_6    "#*#Solitude was the best gift you ever gave me.#*#"

	/doevents Randal_0     "#*#Randal recites a line of his song and beckons for you to approach him and sing the next line.#*#"
	/doevents Randal_1     "#*#Where'er he smelled Jumjum he'd pick it.#*#"
	/doevents Randal_2     "#*#The halflings grew cross#*#"
	/doevents Randal_3     "#*#when their profits were lost,#*#"
	/doevents Randal_4     "#*#screamin', 'Where is that brownie?  I'll kick it!'#*#"

	/doevents Seth_0        "#*#Seth recites a line of his song and beckons for you to approach him and sing the next line#*#"
	/doevents Seth_1        "#*#Another night, in eternal darkness#*#"
	/doevents Seth_2        "#*#Time bleeds like a wound that's lost all meaning#*#"
	/doevents Seth_3        "#*#It's a long winter in the swirling chaotic void#*#"
	/doevents Seth_4        "#*#This is my torture#*#"
	/doevents Seth_5        "#*#my pain and suffering!#*#"
	/doevents Seth_6        "#*#Pinch me#*#"

	|--Mayong
	/doevents MayongSlash	 "#*#The Master raises his sword#*#"
/return

|--TRIS EVENTS
Sub Event_TrisStare
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/varset tStare	TRUE
	/stick off
	/target Tris
	/delay 1
	/face fast
	/delay 1
	/face away
	/delay 1
/return

Sub Event_TrisStareEnd
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/varset tStare FALSE
	
	/if (${strArchetype.Equal[MELEE]} || ${strArchetype.Equal[CASTER]}) {
		/assist ${Spawn[id ${int_MTID}]}
	} else /if (${strArchetype.Equal[PRIEST]}) {
		/target id ${int_MTID}
	}
/return
	
Sub Event_TrisHuman
	/echo Tris is Human!
	/declare archetype string local
	/if (${Select[${Me.Class.ShortName},MNK,ROG,BER]}) /varset archetype MELEE
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /varset archetype PRIEST
	/if (${Select[${Me.Class.ShortName},NEC]}) /varset archetype CASTER
	/varset tHuman TRUE
	/if (${archetype.Equal[MELEE]}) {
		:meleeAssist
		/if (${Select[${Target.Race},Human]}) {
			/assist ${Spawn[id ${int_MTID}]}
			/goto :meleeAssist
		} else {
			/return
		}
	} else /if (${archetype.Equal[PRIEST]}) {
		/echo I am a priest, and I am healing/curing Tris!
		:priestAssist
		/target Tris
		/if (${Select[${Target.Race},Werewolf]}) /return
		/if (${Me.Class.ShortName.Equal[SHM]}) {
			/if (${Target.PctHPs} >= 99) {
				/call MQ2Cast "Remove Greater Curse" gem10 1s
			} else {
				/call MQ2Cast "Qirik's Mending Rk. II" gem5 1s
			}
			/goto :priestAssist
		} else /if (${Me.Class.ShortName.Equal[CLR]}) {
			/varset str_currentHeal ${str_heal_normal_efficient}
			/if (${Target.PctHPs} >= 99) {
				/call MQ2Cast "Remove Greater Curse" gem10 1s
			} else {
				/call MQ2Cast "${str_currentHeal}" gem1 1s
			}
			/goto :priestAssist
		}
	}
/return


Sub Event_TrisWere
	/echo triswere
	
	/varset tHuman FALSE
	/if (${strArchetype.Equal[MELEE]} || ${strArchetype.Equal[CASTER]}) {
		:targetCheck
		/if (${Target.Name.Equal[Tris Wallow III]} && ${Target.PctHPs} < 8) {
			/attack off
			/assist ${Spawn[id ${int_MTID}]}
			/goto :targetCheck
		} else {
			/attack on
			/return
		}
	}
/return


|--HATCHET EVENTS
Sub Event_HatchetDuck
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/varset hDuck	TRUE
	/stick off
	/attack off
	/if (!${Me.Ducking}) /keypress duck
	/delay 1s
	/if (!${Me.Ducking}) /keypress duck
	/stick off

/return


Sub Event_HatchetStand
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/varset hDuck	FALSE
	/if (${Me.Ducking}) /keypress duck
/return

Sub Event_HatchetClose
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/dgt Hatchet needs me under his legs.
	/varset hClose	TRUE
	/waypoint update ${Me.Name}_Hatchet
	/tar Hatchet
	/delay 1s
	/warp t
/return

Sub Event_HatchetCloseEnd
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/dgt Hatchet no longer needs me under his legs.
	/varset hClose	FALSE
	/warp wp ${Me.Name}_Hatchet
	/delay 1
/return

Sub Event_HatchetFar
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/dgt Hatchet has me running away
	/varset hFar	TRUE
	/waypoint update ${Me.Name}_Hatchet
	/warp s
/return

Sub Event_HatchetFarEnd
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/dgt Hatchet no longer needs me a zone in
	/varset hFar	FALSE
	/warp wp ${Me.Name}_Hatchet
/return

Sub Event_HatchetBugged
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/fade
	/delay 5s
/return

Sub Event_HatchetRun
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/varset hRun	TRUE
	/waypoint update ${Me.Name}_Hatchet
	/setgrav 0
	/zwarp 1000
	/keypress forward hold
	/delay 15s
	/keypress forward
	/warp wp ${Me.Name}_Hatchet
	/setgrav .4
	/varset hRun	FALSE
/return

Sub Event_HatchetRunEnd
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	/varset hRun	FALSE
	/warp wp ${Me.Name}_Hatchet
	/setgrav .4
/return

Sub EventWait
	/if (${Zone.ShortName.NotEqual[dreadspire_raid]}) /return
	| check for the wait conditions
	
	:event_waitLoop
	|Hatchet
	/if (${hDuck} || ${hClose} || ${hFar} || ${hRun}) {
		/varset sEventWait TRUE
	|Tris
	} else /if (${tStare}) {
		/varset sEventWait TRUE
	} else /if (${pTrigger}) {
		/varset sEventWait TRUE
	} else {
		/varset sEventWait FALSE
	}
	
	| create the waiting loop
	/call RunRaidEvents	
	/if (${sEventWait}) {
		/delay 10
		/goto :event_waitLoop
	} else {
		/return
	}
/return

|--PERFORMER EVENTS
Sub PerformVerse
	/if (!${pTrigger}) /return
	/echo PerformVerse
	/varset pTrigger FALSE
	/declare archetype string local
	/if (${Select[${Me.Class.ShortName},MNK,ROG,BER]})	/varset archetype MELEE
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]})	/varset archetype PRIEST
	/if (${Select[${Me.Class.ShortName},NEC]}) 			/varset archetype CASTER
	/declare	oldAssistID int local
	/varset		oldAssistID ${Target.ID}
	/stick off
	/attack off
	/delay 10
	/target npc "${TriggerNPC}"
	/delay 10
	/saytarget ${TriggerRespond}
	/delay 10
	/target ID ${oldAssistID}
	/if (${Target.ID} == ${oldAssistID}) {
		/if (${archetype.Equal[MELEE]}) {
			/if (${Select[${Target.Race},Skeleton]}) {
				/keypress escape
			} else {
				/attack on
			}
		}
	} 
	/varset pTrigger FALSE
	/varset TriggerNPC
	/varset TriggerRespond
/return
		
Sub Event_Flawless
    /varcalc Flawless ${Flawless}+1
    /popup ${If[${Flawless}>1,Another ,]}FLAWLESS performance! (${Flawless})
    /varset pTrigger FALSE
/return

Sub Event_Failed(string Line, string Actor)
    /declare i                int        local    0
    /for i 1 to ${Failed.Size}
        /if (${Failed[${i},1].Equal[NA]}) {
            /varset Failed[${i},1] ${Actor}
            /varset Failed[${i},2] 1
            /goto :break
        }
        /if (${Failed[${i},1].Equal[${Actor}]}) {
            /varcalc Failed[${i},2] ${Failed[${i},2]}+1
            /goto :break
        }
    /next i
    :break
    /popup ${Failed[${i},1]} made a MISTAKE!${If[${Failed[${i},2]} > 1, (${Int[${Failed[${i},2]}]} TIMES NOW!),]}
    /varset pTrigger FALSE
/return

Sub Event_FailStats
    /declare i                int        local    0
    /declare Fails            int        local    0
    /for i 1 to ${Failed.Size}
        /if (${Failed[${i},1].Equal[NA]}) /goto :break
        /if (${Failed[${i},2]}) {
            /echo ${Failed[${i},1]} have failed ${Int[${Failed[${i},2]}]} times.
            /varcalc Fails ${Fails}+${Failed[${i},2]}
        }
    /next i
    :break
    /echo Total (personal) flawless: ${Flawless}
    /echo Total fails: ${Fails}
/return		
		
Sub Event_Aelfric_0
	/varset pTrigger TRUE
/return

Sub Event_Randal_0
	/varset pTrigger TRUE
/return

Sub Event_Britton_0
	/varset pTrigger TRUE
/return

Sub Event_Seth_0
	/varset pTrigger TRUE
/return
		
Sub Event_Aelfric_1
	/echo aelfric_1
    /varset TriggerNPC Aelfric the Flautist
    /varset TriggerRespond Where will you be?
    /call PerformVerse
/return
Sub Event_Aelfric_2
    /varset TriggerNPC Aelfric the Flautist
    /varset TriggerRespond Dreaming with me
    /call PerformVerse
/return
Sub Event_Aelfric_3
    /varset TriggerNPC Aelfric the Flautist
    /varset TriggerRespond Please,
    /call PerformVerse
/return
Sub Event_Aelfric_4
    /varset TriggerNPC Aelfric the Flautist
    /varset TriggerRespond everybody, hear the music.
    /call PerformVerse
/return
Sub Event_Aelfric_5
    /varset TriggerNPC Aelfric the Flautist
    /varset TriggerRespond Touched tenderly
    /call PerformVerse
/return


Sub Event_Britton_1
    /varset TriggerNPC Britton Harmony
    /varset TriggerRespond it felt like the end of my days.
    /call PerformVerse
/return
Sub Event_Britton_2
    /varset TriggerNPC Britton Harmony
    /varset TriggerRespond Until I suddenly,
    /call PerformVerse
/return
Sub Event_Britton_3
    /varset TriggerNPC Britton Harmony
    /varset TriggerRespond suddenly realized
    /call PerformVerse
/return
Sub Event_Britton_4
    /varset TriggerNPC Britton Harmony
    /varset TriggerRespond this life was better off alone.
    /call PerformVerse
/return
Sub Event_Britton_5
    /varset TriggerNPC Britton Harmony
    /varset TriggerRespond Solitude was the best gift you ever gave me.
    /call PerformVerse
/return
Sub Event_Britton_6
    /varset TriggerNPC Britton Harmony
    /varset TriggerRespond When she and I split ways,
    /call PerformVerse
/return


Sub Event_Randal_1
    /varset TriggerNPC Randal Reedsea
    /varset TriggerRespond Where'er he smelled Jumjum he'd pick it.
    /call PerformVerse
/return
Sub Event_Randal_2
    /varset TriggerNPC Randal Reedsea
    /varset TriggerRespond The halflings grew cross.
    /call PerformVerse
/return
Sub Event_Randal_3
    /varset TriggerNPC Randal Reedsea
    /varset TriggerRespond when their profits were lost,
    /call PerformVerse
/return
Sub Event_Randal_4
    /varset TriggerNPC Randal Reedsea
    /varset TriggerRespond screamin', where is that brownie? I'll kick it!
    /call PerformVerse
/return
Sub Event_Randal_5
    /varset TriggerNPC Randal Reedsea
    /varset TriggerRespond Ol' Nilipus hailed from Misty Thicket.
    /call PerformVerse
/return


Sub Event_Seth_1
    /varset TriggerNPC Seth Kimble
    /varset TriggerRespond Time bleeds like a wound that's lost all meaning.
    /call PerformVerse
/return
Sub Event_Seth_2
    /varset TriggerNPC Seth Kimble
    /varset TriggerRespond It's a long winter in the swirling chaotic void
    /call PerformVerse
/return
Sub Event_Seth_3
    /varset TriggerNPC Seth Kimble
    /varset TriggerRespond This is my torture,
    /call PerformVerse
/return
Sub Event_Seth_4
    /varset TriggerNPC Seth Kimble
    /varset TriggerRespond my pain and suffering!
    /call PerformVerse
/return
Sub Event_Seth_5
    /varset TriggerNPC Seth Kimble
    /varset TriggerRespond Pinch me, O' Death. . .
    /call PerformVerse
/return
Sub Event_Seth_6
    /varset TriggerNPC Seth Kimble
    /varset TriggerRespond Another night, in eternal darkness.
    /call PerformVerse
/return		



|--MAYONG EVENTS
Sub Event_MayongSlash
	/declare archetype string local
	/if (${Select[${Me.Class.ShortName},MNK,ROG,BER]})	/varset archetype MELEE
	/if (${archetype.Equal[MELEE]}) {
		/warp s
		/delay 12s
		/warp b 10
	}	
/return
		