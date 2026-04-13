# API Reference

Complete function reference for the EmberMUD codebase, organized by source file. All player-facing commands follow the signature `void do_xxx(CHAR_DATA *ch, char *argument)`. All spell functions follow `void spell_xxx(int sn, int level, CHAR_DATA *ch, void *vo)`.

## Core Engine

### `src/comm.c` -- Network I/O and Game Loop

| Function | Description |
|---|---|
| `int main(int argc, char **argv)` | Entry point, calls `embermain()` |
| `int embermain(int argc, char **argv)` | Initialize sockets, boot database, enter game loop |
| `int init_socket(int port)` | Create and bind the listening TCP socket |
| `int game_loop(int control)` | Main `select()` loop: accept connections, read input, run updates, write output |
| `void new_descriptor(int control)` | Accept new TCP connection, allocate `DESCRIPTOR_DATA` |
| `void close_socket(DESCRIPTOR_DATA *dclose)` | Close connection, extract character, clean up descriptor |
| `bool read_from_descriptor(DESCRIPTOR_DATA *d, bool color)` | Read raw bytes from socket into input buffer |
| `void read_from_buffer(DESCRIPTOR_DATA *d, bool color)` | Extract one command line from input buffer |
| `bool process_output(DESCRIPTOR_DATA *d, bool fPrompt)` | Flush output buffer to socket, prepend prompt |
| `void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, int length)` | Append text to descriptor output buffer |
| `bool write_to_descriptor(int desc, char *txt, int length, bool color)` | Write directly to socket file descriptor |
| `void nanny(DESCRIPTOR_DATA *d, char *argument)` | Login state machine (name, password, race, class, etc.) |
| `bool check_parse_name(char *name)` | Validate character name (length, characters, reserved words) |
| `bool check_reconnect(DESCRIPTOR_DATA *d, char *name, bool fConn)` | Reconnect to existing character in-game |
| `bool check_playing(DESCRIPTOR_DATA *d, char *name)` | Check if character is already connected |
| `void stop_idling(CHAR_DATA *ch)` | Move idle character back from limbo |
| `void send_to_char(const char *txt, CHAR_DATA *ch)` | Send text to a character's connection |
| `void printf_to_char(CHAR_DATA *ch, char *fmt, ...)` | Send formatted text to a character |
| `void page_to_char(const char *txt, CHAR_DATA *ch)` | Send pageable text (for long output) |
| `void show_string(struct descriptor_data *d, char *input)` | Page through long text output |
| `void act(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type)` | Send formatted action message with `$n/$N/$e/$m/$s/$p/$t` tokens |
| `char *act_new(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type, int min_pos)` | Extended act() with minimum position requirement |
| `char *act_string(const char *format, CHAR_DATA *to, CHAR_DATA *ch, const void *arg1, const void *arg2)` | Format act() string without sending |
| `void do_color(register char *inbuf, int inlen, register char *outbuf, int outlen, bool color)` | Translate backtick color codes to ANSI escape sequences |
| `char *doparseprompt(CHAR_DATA *ch)` | Expand prompt tokens (`%h`, `%m`, `%v`, etc.) |
| `char *figurestate(int current, int max)` | Return health state string (e.g., "excellent", "awful") |
| `char *damstatus(CHAR_DATA *ch)` | Return damage status for prompt |
| `int roll_stat(int base_bonus, int stat_max)` | Roll a stat value during character creation |
| `void fix_sex(CHAR_DATA *ch)` | Fix pronoun references based on character sex |
| `void sigchld_handler(int sig)` | Handle SIGCHLD for child process cleanup |

### `src/db.c` -- Database and World Loading

| Function | Description |
|---|---|
| `int boot_db(void)` | Master boot sequence: load areas, fix references, initialize systems |
| `void load_area(FILE *fp)` | Load `#AREADATA` section from area file |
| `void load_mobiles(FILE *fp)` | Load `#MOBILES` section (mob prototypes) |
| `void load_objects(FILE *fp)` | Load `#OBJECTS` section (object prototypes) |
| `void load_rooms(FILE *fp)` | Load `#ROOMS` section (room definitions with exits) |
| `void load_resets(FILE *fp)` | Load `#RESETS` section (spawn/equip instructions) |
| `void load_shops(FILE *fp)` | Load `#SHOPS` section (shopkeeper data) |
| `void load_helps(FILE *fp)` | Load help entries |
| `void load_todo(FILE *fp)` | Load developer todo entries |
| `void load_mudprogs(FILE *fp)` | Load MudProg script definitions |
| `void load_progs(FILE *fp)` | Load prog assignments |
| `void fix_exits(void)` | Resolve exit vnum references to room pointers (post-load) |
| `void area_update(void)` | Increment area ages, trigger resets when due |
| `void reset_room(ROOM_INDEX_DATA *pRoom)` | Execute all resets for a room (spawn mobs, place objects) |
| `void reset_area(AREA_DATA *pArea)` | Reset all rooms in an area |
| `void assign_area_vnum(int vnum)` | Assign a vnum to an area during loading |
| `void skip_section(FILE *fp, char *section)` | Skip unknown file section during loading |
| `void new_reset(ROOM_INDEX_DATA *pR, RESET_DATA *pReset)` | Add a reset to a room's reset list |
| `void clone_mobile(CHAR_DATA *parent, CHAR_DATA *clone)` | Deep-copy mob data for instancing |
| `void clone_object(OBJ_DATA *parent, OBJ_DATA *clone)` | Deep-copy object data for instancing |
| `void clear_char(CHAR_DATA *ch)` | Zero-initialize a character structure |
| `void free_char(CHAR_DATA *ch)` | Free all memory associated with a character |
| `char *get_extra_descr(const char *name, EXTRA_DESCR_DATA *ed)` | Look up an extra description by keyword |
| `void *alloc_mem(int sMem)` | Allocate temporary memory |
| `void free_mem(void *pMemPtr)` | Free temporary memory |
| `void *alloc_perm(int sMem)` | Allocate permanent memory (never freed) |
| `int load_config_file(void)` | Load `ember.cfg` runtime settings |
| `char *get_config_value(char *inbuf, char *outbuf)` | Parse key=value from config line |
| `void do_copyover(CHAR_DATA *ch, char *argument)` | Hot reboot: save state, `execl()` new process |
| `void init_descriptor(DESCRIPTOR_DATA *dnew, int desc)` | Initialize descriptor from raw socket fd |
| `void copyover_recover(void)` | Restore connections after hot reboot |
| `void maxfilelimit(void)` | Set process file descriptor limit to maximum |
| `void do_areas(CHAR_DATA *ch, char *argument)` | List all areas |
| `void do_memory(CHAR_DATA *ch, char *argument)` | Display memory usage statistics |
| `void do_dump(CHAR_DATA *ch, char *argument)` | Dump debug statistics |

#### File I/O Primitives

| Function | Description |
|---|---|
| `char fread_letter(FILE *fp)` | Read one non-whitespace character |
| `int fread_number(FILE *fp)` | Read an integer (supports `|` for bitwise OR) |
| `void fread_to_eol(FILE *fp)` | Skip to end of line |
| `char *fread_word(FILE *fp)` | Read one word (whitespace-delimited) |
| `char *get_word(FILE *fp)` | Read one word (alternate implementation) |

#### Utility Functions

