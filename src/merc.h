#define EMBER_MUD_VERSION         "`WEmberMUD Core 1.0`w\n\r"

#ifndef merc_h__
#define merc_h__
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * Accommodate old non-Ansi compilers.
 */

#if defined(WIN32)
#include <stdio.h>
#if defined(unix)
#undef unix
#endif
#endif
#if defined(TRADITIONAL)
#define const
#define args( list )                    ( )
#define DECLARE_DO_FUN( fun )           void fun( )
#else
#define args( list )                    list
#define DECLARE_DO_FUN( fun )           DO_FUN    fun
#endif

/* system calls */
#if !defined(WIN32)
int unlink(  );
int system(  );
#endif

/*
 * Short scalar types.
 */
#if     !defined(FALSE)
#define FALSE    0
#endif

#if     !defined(TRUE)
#define TRUE     1
#endif

#if     defined(_AIX)
#if     !defined(const)
#define const
#endif
typedef int sh_int;
typedef int bool;
#define unix
#else
typedef short int sh_int;
#if !defined(CPP)
typedef unsigned char bool;
#endif
#endif

/*
 * Structure types.
 */
typedef struct ban_data BAN_DATA;
typedef struct buf_type BUFFER;
typedef struct char_data CHAR_DATA;
typedef struct descriptor_data DESCRIPTOR_DATA;
typedef struct exit_data EXIT_DATA;
typedef struct help_data HELP_DATA;
typedef struct pc_data PC_DATA;
typedef struct room_index_data ROOM_INDEX_DATA;

/*
 * Function types.
 */
typedef void DO_FUN args( ( CHAR_DATA *ch, char *argument ) );

#include "config.h"


/*
 * String and memory management parameters.
 */
#define MAX_KEY_HASH             1024
#define MAX_STRING_LENGTH        4608
#define MAX_INPUT_LENGTH          256
#define MAX_OUTPUT_BUFFER       32768
#define MAX_ALIAS                  20

/*
 * Game parameters.
 */
#define MAX_LEVEL                  60
#define LEVEL_HERO                (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL            (MAX_LEVEL - 8)

#define PULSE_PER_SECOND            4
#define PULSE_TICK              (60 * PULSE_PER_SECOND)

#define IMPLEMENTOR              MAX_LEVEL
#define CREATOR                 (MAX_LEVEL - 1)
#define SUPREME                 (MAX_LEVEL - 2)
#define DEITY                   (MAX_LEVEL - 3)
#define GOD                     (MAX_LEVEL - 4)
#define IMMORTAL                (MAX_LEVEL - 5)
#define DEMI                    (MAX_LEVEL - 6)
#define ANGEL                   (MAX_LEVEL - 7)
#define AVATAR                  (MAX_LEVEL - 8)
#define HERO                     LEVEL_HERO

/*
 * Directions.
 */
#define DIR_NORTH                   0
#define DIR_EAST                    1
#define DIR_SOUTH                   2
#define DIR_WEST                    3
#define DIR_UP                      4
#define DIR_DOWN                    5
#define MAX_DIR                     6

/*
 * Sex.
 */
#define SEX_NEUTRAL                 0
#define SEX_MALE                    1
#define SEX_FEMALE                  2

/*
 * Positions.
 */
#define POS_DEAD                    0
#define POS_MORTAL                  1
#define POS_INCAP                   2
#define POS_STUNNED                 3
#define POS_SLEEPING                4
#define POS_RESTING                 5
#define POS_SITTING                 6
#define POS_FIGHTING                7
#define POS_STANDING                8

/*
 * ACT bits for players (PLR flags).
 */
#define PLR_IS_NPC          (1 <<  0)   /* Auto-set for NPCs */
#define PLR_AUTOEXIT        (1 <<  1)
#define PLR_AUTOMAP         (1 <<  2)
#define PLR_AUTOLOOT        (1 <<  3)
#define PLR_AUTOSAC         (1 <<  4)
#define PLR_AUTOGOLD        (1 <<  5)
#define PLR_AUTOSPLIT       (1 <<  6)
/* Bits 7-12 reserved */
#define PLR_HOLYLIGHT       (1 << 13)
#define PLR_WIZINVIS        (1 << 14)
#define PLR_CANLOOT         (1 << 15)
#define PLR_NOSUMMON        (1 << 16)
#define PLR_NOFOLLOW        (1 << 17)
#define PLR_COLOUR          (1 << 18)
#define PLR_DENY            (1 << 19)
#define PLR_FREEZE          (1 << 20)
#define PLR_LOG             (1 << 22)

/*
 * COMM bits for channels and display.
 */
