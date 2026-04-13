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
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* command procedures needed */
DECLARE_DO_FUN(do_quit);

/* used for auto-save cycling */
static int save_number = 0;

/*
 * char_update() - Per-tick update for all characters.
 *
 * Increments played time, checks idle timeout, and auto-saves.
 */
static void char_update(void)
{
	CHAR_DATA *ch;
	CHAR_DATA *ch_next;
	CHAR_DATA *ch_quit;

	ch_quit = NULL;

	save_number++;
	if (save_number > 30)
		save_number = 0;

	for (ch = char_list; ch != NULL; ch = ch_next) {
		ch_next = ch->next;

		if (IS_NPC(ch))
			continue;

		/* Increment played time. */
		ch->played += (int)(current_time - ch->logon);
		ch->logon = current_time;

		/* Increment idle timer. */
		ch->timer++;

		/* Idle timeout: move to limbo after 12 ticks. */
		if (ch->timer >= 12 && ch->level < LEVEL_IMMORTAL) {
			if (ch->was_in_room == NULL && ch->in_room != NULL) {
				ch->was_in_room = ch->in_room;
				act("$n disappears into the void.",
					ch, NULL, NULL, TO_ROOM);
				send_to_char("You disappear into the void.\n\r", ch);
				if (ch->level > 1)
					save_char_obj(ch);
				char_from_room(ch);
				char_to_room(ch, get_room_index(ROOM_VNUM_LIMBO));
			}
		}

		/* Auto-quit after 30 ticks idle. */
		if (ch->timer > 30 && ch->level < LEVEL_IMMORTAL)
			ch_quit = ch;
	}

	/*
	 * Auto-save and auto-quit pass.
	 */
	for (ch = player_list; ch != NULL; ch = ch_next) {
		ch_next = ch->next_player;

		if (ch->desc != NULL && save_number == 30)
			save_char_obj(ch);

		if (ch == ch_quit)
			do_quit(ch, "");
	}

	return;
}

/*
 * update_handler() - Called once per pulse from game_loop().
 *
 * In this stripped core, only the tick pulse remains.
 */
void update_handler(void)
{
	static int pulse_point;

	if (--pulse_point <= 0) {
		pulse_point = PULSE_TICK;
		char_update();
	}

	tail_chain();
	return;
}
