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

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "merc.h"
#include "db.h"

/*
 * Globals.
 */
bool                    fBootDb;
CHAR_DATA *             char_list;
CHAR_DATA *             player_list;
ROOM_INDEX_DATA *       room_index_hash[MAX_KEY_HASH];
HELP_DATA *             help_first;
HELP_DATA *             help_last;
char *                  help_greeting;
/* ban_list is defined in ban.c */

/* Defined in comm.c */
extern time_t           current_time;
extern bool             merc_down;
extern FILE *           fpReserve;

/* Defined in interp.c */
extern char             log_buf[];

/* SSM globals needed by this file */
char                    str_empty[1];
char *                  string_space;
char *                  top_string;
long                    nAllocString;
long                    sAllocString;

/*
 * Configurable directory and file paths.
 * Initialized to defaults from config.h, can be overridden at runtime.
 */
char area_dir[256];
char player_dir[256];
char player_temp[256];
char log_dir[256];
char help_file[256];
char bug_file[256];

/*
 * Local prototypes.
 */
void    create_minimal_world    args( ( void ) );
void    create_basic_helps      args( ( void ) );
void    load_area_files         args( ( void ) );
void    load_area_file          args( ( char *filename ) );
void    load_rooms              args( ( FILE *fp ) );
void    load_helps              args( ( FILE *fp ) );
void    fix_exits               args( ( void ) );
void    skip_section_areadata   args( ( FILE *fp ) );
void    skip_section_mobiles    args( ( FILE *fp ) );
void    skip_section_objects    args( ( FILE *fp ) );
void    skip_section_resets     args( ( FILE *fp ) );
void    skip_section_shops      args( ( FILE *fp ) );
void    skip_section_progs      args( ( FILE *fp ) );
void    skip_section_unknown    args( ( FILE *fp ) );

/*
 * Big mama top level function.
 */
void boot_db( void )
{
    /*
     * Init shared string space.
     */
    init_string_space();
    fBootDb = TRUE;

    /*
     * Init random number generator.
     */
    init_mm();

    /*
     * Initialize config paths to defaults.
     */
    strncpy( area_dir,    CFG_AREA_DIR,    sizeof(area_dir)    - 1 );
    strncpy( player_dir,  CFG_PLAYER_DIR,  sizeof(player_dir)  - 1 );
    strncpy( player_temp, CFG_PLAYER_TEMP, sizeof(player_temp) - 1 );
    strncpy( log_dir,     CFG_LOG_DIR,     sizeof(log_dir)     - 1 );
    strncpy( help_file,   CFG_HELP_FILE,   sizeof(help_file)   - 1 );
    strncpy( bug_file,    CFG_BUG_FILE,    sizeof(bug_file)    - 1 );

    area_dir[sizeof(area_dir) - 1]       = '\0';
    player_dir[sizeof(player_dir) - 1]   = '\0';
    player_temp[sizeof(player_temp) - 1] = '\0';
    log_dir[sizeof(log_dir) - 1]         = '\0';
    help_file[sizeof(help_file) - 1]     = '\0';
    bug_file[sizeof(bug_file) - 1]       = '\0';

    /*
     * Try loading area files.
     */
    load_area_files();

    /*
     * If no rooms were loaded (missing area.lst or empty), use hardcoded fallback.
     */
    if ( get_room_index( ROOM_VNUM_LIMBO ) == NULL
    &&   get_room_index( ROOM_VNUM_TEMPLE ) == NULL )
    {
        log_string( "No rooms loaded from area files, using hardcoded world." );
        create_minimal_world();
    }
    else
    {
        fix_exits();
    }

    /*
     * If no help entries were loaded, create basic ones.
     */
    if ( help_first == NULL )
    {
        log_string( "No help entries loaded from area files, using built-in helps." );
        create_basic_helps();
    }

    /*
     * Set a default greeting if none was loaded from help files.
     */
    if ( help_greeting == NULL )
    {
        help_greeting = str_dup(
            "\n\r"
            "`W" "Welcome to EmberMUD!" "`w\n\r"
            "\n\r"
            "Based on ROM 2.4, Merc 2.1, and DikuMUD.\n\r"
            "Type your character name to begin.\n\r"
            "\n\r"
        );
    }

    /*
     * Log summary and finalize boot.
     */
    {
        int nRooms = 0, nExits = 0, nHelps = 0;
        ROOM_INDEX_DATA *pRoom;
        HELP_DATA *pHelp;
        int iHash, door;

        for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
            for ( pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next )
            {
                nRooms++;
                for ( door = 0; door < 6; door++ )
                    if ( pRoom->exit[door] )
                        nExits++;
            }
        for ( pHelp = help_first; pHelp; pHelp = pHelp->next )
            nHelps++;

        logf_string( "Boot complete: %d rooms, %d exits, %d help entries.", nRooms, nExits, nHelps );
    }

    boot_done();
    fBootDb = FALSE;
    return;
}