| Function | Description |
|---|---|
| `int number_fuzzy(int number)` | Return number +/- 1 randomly |
| `int number_range(int from, int to)` | Random integer in range [from, to] |
| `int number_percent(void)` | Random integer 1-99 |
| `int number_door(void)` | Random direction 0-5 |
| `int number_bits(int width)` | Random value with `width` bits |
| `int dice(int number, int size)` | Roll `number`d`size` dice |
| `int interpolate(int level, int value_00, int value_32)` | Linear interpolation by level |
| `void init_mm(void)` | Initialize random number generator |
| `int number_mm(void)` | Raw random number (Marsaglia-Mullins) |
| `void smash_tilde(char *str)` | Replace `~` with `-` in string |
| `bool str_cmp(const char *astr, const char *bstr)` | Case-insensitive string comparison |
| `bool str_prefix(const char *astr, const char *bstr)` | Case-insensitive prefix match |
| `bool str_infix(const char *astr, const char *bstr)` | Case-insensitive substring search |
| `bool str_suffix(const char *astr, const char *bstr)` | Case-insensitive suffix match |
| `char *capitalize(const char *str)` | Return string with first letter capitalized |
| `char *str_str(char *str1, char *str2)` | Case-insensitive substring search |
| `char *str_upr(char *str)` | Convert string to uppercase |
| `void append_file(CHAR_DATA *ch, char *file, char *str)` | Append timestamped line to log file |
| `void bug(const char *str, ...)` | Log error message |
| `void bug_trace(const char *str)` | Log error with stack trace |
| `void log_string(const char *str)` | Log informational message |
| `void logf_string(const char *str, ...)` | Log formatted message |
| `void update_last(char *line1, char *line2, char *line3)` | Update crash-recovery "last action" log |
| `void tail_chain(void)` | No-op (debug sentinel) |
| `int mprog_name_to_type(char *name)` | Convert MudProg trigger name to type constant |

### `src/handler.c` -- Entity Manipulation

#### Lookup and Query

| Function | Description |
|---|---|
| `int material_lookup(const char *name)` | Look up material index by name |
| `char *material_name(sh_int num)` | Get material name from index |
| `int race_lookup(const char *name)` | Look up race index by name |
| `int class_lookup(const char *name)` | Look up class index by name |
| `int check_immune(CHAR_DATA *ch, int dam_type)` | Check immunity/resistance/vulnerability to damage type |
| `int get_skill(CHAR_DATA *ch, int sn)` | Get effective skill percentage |
| `int get_weapon_sn(CHAR_DATA *ch)` | Get primary weapon's skill number |
| `int get_second_weapon_sn(CHAR_DATA *ch)` | Get dual-wield weapon's skill number |
| `int get_weapon_skill(CHAR_DATA *ch, int sn)` | Get weapon skill percentage |
| `int get_trust(CHAR_DATA *ch)` | Get effective trust level (considers switched) |
| `int get_age(CHAR_DATA *ch)` | Get character age in MUD years |
| `int get_curr_stat(CHAR_DATA *ch, int stat)` | Get current stat value (base + modifiers, capped) |
| `int get_max_train(CHAR_DATA *ch, int stat)` | Get maximum trainable stat value |
| `int can_carry_n(CHAR_DATA *ch)` | Get max number of items character can carry |
| `int can_carry_w(CHAR_DATA *ch)` | Get max weight character can carry |
| `bool is_full_name(const char *str, char *namelist)` | Full word match against name list |
| `bool is_exact_name(char *str, char *namelist)` | Exact match against name list |
| `bool is_name(char *str, char *namelist)` | Prefix match against name list |
| `bool can_see(CHAR_DATA *ch, CHAR_DATA *victim)` | Can character see another character? |
| `bool can_see_obj(CHAR_DATA *ch, OBJ_DATA *obj)` | Can character see an object? |
| `bool can_see_room(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex)` | Can character see in a room? |
| `bool can_drop_obj(CHAR_DATA *ch, OBJ_DATA *obj)` | Can character drop an object? |
| `bool room_is_dark(ROOM_INDEX_DATA *pRoomIndex)` | Is room dark (considering light sources)? |
| `bool room_is_private(ROOM_INDEX_DATA *pRoomIndex)` | Is room at capacity? |
| `bool has_racial_skill(CHAR_DATA *ch, long sn)` | Does character have a racial skill? |
| `bool can_use(CHAR_DATA *ch, long sn)` | Can character use a skill? |
| `bool can_practice(CHAR_DATA *ch, long sn)` | Can character practice a skill? |
| `int count_obj_list(OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list)` | Count instances of object prototype in list |
| `int count_users(OBJ_DATA *obj)` | Count characters using furniture object |

#### Character Management

| Function | Description |
|---|---|
| `void reset_char(CHAR_DATA *ch)` | Recalculate all derived stats from base + equipment + affects |
| `void char_from_room(CHAR_DATA *ch)` | Remove character from their current room |
| `void char_to_room(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex)` | Place character in a room |
| `void extract_char(CHAR_DATA *ch, bool fPull)` | Remove character from game world entirely |
| `void pk_extract_char(CHAR_DATA *ch, bool fPull)` | PK-specific character extraction |
| `void fix_sex(CHAR_DATA *ch)` | Fix pronoun for character's sex |

#### Affect System

| Function | Description |
|---|---|
| `void affect_modify(CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd)` | Apply or remove stat modification from an affect |
| `void affect_to_char(CHAR_DATA *ch, AFFECT_DATA *paf)` | Add an affect to a character |
| `void affect_remove(CHAR_DATA *ch, AFFECT_DATA *paf)` | Remove an affect from a character |
| `void affect_strip(CHAR_DATA *ch, int sn)` | Remove all affects of a given spell |
| `bool is_affected(CHAR_DATA *ch, int sn)` | Check if character has a specific affect |
| `void affect_join(CHAR_DATA *ch, AFFECT_DATA *paf)` | Add affect, combining with existing if same type |
| `void affect_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)` | Add an affect to an object |
| `void affect_remove_obj(OBJ_DATA *obj, AFFECT_DATA *paf)` | Remove an affect from an object |
| `void newaffect_modify(CHAR_DATA *ch, NEWAFFECT_DATA *npaf, bool fAdd)` | Apply/remove extended affect |
| `void newaffect_to_char(CHAR_DATA *ch, NEWAFFECT_DATA *npaf)` | Add extended affect to character |
| `void newaffect_remove(CHAR_DATA *ch, NEWAFFECT_DATA *npaf)` | Remove extended affect from character |
| `void newaffect_strip(CHAR_DATA *ch, int sn)` | Remove all extended affects of type |
| `bool is_newaffected(CHAR_DATA *ch, int sn)` | Check for extended affect |
| `void newaffect_join(CHAR_DATA *ch, NEWAFFECT_DATA *npaf)` | Combine extended affects |

#### Object Management

| Function | Description |
|---|---|
| `void obj_to_char(OBJ_DATA *obj, CHAR_DATA *ch)` | Add object to character inventory |
| `void obj_from_char(OBJ_DATA *obj)` | Remove object from character inventory |
| `void obj_to_room(OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex)` | Place object in room |
| `void obj_from_room(OBJ_DATA *obj)` | Remove object from room |
| `void obj_to_obj(OBJ_DATA *obj, OBJ_DATA *obj_to)` | Put object in container |
| `void obj_from_obj(OBJ_DATA *obj)` | Remove object from container |
| `void extract_obj(OBJ_DATA *obj)` | Delete object from game |
| `int apply_ac(OBJ_DATA *obj, int iWear, int type)` | Calculate AC contribution for wear slot |
| `void equip_char(CHAR_DATA *ch, OBJ_DATA *obj, int iWear)` | Equip object, apply affects |
| `void unequip_char(CHAR_DATA *ch, OBJ_DATA *obj)` | Unequip object, remove affects |
| `int get_obj_number(OBJ_DATA *obj)` | Get total item count (including contents) |
| `int get_obj_weight(OBJ_DATA *obj)` | Get total weight (including contents) |

