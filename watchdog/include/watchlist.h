
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


#ifndef __WATCHLIST_H
#define __WATCHLIST_H

#include <stdio.h>


/*
 * 
 * Struct check_processes
 * Contains all the information to process from watchdog.ini
 * Use two characters more as required for LF and 0 at the end!!!
 */

typedef struct check_processes {
	char params[128];	/* List of active params	*/
	char check[512];	/* pattern to search */
	char count[5];		/* number of processes expected */
        int  n;                 /* Integer value of count */
	char external[256];	/* external check should return 0 or 1 */
	char restart[256];	/* command to restart the service */
	char mail[5];		/* in case of failure send mail yes/no */
	char retries[5];        /* number of retries to restart process	*/
        int  r;                 /* Integer value of retries */
        int  retry;             /* Retry n-times */
        char delay[5];          /* Delay in seconds between retries */
        int  d;                 /* Integer value of delay */
	char onerror[512];	/* command to execute if not able to restart process	*/
        char syslog[5];         /* Write actions related to this process to syslog YES/NO */
        char debug[5];          /* Enhanced logging for this process YES/NO */
        char owner[512];        /* Owner of this process (for example e-mail address) */
        char service[512];      /* Command which could be used to send owner a message. (for example mailx) */
        char message[1024];     /* Path to file containing a message template which is send in case of failure */
        char on_error_force_reboot[5]; /* Yes or no.... */
        char schedule[512];     /* Cron like scheduler pattern */
        char stat[5];           /* Store statistics of current monitored process to database or not! */
        /*
         * Required pointers to make Watchdog threadsave
         *
         */
        /*
        char *msg;         	// The messagetext from file message
        char *tmp;		// A temporary copy of messagetext used to replace macros 
        FILE *handle;		// File handle of the file containing the messagetext
        char **tok;             // A pointer required by strtok_r for each  thread having a failure
        char *curtok;           // Current token
        int  len;               // Lenght of the file containing the error message
        */
	void *next;		/* Pointer to next item of double linked list */
	void *prev;		/* Pointer to previous item of double linked list */
} check_processes; 

extern struct check_processes * cpRoot;
extern struct check_processes * cpCurrent;

/*
 *
 *  Initializes the list and moves all data from watchdog.ini into a double linked list at cpRoot.
 *  Call this function as often as required to get a list of all commands in ini file.
 *
 *
 */


void cpInit(void);

/*
 * Adds a struct to the list
 *
 */

void cpAdd(char*,char*);

/* 
 * Searches for the process that run's in the processes to be monitored list
 * and if it find a process it returns true
 * and if not it returns false.
 *
 * This function is required to deal with the traffic light in watchdog?PROCESSES 
 * function lib/report/ps.c
 *
 */

int search_in_ini_proc(const char *run);
/*
 * Searches like the function above but returns a pointer to struct or NULL
 *
 * ToDo: Replace int search_in_ini_proc with search_in_ini_proc_struct
 *
 */

struct check_processes * search_in_ini_proc_struct(const char *run);

#endif