/*
 * Create a minimal hardcoded world with 8 rooms and connecting exits.
 * This replaces the area file loading system for the stripped-down core.
 */
void create_minimal_world( void )
{
    ROOM_INDEX_DATA *limbo;
    ROOM_INDEX_DATA *square;
    ROOM_INDEX_DATA *north_road;
    ROOM_INDEX_DATA *south_gate;
    ROOM_INDEX_DATA *east_market;
    ROOM_INDEX_DATA *west_garden;
    ROOM_INDEX_DATA *watch_tower;
    ROOM_INDEX_DATA *cellar;
    EXIT_DATA *pexit;
    int iHash;
    int door;

    /*
     * Room 2: Limbo (ROOM_VNUM_LIMBO) - idle/void room.
     */
    limbo = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    limbo->next          = NULL;
    limbo->people        = NULL;
    for ( door = 0; door < 6; door++ )
        limbo->exit[door] = NULL;
    limbo->name          = str_dup( "Limbo" );
    limbo->description   = str_dup(
        "You are floating in an endless void of grey nothingness.\n\r"
        "There is no ground beneath your feet, no sky above your head.\n\r"
        "The silence here is absolute and oppressive, broken only by\n\r"
        "the faint echo of your own thoughts.\n\r"
    );
    limbo->vnum          = ROOM_VNUM_LIMBO;
    limbo->room_flags    = ROOM_SAFE;
    limbo->light         = 0;
    limbo->sector_type   = SECT_INSIDE;

    iHash = limbo->vnum % MAX_KEY_HASH;
    limbo->next = room_index_hash[iHash];
    room_index_hash[iHash] = limbo;

    /*
     * Room 3001: Town Square (ROOM_VNUM_TEMPLE) - central hub.
     */
    square = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    square->next          = NULL;
    square->people        = NULL;
    for ( door = 0; door < 6; door++ )
        square->exit[door] = NULL;
    square->name          = str_dup( "Town Square" );
    square->description   = str_dup(
        "You stand at the heart of a small medieval town. Cobblestones\n\r"
        "stretch in every direction, worn smooth by centuries of foot\n\r"
        "traffic. A weathered stone fountain sits in the center of the\n\r"
        "square, its basin filled with clear water. Roads lead north,\n\r"
        "south, east, and west, while a spiral staircase leads up to a\n\r"
        "watchtower and a trapdoor opens into a cellar below.\n\r"
    );
    square->vnum          = ROOM_VNUM_TEMPLE;
    square->room_flags    = ROOM_SAFE | ROOM_NO_MOB;
    square->light         = 0;
    square->sector_type   = SECT_CITY;

    iHash = square->vnum % MAX_KEY_HASH;
    square->next = room_index_hash[iHash];
    room_index_hash[iHash] = square;

    /*
     * Room 3002: North Road.
     */
    north_road = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    north_road->next          = NULL;
    north_road->people        = NULL;
    for ( door = 0; door < 6; door++ )
        north_road->exit[door] = NULL;
    north_road->name          = str_dup( "North Road" );
    north_road->description   = str_dup(
        "A hard-packed dirt road stretches northward into rolling green\n\r"
        "hills. Wooden fences line both sides of the path, enclosing\n\r"
        "small pastures where sheep graze lazily. The town square lies\n\r"
        "to the south, its fountain just visible over the rooftops.\n\r"
    );
    north_road->vnum          = 3002;
    north_road->room_flags    = 0;
    north_road->light         = 0;
    north_road->sector_type   = SECT_FIELD;

    iHash = north_road->vnum % MAX_KEY_HASH;
    north_road->next = room_index_hash[iHash];
    room_index_hash[iHash] = north_road;

    /*
     * Room 3003: South Gate.
     */
    south_gate = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    south_gate->next          = NULL;
    south_gate->people        = NULL;
    for ( door = 0; door < 6; door++ )
        south_gate->exit[door] = NULL;
    south_gate->name          = str_dup( "South Gate" );
    south_gate->description   = str_dup(
        "A massive wooden gate reinforced with iron bands marks the\n\r"
        "southern entrance to the town. Two stone pillars flank the\n\r"
        "gateway, each topped with a flickering torch. Beyond the\n\r"
        "gate, a dusty road winds away into dark, dense forest.\n\r"
    );
    south_gate->vnum          = 3003;
    south_gate->room_flags    = 0;
    south_gate->light         = 0;
    south_gate->sector_type   = SECT_CITY;

    iHash = south_gate->vnum % MAX_KEY_HASH;
    south_gate->next = room_index_hash[iHash];
    room_index_hash[iHash] = south_gate;

    /*
     * Room 3004: East Market.
     */
    east_market = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    east_market->next          = NULL;
    east_market->people        = NULL;
    for ( door = 0; door < 6; door++ )
        east_market->exit[door] = NULL;
    east_market->name          = str_dup( "East Market" );
    east_market->description   = str_dup(
        "Rows of wooden stalls and canvas-covered booths fill this\n\r"
        "bustling marketplace. The air carries the mingled scents of\n\r"
        "fresh bread, spiced meats, and exotic herbs. Colorful banners\n\r"
        "hang between the stalls, snapping gently in the breeze.\n\r"
    );
    east_market->vnum          = 3004;
    east_market->room_flags    = 0;
    east_market->light         = 0;
    east_market->sector_type   = SECT_CITY;

    iHash = east_market->vnum % MAX_KEY_HASH;
    east_market->next = room_index_hash[iHash];
    room_index_hash[iHash] = east_market;

    /*
     * Room 3005: West Garden.
     */
    west_garden = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    west_garden->next          = NULL;
    west_garden->people        = NULL;
    for ( door = 0; door < 6; door++ )
        west_garden->exit[door] = NULL;
    west_garden->name          = str_dup( "West Garden" );
    west_garden->description   = str_dup(
        "A tranquil garden occupies this corner of town, enclosed by\n\r"
        "a low stone wall covered in climbing ivy. Ancient oak trees\n\r"
        "provide dappled shade over beds of wildflowers and medicinal\n\r"
        "herbs. A moss-covered stone bench invites quiet reflection.\n\r"
    );
    west_garden->vnum          = 3005;
    west_garden->room_flags    = ROOM_SAFE;
    west_garden->light         = 0;
    west_garden->sector_type   = SECT_FIELD;

    iHash = west_garden->vnum % MAX_KEY_HASH;
    west_garden->next = room_index_hash[iHash];
    room_index_hash[iHash] = west_garden;

    /*
     * Room 3006: Watch Tower.
     */
    watch_tower = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    watch_tower->next          = NULL;
    watch_tower->people        = NULL;
    for ( door = 0; door < 6; door++ )
        watch_tower->exit[door] = NULL;
    watch_tower->name          = str_dup( "Watch Tower" );
    watch_tower->description   = str_dup(
        "You stand atop a tall stone watchtower that rises above the\n\r"
        "town rooftops. From this vantage point, the entire settlement\n\r"
        "spreads below you -- the square, the market, the garden, and\n\r"
        "the roads leading away into the countryside. A cold wind whips\n\r"
        "at your clothing. A spiral staircase leads back down.\n\r"
    );
    watch_tower->vnum          = 3006;
    watch_tower->room_flags    = ROOM_INDOORS;
    watch_tower->light         = 0;
    watch_tower->sector_type   = SECT_INSIDE;

    iHash = watch_tower->vnum % MAX_KEY_HASH;
    watch_tower->next = room_index_hash[iHash];
    room_index_hash[iHash] = watch_tower;

    /*
     * Room 3007: Cellar.
     */
    cellar = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    cellar->next          = NULL;
    cellar->people        = NULL;
    for ( door = 0; door < 6; door++ )
        cellar->exit[door] = NULL;
    cellar->name          = str_dup( "Cellar" );
    cellar->description   = str_dup(
        "A damp, cool cellar stretches beneath the town square. Thick\n\r"
        "stone walls are lined with wooden shelves holding dusty bottles\n\r"
        "and forgotten provisions. Cobwebs drape from the low ceiling,\n\r"
        "and the only light filters down from the trapdoor above.\n\r"
    );
    cellar->vnum          = 3007;
    cellar->room_flags    = ROOM_DARK | ROOM_INDOORS;
    cellar->light         = 0;
    cellar->sector_type   = SECT_INSIDE;

    iHash = cellar->vnum % MAX_KEY_HASH;
    cellar->next = room_index_hash[iHash];
    room_index_hash[iHash] = cellar;

    /*
     * Now create exits linking all rooms together.
     *
     * Town Square (3001):
     *   north -> North Road (3002)
     *   south -> South Gate (3003)
     *   east  -> East Market (3004)
     *   west  -> West Garden (3005)
     *   up    -> Watch Tower (3006)
     *   down  -> Cellar (3007)
     */

    /* Square -> North Road (north) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = north_road;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    square->exit[DIR_NORTH] = pexit;

    /* Square -> South Gate (south) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = south_gate;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    square->exit[DIR_SOUTH] = pexit;

    /* Square -> East Market (east) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = east_market;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    square->exit[DIR_EAST] = pexit;

    /* Square -> West Garden (west) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = west_garden;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    square->exit[DIR_WEST] = pexit;

    /* Square -> Watch Tower (up) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = watch_tower;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    square->exit[DIR_UP] = pexit;

    /* Square -> Cellar (down) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = cellar;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    square->exit[DIR_DOWN] = pexit;

    /* North Road -> Square (south) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = square;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    north_road->exit[DIR_SOUTH] = pexit;

    /* South Gate -> Square (north) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = square;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    south_gate->exit[DIR_NORTH] = pexit;

    /* East Market -> Square (west) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = square;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    east_market->exit[DIR_WEST] = pexit;

    /* West Garden -> Square (east) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = square;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    west_garden->exit[DIR_EAST] = pexit;

    /* Watch Tower -> Square (down) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = square;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    watch_tower->exit[DIR_DOWN] = pexit;

    /* Cellar -> Square (up) */
    pexit = (EXIT_DATA *) alloc_perm( sizeof( EXIT_DATA ) );
    pexit->u1.to_room   = square;
    pexit->next          = NULL;
    pexit->exit_info     = 0;
    pexit->key           = 0;
    pexit->keyword       = &str_empty[0];
    pexit->description   = &str_empty[0];
    cellar->exit[DIR_UP] = pexit;

    log_string( "Minimal world created: 8 rooms, 12 exits." );
    return;
}

