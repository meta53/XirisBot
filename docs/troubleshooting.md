# Troubleshooting

## Macro does not select the expected class file

Check `${Me.Class.ShortName}` and confirm the corresponding `xiris_bot_<class>.mac` exists. Berserkers, monks, and rogues intentionally load `xiris_bot_melee.mac`.

## Character INI is not found

Verify the exact path and naming convention:

```text
xiris_class_ini/BOT_<CLASS>_<Character>.ini
```

Use the clean character name, not a server-qualified DanNet name.

## Undefined variable during initialization

Typical causes:

- A numbered total is larger than the entries present.
- A copied record has the wrong number of pipe-delimited fields.
- The INI belongs to a different code version.
- A required spell/item/AA variable was left empty.

Compare the section with a known-good INI for the same class and the template in the owning include.

## Commands appear in chat but bots do nothing

1. Confirm the macro is running on the recipient.
2. Confirm the command spelling and argument order in [Commands](commands.md).
3. Verify `/dnet fullnames off` was applied during initialization.
4. Try a direct `/dt Character ...` message.
5. Check whether the event set is registered for that class or zone.
6. Check whether the handler immediately rejects the character because of class, mode, range, or readiness.

## Auto-assist does not engage

- Confirm `AutoAssist` is enabled in the loader arguments or command state.
- Confirm the primary tank name is correct and is a visible DanNet peer.
- Confirm the tank has a valid NPC target.
- Check the assist HP threshold.
- Use an explicit `KillMob` command to separate assist-query problems from combat problems.

## Navigation does not start or characters get stuck

- Verify MQ2Nav is loaded.
- Test `/nav target` locally.
- Confirm the zone mesh exists and covers both endpoints.
- Use `NavStop`, reposition manually, and retry.
- Use `FollowMe /type|STICK` as a fallback where appropriate.

## Healer reacts but chooses no spell

Check:

- Heal type and heal mode
- Tank/self/group thresholds
- Current spell set and memorized gems
- Mana requirements
- Target range
- Configured spell names for the server era
- Stop-cast and HoT timers

Use `[PERFORMANCE] VerboseCombat=TRUE` only when its additional output is useful; it does not trace every healing decision.

## Cure requests are ignored

- Confirm MQ2Debuffs is loaded.
- Confirm cure use is enabled for the character.
- Verify cure type and counter threshold.
- Confirm the assigned curer is online, in range, and has the configured cure memorized or available.
- Reset cure coordination with `ResetGroupCurer` if state appears stale.

## Clickies or AAs never fire

- Confirm the lineup total and numbered entries.
- Confirm the configured item name exactly matches the inventory item.
- Check `Use`, type, named-only, HP window, and trigger fields.
- Clicky IDs refresh periodically, but restart the macro after major equipment or configuration changes.

## Excessive CPU or command lag

Temporarily enable:

```ini
[PERFORMANCE]
Stats=TRUE
VerboseCombat=FALSE
```

Compare the reported combat loops and lineup scans across classes. Also check for:

- Missing timers around custom loops
- Repeated navigation failures
- Chat/log spam
- Huge AA/clicky/debuff lineups
- Encounter handlers active in the wrong zone
- Multiple macros controlling the same character

## Safe recovery sequence

From the controller:

```text
/rs BackOff
/rs NavStop
```

Then stop or restart only the affected character's macro. Avoid restarting the full raid until the local failure is understood.