#### Name Formatting

| Function | Description |
|---|---|
| `char *get_curdate(void)` | Current real-world date as string |
| `char *get_curtime(void)` | Current real-world time as string |
| `char *get_date(time_t tm)` | Format timestamp as date |
| `char *get_time(time_t tm)` | Format timestamp as time |
| `char *item_type_name(OBJ_DATA *obj)` | Item type as string ("weapon", "armor", etc.) |
| `char *affect_loc_name(int location)` | Affect location as string ("strength", "hitroll", etc.) |
| `char *affect_bit_name(int vector)` | AFF_ flags as space-separated string |
| `char *extra_bit_name(int extra_flags)` | ITEM_ flags as string |
| `char *act_bit_name(int act_flags)` | ACT_ flags as string |
| `char *comm_bit_name(int comm_flags)` | COMM_ flags as string |
| `char *imm_bit_name(int imm_flags)` | IMM_ flags as string |
| `char *wear_bit_name(int wear_flags)` | WEAR_ flags as string |
| `char *form_bit_name(int form_flags)` | FORM_ flags as string |
| `char *part_bit_name(int part_flags)` | PART_ flags as string |
| `char *weapon_bit_name(int weapon_flags)` | Weapon flags as string |
| `char *off_bit_name(int off_flags)` | OFF_ flags as string |

### `src/update.c` -- Periodic Game Updates

| Function | Description |
|---|---|
| `void update_handler(void)` | Master update dispatcher, called every pulse |
| `void advance_level(CHAR_DATA *ch)` | Award HP/mana/move/practices on level-up |
| `void gain_exp(CHAR_DATA *ch, int gain)` | Add experience, announce level-ups |
| `int hit_gain(CHAR_DATA *ch)` | Calculate HP regeneration per tick |
| `int mana_gain(CHAR_DATA *ch)` | Calculate mana regeneration per tick |
| `int move_gain(CHAR_DATA *ch)` | Calculate movement regeneration per tick |
| `void gain_condition(CHAR_DATA *ch, int iCond, int value)` | Update hunger/thirst/drunk |
| `void mobile_update(void)` | NPC AI: wandering, hunting, scavenging, path following |
| `void weather_update(void)` | Update weather patterns and time of day |
| `void char_update(void)` | Tick-based character updates: affect expiry, regen, conditions |
| `void regen_update(void)` | Fast regeneration update (every 2 pulses) |
| `void obj_update(void)` | Object decay timers, corpse rot |
| `void aggr_update(void)` | Check aggressive mobs for targets |

### `src/interp.c` -- Command Interpreter

| Function | Description |
|---|---|
| `void interpret(CHAR_DATA *ch, char *argument)` | Parse and dispatch a player command |
| `void chk_command(CHAR_DATA *ch, char *argument)` | Process command with logging |
| `void mpinterpret(CHAR_DATA *ch, char *argument)` | Interpret command from MudProg context |
| `bool check_social(CHAR_DATA *ch, char *command, char *argument)` | Check if command matches a social |
| `bool is_immcmd(char *command)` | Is this an immortal-only command? |
| `bool can_do_immcmd(CHAR_DATA *ch, char *cmd)` | Does character have this immortal command? |
| `bool is_number(char *arg)` | Is string a valid number? |
| `int number_argument(char *argument, char *arg)` | Parse `N.keyword` syntax |
| `char *one_argument(char *argument, char *arg_first)` | Extract one word, lowercase |
| `char *one_argument2(char *argument, char *arg_first)` | Extract one word, preserve case |
| `void do_commands(CHAR_DATA *ch, char *argument)` | List available commands |
| `void do_wizhelp(CHAR_DATA *ch, char *argument)` | List available wizard commands |
| `void do_disable(CHAR_DATA *ch, char *argument)` | Enable/disable commands at runtime |
| `bool check_disabled(const struct cmd_type *command)` | Check if command is disabled |
| `void load_disabled(void)` | Load disabled command list from file |
| `void save_disabled(void)` | Save disabled command list to file |

### `src/save.c` -- Character Persistence

| Function | Description |
|---|---|
| `void save_char_obj(CHAR_DATA *ch)` | Save character, equipment, and pets to file |
| `void fwrite_char(CHAR_DATA *ch, FILE *fp)` | Write character data in tagged key-value format |
| `void fwrite_pet(CHAR_DATA *pet, FILE *fp)` | Write pet data |
| `void fwrite_obj(CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest)` | Write object data with nesting depth |
| `bool load_char_obj(DESCRIPTOR_DATA *d, char *name)` | Load character from player file |
| `void fread_char(CHAR_DATA *ch, FILE *fp)` | Read character data from tagged format |
| `void fread_imm(CHAR_DATA *ch, FILE *fp)` | Read immortal-specific data |
| `void fread_pet(CHAR_DATA *ch, FILE *fp)` | Read pet data |
| `void fread_obj(CHAR_DATA *ch, FILE *fp)` | Read object data |
| `char *print_flags(int flag)` | Convert flags to printable string |

## Memory Management

### `src/ssm.c` -- Shared String Manager

| Function | Description |
|---|---|
| `void init_string_space()` | Initialize shared string heap |
| `char *str_dup(const char *str)` | Allocate or reference-count a string in shared space |
| `void free_string(char **strptr)` | Decrement reference count, free when zero |
| `char *fread_string(FILE *fp)` | Read tilde-delimited string from file into shared space |
| `char *fread_string_eol(FILE *fp)` | Read string to end of line into shared space |
| `int temp_fread_string_eol(FILE *fp, char *outbuf)` | Read string to EOL into user buffer |
| `void temp_fread_string(FILE *fp, char *outbuf)` | Read tilde-delimited string into user buffer |
| `int defrag_heap()` | Merge adjacent free blocks in shared string heap |
| `char *temp_hash_find(const char *str)` | Look up string in boot-time hash table |
| `void temp_hash_add(char *str, const int len)` | Add string to boot-time hash table |
| `void boot_done(void)` | Free boot-time hash table |

### `src/mem.c` -- Typed Free-List Allocators