#define COMM_QUIET          (1 <<  0)
#define COMM_DEAF           (1 <<  1)
#define COMM_NOSHOUT        (1 <<  2)
#define COMM_NOTELL         (1 <<  3)
#define COMM_NOCHANNELS     (1 <<  4)
#define COMM_COMPACT        (1 <<  5)
#define COMM_BRIEF          (1 <<  6)
#define COMM_PROMPT         (1 <<  7)
#define COMM_COMBINE        (1 <<  8)
#define COMM_NOEMOTE        (1 <<  9)
#define COMM_AFK            (1 << 10)

/*
 * Room flags.
 */
#define ROOM_DARK               (1 <<  0)
#define ROOM_NO_MOB             (1 <<  2)
#define ROOM_INDOORS            (1 <<  3)
#define ROOM_PRIVATE            (1 <<  9)
#define ROOM_SAFE               (1 << 10)
#define ROOM_SOLITARY           (1 << 11)
#define ROOM_IMP_ONLY           (1 << 14)
#define ROOM_GODS_ONLY          (1 << 15)
#define ROOM_HEROES_ONLY        (1 << 16)
#define ROOM_NEWBIES_ONLY       (1 << 17)

/*
 * Exit flags.
 */
#define EX_ISDOOR               (1 <<  0)
#define EX_CLOSED               (1 <<  1)
#define EX_LOCKED               (1 <<  2)
#define EX_PICKPROOF            (1 <<  5)
#define EX_NOPASS               (1 <<  6)

/*
 * Sector types.
 */
#define SECT_INSIDE                 0
#define SECT_CITY                   1
#define SECT_FIELD                  2
#define SECT_FOREST                 3
#define SECT_HILLS                  4
#define SECT_MOUNTAIN               5
#define SECT_WATER_SWIM             6
#define SECT_WATER_NOSWIM           7
#define SECT_AIR                    9
#define SECT_DESERT                10
#define SECT_MAX                   11

/*
 * Log types for commands.
 */
#define LOG_NORMAL                  0
#define LOG_ALWAYS                  1
#define LOG_NEVER                   2

/*
 * Target types for act().
 */
#define TO_ROOM                     0
#define TO_NOTVICT                  1
#define TO_VICT                     2
#define TO_CHAR                     3

/*
 * act_new flag for minimum position.
 */
#define POS_TO_ACT                  POS_DEAD


/***************************************************************************
 *                                                                         *
 *                           DATA STRUCTURES                               *
 *                                                                         *
 ***************************************************************************/

/*
 * Help data.
 */
struct help_data {
    HELP_DATA *next;
    sh_int level;
    char *keyword;
    char *text;
};

/*
 * Ban data.
 */
struct ban_data {
    BAN_DATA *next;
    bool valid;
    sh_int ban_flags;
    sh_int level;
    char *name;
};

/* Ban flags */
#define BAN_SUFFIX          1
#define BAN_PREFIX          2
#define BAN_NEWBIES         4
#define BAN_ALL             8
#define BAN_PERMIT         16
#define BAN_PERMANENT      32

/*
 * Buffer structure (crash-proof output buffering).
 */
struct buf_type {
    BUFFER *next;
    bool valid;
    sh_int state;
    sh_int size;
    char *string;
};

/*
 * Descriptor (connection) data.
 */
struct descriptor_data {
    DESCRIPTOR_DATA *next;
    CHAR_DATA *character;
    char *host;
    sh_int descriptor;
    sh_int connected;
    bool fcommand;
    bool ansi;
    char inbuf[4 * MAX_INPUT_LENGTH];
    char incomm[MAX_INPUT_LENGTH];
    char inlast[MAX_INPUT_LENGTH];
    int repeat;
    char *outbuf;
    int outsize;
    int outtop;
    char *showstr_head;
    char *showstr_point;
};

/*
 * Per-player data (only for PCs, not NPCs).
 */
struct pc_data {
    PC_DATA *next;
    BUFFER *buffer;
    char *pwd;
    char *title;
    char *prompt;
    char *alias[MAX_ALIAS];
    char *alias_sub[MAX_ALIAS];
    bool confirm_delete;
};

/*
 * Character data (players).
 */
struct char_data {
    CHAR_DATA *next;
    CHAR_DATA *next_player;
    CHAR_DATA *next_in_room;
    DESCRIPTOR_DATA *desc;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *was_in_room;
    PC_DATA *pcdata;
    char *name;
    char *short_descr;
    char *long_descr;
    char *description;
    sh_int sex;
    sh_int level;
    sh_int trust;
    int played;
    int lines;
    time_t logon;
    sh_int timer;
    sh_int wait;
    long act;
    long comm;
    sh_int invis_level;
    sh_int position;
};

