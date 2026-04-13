# MUD Programs (Scripting)

The MudProg system provides a custom scripting language that allows mobs, objects, and rooms to execute programmatic behavior in response to game events. The implementation is spread across `mud_progs.c` (engine), `mud_progs.h` (interface), `mprog_procs.c` (conditional procedures), and `mprog_commands.c` (action commands).

This is a complete rewrite by Zane of the original MobProgs by N'Atas-ha, supporting mob programs, object programs, and room programs.

## Trigger Types

Scripts are activated by trigger events. Each trigger type sets up execution context variables and calls the script driver.

### Mob Triggers

| Trigger | Fired When | Argument |
|---|---|---|
| `act` | A matching `act()` message is sent in the room | Pattern to match |
| `speech` | A player says something matching a pattern | Pattern to match |
| `command` | A player types a command matching a pattern | Pattern to match |
| `random` | Random chance each mob tick | Percentage (1-100) |
| `greet` | A player enters the room | Percentage chance |
| `entry` | The mob enters a new room | Percentage chance |
| `fight` | Each combat round while fighting | Percentage chance |
| `fightgroup` | Each combat round (group context) | Percentage chance |
| `hitprcnt` | Mob's HP drops below a percentage | HP percentage threshold |
| `death` | The mob is killed | Percentage chance |
| `bribe` | A player gives gold to the mob | Minimum gold amount |
| `give` | A player gives an object to the mob | Object keyword |

### Room Triggers

| Trigger | Fired When | Argument |
|---|---|---|
| `enter` | A character enters the room | Percentage chance |
| `leave` | A character leaves the room | Percentage chance |
| `sleep` | A character sleeps in the room | Percentage chance |
| `rest` | A character rests in the room | Percentage chance |
| `rfight` | Combat occurs in the room | Percentage chance |
| `death` | A character dies in the room | Percentage chance |
| `speech` | A character speaks in the room | Pattern to match |
| `random` | Random chance each tick | Percentage chance |
| `act` | A matching `act()` message | Pattern to match |

### Object Triggers

| Trigger | Fired When | Argument |
|---|---|---|
| `speech` | A character speaks near the object | Pattern to match |
| `random` | Random chance each tick | Percentage chance |
| `wear` | The object is worn/equipped | -- |
| `remove` | The object is removed/unequipped | -- |
| `use` | The object is used (wands, staves) | -- |
| `sac` | The object is sacrificed | -- |
| `drop` | The object is dropped | -- |
| `get` | The object is picked up | -- |
| `zap` | The object is zapped (wand) | -- |
| `examine` | The object is examined | -- |
| `damage` | The object takes damage | -- |
| `repair` | The object is repaired | -- |
| `greet` | A character enters the room with the object | -- |
| `hit` | The weapon hits in combat | -- |
| `act` | A matching `act()` message | Pattern to match |

## Script Language

### Variables

Variables are referenced with `$` followed by a single character:

| Variable | Meaning |
|---|---|
| `$i` | Source entity (the mob/obj/room running the script) |
| `$n` | The triggering character (e.g., who spoke, who entered) |
| `$t` | The victim/target character |
| `$r` | A random PC in the room |
| `$x` | The most evil fighter in the room |
| `$o` | The source object |
| `$p` | The victim/target object |
| `$a` | Extra argument string |

When used in commands, variables expand to the entity's name. In string contexts, additional modifiers are available (name, short description, he/she/it, him/her/it, his/her/its).

### Control Flow

```
if (condition)
    commands...
elseif (condition)
    commands...
else
    commands...
endif
```

Conditionals support:
- Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical operators: `&&` (and), `||` (or)
- Parenthesized grouping
- Procedure calls that return values

### Inline Expressions

Expressions can be embedded in commands using `#expression#`:

```
say I have #goldamount($i)# gold coins.
```

### Procedure Calls

Procedures are called with function syntax: `procname(argument)`. They return integer values for use in conditions.

## Available Procedures

### Character Queries

| Procedure | Argument | Returns |
|---|---|---|
| `alignment(char)` | `$i`, `$n`, `$t`, `$r` | Character's alignment (-1000 to 1000) |
| `clan(char)` | Character variable | Clan number (0 = none) |
| `class(char)` | Character variable | Class index |
| `goldamount(char)` | Character variable | Gold carried |
| `hitpercent(char)` | Character variable | HP as percentage (0-100) |
| `level(char)` | Character variable | Character level |
| `position(char)` | Character variable | Position value |
| `sex(char)` | Character variable | Sex (0=neutral, 1=male, 2=female) |
| `mobvnum(char)` | Character variable | Mob vnum (NPCs only) |

### Boolean Checks

| Procedure | Argument | Returns |
|---|---|---|
| `isawake(char)` | Character variable | TRUE if awake |
| `ischarmed(char)` | Character variable | TRUE if charmed |
| `isfight(char)` | Character variable | TRUE if fighting |
| `isfollow(char)` | Character variable | TRUE if following someone |
| `isimmort(char)` | Character variable | TRUE if immortal |
| `isgood(char)` | Character variable | TRUE if good alignment |
| `isnpc(char)` | Character variable | TRUE if NPC |
| `ispc(char)` | Character variable | TRUE if PC |
| `isname(char, name)` | Character, string | TRUE if name matches |
| `isequal(var, string)` | Variable, string | TRUE if strings match |