/*
 * Add a single help entry to the global linked list.
 */
static void add_help( sh_int level, char *keyword, char *text )
{
    HELP_DATA *pHelp;

    pHelp        = alloc_perm( sizeof( *pHelp ) );
    pHelp->level = level;
    pHelp->keyword = str_dup( keyword );
    pHelp->text    = str_dup( text );
    pHelp->next    = NULL;

    if ( help_first == NULL )
        help_first = pHelp;
    if ( help_last != NULL )
        help_last->next = pHelp;
    help_last = pHelp;
}

/*
 * Create a handful of built-in help entries so the MOTD and basic
 * commands are available without loading area help files.
 */
void create_basic_helps( void )
{
    add_help( 0, "MOTD",
        "Welcome back!  This MUD is currently in development.\n\r"
        "Please report any bugs you find.\n\r" );

    add_help( 0, "RULES",
        "1. Be respectful to other players.\n\r"
        "2. No harassment or abuse.\n\r"
        "3. Have fun!\n\r" );

    add_help( 0, "COMMANDS SUMMARY",
        "`WAvailable Commands:`0\n\r"
        "  look        - Look at your surroundings\n\r"
        "  exits       - Show available exits\n\r"
        "  north south east west up down - Move in a direction\n\r"
        "  say <msg>   - Say something to the room\n\r"
        "  tell <who> <msg> - Send a private message\n\r"
        "  who         - List online players\n\r"
        "  save        - Save your character\n\r"
        "  quit        - Save and leave the game\n\r"
        "  help <topic>- Get help on a topic\n\r" );
}

