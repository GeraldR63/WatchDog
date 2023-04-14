
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


#ifndef __WATCHDOG_H
#define __WATCHDOG_H

/*
 * (w) 2007 Gerald Roehrbein
 * Defines all functions required to compare list of
 * have to run processes as defined in watchdog.ini
 * with really running processes.
 *
 */

#include <processes.h>
#include <watchlist.h>
#include <pthread.h>


struct threads{
                pthread_t *l;
              } threads;

/*
 * Function iIsRunning
 * Parameters double linked list to running processes
 * 	      double linked list to watchdog.ini
 * Returns NULL is everything is fine or pointer to a check_process structure
 *
 * This function compares everything in both lists.
 * Check starts at the element check_processes points to. If there is any failure
 * check_processes returns the watchdog.ini structure containing the element with the
 * failure.
 *
 *
 */

struct check_processes * iIsRunning(struct running_processes *, struct check_processes *);

/* 
 *
 * Complete error management for a process 
 * ptr is a pointer to a process to check 
 *
 * struct check_processes
 *
 * This function and the called functions have to be thread and reentrant save.
 * All local variables used in these functions have to be stored in
 *
 * the pointer which points to struct check_processes
 *
 */

void *onErrorHandler( void *ptr );

#endif 
