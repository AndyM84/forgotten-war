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
#include <time.h>
#include "merc.h"

/*
 * Direction names, indexed by DIR_NORTH .. DIR_DOWN.
 */
const char *dir_name[] = {
	"north", "east", "south", "west", "up", "down"
};

/*
 * Reverse direction, indexed by DIR_NORTH .. DIR_DOWN.
 */
const int rev_dir[] = {
	DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP
};

/*
 * Sector type names, indexed by SECT_* constants.
 */
const char *sector_name[] = {
	"inside", "city", "field", "forest", "hills",
	"mountain", "water (swim)", "water (no swim)",
	"unused", "air", "desert"
};
