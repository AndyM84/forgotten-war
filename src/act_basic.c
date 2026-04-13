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
#include <time.h>
#include "merc.h"
#include "interp.h"

/*
 * Extern globals.
 */
extern const char *dir_name[];
extern const int rev_dir[];
extern char log_buf[2 * MAX_INPUT_LENGTH];
extern DESCRIPTOR_DATA *descriptor_list;
extern HELP_DATA *help_first;

/*
 * Local function prototypes.
 */
static void move_char args((CHAR_DATA *ch, int door));
static void show_char_to_char args((CHAR_DATA *list, CHAR_DATA *ch));
static void show_exits_to_char args((CHAR_DATA *ch));


/*
 * substitute_alias() - Check for alias expansion before interpreting.
 *
 * If the first word of input matches one of the player's aliases,
 * replace it with the alias_sub and interpret.  Otherwise, just
 * interpret the original input.
 */
void substitute_alias(DESCRIPTOR_DATA *d, char *input)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

	CHAR_DATA *ch = d->character;

	if (ch == NULL || IS_NPC(ch) || ch->pcdata == NULL) {
		if (ch != NULL) {
			interpret(ch, input);
		}

		return;
	}

	/* Extract the first word. */
	one_argument(input, arg);

	/* Check aliases. */
	for (int alias = 0; alias < MAX_ALIAS; alias++) {
		if (ch->pcdata->alias[alias] == NULL) {
			break;
		}

		if (!str_cmp(arg, ch->pcdata->alias[alias])) {
			/* Found a match -- substitute. */

			if (strlen(ch->pcdata->alias_sub[alias]) + strlen(input) > MAX_STRING_LENGTH - 1)
			{
				send_to_char("Alias substitution too long.\n\r", ch);

				return;
			}

			/* Skip past the first word in the original input. */
			char *point = input;

			while (*point != '\0' && !isspace((int)*point)) {
				point++;
			}

			while (isspace((int)*point)) {
				point++;
			}

			sprintf(buf, "%s %s", ch->pcdata->alias_sub[alias], point);
			interpret(ch, buf);

			return;
		}
	}

	interpret(ch, input);

	return;
}

/*
 * set_title() - Set a player's title string.
 */
void set_title(CHAR_DATA *ch, char *title)
{
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch)) {
		bug("set_title: NPC.", 0);

		return;
	}

	if (ch->pcdata == NULL) {
		return;
	}

	/* Ensure title always starts with a space for clean display. */
	if (title[0] != ' ') {
		sprintf(buf, " %s", title);
		title = buf;
	}

	free_string(&ch->pcdata->title);
	ch->pcdata->title = str_dup(title);

	return;
}

/*
 * check_social() - Stub for social command lookup.
 *
 * In a full MUD this would search the social table and perform the
 * appropriate emote.  Returns FALSE (no match) in this stripped core.
 */
bool check_social(CHAR_DATA *ch, char *command, char *argument)
{
	return FALSE;
}


/***************************************************************************
 *                                                                         *
 *                         INFORMATIONAL COMMANDS                          *
 *                                                                         *
 ***************************************************************************/

/*
 * show_exits_to_char() - Display auto-exit line for a character.
 */
static void show_exits_to_char(CHAR_DATA *ch)
{
	char buf[MAX_STRING_LENGTH];

	sprintf(buf, "`C[Exits:");

	bool found = FALSE;

	for (int door = 0; door < MAX_DIR; door++) {
		EXIT_DATA *pexit = ch->in_room->exit[door];

		if (pexit != NULL && pexit->u1.to_room != NULL && can_see_room(ch, pexit->u1.to_room)) {
			found = TRUE;

			if (IS_SET(pexit->exit_info, EX_CLOSED)) {
				strcat(buf, " (");
				strcat(buf, dir_name[door]);
				strcat(buf, ")");
			} else {
				strcat(buf, " ");
				strcat(buf, dir_name[door]);
			}
		}
	}

	if (!found) {
		strcat(buf, " none");
	}

	strcat(buf, "]`0\n\r");
	send_to_char(buf, ch);

	return;
}

/*
 * show_char_to_char() - Show all other characters in the room to ch.
 */