/***************************************************************************
 *                                                                         *
 *                        AREA FILE LOADER                                 *
 *                                                                         *
 ***************************************************************************/

/*
 * Load all area files listed in area.lst.
 * If area.lst is missing, returns silently (boot_db will use fallback).
 */
void load_area_files( void )
{
    FILE *fpList;
    char  strsave[512];

    snprintf( strsave, sizeof(strsave), "%s%s", area_dir, "area.lst" );

    if ( ( fpList = fopen( strsave, "r" ) ) == NULL )
    {
        logf_string( "load_area_files: %s not found, using hardcoded world.", strsave );
        return;
    }

    for ( ; ; )
    {
        char *word;
        char  filename[256];

        word = fread_word( fpList );
        if ( word[0] == '$' )
            break;

        sprintf( filename, "%s%s", area_dir, word );
        load_area_file( filename );
    }

    fclose( fpList );
    return;
}

/*
 * Load a single area file, dispatching each section.
 */
void load_area_file( char *filename )
{
    FILE *fp;
    char  letter;
    char *word;

    if ( ( fp = fopen( filename, "r" ) ) == NULL )
    {
        logf_string( "load_area_file: could not open %s, skipping.", filename );
        return;
    }

    for ( ; ; )
    {
        letter = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "load_area_file: # not found in %s.", 0 );
            break;
        }

        word = fread_word( fp );

        if ( word[0] == '$' )
            break;
        else if ( !str_cmp( word, "AREADATA" ) )  skip_section_areadata( fp );
        else if ( !str_cmp( word, "HELPS" ) )     load_helps( fp );
        else if ( !str_cmp( word, "MOBILES" ) )    skip_section_mobiles( fp );
        else if ( !str_cmp( word, "OBJECTS" ) )    skip_section_objects( fp );
        else if ( !str_cmp( word, "ROOMS" ) )      load_rooms( fp );
        else if ( !str_cmp( word, "RESETS" ) )     skip_section_resets( fp );
        else if ( !str_cmp( word, "SHOPS" ) )      skip_section_shops( fp );
        else if ( !str_cmp( word, "PROGS" ) )      skip_section_progs( fp );
        else
        {
            logf_string( "load_area_file: unknown section '%s' in %s, skipping.", word, filename );
            skip_section_unknown( fp );
        }
    }

    fclose( fp );
    return;
}

/*
 * Load #ROOMS section from an area file.
 * Format per room:
 *   #vnum
 *   name~
 *   description~
 *   area_num room_flags sector_type
 *   [D<dir> description~ keyword~ locks key dest_vnum]
 *   [E keyword~ text~]
 *   S
 * Terminated by #0.
 */
