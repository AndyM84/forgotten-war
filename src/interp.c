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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

/*
 * Log buffer shared across files.
 */
char log_buf[2 * MAX_INPUT_LENGTH];

/*
 * Command table.
 */
const struct cmd_type cmd_table[] = {
	/*
	 * Movement commands (priority at top for speed).
	 */
	{ "north",      do_north,    POS_STANDING,  0, LOG_NEVER  },
	{ "east",       do_east,     POS_STANDING,  0, LOG_NEVER  },
	{ "south",      do_south,    POS_STANDING,  0, LOG_NEVER  },
	{ "west",       do_west,     POS_STANDING,  0, LOG_NEVER  },
	{ "up",         do_up,       POS_STANDING,  0, LOG_NEVER  },
	{ "down",       do_down,     POS_STANDING,  0, LOG_NEVER  },

	/*
	 * Common commands.
	 */
	{ "look",       do_look,     POS_RESTING,   0, LOG_NORMAL },
	{ "exits",      do_exits,    POS_RESTING,   0, LOG_NORMAL },
	{ "say",        do_say,      POS_RESTING,   0, LOG_NORMAL },
	{ "'",          do_say,      POS_RESTING,   0, LOG_NORMAL },
	{ "tell",       do_tell,     POS_RESTING,   0, LOG_NORMAL },
	{ "who",        do_who,      POS_DEAD,      0, LOG_NORMAL },
	{ "save",       do_save,     POS_DEAD,      0, LOG_NORMAL },
	{ "quit",       do_quit,     POS_DEAD,      0, LOG_NORMAL },
	{ "help",       do_help,     POS_DEAD,      0, LOG_NORMAL },

	/*
	 * End of table.
	 */
	{ "",           NULL,        POS_DEAD,      0, LOG_NORMAL }
};

/*
 * interpret() - The main command interpreter.
 *
 * Parses input and dispatches to the appropriate do_fun.
 */
void interpret(CHAR_DATA *ch, char *argument)
{
	char command[MAX_INPUT_LENGTH];
	char logline[MAX_INPUT_LENGTH];
	int cmd;
	bool found;

	/*
	 * Strip leading spaces.
	 */
	while (isspace((int)*argument))
		argument++;
	if (argument[0] == '\0')
		return;

	/*
	 * Grab the command word.
	 * Special handling for single-character commands ('say shortcut).
	 */
	strcpy(logline, argument);
	if (!isalpha((int)argument[0]) && !isdigit((int)argument[0])) {
		command[0] = argument[0];
		command[1] = '\0';
		argument++;
		while (isspace((int)*argument))
			argument++;
	} else {
		argument = one_argument(argument, command);
	}

	/*
	 * Look up the command in the command table.
	 */
	found = FALSE;
	for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++) {
		if (command[0] == cmd_table[cmd].name[0]
			&& !str_prefix(command, cmd_table[cmd].name))
		{
			/* Check position. */
			if (ch->position < cmd_table[cmd].position) {
				send_to_char("You can't do that right now.\n\r", ch);
				return;
			}

			/* Check level. */
			if (ch->level < cmd_table[cmd].level) {
				send_to_char("Huh?\n\r", ch);
				return;
			}

			/* Log if needed. */
			if (cmd_table[cmd].log == LOG_ALWAYS) {
				sprintf(log_buf, "Log %s: %s", ch->name, logline);
				log_string(log_buf);
			}

			/* Dispatch. */
			(*cmd_table[cmd].do_fun)(ch, argument);

			found = TRUE;
			break;
		}
	}

	/*
	 * If not found, check socials, then show error.
	 */
	if (!found) {
		if (!check_social(ch, command, argument))
			send_to_char("Huh?\n\r", ch);
	}

	tail_chain();
	return;
}

/*
 * is_number() - Check if a string is a valid numeric value.
 */
bool is_number(char *arg)
{
	if (*arg == '\0')
		return FALSE;

	if (*arg == '+' || *arg == '-')
		arg++;

	for (; *arg != '\0'; arg++) {
		if (!isdigit((int)*arg))
			return FALSE;
	}

	return TRUE;
}

/*
 * number_argument() - Parse "N.keyword" syntax.
 *
 * Given "3.sword", returns 3 and copies "sword" into arg.
 * If no dot, returns 1 and copies the whole string.
 */
int number_argument(char *argument, char *arg)
{
	char *pdot;
	int number;

	for (pdot = argument; *pdot != '\0'; pdot++) {
		if (*pdot == '.') {
			*pdot = '\0';
			number = atoi(argument);
			*pdot = '.';
			strcpy(arg, pdot + 1);
			return number;
		}
	}

	strcpy(arg, argument);
	return 1;
}

/*
 * one_argument() - Pick off one argument from a string.
 *
 * Returns a pointer to the rest of the string.
 * The extracted word is copied to arg_first, lowercased.
 * Handles single-quote grouping: 'two words' counts as one argument.
 */
char *one_argument(char *argument, char *arg_first)
{
	char cEnd;

	while (isspace((int)*argument))
		argument++;

	cEnd = ' ';
	if (*argument == '\'') {
		cEnd = *argument;
		argument++;
	}

	while (*argument != '\0') {
		if (*argument == cEnd) {
			argument++;
			break;
		}
		*arg_first = LOWER(*argument);
		arg_first++;
		argument++;
	}
	*arg_first = '\0';

	while (isspace((int)*argument))
		argument++;

	return argument;
}