| Function | Description |
|---|---|
| `RESET_DATA *new_reset_data(void)` | Allocate reset command |
| `void free_reset_data(RESET_DATA *pReset)` | Recycle reset command |
| `MPROG_DATA *new_mudprog(void)` | Allocate MudProg script |
| `void free_mudprog(MPROG_DATA *mprog)` | Recycle MudProg script |
| `MPROG_GROUP *new_mudprog_group(void)` | Allocate MudProg group |
| `void free_mudprog_group(MPROG_GROUP *pMprogGroup)` | Recycle MudProg group |
| `AREA_DATA *new_area(void)` | Allocate area structure |
| `void free_area(AREA_DATA *pArea)` | Recycle area structure |
| `EXIT_DATA *new_exit(void)` | Allocate exit structure |
| `void free_exit(EXIT_DATA *pExit)` | Recycle exit structure |
| `EXTRA_DESCR_DATA *new_extra_descr(void)` | Allocate extra description |
| `void free_extra_descr(EXTRA_DESCR_DATA *pExtra)` | Recycle extra description |
| `ROOM_INDEX_DATA *new_room_index(void)` | Allocate room template |
| `void free_room_index(ROOM_INDEX_DATA *pRoom)` | Recycle room template (cascading) |
| `AFFECT_DATA *new_affect(void)` | Allocate classic affect |
| `void free_affect(AFFECT_DATA *pAf)` | Recycle classic affect |
| `NEWAFFECT_DATA *new_newaffect(void)` | Allocate extended affect |
| `void free_newaffect(NEWAFFECT_DATA *npAf)` | Recycle extended affect |
| `SHOP_DATA *new_shop(void)` | Allocate shop definition |
| `void free_shop(SHOP_DATA *pShop)` | Recycle shop definition |
| `OBJ_INDEX_DATA *new_obj_index(void)` | Allocate object template |
| `void free_obj_index(OBJ_INDEX_DATA *pObj)` | Recycle object template (cascading) |
| `MOB_INDEX_DATA *new_mob_index(void)` | Allocate mob template |
| `void free_mob_index(MOB_INDEX_DATA *pMob)` | Recycle mob template (cascading) |
| `char *remove_color(const char *str)` | Strip backtick color codes from string |

### `src/recycle.c` -- Validated Recyclables and Buffers

| Function | Description |
|---|---|
| `BAN_DATA *new_ban(void)` | Allocate ban structure with validation |
| `void free_ban(BAN_DATA *ban)` | Recycle ban structure (checks IS_VALID) |
| `BUFFER *new_buf(void)` | Allocate dynamic buffer (default size) |
| `BUFFER *new_buf_size(int size)` | Allocate dynamic buffer with specific size |
| `void free_buf(BUFFER *buffer)` | Recycle buffer |
| `bool add_buf(BUFFER *buffer, char *string)` | Append string to buffer, auto-resize |
| `void clear_buf(BUFFER *buffer)` | Reset buffer to empty |
| `char *buf_string(BUFFER *buffer)` | Get buffer content pointer |
| `int get_size(int val)` | Find buffer size bucket for value |

## Player Commands

### `src/act_comm.c` -- Communication

| Function | Description |
|---|---|
| `do_gossip` | Global chat channel |
| `do_ooc` | Out-of-character channel |
| `do_music` | Music channel |
| `do_question` / `do_answer` | Q&A channel pair |
| `do_immtalk` | Immortal-only channel |
| `do_admintalk` | Admin-only channel |
| `do_herotalk` | Hero/remort channel |
| `do_say` | Speak to current room |
| `do_yell` | Area-wide message |
| `do_shout` | Area-wide message with cooldown |
| `do_tell` / `do_reply` | Private messaging |
| `do_gtell` | Group chat |
| `do_spousetalk` | Spouse private channel |
| `do_clantalk` | Clan private channel (in `clan.c`) |
| `do_emote` | Custom action message |
| `do_channels` | List channel on/off status |
| `do_quiet` | Toggle all channels |
| `do_deaf` | Toggle shout hearing |
| `do_telloff` | Block tell messages |
| `do_last` / `do_lastimm` / `do_lastadmin` / `do_lasthero` | View channel history |
| `do_messages` / `do_tq` | View queued tells |
| `do_beep` | Send beep to another player |
| `do_sendinfo` | Send information broadcast |
| `do_bug` / `do_idea` / `do_typo` | Submit reports |
| `do_follow` | Follow another character |
| `do_group` | Manage group membership |
| `do_order` | Order followers |
| `do_split` | Split gold among group |
| `do_quit` | Quit the game |
| `do_save` | Save character |
| `do_delete` | Delete character permanently |

#### Internal Helpers

| Function | Description |
|---|---|
| `add_follower(CHAR_DATA *ch, CHAR_DATA *master)` | Add character as follower |
| `stop_follower(CHAR_DATA *ch)` | Stop following |
| `nuke_pets(CHAR_DATA *ch)` | Remove all pets |
| `die_follower(CHAR_DATA *ch)` | Handle follower death |
| `bool is_same_group(CHAR_DATA *ach, CHAR_DATA *bch)` | Check group membership |
| `add2queue(...)` / `add2tell(...)` | Message queuing |
| `add2last(...)` / `add2last_imm(...)` / `add2last_admin(...)` / `add2last_hero(...)` | Channel history |

### `src/act_info.c` -- Information

| Function | Description |
|---|---|
| `do_look` / `do_examine` | View rooms, objects, characters |
| `do_read` | Read objects or boards |
| `do_exits` | Show available exits |
| `do_scan` | Scan adjacent rooms |
| `do_score` | Full character sheet |
| `do_effects` | Active affects list |
| `do_levels` | XP requirements table |
| `do_worth` | Gold and XP display |
| `do_who` / `do_cwho` | Online player lists |
| `do_finger` | View player info |
| `do_inventory` / `do_equipment` | View carried/worn items |
| `do_compare` | Compare two items |
| `do_where` | Locate characters |
| `do_consider` | Assess target difficulty |
| `do_time` / `do_weather` | Game time and weather |
| `do_help` / `do_todo` | Help system |
| `do_practice` | Train skills |
| `do_prompt` | Customize prompt |
| `do_scroll` | Set page length |
| `do_alias` / `do_unalias` | Manage aliases |
| `do_title` / `do_description` / `do_email` | Set character info |
| `do_password` | Change password |
| `do_wimpy` | Set auto-flee threshold |
| `do_afk` / `do_anonymous` / `do_pk` | Toggle status flags |
| `do_autolist` / `do_autoloot` / `do_autogold` / `do_autosac` / `do_autosplit` / `do_autoassist` / `do_autoexit` | Auto-action toggles |
| `do_brief` / `do_compact` / `do_combine` | Display toggles |
| `do_nofollow` / `do_nosummon` / `do_noloot` / `do_nocolor` | Protection toggles |
| `do_search` | Search for hidden items |
| `do_levelgain` | Level up |
| `do_report` | Announce HP/mana/move to room |
| `do_rebirth` | Rebirth character |
| `substitute_alias(DESCRIPTOR_DATA *d, char *argument)` | Expand aliases before interpretation |

#### Internal Helpers

| Function | Description |
|---|---|
| `char *format_obj_to_char(OBJ_DATA *obj, CHAR_DATA *ch, bool fShort)` | Format object for display |
| `void show_char_to_char_0(...)` | Show character in brief |
| `void show_char_to_char_1(...)` | Show character in detail |
| `void show_char_to_char(...)` | Show character list |
| `bool check_blind(CHAR_DATA *ch)` | Check blindness |
| `char *statdiff(int normal, int modified)` | Format stat differences |
| `char *who_clan(...)` | Format clan info for who list |
| `void insert_sort(...)` / `void chaos_sort(...)` | Sort who list |
| `void eval_dir(...)` / `void show_dir_mobs(...)` | Scan helpers |

### `src/act_move.c` -- Movement

