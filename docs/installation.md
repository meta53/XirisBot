# Installation

## Requirements

XirisBot targets a RoF2 EverQuest client with a compatible MacroQuest/MQNext build. The exact plugin package depends on your distribution, but the framework uses these capabilities:

| Plugin | Used for |
|---|---|
| MQ2DanNet | Peer messaging and remote queries |
| MQ2Cast | Spell, AA, discipline, and item casting |
| MQ2Nav | Mesh-based following and navigation |
| MQ2MoveUtils | Stick-based movement and melee positioning |
| MQ2Exchange | Equipment and weapon-set changes |
| MQ2Debuffs | Debuff and cure-counter inspection |
| MQ2WorstHurt | Priority healing selection |
| MQ2Medley | Bard song rotation and queued casts |

Some classes or encounters use only a subset. Loading every plugin used by your raid is the simplest setup.

## Install the files

Place the repository contents in the MacroQuest macros directory so these paths exist together:

```text
Macros/xiris_bot.mac
Macros/bot_clr.mac
Macros/xiris_common/xiris_common.inc
Macros/xiris_class_ini/BOT_CLR_Character.ini
```

Do not flatten `xiris_common` or `xiris_class_ini`; includes and INI lookups use those relative paths.

## Load plugins

From the MacroQuest console, load the plugins available in your build:

```text
/plugin mq2dannet
/plugin mq2cast
/plugin mq2nav
/plugin mq2moveutils
/plugin mq2exchange
/plugin mq2debuffs
/plugin mq2worsthurt
```

Bards additionally need:

```text
/plugin mq2medley
```

Use `/plugin list` to verify the loaded names. Plugin spelling and availability vary slightly between MacroQuest distributions.

## MQ2Nav meshes

MQ2Nav requires a usable mesh for every zone in which navigation commands will run. Put meshes in the directory expected by your MQ2Nav build and test locally:

```text
/nav target
/nav stop
```

Never test raid-wide navigation until local pathing works reliably.

## Bard medleys

Define medleys in the character-specific MacroQuest INI. The medley names must match the values selected by the bard macro and character INI.

```ini
[MQ2Medley]
Delay=3
Quiet=1
Debug=0

[MQ2Medley-tank]
song1=Niv's Harmonic^18^1
song2=Guardian Rhythms^18^1
song3=Verse of Vesagran^18^1
song4=War March of Muram^18^1
song5=Psalm of Mystic Shielding^18^1
```

## First-character test

1. Copy the closest character INI and rename it correctly.
2. Confirm configured spells, AAs, disciplines, and items exist on that character.
3. Start without auto-assist:

   ```text
   /mac xiris_bot MainTank SecondTank ThirdTank 85 "FALSE,98"
   ```

4. Watch the MQ console for missing plugin, undefined variable, spell, or INI errors.
5. Test `FollowMe`, `NavStop`, `KillMob`, and `BackOff` before starting the entire crew.

## DanNet setup

XirisBot runs `/dnet fullnames off` during initialization so event senders use short character names. Confirm peers are visible with your build's DanNet information command before using `/dgza`, `/dgt`, or `/dt` hotkeys.

## Updating

Before pulling or replacing files:

- Commit or back up character INIs.
- Review changes to shared includes and event patterns.
- Test one healer, one caster, and one melee character before raid-wide deployment.
