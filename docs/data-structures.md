# Core Data Structures

All core data structures are defined in `src/merc.h`. This document covers the primary types that form the backbone of EmberMUD.

## CHAR_DATA -- Characters (PCs and NPCs)

Defined in `merc.h:493`. Represents both player characters and NPCs in the game world.

```
struct char_data {
    // Linked list pointers
    CHAR_DATA *next;            // Next in global char_list
    CHAR_DATA *next_player;     // Next in player_list (PCs only)
    CHAR_DATA *next_in_room;    // Next character in same room
    CHAR_DATA *next_in_shell;   // Next in shell_char_list (shell users)

    // Relationships
    CHAR_DATA *master;          // Who this character follows
    CHAR_DATA *leader;          // Group leader
    CHAR_DATA *fighting;        // Current combat target
    CHAR_DATA *reply;           // Last person who sent a tell
    CHAR_DATA *pet;             // Pet NPC
    CHAR_DATA *memory;          // Memorized target (for mobprogs)
    HATE_DATA *hate;            // Hatred target (for auto-aggro)

    // Core references
    MOB_INDEX_DATA *pIndexData; // Prototype (NPCs only; NULL for PCs)
    DESCRIPTOR_DATA *desc;      // Network connection (NULL for NPCs)
    AFFECT_DATA *affected;      // Classic affect linked list
    NEWAFFECT_DATA *newaffected; // Extended affect linked list
    OBJ_DATA *carrying;         // Inventory (linked list via next_content)
    OBJ_DATA *on;               // Furniture object currently on
    ROOM_INDEX_DATA *in_room;   // Current room
    ROOM_INDEX_DATA *was_in_room; // Previous room (for link-death recovery)
    PC_DATA *pcdata;            // Player-specific data (NULL for NPCs)
    GEN_DATA *gen_data;         // Character generation data (temporary)

    // Identity
    char *name;                 // Name/keywords
    char *short_descr;          // Short description ("a goblin guard")
    char *long_descr;           // Long description (seen in room)
    char *description;          // Full description (when looked at)

    // Core attributes
    sh_int sex;                 // SEX_MALE, SEX_FEMALE, SEX_NEUTRAL
    sh_int Class;               // Class index into class_table[]
    sh_int race;                // Race index into race_table[]
    sh_int level;               // Character level (1-100)
    sh_int trust;               // Trust level (overrides level for permissions)

    // Vitals
    sh_int hit, max_hit;        // Hit points
    sh_int mana, max_mana;      // Mana points
    sh_int move, max_move;      // Movement points
    long gold;                  // Gold carried
    long exp;                   // Experience points

    // Flags (bitmasks)
    long act;                   // ACT_IS_NPC, ACT_SENTINEL, PLR_HOLYLIGHT, etc.
    long comm;                  // COMM_NOGOSSIP, COMM_QUIET, etc.
    long affected_by;           // AFF_BLIND, AFF_INVISIBLE, AFF_SANCTUARY, etc.
    char newaff[];              // Extended affect bits (byte array)
    long imm_flags;             // Immunity flags
    long res_flags;             // Resistance flags
    long vuln_flags;            // Vulnerability flags

    // Combat
    sh_int position;            // POS_DEAD through POS_STANDING
    sh_int hitroll;             // Bonus to hit
    sh_int damroll;             // Bonus to damage
    sh_int armor[4];            // AC by type: pierce, bash, slash, exotic
    sh_int saving_throw;        // Saving throw bonus
    sh_int wimpy;               // Auto-flee HP threshold
    sh_int damage[3];           // NPC damage dice: number, type, bonus
    sh_int dam_type;            // NPC damage type

    // Stats
    sh_int perm_stat[MAX_STATS]; // Permanent stats (STR, INT, WIS, DEX, CON)
    sh_int mod_stat[MAX_STATS];  // Stat modifiers from equipment/spells

    // Miscellaneous
    sh_int alignment;           // -1000 (evil) to +1000 (good)
    sh_int practice;            // Practice sessions available
    sh_int train;               // Training sessions available
    sh_int incarnations;        // Number of remorts
    sh_int timer;               // Idle timer
    sh_int wait;                // Command lag (in pulses)
    sh_int daze;                // Combat stun timer
    sh_int invis_level;         // Wizard invisibility level

    // NPC-specific
    long off_flags;             // Offensive behavior flags
    long form;                  // Body form flags
    long parts;                 // Body part flags
    sh_int size;                // SIZE_TINY through SIZE_GIANT
    MPROG_ACT_LIST *mpact;      // Pending mudprog act triggers
    sh_int jail_timer;          // Jail duration remaining
};
```

