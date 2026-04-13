# Networking and Communication

The networking layer is implemented in `src/comm.c` and handles all TCP socket I/O, the main game loop, the login state machine, output formatting, ANSI color translation, and the hot-reboot (copyover) system.

## Entry Point

`main()` (or `embermain()` on Windows) performs initial setup:

1. Sets the time seed and opens a reserved file handle (`fpReserve`)
2. Parses command-line arguments: port number and optional copyover flag
3. Calls `init_socket()` to create the listening TCP socket
4. Calls `boot_db()` to load the entire game world
5. If recovering from a copyover, calls `copyover_recover()`
6. Enters `game_loop()`

## Socket Initialization

`init_socket(int port)` creates a TCP listening socket:

- Creates a stream socket (`SOCK_STREAM`)
- Sets `SO_REUSEADDR` to allow quick restarts
- Binds to `INADDR_ANY` on the specified port
- Calls `listen()` with a backlog of 3
- Returns the socket file descriptor

Platform-specific: on Win32, uses Winsock (`WSAStartup`/`WSASocket`).

## Game Loop

`game_loop(int control)` is the heart of the server. See [Architecture](architecture.md) for the full loop description.

Key timing constants:

| Constant | Value | Description |
|---|---|---|
| `PULSE_PER_SECOND` | 4 | Ticks per real second |
| `PULSE_VIOLENCE` | 12 | Combat round (3 seconds) |
| `PULSE_MOBILE` | 16 | NPC AI update (4 seconds) |
| `PULSE_TICK` | 120 | Game tick (30 seconds) |
| `PULSE_AREA` | 240 | Area reset check (60 seconds, with jitter) |
| `PULSE_AUCTION` | 40 | Auction timer (10 seconds) |

## Connection Handling

### New Connections

`new_descriptor(int control)`:

1. Accepts the TCP connection via `accept()`
2. Creates a `DESCRIPTOR_DATA` structure
3. Performs reverse DNS lookup (unless `NO_RDNS` is defined)
4. Checks site bans via `check_ban()`
5. Sends the connection greeting (`CFG_CONNECT_MSG`)
6. Sets initial state to `CON_GET_ANSI`
7. Adds to the front of `descriptor_list`

### Input Processing

**`read_from_descriptor()`**: Reads raw bytes from the socket into `d->inbuf` using non-blocking I/O. Detects connection closure (returns FALSE) or overflow.

**`read_from_buffer()`**: Extracts one complete line from `d->inbuf` into `d->incomm`:
- Handles backspace characters
- Supports `!` to repeat the last command
- Shifts remaining data in `inbuf`

### Output Processing

**`write_to_buffer()`**: Appends text to the descriptor's output buffer (`d->outbuf`), expanding it up to `MAX_OUTPUT_BUFFER` (32KB). Respects the `silentmode` flag.

**`process_output()`**: For each descriptor ready for output:
1. If the character is playing, generates the prompt via `doparseprompt()`
2. Calls `write_to_descriptor()` to flush the buffer

**`write_to_descriptor()`**: The lowest-level output function:
1. Runs the text through `do_color()` to translate backtick color codes to ANSI escape sequences (or strip them if ANSI is disabled)
2. Writes to the socket via `send()` or `write()`

### Closing Connections

`close_socket(DESCRIPTOR_DATA *dclose)`:

1. Flushes any remaining output
2. Cleans up snoop references
3. If the character was playing, handles link-death (saves the character but leaves them in-game)
4. Removes from `descriptor_list`
5. Closes the socket file descriptor
6. Frees allocated memory

## Login State Machine (`nanny()`)

`nanny(DESCRIPTOR_DATA *d, char *argument)` handles all pre-game states. The flow is:

