# Configuration

## Character INI naming

The active INI is selected from the character's class short name and clean name:

```text
xiris_class_ini/BOT_<CLASS>_<Character>.ini
```

Examples:

```text
xiris_class_ini/BOT_WAR_Xiris.ini
xiris_class_ini/BOT_CLR_Xanshia.ini
xiris_class_ini/BOT_WIZ_Xiexi.ini
```

Names and case should match `${Me.Class.ShortName}` and `${Me.CleanName}`.

## Loader arguments

```text
/mac xiris_bot <tank1> <tank2> <tank3> <tank-heal-point> "<auto-assist>,<assist-at>"
```

| Argument | Meaning | Example |
|---|---|---|
| `tank1` | Primary tank watched by the bot | `Xiris` |
| `tank2` | Secondary replacement tank | `Xiria` |
| `tank3` | Third replacement tank | `Xirea` |
| `tank-heal-point` | Normal tank heal threshold | `85` |
| `auto-assist` | Whether the bot queries the tank target automatically | `FALSE` |
| `assist-at` | Target HP threshold used by assist behavior | `98` |

Missing values fall back to INI defaults. Quote the comma-separated assist argument.

## Common configuration areas

Character files differ by class, but commonly contain these behavior families:

| Area | Controls |
|---|---|
| `DEFAULT_SETTINGS` | Tank defaults, assist point, spell set, sitting and utility behavior |
| `Heal_Points` | Tank, self, group, frantic, HoT, and stop-cast thresholds |
| `NUKE`, `QNUKE`, `AENUKE` | Spell order, HP windows, mana floors, named-only behavior |
| `DOT` | DoT order, recast timing, HP windows, resist rules |
| `DEBUFF` | Debuff order, forced/named behavior, HP windows |
| `AA` | Offensive and defensive AA lineups and triggers |
| `BURN` | Burn sequence and cohort behavior |
| `CLICKY` | Item click order, type, trigger, and named restrictions |
| Buff sections | Out-of-combat, combat, single-target, and buff-line definitions |
| Pet sections | Pet summoning, buffs, weapons, and attack behavior |
| Cure sections | Cure spells, thresholds, assignments, and auto-cure behavior |

Use an existing INI for the same class as the schema reference. The macro frequently builds dynamic variables from numbered entries; totals must agree with the entries present.

## Numbered lineup pattern

Many sections use a total followed by numbered pipe-delimited records:

```ini
[AA]
AA_Total=2
AA_1=Example Offensive AA|OFFENSE|TRUE|FALSE|98|1|FALSE
AA_2=Example Defensive AA|DEFENSE|TRUE|FALSE|100|1|FALSE
```

Field order is defined by the corresponding template in the shared include. For example, AA, burn, and clicky templates live in `xiris_common/xiris_burn.inc`. Copying a known-good entry is safer than guessing field order.

## Spell sets

Several routines temporarily load named spell sets. At minimum, classes that buff should normally have:

- `default`: normal combat spell arrangement
- `buff`: raid-buff spell arrangement

Specialized class or event routines may require additional sets. Search the class macro and character INI for `memspellset` before deploying a copied configuration.

## Shared INIs

| File | Purpose |
|---|---|
| `xiris_common/xiris_common.ini` | Shared raid and common definitions |
| `xiris_common/xiris_healing.ini` | Shared healing definitions |
| `xiris_common/xiris_loot_list.ini` | Loot policy |
| `xiris_common/xiris_exclude.ini` | Excluded targets |
| `xiris_common/xiris_casting_resists.ini` | Resist-related casting definitions |
| `xiris_common/xiris_damageshields.ini` | Damage shield definitions |
| `xiris_common/xiris_pull_zoneinfo.ini` | Puller zone data |

## Loot entries

Loot rules are pipe-delimited:

```ini
Loot1=Minor Muramite Rune|QUEST|*|*|!|Aergia|F
```

Fields:

1. Item name
2. Type, such as `VENDOR`, `TRADE`, or `QUEST`
3. Desired count, or `*`
4. Allowed class short names, or `*`
5. Excluded character, or `!`
6. Required character, or `*`
7. Beep flag, `T` or `F`

Test loot changes on one designated looter. Incorrect loot rules can leave items behind or assign them incorrectly.

## Performance diagnostics

The common initializer recognizes an optional section:

```ini
[PERFORMANCE]
Debug=FALSE
VerboseCombat=FALSE
Stats=FALSE
```

Set `Stats=TRUE` temporarily to print 60-second combat-loop and scan counters. `VerboseCombat=TRUE` enables detailed burn diagnostics and should normally remain off on large crews.

## Safe editing workflow

1. Change one character INI.
2. Restart that character's macro.
3. Test idle, engage, disengage, death, zone, and recovery behavior.
4. Copy the verified pattern to comparable characters.
5. Commit the configuration change with the code version it requires.