/*
 * Exit data.
 */
struct exit_data {
    union {
        ROOM_INDEX_DATA *to_room;
        sh_int vnum;
    } u1;
    EXIT_DATA *next;
    sh_int exit_info;
    sh_int key;
    char *keyword;
    char *description;
};

/*
 * Room data.
 */
struct room_index_data {
    ROOM_INDEX_DATA *next;
    CHAR_DATA *people;
    EXIT_DATA *exit[6];
    char *name;
    char *description;
    sh_int vnum;
    long room_flags;
    sh_int light;
    sh_int sector_type;
};


/***************************************************************************
 *                                                                         *
 *                          GLOBAL VARIABLES                               *
 *                                                                         *
 ***************************************************************************/

/*
 * Descriptor lists.
 */
extern DESCRIPTOR_DATA *descriptor_list;

/*
 * Character lists.
 */
extern CHAR_DATA *char_list;
extern CHAR_DATA *player_list;

/*
 * Room data.
 */
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];

/*
 * Help data.
 */
extern HELP_DATA *help_first;
extern HELP_DATA *help_last;
extern char *help_greeting;

/*
 * Ban data.
 */
extern BAN_DATA *ban_list;

/*
 * Boot flag.
 */
extern bool fBootDb;
extern time_t current_time;
extern bool merc_down;

/*
 * Telnet strings.
 */
extern const char echo_off_str[];
extern const char echo_on_str[];
extern const char go_ahead_str[];

/*
 * Room vnums for special locations.
 */
#define ROOM_VNUM_LIMBO              2
#define ROOM_VNUM_TEMPLE          3001


/***************************************************************************
 *                                                                         *
 *                             UTILITY MACROS                              *
 *                                                                         *
 ***************************************************************************/

#define IS_NPC(ch)          (IS_SET((ch)->act, PLR_IS_NPC))
#define IS_IMMORTAL(ch)     (get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)         (get_trust(ch) >= LEVEL_HERO)
#define IS_TRUSTED(ch,lev)  (get_trust(ch) >= (lev))
#define IS_ADMIN(ch)        (get_trust(ch) >= SUPREME)
#define IS_AWAKE(ch)        ((ch)->position > POS_SLEEPING)
#define GET_AGE(ch)         ((int)(17 + ((ch)->played / 72000)))

#define CH(d)               ((d)->character)

#define IS_VALID(data)          ((data) != NULL && (data)->valid)
#define VALIDATE(data)          ((data)->valid = TRUE)
#define INVALIDATE(data)        ((data)->valid = FALSE)

#define IS_SET(flag, bit)       ((flag) & (bit))
#define SET_BIT(var, bit)       ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))

#define UMIN(a, b)          ((a) < (b) ? (a) : (b))
#define UMAX(a, b)          ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)     ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)            ((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)            ((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))

#define WAIT_STATE(ch, npulse)  ((ch)->wait = UMAX((ch)->wait, (npulse)))

#define PERS(ch, looker)    ((ch)->name)

/*
 * String comparison macros (used everywhere).
 */
#define str_cmp_nofunc(s1, s2) \
    (strcasecmp((s1), (s2)))

/*
 * Null file for portability.
 */
#if defined(WIN32)
#define NULL_FILE   "nul"
#else
#define NULL_FILE   "/dev/null"
#endif


/***************************************************************************
 *                                                                         *
 *                          FUNCTION PROTOTYPES                            *
 *                                                                         *
 ***************************************************************************/

/* act_basic.c */
DECLARE_DO_FUN( do_look );
DECLARE_DO_FUN( do_say );
DECLARE_DO_FUN( do_tell );
DECLARE_DO_FUN( do_who );
DECLARE_DO_FUN( do_quit );
DECLARE_DO_FUN( do_save );
DECLARE_DO_FUN( do_help );
DECLARE_DO_FUN( do_exits );
DECLARE_DO_FUN( do_north );
DECLARE_DO_FUN( do_south );
DECLARE_DO_FUN( do_east );
DECLARE_DO_FUN( do_west );
DECLARE_DO_FUN( do_up );
DECLARE_DO_FUN( do_down );
void substitute_alias args( ( DESCRIPTOR_DATA *d, char *input ) );
void set_title args( ( CHAR_DATA *ch, char *title ) );
bool check_social args( ( CHAR_DATA *ch, char *command, char *argument ) );

/* ban.c */
void check_ban args( ( DESCRIPTOR_DATA *d ) );
void load_bans args( ( void ) );
void save_bans args( ( void ) );

