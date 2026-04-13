# Online Creation (OLC)

The OLC system allows immortals to create and edit the game world in real-time without modifying area files manually. The implementation spans `olc.c`, `olc.h`, `olc_act.c`, and `olc_save.c`, plus specialized editors in `socialolc.c`, `helpolc.c`, and `todoolc.c`.

## Architecture

OLC uses a state-machine approach. When a builder enters an editor:

1. An entry command (`do_aedit`, `do_redit`, etc.) sets `d->editor` to the editor type and `d->pEdit` to the entity being edited
2. Input is routed through `run_olc_editor()` instead of the normal command interpreter
3. The editor-specific interpreter parses commands against its command table
4. Unrecognized commands fall through to the standard interpreter
5. Typing `done` exits the editor via `edit_done()`

### Editor Types

| Editor | Constant | Entry Command | Purpose |
|---|---|---|---|
| Area | `ED_AREA` | `aedit` | Edit area metadata |
| Room | `ED_ROOM` | `redit` | Edit rooms, exits, extra descriptions |
| Object | `ED_OBJECT` | `oedit` | Edit object prototypes |
| Mobile | `ED_MOBILE` | `medit` | Edit mob prototypes |
| MudProg | `ED_MPROG` | `mpedit` | Edit MudProg scripts |
| MudProg Group | `ED_MPGROUP` | `mpgedit` | Edit MudProg groups |
| Clan | `ED_CLAN` | `cedit` | Edit clans |
| Faction | `ED_FACTION` | `factionedit` | Edit factions |
| Social | `ED_SOCIAL` | `socialedit` | Edit social emotes |
| Help | `ED_HELPOLC` | `helpedit` | Edit help entries |
| Todo | `ED_TODOOLC` | `todoedit` | Edit todo entries |

### Security System

OLC access is controlled by two mechanisms:

1. **Builder list**: Each area has a `builders` string listing authorized builder names. The `IS_BUILDER(ch, area)` macro checks if the character is listed or has sufficient security level.
2. **Security level**: Each area and builder has a numeric security level (0-9). Builders can only modify areas with a security level at or below their own.

## Area Editor (aedit)

**Entry**: `aedit <vnum|name>` or `aedit create`

| Command | Description |
|---|---|
| `show` | Display area information |
| `create` | Create a new area |
| `name <text>` | Set area name |
| `file <filename>` | Set area filename |
| `age <ticks>` | Set area age (reset timer) |
| `reset` | Force area reset |
| `security <0-9>` | Set security level |
| `builder <name>` | Add/remove authorized builder |
| `vnum <lower> <upper>` | Set vnum range |
| `lvnum <vnum>` | Set lower vnum |
| `uvnum <vnum>` | Set upper vnum |
| `done` | Exit editor |

## Room Editor (redit)

**Entry**: `redit` (edit current room) or `redit <vnum>` or `redit create <vnum>`

| Command | Description |
|---|---|
| `show` | Display room information |
| `create <vnum>` | Create a new room |
| `name <text>` | Set room name |
| `desc` | Enter string editor for description |
| `north/south/east/west/up/down` | Edit exits (subcommands below) |
| `ed <keyword>` | Add/edit extra descriptions |
| `format` | Auto-format room description |
| `flags <flag list>` | Set room flags |
| `sector <type>` | Set sector type |
| `done` | Exit editor |

### Exit Subcommands

`<direction> <subcommand>`:

| Subcommand | Description |
|---|---|
| `link <vnum>` | Create two-way exit to a room |
| `room <vnum>` | Set one-way exit destination |
| `key <vnum>` | Set key object vnum |
| `name <keywords>` | Set door keywords |
| `desc` | Set exit description |
| `flags <flag list>` | Set exit flags (door, locked, hidden, etc.) |
| `delete` | Remove the exit |

## Object Editor (oedit)

**Entry**: `oedit <vnum>` or `oedit create <vnum>`

| Command | Description |
|---|---|
| `show` | Display object information |
| `create <vnum>` | Create a new object |
| `name <keywords>` | Set name/keywords |
| `short <desc>` | Set short description |
| `long <desc>` | Set long (room) description |
| `desc` | Enter string editor for full description |
| `type <item_type>` | Set item type (weapon, armor, potion, etc.) |
| `extra <flags>` | Set extra flags (glow, hum, evil, etc.) |
| `wear <flags>` | Set wear flags (take, head, body, etc.) |
| `level <num>` | Set object level |
| `weight <num>` | Set weight |
| `cost <num>` | Set gold value |
| `condition <num>` | Set condition (0-100%) |
| `material <type>` | Set material type |
| `values` | Edit item-type-specific values |
| `affect <location> <modifier>` | Add a permanent affect |
| `clan <num>` | Set clan restriction |
| `done` | Exit editor |

## Mobile Editor (medit)

**Entry**: `medit <vnum>` or `medit create <vnum>`