| Function | Description |
|---|---|
| `void move_char(CHAR_DATA *ch, int door, bool follow)` | Core movement logic |
| `do_north` / `do_east` / `do_south` / `do_west` / `do_up` / `do_down` | Directional movement |
| `do_open` / `do_close` | Open/close doors and containers |
| `do_lock` / `do_unlock` | Lock/unlock with keys |
| `do_pick` | Pick locks |
| `do_stand` / `do_sit` / `do_rest` / `do_sleep` / `do_wake` | Change position |
| `do_sneak` / `do_hide` / `do_visible` | Stealth commands |
| `do_recall` | Teleport to recall point |
| `do_beacon` / `do_beaconreset` | Set/reset personal recall |
| `do_train` | Train stats at trainer |
| `do_track` | BFS pathfinding |
| `int find_door(CHAR_DATA *ch, char *arg)` | Find door by keyword |
| `bool has_key(CHAR_DATA *ch, int key)` | Check for key in inventory |
| `bool check_web(CHAR_DATA *ch)` | Check if webbed |
| `bool can_go(CHAR_DATA *ch, int dir)` | Check if direction passable |
| `int find_first_step(ROOM_INDEX_DATA *src, ROOM_INDEX_DATA *target)` | BFS pathfinding |

### `src/act_obj.c` -- Object Manipulation

| Function | Description |
|---|---|
| `do_get` / `do_put` / `do_drop` / `do_give` | Move objects |
| `do_donate` | Donate to pit |
| `do_wear` / `do_remove` | Equip/unequip |
| `do_eat` / `do_drink` / `do_fill` / `do_pour` | Consume food and drink |
| `do_quaff` / `do_recite` / `do_brandish` / `do_zap` | Use magic items |
| `do_sacrifice` / `do_junk` | Destroy objects |
| `do_steal` | Steal from characters |
| `do_buy` / `do_sell` / `do_list` / `do_value` | Shop transactions |
| `do_heal` | Buy healing |
| `do_brew` / `do_scribe` | Craft potions/scrolls |
| `do_lore` | Identify items |
| `do_enter` / `do_portal` | Enter portals |
| `bool can_loot(CHAR_DATA *ch, OBJ_DATA *obj)` | Check loot permission |
| `void get_obj(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container)` | Core get logic |
| `bool remove_obj(CHAR_DATA *ch, int iWear, bool fReplace)` | Core remove logic |
| `void wear_obj(CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace)` | Core wear logic |
| `int get_cost(CHAR_DATA *ch, CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy)` | Calculate shop price |
| `bool shopkeeper_kick_ch(...)` | Remove character from shop |

## Combat and Magic

### `src/fight.c` -- Combat System

| Function | Description |
|---|---|
| `void violence_update(void)` | Per-round combat processing for all fighters |
| `void multi_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt)` | Calculate and execute all attacks per round |
| `void mob_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt)` | NPC combat AI (area attacks, special abilities) |
| `void one_hit(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *weapon, int dt)` | Single attack: THAC0 roll, damage, weapon effects |
| `bool check_parry(CHAR_DATA *ch, CHAR_DATA *victim)` | Parry defense check |
| `bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim)` | Dodge defense check |
| `bool check_block(CHAR_DATA *ch, CHAR_DATA *victim)` | Shield block defense check |
| `void update_pos(CHAR_DATA *victim)` | Update position from HP |
| `void set_fighting(CHAR_DATA *ch, CHAR_DATA *victim)` | Initiate combat |
| `void stop_fighting(CHAR_DATA *ch, bool fBoth)` | End combat |
| `bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)` | PvP safety check |
| `bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area)` | Spell safety check |
| `void check_killer(CHAR_DATA *ch, CHAR_DATA *victim)` | Flag as PKer |
| `void check_assist(CHAR_DATA *ch, CHAR_DATA *victim)` | Have allies join fight |
| `void make_corpse(CHAR_DATA *ch)` | Create NPC corpse with inventory |
| `void make_pk_corpse(CHAR_DATA *ch)` | Create PC corpse |
| `void death_cry(CHAR_DATA *ch)` | Death announcement and body parts |
| `void raw_kill(CHAR_DATA *victim)` | Standard NPC death handler |
| `void pk_kill(CHAR_DATA *victim)` | PK death handler |
| `void chaos_kill(CHAR_DATA *victim)` | Chaos mode death (no real death) |
| `void group_gain(CHAR_DATA *ch, CHAR_DATA *victim)` | Distribute XP to group |
| `void disarm(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *target_weapon)` | Disarm logic |
| `bool is_hating(CHAR_DATA *ch, CHAR_DATA *victim)` | Check hate memory |
| `void start_hating(CHAR_DATA *ch, CHAR_DATA *victim)` | Add to hate memory |
| `void stop_hating(CHAR_DATA *ch)` | Clear hate memory |

#### Combat Skill Commands

`do_backstab`, `do_circle`, `do_bash`, `do_kick`, `do_dirt`, `do_trip`, `do_berserk`, `do_disarm`, `do_blackjack`, `do_kill`, `do_murder`, `do_flee`, `do_rescue`, `do_slay`, `do_mortslay`, `do_weapon`, `do_shield`

### `src/magic.c` -- Spell System

| Function | Description |
|---|---|
| `int skill_lookup(const char *name)` | Find spell/skill by name |
| `int slot_lookup(int slot)` | Find spell by slot number |
| `void say_spell(CHAR_DATA *ch, int sn)` | Generate spell incantation text |
| `bool saves_spell(int level, CHAR_DATA *victim)` | Saving throw vs. spell |
| `bool saves_dispel(int dis_level, int spell_level, int duration)` | Saving throw vs. dispel |
| `bool check_dispel(int dis_level, CHAR_DATA *victim, int sn)` | Attempt to dispel a specific spell |
| `int mana_cost(CHAR_DATA *ch, int min_mana, int level)` | Calculate mana cost |
| `void do_cast(CHAR_DATA *ch, char *argument)` | Cast a spell |
| `void do_spellup(CHAR_DATA *ch, char *argument)` | Auto-cast beneficial spells |
| `void do_mpsilentcast(CHAR_DATA *ch, char *argument)` | Silent cast from MudProg |
| `void obj_cast_spell(int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)` | Cast from item |
| `void spawn_portal(int vnum1, int vnum2)` | Create portal between rooms |

#### Spell Functions

All follow: `void spell_xxx(int sn, int level, CHAR_DATA *ch, void *vo)`

**Offensive**: `acid_blast`, `burning_hands`, `call_lightning`, `chain_lightning`, `chill_touch`, `colour_spray`, `demonfire`, `disintegrate`, `dispel_evil`, `earthquake`, `energy_drain`, `fireball`, `firewind`, `flamestrike`, `harm`, `high_explosive`, `holyfire`, `holy_word`, `ice_ray`, `ice_storm`, `lightning_bolt`, `magic_missile`, `meteor_swarm`, `multi_missile`, `shocking_grasp`

**Breath**: `acid_breath`, `fire_breath`, `frost_breath`, `gas_breath`, `lightning_breath`

**Defensive/Buff**: `armor`, `bless`, `change_sex`, `fly`, `frenzy`, `giant_strength`, `haste`, `infravision`, `invis`, `mass_invis`, `pass_door`, `protection`, `regeneration`, `sanctuary`, `shield`, `stone_skin`

**Healing**: `cure_blindness`, `cure_critical`, `cure_disease`, `cure_light`, `cure_poison`, `cure_serious`, `heal`, `mass_healing`, `refresh`, `restoration`

**Debuff**: `blindness`, `calm`, `charm_person`, `curse`, `plague`, `poison`, `sleep`, `web`, `weaken`

**Utility**: `cancellation`, `continual_light`, `control_weather`, `create_food`, `create_spring`, `create_water`, `detect_evil`, `detect_hidden`, `detect_invis`, `detect_magic`, `detect_poison`, `dispel_magic`, `enchant_armor`, `enchant_weapon`, `energize`, `gate`, `identify`, `imprint`, `knock`, `know_alignment`, `locate_object`, `nexus`, `remove_align`, `remove_curse`, `summon`, `teleport`, `ventriloquate`, `vision`, `word_of_recall`

