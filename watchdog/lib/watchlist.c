
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

#include <watchlist.h>
#include <stdio.h>
#include <debug.h>
#include <malloc.h>
#include <string.h>
#include <profile.h>
#include <util.h>
#include <stdlib.h>

/*
 * Local variables
 *
 */

struct check_processes * cpRoot;
struct check_processes * cpCurrent;


/*
 * Declaration of function prototypes
 *
 */

void cpFree(void);

/*
 * cpInit - move content of ini file into double linked memory structure
 *
 */


void cpInit(void)
{
 if (cpRoot != NULL)
    cpFree();

   cpCurrent=cpRoot=(void *) malloc(sizeof(check_processes));
   /* Initialize initial record */

   memset(cpCurrent,0,sizeof(check_processes));
   cpRoot->prev=NULL;  /*First element of double linked list points to NULL */

}

void cpAdd(char *section, char *ini)
{
   struct check_processes *t;

   /* Move data to structure */
   getPrivateProfileString(section,"params",cpCurrent->params,ini);
   getPrivateProfileString(section,"check",cpCurrent->check,ini);
   getPrivateProfileString(section,"count",cpCurrent->count,ini);
   cpCurrent->n=atoi(cpCurrent->count);
   getPrivateProfileString(section,"external",cpCurrent->external,ini);
   getPrivateProfileString(section,"restart",cpCurrent->restart,ini);
   getPrivateProfileString(section,"mail",cpCurrent->mail,ini);
   getPrivateProfileString(section,"owner",cpCurrent->owner,ini);
   getPrivateProfileString(section,"retries",cpCurrent->retries,ini);
   cpCurrent->r=atoi(cpCurrent->retries);
   getPrivateProfileString(section,"onerror",cpCurrent->onerror,ini);

   getPrivateProfileString(section,"debug",cpCurrent->debug,ini);
    
   if (strcmpi(cpCurrent->debug,"yes")!=0)
     strcpy(cpCurrent->debug,"no");
   

   getPrivateProfileString(section,"syslog",cpCurrent->syslog,ini);
   
   if(strcmpi(cpCurrent->syslog,"yes")!=0)
     strcpy(cpCurrent->syslog,"no");
   

   getPrivateProfileString(section,"delay",cpCurrent->delay,ini);
   cpCurrent->d=atoi(cpCurrent->delay);

   getPrivateProfileString(section,"owner",cpCurrent->owner,ini);
   getPrivateProfileString(section,"service",cpCurrent->service,ini);
   getPrivateProfileString(section,"message",cpCurrent->message,ini);
   getPrivateProfileString(section,"on_error_force_reboot",cpCurrent->on_error_force_reboot,ini);

   getPrivateProfileString(section,"stat",cpCurrent->stat,ini);

   cpCurrent->next=(void *) malloc(sizeof(check_processes));
   memset(cpCurrent->next,0,sizeof(check_processes));
   t=cpCurrent;
   cpCurrent=cpCurrent->next;
   cpCurrent->prev=t;
   
   
}

void cpFree(void)
/*
 * Frees all allocated memory at cpRoot
 *
 */
{
  if (cpCurrent != NULL) /* should point to a valid adress */
  {
     /* We start  at the last element of the linked list
     // and delete all elements step by step until we
     // reach the first element of the list
     */
     while (cpRoot != cpCurrent->prev)
     {
           /*remember current pointer to object we have to delete */
           struct check_processes *tmp=cpCurrent;

           /*move current pointer to element before last element*/
           cpCurrent=cpCurrent->prev;
  
           /*free memory hold by last element of the list*/
           free(tmp);
     
     }
     /* free first element of the list because it will initialized with new data */
     free(cpRoot);
     /* set all pointer of the list to NULL */
     cpCurrent=cpRoot=NULL;
   }
}

/*
 * Prints the content of the stored data
 * in the double linked list
 * Just for debugging..
 *
 */

void cpPrint(void)
{
   struct check_processes *t=cpCurrent;

   while (t->prev != NULL)
   {
      printf("%s%s%s%s",t->params,t->check,t->count,t->external);
      t=t->prev;
   }
     printf("%s%s%s%s",t->params,t->check,t->count,t->external);
}

/*
 *
 * search_proc searches for a process found by the m_linux functions (top)
 * in the double linked list which represents the content of Watchdog's
 * Ini file.
 *
 * Called by lib/gui/reports/ps.c
 *
 */

struct check_processes * search_in_ini_proc_struct(const char *run)
{
   struct check_processes *t=cpRoot;

   while (t != NULL)
   {
      if ((t->check != NULL) & (strcmp(t->check,run) == 0)) 
          return t;
      t=t->next;
   }
return NULL;
}

int search_in_ini_proc(const char *run)
{
   struct check_processes *t=cpRoot;

   while (t != NULL)
   {
      if ((t->check != NULL) & (strcmp(t->check,run) == 0)) 
          return 1;
      t=t->next;
   }
return 0;
}




