# XirisBot

XirisBot is an INI-driven MacroQuest bot framework for EverQuest emulator raids. It coordinates class combat routines, healing, curing, buffing, movement, burns, pets, looting, and encounter-specific reactions across a multi-box crew.

The project is built for the RoF2-era MacroQuest environment used by this repository. It assumes a trusted private group or raid: many commands are intentionally accepted through tells, group chat, raid chat, and DanNet messages.

## Start here

1. Read [Installation](docs/installation.md) and load the required plugins.
2. Create or copy the character INI described in [Configuration](docs/configuration.md).
3. Start one character with `/mac xiris_bot` and resolve any initialization errors.
4. Start the crew and create the recommended [Hotkeys](docs/hotkeys.md).
5. Use the [Command reference](docs/commands.md) when building raid or character-specific controls.

## Minimal quick start

The loader accepts three tank names, the normal tank heal threshold, and optional auto-assist settings:

```text
/mac xiris_bot MainTank SecondTank ThirdTank 85 "FALSE,98"
```

If arguments are omitted, XirisBot reads its defaults from the active character INI:

```text
xiris_class_ini/BOT_<CLASS>_<Character>.ini
```

Example:

```text
xiris_class_ini/BOT_CLR_Xanshia.ini
```

To start every connected character in the current zone from a tank:

```text
/dgza /mac xiris_bot ${Me.Name} Xiria Xirea 85 "FALSE,98"
```

## Everyday controls

```text
/rs KillMob ${Target.ID} "${Target.CleanName}" ${Time.Time24}
/rs BackOff
/rs FollowMe /type|NAV
/rs NavStop
/rs doRaidBuffs ALL
/rs DebuffTarget ${Target.ID}
/rs BurnOnAll
```

These are message-driven commands. Sending the text through a supported channel causes each running macro that registered the event to handle it. See [Commands](docs/commands.md) for scopes, arguments, and targeted examples.

## Supported class macros

| Class | Macro | Major capabilities |
|---|---|---|
| Bard | `xiris_bot_brd.mac` | Medley, melee, nukes, DoTs, debuffs |
| Beastlord | `xiris_bot_bst.mac` | Pet, melee, spells, attack buffs |
| Cleric | `xiris_bot_clr.mac` | Healing, curing, buffs, rez, battle-cleric mode |
| Druid | `xiris_bot_dru.mac` | Healing, curing, buffs, nukes, DoTs |
| Enchanter | `xiris_bot_enc.mac` | Debuffs, buffs, charm, runes, pet |
| Magician | `xiris_bot_mag.mac` | Pet, buffs, nukes, AE nukes |
| Necromancer | `xiris_bot_nec.mac` | Pet, nukes, DoTs, feign death |
| Paladin | `xiris_bot_pal.mac` | Tanking, healing, stuns, offtanking |
| Ranger | `xiris_bot_rng.mac` | Melee, spells, self/group healing |
| Shadowknight | `xiris_bot_shd.mac` | Tanking, lifetaps, DoTs, feign death |
| Shaman | `xiris_bot_shm.mac` | Healing, curing, buffs, slows, DoTs |
| Warrior | `xiris_bot_war.mac` | Tanking, disciplines, offtanking |
| Wizard | `xiris_bot_wiz.mac` | Nukes, quick nukes, AE nukes, aggro reduction |
| Berserker, Monk, Rogue | `xiris_bot_melee.mac` | Shared endurance-melee routine |

`xiris_bot.mac` is only the loader. It selects `xiris_bot_<CLASS>.mac`, with BER/MNK/ROG routed to `xiris_bot_melee.mac`.

## Documentation

- [Installation and prerequisites](docs/installation.md)
- [Character configuration](docs/configuration.md)
- [Command reference](docs/commands.md)
- [Recommended hotkeys](docs/hotkeys.md)
- [Architecture and extension points](docs/architecture.md)
- [Troubleshooting](docs/troubleshooting.md)

## Repository layout

| Path | Purpose |
|---|---|
| `xiris_bot.mac` | Loader and class router |
| `xiris_bot_*.mac` | Class main loops and class-specific behavior |
| `xiris_common/*.inc` | Shared behavior libraries |
| `xiris_common/xiris_events_raid_*.inc` | Zone and encounter reactions |
| `xiris_class_ini/` | Per-character behavior configuration |
| `xiris_common/*.ini` | Shared definitions, loot rules, exclusions, and encounter data |
| `xiris_trade_ini/` | Trade-skill configuration |

## Safety notes

- Test new INI entries on one character before deploying them raid-wide.
- Keep a `BackOff` and `NavStop` hotkey available at all times.
- Verify the target before broadcasting `KillMob`, `DebuffTarget`, charm, or movement commands.
- Encounter includes can move characters or react to emotes automatically. Review the relevant raid include before a first attempt.
- Character INIs in this repository contain crew-specific names and spell lineups. Treat them as examples, not portable defaults.

## Contributing

Keep command names synchronized with their `#EVENT` definitions, document new INI keys, and include a short test description with behavior changes. Avoid committing account credentials, server login information, or private character data.