**Other**: `general_purpose`, `null`, `test_area`

### `src/skills.c` -- Skill System

| Function | Description |
|---|---|
| `void check_improve(CHAR_DATA *ch, int sn, bool success, int multiplier)` | Skill improvement on use |
| `void do_gain(CHAR_DATA *ch, char *argument)` | Learn skills from trainer |
| `void do_spells(CHAR_DATA *ch, char *argument)` | List known spells |
| `void do_splist(CHAR_DATA *ch, char *argument)` | List available spells |
| `void do_skills(CHAR_DATA *ch, char *argument)` | List known skills |
| `void do_sklist(CHAR_DATA *ch, char *argument)` | List available skills |
| `void do_groups(CHAR_DATA *ch, char *argument)` | Manage skill groups |
| `void list_group_costs(CHAR_DATA *ch)` | Show group costs at creation |
| `void list_group_chosen(CHAR_DATA *ch)` | Show chosen groups at creation |
| `bool parse_gen_groups(CHAR_DATA *ch, char *argument)` | Parse group selection during creation |
| `int group_lookup(const char *name)` | Find group by name |
| `void gn_add(CHAR_DATA *ch, int gn)` | Add group to character |
| `void gn_remove(CHAR_DATA *ch, int gn)` | Remove group from character |
| `void group_add(CHAR_DATA *ch, const char *name, bool deduct)` | Add skill/group |
| `void group_remove(CHAR_DATA *ch, const char *name)` | Remove skill/group |

## Immortal Commands (`src/act_wiz.c`)

| Function | Description |
|---|---|
| `do_goto` | Teleport to room or player |
| `do_transfer` | Teleport a player to you |
| `do_at` | Execute command at remote location |
| `do_repop` | Force area reset |
| `do_peace` | Stop all fighting in room |
| `do_purge` | Remove NPCs/objects from room |
| `do_advance` | Set player level |
| `do_trust` | Set player trust level |
| `do_restore` | Restore HP/mana/move to max |
| `do_freeze` | Freeze player (cannot act) |
| `do_jail` | Send player to jail room |
| `do_deny` | Ban player from game |
| `do_pardon` | Pardon a denied player |
| `do_outfit` | Give starter equipment |
| `do_award` | Award XP or items |
| `do_stat` / `do_mstat` / `do_ostat` / `do_rstat` | View detailed stats |
| `do_vnum` / `do_mfind` / `do_ofind` / `do_rfind` / `do_mpfind` | Find by name |
| `do_mwhere` / `do_owhere` | Locate in world |
| `do_mload` / `do_oload` / `do_load` | Spawn mobs/objects |
| `do_clone` | Clone mob or object |
| `do_set` / `do_mset` / `do_oset` / `do_rset` / `do_sset` | Set properties |
| `do_string` | Edit string properties |
| `do_setedit` | Edit character settings |
| `do_slookup` | Look up spell/skill |
| `do_reboot` / `do_shutdown` | Server control |
| `do_snoop` | Monitor player connection |
| `do_switch` / `do_return` | Possess/unpossess mob |
| `do_force` | Force player to execute command |
| `do_repeat` | Repeat command N times |
| `do_echo` / `do_recho` / `do_pecho` | Send messages |
| `do_invis` | Set wizard invisibility |
| `do_holylight` | Toggle omniscient vision |
| `do_sockets` | List active connections |
| `do_wizlock` / `do_newlock` | Lock server |
| `do_disconnect` / `do_new_discon` | Disconnect player |
| `do_nochannels` / `do_noemote` / `do_noshout` / `do_notell` | Restrict player communication |
| `do_log` | Toggle command logging for player |
| `do_permit` | Grant permission to player |
| `do_wizgrant` / `do_wizrevoke` | Grant/revoke immortal commands |
| `do_pload` / `do_punload` | Load/unload offline player |
| `do_rlist` | List rooms in vnum range |
| `do_olevel` / `do_mlevel` | Find objects/mobs by level |
| `do_objcheck` | Check area objects for issues |
| `do_aexits` / `do_aentrances` | List area exits/entrances |
| `do_chaos` | Toggle chaos (PvP arena) mode |
| `do_shell` | Execute shell command |
| `do_setprog` | Set program flags |
| `do_bamfin` / `do_bamfout` | Set teleport messages |
| `bool obj_check(CHAR_DATA *ch, OBJ_DATA *obj)` | Validate object integrity |
| `void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)` | Deep-clone object with contents |

## OLC System

### `src/olc.c` -- Editor Core

| Function | Description |
|---|---|
| `bool run_olc_editor(DESCRIPTOR_DATA *d)` | Dispatch input to active editor |
| `char *olc_ed_name(CHAR_DATA *ch)` | Get current editor type name |
| `void show_olc_cmds(CHAR_DATA *ch, const struct olc_cmd_type *olc_table)` | Show editor commands |
| `bool edit_done(CHAR_DATA *ch)` | Exit editor |
| `AREA_DATA *get_area_data(int vnum)` | Find area by vnum |
| `void display_resets(CHAR_DATA *ch)` | Show room resets |
| `void add_reset(ROOM_INDEX_DATA *room, RESET_DATA *pReset, int index)` | Insert reset |
| `int get_new_mprog_vnum(void)` | Generate next MudProg vnum |

Entry commands: `do_aedit`, `do_redit`, `do_oedit`, `do_medit`, `do_mpedit`, `do_mpgedit`, `do_olc`, `do_resets`, `do_alist`

### `src/olc_act.c` -- Editor Actions

Area editor, room editor, object editor, mobile editor, MudProg editor, and MudProg group editor sub-commands. Key helpers:

| Function | Description |
|---|---|
| `AREA_DATA *get_vnum_area(int vnum)` | Find area containing a vnum |
| `bool change_exit(CHAR_DATA *ch, char *argument, int door)` | Modify room exit |
| `void show_obj_values(CHAR_DATA *ch, OBJ_INDEX_DATA *obj)` | Display object values |
| `bool set_obj_values(...)` | Set object type-specific values |
| `void show_mprog(CHAR_DATA *ch, MPROG_DATA *pMudProg)` | Display MudProg |
| `void show_mpgroup(CHAR_DATA *ch, MPROG_GROUP *pGroup)` | Display MudProg group |
| `void show_flag_cmds(CHAR_DATA *ch, const struct flag_type *flag_table)` | List flag options |
| `bool check_range(int lower, int upper)` | Validate vnum range |

### `src/olc_save.c` -- Area File Writing

| Function | Description |
|---|---|
| `void save_area(AREA_DATA *pArea)` | Write complete area file |
| `void save_area_list(void)` | Write `area.lst` |
| `void save_mobile(FILE *fp, MOB_INDEX_DATA *pMobIndex)` | Write one mob prototype |
| `void save_mobiles(FILE *fp, AREA_DATA *pArea)` | Write all mobs in area |
| `void save_object(FILE *fp, OBJ_INDEX_DATA *pObjIndex)` | Write one object prototype |
| `void save_objects(FILE *fp, AREA_DATA *pArea)` | Write all objects in area |
| `void save_rooms(FILE *fp, AREA_DATA *pArea)` | Write all rooms in area |
| `void save_resets(FILE *fp, AREA_DATA *pArea)` | Write reset commands |
| `void save_door_resets(FILE *fp, AREA_DATA *pArea)` | Write door state resets |
| `void save_shops(FILE *fp, AREA_DATA *pArea)` | Write shop data |
| `void save_mudprogs_area(FILE *fp, AREA_DATA *pArea)` | Write MudProgs for area |
| `void save_helps(void)` | Write all help entries |
| `void save_todo(void)` | Write all todo entries |
| `void save_mudprogs(void)` | Write all MudProg definitions |
| `void do_asave(CHAR_DATA *ch, char *argument)` | Save command handler |
| `char *fwrite_flag(long flags, char buf[])` | Convert flags to compact alphabetic format |
| `char *fix_string(const char *str)` | Clean string for file output |

