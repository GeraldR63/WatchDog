
/*    Watchdog , a simple to use Linux  High Availability management software
 *   Copyright (C) 1989-2007  Gerald Roehrbein
 *
 *   This software contains libraries I've alread implemented between 1989
 *   and 2007.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 *   Contact information:
 *
 *   e-mail: Gerald.Roehrbein@OraForecast.com
 *
 *   Gerald Roehrbein
 *   Boskamp 19
 *   24214 Gettorf
 *   Germany
 *
 *
 *   Watchdog version 0.9, Copyright (C) 1989-2007 by Gerald Roehrbein 
 *   Watchdog comes with ABSOLUTELY NO WARRANTY
 *   This is free software, and you are welcome to redistribute it
 *   under certain conditions; Please read additional LICENSE file.
 *
 */


#ifndef __MACHINE_H
#define __MACHINE_H

#include <watchdog.h>

/*
 *  This file defines the interface between top and the machine-dependent
 *  module.  It is NOT machine dependent and should not need to be changed
 *  for any specific machine.
 */

/*
 * The statics struct is filled in by machine_init.  Fields marked as
 * "optional" are not filled in by every module.
 */
struct statics
{
    char **procstate_names;
    char **cpustate_names;
    char **memory_names;
    char **swap_names;		/* optional */
    char **order_names;		/* optional */
    char **color_names;		/* optional */
    time_t boottime;		/* optional */
    struct {
	unsigned int fullcmds : 1;
	unsigned int idle : 1;
	unsigned int warmup : 1;
    } flags;
} statics;

struct top_proc
{
    pid_t pid;
    uid_t uid;
    char *name;
    int pri, nice;
    unsigned long size, rss;    /* in k */
    int state;
    unsigned long time;
    unsigned long start_time;
    double pcpu, wcpu;
    struct top_proc *next;
};

#define NPROCSTATES 7
static char *state_abbrev[NPROCSTATES+1];


/*
 * the system_info struct is filled in by a machine dependent routine.
 */

#ifdef p_active     /* uw7 define macro p_active */
#define P_ACTIVE p_pactive
#else
#define P_ACTIVE p_active
#endif

struct system_info
{
    int    last_pid;
    double load_avg[NUM_AVERAGES];
    int    p_total;
    int    P_ACTIVE;     /* number of procs considered "active" */
    int    *procstates;
    int    *cpustates;
    long   *memory;
    long   *swap;
} system_info;

/* cpu_states is an array of percentages * 10.  For example, 
   the (integer) value 105 is 10.5% (or .105).
 */

/*
 * the process_select struct tells get_process_info what processes we
 * are interested in seeing
 */

struct process_select
{
    int idle;		/* show idle processes */
    int system;		/* show system processes */
    int fullcmd;	/* show full command */
    int uid;		/* only this uid (unless uid == -1) */
    char *command;	/* only this command (unless == NULL) */
};

/* routines defined by the machine dependent module */
int machine_init(struct statics *);
void get_system_info(struct system_info *);
caddr_t get_process_info(struct system_info *, struct process_select *, int);
char *format_header(char *);
char *format_next_process(caddr_t, char *(*)(int));
int proc_owner(int);

/*
static struct top_proc **pactive;
static struct top_proc **nextactive;
*/

void resetnextproc();
struct top_proc * getnextproc();
char * getStateAbbrev(int );

#endif
