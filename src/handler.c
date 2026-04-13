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

/*
 * Static buffers for date/time strings.
 */
static char thedate[32];
static char thetime[32];

/*
 * get_curtime() - Return current time as a string, newline stripped.
 */
char *get_curtime(void)
{
	char *str;

	str = ctime(&current_time);
	strncpy(thetime, str, sizeof(thetime) - 1);
	thetime[sizeof(thetime) - 1] = '\0';

	/* Strip trailing newline. */
	str = thetime;
	while (*str != '\0') {
		if (*str == '\n' || *str == '\r') {
			*str = '\0';
			break;
		}
		str++;
	}

	return thetime;
}

/*
 * get_curdate() - Return current date in mm/dd/yy format.
 */
char *get_curdate(void)
{
	time_t tm;
	struct tm now;

	time(&tm);
	now = *localtime(&tm);

	sprintf(thedate, "%02d/%02d/%02d",
		now.tm_mon + 1, now.tm_mday, now.tm_year % 100);

	return thedate;
}

/*
 * get_trust() - Retrieve a character's trust level.
 */
int get_trust(CHAR_DATA *ch)
{
	if (ch->trust != 0)
		return ch->trust;

	return ch->level;
}

/*
 * is_name() - Check if str matches any word in namelist (prefix match).
 *
 * Classic MUD implementation: for each word in namelist, if str_prefix
 * matches on the current word, return TRUE.
 */
bool is_name(char *str, char *namelist)
{
	char name[MAX_INPUT_LENGTH];
	char part[MAX_INPUT_LENGTH];
	char *list;
	char *string;

	if (str == NULL || str[0] == '\0')
		return FALSE;

	if (namelist == NULL || namelist[0] == '\0')
		return FALSE;

	/* We need to match every word in str against some word in namelist. */
	string = str;
	for (;;) {
		/* Extract the next word from str. */
		str = one_argument(string, part);
		if (part[0] == '\0')
			return TRUE;

		/* Check if this word matches any word in namelist. */
		list = namelist;
		for (;;) {
			list = one_argument(list, name);
			if (name[0] == '\0')
				return FALSE;
			if (!str_prefix(part, name))
				return TRUE;
		}
	}
}

/*
 * is_exact_name() - Exact match version of is_name.
 */
bool is_exact_name(char *str, char *namelist)
{
	char name[MAX_INPUT_LENGTH];
	char part[MAX_INPUT_LENGTH];
	char *list;
	char *string;

	if (str == NULL || str[0] == '\0')
		return FALSE;

	if (namelist == NULL || namelist[0] == '\0')
		return FALSE;

	string = str;
	for (;;) {
		str = one_argument(string, part);
		if (part[0] == '\0')
			return TRUE;

		list = namelist;
		for (;;) {
			list = one_argument(list, name);
			if (name[0] == '\0')
				return FALSE;
			if (!str_cmp(part, name))
				return TRUE;
		}
	}
}

/*
 * char_from_room() - Remove a character from their current room.
 */
void char_from_room(CHAR_DATA *ch)
{
	CHAR_DATA *prev;

	if (ch->in_room == NULL) {
		bug("char_from_room: NULL.", 0);
		return;
	}

	/* Unlink from the room's people list. */
	if (ch == ch->in_room->people) {
		ch->in_room->people = ch->next_in_room;
	} else {
		for (prev = ch->in_room->people; prev != NULL;
			prev = prev->next_in_room)
		{
			if (prev->next_in_room == ch) {
				prev->next_in_room = ch->next_in_room;
				break;
			}
		}

		if (prev == NULL)
			bug("char_from_room: ch not found.", 0);
	}

	ch->in_room      = NULL;
	ch->next_in_room = NULL;
	return;
}

/*
 * char_to_room() - Place a character into a room.
 */
void char_to_room(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex)
{
	if (pRoomIndex == NULL) {
		ROOM_INDEX_DATA *room;

		bug("char_to_room: NULL room.", 0);

		room = get_room_index(ROOM_VNUM_TEMPLE);
		if (room == NULL) {
			bug("char_to_room: ROOM_VNUM_TEMPLE does not exist.", 0);
			return;
		}
		pRoomIndex = room;
	}

	ch->in_room       = pRoomIndex;
	ch->next_in_room  = pRoomIndex->people;
	pRoomIndex->people = ch;

	return;
}