### State Queries

| Procedure | Argument | Returns |
|---|---|---|
| `immune(char, type)` | Character, damage type name | TRUE if immune |
| `crimethief(char)` | Character variable | TRUE if flagged as thief |
| `fightinroom()` | None | TRUE if any fighting in room |
| `hasmemory()` | None | TRUE if source mob has a memorized target |
| `memory(name)` | Character name | TRUE if memorized target matches |
| `hour()` | None | Current MUD hour (0-23) |
| `roomvnum()` | None | Vnum of source mob's room |
| `faction(char, vnum)` | Character, faction vnum | Faction standing value |

### Object Queries

| Procedure | Argument | Returns |
|---|---|---|
| `objvnum(obj)` | Object variable | Object vnum |
| `objtype(obj)` | Object variable | Item type |
| `objval0-3(obj)` | Object variable | Item value[0-3] |

### Random Functions

| Procedure | Argument | Returns |
|---|---|---|
| `rand(percent)` | Percentage (1-100) | TRUE if random roll succeeds |
| `getrand(max)` | Maximum value | Random number 1 to max |
| `sgetrand(max)` | Maximum value | "Sticky" random -- same result within one script execution |
| `sreset()` | None | Resets the sticky random counter |

## Action Commands

These commands are available for mob scripts to execute (implemented in `mprog_commands.c`):

| Command | Description |
|---|---|
| `mpkill <target>` | Attack a target without murder penalties |
| `mpjunk <object>` | Destroy objects in mob's inventory |
| `mpecho <text>` | Send message to entire room |
| `mpechoat <target> <text>` | Send message to one character |
| `mpechoaround <target> <text>` | Send message to room except target |
| `mpasound <text>` | Send message to adjacent rooms |
| `mpmload <vnum>` | Load a mob by vnum |
| `mpoload <vnum>` | Load an object by vnum |
| `mppurge [target]` | Remove NPCs/objects from room (or specific target) |
| `mpgoto <location>` | Teleport mob to a location |
| `mpat <location> <command>` | Execute a command at a remote location |
| `mptransfer <target> <location>` | Teleport a player to a location |
| `mpforce <target> <command>` | Force a character to execute a command |
| `mpsilentforce <target> <command>` | Force silently (no output) |
| `mpdosilent <command>` | Mob executes a command silently |
| `mpdefault <target> <command>` | Force through the mudprog interpreter |
| `mpremember <target>` | Memorize a character |
| `mpforget <target>` | Forget a memorized character |
| `mpinvis <level>` | Set mob wizard invisibility |
| `mpfollowpath <path>` | Follow an encoded path string |
| `mpeatcorpse` | Devour NPC corpses in the room |
| `mpclean` | Pick up low-value objects |
| `mprandomsocial` | Perform a random social |
| `mpsilentcast <spell>` | Cast a spell silently |

Standard MUD commands can also be executed from scripts (the script engine calls `interpret()` for unrecognized commands).

## The SuperMob

Objects and rooms cannot directly execute MUD commands (only characters can). The scripting system uses a special hidden mob called the "SuperMob":

1. `set_supermob(room)` moves the SuperMob to the target room and configures it
2. The script executes with the SuperMob as `$i`
3. `release_supermob()` resets the SuperMob after execution

The SuperMob is initialized once at boot via `init_supermob()`.

## MudProg Organization

### Individual Progs (`MPROG_DATA`)

Each prog has:
- `vnum` -- Unique identifier
- `name` -- Descriptive name
- `trigger_type` -- When it fires (act, speech, greet, etc.)
- `arglist` -- Trigger argument (pattern, percentage, etc.)
- `comlist` -- The script code

### Prog Groups (`MPROG_GROUP`)

Groups bundle multiple progs for reusable assignment:
- `vnum` -- Group identifier
- `name` -- Group name
- `mudprogs` -- Linked list of prog references

### Assignment

Progs and groups are assigned to entities via:
- `assign_mobprog()` / `unassign_mobprog()`
- `assign_objprog()` / `unassign_objprog()`
- `assign_roomprog()` / `unassign_roomprog()`

Entity references:
- `MOB_INDEX_DATA->mudprogs` / `->mprog_groups` / `->progtypes`
- `OBJ_INDEX_DATA->mudprogs` / `->mprog_groups` / `->progtypes`
- `ROOM_INDEX_DATA->mudprogs` / `->mprog_groups` / `->progtypes`

The `progtypes` bitmask stores which trigger types have assigned progs, allowing quick checks without walking the prog list.

## Script Example

A guard mob that responds to "password" and attacks thieves:

```
if (ispc($n) && class($n) == 2)
    say Thief! Guards, seize them!
    mpkill $n
elseif (ispc($n))
    if (rand(50))
        say Move along, citizen.
    else
        emote nods at $n.
    endif
endif
```

## Debugging

`mpstat mob/obj/room <target>` shows all MudProgs attached to an entity, including group memberships, trigger types, and script content.
