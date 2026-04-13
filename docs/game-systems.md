# Game Systems

## Clan System (`src/clan.c`)

Written by Kyle Boyd. A full clan/guild system with membership management, ranks, PvP tracking, clan funding, and OLC editing.

### Data Structure

Each clan (`CLAN_DATA`) contains:
- Identity: number, name, whoname, leader, god (sponsor)
- Members: up to `MAX_CLAN_MEMBERS` (50), tracked by name
- PvP tracking: kills and deaths arrays per opposing clan
- Ranks: up to `MAX_RANK` (10) named ranks
- Settings: join flags, min level, max members, cost to join
- Economy: clan fund (gold pool)
- Location: recall room vnum

### Player Commands

| Command | Description |
|---|---|
| `clan list` | List all clans |
| `clan info <clan>` | Detailed clan information |
| `clan members` | View member roster |
| `clan log` | View clan activity log |
| `join <clan>` | Join a clan (requirements vary by join flags) |
| `petition <clan>` | Petition to join an invite-only clan |
| `resign` | Leave your clan |
| `accept <player>` | Leader accepts a petitioner |
| `decline <player>` | Leader declines a petitioner |
| `offer <player>` | Leader offers membership |
| `promote <player>` | Leader promotes a member |
| `demote <player>` | Leader demotes a member |
| `clantalk <message>` | Clan-private chat channel |
| `cdeposit <amount>` | Deposit gold into the clan fund |
| `crecall` | Teleport to clan recall room |
| `roster` | View the member list |
| `show` | Show clan info |

### Join Flags

| Flag | Description |
|---|---|
| `free_join` | Anyone meeting requirements can join |
| `invite_only` | Must be invited/offered by the leader |
| `petition` | Must petition and be accepted |
| `cost` | Requires gold payment to join |

### OLC Editing

`cedit <clan#>` opens the clan editor with subcommands for: create, leader, sponsor, name, whoname, minlevel, members, recall, joinflags, clanflags, rank, cost.

### Persistence

Clans are saved to a single file (`clans.are`) with sections for each clan including member lists, kill/death statistics, and rank names.

## Faction System (`src/factions.c`)

A reputation system where player standing with factions changes based on gameplay actions, primarily killing NPCs.

### How Factions Work

1. **Faction definitions** have a vnum, name, and custom increase/decrease messages
2. **Mob faction affects** associate mobs with factions: "killing this mob changes faction X by Y amount"
3. When a player kills a mob, `affect_factions()` applies all of the mob's faction affects to the killer and their group
4. Faction standings are stored per-player as a linked list of `FACTIONPC_DATA`
5. Standings are clamped to min/max values

### Effects of Faction Standing

- **Shop prices**: `faction_cost_multiplier()` adjusts buy/sell prices based on standing with the shopkeeper's faction. Hated = 2x buy price, loved = 0.8x buy price.
- **Consider**: `consider_factions()` shows how killing a mob would affect your faction standings
- **Display**: `show_faction_standings()` shows all standings with color-coded percentage bars

### Standing Levels

| Value Range | Label |
|---|---|
| Very high | Loved |
| High | Liked |
| Moderate | Amiable |
| Low positive | Indifferent |
| Low negative | Apprehensive |
| Moderate negative | Disliked |
| High negative | Hated |
| Very high negative | Despised |

### OLC Editing

- `factionedit <vnum>` -- Edit faction definitions (name, increase/decrease messages)
- `medit faction <args>` -- Edit faction affects on mobs (which factions change on kill)
- `factionfind` -- List all factions

### MudProg Integration

`mpchangefaction <player> <faction_vnum> <value>` allows scripts to directly modify faction standings.

## Auction System (`src/auction.c`)

Written by Brian Babey. A timed item auction system with automatic progression through bidding stages.

### Flow

1. Player puts an item up for auction: `auction <item>`
2. System announces the auction start to all players
3. `auction_update()` runs every `PULSE_AUCTION` (10 seconds), incrementing the status counter
4. At `AUCTION_LENGTH - 2`: If no bids, item is removed and returned to owner
5. At `AUCTION_LENGTH - 1`: "Going twice!" announcement
6. At `AUCTION_LENGTH`: Sale completes -- gold transferred, item delivered

### Commands

| Command | Description |
|---|---|
| `auction` | Toggle auction channel on/off |
| `auction <item>` | Put an item up for auction |
| `auction bid <amount>` | Place a bid |
| `auction info` | View detailed item stats |
| `auction stop` | Cancel auction (owner or immortal) |

### Rules

- One auction at a time (global)
- Cannot auction no-drop, cursed, or clan-restricted items
- Minimum bid increment (`BID_INCREMENT`, default 1)
- Gold is held from the bidder until outbid or auction ends
- Previous bidder's held gold is refunded when outbid

## Banking System (`src/bank.c`)

Written by Gothar (v1.2, 1998). Location-based gold banking with class-specific access points.

### Rooms