The `IS_NPC(ch)` macro checks the `ACT_IS_NPC` bit to distinguish NPCs from players.

## PC_DATA -- Player-Specific Data

Defined in `merc.h:644`. Contains data that only player characters have. Accessed via `ch->pcdata` (NULL for NPCs).

```
struct pc_data {
    // Message queues
    QUEUE_DATA *message;        // Queued messages (head)
    QUEUE_DATA *fmessage;       // Queued messages (tail)
    QUEUE_DATA *tell_q;         // Tell queue (head)
    QUEUE_DATA *ftell_q;        // Tell queue (tail)
    BUFFER *buffer;             // General-purpose buffer

    // Board system
    BOARD_DATA *board;          // Current board being read
    time_t last_note[MAX_BOARD]; // Last read note timestamp per board
    NOTE_DATA *in_progress;     // Note currently being written
    int messages;               // Queued message count
    int tells;                  // Queued tell count

    // Authentication
    char *pwd;                  // Encrypted password

    // Display
    char *bamfin;               // Custom immortal arrival message
    char *bamfout;              // Custom immortal departure message
    char *title;                // Character title (shown in who)
    char *prompt;               // Custom prompt format string
    char *who_race;             // Custom who-list race display
    char *who_prefix;           // Custom who-list prefix

    // Base vitals (before equipment/affects)
    sh_int perm_hit;            // Base max HP
    sh_int perm_mana;           // Base max mana
    sh_int perm_move;           // Base max move
    sh_int true_sex;            // Original sex (before sex-change spells)
    int last_level;             // Ticks since last level gain

    // OLC
    int security;               // OLC security level (0-9)
    int vnum_range[2];          // Assigned vnum range for building

    // Skills
    sh_int learned[MAX_SKILL];  // Skill proficiency (0-100%)
    bool group_known[MAX_GROUP]; // Known skill groups
    sh_int points;              // Creation points spent
    sh_int condition[3];        // COND_DRUNK, COND_FULL, COND_THIRST

    // Clan
    sh_int clan;                // Clan number (0 = none)
    sh_int clan_rank;           // Rank within clan
    sh_int join_status;         // Petitioning status
    CHAR_DATA *clan_ch;         // Character being petitioned/offered

    // Social
    char *spouse;               // Married to (name or "(none)")
    int spousec;                // Marriage count
    char *email;                // Email address
    char *comment;              // Immortal comment on player
    char *nemesis;              // Nemesis name

    // PK
    int pk_deaths;              // PK death count
    int pk_kills;               // PK kill count
    int chaos_score;            // Chaos mode kill count

    // Economy
    long gold_bank;             // Gold stored in bank

    // Aliases
    char *alias[MAX_ALIAS];     // Alias names
    char *alias_sub[MAX_ALIAS]; // Alias expansions

    // Confirmations
    bool confirm_delete;        // Pending character deletion
    bool confirm_remort;        // Pending remort
    bool confirm_pk;            // Pending PK flag toggle

    // Systems
    FACTIONPC_DATA *faction_standings; // Faction reputation values
    IMMCMD_TYPE *immcmdlist;    // Granted immortal commands
    ROOM_INDEX_DATA *recall_room; // Personal recall location
};
```

## OBJ_DATA -- Object Instances

Defined in `merc.h:805`. Represents a single object in the game world.