/*
 * extract_char() - Remove a character from the game.
 *
 * If fPull is TRUE, the character is fully removed from char_list
 * and player_list (used for quit/linkdead cleanup).
 */
void extract_char(CHAR_DATA *ch, bool fPull)
{
	CHAR_DATA *prev;

	if (ch == NULL) {
		bug("extract_char: NULL ch.", 0);
		return;
	}

	/* Remove from room. */
	if (ch->in_room != NULL)
		char_from_room(ch);

	if (!fPull)
		return;

	/*
	 * Unlink from char_list.
	 */
	if (ch == char_list) {
		char_list = ch->next;
	} else {
		for (prev = char_list; prev != NULL; prev = prev->next) {
			if (prev->next == ch) {
				prev->next = ch->next;
				break;
			}
		}

		if (prev == NULL)
			bug("extract_char: char not found in char_list.", 0);
	}

	/*
	 * Unlink from player_list if not an NPC.
	 */
	if (!IS_NPC(ch)) {
		if (ch == player_list) {
			player_list = ch->next_player;
		} else {
			for (prev = player_list; prev != NULL;
				prev = prev->next_player)
			{
				if (prev->next_player == ch) {
					prev->next_player = ch->next_player;
					break;
				}
			}

			if (prev == NULL)
				bug("extract_char: player not found in player_list.", 0);
		}
	}

	/*
	 * Disconnect the descriptor from the character.
	 */
	if (ch->desc != NULL)
		ch->desc->character = NULL;

	/*
	 * Clear fields.
	 */
	ch->next        = NULL;
	ch->next_player = NULL;
	ch->desc        = NULL;

	return;
}

/*
 * get_char_room() - Find a character in the same room by name.
 */
CHAR_DATA *get_char_room(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *rch;
	int number;
	int count;

	if (ch == NULL || ch->in_room == NULL)
		return NULL;

	number = number_argument(argument, arg);
	if (arg[0] == '\0')
		return NULL;

	count = 0;
	for (rch = ch->in_room->people; rch != NULL;
		rch = rch->next_in_room)
	{
		if (!can_see(ch, rch))
			continue;
		if (!is_name(arg, rch->name))
			continue;
		if (++count == number)
			return rch;
	}

	return NULL;
}

/*
 * get_player_world() - Find a player anywhere in the world by name.
 */
CHAR_DATA *get_player_world(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *wch;
	int count;
	int number;

	number = number_argument(argument, arg);
	if (arg[0] == '\0')
		return NULL;

	count = 0;
	for (wch = player_list; wch != NULL; wch = wch->next_player) {
		if (!can_see(ch, wch))
			continue;
		if (!is_name(arg, wch->name))
			continue;
		if (++count == number)
			return wch;
	}

	return NULL;
}

/*
 * can_see() - Simplified visibility check.
 *
 * If the victim's invis_level exceeds the looker's trust, they cannot
 * be seen.
 */
bool can_see(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (ch == victim)
		return TRUE;

	if (victim->invis_level > get_trust(ch))
		return FALSE;

	return TRUE;
}

/*
 * can_see_room() - Simplified room visibility check.
 *
 * Always returns TRUE in this stripped-down core.
 */
bool can_see_room(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex)
{
	return TRUE;
}

/*
 * room_is_dark() - Check if a room is dark.
 */
bool room_is_dark(ROOM_INDEX_DATA *pRoomIndex)
{
	if (pRoomIndex == NULL)
		return FALSE;

	if (IS_SET(pRoomIndex->room_flags, ROOM_DARK)
		&& pRoomIndex->light < 1)
		return TRUE;

	return FALSE;
}

/*
 * room_is_private() - Check if a room is private (occupancy limit).
 */
bool room_is_private(ROOM_INDEX_DATA *pRoomIndex)
{
	CHAR_DATA *rch;
	int count;

	if (pRoomIndex == NULL)
		return FALSE;

	count = 0;
	for (rch = pRoomIndex->people; rch != NULL;
		rch = rch->next_in_room)
	{
		count++;
	}

	if (IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE) && count >= 2)
		return TRUE;

	if (IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1)
		return TRUE;

	return FALSE;
}

/*
 * reset_char() - Stub for game-specific stats recalculation.
 *
 * In a full MUD this would recalculate hitroll, damroll, saves, etc.
 * from equipment and affects. Left as a no-op in this stripped core.
 */
void reset_char(CHAR_DATA *ch)
{
	return;
}
