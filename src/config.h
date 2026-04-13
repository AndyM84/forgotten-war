#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONFIG_FILE "ember.cfg"

/* Connection messages */
#define CFG_QUIT "Alas, all good things must come to an end.\n\r"
#define CFG_CONNECT_MSG "Welcome to a MUD based on EmberMUD.\n\r"
#define CFG_ASK_ANSI "Use ANSI Color? [Y/n]: "

/*
 * Channel/communication format strings.
 * $n = The speaker's name
 * $t = The text
 */
#define CFG_SAY        "`G$n says `g'`G$t`g'`0"
#define CFG_SAY_SELF   "`GYou say `g'`G$t`g'`0"

/*
 * Shared String Manager (ssm.c) configuration.
 */
#define MAX_CHUNKS                 80

/*
 * Connection states for the nanny() state machine.
 */
#define CON_PLAYING              0
#define CON_GET_NAME             1
#define CON_GET_OLD_PASSWORD     2
#define CON_CONFIRM_NEW_NAME     3
#define CON_GET_NEW_PASSWORD     4
#define CON_CONFIRM_NEW_PASSWORD 5
#define CON_GET_NEW_ROLE         6
#define CON_READ_MOTD            7
#define CON_BREAK_CONNECT        8
#define CON_GET_ANSI             9

/*
 * Configurable directory and file paths.
 * These can be overridden in the config file.
 */
#define CFG_AREA_DIR      "./area/"
#define CFG_PLAYER_DIR    "./player/"
#define CFG_PLAYER_TEMP   "./player/temp.tmp"
#define CFG_LOG_DIR       "./log/"
#define CFG_HELP_FILE     "help.area"
#define CFG_BUG_FILE      "./log/bugs.txt"
#define CFG_TYPO_FILE     "./log/typos.txt"
#define CFG_SHUTDOWN_FILE "./area/shutdown.txt"

/*
 * Runtime-configurable paths (set by load_config_file).
 */
extern char area_dir[];
extern char player_dir[];
extern char player_temp[];
extern char log_dir[];
extern char help_file[];
extern char bug_file[];

#endif /* _CONFIG_H_ */
