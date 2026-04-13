# Architecture

## System Overview

EmberMUD follows a traditional MUD server architecture: a single-threaded, event-driven TCP server that manages multiple concurrent player connections via non-blocking I/O and `select()`. The server runs a continuous game loop that processes player input, executes autonomous world updates, and flushes output at a fixed tick rate.

## Architectural Layers

```
+------------------------------------------------------------------+
|                        Network Layer                              |
|  comm.c: main(), game_loop(), select(), socket I/O, nanny()      |
+------------------------------------------------------------------+
         |                              |
         v                              v
+-------------------+     +----------------------------+
| Command Dispatch  |     |   Autonomous Updates       |
| interp.c:        |     |   update.c:                |
|   interpret()     |     |     update_handler()       |
|   cmd_table[]     |     |     char_update()          |
|   check_social()  |     |     mobile_update()        |
+-------------------+     |     violence_update()      |
         |                |     weather_update()       |
         v                +----------------------------+
+------------------------------------------------------------------+
|                     Command Handlers                              |
|  act_comm.c  act_info.c  act_move.c  act_obj.c  act_wiz.c       |
|  fight.c     magic.c     skills.c    clan.c     auction.c  ...   |
+------------------------------------------------------------------+
         |
         v
+------------------------------------------------------------------+
|                    Core Game Logic                                |
|  handler.c: affects, movement, equipment, visibility             |
|  const.c: skill_table, class_table, race_table, attack_table     |
+------------------------------------------------------------------+
         |
         v
+------------------------------------------------------------------+
|                   Data / Persistence Layer                        |
|  db.c: boot_db(), area loading, mob/obj/room indexes             |
|  save.c: player file I/O                                         |
|  olc_save.c: area file serialization                             |
+------------------------------------------------------------------+
         |
         v
+------------------------------------------------------------------+
|                   Memory Management                               |
|  ssm.c: shared string heap     mem.c: OLC structure allocators   |
|  recycle.c: buffer/ban pools   db.c: alloc_mem/alloc_perm        |
+------------------------------------------------------------------+
```

## Main Game Loop

The game loop in `comm.c:game_loop()` runs at `PULSE_PER_SECOND` (typically 4 pulses per second). Each iteration:

1. **Accept new connections** -- `new_descriptor()` accepts incoming TCP connections
2. **Kick bad descriptors** -- Close any descriptors with errors
3. **Read input** -- `read_from_descriptor()` and `read_from_buffer()` extract one line of input per descriptor
4. **Process commands** -- For each descriptor with pending input:
   - If not yet playing: `nanny()` handles the login/creation state machine
   - If playing: `substitute_alias()` expands aliases, then `interpret()` dispatches the command
5. **Run autonomous updates** -- `update_handler()` checks pulse counters and dispatches:
   - `PULSE_VIOLENCE`: `violence_update()` -- combat round resolution
   - `PULSE_MOBILE`: `mobile_update()` -- NPC AI (wandering, scavenging, hate memory)
   - `PULSE_TICK`: `weather_update()`, `char_update()`, `obj_update()` -- regeneration, affect decay, object timers
   - `PULSE_AREA`: `area_update()` -- area repopulation
   - `PULSE_AUCTION`: `auction_update()` -- auction timer progression
   - Every pulse: `aggr_update()` -- aggressive mob checks
6. **Flush output** -- `process_output()` generates prompts and writes buffered output to sockets
7. **Sleep** -- Synchronize to the target pulse rate

## Module Dependency Graph

```
merc.h (master header - included by everything)
  |
  +-- config.h (compile-time settings)
  +-- board.h (board types)
  +-- factions.h (faction types)
  +-- newbits.h (extended bitfield types)

comm.c (entry point)
  +-- db.c (boot_db)
  +-- interp.c (interpret, substitute_alias)
  +-- update.c (update_handler)
  +-- handler.c (char_to_room, etc.)
  +-- save.c (save_char_obj, load_char_obj)
  +-- olc.c (run_olc_editor)

interp.c (command dispatch)
  +-- act_comm.c, act_info.c, act_move.c, act_obj.c, act_wiz.c
  +-- fight.c, magic.c, skills.c
  +-- clan.c, auction.c, bank.c, board.c, marry.c, remort.c
  +-- olc.c (editor entry points)
  +-- mprog_commands.c (mob program commands)

handler.c (core manipulation)
  +-- db.c (index lookups)
  +-- ssm.c (string management)
  +-- mud_progs.c (entry triggers)

db.c (world loading)
  +-- ssm.c (shared string manager)
  +-- mem.c (structure allocators)
  +-- handler.c (character/object manipulation)

fight.c <--> magic.c (bidirectional: spells cause damage, combat uses skills)
  +-- handler.c (affects, extraction)
  +-- skills.c (check_improve)
  +-- const.c (skill_table, attack_table, class_table)

mud_progs.c (scripting engine)
  +-- mprog_procs.c (conditional procedures)
  +-- mprog_commands.c (action commands)
  +-- interp.c (interpret - scripts execute normal MUD commands)
```

