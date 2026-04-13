/* this is a listing of all the commands and command related data */
#ifndef _INTERP_H_
#define _INTERP_H_

/* for command types */
#define ML      MAX_LEVEL       /* implementor */
#define L1      MAX_LEVEL - 1   /* creator */
#define L2      MAX_LEVEL - 2   /* supreme being */
#define L3      MAX_LEVEL - 3   /* deity */
#define L4      MAX_LEVEL - 4   /* god */
#define L5      MAX_LEVEL - 5   /* immortal */
#define L6      MAX_LEVEL - 6   /* demigod */
#define L7      MAX_LEVEL - 7   /* angel */
#define L8      MAX_LEVEL - 8   /* avatar */
#define IM      LEVEL_IMMORTAL  /* angel */
#define HE      LEVEL_HERO      /* hero */

/*
 * Structure for a command in the command lookup table.
 */
struct cmd_type {
    char *const name;
    DO_FUN *do_fun;
    sh_int position;
    sh_int level;
    sh_int log;
    bool show;
};

/* the command table itself */
extern const struct cmd_type cmd_table[];

/*
 * Command functions.
 * Defined in act_basic.c.
 */
DECLARE_DO_FUN( do_north );
DECLARE_DO_FUN( do_south );
DECLARE_DO_FUN( do_east );
DECLARE_DO_FUN( do_west );
DECLARE_DO_FUN( do_up );
DECLARE_DO_FUN( do_down );
DECLARE_DO_FUN( do_look );
DECLARE_DO_FUN( do_exits );
DECLARE_DO_FUN( do_say );
DECLARE_DO_FUN( do_tell );
DECLARE_DO_FUN( do_who );
DECLARE_DO_FUN( do_save );
DECLARE_DO_FUN( do_quit );
DECLARE_DO_FUN( do_help );

#endif
