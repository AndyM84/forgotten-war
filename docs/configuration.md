# Configuration

EmberMUD has two layers of configuration: compile-time settings in `config.h` and runtime settings in `ember.cfg`.

## Compile-Time Configuration (`src/config.h`)

These settings require recompilation to change.

### Feature Toggles

| Define | Description |
|---|---|
| `USE_MUSIC` | Enable the music channel |
| `USE_ADMINTALK` | Enable the admin-only chat channel |
| `USE_HEROTALK` | Enable the hero/remort chat channel |
| `USE_GOCIAL` | Enable global socials (social emotes broadcast on a channel) |
| `USE_REMORT` | Enable the remort system (character rebirth at max level) |
| `USE_REBIRTH` | Enable the rebirth system (alternative to remort) |

### Channel Format Strings

Channel output formats use `$n` for the speaker's name and `$t` for the message text. Both variables must be present. Color codes use backtick notation (e.g., `` `W `` for white, `` `K `` for dark gray).

| Define | Default Format |
|---|---|
| `CFG_OOC` | `` `K[`WOOC`K]`w $n`K: `c'`C$t`c'`0 `` |
| `CFG_GOS` | `` `K[`WGOSSIP`K]`w $n`K: `b'`B$t`b'`0 `` |
| `CFG_QUESTION` | `` `K[`WQUESTION`K]`w $n`K: `r'`R$t`r'`0 `` |
| `CFG_ANSWER` | `` `K[`WANSWER`K]`w $n`K: `r'`R$t`r'`0 `` |
| `CFG_IMM` | `` `K[`WIMM`K]`w $n`K: `w'`W$t`w'`0 `` |
| `CFG_SAY` | `` `G$n says `g'`G$t`g'`0 `` |
| `CFG_YELL` | `` `Y$n yells `y'`Y$t`y'`0 `` |
| `CFG_SHOUT` | `` `W$n shouts `w'`W$t`w'`0 `` |

### Channel Names

| Define | Default | Description |
|---|---|---|
| `CFG_GOS_NAME` | `"gossip"` | Name of the gossip channel |
| `CFG_OOC_NAME` | `"ooc"` | Name of the OOC channel |
| `CFG_QUESTION_NAME` | `"question"` | Name of the question channel |
| `CFG_ANSWER_NAME` | `"answer"` | Name of the answer channel |

### Drunk Speech Toggles

Each channel can optionally apply drunk speech garbling. Define these to enable:

| Define | Channel |
|---|---|
| `GOS_DRUNK` | Gossip |
| `OOC_DRUNK` | OOC |
| `SAY_DRUNK` | Say |
| `YELL_DRUNK` | Yell |
| `SHOUT_DRUNK` | Shout |
| `CLANTALK_DRUNK` | Clan talk |
| `HEROTALK_DRUNK` | Hero talk |
| `MUSIC_DRUNK` | Music |
| `GTELL_DRUNK` | Group tell |
| `ADMINTALK_DRUNK` | Admin talk |
| `IMMTALK_DRUNK` | Immortal talk |

### Movement Configuration

| Define | Default | Description |
|---|---|---|
| `DRUNK_MOVE` | (defined) | Enable random direction movement when drunk |
| `TRACK_IS_SKILL` | (optional) | Make tracking a learnable skill vs. always available |
| `TRACK_THROUGH_DOORS` | (optional) | Allow the track algorithm to path through closed doors |

### Auction Configuration

| Define | Default | Description |
|---|---|---|
| `AUCTION_LENGTH` | `5` | Number of ticks before auction closes |
| `MIN_POS_AUCTION` | `POS_SLEEPING` | Minimum position to participate in auction |
| `BID_INCREMENT` | `1` | Minimum bid increase |
| `CFG_AUC_*` | (various) | Auction message format strings |

### Auto-Hate System

| Define | Description |
|---|---|
| `AUTO_HATE` | If defined, mobs automatically hate (remember and re-attack) players who attack them |

### Game Constants

| Define | Default | Description |
|---|---|---|
| `MAX_LEVEL` | `100` | Maximum achievable level |
| `LEVEL_HERO` | `91` | Hero (mortal cap) level |
| `LEVEL_IMMORTAL` | `92` | First immortal level |
| `MAX_ATTAINABLE_STATS` | `25` or `30` | Maximum stat value (changes stat bonus tables) |
| `MAX_SKILL` | `150` | Maximum number of skills/spells |
| `MAX_GROUP` | `30` | Maximum number of skill groups |
| `MAX_CLASS` | `4` | Number of character classes |
| `MAX_STATS` | `5` | Number of stats (STR, INT, WIS, DEX, CON) |
| `MAX_CLAN` | `25` | Maximum number of clans |
| `MAX_CLAN_MEMBERS` | `50` | Maximum members per clan |
| `MAX_RANK` | `10` | Maximum clan ranks |
| `MAX_ALIAS` | `20` | Maximum aliases per character |
| `MAX_SOCIALS` | `256` | Maximum social commands |
| `MAX_IN_GROUP` | `15` | Maximum skills per skill group |
| `MAX_INPUT_LENGTH` | `256` | Maximum input line length |
| `MAX_OUTPUT_BUFFER` | `32768` | Maximum output buffer size |
| `MAX_NEST` | `100` | Maximum container nesting depth |
| `PULSE_PER_SECOND` | `4` | Game loop ticks per second |
| `PULSE_VIOLENCE` | `3 * PULSE_PER_SECOND` | Combat round frequency |
| `PULSE_MOBILE` | `4 * PULSE_PER_SECOND` | NPC AI update frequency |
| `PULSE_TICK` | `30 * PULSE_PER_SECOND` | Game tick frequency (30 seconds) |
| `PULSE_AREA` | `60 * PULSE_PER_SECOND` | Area reset check frequency |
| `PULSE_AUCTION` | `10 * PULSE_PER_SECOND` | Auction update frequency |

