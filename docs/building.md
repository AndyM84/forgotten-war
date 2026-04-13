# Building EmberMUD

## Prerequisites

### Linux / Unix

- GCC compiler
- `libcrypt` (password hashing)
- `libm` (math library)
- Standard POSIX development headers

### Windows

- Microsoft Visual C++ (project files in `src/MSVC/`), or
- Borland C++ Builder 3/4 (project files in `src/BCB3/` and `src/BCB4/`), or
- Cygwin with gcc

## Compiling on Linux

From the `src/` directory:

```bash
cd src
make
```

This produces the `ember` executable in the `src/` directory.

### Makefile Details

| Variable | Value | Description |
|---|---|---|
| `CC` | `gcc` | C compiler |
| `PROF` | `-DLINUX` | Platform define |
| `C_FLAGS` | `-Wall -ggdb -O3 $(PROF)` | Compile flags: all warnings, debug symbols, optimization |
| `L_FLAGS` | `-O2 $(PROF)` | Linker flags |
| Libraries | `-lm -lcrypt` | Math and crypt libraries |

### Makefile Targets

| Target | Description |
|---|---|
| `make` / `make all` | Build the `ember` executable |
| `make clean` | Remove object files and temp files |
| `make distclean` | Remove area backups, player files, logs, god files |
| `make src` | Create a `src.tar.gz` archive of source files |
| `make diff` | Generate a patch file against `./original` directory |
| `make update` | Copy current source to `./original` for future diffing |

### Source Files Compiled

The Makefile compiles these object files (in link order):

```
act_comm.o  act_info.o  act_move.o  act_obj.o  act_wiz.o
auction.o   ban.o       bank.o      bit.o      board.o
comm.o      const.o     clan.o      class.o    db.o
drunk2.o    factions.o  fight.o     handler.o  helpolc.o
interp.o    magic.o     marry.o     mem.o      mprog_commands.o
mprog_procs.o  mud_progs.o  olc.o   olc_act.o  olc_save.o
newbits.o   pty.o       random.o    recycle.o  route_io.o
save.o      skills.o    socialolc.o ssm.o      string.o
todoolc.o   update.o    remort.o
```

## Compile-Time Defines

Key preprocessor defines that affect compilation:

| Define | Description |
|---|---|
| `LINUX` | Linux/Unix platform (set by Makefile) |
| `WIN32` | Windows platform |
| `cbuilder` | Borland C++ Builder platform |
| `NOCRYPT` | Disable password encryption (stores passwords in plain text) |
| `NO_RDNS` | Disable reverse DNS lookups on new connections |
| `TRADITIONAL` | Support pre-ANSI C compilers |
| `CPP` | C++ compilation mode |

## Running the Server

### Starting

From the project root directory:

```bash
cd src
./startup &
```

The `startup` script runs the MUD in a loop, automatically restarting after crashes or reboots. It accepts an optional port number argument (default is set in the startup script).

Alternatively, run directly:

```bash
cd src
./ember <port>
```

The server must be run from the `src/` directory so it can find relative paths to the `area/`, `player/`, and other data directories (unless paths are configured in `ember.cfg`).

### Connecting

Connect with any MUD client or raw telnet:

```bash
telnet localhost <port>
```

### Shutting Down

From within the MUD (as an implementor):

- `shutdown` -- Graceful shutdown
- `reboot` -- Restart the server (the `startup` script will relaunch it)
- `copyover` -- Hot reboot: saves all players, re-execs the binary, and reconnects players without disconnection (Unix only)

## File Dependencies

All `.c` files depend on `merc.h`, which includes:

- `config.h` -- compile-time configuration
- `board.h` -- board system types
- `factions.h` -- faction system types
- `newbits.h` -- extended bitfield types

Some files additionally include:

- `olc.h` -- OLC system (olc.c, olc_act.c, olc_save.c, clan.c, factions.c, socialolc.c, helpolc.c, todoolc.c, string.c, bit.c, mem.c, act_wiz.c)
- `interp.h` -- command table (interp.c, act_wiz.c, class.c, mprog_commands.c, save.c)
- `magic.h` -- spell functions (magic.c, fight.c, skills.c, const.c, act_obj.c)
- `mud_progs.h` -- script system (mud_progs.c, mprog_commands.c, mprog_procs.c)
- `db.h` -- database externs (random.c, olc_act.c)
- `recycle.h` -- buffer/ban recycling (recycle.c, ban.c)