void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;

    for ( ; ; )
    {
        sh_int vnum;
        char   letter;
        int    door;
        int    iHash;

        letter = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "load_rooms: # not found.", 0 );
            exit( 1 );
        }

        vnum = fread_number( fp );
        if ( vnum == 0 )
            break;

        if ( get_room_index( vnum ) != NULL )
        {
            bug( "load_rooms: duplicate vnum %d.", vnum );
            exit( 1 );
        }

        pRoomIndex = (ROOM_INDEX_DATA *) alloc_perm( sizeof( *pRoomIndex ) );
        pRoomIndex->next        = NULL;
        pRoomIndex->people      = NULL;
        for ( door = 0; door < 6; door++ )
            pRoomIndex->exit[door] = NULL;
        pRoomIndex->vnum        = vnum;
        pRoomIndex->name        = fread_string( fp );
        pRoomIndex->description = fread_string( fp );
        /* area_num */ fread_number( fp );
        pRoomIndex->room_flags  = fread_flag( fp );
        pRoomIndex->sector_type = fread_number( fp );
        pRoomIndex->light       = 0;

        for ( ; ; )
        {
            letter = fread_letter( fp );

            if ( letter == 'S' )
                break;

            if ( letter == 'D' )
            {
                EXIT_DATA *pexit;
                int        locks;

                door = fread_number( fp );
                if ( door < 0 || door > 5 )
                {
                    bug( "load_rooms: bad door %d in vnum %d.", door );
                    exit( 1 );
                }

                pexit = (EXIT_DATA *) alloc_perm( sizeof( *pexit ) );
                pexit->description = fread_string( fp );
                pexit->keyword     = fread_string( fp );
                pexit->next        = NULL;
                locks              = fread_number( fp );
                pexit->key         = fread_number( fp );
                pexit->u1.vnum     = fread_number( fp );

                switch ( locks )
                {
                case 0:  pexit->exit_info = 0;                                        break;
                case 1:  pexit->exit_info = EX_ISDOOR;                                break;
                case 2:  pexit->exit_info = EX_ISDOOR | EX_CLOSED | EX_LOCKED
                                          | EX_PICKPROOF;                              break;
                default: pexit->exit_info = EX_ISDOOR | EX_CLOSED | EX_LOCKED;        break;
                }

                pRoomIndex->exit[door] = pexit;
            }
            else if ( letter == 'E' )
            {
                /* Extra description — read and discard keyword~ and text~ */
                fread_string( fp );
                fread_string( fp );
            }
            else
            {
                bug( "load_rooms: unknown sub-record '%c' in vnum %d.", letter );
                exit( 1 );
            }
        }

        iHash = pRoomIndex->vnum % MAX_KEY_HASH;
        pRoomIndex->next = room_index_hash[iHash];
        room_index_hash[iHash] = pRoomIndex;
    }

    return;
}

/*
 * Load #HELPS section from an area file.
 * Format: level keyword~\n text~
 * Terminated by 0 $~
 */
void load_helps( FILE *fp )
{
    HELP_DATA *pHelp;

    for ( ; ; )
    {
        sh_int level;
        char  *keyword;

        level   = fread_number( fp );
        keyword = fread_string( fp );

        if ( keyword[0] == '$' )
            break;

        pHelp          = (HELP_DATA *) alloc_perm( sizeof( *pHelp ) );
        pHelp->level   = level;
        pHelp->keyword = keyword;
        pHelp->text    = fread_string( fp );
        pHelp->next    = NULL;

        if ( help_first == NULL )
            help_first = pHelp;
        if ( help_last != NULL )
            help_last->next = pHelp;
        help_last = pHelp;

        if ( !str_cmp( keyword, "GREETING" ) )
            help_greeting = pHelp->text;
        if ( !str_cmp( keyword, "ANSIGREET" ) )
            help_greeting = pHelp->text;
    }

    return;
}

/*
 * After all rooms are loaded, resolve exit vnums to room pointers.
 */
void fix_exits( void )
{
    ROOM_INDEX_DATA *pRoomIndex;
    int iHash;
    int door;
    int nUnresolved = 0;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for ( pRoomIndex = room_index_hash[iHash];
              pRoomIndex != NULL;
              pRoomIndex = pRoomIndex->next )
        {
            for ( door = 0; door < 6; door++ )
            {
                EXIT_DATA *pexit = pRoomIndex->exit[door];
                if ( pexit != NULL )
                {
                    sh_int vnum = pexit->u1.vnum;
                    if ( vnum <= 0 )
                        pexit->u1.to_room = NULL;
                    else
                    {
                        pexit->u1.to_room = get_room_index( vnum );
                        if ( pexit->u1.to_room == NULL )
                        {
                            logf_string( "fix_exits: room %d exit %d -> vnum %d not found.",
                                pRoomIndex->vnum, door, vnum );
                            nUnresolved++;
                        }
                    }
                }
            }
        }
    }

    if ( nUnresolved > 0 )
        logf_string( "fix_exits: %d unresolved exit(s).", nUnresolved );

    return;
}

/*
 * Skip #AREADATA section (reads words until "End").
 */
