# EmberMUD 0.9.47 Source Code Documentation

EmberMUD is a Multi-User Dungeon (MUD) server written in C, descended from the Diku -> Merc -> ROM lineage. It is an interactive multiplayer text-based fantasy RPG server featuring real-time combat, spell casting, online world building, clan warfare, scripting, and much more.

## Heritage

- **DikuMUD** (1990-1991) -- Sebastian Hammer, Michael Seifert, Hans Henrik Staerfeldt, Tom Madsen, Katja Nyboe
- **Merc DikuMUD** (1992-1993) -- Michael Chastain, Michael Quan, Mitchell Tse
- **ROM** (Rivers of MUD) -- Russ Taylor
- **EmberMUD** -- Lancelight, Kyle Boyd, Zane, and other contributors

## Documentation Index

| Document | Description |
|---|---|
| [Architecture](architecture.md) | System architecture, module relationships, and data flow |
| [Building](building.md) | Compilation, Makefile, and platform support |
| [Configuration](configuration.md) | Compile-time (`config.h`) and runtime (`ember.cfg`) configuration |
| [Data Structures](data-structures.md) | Core data types defined in `merc.h` |
| [Networking](networking.md) | Network I/O, game loop, descriptors, and the login state machine |
| [Database](database.md) | World loading, area file format, persistence, and resets |
| [Commands](commands.md) | Command interpreter, command table, and player commands |
| [Combat and Magic](combat.md) | Combat engine, spell system, skills, and XP |
| [Online Creation (OLC)](olc.md) | In-game world building editors |
| [MUD Programs](mob-progs.md) | Scripting system for mobs, objects, and rooms |
| [Game Systems](game-systems.md) | Clans, factions, auctions, banking, marriage, boards, and more |
| [Memory Management](memory.md) | Shared string manager (SSM), free-list allocators, and buffers |
| [API Reference](api-reference.md) | Function reference organized by source file |

## Source File Overview

### Core Engine

| File | Purpose |
|---|---|
| `merc.h` | Master header -- all type definitions, macros, globals, and prototypes |
| `config.h` | Compile-time configuration constants |
| `comm.c` | Entry point (`main`), game loop, network I/O, login state machine, ANSI color |
| `db.c` | Boot-time world loading, memory allocation primitives, string/number utilities |
| `db.h` | Database header -- hash table externs and counters |
| `handler.c` | Character/object manipulation: affects, movement, equipment, visibility |
| `interp.c` | Command interpreter and master command table |
| `interp.h` | Command table structure and `do_*` function declarations |
| `update.c` | Autonomous game updates: ticks, regeneration, weather, mob AI, object decay |
| `save.c` | Player character save/load (pfile I/O) |

### Player Commands

| File | Purpose |
|---|---|
| `act_comm.c` | Communication: channels, tells, group talk, quit, follow, order |
| `act_info.c` | Information: look, score, who, equipment, practice, help, aliases |
| `act_move.c` | Movement: directions, doors, sneak, hide, recall, train, track |
| `act_obj.c` | Object handling: get, drop, wear, eat, drink, buy, sell, steal, brew, scribe |
| `act_wiz.c` | Immortal commands: goto, stat, load, purge, set, restore, shutdown |

### Combat and Magic

| File | Purpose |
|---|---|
| `fight.c` | Combat engine: violence loop, hit resolution, damage, death, XP, combat skills |
| `magic.c` | Spell system: casting, 100+ spell functions, magic items |
| `magic.h` | Magic system constants and spell function declarations |
| `skills.c` | Skill/group management, XP calculation, character generation, improvement |
| `const.c` | Static game data: attack types, races, classes, stat tables, skill table, liquids |

### Online Creation (OLC)

| File | Purpose |
|---|---|
| `olc.c` | OLC dispatcher and editor entry points |
| `olc.h` | OLC types, macros, editor constants, and function prototypes |
| `olc_act.c` | All editor sub-commands (area, room, object, mob, mudprog editing) |
| `olc_save.c` | Serialization of areas, mobs, objects, rooms to area files |
| `socialolc.c` | Social emote editor |
| `helpolc.c` | Help file editor |
| `todoolc.c` | Developer todo list editor |

### MUD Programs (Scripting)

| File | Purpose |
|---|---|
| `mud_progs.c` | Script engine: parser, variable expansion, trigger dispatch |
| `mud_progs.h` | Script command table and procedure declarations |
| `mprog_commands.c` | In-prog action commands (mob teleport, force, load, echo) |
| `mprog_procs.c` | Conditional query procedures for script `if` expressions |

### Game Systems

| File | Purpose |
|---|---|
| `clan.c` | Clan/guild system with OLC editor |
| `factions.c` | Faction reputation system with OLC editor |
| `factions.h` | Faction type definitions and prototypes |
| `auction.c` | Timed item auction system |
| `bank.c` | Location-based gold banking |
| `board.c` | Multi-board persistent message/note system |
| `board.h` | Board type definitions and constants |
| `marry.c` | Immortal-administered marriage system |
| `remort.c` | Endgame character remort/rebirth |
| `class.c` | Online class-skill level assignment editing |
| `random.c` | Procedural random object generation for NPCs |

### Utilities

| File | Purpose |
|---|---|
| `ssm.c` | Shared String Manager -- reference-counted string heap |
| `mem.c` | Free-list allocators for OLC data structures |
| `recycle.c` | Recycling allocators for bans and dynamic buffers |
| `string.c` | In-game text editor for OLC string editing |
| `bit.c` | Flag name/value tables and conversion functions |
| `ban.c` | Site banning with prefix/suffix wildcards |
| `newbits.c` | Extended byte-array bitfield operations |
| `newbits.h` | Newbits constants and prototypes |
| `drunk2.c` | Drunk speech garbling |
| `pty.c` | Unix pseudo-terminal allocation |
| `route_io.c` | Telnet I/O routing for in-game shell access |

## Directory Structure

```
embermud/
  src/           Core C source code (this documentation covers these files)
  area/          Game world area data files (.are)
  clan/          Clan data and log files
  classes/       Class skill-level assignment files
  doc/           Legacy documentation (area format PDF, MOBProg docs, etc.)
  gods/          Immortal character data
  log/           Server log files
  notes/         Board message storage
  player/        Player character save files (pfiles)
  Changelog      Project change history
  INSTALL        Installation instructions
  README-0.9.47  Release notes
```