```
CON_GET_ANSI          Ask ANSI color preference
    |
CON_GET_NAME          Enter character name
    |
    +-- Existing character --> CON_GET_OLD_PASSWORD
    |                              |
    |                         CON_BREAK_CONNECT (if already playing)
    |                              |
    |                         CON_READ_MOTD --> CON_PLAYING
    |
    +-- New character --> CON_CONFIRM_NEW_NAME
                              |
                         CON_GET_NEW_PASSWORD
                              |
                         CON_CONFIRM_NEW_PASSWORD
                              |
                         CON_GET_NEW_RACE
                              |
                         CON_GET_NEW_SEX
                              |
                         CON_GET_NEW_CLASS
                              |
                         CON_GET_ALIGNMENT
                              |
                         CON_ROLL_STATS
                              |
                         CON_DEFAULT_CHOICE
                              |
                         CON_GEN_GROUPS (optional customization)
                              |
                         CON_PICK_WEAPON
                              |
                         CON_READ_IMOTD --> CON_READ_MOTD --> CON_PLAYING
```

When a character enters `CON_PLAYING`:
- The character is placed in their last room (or the default room)
- Login announcements are sent
- The character's equipment is re-applied via `reset_char()`
- Boards with unread notes are announced

## Prompt System

`doparseprompt(CHAR_DATA *ch)` substitutes tokens in the player's prompt string:

| Token | Expansion |
|---|---|
| `%h` / `%H` | Current / max hit points |
| `%m` / `%M` | Current / max mana |
| `%v` / `%V` | Current / max movement |
| `%x` / `%X` | Current XP / XP to next level |
| `%g` | Gold carried |
| `%a` | Alignment |
| `%r` | Current room name |
| `%e` | Visible exits |
| `%c` | Carriage return |
| `%n` | Character name |
| `%o` | OLC editor name (if editing) |
| `%O` | OLC vnum (if editing) |

The default prompt shows `<HP Mana Move>` values.

## ANSI Color Translation

`do_color(char *inbuf, int inlen, char *outbuf, int outlen, bool color)`:

Scans the input for backtick sequences and replaces them with ANSI escape codes (if `color` is TRUE) or strips them (if FALSE). See the color code table in [Configuration](configuration.md).

## Hot Reboot (Copyover)

The copyover system allows the MUD to restart without disconnecting players.

**`do_copyover()`** (Unix only):
1. Saves all connected players
2. Writes a temp file listing each descriptor's: file descriptor number, character name, and hostname
3. Calls `execl()` to replace the current process with the new binary, passing the copyover control socket and port as arguments

**`copyover_recover()`**:
1. Reads the temp file
2. For each entry, creates a new `DESCRIPTOR_DATA` using the saved file descriptor
3. Loads the character via `load_char_obj()`
4. Reconnects the character to the game
5. Sends a recovery message

## The `act()` Messaging System

`act(format, ch, arg1, arg2, type)` sends formatted messages to characters:

**Format tokens:**
- `$n` -- `ch`'s name (as seen by recipient)
- `$N` -- `arg2` character's name
- `$e/$E` -- he/she/it for `ch`/`arg2`
- `$m/$M` -- him/her/it for `ch`/`arg2`
- `$s/$S` -- his/her/its for `ch`/`arg2`
- `$p/$P` -- `arg1`/`arg2` object short description
- `$t/$T` -- `arg1`/`arg2` as text strings
- `$d` -- first word of `arg2` as a door name

**Target types:**
- `TO_CHAR` -- send to `ch` only
- `TO_VICT` -- send to `arg2` character only
- `TO_ROOM` -- send to everyone in room except `ch`
- `TO_NOTVICT` -- send to everyone in room except `ch` and `arg2`

`act_new()` is an extended version that also accepts a minimum position requirement and returns the formatted string.

## Shell System

Immortals with shell access can execute Unix commands from within the MUD:

1. `do_shell()` forks a child process
2. The child opens a pseudo-terminal via `master_pty()` / `slave_tty()` (from `pty.c`)
3. I/O between the socket and PTY is routed by `route_io()` (from `route_io.c`)
4. The parent tracks shell users in `shell_char_list`
5. Shell output is periodically checked via `can_read_descriptor()` and reaping is done in the game loop