```
struct obj_data {
    OBJ_DATA *next;             // Next in global object_list
    OBJ_DATA *next_content;     // Next in container/inventory
    OBJ_DATA *contains;         // Contents (if container)
    OBJ_DATA *in_obj;           // Container this is inside
    OBJ_DATA *on;               // Object this is on (furniture)
    CHAR_DATA *carried_by;      // Character carrying this
    EXTRA_DESCR_DATA *extra_descr; // Extra descriptions
    AFFECT_DATA *affected;      // Object affects
    OBJ_INDEX_DATA *pIndexData; // Prototype reference
    ROOM_INDEX_DATA *in_room;   // Room this is in (NULL if carried/contained)

    // Strings (may point to prototype or be overridden)
    char *owner;                // Owner name (for corpses)
    char *name;                 // Keywords
    char *short_descr;          // Short description ("a rusty sword")
    char *description;          // Room description ("A rusty sword lies here.")

    // Properties
    sh_int item_type;           // ITEM_WEAPON, ITEM_ARMOR, ITEM_POTION, etc.
    int extra_flags;            // ITEM_GLOW, ITEM_HUM, ITEM_EVIL, etc.
    sh_int wear_flags;          // Where it can be worn
    sh_int wear_loc;            // Where it is currently worn (-1 = not worn)
    sh_int weight;              // Weight in 10ths of a pound
    int cost;                   // Value in gold
    sh_int level;               // Object level
    sh_int condition;           // Condition percentage (0-100)
    sh_int material;            // Material type
    sh_int timer;               // Decay timer (ticks until destruction)
    int value[5];               // Item-type-specific values
    sh_int clan;                // Clan restriction (0 = none)
    bool enchanted;             // Has been enchanted (uses own affects, not prototype's)
};
```

### Item Type Values

The `value[5]` array has different meanings per item type:

| Item Type | value[0] | value[1] | value[2] | value[3] | value[4] |
|---|---|---|---|---|---|
| `ITEM_WEAPON` | weapon class | dice number | dice type | damage type | weapon flags |
| `ITEM_ARMOR` | AC pierce | AC bash | AC slash | AC exotic | -- |
| `ITEM_POTION/PILL/SCROLL` | spell level | spell 1 sn | spell 2 sn | spell 3 sn | -- |
| `ITEM_WAND/STAFF` | spell level | max charges | charges left | spell sn | -- |
| `ITEM_CONTAINER` | capacity | flags | key vnum | max weight | -- |
| `ITEM_DRINK_CON` | capacity | current | liquid type | poisoned | -- |
| `ITEM_FOOD` | fullness | -- | -- | poisoned | -- |
| `ITEM_PORTAL` | -- | -- | -- | destination room vnum | -- |
| `ITEM_FOUNTAIN` | capacity | current | liquid type | -- | -- |
| `ITEM_FURNITURE` | max people | max weight | furniture flags | heal bonus | mana bonus |

## MOB_INDEX_DATA -- NPC Prototypes

Defined in `merc.h:440`. Template data for creating NPC instances. Stored in `mob_index_hash[]`.

Key fields include vnum, name strings, stats, combat dice, AC by type, flags (act, affect, offense, immunity, resistance, vulnerability), race, size, material, shop data, MudProg assignments, random object generation settings, faction affects, and path data.

## OBJ_INDEX_DATA -- Object Prototypes

Defined in `merc.h:776`. Template data for creating object instances. Stored in `obj_index_hash[]`.

Key fields include vnum, name strings, item type, extra/wear flags, level, condition, material, weight, cost, values array, extra descriptions, affects, MudProg assignments, and clan restriction.

## ROOM_INDEX_DATA -- Rooms

Defined in `merc.h:902`. Rooms are not instanced -- the index data IS the room.

```
struct room_index_data {
    ROOM_INDEX_DATA *next;      // Hash chain
    RESET_DATA *reset_first;    // Reset commands for this room
    RESET_DATA *reset_last;
    CHAR_DATA *people;          // Characters in this room
    OBJ_DATA *contents;         // Objects on the floor
    EXTRA_DESCR_DATA *extra_descr; // Extra descriptions
    AREA_DATA *area;            // Area this room belongs to
    EXIT_DATA *exit[6];         // Exits: N, E, S, W, Up, Down
    char *name;                 // Room name
    char *description;          // Room description
    sh_int vnum;                // Virtual number
    int room_flags;             // ROOM_DARK, ROOM_SAFE, ROOM_INDOORS, etc.
    sh_int light;               // Light level (number of light sources)
    sh_int sector_type;         // SECT_INSIDE, SECT_CITY, SECT_FOREST, etc.
    // MudProg data
    MPROG_LIST *mudprogs;
    MPROG_GROUP_LIST *mprog_groups;
    int progtypes;
};
```