## MudProg Scripting

### `src/mud_progs.c` -- Script Engine

| Function | Description |
|---|---|
| `void mprog_driver(char *prog)` | Execute a MudProg script |
| `char *parse_script(char *script)` | Parse script into commands |
| `char *parse_if(char *instring)` | Parse if/elseif/else/endif blocks |
| `int parse_expression(char *instring)` | Evaluate conditional expression |
| `int parse_proc(char *proc)` | Parse procedure call |
| `int exec_proc(char *procname, int intarg, char *chararg)` | Execute procedure |
| `void *mprog_get_actor(char *arg, char type)` | Resolve $-variable to entity |
| `char *parse_command(char *instring)` | Parse single command line |
| `void parse_command_var(char var, char *outbuf)` | Expand $-variable in command |
| `void init_supermob(void)` | Create SuperMob for obj/room prog execution |
| `void set_supermob(void *source, int prog_type)` | Configure SuperMob for execution |
| `void release_supermob(void)` | Reset SuperMob after execution |
| `int evaluate(char *line, double *val)` | Evaluate mathematical expression |

#### Trigger Functions

| Function | Fires When |
|---|---|
| `mprog_act_trigger` | Matching act() message |
| `mprog_bribe_trigger` | Player gives gold |
| `mprog_death_trigger` | Mob dies |
| `mprog_entry_trigger` | Mob enters room |
| `mprog_fight_trigger` | Each combat round |
| `mprog_fightgroup_trigger` | Group combat round |
| `mprog_give_trigger` | Player gives object |
| `mprog_greet_trigger` | Player enters room |
| `mprog_hitprcnt_trigger` | HP below threshold |
| `mprog_random_trigger` | Random chance per tick |
| `mprog_speech_trigger` | Player speaks |
| `mprog_command_trigger` | Player enters command |
| `rprog_act_trigger` / `rprog_enter_trigger` / `rprog_leave_trigger` / `rprog_sleep_trigger` / `rprog_rest_trigger` / `rprog_rfight_trigger` / `rprog_death_trigger` / `rprog_speech_trigger` / `rprog_random_trigger` | Room triggers |
| `oprog_greet_trigger` / `oprog_speech_trigger` / `oprog_random_trigger` / `oprog_wear_trigger` / `oprog_use_trigger` / `oprog_remove_trigger` / `oprog_sac_trigger` / `oprog_get_trigger` / `oprog_damage_trigger` / `oprog_repair_trigger` / `oprog_drop_trigger` / `oprog_examine_trigger` / `oprog_zap_trigger` / `oprog_act_trigger` / `oprog_hit_trigger` | Object triggers |

### `src/mprog_commands.c` -- Script Action Commands

| Function | Description |
|---|---|
| `do_mpkill` | Attack a target |
| `do_mpjunk` | Destroy objects |
| `do_mpecho` / `do_mpechoat` / `do_mpechoaround` | Send messages |
| `do_mpasound` | Sound to adjacent rooms |
| `do_mpmload` / `do_mpoload` | Load mob/object |
| `do_mppurge` | Remove entities |
| `do_mpgoto` | Teleport mob |
| `do_mpat` | Execute at location |
| `do_mptransfer` | Teleport player |
| `do_mpforce` / `do_mpsilentforce` / `do_mpdosilent` | Force actions |
| `do_mpdefault` | Force through MudProg interpreter |
| `do_mpremember` / `do_mpforget` | Memory management |
| `do_mpinvis` | Set wizard invisibility |
| `do_mpfollowpath` | Follow encoded path |
| `do_mpeatcorpse` | Consume corpses |
| `do_mpclean` | Pick up junk |
| `do_mprandomsocial` | Random social emote |
| `do_mpstat` / `do_mobstat` / `do_roomstat` / `do_objstat` | Show prog stats |
| `char *mprog_type_to_name(int type)` | Trigger type to string |

### `src/mprog_procs.c` -- Script Procedures

All procedures follow: `int mprog_xxx(void *vo)` returning an integer value.

`mprog_alignment`, `mprog_clan`, `mprog_class`, `mprog_crimethief`, `mprog_faction`, `mprog_fightinroom`, `mprog_getrand`, `mprog_goldamount`, `mprog_hasmemory`, `mprog_hitpercent`, `mprog_hour`, `mprog_immune`, `mprog_isawake`, `mprog_ischarmed`, `mprog_isequal`, `mprog_isfight`, `mprog_isfollow`, `mprog_isgood`, `mprog_isimmort`, `mprog_isname`, `mprog_isnpc`, `mprog_ispc`, `mprog_level`, `mprog_memory`, `mprog_mobvnum`, `mprog_objtype`, `mprog_objval0`, `mprog_objval1`, `mprog_objval2`, `mprog_objval3`, `mprog_objvnum`, `mprog_position`, `mprog_rand`, `mprog_roomvnum`, `mprog_sex`, `mprog_sgetrand`, `mprog_sreset`

## Game Systems

### `src/clan.c` -- Clan System

| Function | Description |
|---|---|
| `void save_clans(void)` / `void load_clans(FILE *fp)` | Persistence |
| `CLAN_DATA *get_clan(int clannum)` | Find clan by number |
| `CLAN_DATA *new_clan(void)` | Create clan structure |
| `void clan_log(CLAN_DATA *clan, char *str, ...)` | Log activity |
| `bool can_ch_join(CHAR_DATA *ch, CLAN_DATA *clan, bool is_petitioning)` | Check eligibility |
| `void add_member(...)` / `void remove_member(...)` | Membership management |

Commands: `do_clan`, `do_join`, `do_petition`, `do_accept`, `do_decline`, `do_offer`, `do_promote`, `do_demote`, `do_resign`, `do_clantalk`, `do_cdeposit`, `do_crecall`, `do_roster`, `do_show`, `do_cedit`, `cedit`

### `src/factions.c` -- Faction Reputation

| Function | Description |
|---|---|
| `void load_factions(FILE *fp)` / `void save_factions(void)` | Persistence |
| `void load_factionaffs(FILE *fp)` / `void save_factionaffs(FILE *fp, AREA_DATA *pArea)` | Mob-faction links |
| `FACTIONLIST_DATA *get_faction_by_vnum(sh_int vnum)` | Find faction |
| `void affect_factions(CHAR_DATA *ch, CHAR_DATA *victim)` | Apply faction changes on kill |
| `double faction_cost_multiplier(CHAR_DATA *ch, CHAR_DATA *keeper, bool buy)` | Shop price modifier |
| `void show_faction_standings(CHAR_DATA *ch, char *argument)` | Display standings |
| `sh_int consider_factions(CHAR_DATA *ch, CHAR_DATA *victim, bool show)` | Show kill impact |
| `void fread_faction_standings(...)` / `void fwrite_faction_standings(...)` | Player save/load |
| `void free_faction_standings(FACTIONPC_DATA *pFactPC)` | Free faction data |
| `bool medit_faction(CHAR_DATA *ch, char *argument)` | OLC faction editing |