void skip_section_areadata( FILE *fp )
{
    char *word;

    for ( ; ; )
    {
        word = fread_word( fp );
        if ( !str_cmp( word, "End" ) )
            break;
        fread_to_eol( fp );
    }

    return;
}

/*
 * Skip #MOBILES or #OBJECTS section (scan for #0).
 */
static void skip_section_vnum_list( FILE *fp )
{
    signed char c;
    bool at_hash = FALSE;

    for ( ; ; )
    {
        c = getc( fp );
        if ( c == EOF )
            break;

        if ( c == '#' )
        {
            at_hash = TRUE;
            continue;
        }

        if ( at_hash && c == '0' )
        {
            /* Peek at next char — must be whitespace/EOF to confirm #0 */
            signed char next = getc( fp );
            if ( next == EOF || isspace( next ) )
            {
                if ( next != EOF )
                    ungetc( next, fp );
                break;
            }
            /* Not really #0, keep going */
        }

        at_hash = FALSE;
    }

    return;
}

void skip_section_mobiles( FILE *fp )
{
    skip_section_vnum_list( fp );
}

void skip_section_objects( FILE *fp )
{
    skip_section_vnum_list( fp );
}

/*
 * Skip #RESETS or #PROGS section (read words until lone "S").
 */
static void skip_section_word_until_s( FILE *fp )
{
    char *word;

    for ( ; ; )
    {
        word = fread_word( fp );
        if ( word[0] == 'S' && word[1] == '\0' )
            break;
        fread_to_eol( fp );
    }

    return;
}

void skip_section_resets( FILE *fp )
{
    skip_section_word_until_s( fp );
}

void skip_section_progs( FILE *fp )
{
    skip_section_word_until_s( fp );
}

/*
 * Skip #SHOPS section (read numbers until 0).
 */
void skip_section_shops( FILE *fp )
{
    for ( ; ; )
    {
        int keeper = fread_number( fp );
        if ( keeper == 0 )
            break;
        fread_to_eol( fp );
    }

    return;
}

/*
 * Skip an unknown section by scanning for the next '#' at start of line
 * followed by an uppercase letter or '$'.
 */
void skip_section_unknown( FILE *fp )
{
    signed char c;
    bool at_line_start = TRUE;

    for ( ; ; )
    {
        c = getc( fp );
        if ( c == EOF )
            break;

        if ( at_line_start && c == '#' )
        {
            signed char next = getc( fp );
            if ( next == EOF )
                break;
            if ( isupper( next ) || next == '$' )
            {
                ungetc( next, fp );
                ungetc( '#', fp );
                break;
            }
            /* Not a section header, keep going */
        }

        at_line_start = ( c == '\n' || c == '\r' );
    }

    return;
}


/***************************************************************************
 *                                                                         *
 *                       MEMORY MANAGEMENT                                 *
 *                                                                         *
 ***************************************************************************/

#define MAX_PERM_BLOCK  131072

int nAllocPerm;
int sAllocPerm;

/*
 * Allocate some memory (may be freed later with free_mem).
 */
void *alloc_mem( int sMem )
{
    void *pMem;

    if ( !( pMem = calloc( sMem, sizeof(char) ) ) )
    {
        perror( "alloc_mem: calloc failure" );
        exit( 1 );
    }

    return pMem;
}

/*
 * Free some memory.
 *
 * Pass the ADDRESS of the pointer (e.g., free_mem(&ptr)).
 * The pointer is set to NULL after freeing.
 */
void free_mem( void *pMemPtr )
{
    void **tmp = (void **)pMemPtr;
    void *pMem = *tmp;

    free( pMem );
    *tmp = NULL;
    return;
}

/*
 * Allocate permanent memory (never freed).
 * Uses a slab allocator for efficiency.
 */
void *alloc_perm( int sMem )
{
    static char *pMemPerm;
    static int   iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
        sMem++;

    if ( sMem > MAX_PERM_BLOCK )
    {
        bug( "alloc_perm: %d too large.", sMem );
        exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
        iMemPerm = 0;
        if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
        {
            perror( "alloc_perm" );
            exit( 1 );
        }
    }

    pMem = pMemPerm + iMemPerm;
    iMemPerm += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}


/*
 * Translates room virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
    ROOM_INDEX_DATA *pRoomIndex;

    for ( pRoomIndex = room_index_hash[vnum % MAX_KEY_HASH];
          pRoomIndex != NULL;
          pRoomIndex = pRoomIndex->next )
    {
        if ( pRoomIndex->vnum == vnum )
            return pRoomIndex;
    }

    return NULL;
}

/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;

    *ch                  = ch_zero;
    ch->name             = &str_empty[0];
    ch->short_descr      = &str_empty[0];
    ch->long_descr       = &str_empty[0];
    ch->description      = &str_empty[0];
    ch->logon            = current_time;
    ch->lines            = 22;  /* PAGELEN */
    ch->comm             = 0;
    ch->position         = POS_STANDING;
    return;
}