static void show_char_to_char(CHAR_DATA *list, CHAR_DATA *ch)
{
	char buf[MAX_STRING_LENGTH];

	for (CHAR_DATA *rch = list; rch != NULL; rch = rch->next_in_room) {
		if (rch == ch) {
			continue;
		}

		if (!can_see(ch, rch)) {
			continue;
		}

		if (rch->long_descr != NULL && rch->long_descr[0] != '\0' && rch->position == POS_STANDING && IS_NPC(rch)) {
			send_to_char(rch->long_descr, ch);
		} else {
			sprintf(buf, "%s%s is here.\n\r",
				PERS(rch, ch),
				(rch->position == POS_RESTING)  ? " (Resting)"  :
				(rch->position == POS_SLEEPING)  ? " (Sleeping)" :
				(rch->position == POS_FIGHTING)  ? " (Fighting)" :
				(rch->position == POS_SITTING)   ? " (Sitting)"  :
				"");
			send_to_char(buf, ch);
		}
	}

	return;
}

/*
 * do_look() - Look at the room (or "auto" look on entry).
 */
void do_look(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (ch->in_room == NULL) {
		return;
	}

	if (ch->desc == NULL) {
		return;
	}

	if (ch->position < POS_SLEEPING) {
		send_to_char("You can't see anything but stars!\n\r", ch);

		return;
	}

	if (ch->position == POS_SLEEPING) {
		send_to_char("You can't see anything, you're sleeping!\n\r", ch);

		return;
	}

	if (argument[0] == '\0' || !str_cmp(argument, "auto")) {
		/* Show room name. */
		sprintf(buf, "`C%s`0\n\r", ch->in_room->name);
		send_to_char(buf, ch);

		/* Show room description if not brief or not auto-look. */
		if (argument[0] == '\0' || !IS_SET(ch->comm, COMM_BRIEF)) {
			if (ch->in_room->description != NULL && ch->in_room->description[0] != '\0') {
				send_to_char(ch->in_room->description, ch);
			}
		}

		/* Auto-exits. */
		if (IS_SET(ch->act, PLR_AUTOEXIT)) {
			show_exits_to_char(ch);
		}

		/* Show people in the room. */
		show_char_to_char(ch->in_room->people, ch);
	}

	return;
}

/*
 * do_exits() - Show verbose exit listing.
 */
void do_exits(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (ch->in_room == NULL) {
		return;
	}

	bool found = FALSE;
	buf[0] = '\0';

	for (int door = 0; door < MAX_DIR; door++) {
		EXIT_DATA *pexit = ch->in_room->exit[door];

		if (pexit != NULL && pexit->u1.to_room != NULL && can_see_room(ch, pexit->u1.to_room)) {
			found = TRUE;

			if (IS_SET(pexit->exit_info, EX_CLOSED)) {
				printf_to_char(ch, "%-5s - (closed)\n\r",
					capitalize(dir_name[door]));
			} else {
				printf_to_char(ch, "%-5s - %s\n\r",
					capitalize(dir_name[door]),
					pexit->u1.to_room->name
						? pexit->u1.to_room->name
						: "(no name)");
			}
		}
	}

	if (!found) {
		send_to_char("None.\n\r", ch);
	}

	return;
}


/***************************************************************************
 *                                                                         *
 *                       COMMUNICATION COMMANDS                            *
 *                                                                         *
 ***************************************************************************/

/*
 * do_say() - Say something to the room.
 */
void do_say(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0') {
		send_to_char("Say what?\n\r", ch);

		return;
	}

	act_new(CFG_SAY, ch, argument, NULL, TO_ROOM, POS_RESTING);
	act_new(CFG_SAY_SELF, ch, argument, NULL, TO_CHAR, POS_RESTING);

	return;
}

/*
 * do_tell() - Send a private message to another player.
 */
void do_tell(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg);

	if (arg[0] == '\0' || argument[0] == '\0') {
		send_to_char("Tell whom what?\n\r", ch);

		return;
	}

	CHAR_DATA *victim = get_player_world(ch, arg);

	if (victim == NULL) {
		send_to_char("They aren't here.\n\r", ch);

		return;
	}

	if (victim->desc == NULL && !IS_NPC(victim)) {
		act("$N seems to have misplaced $S link...try again later.", ch, NULL, victim, TO_CHAR);

		return;
	}

	act_new("$n tells you '$t'", ch, argument, victim, TO_VICT, POS_DEAD);
	printf_to_char(ch, "You tell %s '%s'\n\r", victim->name, argument);

	return;
}


/***************************************************************************
 *                                                                         *
 *                           WHO / HELP / SAVE                             *
 *                                                                         *
 ***************************************************************************/

/*
 * do_who() - Show list of visible players currently online.
 */