/* comm.c */
void send_to_char args( ( const char *txt, CHAR_DATA *ch ) );
void page_to_char args( ( const char *txt, CHAR_DATA *ch ) );
void printf_to_char args( ( CHAR_DATA *ch, char *fmt, ... ) );
void send_to_room args( ( const char *txt, ROOM_INDEX_DATA *room ) );
void act args( ( const char *format, CHAR_DATA *ch, const void *arg1,
                 const void *arg2, int type ) );
void act_new args( ( const char *format, CHAR_DATA *ch, const void *arg1,
                     const void *arg2, int type, int min_pos ) );
void show_string args( ( DESCRIPTOR_DATA *d, char *input ) );
void close_socket args( ( DESCRIPTOR_DATA *dclose ) );
void write_to_buffer args( ( DESCRIPTOR_DATA *d, const char *txt, int length ) );
bool check_parse_name args( ( char *name ) );
bool check_reconnect args( ( DESCRIPTOR_DATA *d, char *name, bool fConn ) );
bool check_playing args( ( DESCRIPTOR_DATA *d, char *name ) );
void stop_idling args( ( CHAR_DATA *ch ) );
char *do_color args( ( char *plaintext, bool ansi ) );
void nanny args( ( DESCRIPTOR_DATA *d, char *argument ) );

/* db.c */
void boot_db args( ( void ) );
ROOM_INDEX_DATA *get_room_index args( ( int vnum ) );
char fread_letter args( ( FILE *fp ) );
int fread_number args( ( FILE *fp ) );
long fread_flag args( ( FILE *fp ) );
char *fread_string args( ( FILE *fp ) );
char *fread_string_eol args( ( FILE *fp ) );
void fread_to_eol args( ( FILE *fp ) );
char *fread_word args( ( FILE *fp ) );
void *alloc_mem args( ( int sMem ) );
void *alloc_perm args( ( int sMem ) );
void free_mem args( ( void *pMemPtr ) );
void bug args( ( const char *str, int param ) );
void log_string args( ( const char *str ) );
void logf_string args( ( char *fmt, ... ) );
int number_fuzzy args( ( int number ) );
int number_range args( ( int from, int to ) );
int number_percent args( ( void ) );
int number_door args( ( void ) );
int number_bits args( ( int width ) );
int number_mm args( ( void ) );
int dice args( ( int number, int size ) );
void smash_tilde args( ( char *str ) );
bool str_cmp args( ( const char *astr, const char *bstr ) );
bool str_prefix args( ( const char *astr, const char *bstr ) );
bool str_infix args( ( const char *astr, const char *bstr ) );
bool str_suffix args( ( const char *astr, const char *bstr ) );
char *capitalize args( ( const char *str ) );
void append_file args( ( CHAR_DATA *ch, char *file, char *str ) );
void tail_chain args( ( void ) );
void clear_char args( ( CHAR_DATA *ch ) );
void free_char args( ( CHAR_DATA *ch ) );

/* handler.c */
int get_trust args( ( CHAR_DATA *ch ) );
bool is_name args( ( char *str, char *namelist ) );
bool is_exact_name args( ( char *str, char *namelist ) );
void char_from_room args( ( CHAR_DATA *ch ) );
void char_to_room args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void extract_char args( ( CHAR_DATA *ch, bool fPull ) );
CHAR_DATA *get_char_room args( ( CHAR_DATA *ch, char *argument ) );
CHAR_DATA *get_player_world args( ( CHAR_DATA *ch, char *argument ) );
bool can_see args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool can_see_room args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
bool room_is_dark args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool room_is_private args( ( ROOM_INDEX_DATA *pRoomIndex ) );

/* interp.c */
void interpret args( ( CHAR_DATA *ch, char *argument ) );
bool is_number args( ( char *arg ) );
int number_argument args( ( char *argument, char *arg ) );
char *one_argument args( ( char *argument, char *arg_first ) );

/* save.c */
void save_char_obj args( ( CHAR_DATA *ch ) );
bool load_char_obj args( ( DESCRIPTOR_DATA *d, char *name ) );

/* ssm.c (shared string manager) */
char *str_dup args( ( const char *str ) );
void free_string args( ( char **pstr ) );
char *fread_string args( ( FILE *fp ) );
void init_string_space args( ( void ) );
void boot_done args( ( void ) );

/* random.c */
void init_mm args( ( void ) );

/* update.c */
void update_handler args( ( void ) );

/* recycle.c / recycle.h */
#include "recycle.h"

#endif /* merc_h__ */