## DESCRIPTOR_DATA -- Network Connections

Defined in `merc.h:259`. Represents a single TCP connection.

```
struct descriptor_data {
    DESCRIPTOR_DATA *next;      // Next in descriptor_list
    DESCRIPTOR_DATA *snoop_by;  // Descriptor snooping this one
    CHAR_DATA *character;       // Character for this connection
    CHAR_DATA *original;        // Original body (when switched)
    char *host;                 // Hostname or IP
    sh_int descriptor;          // File descriptor (socket)
    sh_int connected;           // Connection state (CON_* values)
    bool fcommand;              // Has a command pending
    bool ansi;                  // ANSI color enabled
    char inbuf[4*MAX_INPUT_LENGTH]; // Raw input buffer
    char incomm[MAX_INPUT_LENGTH];  // Current command being processed
    char inlast[MAX_INPUT_LENGTH];  // Last command (for ! repeat)
    int repeat;                 // Command repeat counter
    char *outbuf;               // Output buffer
    int outsize;                // Output buffer allocated size
    int outtop;                 // Output buffer current length
    char *showstr_head;         // Pager: start of text
    char *showstr_point;        // Pager: current position
    void *pEdit;                // OLC: object being edited
    char **pString;             // OLC: string being edited
    int editor;                 // OLC: current editor type (ED_*)
};
```

### Connection States (CON_*)

| State | Description |
|---|---|
| `CON_PLAYING` | Normal gameplay |
| `CON_GET_ANSI` | Asking about ANSI color |
| `CON_GET_NAME` | Entering character name |
| `CON_GET_OLD_PASSWORD` | Entering password for existing character |
| `CON_CONFIRM_NEW_NAME` | Confirming new character name |
| `CON_GET_NEW_PASSWORD` | Creating password |
| `CON_CONFIRM_NEW_PASSWORD` | Confirming password |
| `CON_GET_NEW_RACE` | Choosing race |
| `CON_GET_NEW_SEX` | Choosing sex |
| `CON_GET_NEW_CLASS` | Choosing class |
| `CON_GET_ALIGNMENT` | Choosing alignment |
| `CON_DEFAULT_CHOICE` | Choosing default skill package |
| `CON_GEN_GROUPS` | Customizing skill groups |
| `CON_PICK_WEAPON` | Choosing starting weapon |
| `CON_READ_IMOTD` / `CON_READ_MOTD` | Reading message of the day |
| `CON_BREAK_CONNECT` | Breaking existing connection |
| `CON_NOTE_*` | Note composition states |
| `CON_BEGIN_REMORT` | Starting remort process |
| `CON_ROLL_STATS` | Rolling character stats |

## AFFECT_DATA -- Spell/Skill Effects

Defined in `merc.h:408`. Represents a temporary effect on a character or object.

```
struct affect_data {
    AFFECT_DATA *next;
    sh_int type;        // Skill/spell number (index into skill_table)
    sh_int level;       // Caster/source level
    sh_int duration;    // Ticks remaining (-1 = permanent)
    sh_int location;    // What stat it modifies (APPLY_*)
    sh_int modifier;    // How much it modifies
    int bitvector;      // AFF_* flag it sets
};
```

### APPLY Locations

| Constant | Effect |
|---|---|
| `APPLY_STR` | Modify strength |
| `APPLY_DEX` | Modify dexterity |
| `APPLY_INT` | Modify intelligence |
| `APPLY_WIS` | Modify wisdom |
| `APPLY_CON` | Modify constitution |
| `APPLY_HIT` | Modify max HP |
| `APPLY_MANA` | Modify max mana |
| `APPLY_MOVE` | Modify max movement |
| `APPLY_AC` | Modify armor class |
| `APPLY_HITROLL` | Modify hit bonus |
| `APPLY_DAMROLL` | Modify damage bonus |
| `APPLY_SAVING_SPELL` | Modify saving throw |
| `APPLY_SEX` | Change sex |

