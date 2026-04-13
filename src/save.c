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
#include <time.h>
#include "merc.h"
#include "db.h"

/*
 * Reserved file handle -- kept open so we can always guarantee at least
 * one fd is available for writing player files.  We close it before
 * saving and reopen it afterward.
 */
extern FILE *fpReserve;

/*
 * Local function prototypes.
 */
void fwrite_char args((CHAR_DATA *ch, FILE *fp));
void fread_char  args((CHAR_DATA *ch, FILE *fp));

#define PAGELEN  22

/*
 * save_char_obj() - Save a player character to disk.
 *
 * Writes to a temporary file first, then renames to the final path
 * to avoid partial-write corruption.
 */
void save_char_obj(CHAR_DATA *ch)
{
	char strsave[MAX_INPUT_LENGTH];
	char strtmp[MAX_INPUT_LENGTH];
	FILE *fp;

	if (IS_NPC(ch))
		return;

	sprintf(strsave, "%s%s", player_dir, capitalize(ch->name));
	sprintf(strtmp, "%s", player_temp);

	fclose(fpReserve);

	if ((fp = fopen(strtmp, "w")) == NULL) {
		bug("save_char_obj: fopen", 0);
		perror(strtmp);
		fpReserve = fopen(NULL_FILE, "r");
		return;
	}

	fwrite_char(ch, fp);
	fprintf(fp, "#END\n");
	fclose(fp);

	/* Atomically replace the old save file. */
	remove(strsave);
	rename(strtmp, strsave);

	fpReserve = fopen(NULL_FILE, "r");
	return;
}

/*
 * fwrite_char() - Write all character fields to the given FILE.
 */
void fwrite_char(CHAR_DATA *ch, FILE *fp)
{
	int i;

	fprintf(fp, "#PLAYER\n");

	fprintf(fp, "Name %s~\n", ch->name);
	fprintf(fp, "ShD  %s~\n", ch->short_descr);
	fprintf(fp, "LnD  %s~\n", ch->long_descr);
	fprintf(fp, "Desc %s~\n", ch->description);
	fprintf(fp, "Sex  %d\n",  ch->sex);
	fprintf(fp, "Levl %d\n",  ch->level);
	fprintf(fp, "Tru  %d\n",  ch->trust);
	fprintf(fp, "Plyd %d\n",
		ch->played + (int)(current_time - ch->logon));
	fprintf(fp, "Scro %d\n",  ch->lines);
	fprintf(fp, "Room %d\n",
		ch->in_room ? ch->in_room->vnum : ROOM_VNUM_TEMPLE);
	fprintf(fp, "Act  %ld\n", ch->act);
	fprintf(fp, "Comm %ld\n", ch->comm);
	fprintf(fp, "Pos  %d\n",
		ch->position == POS_FIGHTING ? POS_STANDING : ch->position);
	fprintf(fp, "InvL %d\n",  ch->invis_level);

	/* PC-specific data. */
	if (ch->pcdata != NULL) {
		fprintf(fp, "Pass %s~\n", ch->pcdata->pwd);
		fprintf(fp, "Titl %s~\n", ch->pcdata->title);
		fprintf(fp, "Prom %s~\n", ch->pcdata->prompt);

		for (i = 0; i < MAX_ALIAS; i++) {
			if (ch->pcdata->alias[i] != NULL
				&& ch->pcdata->alias[i][0] != '\0'
				&& ch->pcdata->alias_sub[i] != NULL
				&& ch->pcdata->alias_sub[i][0] != '\0')
			{
				fprintf(fp, "Alias %s~ %s~\n",
					ch->pcdata->alias[i],
					ch->pcdata->alias_sub[i]);
			}
		}
	}

	fprintf(fp, "End\n\n");
	return;
}

/*
 * load_char_obj() - Load a player character from disk.
 *
 * Allocates a new CHAR_DATA and PC_DATA, sets defaults, and attempts
 * to read the player file.  The character is linked to the descriptor
 * but NOT added to char_list or player_list (nanny handles that).
 *
 * Returns TRUE if an existing player file was found, FALSE otherwise.
 */