| Command | Description |
|---|---|
| `show` | Display mob information |
| `create <vnum>` | Create a new mob |
| `name <keywords>` | Set name/keywords |
| `short <desc>` | Set short description |
| `long <desc>` | Set long (room) description |
| `desc` | Enter string editor for full description |
| `level <num>` | Set level (auto-calculates stats) |
| `alignment <num>` | Set alignment (-1000 to 1000) |
| `sex <male/female/neutral>` | Set sex |
| `race <name>` | Set race |
| `act <flags>` | Set ACT flags (sentinel, scavenger, aggressive, etc.) |
| `affect <flags>` | Set AFF flags (sanctuary, invisible, etc.) |
| `off <flags>` | Set offense flags (bash, berserk, disarm, etc.) |
| `imm <flags>` | Set immunity flags |
| `res <flags>` | Set resistance flags |
| `vuln <flags>` | Set vulnerability flags |
| `form <flags>` | Set body form flags |
| `part <flags>` | Set body part flags |
| `size <size>` | Set size (tiny to giant) |
| `hitroll <num>` | Set hit bonus |
| `hitdice <NdN+N>` | Set HP dice |
| `manadice <NdN+N>` | Set mana dice |
| `damdice <NdN+N>` | Set damage dice |
| `damtype <type>` | Set damage type |
| `ac <pierce> <bash> <slash> <exotic>` | Set armor class values |
| `gold <num>` | Set gold carried |
| `position <pos>` | Set default position |
| `start <pos>` | Set starting position |
| `shop <args>` | Configure shop behavior |
| `faction <args>` | Configure faction affects on kill |
| `done` | Exit editor |

## MudProg Editor (mpedit)

**Entry**: `mpedit <vnum>` or `mpedit create`

| Command | Description |
|---|---|
| `show` | Display MudProg information |
| `create` | Create a new MudProg |
| `name <text>` | Set prog name |
| `desc` | Set description |
| `trigger <type>` | Set trigger type (act, speech, greet, entry, etc.) |
| `arglist <text>` | Set trigger argument/pattern |
| `code` | Enter string editor for script code |
| `assign mob/obj/room <vnum>` | Assign to an entity |
| `unassign mob/obj/room <vnum>` | Remove from an entity |
| `done` | Exit editor |

## MudProg Group Editor (mpgedit)

**Entry**: `mpgedit <vnum>` or `mpgedit create`

Groups MudProgs into reusable bundles that can be assigned to multiple entities.

| Command | Description |
|---|---|
| `show` | Display group information |
| `create` | Create a new group |
| `name <text>` | Set group name |
| `desc` | Set description |
| `add <vnum>` | Add a MudProg to the group |
| `remove <vnum>` | Remove a MudProg from the group |
| `assign mob/obj/room <vnum>` | Assign group to an entity |
| `unassign mob/obj/room <vnum>` | Remove group from an entity |
| `done` | Exit editor |

## Social Editor (socialedit)

**Entry**: `socialedit <name|vnum>` or `socialedit create`

| Command | Description |
|---|---|
| `show` | Display social messages |
| `create` | Create a new social |
| `name <text>` | Set social name |
| `chnoarg <text>` | Set "no argument" message (performer) |
| `othersnoarg <text>` | Set "no argument" message (room) |
| `chfound <text>` | Set "target found" message (performer) |
| `othersfound <text>` | Set "target found" message (room) |
| `victfound <text>` | Set "target found" message (victim) |
| `chnotfound <text>` | Set "target not found" message |
| `chauto <text>` | Set "self-targeted" message (performer) |
| `othersauto <text>` | Set "self-targeted" message (room) |
| `done` | Exit editor and save |

## Help Editor (helpedit)

**Entry**: `helpedit <keyword>` or `helpedit create`

| Command | Description |
|---|---|
| `show` | Display help entry info |
| `create` | Create a new help entry |
| `keyword <text>` | Set keywords |
| `level <num>` | Set minimum level to view |
| `text` | Enter string editor for help text |
| `done` | Exit editor and save |

## Saving

### Manual Save

`asave <option>`:

| Option | Description |
|---|---|
| `changed` | Save all modified areas |
| `world` | Save everything |
| `area` | Save current area |
| `list` | Save the area list file |
| `helps` | Save help entries |
| `todo` | Save todo entries |
| `mudprogs` | Save MudProg definitions |
| `socials` | Save social commands |
| `factions` | Save faction definitions |
| `<name>` | Save a specific area by name |

### Automatic Save

Areas are marked with `AREA_CHANGED` when modified. The `asave changed` command (or periodic auto-save) writes only modified areas.

### Save Format

Area files are written by `olc_save.c` functions:
- `save_area()` orchestrates the full save of a single area file
- Each section (`#AREADATA`, `#MOBILES`, `#OBJECTS`, `#ROOMS`, `#MUDPROGS`, `#RESETS`, `#SHOPS`, `#FACTIONAFFS`, `#$`) is written by a dedicated function
- The compact alphabetic flag format is used for bitmasks (A=bit0, B=bit1, ... Z=bit25, a=bit26, etc.)

## Utility Commands

| Command | Description |
|---|---|
| `alist` | List all areas with vnum ranges, builders, security, and flags |
| `resets` | View/modify room resets (add mob, add obj, edit, delete) |
| `rlist <lower> <upper>` | List rooms in a vnum range |

## The String Editor

When editing descriptions, the OLC system drops into an in-line text editor (`string.c`):

| Dot-Command | Description |
|---|---|
| `.c` | Clear the entire string |
| `.s` | Show the string with line numbers |
| `.r <old> <new>` | Replace substring |
| `.l <line#> <text>` | Replace a specific line |
| `.d <line#>` | Delete a line |
| `.a <line#> <text>` | Insert a line |
| `.f` | Format/word-wrap the text |
| `.h` | Show help |
| `~` or `@` | Exit the string editor |

Plain text input (without a dot prefix) appends to the string.
