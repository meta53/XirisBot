# Command reference

XirisBot commands are chat/event messages, not conventional slash-command binds. Send the command text through a channel recognized by its `#EVENT` patterns.

Common delivery forms used by this repository:

```text
/rs <command>             raid chat
/gs <command>             group chat
/dt Character <command>   one DanNet peer
/dgt <command>            configured DanNet group
/dgza <command>           connected peers in the current zone
```

DanNet scope aliases can vary by build and local configuration. Verify them before issuing movement or combat commands raid-wide.

## Combat control

| Command text | Purpose | Example |
|---|---|---|
| `KillMob <id> <name> <time>` | Engage an NPC by spawn ID | `/rs KillMob ${Target.ID} "${Target.CleanName}" ${Time.Time24}` |
| `BackOff` | Disengage, stop attacks/casts, and reset combat state | `/rs BackOff` |
| `AutoAssist <value>` | Toggle or modify auto-assist behavior | `/dgt AutoAssist TRUE` |
| `ChangeMT <name>` | Change the watched primary tank | `/dgt ChangeMT Xiris` |
| `ChangeAP <percent>` | Change assist threshold | `/dgt ChangeAP 98` |
| `MeleeOverride TRUE|FALSE` | Force melee behavior for one character | `/dt Xanshia MeleeOverride FALSE` |
| `ForceNamed TRUE|FALSE` | Override named-target classification | `/dgt ForceNamed TRUE` |
| `RiposteDisc` | Request configured riposte discipline | `/dgt RiposteDisc` |
| `UpdateStick <args>` | Change melee stick command | `/dt Ophidia UpdateStick behind 10` |

Always validate `${Target.ID}` before broadcasting `KillMob`.

## Burns and veteran abilities

| Command text | Purpose |
|---|---|
| `BurnOnAll` | Enable the full configured burn lineup |
| `BurnOn1` | Enable burn cohort 1 |
| `BurnOn2` | Enable burn cohort 2 |
| `BurnOff` | Disable burn mode |
| `DoStaunchRecovery` | Request Staunch Recovery |
| `DoIntensity` | Request Intensity of the Resolute |
| `DoServants` | Request Steadfast Servant |

Examples:

```text
/rs BurnOnAll
/rs BurnOff
/dgt DoStaunchRecovery
```

`ShortBurn1`, `ShortBurn2`, `LongBurn1`, `LongBurn2`, and `MaximumBurn` are declared by the burn library but should be verified against their handlers and current character lineups before operational use.

## Movement

| Command text | Purpose | Example |
|---|---|---|
| `FollowMe /type|NAV` | Follow sender using MQ2Nav | `/rs FollowMe /type|NAV` |
| `FollowMe /type|STICK` | Follow sender using stick | `/rs FollowMe /type|STICK` |
| `NavStop` | Stop navigation/following | `/rs NavStop` |
| `MoveTo <arguments>` | Move to supplied location/target data | Encounter-specific |
| `NavToWP <waypoint>` | Navigate to recorded waypoint | `/rs NavToWP westwall` |
| `CreateCamp <name>` | Create/set camp through the movement library | `/rs CreateCamp raidcamp` |

Record and test waypoints locally before broadcasting `NavToWP`.

## Buffing and damage shields

| Command text | Purpose |
|---|---|
| `doRaidBuffs ALL` | Run configured raid-wide buffing |
| `doSingleTargetBuffs <name>` | Apply configured single-target buffs |
| `doCharBuffs <name>` | Buff a specified character |
| `NoDS` | Disable/use removal behavior for damage shields |
| `YesDS` | Enable damage shields |
| `RemoveBuff <name>` | Remove a named buff |
| `RemoveAllBuffs` | Remove all removable buffs handled by the routine |

Examples:

```text
/rs doRaidBuffs ALL
/dgt doCharBuffs Xiris
/dgt NoDS
/dgt RemoveBuff Circle of Fireskin
```

## Casting and debuffing

| Command text | Purpose |
|---|---|
| `DebuffTarget <id>` | Force configured debuffers to process a target |
| `SlowTarget <id>` | Force a slow attempt |
| `ResistTypes <list>` | Limit enabled resist types |
| `UseFastOnly ON|OFF` | Toggle quick-nuke-only behavior where configured |

