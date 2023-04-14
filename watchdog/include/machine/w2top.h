
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


#ifndef __W2TOP_H
#define __W2TOP_H

/* W2TOP means watchdog 2 top thread safe interface! 
 *
 * The idea of this code is simple:
 *
 * Each thread could access /proc if required to do so.
 * But this will take a lot of resources and I do not like wasting resources.
 *
 * The idea to avoid this is only to read /proc directory once a minute and synchronize reading
 * and access to the gathered data via semaphores.
 *
 */


/*
 * This function is called from the main loop in watchdog.c.
 * 
 *
 * Refresh checks that there is no scheduled thread accessing data structures
 * changed by refresh. 
 *
 * The function set a flag for all processes that they have to wait because
 * an update of process information is planned.
 *
 * than it set a mutex to disallow other processes 
 * access to data structures containing process information from /proc 
 * directory.
 * 
 */

#include <pthread.h>

struct globalProcessSync
{
	unsigned proc_refresh_scheduled:1;  /* If refresh is called this is set to 1 
                                             * This is used to stop all scheduled monitoring processes.
                                             * They will only run if this flag is set to 0!
                                             *
                                             */
       volatile int number_of_active_scheduler;
				            /*
                                             * This variable contains the number of actual running scheduler
                                             * processes. This means processes which need to access
                                             * process information from /proc directory.
                                             * This is NOT the number of scheduler processes!
                                             *
                                             * Refresh will wait until this counter is set to 0!
                                             *
                                             */
        
}globalProcessSync;


/*
 * Refresh read process information from system (for example from /proc directory if using LINUX)
 * into internal data structures.
 *
 * This function handles a safe refresh of process information from the system.
 * It avoids that a scheduled monitoring process accesses data structures actual updated or destroyed!
 *
 * It's just a synchronization between reading of process information from operating system and 
 * comparing this data with the data Watchdog expected!
 *
 */

int proc_refresh(void);

/* Global variables required for synchronization of scheduled threads and reading process information from OS 
 * They are initialized in function main 
 *
 */

static pthread_cond_t  proc_cond;
static pthread_mutex_t proc_mutex;

int proc_thread_start(void);
int proc_thread_end(void);



#endif
