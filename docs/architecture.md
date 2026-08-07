# Architecture

## Startup flow

```text
xiris_bot.mac
  -> resolves default tanks and heal point
  -> selects bot_<CLASS>.mac
  -> routes BER/MNK/ROG to bot_melee.mac
  -> class macro calls xbot_initialize
  -> shared libraries load INI-driven lineups and events
  -> class macro enters MainLoop
```

## Main-loop priorities

Class loops generally process:

1. Critical and registered events
2. Tank validity
3. Curing and healing, where applicable
4. Auto-assist and engagement state
5. Class combat actions
6. AA, burn, and clicky lineups
7. Target/death validation
8. Idle utilities, sitting, looting, and downshifts

The exact order differs by class. Healing classes deliberately check health before DPS.

## Shared libraries

| Include | Responsibility |
|---|---|
| `xiris_common.inc` | Initialization, targeting, engagement, common state, utilities |
| `xiris_events.inc` | Registration and dispatch of event sets |
| `xiris_spell_routines.inc` | Common casting execution and result handling |
| `xiris_casting.inc` | Nukes, quick nukes, AE nukes, DoTs, stuns, resist filters |
| `xiris_healing.inc` | Heal selection, group health, rez, CH chains |
| `xiris_curing.inc` | Self/group/tank cure decisions and coordination |
| `xiris_buffing.inc` | Raid, character, single-target, and combat buffs |
| `xiris_buffing_lines.inc` | Reusable buff-line definitions |
| `xiris_debuffing.inc` | Debuff and slow lineups |
| `xiris_burn.inc` | Burn, AA, and clicky lineups |
| `xiris_melee.inc` | Melee abilities, stick, positioning, melee events |
| `xiris_movement.inc` | Follow, navigation, waypoints, and camps |
| `xiris_offtank.inc` | Offtank selection and control |
| `xiris_pets.inc` | Pet creation, equipment, buffs, and attacks |
| `xiris_looting.inc` | Loot selection, corpse handling, selling, and hand-ins |
| `xiris_charm.inc` | Charm selection and control |
| `xiris_exclude.inc` | Target exclusions and alerts |

## Event system

Events are declared with `#EVENT`, grouped into dispatcher subroutines, and registered during initialization as background, raid, class, or rapid event sets.

When adding a command:

1. Add all required chat/DanNet patterns.
2. Add an `EVENT_<name>` handler.
3. Add `/doevents <name>` to the correct event-set subroutine.
4. Register the event set with an accurate count.
5. Document the command and its arguments.
6. Test tell, raid/group chat, and DanNet delivery forms that the pattern claims to support.

Rapid events are used where reaction time matters during casting or other waits. Avoid moving ordinary maintenance work into rapid dispatch.

## Raid events

`xiris_events.inc` includes encounter libraries for Gates of Discord, Omens of War, Epic encounters, and Depths of Darkhollow. Initialization selects relevant sets from zone short names.

Raid handlers may:

- Duck or interrupt casts
- Move with MQ2Nav
- Change targets
- Change healing or burn modes
- React to boss emotes
- Coordinate group assignments

Because these reactions are encounter-sensitive, test changes in the target zone and preserve the original emote text.

## INI-driven lineups

Several systems construct dynamic variables from numbered INI records. A typical flow is:

```text
read <TYPE>_Total
for each numbered entry
  split pipe-delimited properties
  declare <TYPE>_<index> fields
main loop scans the resulting lineup
```

Changing a template field order requires coordinated updates to every affected character INI.

## Adding a class behavior

Prefer extending a shared library when behavior is common to multiple classes. Keep class macros focused on ordering and class-only events.

For a new class-specific action:

1. Add configuration to the class INI.
2. Initialize variables in the class macro or relevant shared library.
3. Add a guarded subroutine with cheap early returns.
4. Place its call according to gameplay priority.
5. Avoid blocking `/delay` calls in the main loop.
6. Pump rapid/raid events during unavoidable waits.

## Performance conventions

- Cache values that remain stable for the current target.
- Timer-gate full AA, item, inventory, radius, and group scans.
- Keep death, emergency healing, curing, and rapid events responsive.
- Avoid synchronous DanNet waits in the main loop.
- Keep verbose logging disabled during normal raid operation.
- Use `[PERFORMANCE] Stats=TRUE` temporarily when measuring scan frequency.