## Key Design Patterns

### Linked Lists

Nearly all game entities are stored in singly-linked lists. Global lists include:

- `char_list` / `player_list` -- all characters / all players
- `object_list` -- all instantiated objects
- `descriptor_list` -- all network connections
- `area_first` / `area_last` -- all loaded areas
- `ban_list` -- site bans

### Hash Tables

Prototype data (templates for creating instances) uses hash tables indexed by vnum:

- `mob_index_hash[MAX_KEY_HASH]` -- mob prototypes
- `obj_index_hash[MAX_KEY_HASH]` -- object prototypes
- `room_index_hash[MAX_KEY_HASH]` -- room data (rooms are not instanced)

### Free Lists

Memory allocation uses free-list recycling throughout. When a structure is "freed", it is pushed onto a type-specific free list rather than returned to the OS. The next allocation checks the free list before calling `alloc_perm()`:

- `char_free`, `obj_free`, `pcdata_free` -- character/object/pcdata recycling
- `affect_free`, `newaffect_free` -- affect recycling
- `descriptor_free`, `note_free`, `ban_free` -- descriptor/note/ban recycling
- `exit_free`, `room_index_free`, `extra_descr_free` -- OLC structure recycling

### Prototype/Instance Pattern

Mobs and objects follow a prototype/instance pattern:

- **Prototypes** (`MOB_INDEX_DATA`, `OBJ_INDEX_DATA`) are loaded once from area files and stored in hash tables
- **Instances** (`CHAR_DATA`, `OBJ_DATA`) are created from prototypes via `create_mobile()` / `create_object()`
- Instances can override prototype fields (strings, values) or share them via the shared string manager

### The `act()` Messaging System

`act()` and `act_new()` provide a printf-like system for sending formatted messages to characters in a room. Format tokens include:

- `$n` -- character name (as seen by the recipient)
- `$N` -- victim name
- `$e/$E` -- he/she/it pronouns
- `$m/$M` -- him/her/it pronouns
- `$s/$S` -- his/her/its pronouns
- `$p/$P` -- object short descriptions
- `$t/$T` -- text strings

Target types: `TO_CHAR`, `TO_VICT`, `TO_ROOM`, `TO_NOTVICT`.

### The Dual Affect System

EmberMUD has two parallel affect systems:

1. **Classic affects** (`AFFECT_DATA`) -- Traditional bitmask-based affects using `affected_by` (a `long` bitmask). Limited to ~32 affect types.
2. **New affects** (`NEWAFFECT_DATA`) -- Byte-array based affects using `newaff[]` via `newbits.c`. Allows unlimited affect types. Used for ghost states, blackjack, and other extensions.

Every affect function has a "new" counterpart (e.g., `affect_to_char` / `newaffect_to_char`).

### SuperMob Pattern

Objects and rooms cannot directly execute MUD commands (only characters can). The scripting system uses a "SuperMob" -- a hidden mob that is temporarily placed in the target room, given the script context, executes the commands, and is then released. See `set_supermob()` / `release_supermob()` in `mud_progs.c`.

## Threading Model

EmberMUD is entirely single-threaded. All I/O is non-blocking, multiplexed via `select()`. The only concurrency mechanism is the Unix `fork()` used for the in-game shell command, which creates child processes communicating via pipes.

## Platform Support

- **Primary**: Linux/Unix with gcc
- **Secondary**: Win32 via MSVC or Borland C++ Builder
- **Compatibility**: Cygwin, AIX, HP-UX, SunOS, Ultrix, NeXT, MIPS, Sequent

Platform differences are handled via `#ifdef` blocks throughout the codebase, primarily in `comm.c` (socket I/O), `merc.h` (type definitions), and `pty.c`/`route_io.c` (Unix-only features).
