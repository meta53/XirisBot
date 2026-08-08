# Future roadmap

This page preserves the recommended next steps if active XirisBot development resumes. The intent is incremental modernization: retain the encounter knowledge and working raid behavior while making changes safer to configure, test, and maintain.

## Guiding principles

- Do not rewrite the framework wholesale.
- Preserve working raid behavior before improving its structure.
- Prioritize correctness and observability before additional automation.
- Test shared-library changes on one healer, one caster, and one melee character before raid deployment.
- Update the relevant documentation whenever behavior, commands, configuration, dependencies, or architecture change.
- Keep emergency healing, curing, death detection, and raid reactions ahead of maintenance work.

## Recommended order

### 1. INI validation

Add a startup validator for character and shared configuration.

It should detect:

- Missing required sections and keys
- Numbered totals that do not match available entries
- Incorrect pipe-delimited field counts
- Empty spell, AA, discipline, item, or buff names
- Spells that are not known or configured in the expected gem
- AAs, abilities, and clickies unavailable to the character
- Invalid boolean, number, timer, class, resist, and mode values
- Duplicate entries where duplication is unsafe

Prefer warnings for optional capabilities and hard failures only where continuing would produce unsafe behavior.

Completion criteria:

- A malformed test INI produces specific section/key diagnostics.
- A valid INI starts without false-positive warnings.
- Every dynamic lineup reports its loaded entry count.
- Validation behavior is documented in [Configuration](configuration.md) and [Troubleshooting](troubleshooting.md).

### 2. Command registry and authorization

Centralize operational command metadata instead of manually duplicating command knowledge across `#EVENT` declarations, dispatchers, and documentation.

The registry should describe:

- Canonical command name and aliases
- Expected arguments
- Allowed delivery scopes
- Handler subroutine
- Event priority
- Applicable classes or roles
- Whether the command can move, target, trade, loot, or otherwise perform a dangerous action
- Optional sender authorization rules

This should reduce drift between registered patterns and [Commands](commands.md). Do not remove existing delivery forms until compatibility has been verified.

Completion criteria:

- A command can be found from one canonical inventory.
- Duplicate/missing handlers and registrations are detected automatically.
- Sensitive commands can reject unknown senders if authorization is enabled.
- Documentation generation or validation can compare the registry with `docs/commands.md`.

### 3. Explicit task scheduler

Replace organically ordered main-loop calls with a small scheduler that makes frequency and priority visible.

Suggested tiers:

| Tier | Examples | Typical frequency |
|---|---|---|
| Critical | Raid emotes, emergency heals, critical cures, death | Every available pass |
| Combat | Target validation, rotation decisions, aggro response | 100-200 ms as appropriate |
| Periodic | AA/clicky scans, buffs, inventory readiness | 250 ms to several seconds |
| Idle | Sitting, looting, rez sweeps, maintenance | Out of combat and timer-gated |

The scheduler must allow critical work during unavoidable cast, navigation, and readiness waits.

Completion criteria:

- Task frequencies are declared rather than scattered through class loops.
- One slow subsystem cannot stall critical tasks.
- Per-task call counts and elapsed-time diagnostics are available.
- All class macros preserve their documented priority order.

### 4. Separate decisions from actions

Begin with healing and targeting. Decision routines should inspect state and return a result; action routines should target, cast, move, or communicate.

Example direction:

```text
SelectHealTarget -> target ID and urgency
SelectHealSpell  -> spell and reason
ExecuteHeal      -> targeting, memorization, cast, result
```

Benefits include easier testing, fewer surprise target changes, and clearer logs.

Completion criteria:

- Selection routines do not issue movement, targeting, casting, or chat commands.
- Decision results include a machine-readable reason.
- Existing healing and targeting scenarios can be replayed against selection logic.

### 5. Standard return contracts

Replace ambiguous or stale `${Macro.Return}` dependencies with documented return values.

Define shared result constants for systems such as:

- Casting
- Target validation
- Healing selection
- Cure selection
- Navigation
- Looting

Callers should capture a result immediately after `/call` and avoid depending on it after unrelated calls.

Completion criteria:

- Core subroutines document their inputs and possible results.
- Callers store results locally before invoking another subroutine.
- Unknown result values are logged as errors in debug mode.

### 6. Structured diagnostics

Replace scattered unconditional `/echo` statements with consistent categories and levels.

Suggested categories:

- `COMBAT`
- `HEAL`
- `CURE`
- `CAST`
- `TARGET`
- `MOVE`
- `EVENT`
- `CONFIG`
- `LOOT`
- `PERF`

Suggested levels:

- Error
- Warning
- State transition
- Action
- Debug
- Trace

Keep important failures and state transitions visible by default. Debug and trace output should be opt-in per category.

Completion criteria:

- Hot loops do not emit repeated informational messages by default.
- A single INI section controls logging levels/categories.
- Important messages include character, subsystem, action, and reason consistently.

### 7. Remove embedded crew assumptions

Move hard-coded character names, cohorts, cleric orders, tank precedence, and encounter assignments into shared role configuration.

Possible role concepts:

- Primary/secondary/third tank
- Main assist
- Cure assignments
- Complete Heal chains
- Burn cohorts
- Raid group leaders
- Divine Intervention clerics
- Puller, looter, crowd control, and charm roles

Preserve existing character INIs as examples or migration fixtures.

Completion criteria:

- A new crew can be configured without editing `.mac` or `.inc` files.
- Missing role assignments produce useful warnings.
- Encounter handlers resolve roles instead of character literals where practical.

### 8. Lightweight test harness

Build tests around logic that can run without an EverQuest client.

Start with:

- INI parsing and schema validation
- Pipe-delimited lineup parsing
- Command inventory consistency
- Named-target classification
- Heal threshold and spell selection
- Cure selection
- Tank precedence
- Loot rule matching

MQ-dependent state can be represented as small fixtures. Avoid attempting to emulate the entire client.

Completion criteria:

- Tests run from one documented command.
- Representative valid and invalid character INIs are covered.
- Pure decision logic is exercised without launching EverQuest.
- Pull requests can report test results consistently.

### 9. Documentation as part of completion

For every major change, update the relevant pages in the same commit or pull request:

| Change | Documentation |
|---|---|
| New or changed command | `commands.md` and relevant hotkeys |
| New INI key or schema | `configuration.md` |
| Plugin or setup change | `installation.md` |
| Main-loop or library design | `architecture.md` |
| New failure/recovery behavior | `troubleshooting.md` |
| Changed priorities or future work | This roadmap |

A change is not complete if operators cannot discover how to configure or use it.

## Suggested first development session

If the project is resumed after a long break:

1. Confirm the supported EverQuest client, server, and MacroQuest build.
2. Inventory available plugin versions and renamed TLOs/commands.
3. Run one character from each major archetype and record startup errors.
4. Enable performance statistics and capture a short idle/combat baseline.
5. Fix compatibility failures without structural refactoring.
6. Implement the INI validator as the first modernization feature.
7. Add fixtures for the character INIs actually used in that session.

## Deferred ideas

These may be valuable, but should follow the work above:

- Generated character INI templates
- A command/help overlay in game
- Encounter enable/disable controls independent of zone detection
- Automated migration of older INI schemas
- Shared role profiles for multiple raid compositions
- More granular performance timing than call counters
- Replacement of selected `/goto` state machines where it materially improves safety

Avoid style-only rewrites of stable encounter code unless a testable behavior or maintenance benefit justifies the risk.