### Connection Messages

| Define | Default | Description |
|---|---|---|
| `CFG_QUIT` | `"Alas, all good things must come to an end.\n\r"` | Message on quit |
| `CFG_CONNECT_MSG` | `"Welcome to a MUD based on EmberMUD.\n\r"` | Initial connection message |
| `CFG_ASK_ANSI` | `"Use ANSI Color? [Y/n]: "` | ANSI color prompt |

### Room VNUMs

| Define | Description |
|---|---|
| `ROOM_VNUM_LIMBO` | Limbo room (default for idle players) |
| `ROOM_VNUM_TEMPLE` | Default recall room |
| `ROOM_VNUM_ALTAR` | Altar room (respawn point) |
| `ROOM_VNUM_SCHOOL` | Mud School start room |
| `ROOM_VNUM_DONATION` | Donation pit room |
| `ROOM_VNUM_BANK` | Banking room |
| `ROOM_VNUM_BANK_THIEF` | Thief banking room (fence) |
| `ROOM_VNUM_JAIL` | Jail room |

### Object VNUMs

| Define | Description |
|---|---|
| `OBJ_VNUM_MONEY_ONE` | Single gold coin object |
| `OBJ_VNUM_MONEY_SOME` | Pile of gold coins object |
| `OBJ_VNUM_CORPSE_NPC` | NPC corpse |
| `OBJ_VNUM_CORPSE_PC` | Player corpse |
| `OBJ_VNUM_SEVERED_HEAD` | Severed head (death cry) |
| `OBJ_VNUM_TORN_HEART` | Torn heart (death cry) |
| `OBJ_VNUM_PORTAL` | Magic portal |
| `OBJ_VNUM_SPRING` | Created spring |
| `OBJ_VNUM_PIT` | Donation pit |
| `OBJ_VNUM_MUSHROOM` | Created food |
| `OBJ_VNUM_SCHOOL_*` | Starting equipment vnums |
| `RAND_VNUM_ARMOR` | Random armor template (30) |
| `RAND_VNUM_RING` | Random ring template (31) |
| `RAND_VNUM_WEAPON` | Random weapon template (33) |
| `RAND_VNUM_BAG` | Random bag template (34) |

### OLC Configuration

| Define | Default | Description |
|---|---|---|
| `HELP_EDIT_LEVEL` | `L3` | Minimum trust to edit help files |
| `TODO_EDIT_LEVEL` | `L3` | Minimum trust to edit todo entries |

## Runtime Configuration (`src/ember.cfg`)

The `ember.cfg` file is read at boot by `load_config_file()` in `db.c`. It uses a `KEY value` format. The configuration populates the global `sysconfig` structure.

### Directory Settings

| Key | Default | Description |
|---|---|---|
| `AreaDir` | `../area/` | Path to area data files |
| `PlayerDir` | `../player/` | Path to player save files |
| `PlayerTemp` | `../player/` | Temp directory for safe saves |
| `GodDir` | `../gods/` | Path to immortal data files |
| `ClanDir` | `../clan/` | Path to clan data and logs |
| `NoteDir` | `../notes/` | Path to board message files |
| `LogDir` | `../log/` | Path to server logs |
| `ClassDir` | `../classes/` | Path to class skill files |

### File Settings

| Key | Default | Description |
|---|---|---|
| `AreaList` | `area.lst` | Master area list file |
| `HelpFile` | `helps.are` | Help entries file |
| `TodoFile` | `todo.are` | Todo entries file |
| `MudProgsFile` | `mudprogs.are` | MudProg definitions file |
| `BugFile` | `bugs.txt` | Bug report log |
| `IdeaFile` | `ideas.txt` | Idea log |
| `TypoFile` | `typos.txt` | Typo report log |
| `ShutdownFile` | `shutdown.txt` | Shutdown message file |
| `ChaosFile` | `chaos.txt` | Chaos mode message file |
| `ClansFile` | `clans.are` | Clan data file |
| `FactionsFile` | `factions.are` | Faction definitions file |
| `SocialsFile` | `socials.are` | Social commands file |
| `HelpLogFile` | `helplog.txt` | Missing help topic log |
| `BanFile` | `ban.txt` | Banned sites file |
| `DisableFile` | `disabled.txt` | Disabled commands file |

## ANSI Color Codes

EmberMUD uses backtick-letter sequences for color:

| Code | Color | Code | Color |
|---|---|---|---|
| `` `k `` | Dark black | `` `K `` | Bright black (dark gray) |
| `` `r `` | Dark red | `` `R `` | Bright red |
| `` `g `` | Dark green | `` `G `` | Bright green |
| `` `y `` | Dark yellow | `` `Y `` | Bright yellow |
| `` `b `` | Dark blue | `` `B `` | Bright blue |
| `` `m `` | Dark magenta | `` `M `` | Bright magenta |
| `` `c `` | Dark cyan | `` `C `` | Bright cyan |
| `` `w `` | Dark white (gray) | `` `W `` | Bright white |
| `` `0 `` | Reset to default | | |

Color translation happens in `do_color()` in `comm.c` at output time. Players without ANSI enabled have color codes stripped.
