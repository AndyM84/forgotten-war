# Database and World Loading

The database module (`src/db.c`) is responsible for loading the entire game world from area files at boot time, maintaining in-memory indexes for mobs/objects/rooms, area reset management, and providing core utility functions used throughout the codebase.

## Boot Sequence

`boot_db()` is called once from `main()` during server startup:

1. **Load configuration** -- `load_config_file()` reads `ember.cfg`
2. **Initialize string space** -- `init_string_space()` sets up the shared string heap (SSM)
3. **Initialize random number generator** -- `init_mm()`
4. **Set game time/weather** -- Derives in-game time from real time, initializes weather state
5. **Assign skill GSNs** -- Walks `skill_table[]` setting each `pgsn` pointer to its global variable
6. **Load area files** -- Reads `area.lst` and processes each area file in two passes:
   - **Pass 1**: Loads `#AREADATA`, `#MOBILES`, `#OBJECTS`, `#ROOMS`, `#HELPS`, `#MUDPROGS`, `#SOCIALS`, `#FACTIONS`, `#TODOS`
   - **Pass 2**: Loads `#RESETS`, `#SHOPS`, `#FACTIONAFFS` (these reference entities from pass 1)
7. **Post-processing**:
   - `fix_exits()` -- Resolves exit vnum references to room pointers
   - `boot_done()` -- Frees the temporary string hash (SSM optimization)
   - `init_supermob()` -- Creates the SuperMob for script execution
   - `area_update()` -- Performs initial area resets (populate the world)
   - `load_boards()` -- Load persistent board messages
   - `load_bans()` -- Load site bans
   - `load_classes()` -- Load class skill assignments
   - `load_disabled()` -- Load disabled commands

## Area File Format

Area files use a section-based text format. Each section starts with a `#SECTIONNAME` header. Strings are tilde-terminated (`~`). Numbers use the compact alphabetic flag format for bitmasks.

### Master Area List (`area.lst`)

Lists all area files to load, one per line. Special entries:
- `$` -- End of list marker
- Lines starting with `-` -- Prefixed by the configured area directory
- `helps.are`, `todo.are`, `mudprogs.are`, `socials.are`, `factions.are`, `clans.are` -- System data files loaded via the area list

### Section: `#AREADATA`

Defines an area's metadata:

```
#AREADATA
Name    The Midgaard Area~
Builders    Diku~
VNUMs   3000 3399
Security    9
End
```

### Section: `#MOBILES`

Defines NPC prototypes. Each mob starts with its vnum:

```
#MOBILES
#3000
cityguard~
the cityguard~
A cityguard patrols the area.
~
A stern-looking cityguard glares at you suspiciously.
~
human~
ABD CG 500 0
30 20 6d6+350 5d8+10 3d5+9 punch
-4 -4 -4 -2
ABCD 0 0 0
stand stand male 2
0 0 0 0 0
S
```

Fields include: name, short description, long description, full description, race, act/affect/alignment flags, level, hitroll, hit dice, mana dice, damage dice, damage type, AC values (pierce/bash/slash/exotic), offense/immunity/resistance/vulnerability flags, position, sex, size, and random object generation settings.

### Section: `#OBJECTS`

Defines object prototypes:

```
#OBJECTS
#3010
short sword~
a short sword~
A short sword has been left here.~
iron~
weapon 0 AK
5 1 5 3 0
2 A 0 1
S
```

Fields include: vnum, name, short/long descriptions, material, item type, extra flags, wear flags, item-specific values, level, weight, cost, and affects.

### Section: `#ROOMS`

Defines rooms with exits and extra descriptions:

```
#ROOMS
#3001
The Temple of Midgaard~
You are in the southern end of the temple hall.  The cathedral
ceiling casts long shadows over the stone floor.
~
0 AD 0
D0
You see the grand altar to the north.
~
~
0 0 3002
E
altar~
The altar gleams with holy light.
~
S
```

Fields include: vnum, name, description, area vnum, room flags, sector type, exits (direction, description, keywords, flags, key, destination vnum), extra descriptions, and MudProg assignments.

### Section: `#RESETS`

Defines how the world repopulates. Reset commands:

| Command | Description | Arguments |
|---|---|---|
| `M` | Load a mob | vnum, limit, room_vnum |
| `O` | Load an object in a room | vnum, limit, room_vnum |
| `P` | Put an object in another object | vnum, limit, container_vnum |
| `G` | Give an object to the last mob | vnum, limit |
| `E` | Equip an object on the last mob | vnum, limit, wear_location |
| `D` | Set a door's state | room_vnum, direction, state |
| `R` | Randomize room exits | room_vnum, num_exits |
| `S` | End of resets | -- |

### Section: `#SHOPS`

Defines shopkeeper behavior for mobs with shops:

```
#SHOPS
3064 13 0 0 0 0 150 50 0 23
0
```

Fields: keeper vnum, buy types (up to 5 item types), profit buy %, profit sell %, open hour, close hour.

### Section: `#FACTIONAFFS`

Associates faction effects with mobs (changes to faction standings when a mob is killed):

```
#FACTIONAFFS
3001 1 50
3001 2 -25
0
```

Format: mob_vnum, faction_vnum, change_value.

## In-Memory Indexes

### Hash Tables

Prototypes are stored in hash tables indexed by vnum (`vnum % MAX_KEY_HASH`):

- `mob_index_hash[MAX_KEY_HASH]` -- `MOB_INDEX_DATA` chains
- `obj_index_hash[MAX_KEY_HASH]` -- `OBJ_INDEX_DATA` chains
- `room_index_hash[MAX_KEY_HASH]` -- `ROOM_INDEX_DATA` chains

