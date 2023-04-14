
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


#include <malloc.h>
#include <string.h>
#include <dirdo.h>
#include <stdio.h>
#include <processes.h>


void rpFree(void);

/*
 *  Call Back function cbd
 *  Called for each directory entry in /proc
 *
 *  Reads the content of file cmdline into double linked list located at cpRoot 
 *  and move rpCurrent to the current (next) element of the linked list
 *  The list will have one item more as found processes! 
 */

void rpCBD(char * n, int sock)
{
        char cmdfile[2048]="/proc/";
        FILE * in;

        strcat(cmdfile,n);
        strcat(cmdfile,"/cmdline");

        in=fopen(cmdfile,"rt");
        if (in)
        {
                fgets(rpCurrent->cmdline,2048,in);
                /* printf("%s\n",rpCurrent->cmdline); */

                /* Create next item */
                rpCurrent->next=(void *) malloc (sizeof(running_processes));
                /* Remember previous item */
                rpCurrent->prev=rpCurrent;
                /* Make current point pointing to next item */
                rpCurrent=rpCurrent->next;
                rpCurrent->next=NULL;   /* Probably last element of the list */
                fclose(in);
        }

}

void rpInit(void)
/*
 * Creates a structure at rpRoot with a double linked list of all running processes
 *
 */

{
 if (rpRoot != NULL)
   rpFree();

 rpCurrent=rpRoot = (void *) malloc (sizeof (running_processes));
 rpRoot->prev=rpRoot;  /* First element of double linked list points to itself */
 dir_do("/proc",rpCBD,0);
}

void rpFree(void)
/*
 * Frees all allocated memory at cpRoot
 *
 */

{
  if (rpCurrent) /* should point to a valid adress */
  {
     /* We start  at the last element of the linked list 
     // and delete all elements step by step until we
     // reach the first element of the list
     */
     while (rpRoot != rpCurrent->prev) 
     {
        /*remember current pointer to object we have to delete */
        struct running_processes *tmp=rpCurrent;
       
        /*move current pointer to element before last element */
        rpCurrent=rpCurrent->prev;

        /*free memory hold by last element of the list*/
        free(tmp);
 
     }
     /* free first element of the list because it will initialized with new data */
     free(rpRoot);
     /* set all pointer of the list to NULL */
     rpCurrent=rpRoot=NULL;
  }
}

int rpFind(char *process)
{
   int retval=0;
   struct running_processes *t=rpRoot;

   while (t->next != NULL)
   {
      t=t->next;
   }

  return retval;
}

/*
 *  Prints the content of the stored data
 *  in the double linked list
 *  Just for debugging..
 *  
 */

void rpPrint(void)
{
   struct running_processes *t=rpRoot;

   while (t->next != NULL)
   {
      printf("%s\n",t->cmdline);
      t=t->next;
   }
     printf("%s\n",t->cmdline);
}

