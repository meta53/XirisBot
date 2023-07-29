|**
----------------------------
xiris_curing.inc
----------------------------
This include contains logic for curing self/group debuffs.
Useful for raids like Txevu

Works for all classes. Will set a group member as dedicated curer.

Will cure with
RGC, Antidote, Radiant Cure for effects that DRAIN HP or MANA. Resist/Movement debuffs are ignored.
______________________________________________
REVISION HISTORY
    03.19.16    xiris	INITIAL REVISION
	12.20.16	xiris 	On curing group, checks for own debuff before casting on self, 
						potentially saving time not curing self 
						(note, can't see other group members debuff on eqtitan)
	09.03.18	xiris   Added to common include, so this is initialized. All toons now ask for cures
						If in combat, only important debuffs, out of combat, all debuffs
	03.17.22	xiris	Updated cure request EVENT and cleaned up some code
						Adding curer interrogation on start up to bypass un-equiped (no spell) potential group curers
	05.23.22	xiris	Reduced events to only have BC
	06.13.22	xiris	Restored events for Group/Party/Raid due to need to split raids sometimes (multiple camp pulling, jelvan balancing etc)	
						Added 'simple' cureMe request handler for manual entry (usually by MT)
	06.29.22	xiris 	Updated all events to only be either generic, or [/gsay, /rsay, /tell, MQ2DanNet(dgt, dt)] only			
	07.28.22	xiris   Updated the chkNeedCure request to have more must cure spells
	07.24.23	xiris	V2.0 Rewrote majority of code to handle new INI implementations
**|


| Request a Cure by TYPE[IMMEDIATE|REQUIRED|LAZY|ALL]
#EVENT cureRequest		"#1# tells the group#*#'cureRequest #2#'"
#EVENT cureRequest		"#1# tells you#*#'cureRequest #2#'"
#EVENT cureRequest		"#1# tells the raid#*#'cureRequest #2#'"
#EVENT cureRequest  	"[ #1# #*#]#*#cureRequest #2#"

| Request checking a group for a cure with LAZY[TRUE|FALSE]
#EVENT cureGroup	    "#1# tells the group#*#'cureGroup #2#'"
#EVENT cureGroup        "#1# tells you#*#'cureGroup #2#'"
#EVENT cureGroup        "#1# tells the raid#*#'cureGroup #2#'"
#EVENT cureGroup        "[ #1# #*#]#*#cureGroup #2#"

| Request an IMMEDIATE cure TYPE
#EVENT cureImmediate     "#1# tells the group#*#'cureImmediate#*#'"
#EVENT cureImmediate     "#1# tells you#*#'cureImmediate#*#'"
#EVENT cureImmediate     "#1# tells the raid#*#'cureImmediate#*#'"
#EVENT cureImmediate     "[ #1# #*#]#*#cureImmediate#*#"

| Request a REQUIRED cure TYPE
#EVENT cureRequired     "#1# tells the group#*#'cureRequired#*#'"
#EVENT cureRequired     "#1# tells you#*#'cureRequired#*#'"
#EVENT cureRequired     "#1# tells the raid#*#'cureRequired#*#'"
#EVENT cureRequired     "[ #1# #*#]#*#cureRequired#*#"

| Request a LAZY cure TYPE
#EVENT cureLazy         "#1# tells the group#*#'cureLazy#*#'"
#EVENT cureLazy         "#1# tells you#*#'cureLazy#*#'"
#EVENT cureLazy         "#1# tells the raid#*#'cureLazy#*#'"
#EVENT cureLazy         "[ #1# #*#]#*#cureLazy#*#"

| Toggle autoCuring for MT [TRUE|FALSE]
#EVENT autoCureMT       "#1# tells the group#*#'autoCureMT #2#'"
#EVENT autoCureMT       "#1# tells you#*#'autoCureMT #2#'"
#EVENT autoCureMT       "#1# tells the raid#*#'autoCureMT #2#'"
#EVENT autoCureMT       "[ #1# #*#]#*#autoCureMT #2#"


Sub initEvents_Curing
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /call RegisterClassEvents Events_Curing 3
	/call RegisterBackgroundEvents Events_Curing_Background 3
/return

Sub Events_Curing
	/doevents cureRequest
	/doevents cureGroup
	/doevents cureImmediate
    /doevents cureRequired 
/return

Sub Events_Curing_Background
    /doevents autoCureMT
    /doevents cureLazy
    /if (${Select[${Me.Class.ShortName},CLR,DRU,PAL,SHM]} && ((!${bln_engaged} && !${timer_check_cures}) || (${bln_engaged}))) /call chkCures
/return

Sub EVENT_cureRequest(string _line, string _sender,  string _eventParams)
    /call chkCureType_${_eventParams} ${_sender} ${Spawn[pc ${_sender}].ID}
    /doevents flush cureRequest
/return

Sub EVENT_cureGroup(string _line, string _sender, string _eventParams)
    /call chkCure_Group ${_eventParams}
    /doevents flush cureGroup
/return 

Sub EVENT_cureImmediate(string _line, string _sender,  string _eventParams)
    /call chkCureType_IMMEDIATE ${_sender} ${Spawn[pc ${_sender}].ID}
    /doevents flush cureImmediate
/return

Sub EVENT_cureRequired(string _line, string _sender,  string _eventParams)
    /call chkCureType_REQUIRED ${_sender} ${Spawn[pc ${_sender}].ID}
    /doevents flush cureRequired
/return

Sub EVENT_cureLazy(string _line, string _sender,  string _eventParams)
    /call chkCureType_LAZY ${_sender} ${Spawn[pc ${_sender}].ID}
    /doevents flush cureLazy
/return

Sub EVENT_autoCureMT(string _line, string _sender, string _eventParams)
    /varset bln_autoCureMT ${_eventParams}
    /doevents flush autoCureMT
/return 




Sub xbot_initialize_curing
    /echo \n \apInitialize: \aoCuring \aw-------------------------

    /if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /call processCureSpellSettings
    /call processCureDebuffList
    
    /call initEvents_Curing
/return 




| Set up the cure spells for priest classes
Sub processCureSpellSettings
    /echo \awprocessCureSettings
    /declare timer_check_cures  timer   outer   0
    
    /declare int_defaultCureGem             int     outer   7
    /declare bln_hasCure_radiant            bool    outer   FALSE
    /declare bln_useCure_radiant            bool    outer   FALSE
    /declare str_cure_radiant               string  outer   "Radiant Cure"
    /declare str_cure_radiant_counters      string  outer   25

    /declare bln_hasCure_corruption         bool    outer   FALSE
    /declare bln_hasCure_corruption_group   bool    outer   FALSE 
    /declare bln_hasCure_curse              bool    outer   FALSE 
    /declare bln_hasCure_curse_group        bool    outer   FALSE 
    /declare bln_hasCure_disease            bool    outer   FALSE 
    /declare bln_hasCure_disease_group      bool    outer   FALSE 
    /declare bln_hasCure_poison             bool    outer   FALSE 
    /declare bln_hasCure_poison_group       bool    outer   FALSE 
    
    /declare str_cure_disease               string  outer   NULL
    /declare str_cure_disease_group         string  outer   NULL
    /declare str_cure_curse                 string  outer   NULL
    /declare str_cure_curse_group           string  outer   NULL
    /declare str_cure_poison                string  outer   NULL
    /declare str_cure_poison_group          string  outer   NULL
    /declare str_cure_corruption            string  outer   NULL
    /declare str_cure_corruption_group      string  outer   NULL

    /declare bln_curing_enabled             bool    outer  TRUE
    /declare bln_autoCureMT                 bool    outer  TRUE
    /declare bln_autoCureSelf               bool    outer  TRUE
    /declare bln_autoCureGroup              bool    outer  TRUE
    /declare bln_autoCureLazy               bool    outer  TRUE


    |; _enabled|_autoCureMT|_autoCureSelf|_autoCureGroup|_checkLazyCures
	| settings=TRUE|TRUE|TRUE|TRUE|TRUE
    /if (!${Ini[INI_CHAR,CURES,settings].Arg[1,|]})  /varset bln_curing_enabled     FALSE
    /if (!${Ini[INI_CHAR,CURES,settings].Arg[2,|]})  /varset bln_autoCureMT         FALSE
    /if (!${Ini[INI_CHAR,CURES,settings].Arg[3,|]})  /varset bln_autoCureSelf       FALSE
    /if (!${Ini[INI_CHAR,CURES,settings].Arg[4,|]})  /varset bln_autoCureGroup      FALSE
    /if (!${Ini[INI_CHAR,CURES,settings].Arg[5,|]})  /varset bln_autoCureLazy       FALSE


    |; _radiant|_use|_type
    /if (${Ini[INI_CHAR,CURES,CURE_radiant].Arg[1,|].NotEqual[NULL]})  /varset bln_hasCure_radiant TRUE
    /if (${Ini[INI_CHAR,CURES,CURE_radiant].Arg[1,|].NotEqual[NULL]})  /varset str_cure_radiant    ${Ini[INI_CHAR,CURES,CURE_radiant].Arg[1,|]}  
    /if (${Ini[INI_CHAR,CURES,CURE_radiant].Arg[2,|]})  /varset bln_useCure_radiant TRUE  
    /if (${Ini[INI_CHAR,CURES,CURE_radiant].Arg[3,|]})  /varset bln_useCure_radiant ${Ini[INI_CHAR,CURES,CURE_radiant].Arg[3,|]}    

    /echo \a-w    Settings Processed: \awEnabled:\ag${bln_curing_enabled} \awAllowMT:\ag${bln_autoCureMT} \awAllowSelf:\ag${bln_autoCureSelf} \awAllowGroup:\ag${bln_autoCureGroup} \awLazy:\ag${bln_autoCureLazy}

    |;CURE_poison="Antidote"|TRUE|64
    |CURE_curse="Remove Greater Curse"|TRUE|45 
    | curse
    /if (${Ini[INI_CHAR,CURES,CURE_curse].NotEqual[NULL]}) {
        /varset bln_hasCure_curse           TRUE 
        /varset str_cure_curse ${Ini[INI_CHAR,CURES,CURE_curse]}
    }        
    /if (${Ini[INI_CHAR,CURES,CURE_curse_group].NotEqual[NULL]}) {
        /varset bln_hasCure_curse_group     TRUE 
        /varset str_cure_curse_group ${Ini[INI_CHAR,CURES,CURE_curse_group]}
    }

    | disease
    /if (${Ini[INI_CHAR,CURES,CURE_disease].NotEqual[NULL]}) {
        /varset bln_hasCure_disease         TRUE 
        /varset str_cure_disease ${Ini[INI_CHAR,CURES,CURE_disease]}
    }     
    /if (${Ini[INI_CHAR,CURES,CURE_disease_group].NotEqual[NULL]}) {
        /varset bln_hasCure_disease_group   TRUE 
        /varset str_cure_disease_group ${Ini[INI_CHAR,CURES,CURE_disease_group]}
    }

    | poison
    /if (${Ini[INI_CHAR,CURES,CURE_poison].NotEqual[NULL]}) {
        /varset bln_hasCure_poison          TRUE    
        /varset str_cure_poison ${Ini[INI_CHAR,CURES,CURE_poison]}
    }
    /if (${Ini[INI_CHAR,CURES,CURE_poison_group].NotEqual[NULL]}) { 
        /varset bln_hasCure_poison_group    TRUE    
        /varset str_cure_poison_group ${Ini[INI_CHAR,CURES,CURE_poison_group]}
    }

    | Group Cure settings (for use with chkCure_Group)
    /declare groupCurseCured    int     outer ${str_cure_curse_group.Arg[3,|]}
    /declare groupDiseaseCured  int     outer ${str_cure_disease_group.Arg[3,|]}
    /declare groupPoisonCured   int     outer ${str_cure_poison_group.Arg[3,|]}

    /declare maxGroupCurseCured int outer ${groupCurseCured}
    /varcalc maxGroupCurseCured ${groupCurseCured} * 2

    /declare maxGroupDiseaseCured int outer ${groupDiseaseCured}
    /varcalc maxGroupDiseaseCured ${groupDiseaseCured} * 2

    /declare maxGroupPoisonCured int outer ${groupPoisonCured}
    /varcalc maxGroupPoisonCured ${groupPoisonCured} * 2


    /echo \a-w    Settings Processed: \aySINGLE\awCurse:\ag${bln_hasCure_curse}:\ao${str_cure_curse} \awDisease:\ag${bln_hasCure_disease}:\ao${str_cure_disease} \awPoison:\ag${bln_hasCure_poison}:\ao${str_cure_poison}
    /echo \a-w    Settings Processed: \ayGROUP \awCurse:\ag${bln_hasCure_curse_group}:\ao${str_cure_curse_group} \awDisease:\ag${bln_hasCure_disease_group}:\ao${str_cure_disease_group} \awPoison:\ag${bln_hasCure_poison_group}:\ao${str_cure_poison_group}
/return 

| Set up the cure requiremmments for all classes (for their inner loop checks) 
Sub processCureDebuffList
    /echo \awprocessCureDebuffList
    /declare int_immediateDebuffCures_total int outer 0
    /declare int_requiredDebuffCures_total  int outer 0

    | Debuffs that must be cured as soon as possible
    | [CURING_DEBUFF_IMMEDIATE]
    | ;_name|_cureType[disease||poison||curse||corruption]|_targets[ALL|MT|Class.ShortName CSV]
    /declare raw_immediateCures[20]     string  local NULL
    /declare raw_requiredCures[20]      string  local NULL
    /declare i                          int     local 0
    /declare sz                         int     local 0 
    /declare test                       string  local NULL


    | Tracking information for all classes
    | Immediate Cure Requirments, clients keep a list of /counters per type
    | We can then more quickly query them and determine if use group
    /declare str_queryCurable           string  outer   NULL
    /declare has_immediate              bool    outer   NULL
    /declare int_immediate_curse        int     outer   NULL
    /declare int_immediate_disease      int     outer   NULL
    /declare int_immediate_poison       int     outer   NULL

    /declare has_required               bool    outer   NULL
    /declare int_required_curse         int     outer   NULL
    /declare int_required_disease       int     outer   NULL
    /declare int_required_poison        int     outer   NULL

    /declare str_debuffQueryResult      string outer NULL
    /declare int_IMMEDIATE_total        int outer 0
    /declare int_IMMEDIATE_curse        int outer 0
    /declare int_IMMEDIATE_disease      int outer 0
    /declare int_IMMEDIATE_poison       int outer 0

    /declare int_REQUIRED_total         int outer 0
    /declare int_REQUIRED_curse         int outer 0
    /declare int_REQUIRED_disease       int outer 0
    /declare int_REQUIRED_poison        int outer 0

    /declare str_queryTF                string outer NULL
    /declare str_queryDetails           string outer NULL

    :immediate
    /for i 1 to ${raw_immediateCures.Size}
        /varset test ${Ini[INI_COMMON,CURING_DEBUFF_IMMEDIATE,DEBUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_immediateDebuffCures_total ${sz}
            /goto :required
        }
        | Not null, set the values for the debuffs to be cured
        /varcalc sz ${sz}+1
        /declare DEBUFF_IMMEDIATE_${i}          string  outer   ${Ini[INI_COMMON,CURING_DEBUFF_IMMEDIATE,DEBUFF_${i}].Arg[1,|]}
        /declare DEBUFF_IMMEDIATE_${i}_Type     string  outer   ${Ini[INI_COMMON,CURING_DEBUFF_IMMEDIATE,DEBUFF_${i}].Arg[2,|]}
        /declare DEBUFF_IMMEDIATE_${i}_Targets  string  outer   ${Ini[INI_COMMON,CURING_DEBUFF_IMMEDIATE,DEBUFF_${i}].Arg[3,|]}
        /echo \ao        DEBUFF_IMMEDIATE[\ag${i}\ao]: \awDebuff:\ag${DEBUFF_IMMEDIATE_${i}} \awType:\ag${DEBUFF_IMMEDIATE_${i}_Type} \awTargets:\ag${DEBUFF_IMMEDIATE_${i}_Targets}
    /next i

    :required
    /varset sz 0
    /for i 1 to ${raw_requiredCures.Size}
        | if the value of the entry ends with a quoted string, we need to wrap it all in quotes again.
        /varset test ${Ini[INI_COMMON,CURING_DEBUFF_REQUIRED,DEBUFF_${i}]}
        /if (${test.Arg[1,|].Equal[NULL]}) {
            /varset int_requiredDebuffCures_total ${sz}
            /return
        }
        | Not null, set the values for the debuffs to be cured
        /varcalc sz ${sz}+1
        /declare DEBUFF_REQUIRED_${i}          string   outer   ${Ini[INI_COMMON,CURING_DEBUFF_REQUIRED,DEBUFF_${i}].Arg[1,|]}
        /declare DEBUFF_REQUIRED_${i}_Type     string   outer   ${Ini[INI_COMMON,CURING_DEBUFF_REQUIRED,DEBUFF_${i}].Arg[2,|]}
        /declare DEBUFF_REQUIRED_${i}_Targets  string   outer   ${Ini[INI_COMMON,CURING_DEBUFF_REQUIRED,DEBUFF_${i}].Arg[3,|]}
        /echo \ao        DEBUFF_REQUIRED[\ag${i}\ao]: \awDebuff:\ag${DEBUFF_REQUIRED_${i}} \awType:\ag${DEBUFF_REQUIRED_${i}_Type} \awTargets:\ag${DEBUFF_REQUIRED_${i}_Targets}
    /next i    
/return 

| RUN on everytoon, in their loop, so the DQUERY from curer can check the bools for each type instead of querying each type?
| Since we want the immediate debuffs here already counted by the clients
| So we can determine in the curegroup function if we use group cures
Sub determineMyCureCounters
    /declare i int local
    /declare bln_MTApplies           bool local TRUE

    /varset has_immediate           FALSE
    /varset int_immediate_curse     0
    /varset int_immediate_disease   0
    /varset int_immediate_poison    0 
    
    /varset has_required            FALSE
    /varset int_required_curse      0
    /varset int_required_disease    0
    /varset int_required_poison     0 

    /varset str_queryCurable        NULL

    /for i 1 to ${int_immediateDebuffCures_total}
        |/echo IMMEDIATE: ${DEBUFF_IMMEDIATE_${i}} ${Me.Buff[${DEBUFF_IMMEDIATE_${i}}].ID} 
        /if (${DEBUFF_IMMEDIATE_${i}_Targets.Equal[MT]} && (${Me.ID} != ${int_MTID})) /varset bln_MTApplies FALSE
        /if (!${Select[${Me.Class.ShortName},${DEBUFF_REQUIRED_${i}_Targets} ]} && !${DEBUFF_IMMEDIATE_${i}_Targets.Equal[ALL]} && (${bln_MTApplies})) {
            |/echo not a spell my class needs to cure
            /goto :nextImmediate
        }
        /if (${Me.Buff[${DEBUFF_IMMEDIATE_${i}}].ID}) {
            /if (${DEBUFF_IMMEDIATE_${i}_Type.Equal[curse]})    /varset int_immediate_curse    ${Debuff.Cursed}
            /if (${DEBUFF_IMMEDIATE_${i}_Type.Equal[disease]})  /varset int_immediate_disease  ${Debuff.Diseased}
            /if (${DEBUFF_IMMEDIATE_${i}_Type.Equal[poison]})   /varset int_immediate_poison   ${Debuff.Poisoned}
            /varset has_immediate TRUE
        }
    :nextImmediate
    /next i

    | We don't care as much about required buffs, so we dont need their counters
    /for i 1 to ${int_requiredDebuffCures_total}
        /if (${DEBUFF_REQUIRED_${i}_Targets.Equal[MT]} && (${Me.ID} != ${int_MTID})) /varset bln_MTApplies FALSE
        /if (!${Select[${Me.Class.ShortName},${DEBUFF_REQUIRED_${i}_Targets} ]} && !${DEBUFF_REQUIRED_${i}_Targets.Equal[ALL]} && (${bln_MTApplies})) {
            |/echo not a spell my class needs to cure
            /goto :nextRequired
        }
        /if (${Me.Buff[${DEBUFF_REQUIRED_${i}}].ID}) {
            /if (${DEBUFF_REQUIRED_${i}_Type.Equal[curse]})    /varset int_required_curse    ${Debuff.Cursed}
            /if (${DEBUFF_REQUIRED_${i}_Type.Equal[disease]})  /varset int_required_disease  ${Debuff.Diseased}
            /if (${DEBUFF_REQUIRED_${i}_Type.Equal[poison]})   /varset int_required_poison   ${Debuff.Poisoned}
            /varset has_required TRUE
        }
    :nextRequired
    /next i

    /varset str_queryCurable ${has_immediate}~${int_immediate_curse},${int_immediate_disease},${int_immediate_poison}|${has_required}~${int_required_curse},${int_required_disease},${int_required_poison}
    |/echo \aodetermineMyCureCounters \ag ${str_queryCurable} \aw${Time.Time24}
   
/return 



| Check Cures, we can use !bln_engaged as a determination for _lazy cures.
| Note this is generally disabled, since only a few priests are ever assigned to autocure MT
Sub chkCures
    /declare lazy bool local TRUE
    /if (${bln_engaged}) /varset lazy FALSE

    /echo chkCures MT: ${bln_autoCureMT} Self:${bln_autoCureSelf} Group: ${bln_autoCureGroup} Lazy:${lazy} Engaged?:${bln_engaged}
	/if (${bln_autoCureMT}) 	/call chkCure_MT    ${lazy}
 	/if (${bln_autoCureSelf}) 	/call chkCure_Self  ${lazy}
	/if (${bln_autoCureGroup})  /call chkCure_Group ${lazy}
    /varset timer_check_cures 10s
/return

| Check Cures for the MT. Optionally pass in TRUE for Lazy if you want to check LAZY cures as well.
| Note this is generally disabled, since "/dt CLERICNAME autocure TRUE" sets MT checking for priests
Sub chkCure_MT(bool _lazyMT)
    /echo \awchkCure_MT(\ao${_lazyMT}\aw)
    | Check Immediate Cures
    /call chkCureType_IMMEDIATE ${Spawn[id ${int_MTID}]} ${int_MTID}
    | Check Required Cures
    /call chkCureType_REQUIRED ${Spawn[id ${int_MTID}]} ${int_MTID}
    | Check Lazy Cures 
    /if (${_lazyMT} && ${bln_autoCureLazy}) /call chkCureType_LAZY ${Spawn[id ${int_MTID}]} ${int_MTID}
/return 

| Check Cures for SELF. Optionally pass in TRUE for Lazy if you want to check LAZY cures as well.
| Note this is generally disabled, since GROUP checks self
Sub chkCure_Self(bool _lazySelf)
    /echo \awchkCure_Self(\ao${_lazySelf}\aw)
    | Check Immediate Cures
    /call chkCureType_IMMEDIATE ${Me.Name} ${Me.ID}
    | Check Required Cures
    /call chkCureType_REQUIRED  ${Me.Name} ${Me.ID}
    | Check Lazy Cures 
    /if (${_lazySelf} && ${bln_autoCureLazy}) /call chkCureType_LAZY ${Me.Name} ${Me.ID}
/return 

| Check Cures for GROUP. Optionally pass in TRUE for Lazy if you want to check LAZY cures as well.
| This is usually the only check a group priest will use.
Sub chkCure_Group(bool _lazyGroup)
    /echo \awchkCure_Group(\ao${_lazyGroup}\aw)

    | Create local vars for processing
    /declare r int local 0
    /declare g int local ${Group.GroupSize}
    /varcalc g (${Group.GroupSize} - 1)
    /declare sz int local 0
    /declare useGroupCure               bool    local FALSE
    /declare castCount                  int     local 0
    /declare cureType                   string  local disease
    /declare arr_IMMEDIATE_list[10]      string local NULL
    /declare arr_REQUIRED_list[10]       string local NULL

    | Reset processing vars
    /varset str_debuffQueryResult       NULL
    /varset str_queryTF                 NULL
    /varset str_queryDetails            NULL
    /varset int_IMMEDIATE_total         0
    /varset int_IMMEDIATE_curse         0
    /varset int_IMMEDIATE_disease       0
    /varset int_IMMEDIATE_poison        0

    /varset int_REQUIRED_total          0
    /varset int_REQUIRED_curse          0
    /varset int_REQUIRED_disease        0
    /varset int_REQUIRED_poison         0

    | ------
    | IMMEDIATE/REQUIRED Debuff Determination
    | Looka at the queryCurable which every toon sets in a loop with determineMyCureCounters()
    | If found, += the current counters for each curetype
    | ------

    /for r 1 to ${g}
        /varcalc sz ${r}-1
        /dquery ${Group.Member[${sz}].Name} -q "str_queryCurable" -o str_debuffQueryResult
        /delay 20 ${DanNet.Q.Received}
        
        |Process the immediate results
        /echo \apstr_debuffQueryResult \ao${Group.Member[${sz}].Name} \ag ${str_debuffQueryResult}
        /varset str_queryTF         ${str_debuffQueryResult.Arg[1,|].Arg[1,~]}
        /varset str_queryDetails    ${str_debuffQueryResult.Arg[1,|].Arg[2,~]}
        /echo \ao ${Group.Member[${sz}].Name} IMMEDIATE \apstr_debuffQueryParsed:: \ao${str_queryTF} \apRaw:: \ao${str_queryDetails}
        /if (${str_queryTF.Equal[TRUE]}) {
  
            /varset arr_IMMEDIATE_list[${r}] ${Group.Member[${sz}].Name} 
            /varcalc  int_IMMEDIATE_total    ${int_IMMEDIATE_total} + 1
            /echo \ar${Group.Member[${sz}].Name} has REQUIRED! sz:${sz}::${Group.Member[${sz}].Name} arr_REQUIRED_list:${arr_IMMEDIATE_list[${r}]}
            /if (${Int[${str_queryDetails.Arg[1,,]}]} > ${int_IMMEDIATE_curse})   /varset int_IMMEDIATE_curse      ${Int[${str_queryDetails.Arg[1,,]}]}
            /if (${Int[${str_queryDetails.Arg[2,,]}]} > ${int_IMMEDIATE_disease}) /varset int_IMMEDIATE_disease    ${Int[${str_queryDetails.Arg[2,,]}]}
            /if (${Int[${str_queryDetails.Arg[3,,]}]} > ${int_IMMEDIATE_poison})  /varset int_IMMEDIATE_poison     ${Int[${str_queryDetails.Arg[3,,]}]}
            /echo \ap    curse:\aw${int_IMMEDIATE_curse} \apdisease:\aw${int_IMMEDIATE_disease} \appoison: \aw${int_IMMEDIATE_poison}
        }

        /varset str_queryTF         ${str_debuffQueryResult.Arg[2,|].Arg[1,~]}
        /varset str_queryDetails    ${str_debuffQueryResult.Arg[2,|].Arg[2,~]}
        /echo  \ao ${Group.Member[${sz}].Name} REQUIRED \apstr_debuffQueryParsed:: \ao${str_queryTF} \apRaw:: \ao${str_queryDetails}
        /if (${str_queryTF.Equal[TRUE]}) {

            /varset arr_REQUIRED_list[${r}] ${Group.Member[${sz}].Name} 
            /varcalc  int_REQUIRED_total    ${int_REQUIRED_total} + 1
             /echo \ar${Group.Member[${sz}].Name} has REQUIRED! sz:${sz}::${Group.Member[${sz}].Name} arr_REQUIRED_list:${arr_REQUIRED_list[${r}]}
            /if (${Int[${str_queryDetails.Arg[1,,]}]} > ${int_REQUIRED_curse})   /varset int_REQUIRED_curse    ${Int[${str_queryDetails.Arg[1,,]}]}
            /if (${Int[${str_queryDetails.Arg[2,,]}]} > ${int_REQUIRED_disease}) /varset int_REQUIRED_disease  ${Int[${str_queryDetails.Arg[2,,]}]}
            /if (${Int[${str_queryDetails.Arg[3,,]}]} > ${int_REQUIRED_poison})  /varset int_REQUIRED_poison   ${Int[${str_queryDetails.Arg[3,,]}]}
            /echo \ap    curse:\aw${int_REQUIRED_curse} \apdisease:\aw${int_REQUIRED_disease} \appoison: \aw${int_REQUIRED_poison}
        }
        /echo --------------------
    /next r

    | ------
    | DETERMINE the IMMEDIATE requirements/counters
    | ------
    /if (${int_IMMEDIATE_total} > 2) {
        /echo \awint_IMMEDIATE_curse:\ag${int_IMMEDIATE_curse} \awint_IMMEDIATE_disease:\ag${int_IMMEDIATE_disease} \awint_IMMEDIATE_poison:\ag${int_IMMEDIATE_poison}
        /echo   \awgroupCurseCured:\ag${groupCurseCured} \awmaxGroupCurseCured:\ag${maxGroupCurseCured}
        /echo   \awgroupDiseaseCured:\ag${groupDiseaseCured} \awmaxGroupDiseaseCured:\ag${maxGroupDiseaseCured}
        /echo   \awgroupPoisonCured:\ag${groupPoisonCured} \awmaxGroupPoisonCured:\ag${maxGroupPoisonCured}
        /if (${int_IMMEDIATE_curse} > 0 &&  ${int_IMMEDIATE_curse} <= ${maxGroupCurseCured}) {
            /varcalc castCount ${Math.Calc[${Math.Calc[${Math.Calc[${int_IMMEDIATE_curse}+${groupCurseCured}]}-1]} / ${groupCurseCured} ]}
            /echo curseCastCount ${castCount}
            /varset cureType curse
            /varset useGroupCure TRUE
        } else /if (${int_IMMEDIATE_disease} > 0 &&  ${int_IMMEDIATE_disease} <= ${maxGroupDiseaseCured}) {
            /varcalc castCount ${Math.Calc[${Math.Calc[${Math.Calc[${int_IMMEDIATE_disease}+${groupDiseaseCured}]}-1]} / ${groupDiseaseCured} ]}
            /echo diseaseCastCount ${castCount}
            /varset useGroupCure TRUE
            /varset cureType disease
        } else /if (${int_IMMEDIATE_poison} > 0 &&  ${int_IMMEDIATE_poison} <= ${maxGroupPoisonCured}) {
            /varcalc castCount ${Math.Calc[${Math.Calc[${Math.Calc[${int_IMMEDIATE_poison}+${groupPoisonCured}]}-1]} / ${groupPoisonCured} ]}
            /echo poisonCastCount ${castCount}
            /varset useGroupCure TRUE
            /varset cureType poison
        }
    }
    | ------
    | CAST the GROUP/SOLO IMMEDIATE Cures
    | We had immediate! and we have group cure! fire it!
    | ------
    /if (${int_IMMEDIATE_total} && ${useGroupCure}) {
        | We determined to use group cure here
        /echo \arCURING GROUP!
        /call castCureV2 ${Me.Name} ${Me.ID} "${str_cure_${cureType}_group.Arg[1,|]}" ${castCount} TRUE
    } else /if (${int_IMMEDIATE_total} && !${useGroupCure}) {
        | No group cure, cure by single, cure until all are cured
        /for r 1 to ${arr_IMMEDIATE_list.Size}
            /varcalc sz ${r}-1
            /echo \aoCURING \arIMMEDIATE \aoSINGLE GROUP MEMBERS! ${arr_IMMEDIATE_list[${r}]} ${Group.Member[${sz}].Name} ${Group.Member[${sz}].ID}
            /if ( ${arr_IMMEDIATE_list[${r}].NotEqual[NULL]}) {
                /echo calling chkCureType_IMMEDIATE
                /call chkCureType_IMMEDIATE ${Group.Member[${sz}].Name} ${Group.Member[${sz}].ID}
            } 
        /next r
    }


    | We had to have cured some immediates, lets return out so we can recheck
    /if (${int_IMMEDIATE_total}) {
        /echo \arWE Had Immediates
    } 

    | ------
    | DETERMINE the IMMEDIATE requirements/counters
    | ------
    /if (${int_REQUIRED_total} > 2) {
        /echo \awint_REQUIRED_curse:\ag${int_REQUIRED_curse} \awint_REQUIRED_disease:\ag${int_REQUIRED_disease} \awint_REQUIRED_poison:\ag${int_REQUIRED_poison}
        /echo   \awgroupCurseCured:\ag${groupCurseCured} \awmaxGroupCurseCured:\ag${maxGroupCurseCured}
        /echo   \awgroupDiseaseCured:\ag${groupDiseaseCured} \awmaxGroupDiseaseCured:\ag${maxGroupDiseaseCured}
        /echo   \awgroupPoisonCured:\ag${groupPoisonCured} \awmaxGroupPoisonCured:\ag${maxGroupPoisonCured}
        /if (${int_REQUIRED_curse} > 0 &&  ${int_REQUIRED_curse} <= ${maxGroupCurseCured}) {
            /varcalc castCount ${Math.Calc[${Math.Calc[${Math.Calc[${int_REQUIRED_curse}+${groupCurseCured}]}-1]} / ${groupCurseCured} ]}
            /echo curseCastCount ${castCount}
            /varset cureType curse
            /varset useGroupCure TRUE
        } else /if (${int_REQUIRED_disease} > 0 &&  ${int_REQUIRED_disease} <= ${maxGroupDiseaseCured}) {
            /varcalc castCount ${Math.Calc[${Math.Calc[${Math.Calc[${int_REQUIRED_disease}+${groupDiseaseCured}]}-1]} / ${groupDiseaseCured} ]}
            /echo diseaseCastCount ${castCount}
            /varset useGroupCure TRUE
            /varset cureType disease
        } else /if (${int_REQUIRED_poison} > 0 &&  ${int_REQUIRED_poison} <= ${maxGroupPoisonCured}) {
            /varcalc castCount ${Math.Calc[${Math.Calc[${Math.Calc[${int_REQUIRED_poison}+${groupPoisonCured}]}-1]} / ${groupPoisonCured} ]}
            /echo poisonCastCount ${castCount}
            /varset useGroupCure TRUE
            /varset cureType poison
        }
    }

     | We had required! and we have group cure! fire it!
    /if (${int_REQUIRED_total} && ${useGroupCure}) {
        | We determined to use group cure here
        /echo \arCURING GROUP!
        /call castCureV2 ${Me.Name} ${Me.ID} "${str_cure_${cureType}_group.Arg[1,|]}" ${castCount} TRUE
    } else /if (${int_REQUIRED_total} && !${useGroupCure}) {
        | No group cure, cure by single, if someone is cured, exit and recheck since required asrent as important as immediates
        /echo \aoCURING SINGLE GROUP MEMBERS!
          /for r 1 to ${arr_REQUIRED_list.Size}
            /varcalc sz ${r}-1
            /echo \aoCURING \arREQUIRED \aoSINGLE GROUP MEMBERS! ${arr_REQUIRED_list[${r}]} ${Group.Member[${sz}].Name} ${Group.Member[${sz}].ID}
            /if ( ${arr_REQUIRED_list[${r}].NotEqual[NULL]}) {
                /echo calling chkCureType_IMMEDIATE
                /call chkCureType_REQUIRED ${Group.Member[${sz}].Name} ${Group.Member[${sz}].ID}
            } 
            /if (${Macro.Return.Equal[CURED]}) /return
        /next r
    }

    | We had to have cured some required, lets return out so we can recheck
    /if (${int_REQUIRED_total}) /return 

    | Check the lazy debuffs, ie: we are out of combat and have the time
    /if (${_lazyGroup} && ${bln_autoCureLazy}) {
        /for r 1 to ${g}
            /varcalc sz ${r}-1
            /call chkCureType_LAZY ${Group.Member[${sz}].Name} ${Group.Member[${sz}].ID}
        /next r
    }
/return 

| Utility method, returns the correct TLO property for the Debuff.TLO - annoyingly they are adjectives
Sub getMQ2DebuffCounterString(string _cureType)
    /declare q string local Diseased
    /if (${_cureType.Equal[poison]}) {
        /varset q Poisoned
    } else /if (${_cureType.Equal[curse]}) {
        /varset q Cursed
    }
/return  ${q}

| Direct call from EVENT cureRequest(_TYPE[ALL|IMMEDIATE|REQUIRED|LAZY]) not generally used
Sub chkCureType_ALL(string _victimName, int _id)
    /call chkCureType_IMMEDIATE ${_victimName} ${_id}
    /call chkCureType_REQUIRED  ${_victimName} ${_id}
/return 

| Since we want the immediate debuffs here already counted by the clients
| So we can determine in the curegroup function if we use group cures
sub chkCureType_IMMEDIATE(string _victimName, int _id)
    /echo \awchkCureType_IMMEDIATE(\ao${_victimName}) 
    /declare i 				    int 	local	0
    /declare foundCounters	    int		local	0
    /declare foundDebuff        bool    local   FALSE
    /declare qImmediateString   string  local  FALSE
    /declare dCounterString     string  local NULL

    | Check if the victim has any immediates
    /dquery ${_victimName} -q "has_immediate" -o foundDebuff
    /delay 10 ${DanNet.Q.Received}
    /if (!${foundDebuff}) /return 

    /for i 1 to ${int_immediateDebuffCures_total}
        /varset foundCounters 0
        |---------
        /echo \aoDebuffName \ag${DEBUFF_IMMEDIATE_${i}} \aoDebuffType \ag${DEBUFF_IMMEDIATE_${i}_Type} \aoDebuffType \ag${DEBUFF_IMMEDIATE_${i}_Targets}
        /if (!${Select[${Spawn[id ${_id}].Class.ShortName},${DEBUFF_IMMEDIATE_${i}_Targets}]} && !${DEBUFF_IMMEDIATE_${i}_Targets.Equal[ALL]} && (${DEBUFF_IMMEDIATE_${i}_Targets.Equal[MT]} && ${_id} != ${int_MTID})) {
            /echo DID NOT FIND THIS CLASS AS PART OF CHECK ${Spawn[id ${_id}].Class.ShortName} => ${DEBUFF_IMMEDIATE_${i}_Targets}
            /goto :nextImmediate
        }
        /dquery ${_victimName} -q "Me.Buff[${DEBUFF_IMMEDIATE_${i}}].ID" -o foundDebuff
        /delay 10 ${DanNet.Q.Received}
        /if (!${foundDebuff}) /goto :nextImmediate
        |---------
        /call getMQ2DebuffCounterString ${DEBUFF_IMMEDIATE_${i}_Type}
        /varset dCounterString ${Macro.Return}
        /dquery ${_victimName} -q "Debuff.${dCounterString}" -o foundCounters
        /delay 10 ${DanNet.Q.Received}
        /if (!${foundCounters}) /goto :nextImmediate
        |---------
        /echo \ar Immediate Cure Necessary! \ao ${DEBUFF_IMMEDIATE_${i}}
        /call handleCureRequestV2 ${_victimName} ${_id} "${DEBUFF_IMMEDIATE_${i}}" ${DEBUFF_IMMEDIATE_${i}_Type} ${foundCounters} TRUE
        | If we cured something from immediate cures, we want to bail out of this to continue our checks on others
        /return CURED
    :nextImmediate
    /next i
    /echo \ag No Immediate Cures Necessary.
/return


Sub chkCureType_REQUIRED(string _victimName, int _id)
    /echo \awchkCureType_REQUIRED \aw(\ao${_victimName}\ao(\aw${_id}\ao)) 
    /declare i 				int 	local	0
    /declare foundCounters	int		local	0
    /declare foundDebuff    bool    local   FALSE
    /declare dCounterString string  local NULL

    | Check if the victim has any immediates
    /dquery ${_victimName} -q "has_required" -o foundDebuff
    /delay 10 ${DanNet.Q.Received}
    /if (!${foundDebuff}) /return 

    /for i 1 to ${int_requiredDebuffCures_total}
        /varset foundCounters 0
        |---------
        |/echo \aoDebuffName \ag${DEBUFF_REQUIRED_${i}} \aoDebuffType \ag${DEBUFF_REQUIRED_${i}_Type} \aoDebuffType \ag${DEBUFF_REQUIRED_${i}_Targets}
        /if (!${Select[${Spawn[id ${_id}].Class.ShortName},${DEBUFF_REQUIRED_${i}_Targets}]} && !${DEBUFF_REQUIRED_${i}_Targets.Equal[ALL]} && (${DEBUFF_REQUIRED_${i}_Targets.Equal[MT]} && ${_id} != ${int_MTID})) {
            |/echo DID NOT FIND THIS CLASS AS PART OF CHECK ${Spawn[id ${_id}].Class.ShortName} => ${DEBUFF_REQUIRED_${i}_Targets}
            /goto :nextRequired
        }
        /dquery ${_victimName} -q "Me.Buff[${DEBUFF_REQUIRED_${i}}].ID" -o foundDebuff
        /delay 10 ${DanNet.Q.Received}
        /if (!${foundDebuff}) /goto :nextRequired
        |---------
        /call getMQ2DebuffCounterString ${DEBUFF_REQUIRED_${i}_Type}
        /varset dCounterString ${Macro.Return}
        /dquery ${_victimName} -q "Debuff.${dCounterString}" -o foundCounters
        /delay 10 ${DanNet.Q.Received}
        /if (!${foundCounters}) /goto :nextInextRequiredmmediate
        |---------
        /echo \ar Required Cure Necessary! \ao ${DEBUFF_REQUIRED_${i}}
        /call handleCureRequestV2 ${_victimName} ${_id} "${DEBUFF_REQUIRED_${i}}" ${DEBUFF_REQUIRED_${i}_Type} ${foundCounters} TRUE
        | If we cured something from immediate cures, we want to bail out of this to continue our checks on others
        /return CURED
    :nextRequired
    /next i
    /echo \ag No Required Cures Necessary.
/return 


Sub chkCureType_LAZY(string _victimName, int _id)
    /echo \awchkCureType_LAZY(\ao${_victimName}) 
	| Check if MT needs cure
	| Specific items that need force cure
	/declare foundCounters	int		local	0
    /declare foundDebuff    bool    local   FALSE

    | If no counters, return out
    /dquery ${_victimName} -q "Debuff.Counters" -o foundCounters
    /delay 10 ${DanNet.Q.Received}
    /if (!${foundCounters}) /return

    | Check Disease
    :diseaseCure
    /if (!${bln_hasCure_disease}) /goto :poisonCure
    /varset foundCounters 0
    /dquery ${_victimName} -q "Debuff.Diseased" -o foundCounters
    /delay 10 ${DanNet.Q.Received}
    /if (${foundCounters}) {
        /call handleCureRequestV2 ${_victimName} ${_id} NOTSPECIFIED disease ${foundCounters} FALSE
    }

    | Check Poison
    :poisonCure
    /if (!${bln_hasCure_poison}) /goto :curseCure
    /varset foundCounters 0
    /dquery ${_victimName} -q "Debuff.Poisoned" -o foundCounters
    /delay 10 ${DanNet.Q.Received}
    /if (${foundCounters}) {
        /call handleCureRequestV2 ${_victimName} ${_id} NOTSPECIFIED poison ${foundCounters} FALSE
    }

    | Check Curse
    :curseCure
    /if (!${bln_hasCure_curse}) /return
    /varset foundCounters 0
    /dquery ${_victimName} -q "Debuff.Cursed" -o foundCounters
    /delay 10 ${DanNet.Q.Received}
    /if (${foundCounters}) {
        /call handleCureRequestV2 ${_victimName} ${_id} NOTSPECIFIED curse ${foundCounters} FALSE
    }
/return


| note: curestring looks like "Wanton Destruction"|curse|ALL
| note: str_cure_curse looks like "Remove Greater Curse"|45 <= this is due to not being able to programmatically retrieve the amount of counters cured
Sub handleCureRequestV2(string _name, int _id, string _debuffName, string _cureType, int _counters, bool _forced)
	/echo \ayhandleCureRequestV2: \ag${_name}\ao, \ag${_id}\ao, \ag${_debuffName}\ao, \ag${_cureType}, \ag${_counters}\ao, \ag${_forced}
	| Verify I can actually cure this
	/if (!${bln_hasCure_${_cureType}}) {
		/echo \ar Cannot cure ${_cureType} due to lack of available cure.
        /return
	}

    | Debug information
    |/echo _cureString                                == "[debuff name]"|curse|ALL =>${_cureString}          
    |/echo _cureString.Arg[1,|]                       == "[debuff name]"=>${_cureString.Arg[1,|]}
    |/echo _cureString.Arg[2,|]                       == "[disease||poison||curse"]=>${_cureString.Arg[2,|]} 
    |/echo {str_cure_{_cureString.Arg[2,|]}}          == "Remove Greater Curse|45"=> ${str_cure_${_cureString.Arg[2,|]}}
    |/echo {str_cure_{_cureString.Arg[2,|]}.Arg[2,|]} == 45 =>${str_cure_${_cureString.Arg[2,|]}.Arg[2,|]}   

    | this string will look like "Remove Greater Curse"
	/declare cureSpell string local ${str_cure_${_cureType}.Arg[1,|]}
    /echo cureSpell ${cureSpell}
    /if (${cureSpell.Equal[NULL]}) {
        /echo \arCannot cure ${_name} due to lack of ${_cureType} cure.
        /return
    }
	/if (!${SpawnCount[pc ${_name}]} || ${Spawn[pc ${_name}].Distance3D} > ${Spell[${cureSpell}].MyRange}) {
		/echo \arCannot cure ${_name} due to out of zone or out of range.
        /return
	}

	| Determine cast counts
	/declare countersToCure	int 	local ${_counters}
	/declare countersCured	int		local ${str_cure_${_cureType}.Arg[3,|]}
	
    /echo countersToCure ${countersToCure} countersCured ${countersCured}
    
    /declare castCount 		int 	local ${Math.Calc[${Math.Calc[${Math.Calc[${countersToCure}+${countersCured}]}-1]} / ${countersCured} ]}


    | Is the spell ready?
    /call chkSpellMemorized "${cureSpell}" TRUE ${int_defaultCureGem}
    | Cast the spell
    /call castCureV2 ${_name} ${_id} "${cureSpell}" ${castCount} ${_forced}
/return


Sub castCureV2(string _name, int _id, string _spell, int _castCount, bool _forced)
    /echo \ayCastCureV2: \ag${_name}\ao(\ag${_id}\ao) => \ag${_spell}\ao, \ag${_castCount}x\ao, \ag${_forced}
    /declare i int local 0
    /for i 1 to ${_castCount}
        /if (!${SpawnCount[pc ${_name}]}) /return
        /if (!${_forced} && (${int_healMode} > 0 && ${int_healMode} < 3) ) /call chkHeal_MT 
        /call TrueTarget ${_id}
        /call MQ2Cast "${_spell}" ${int_defaultCureGem} RunPriorityEvents
     /next i
/return 