Lookup functions:
- `get_mob_index(int vnum)` -- Returns the mob prototype or NULL
- `get_obj_index(int vnum)` -- Returns the object prototype or NULL
- `get_room_index(int vnum)` -- Returns the room or NULL

### Creating Instances

- `create_mobile(MOB_INDEX_DATA *pMobIndex)` -- Creates a new NPC from a prototype. Copies stats, allocates the `CHAR_DATA`, and adds to `char_list`. Increments `pMobIndex->count`.
- `create_object(OBJ_INDEX_DATA *pObjIndex, int level)` -- Creates a new object from a prototype. Copies properties, adds to `object_list`. Increments `pObjIndex->count`.
- `clone_mobile()` / `clone_object()` -- Deep-copy an existing instance.

## Area Reset System

Areas automatically repopulate on a timer managed by `area_update()`:

1. Each area has an `age` counter (incremented each `PULSE_AREA`)
2. When `age >= 3` (roughly 3 minutes) and the area is empty (no players), or `age >= 15` regardless, the area resets
3. `reset_area()` iterates the area's rooms and calls `reset_room()` for each
4. `reset_room()` executes the room's reset commands in order:
   - `M` resets: Create a mob if under the limit, place in room
   - `O` resets: Create an object if not already present, place in room
   - `G`/`E` resets: Give/equip objects to the last loaded mob
   - `D` resets: Set door states (open/closed/locked)

## Player Persistence (`src/save.c`)

### Saving

`save_char_obj(CHAR_DATA *ch)`:
1. Skips NPCs and characters in chaos mode
2. Opens a temporary file
3. Writes `#PLAYER` section via `fwrite_char()`:
   - All character attributes as tagged key-value pairs
   - Skills and proficiency levels
   - Group membership
   - Aliases
   - Board read timestamps
4. Writes `#O` sections via `fwrite_obj()` for each object in inventory/equipment:
   - Objects are saved recursively with nesting levels for containers
   - Only fields that differ from the prototype are written
   - Keys and maps may "poof" (not save) based on level rules
5. Writes `#PET` section if the character has a pet
6. Writes `#IMM` section with granted immortal commands
7. Writes `#FACTION` section with faction standings
8. Writes `#END` marker
9. Atomically renames the temp file to the final player file
10. Creates a "god" file for immortals (separate log)

### Loading

`load_char_obj(DESCRIPTOR_DATA *d, char *name)`:
1. Opens the player file from the player directory
2. Creates a new `CHAR_DATA` with defaults
3. Reads sections until `#END`:
   - `#PLAYER` -- Restores all character attributes via `fread_char()`
   - `#O` / `#OBJECT` -- Restores objects via `fread_obj()` with container nesting
   - `#PET` -- Restores pet via `fread_pet()`
   - `#IMM` -- Restores immortal command list
   - `#FACTION` -- Restores faction standings
4. Returns TRUE if the file existed, FALSE for new characters

### File Format

The player file uses tagged key-value pairs readable with `fread_word()`/switch pattern:

```
#PLAYER
Name Gandalf~
Vers 4
Race human~
Sex  1
Cla  0
Levl 50
...
Sk 100 'fireball'
Sk  85 'dodge'
...
#END
```

## File I/O Primitives

`db.c` provides low-level file reading functions used throughout:

| Function | Description |
|---|---|
| `fread_letter(fp)` | Read one non-whitespace character |
| `fread_number(fp)` | Read an integer (supports negative, `|` for OR) |
| `fread_flag(fp)` | Read alphabetic flag encoding or numeric |
| `fread_word(fp)` | Read one word (whitespace-delimited) |
| `fread_to_eol(fp)` | Skip to end of line |
| `fread_string(fp)` | Read a tilde-terminated string (via SSM) |
| `fread_string_eol(fp)` | Read a string until end of line |

## Utility Functions

### Memory Allocation

| Function | Description |
|---|---|
| `alloc_mem(size)` | Allocate memory from size-bucketed free lists |
| `alloc_perm(size)` | Permanent allocation from a large block (never freed) |
| `free_mem(ptr)` | Return memory to its size bucket's free list |

### String Utilities

| Function | Description |
|---|---|
| `str_cmp(a, b)` | Case-insensitive string comparison (returns TRUE if different) |
| `str_prefix(prefix, str)` | Case-insensitive prefix match |
| `str_infix(needle, haystack)` | Case-insensitive substring search |
| `str_suffix(suffix, str)` | Case-insensitive suffix match |
| `capitalize(str)` | Return a capitalized copy |
| `smash_tilde(str)` | Replace `~` with `-` in-place |

### Random Numbers

| Function | Description |
|---|---|
| `number_range(from, to)` | Random integer in [from, to] |
| `number_percent()` | Random 1-100 |
| `number_bits(width)` | Random with `width` bits |
| `number_door()` | Random 0-5 (direction) |
| `number_fuzzy(n)` | n-1, n, or n+1 randomly |
| `dice(number, size)` | Roll `number`d`size` |

### Logging

| Function | Description |
|---|---|
| `bug(fmt, ...)` | Log a bug with file/line context |
| `log_string(str)` | Log a string with timestamp |
| `logf_string(fmt, ...)` | Printf-style logging |
| `append_file(ch, file, str)` | Append a timestamped line to a file |
| `update_last(s1, s2, s3)` | Record last operation for crash debugging |
