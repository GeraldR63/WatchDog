
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


#ifndef __PROCESSES_H
#define __PROCESSES_H

/*
 * struct running_processes:
 *  
 *   
 * Double linked list of process tree containg
 * pid
 * command line
 * next points to next item
 * prev points to previous item
 *        
 */

typedef struct running_processes {
  long pid;
  char cmdline[2048];
  void *next;
  void *prev;
} running_processes;

/* The root node of all running processes */
struct running_processes * rpRoot ;
/* A pointer to the current process we process */
struct running_processes * rpCurrent;

/*
 * rpInit
 *
 * Initializes the list and loads all processes into a double linked list at rpRoot.
 * Call this function as often as required to get a list of currently running processes.
 *
 *
 */

void rpInit(void);

/*
 * Searches for a process pattern containing command line as given by argument
 * Returns true if the algorithm found a processing maching giving pattern
 * and false if not!
 *
 */

int rpFind(char *process);

#endif
