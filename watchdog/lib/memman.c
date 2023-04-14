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

/*
	Memory Manager.
        The coding is a little bit crazy but I did it my way.
        So you can see that this code is really designed by myself and not  stolen or
        probably patent pending.

       
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memman.h>

int iMemCtr;					// Counter to handle different list
int iMemCurrent;				// Current index to handle

int iMemInit(void)				// Initialize memory list
{
        int retval=-1;
        if (iMemCtr<MEM_MAX_LISTS)
	{
        root[iMemCtr]=(void *) malloc(sizeof(dynmem));
        last[iMemCtr]=root[iMemCtr];
        memset((void*)root[iMemCtr],0,sizeof(dynmem));
        root[iMemCtr]->id=0; 
	iMemCtr++;
	retval=iMemCurrent=iMemCtr-1;
        }
	return retval;
}

void MemCurrent(int i)				// Pointer to current list
{
	if (iMemCtr<i-1 & i>0 & i<MEM_MAX_LISTS) iMemCurrent=i;
}

int iMemPush (void * p, long s)				// add a new item to the list
{
        int retval=-1;
        // Assign user defined data struct to linked list
        current[iMemCtr]->data=p;
        current[iMemCtr]->size=s;

        // Create a new entry for our linked list for
        // next possible element
        current[iMemCtr]->next=(void *) malloc(sizeof(dynmem)); // Allocate required memory
        memset((void*)current[iMemCtr]->next,0,sizeof(dynmem)); // Initialize memory
        struct dynmem *t=current[iMemCtr];			// Temporary pointer
        long id=t->id+1;					// Temporary object id
        current[iMemCtr]=current[iMemCtr]->next;		// Move current pointer to next element
        current[iMemCtr]->prev=t; 				// Set prev pointer to prev element
        current[iMemCtr]->id=id;				// Set ID of current element
        last[iMemCtr]=current[iMemCtr];
	return retval;
}

int iMemReplace(long id, void * p, long s) 	// replace element at id with element at pointer
{
	int retval=0;
        struct dynmem *t=root[iMemCtr];
        // Search for element to replace until end of list
        while (t->id <= id & t->next!=NULL) t=t->next;
        // If elememt found than replace
        if (t->id == id )
        {
                // If there is any memory structure assigned free it
                if (t->data != NULL) free (t->data);
                // Assign new data pointer and size
        	t->data=p;
        	t->size=s;
        }
	return retval;
}

void * pMemFirst ()				// return pointer to first element
{
	return root[iMemCurrent];
}

void * pMemNext ()				// return pointer to next element
{
         struct dynmem *t=NULL;
         if (current[iMemCurrent]->next != NULL) 
         {
            t=current[iMemCurrent]->next;
            current[iMemCurrent]=t;
         }
	return t;
}

void * pMemLast ()				// return pointer to last element
{
	return last[iMemCurrent];
}

void * pMemGetElement(long id)			// return pointer to element with ID
						// or null
{
        struct dynmem *t=root[iMemCtr];
        // Search for element until end of list
        while (t->id <= id & t->next!=NULL) t=t->next;
        if (t->id != id) t=NULL;
	return t;
}



int pMemFreeAll(long id)				// Free all memory associated with linked list
{
        struct dynmem *t=last[id];
        while(t->prev !=NULL)
        {
		// If assigned than cleand up used memory and free user defined record
		if (t->data != NULL) {memset ((void *) t->data,0,t->size) ; free (t->data);}
                t=t->prev;
                if (t->next != NULL) {memset ((struct dynmem *)t->next,0,sizeof(struct dynmem)); free (t->next);}
	}
        // Free first element of list
        if (t != NULL)        
	{
	   if (t->data != NULL) {memset ((void *) t->data,0,t->size) ; free (t->data);}
           memset((struct dynmem*)t,0,sizeof(struct dynmem));
           free(t);
           t=NULL;
        }
	int retval=0;
}
