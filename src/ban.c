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
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

BAN_DATA *ban_list;

extern FILE *fpReserve;

#define BAN_FILE "../area/ban.txt"

/*
 * save_bans() - Write permanent bans to disk.
 */
void save_bans(void)
{
	BAN_DATA *pban;
	FILE *fp;
	bool found = FALSE;

	fclose(fpReserve);
	if ((fp = fopen(BAN_FILE, "w")) == NULL) {
		perror(BAN_FILE);
		fpReserve = fopen(NULL_FILE, "r");
		return;
	}

	for (pban = ban_list; pban != NULL; pban = pban->next) {
		if (IS_SET(pban->ban_flags, BAN_PERMANENT)) {
			found = TRUE;
			fprintf(fp, "%-20s %-2d %d\n",
				pban->name, pban->level, pban->ban_flags);
		}
	}

	fclose(fp);
	fpReserve = fopen(NULL_FILE, "r");

	if (!found)
		unlink(BAN_FILE);
}

/*
 * load_bans() - Read bans from disk.
 */
void load_bans(void)
{
	FILE *fp;
	BAN_DATA *ban_last;

	if ((fp = fopen(BAN_FILE, "r")) == NULL)
		return;

	ban_last = NULL;
	for (;;) {
		BAN_DATA *pban;

		if (feof(fp)) {
			fclose(fp);
			return;
		}

		pban = new_ban();

		pban->name      = str_dup(fread_word(fp));
		pban->level     = fread_number(fp);
		pban->ban_flags = fread_flag(fp);
		fread_to_eol(fp);

		if (ban_list == NULL)
			ban_list = pban;
		else
			ban_last->next = pban;
		ban_last = pban;
	}
}

/*
 * check_ban() - Check if a descriptor's host is banned.
 *
 * Called when a new connection is accepted.  If the site is banned,
 * the descriptor is closed.
 */
void check_ban(DESCRIPTOR_DATA *d)
{
	BAN_DATA *pban;
	char host[MAX_STRING_LENGTH];

	if (d == NULL || d->host == NULL)
		return;

	strcpy(host, capitalize(d->host));
	host[0] = LOWER(host[0]);

	for (pban = ban_list; pban != NULL; pban = pban->next) {
		if (!IS_SET(pban->ban_flags, BAN_ALL))
			continue;

		if (IS_SET(pban->ban_flags, BAN_PREFIX)
			&& IS_SET(pban->ban_flags, BAN_SUFFIX)
			&& strstr(host, pban->name) != NULL)
		{
			write_to_buffer(d, "Your site has been banned.\n\r", 0);
			close_socket(d);
			return;
		}

		if (IS_SET(pban->ban_flags, BAN_PREFIX)
			&& !str_suffix(pban->name, host))
		{
			write_to_buffer(d, "Your site has been banned.\n\r", 0);
			close_socket(d);
			return;
		}

		if (IS_SET(pban->ban_flags, BAN_SUFFIX)
			&& !str_prefix(pban->name, host))
		{
			write_to_buffer(d, "Your site has been banned.\n\r", 0);
			close_socket(d);
			return;
		}
	}
}