void do_who(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	int count;

	count = 0;

	send_to_char("`W---[ Who is Online ]---`0\n\r", ch);

	for (DESCRIPTOR_DATA *d = descriptor_list; d != NULL; d = d->next) {
		if (d->connected != CON_PLAYING)
			continue;

		CHAR_DATA *wch = d->character;

		if (wch == NULL) {
			continue;
		}

		if (!can_see(ch, wch)) {
			continue;
		}

		count++;
		sprintf(buf, "[%3d] %s%s\n\r",
			wch->level,
			wch->name,
			(wch->pcdata && wch->pcdata->title
				&& wch->pcdata->title[0] != '\0')
				? wch->pcdata->title
				: "");
		send_to_char(buf, ch);
	}

	printf_to_char(ch, "\n\r%d player%s online.\n\r",
		count, count == 1 ? "" : "s");

	return;
}

/*
 * do_quit() - Leave the game.
 */
void do_quit(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) {
		return;
	}

	if (ch->position == POS_FIGHTING) {
		send_to_char("No way! You are fighting.\n\r", ch);

		return;
	}

	send_to_char(CFG_QUIT, ch);
	act("$n has left the game.", ch, NULL, NULL, TO_ROOM);
	sprintf(log_buf, "%s has quit.", ch->name);
	log_string(log_buf);

	save_char_obj(ch);

	DESCRIPTOR_DATA *d = ch->desc;
	extract_char(ch, TRUE);

	if (d != NULL) {
		close_socket(d);
	}

	return;
}

/*
 * do_save() - Save the character to disk.
 */
void do_save(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) {
		return;
	}

	save_char_obj(ch);
	send_to_char("Saved.\n\r", ch);

	return;
}

/*
 * do_help() - Display a help topic.
 */
void do_help(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0')
		argument = "summary";

	BUFFER *output = new_buf();
	bool found = FALSE;

	for (HELP_DATA *pHelp = help_first; pHelp != NULL; pHelp = pHelp->next) {
		if (pHelp->level > get_trust(ch)) {
			continue;
		}

		if (is_name(argument, pHelp->keyword)) {
			if (pHelp->text != NULL && pHelp->text[0] != '\0') {
				/*
				 * Strip leading '.' from help text (old formatting trick).
				 */
				if (pHelp->text[0] == '.') {
					add_buf(output, pHelp->text + 1);
				} else {
					add_buf(output, pHelp->text);
				}
			}
			found = TRUE;

			break;
		}
	}

	if (!found) {
		send_to_char("No help on that word.\n\r", ch);
	} else {
		page_to_char(buf_string(output), ch);
	}

	free_buf(output);

	return;
}


/***************************************************************************
 *                                                                         *
 *                          MOVEMENT COMMANDS                              *
 *                                                                         *
 ***************************************************************************/

/*
 * move_char() - Move a character in the given direction.
 */
static void move_char(CHAR_DATA *ch, int door)
{
	if (ch->in_room == NULL) {
		return;
	}

	if (door < 0 || door >= MAX_DIR) {
		bug("move_char: bad door %d.", door);

		return;
	}

	ROOM_INDEX_DATA *in_room = ch->in_room;
	EXIT_DATA *pexit = in_room->exit[door];

	if (pexit == NULL || pexit->u1.to_room == NULL) {
		send_to_char("Alas, you cannot go that way.\n\r", ch);

		return;
	}

	ROOM_INDEX_DATA *to_room = pexit->u1.to_room;

	/* Check for closed door. */
	if (IS_SET(pexit->exit_info, EX_CLOSED)) {
		act("The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR);

		return;
	}

	/* Check for private room. */
	if (room_is_private(to_room)) {
		send_to_char("That room is private right now.\n\r", ch);

		return;
	}

	/* Announce departure. */
	act("$n leaves $T.", ch, NULL, dir_name[door], TO_ROOM);

	/* Move the character. */
	char_from_room(ch);
	char_to_room(ch, to_room);

	/* Announce arrival. */
	act("$n has arrived.", ch, NULL, NULL, TO_ROOM);

	/* Show the new room. */
	do_look(ch, "auto");

	return;
}

void do_north(CHAR_DATA *ch, char *argument)
{
	move_char(ch, DIR_NORTH);

	return;
}

void do_east(CHAR_DATA *ch, char *argument)
{
	move_char(ch, DIR_EAST);

	return;
}

void do_south(CHAR_DATA *ch, char *argument)
{
	move_char(ch, DIR_SOUTH);

	return;
}

void do_west(CHAR_DATA *ch, char *argument)
{
	move_char(ch, DIR_WEST);

	return;
}

void do_up(CHAR_DATA *ch, char *argument)
{
	move_char(ch, DIR_UP);

	return;
}

void do_down(CHAR_DATA *ch, char *argument)
{
	move_char(ch, DIR_DOWN);

	return;
}