Examples:

```text
/rs DebuffTarget ${Target.ID}
/rs SlowTarget ${Target.ID}
/dgt ResistTypes fire|magic
/dgt UseFastOnly ON
```

## Healing

| Command text | Purpose |
|---|---|
| `DoGroupHeal` | Request configured group-heal actions |
| `InterruptON` / `InterruptOFF` | Enable or disable heal interruption |
| `ChangeHP <percent>` | Change general heal threshold |
| `changeHPTank <percent>` | Change tank threshold |
| `changeHPSelf <percent>` | Change self threshold |
| `changeHPGroup <percent>` | Change group threshold |
| `HealMode DEFAULT|EFFICIENT` | Select healing spell strategy |
| `HealType 0|1|2|3` | Select healing coverage |
| `FocusHealMTOn` / `FocusHealMTOff` | Toggle focus on self and watched tank |
| `FireTotem <group>` | Fire configured priest totem group |
| `FireRegen <group>` | Fire configured regeneration group |

Heal types:

| Value | Coverage |
|---|---|
| `0` | Self only |
| `1` | Watched tank and self |
| `2` | Group, watched tank, and self |
| `3` | Group only, including self |

Examples:

```text
/dgt HealMode EFFICIENT
/dt Xanshia HealType 1
/dgt changeHPTank 80
/dt Xanshia FocusHealMTOn
```

## Complete Heal chains

```text
/dgt CHStart <chain-index> <target> <backup> <delay> <cleric1,cleric2,...>
/dgt CHStop <chain-index>
/dgt CHPause <chain-index>
/dgt CHResume
/dgt CHSwitch <chain-index> <new-target>
```

Example:

```text
/dgt CHStart 1 Xiris Xiria 30 Clericone,Clerictwo,Clericthree,Clericfour
```

The delay units and cleric order are interpreted by the existing healing routine. Test a new chain outside combat.

## Curing

| Command text | Purpose |
|---|---|
| `cureGroupPoison <counters>` | Inspect and cure group poison counters |
| `cureGroupDisease <counters>` | Inspect and cure group disease counters |
| `cureGroupCurse <counters>` | Inspect and cure group curse counters |
| `cureMe <type>` | Ask a specific curer to cure the sender |
| `AutoCureMTOn` / `AutoCureMTOff` | Toggle proactive watched-tank curing |
| `ResetGroupCurer` | Reset cure assignment state |
| `fireNadox` | Request configured Nadox action |

Examples:

```text
/dgt cureGroupPoison 45
/dgt cureGroupDisease 45
/dgt cureGroupCurse 45
/dt Xanshia AutoCureMTOn
/dt Xanshia cureMe curse
```

## Looting

| Command text | Purpose |
|---|---|
| `looton ALL` | Enable all configured loot types |
| `looton QUEST` | Enable configured quest loot only |
| `lootoff` | Disable automated looting |
| `lootrefresh` | Reload/refresh loot state |
| `DoHideCorpses` | Hide corpses according to routine |
| `DoHideLooted` | Hide looted corpses |
| `LootYourself` | Attempt self-corpse looting |
| `selltovendor` | Run configured vendor selling |

Examples:

```text
/dt Looter looton ALL
/dt Looter looton QUEST
/dt Looter lootoff
/dgt DoHideLooted
```

## Offtanking

```text
/dt TankName OfftankOn
/dt TankName OfftankOff
/dt TankName OfftankRadius 75
/dt TankName DIOn
/dt TankName DIOff
```

Offtanking is primarily intended for plate tanks. Monitor the first pulls after changing its radius.

## Charm

```text
/dt Enchanter CharmON
/dt Enchanter CharmNPCByID ${Target.ID}
/dt Enchanter CharmNPCByName "an npc name"
/dt Enchanter CharmOFF
```

Charm support is specialized and should be tested for the exact encounter.

## Raid setup and utilities

```text
/dgt SetupRaid DEFAULT
/dgt RefreshXTarget
/dgt CheckNaked
/dgt fixCorpses
```

Raid-specific includes register many additional encounter commands. Those are implementation controls, not general-purpose commands; review the corresponding `xiris_events_raid_*.inc` before using them.
