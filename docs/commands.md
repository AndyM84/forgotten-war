# Command Interpreter and Player Commands

## Command Dispatch (`src/interp.c`)

### The Command Table

The master command table `cmd_table[]` in `interp.c` is a flat static array of `struct cmd_type` entries:

```c
struct cmd_type {
    char *const name;   // Command name
    DO_FUN *do_fun;     // Function pointer
    sh_int position;    // Minimum position to use
    bool imm;           // Is this an immortal command?
    sh_int log;         // Logging type: LOG_NORMAL, LOG_ALWAYS, LOG_NEVER
    bool show;          // Show in 'commands' list?
};
```

Commands are searched by prefix match -- typing `nor` matches `north`. The first matching entry wins.

### Command Execution Flow

`interpret(CHAR_DATA *ch, char *argument)`:

1. Strip leading whitespace
2. Remove hide status
3. Check if frozen (cannot execute commands if frozen)
4. Parse the first word as the command name
5. Check MudProg command triggers (`mprog_command_trigger`)
6. Search `cmd_table[]` for a matching command:
   - If the command is marked as `imm` (immortal), check `can_do_immcmd(ch, name)` -- skip if the character lacks permission, continue searching for a non-immortal match
7. If no match found, check socials via `check_social()`
8. If still no match, send "Huh?" error
9. Log the command if required (`LOG_ALWAYS`, or `fLogAll`, or per-player logging)
10. Check if the command is disabled (`check_disabled()`)
11. Check position requirements
12. Call the command's `do_fun(ch, argument)`

### Immortal Command Permissions

Rather than using a simple level check, EmberMUD uses per-command permissions:

- Each immortal has an `immcmdlist` (linked list of `IMMCMD_TYPE`)
- `is_immcmd(command)` checks if any command in the table with that name has the `imm` flag set
- `can_do_immcmd(ch, cmd)` checks if the command is in the character's `immcmdlist`
- `do_wizgrant` / `do_wizrevoke` add/remove individual command permissions

### Disabled Commands

Administrators can disable commands at runtime:
- `do_disable(ch, command)` toggles a command on/off
- `check_disabled(command)` returns TRUE if the command is disabled
- Disabled commands persist across reboots via `save_disabled()` / `load_disabled()`

### Argument Parsing Utilities

| Function | Description |
|---|---|
| `one_argument(arg, first)` | Extract one word, convert to lowercase |
| `one_argument2(arg, first)` | Extract one word, preserve case |
| `number_argument(arg, first)` | Parse `N.name` syntax (e.g., `2.sword`) |
| `is_number(arg)` | Test if a string is numeric |

## Player Commands by Category

### Communication (`act_comm.c`)

| Command | Description |
|---|---|
| `gossip` | Global chat channel |
| `ooc` | Out-of-character channel |
| `question` / `answer` | Q&A channel pair |
| `music` | Music channel (optional) |
| `say` | Speak to the current room (triggers speech MudProgs) |
| `yell` | Area-wide message |
| `shout` | Area-wide message with cooldown |
| `tell` / `reply` | Private messaging |
| `gtell` | Group-only chat |
| `emote` | Custom action message |
| `immtalk` | Immortal-only channel |
| `admintalk` | Admin-only channel (optional) |
| `herotalk` | Hero/remort channel (optional) |
| `clantalk` | Clan-private channel |
| `spousetalk` | Spouse-private channel |
| `channels` | List channel on/off status |
| `quiet` | Toggle all channels |
| `deaf` | Toggle shout hearing |
| `last` / `lastimm` | View recent channel history |
| `messages` / `tq` | View queued messages/tells |
| `bug` / `idea` / `typo` | Submit reports |
| `follow` / `group` / `order` | Group management |
| `split` | Split gold among group |
| `quit` / `save` / `delete` | Session management |

### Information (`act_info.c`)

| Command | Description |
|---|---|
| `look` / `examine` | View rooms, objects, characters |
| `exits` | Show available exits |
| `scan` | Scan adjacent rooms |
| `score` | Full character sheet |
| `effects` | Active spell/affect list |
| `levels` | XP requirements per level |
| `worth` | Gold and XP display |
| `who` | Online player list with filtering |
| `cwho` | Same-class player list |
| `finger` | View player info (online or offline) |
| `inventory` / `equipment` | View carried/worn items |
| `compare` | Compare two items |
| `where` | Locate characters in area |
| `consider` | Assess target difficulty |
| `time` / `weather` | Game time and weather |
| `help` / `todo` | Help system and developer todos |
| `practice` | Train skills at a trainer |
| `prompt` | Customize prompt display |
| `scroll` | Set page length |
| `alias` / `unalias` | Manage command aliases |
| `title` / `description` | Set character display info |
| `password` | Change password |
| `wimpy` | Set auto-flee threshold |
| `afk` / `anonymous` | Toggle status flags |
| `autolist` | Show auto-action settings |
| `autoloot` / `autogold` / `autosac` / `autosplit` / `autoassist` / `autoexit` | Toggle auto-actions |
| `brief` / `compact` | Toggle display brevity |
| `combine` | Toggle combined item display |
| `nofollow` / `nosummon` / `noloot` / `nocolor` | Toggle protections |
| `pk` | Toggle PK flag |
| `search` | Search for hidden exits/objects |
| `levelgain` | Level up when enough XP |
| `report` | Announce HP/mana/move to room |