/*
 * Free a character.
 */
void free_char( CHAR_DATA *ch )
{
    sh_int i;

    if ( ch->name )
        free_string( &ch->name );
    if ( ch->short_descr )
        free_string( &ch->short_descr );
    if ( ch->long_descr )
        free_string( &ch->long_descr );
    if ( ch->description )
        free_string( &ch->description );

    if ( ch->pcdata != NULL )
    {
        for ( i = 0; i < MAX_ALIAS; i++ )
        {
            if ( ch->pcdata->alias[i] == NULL
            ||   ch->pcdata->alias_sub[i] == NULL )
                break;

            free_string( &ch->pcdata->alias[i] );
            free_string( &ch->pcdata->alias_sub[i] );
        }

        if ( ch->pcdata->pwd )
            free_string( &ch->pcdata->pwd );
        if ( ch->pcdata->title )
            free_string( &ch->pcdata->title );
        if ( ch->pcdata->prompt )
            free_string( &ch->pcdata->prompt );

        /* Don't free pcdata itself -- it was alloc_perm'd. */
    }

    /* Don't free ch itself -- it was alloc_perm'd. */
    return;
}


/***************************************************************************
 *                                                                         *
 *                         FILE I/O UTILITIES                              *
 *                                                                         *
 ***************************************************************************/

/*
 * Read a letter from a file.
 */
char fread_letter( FILE *fp )
{
    signed char c;

    do
    {
        c = getc( fp );
    }
    while ( isspace( c ) );

    return c;
}

/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    signed char c;

    do
    {
        c = getc( fp );
    }
    while ( isspace( c ) );

    number = 0;

    sign = FALSE;
    if ( c == '+' )
    {
        c = getc( fp );
    }
    else if ( c == '-' )
    {
        sign = TRUE;
        c = getc( fp );
    }

    if ( !isdigit( c ) )
    {
        bug( "Fread_number: bad format.", 0 );
        exit( 1 );
    }

    while ( isdigit( c ) )
    {
        number = number * 10 + c - '0';
        c = getc( fp );
    }

    if ( sign )
        number = 0 - number;

    if ( c == '|' )
        number += fread_number( fp );
    else if ( c != ' ' )
        ungetc( c, fp );

    return number;
}

/*
 * Read a flag value from a file (supports alpha flag encoding).
 */
long fread_flag( FILE *fp )
{
    int number;
    signed char c;
    long bitsum;
    char i;

    do
    {
        c = getc( fp );
    }
    while ( isspace( c ) );

    number = 0;

    if ( !isdigit( c ) && c != '-' )
    {
        while ( ( 'A' <= c && c <= 'Z' ) || ( 'a' <= c && c <= 'z' ) )
        {
            /* Inline flag_convert */
            bitsum = 0;
            if ( 'A' <= c && c <= 'Z' )
            {
                bitsum = 1;
                for ( i = c; i > 'A'; i-- )
                    bitsum *= 2;
            }
            else if ( 'a' <= c && c <= 'z' )
            {
                bitsum = 67108864;  /* 2^26 */
                for ( i = c; i > 'a'; i-- )
                    bitsum *= 2;
            }
            number += bitsum;
            c = getc( fp );
        }
    }

    if ( c == '-' )
    {
        number = fread_number( fp );
        return -number;
    }

    while ( isdigit( c ) )
    {
        number = number * 10 + c - '0';
        c = getc( fp );
    }

    if ( c == '|' )
        number += fread_flag( fp );
    else if ( c != ' ' )
        ungetc( c, fp );

    return number;
}

/* fread_string_eol is defined in ssm.c */

/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    signed char c;

    do
    {
        c = getc( fp );
    }
    while ( c != '\n' && c != '\r' );

    do
    {
        c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetc( c, fp );
    return;
}

/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp )
{
    static signed char word[MAX_INPUT_LENGTH];
    signed char *pword;
    signed char cEnd;

    do
    {
        cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
        pword = word;
    }
    else
    {
        word[0] = cEnd;
        pword = word + 1;
        cEnd = ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
        *pword = getc( fp );

        if ( cEnd == ' ' ? isspace( *pword ) : *pword == cEnd )
        {
            if ( cEnd == ' ' )
            {
                ungetc( *pword, fp );
            }

            *pword = '\0';
            return (char *) word;
        }
    }

    bug( "Fread_word: word too long.", 0 );
    exit( 1 );
    return NULL;
}


/***************************************************************************
 *                                                                         *
 *                         NUMBER UTILITIES                                *
 *                                                                         *
 ***************************************************************************/

/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */
static int rgiState[2 + 55];

void init_mm( void )
{
    int *piState;
    int iState;

    piState = &rgiState[2];

    piState[-2] = 55 - 55;
    piState[-1] = 55 - 24;

    piState[0]  = ( (int) current_time ) & ( (1 << 30) - 1 );
    piState[1]  = 1;
    for ( iState = 2; iState < 55; iState++ )
    {
        piState[iState] = ( piState[iState - 1] + piState[iState - 2] )
            & ( (1 << 30) - 1 );
    }
    return;
}