bool load_char_obj(DESCRIPTOR_DATA *d, char *name)
{
	char strsave[MAX_INPUT_LENGTH];
	CHAR_DATA *ch;
	PC_DATA *pcdata;
	FILE *fp;
	bool found;
	int i;

	ch = (CHAR_DATA *)alloc_perm(sizeof(*ch));
	clear_char(ch);

	pcdata = (PC_DATA *)alloc_perm(sizeof(*pcdata));
	memset(pcdata, 0, sizeof(*pcdata));

	ch->pcdata = pcdata;

	/* Set defaults. */
	ch->name         = str_dup(name);
	ch->short_descr  = str_dup("");
	ch->long_descr   = str_dup("");
	ch->description  = str_dup("");
	ch->level        = 0;
	ch->trust        = 0;
	ch->lines        = PAGELEN;
	ch->position     = POS_STANDING;
	ch->logon        = current_time;
	ch->played       = 0;
	ch->act          = 0;
	ch->comm         = 0;
	ch->invis_level  = 0;
	ch->sex          = SEX_NEUTRAL;

	pcdata->pwd      = str_dup("");
	pcdata->title    = str_dup("");
	pcdata->prompt   = str_dup("> ");
	pcdata->buffer   = new_buf();
	pcdata->confirm_delete = FALSE;

	for (i = 0; i < MAX_ALIAS; i++) {
		pcdata->alias[i]     = NULL;
		pcdata->alias_sub[i] = NULL;
	}

	/* Link descriptor and character. */
	d->character = ch;
	ch->desc     = d;

	/* Try to open the player file. */
	found = FALSE;
	sprintf(strsave, "%s%s", player_dir, capitalize(name));

	fclose(fpReserve);

	if ((fp = fopen(strsave, "r")) != NULL) {
		char *word;
		bool done = FALSE;

		/*
		 * Scan for the #PLAYER section.
		 */
		while (!done) {
			int c;

			/* Skip whitespace / newlines. */
			c = getc(fp);
			if (c == EOF) {
				done = TRUE;
				break;
			}

			if (c == '#') {
				word = fread_word(fp);
				if (!str_cmp(word, "PLAYER")) {
					fread_char(ch, fp);
					found = TRUE;
				} else if (!str_cmp(word, "END")) {
					done = TRUE;
				}
			}
		}

		fclose(fp);
	}

	fpReserve = fopen(NULL_FILE, "r");

	/*
	 * If no file was found, the name is already set from defaults.
	 * The caller (nanny) will detect found==FALSE and run new-char creation.
	 */
	return found;
}

/*
 * fread_char() - Read character data from a player file.
 *
 * Uses the classic MUD keyword-dispatch loop.
 */
void fread_char(CHAR_DATA *ch, FILE *fp)
{
	char *word;
	bool fMatch;

	for (;;) {
		word = fread_word(fp);
		fMatch = FALSE;

		if (feof(fp)) {
			bug("fread_char: EOF without End marker.", 0);
			return;
		}

		if (!str_cmp(word, "End"))
			return;

		switch (UPPER(word[0])) {

		case 'A':
			if (!str_cmp(word, "Act")) {
				ch->act = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Alias")) {
				int i;
				char *alias;
				char *sub;

				alias = fread_string(fp);
				sub   = fread_string(fp);

				/* Find an empty alias slot. */
				for (i = 0; i < MAX_ALIAS; i++) {
					if (ch->pcdata->alias[i] == NULL) {
						ch->pcdata->alias[i]     = alias;
						ch->pcdata->alias_sub[i] = sub;
						break;
					}
				}

				/* If no slot found, discard. */
				if (i >= MAX_ALIAS) {
					free_string(&alias);
					free_string(&sub);
				}

				fMatch = TRUE;
				break;
			}
			break;

		case 'C':
			if (!str_cmp(word, "Comm")) {
				ch->comm = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'D':
			if (!str_cmp(word, "Desc")) {
				free_string(&ch->description);
				ch->description = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'I':
			if (!str_cmp(word, "InvL")) {
				ch->invis_level = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'L':
			if (!str_cmp(word, "Levl")) {
				ch->level = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "LnD")) {
				free_string(&ch->long_descr);
				ch->long_descr = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'N':
			if (!str_cmp(word, "Name")) {
				free_string(&ch->name);
				ch->name = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'P':
			if (!str_cmp(word, "Pass")) {
				free_string(&ch->pcdata->pwd);
				ch->pcdata->pwd = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Plyd")) {
				ch->played = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Pos")) {
				ch->position = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Prom")) {
				free_string(&ch->pcdata->prompt);
				ch->pcdata->prompt = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'R':
			if (!str_cmp(word, "Room")) {
				int vnum;
				ROOM_INDEX_DATA *room;

				vnum = fread_number(fp);
				room = get_room_index(vnum);
				if (room == NULL)
					room = get_room_index(ROOM_VNUM_TEMPLE);
				ch->in_room = room;
				fMatch = TRUE;
				break;
			}
			break;

		case 'S':
			if (!str_cmp(word, "Sex")) {
				ch->sex = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "ShD")) {
				free_string(&ch->short_descr);
				ch->short_descr = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Scro")) {
				ch->lines = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'T':
			if (!str_cmp(word, "Tru")) {
				ch->trust = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Titl")) {
				free_string(&ch->pcdata->title);
				ch->pcdata->title = fread_string(fp);
				fMatch = TRUE;
				break;
			}
			break;
		}

		if (!fMatch) {
			bug("fread_char: no match for keyword.", 0);
			fread_to_eol(fp);
		}
	}
}
