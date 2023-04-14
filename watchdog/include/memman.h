
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



#ifndef __MEMMAN_H
#define __MEMMAN_H

/*
	Generic Memory Manager.
	Dynamic handling of double linked memory structures.

	This set of functions add, replace and searches for element
	of any structure.

	This header offers a generic structure and a set of functions
	for dynamic memory management.
*/



typedef struct dynmem {
	void * data;					// Pointer to user data
	long size;					// Size of data element
	void * prev;					// Pointer to previos record of list
							// If first it is NULL
	void * next;					// Pointer to next record of list
							// If last it is NULL
	long id;
} dynmem;

// Array of struct dynmem to handle multiple memory lists
// Max number of lists we want to handle

#define MEM_MAX_LISTS 2 


struct dynmem  *root[];		// The root nodes of the lists
struct dynmem  *current[];	// Current nodes
struct dynmem  *last[];		// Last nodes of the list

// Prototypes of dynamic memory structure management functions

int iMemInit(void);					//Initialize memory list
void MemCurrent(int);					// Pointer to current list
int iMemAdd (void *,long);				// add a new item to the list
int iMemReplace(long id, void *, long); 		// replace element at id with element at pointer
void * pMemFirst ();					// return pointer to first element
void * pMemNext ();					// return pointer to next element
void * pMemLast ();					// return pointer to last element
void * pMemGetElement(long);				// return pointer to element with ID or NULL
int pMemFreeAll(long);					// Free all

#endif