### Movement (`act_move.c`)

| Command | Description |
|---|---|
| `north` / `east` / `south` / `west` / `up` / `down` | Move in a direction |
| `open` / `close` | Open/close doors and containers |
| `lock` / `unlock` | Lock/unlock with keys |
| `pick` | Pick locks (thief skill) |
| `stand` / `sit` / `rest` / `sleep` / `wake` | Change position (supports furniture) |
| `sneak` | Move silently |
| `hide` | Hide from view |
| `visible` | Remove invisibility/hide/sneak |
| `recall` | Teleport to recall point |
| `beacon` / `beaconreset` | Set/reset personal recall point |
| `train` | Train stats at a trainer |
| `track` | BFS pathfinding to a target |
| `enter` | Enter a portal |

### Object Manipulation (`act_obj.c`)

| Command | Description |
|---|---|
| `get` / `put` / `drop` / `give` | Move objects between locations |
| `donate` | Donate items to a pit |
| `wear` / `remove` | Equip/unequip items |
| `eat` / `drink` / `fill` / `pour` | Consume food and drink |
| `quaff` / `recite` / `brandish` / `zap` | Use magic items |
| `sacrifice` / `junk` | Destroy objects |
| `steal` | Steal from characters |
| `buy` / `sell` / `list` / `value` | Shop transactions |
| `heal` | Buy healing services |
| `brew` / `scribe` | Craft potions/scrolls |
| `lore` | Identify items via skill |

### Combat (`fight.c`)

| Command | Description |
|---|---|
| `kill` / `murder` | Initiate combat |
| `flee` | Attempt to escape combat |
| `rescue` | Rescue an ally |
| `backstab` | Thief sneak attack |
| `circle` | Circle attack during combat |
| `bash` | Shield bash (stun) |
| `kick` | Kick during combat |
| `dirt` | Dirt kick (blind) |
| `trip` | Trip opponent (stun) |
| `berserk` | Enter berserker rage |
| `disarm` | Remove opponent's weapon |
| `blackjack` | Knock out an opponent |
| `slay` / `mortslay` | Instant kill (immortal/PK) |

### Magic (`magic.c`, `skills.c`)

| Command | Description |
|---|---|
| `cast` | Cast a spell |
| `spellup` | Auto-cast beneficial spells |
| `spells` / `splist` | List available/known spells |
| `skills` / `sklist` | List available/known skills |
| `groups` | List skill groups |
| `gain` | Learn skills from a trainer |
| `weapon` / `shield` | Cleave attacks |

### Immortal Commands (`act_wiz.c`)

See the [API Reference](api-reference.md) for a complete list. Key categories:

- **World Management**: `goto`, `transfer`, `at`, `repop`, `peace`, `purge`
- **Character Management**: `advance`, `trust`, `restore`, `freeze`, `jail`, `deny`
- **Information**: `stat`, `mstat`, `ostat`, `rstat`, `sockets`, `vnum`, `mfind`, `ofind`, `mwhere`, `owhere`
- **Object/Mob Loading**: `load`, `mload`, `oload`, `clone`
- **String Editing**: `string`, `set`, `mset`, `oset`, `rset`, `sset`
- **Server Control**: `reboot`, `shutdown`, `copyover`, `wizlock`, `newlock`, `chaos`
- **Monitoring**: `snoop`, `switch`, `return`, `log`, `echo`, `recho`, `pecho`
- **OLC Entry**: `aedit`, `redit`, `oedit`, `medit`, `mpedit`, `mpgedit`
- **Permissions**: `wizgrant`, `wizrevoke`, `disable`, `allow`, `ban`, `permban`
- **Utilities**: `force`, `repeat`, `outfit`, `award`, `rlist`, `objcheck`, `aexits`, `aentrances`

## Social Commands

Socials are custom emote commands loaded from `socials.are`. If a typed command is not found in `cmd_table[]`, `check_social()` searches the social list.

Each social has messages for 8 contexts:
1. No argument (performer sees)
2. No argument (room sees)
3. Target found (performer sees)
4. Target found (room sees)
5. Target found (victim sees)
6. Target not found (performer sees)
7. Self-targeted (performer sees)
8. Self-targeted (room sees)

Socials are editable in-game via the social OLC editor (`socialedit`).

## Alias System

Players can define up to `MAX_ALIAS` (20) command aliases:

- `alias <name> <expansion>` -- Define an alias
- `unalias <name>` -- Remove an alias
- `alias` (no args) -- List all aliases

Alias expansion happens in `substitute_alias()` before command interpretation. Aliases can reference other aliases (single level of expansion).