int number_mm( void )
{
    int *piState;
    int iState1;
    int iState2;
    int iRand;

    piState = &rgiState[2];
    iState1 = piState[-2];
    iState2 = piState[-1];
    iRand   = ( piState[iState1] + piState[iState2] )
        & ( (1 << 30) - 1 );
    piState[iState1] = iRand;
    if ( ++iState1 == 55 )
        iState1 = 0;
    if ( ++iState2 == 55 )
        iState2 = 0;
    piState[-2] = iState1;
    piState[-1] = iState2;

    return iRand >> 6;
}

/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
    switch ( number_bits( 2 ) )
    {
    case 0:
        number -= 1;
        break;
    case 3:
        number += 1;
        break;
    }

    return UMAX( 1, number );
}

/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
    int power;
    int number;

    if ( from == 0 && to == 0 )
        return 0;

    if ( ( to = to - from + 1 ) <= 1 )
        return from;

    for ( power = 2; power < to; power <<= 1 )
        ;

    while ( ( number = number_mm() & ( power - 1 ) ) >= to )
        ;

    return from + number;
}

/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    int percent;

    while ( ( percent = number_mm() & ( 128 - 1 ) ) > 99 )
        ;

    return 1 + percent;
}

/*
 * Generate a random door.
 */
int number_door( void )
{
    int door;

    while ( ( door = number_mm() & ( 8 - 1 ) ) > 5 )
        ;

    return door;
}

int number_bits( int width )
{
    return number_mm() & ( ( 1 << width ) - 1 );
}

/*
 * Roll some dice.
 */
int dice( int number, int size )
{
    int idice;
    int sum;

    switch ( size )
    {
    case 0:
        return 0;
    case 1:
        return number;
    }

    for ( idice = 0, sum = 0; idice < number; idice++ )
        sum += number_range( 1, size );

    return sum;
}


/***************************************************************************
 *                                                                         *
 *                         STRING UTILITIES                                *
 *                                                                         *
 ***************************************************************************/

/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
        if ( *str == '~' )
            *str = '-';
    }

    return;
}

/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
        bug( "Str_cmp: null astr.", 0 );
        return TRUE;
    }

    if ( bstr == NULL )
    {
        bug( "Str_cmp: null bstr.", 0 );
        return TRUE;
    }

    for ( ; *astr || *bstr; astr++, bstr++ )
    {
        if ( LOWER( *astr ) != LOWER( *bstr ) )
            return TRUE;
    }

    return FALSE;
}

/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( !astr || !bstr || !*astr || !*bstr )
    {
        bug( "Str_prefix: null astr or bstr.", 0 );
        return TRUE;
    }

    while ( LOWER( *astr ) == LOWER( *bstr ) )
    {
        if ( !*astr++ || !*bstr++ )
        {
            astr--;
            break;
        }
    }

    if ( !*astr )
        return FALSE;
    else
        return TRUE;
}

/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ( ( c0 = LOWER( astr[0] ) ) == '\0' )
        return FALSE;

    sstr1 = strlen( astr );
    sstr2 = strlen( bstr );

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
        if ( c0 == LOWER( bstr[ichar] )
        &&   !str_prefix( astr, bstr + ichar ) )
            return FALSE;
    }

    return TRUE;
}

/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen( astr );
    sstr2 = strlen( bstr );
    if ( sstr1 <= sstr2
    &&   !str_cmp( astr, bstr + sstr2 - sstr1 ) )
        return FALSE;
    else
        return TRUE;
}

/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
        strcap[i] = LOWER( str[i] );
    strcap[i] = '\0';
    strcap[0] = UPPER( strcap[0] );
    return strcap;
}


/***************************************************************************
 *                                                                         *
 *                            LOGGING                                      *
 *                                                                         *
 ***************************************************************************/

/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
    FILE *fp;

    if ( IS_NPC( ch ) || str[0] == '\0' )
        return;

    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
        perror( file );
        send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
        fprintf( fp, "[%5d] %s: %s\n",
            ch->in_room ? ch->in_room->vnum : 0, ch->name, str );
        fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*
 * Reports a bug.
 */
void bug( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];

    sprintf( buf, "[*****] BUG: %s %d", str, param );
    log_string( buf );
    return;
}

/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
    char *strtime;

    strtime                    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
#if defined(WIN32)
    fprintf( stdout, "%s :: %s\n", strtime, str );
#else
    fprintf( stderr, "%s :: %s\n", strtime, str );
#endif
    return;
}

/*
 * Writes a string to the log accepting printf-style arguments.
 */
void logf_string( char *fmt, ... )
{
    char buf[MAX_STRING_LENGTH * 4];
    va_list param;

    va_start( param, fmt );
    vsprintf( buf, fmt, param );
    va_end( param );

    log_string( buf );
    return;
}

/* update_last is defined in comm.c */

/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}