- **Midgaard Bank** (`ROOM_VNUM_BANK`): Available to non-thief classes
- **Thieves' Fence** (`ROOM_VNUM_BANK_THIEF`): Available to thieves only

### Commands

| Command | Description |
|---|---|
| `account` | View on-hand gold and banked gold |
| `deposit <amount>` | Deposit gold into bank |
| `withdraw <amount>` | Withdraw gold from bank |
| `bank` | Show banking help |

### Restrictions

- Must be in the appropriate banking room
- NPCs, pets, and immortals cannot use banking
- Thieves cannot use the regular bank; non-thieves cannot use the fence

## Message Board System (`src/board.c`)

Written by Erwin S. Andreasen (1995-96). Multi-board persistent note/message system.

### Boards

| Board | Read Level | Write Level | Default | Expiry |
|---|---|---|---|---|
| General | 0 | 2 | Include "all" | 21 days |
| Ideas | 0 | 2 | Normal | 60 days |
| Announce | 0 | Immortal | Normal | 60 days |
| Bugs | 0 | 1 | "imm" only | 60 days |
| Personal | 0 | 1 | Exclude | 28 days |

### Commands

| Command | Description |
|---|---|
| `note read <#>` | Read a specific note |
| `note list` | List notes (marks new/old) |
| `note write` | Begin composing a note |
| `note remove <#>` | Delete a note (owner or immortal) |
| `note catchup` | Mark all notes as read |
| `board <name>` | Switch to a different board |

### Note Composition

When writing a note, the player enters a multi-step process:
1. `To:` -- Set recipient(s)
2. `Subject:` -- Set subject line
3. `Expires in [days]:` -- Set expiration
4. Enter text line by line (supports word wrap via `do_wordwrap`, find/replace via `do_replace`)
5. `~` or `END` to finish text
6. Choose: Post / Continue editing / View / Forget / Word wrap / Replace

### Persistence

Notes are stored as individual files per board in the notes directory. They survive server restarts.

## Marriage System (`src/marry.c`)

Simple immortal-administered marriage system.

### Commands

| Command | Who | Description |
|---|---|---|
| `marry <player1> <player2>` | Immortal | Marry two consenting players |
| `divorce <player1> <player2>` | Immortal | Divorce two consenting players |
| `consent` | Player | Toggle consent flag (required for marry/divorce) |
| `spousetalk <message>` | Player | Private channel with spouse |

### Requirements

- Both players must be online
- Both must have PLR_CONSENT set
- Neither can already be married (for marry)
- Must be married to each other (for divorce)

## Remort System (`src/remort.c`)

Endgame progression system allowing max-level characters to restart with benefits.

### Process

1. Character must be at `LEVEL_HERO`
2. Type `remort` for warning message and confirmation
3. Type `remort` again to confirm
4. Character is extracted from the game world
5. Player file is deleted
6. A fresh character is created with the same name and password
7. `incarnations` counter is incremented
8. `PLR_REMORT` flag is set
9. Player enters race selection (may have access to remort-only races/classes)

### Benefits

- Access to remort-only races and classes (defined in `race_table[]` and `class_table[]` with `remort_race`/`remort_class` flags)
- Incarnation counter tracks number of remorts
- Access to `herotalk` channel

## Class Skill System (`src/class.c`)

Written by Erwin S. Andreasen. Allows online modification of which level each class gains access to each skill.

### The `askill` Command

`askill <class_who_name> <skill_name> <level> <cost>`

- `class_who_name`: 3-letter class abbreviation (Mag, Cle, Thi, War)
- `skill_name`: Skill or spell name
- `level`: Level at which the class gains access (1-100, or LEVEL_IMMORTAL to disable)
- `cost`: Creation point cost (rating)

Changes are saved to class files in the class directory and persist across reboots.

## Random Object Generation (`src/random.c`)

See [Combat and Magic - Random Object Generation](combat.md#random-object-generation-srcrandomc).

## Drunk Speech (`src/drunk2.c`)

When a player's `COND_DRUNK` condition exceeds letter-specific thresholds, their speech is garbled. Each letter has a minimum drunk level and several possible replacements (e.g., 's' might become 'zzZzssZ').

Drunk speech can be enabled/disabled per channel via `config.h` defines (e.g., `GOS_DRUNK`, `SAY_DRUNK`).

## Site Banning (`src/ban.c`)

### Ban Types

| Type | Effect |
|---|---|
| `all` | Block all connections from the site |
| `newbies` | Block new character creation from the site |
| `permit` | Only allow connections from the site if player has a permit |

### Wildcard Matching

- `*example.com` -- Suffix match (prefix wildcard)
- `example*` -- Prefix match (suffix wildcard)
- `*example*` -- Infix match (both wildcards)

### Commands

| Command | Description |
|---|---|
| `ban` | List current bans |
| `ban <site> [type]` | Add a temporary ban |
| `permban <site> [type]` | Add a permanent ban |
| `allow <site>` | Remove a ban |
| `permit <player>` | Allow a player to connect from a banned site |

Permanent bans persist across reboots via the ban file.
