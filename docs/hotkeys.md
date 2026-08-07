# Recommended hotkeys

These examples are designed for the primary tank or raid controller. Replace character names, thresholds, waypoints, and DanNet scopes to match your crew.

## Page 1: core control

### Start bots

```text
Line 1: /dgza /mac xiris_bot ${Me.Name} Xiria Xirea 85 "FALSE,98"
```

### Engage

```text
Line 1: /multiline ; /rs KillMob ${Target.ID} "${Target.CleanName}" ${Time.Time24} ; /attack on
```

### Emergency back off

```text
Line 1: /rs BackOff
Line 2: /rs NavStop
Line 3: /attack off
```

### Follow with Nav

```text
Line 1: /rs FollowMe /type|NAV
```

### Follow with stick fallback

```text
Line 1: /rs FollowMe /type|STICK
```

### Stop movement

```text
Line 1: /rs NavStop
```

## Page 2: targets and positioning

### Change main tank to me

```text
Line 1: /dgt ChangeMT ${Me.CleanName}
```

### Change main tank to target

Use only while targeting the intended player:

```text
Line 1: /dgt ChangeMT ${Target.CleanName}
```

### Navigate to waypoint

```text
Line 1: /rs NavToWP raidcamp
```

### Force debuffs

```text
Line 1: /rs DebuffTarget ${Target.ID}
```

### Force slow

```text
Line 1: /rs SlowTarget ${Target.ID}
```

## Page 3: burns and recovery

### Full burn

```text
Line 1: /rs BurnOnAll
```

### Stop burn mode

```text
Line 1: /rs BurnOff
```

### Cohort burns

```text
Line 1: /rs BurnOn1
```

Create a separate button for `BurnOn2` if cohorts alternate.

### Veteran recovery

```text
Line 1: /dgt DoStaunchRecovery
```

## Page 4: healing and curing

### Efficient healing

```text
Line 1: /dgt HealMode EFFICIENT
```

### Default healing

```text
Line 1: /dgt HealMode DEFAULT
```

### Raise tank heal point

```text
Line 1: /dgt changeHPTank 90
```

### Restore tank heal point

```text
Line 1: /dgt changeHPTank 80
```

### Cure raid groups

```text
Line 1: /dgt cureGroupPoison 45
Line 2: /dgt cureGroupDisease 45
Line 3: /dgt cureGroupCurse 45
```

### Force group heal

```text
Line 1: /dgt DoGroupHeal
```

## Page 5: preparation and cleanup

### Raid buffs

```text
Line 1: /rs doRaidBuffs ALL
```

### Disable damage shields

```text
Line 1: /dgt NoDS
```

### Enable damage shields

```text
Line 1: /dgt YesDS
```

### Enable looter

```text
Line 1: /dt Looter looton ALL
```

### Quest loot only

```text
Line 1: /dt Looter looton QUEST
```

### Disable looter

```text
Line 1: /dt Looter lootoff
```

## Targeted-role examples

### Tank offtank toggle

```text
Line 1: /dt Xiria OfftankOn
Line 2: /dt Xiria OfftankRadius 75
```

Create a paired off button:

```text
Line 1: /dt Xiria OfftankOff
```

### Cleric watches a different tank

```text
Line 1: /dt Xanshia ChangeMT Xiria
Line 2: /dt Xanshia AutoCureMTOn
Line 3: /dt Xanshia HealType 1
```

### Fast-nuke mode

```text
Line 1: /dgt UseFastOnly ON
```

Paired off button:

```text
Line 1: /dgt UseFastOnly OFF
```

## Hotkey design guidance

- Put `BackOff` and `NavStop` on an easy-to-reach page shared by every tank UI.
- Use distinct colors for engage, movement, burn, and emergency controls.
- Prefer spawn IDs over names for NPC combat commands.
- Use `/dt` for dangerous role-specific commands and broader scopes only when intentional.
- Do not combine `KillMob` with movement or burn commands until each works independently.
- Keep paired ON/OFF buttons adjacent.