Commands: `do_factionedit`, `factionedit`, `do_factionfind`, `do_mpchangefaction`, `do_mpsilentchangefaction`

### `src/auction.c` -- Auction System

| Function | Description |
|---|---|
| `do_auction` | Main auction command (bid, info, stop, start) |
| `void auction_update(void)` | Progress auction stages each tick |
| `void auction_channel_begin/bid/once/twice/sell/remove(void)` | Broadcast messages |
| `void show_obj_stats(int sn, int level, CHAR_DATA *ch, void *vo)` | Display item stats |

### `src/bank.c` -- Banking

Commands: `do_account`, `do_deposit`, `do_withdraw`, `do_bank`

### `src/board.c` -- Message Boards

| Function | Description |
|---|---|
| `void load_boards(void)` / `void save_notes(void)` | Persistence |
| `NOTE_DATA *new_note(void)` / `void free_note(NOTE_DATA *note)` | Note allocation |
| `void finish_note(BOARD_DATA *board, NOTE_DATA *note)` | Post note |
| `int board_lookup(const char *name)` | Find board |
| `bool is_note_to(CHAR_DATA *ch, NOTE_DATA *note)` | Check recipient |
| `int unread_notes(CHAR_DATA *ch, BOARD_DATA *board)` | Count unread |
| `void personal_message(...)` | Send system message |
| `void make_note(...)` | Create system note |
| `handle_con_note_to/subject/expire/text/finish` | Note composition state machine |

Commands: `do_note`, `do_board`

### `src/marry.c` -- Marriage

Commands: `do_marry`, `do_divorce`, `do_consent`

### `src/remort.c` -- Remort

Commands: `do_remort`, `do_remor` (typo guard)

### `src/ban.c` -- Site Banning

| Function | Description |
|---|---|
| `void load_bans(void)` / `void save_bans(void)` | Persistence |
| `bool check_ban(char *site, int type)` | Check if site is banned |
| `void ban_site(CHAR_DATA *ch, char *argument, bool fPerm)` | Core ban logic |

Commands: `do_ban`, `do_permban`, `do_allow`

## Editors and Utilities

### `src/string.c` -- In-Game Text Editor

| Function | Description |
|---|---|
| `void string_edit(CHAR_DATA *ch, char **pString)` | Enter editor for existing string |
| `void string_append(CHAR_DATA *ch, char **pString)` | Enter editor in append mode |
| `void string_add(CHAR_DATA *ch, char *argument)` | Process editor input (dot-commands) |
| `char *string_replace(char *orig, char *old, char *new)` | Substring replacement |
| `char *format_string(char *oldstring)` | Word-wrap formatting |
| `char *line_replace(char *orig, int line, char *arg3)` | Replace specific line |
| `char *line_delete(char *orig, int line)` | Delete specific line |
| `char *line_add(char *orig, int line, char *add)` | Insert line |
| `int count_lines(const char *orig)` | Count lines in string |
| `void show_line_numbers(CHAR_DATA *ch, char *string)` | Display with line numbers |
| `char *first_arg(char *argument, char *arg_first, bool fCase)` | Parse first argument |
| `char *string_unpad(char *argument)` | Remove trailing whitespace |
| `char *string_proper(char *argument)` | Capitalize first letter |

### `src/bit.c` -- Flag Tables

| Function | Description |
|---|---|
| `int flag_lookup(const char *name, const struct flag_type *flag_table)` | Find flag value by name |
| `int flag_value(const struct flag_type *flag_table, char *argument)` | Parse flag string to bitmask |
| `char *flag_string(const struct flag_type *flag_table, int bits)` | Convert bitmask to string |
| `bool is_stat(const struct flag_type *flag_table)` | Is table a stat (non-bitmask) table? |

### `src/class.c` -- Class Skill Configuration

| Function | Description |
|---|---|
| `void save_class(int num)` / `void load_class(int num)` | Per-class persistence |
| `void save_classes(void)` / `void load_classes(void)` | All-class persistence |
| `void do_skill(CHAR_DATA *ch, char *argument)` | `askill` command for editing class-skill access |

### `src/socialolc.c` -- Social Editor

Persistence: `load_socials(FILE *fp)`, `save_socials(void)`

Editor: `do_socialedit`, `socialedit`, `socialedit_show/create/name/chnoarg/othersnoarg/chfound/othersfound/victfound/chnotfound/chauto/othersauto`

Search: `do_socialfind`

### `src/helpolc.c` -- Help Editor

Editor: `do_helpedit`, `helpsedit`, `helpsedit_show/create/keyword/level/text`

Lookup: `HELP_DATA *get_help(char *argument)`

### `src/todoolc.c` -- Todo Editor

Editor: `do_todoedit`, `todoedit`, `todoedit_show/create/keyword/level/text`

Lookup: `TODO_DATA *get_todo(char *argument)`

### `src/random.c` -- Procedural Object Generation

| Function | Description |
|---|---|
| `void load_random_objs(CHAR_DATA *mob, MOB_INDEX_DATA *mobIndex)` | Generate and equip random items on mob |
| `OBJ_DATA *make_random_obj(sh_int level, long possible_type)` | Create one random object |
| `OBJ_DATA *make_rand_weapon(sh_int level, bool ismagic)` | Generate random weapon |
| `OBJ_DATA *make_rand_armor(sh_int level, bool ismagic)` | Generate random armor |
| `OBJ_DATA *make_rand_ring(sh_int level)` | Generate random ring |
| `OBJ_DATA *make_rand_bag(sh_int level)` | Generate random container |
| `int get_random_material(sh_int level)` | Select material by level |
| `void wear_rand_obj(CHAR_DATA *ch, OBJ_DATA *obj)` | Auto-equip generated item |

### `src/drunk2.c` -- Drunk Speech

| Function | Description |
|---|---|
| `char *makedrunk(char *string, CHAR_DATA *ch)` | Apply drunk speech garbling |

### `src/newbits.c` -- Extended Bitfield

| Function | Description |
|---|---|
| `int IS_NEWAFF_SET(char *b, int bit)` | Test extended affect bit |
| `void SET_NEWAFF(char *b, int bit)` | Set extended affect bit |
| `void REMOVE_NEWAFF(char *b, int bit)` | Clear extended affect bit |

### `src/pty.c` -- Pseudo-Terminal

| Function | Description |
|---|---|
| `int master_pty(char *ptyname)` | Allocate master pseudo-terminal |
| `int slave_tty(char *ptyname, char *ttyname)` | Open slave terminal |

### `src/route_io.c` -- I/O Routing

| Function | Description |
|---|---|
| `int route_io(int fd1, int fd2)` | Route data between file descriptors |

### `src/const.c` -- Static Data Tables

No exported functions. Contains all static data definitions:

- `attack_table[]` -- Attack types and damage types
- `race_table[]` / `pc_race_table[]` -- Race definitions with stats and abilities
- `class_table[]` -- Class definitions with stat primes and skill access
- `skill_table[]` -- Master spell/skill table (150+ entries)
- `group_table[]` -- Skill group definitions
- `str_app[]` / `int_app[]` / `wis_app[]` / `dex_app[]` / `con_app[]` -- Stat bonus tables
- `liq_table[]` -- Liquid types and effects
- `title_table[][]` -- Level titles per class and sex