## AREA_DATA -- Areas

Defined in `merc.h:882`. Represents a loaded area (collection of rooms, mobs, objects).

```
struct area_data {
    AREA_DATA *next;
    RESET_DATA *reset_first, *reset_last; // Reset commands
    char *name;             // Area name
    sh_int age;             // Ticks since last reset
    sh_int nplayer;         // Number of players in area
    bool empty;             // No players present
    char *filename;         // Area file name
    char *builders;         // Authorized builder names
    int security;           // OLC security level (1-9)
    int lvnum, uvnum;       // Lower and upper vnum range
    int vnum;               // Area's own vnum
    int area_flags;         // AREA_CHANGED, AREA_ADDED, AREA_LOADING
};
```

## CLAN_DATA -- Clans

Defined in `merc.h:160`. Contains all data for a clan/guild.

Key fields: number, name, whoname, leader, god (sponsor), members array, kills/deaths arrays (per-clan PvP tracking), rank names, flags, recall room vnum, min level, clan fund.

## Skill and Class Tables

### skill_type (skill_table[])

Defined in `merc.h:927`. Each entry in the skill table describes one spell or skill:

- `name` -- display name
- `skill_level[MAX_CLASS]` -- level at which each class gets the skill
- `rating[MAX_CLASS]` -- learning difficulty per class
- `spell_fun` -- function pointer for spells (NULL for non-spell skills)
- `target` -- targeting type (TAR_CHAR_OFFENSIVE, TAR_CHAR_DEFENSIVE, etc.)
- `minimum_position` -- required position to use
- `pgsn` -- pointer to the global skill number variable
- `slot` -- slot number for object-based spell references
- `min_mana` -- base mana cost
- `beats` -- command lag in pulses
- `noun_damage` -- damage message noun
- `msg_off` -- wear-off message

### class_type (class_table[])

Defined in `merc.h:343`:

- `name` -- full class name
- `who_name[4]` -- three-letter abbreviation
- `attr_prime` -- primary stat
- `weapon` -- starting weapon vnum
- `guild[MAX_GUILD]` -- guild room vnums
- `thac0_00`, `thac0_32` -- THAC0 at levels 0 and 32 (interpolated)
- `hp_min`, `hp_max` -- HP gain range per level
- `fMana` -- whether class uses mana
- `base_group`, `default_group` -- default skill groups
- `remort_class` -- whether this is a remort-only class

## Key Macros

| Macro | Description |
|---|---|
| `IS_NPC(ch)` | True if character is an NPC |
| `IS_IMMORTAL(ch)` | True if trust >= LEVEL_IMMORTAL |
| `IS_HERO(ch)` | True if trust >= LEVEL_HERO |
| `IS_AFFECTED(ch, sn)` | True if classic affect flag is set |
| `IS_NEWAFFECTED(ch, sn)` | True if extended affect flag is set |
| `IS_SET(flag, bit)` | Test a bit in a bitmask |
| `SET_BIT(var, bit)` | Set a bit |
| `REMOVE_BIT(var, bit)` | Clear a bit |
| `UMIN(a, b)` | Unsigned minimum |
| `UMAX(a, b)` | Unsigned maximum |
| `URANGE(a, b, c)` | Clamp b between a and c |
| `GET_AC(ch, type)` | Effective AC including dexterity bonus |
| `GET_HITROLL(ch)` | Effective hitroll including strength bonus |
| `GET_DAMROLL(ch)` | Effective damroll including strength bonus |
| `IS_GOOD(ch)` | Alignment >= 350 |
| `IS_EVIL(ch)` | Alignment <= -350 |
| `IS_AWAKE(ch)` | Position > POS_SLEEPING |
| `WAIT_STATE(ch, n)` | Set command lag |
| `PERS(ch, looker)` | Character name as seen by looker |
