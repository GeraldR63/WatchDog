
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

#include <w2top.h>
#include <unistd.h>
#include <stdlib.h>

#include <machine.h>
#include <errorhandler.h>
#include <logwriter.h>
#include <options.h>
#include <inimain.h>

#include <pthread.h>     /* required for mutexes and thread synch */


/* Required to synchronize access and refresh of process information 
 *
 * Do not use global structures in this file directly!
 * This may end in false results!
 *
 */

struct globalProcessSync globalProcessSync;

caddr_t processes;
struct process_select ps;
struct system_info system_info;

struct statics statics;

/*
 * Returns 1 if everything is fine and 0 if not
 */
int proc_refresh(void)
{
   int retval;

   ps.idle=1;
   ps.system=1;
   ps.fullcmd=0;
   ps.uid=-1;
   ps.command=NULL;

   /* Possibly reason for memory leak.... */
   /* memset(&system_info, 0, sizeof(struct system_info)); */
   

   retval=1; /* Assume everything will be fine */

   globalProcessSync.proc_refresh_scheduled = 1; /* Tell others that we want to refresh process information */

   /* Wait for scheduler processes working with the data we want to change! */
   while (globalProcessSync.number_of_active_scheduler != 0) sleep(1);

 
    /* This make /proc the home directory for a short time*/
    if (machine_init(&statics) == -1)  /* This changes into directory /proc ! We must go back ! */
    {
        /* This function may abort or not dependent on settings in the ini file.*/
        wdabort(NULL);
        retval=0;
    }
    else
    {

    get_system_info(&system_info);

/* get the current set of processes */

    processes = get_process_info(&system_info,
                                           &ps,
#ifdef ORDER
                                           proc_compares[order_index]);
#else
                               /*            proc_compare); */
                                            0);
#endif

#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog",5,"got system and process information");
#endif

   /* We go back to Watchdogs home directory */
   chdir(getenv("WATCHDOG")==NULL?"/etc/watchdog":getenv("WATCHDOG"));
   }

   /*
    * ToDO: Ad here gathering of network and storage statistics
    *
    */

   /* 
    * If writing to MySQL database is allowed and there is no error
    * occured since last writing to database than
    * write all statistical data gathered as fresh as possible to the database.
    * 
    */

                   if ( (globalArgs.runflags.run_mysql_stats == 1) & (inimain.mysql_porti != -1))
                   {
                       /*
                        * ToDo: If we gathered network and storage statistik than we have to write
                        * them to the database too!
                        */

                         write_process_stats_2_db();
                         write_global_stats_2_db();
                   }

   
   globalProcessSync.proc_refresh_scheduled = 0; /* Tell others that they can access process information */

return retval;
}

/*
 * This is called before a thread accesses the process list
 * it will increment the number of running threads accessing the 
 * list
 */

static pthread_cond_t  proc_cond;
static pthread_mutex_t proc_mutex;

static int safe_proc_mutex_lock( void ) {
#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog", 5, "safe_proc_mutex_lock");
#endif
   if (pthread_mutex_lock (&proc_mutex) != 0) 
   {
     syslogprintf(__FILE__,__LINE__,"watchdog", 5, "Could not lock mutex by thread %ld pid %d\n", pthread_self(),getpid());
     return 0;
    }
#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog", 5, "Mutex locked by thread %ld pid %d\n", pthread_self(),getpid());
#endif
   return 1;
}

static int safe_proc_mutex_unlock( void ) {
#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog", 5, "safe_proc_mutex_unlock");
#endif
   if (pthread_mutex_unlock (&proc_mutex) != 0) 
   {
     syslogprintf(__FILE__,__LINE__,"watchdog", 5, "Could not unlock mutex by thread %ld pid %d\n", pthread_self(),getpid());
     return 0;
    }
#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog", 5, "Mutex unlocked by thread %ld pid %d\n", pthread_self(),getpid());
#endif
   return 1;
}


/*
 * Return 0 on error and 1 if succesfull incremented number of processes
 */

int  proc_thread_start()
{
   if (safe_proc_mutex_lock()==1)
   { 
   globalProcessSync.number_of_active_scheduler++;
#ifndef NDEBUG
       syslogprintf(__FILE__,__LINE__,"watchdog", 5, "Counter incremented");
#endif
     if (safe_proc_mutex_unlock()==1) return 1;
   }
  return 0;
}

/*
 * This is called after a thread accesses the process list
 * it will increment the number of running threads accessing the 
 * list
 */

int proc_thread_end()
{
   if (safe_proc_mutex_lock()==1)
   { 
   globalProcessSync.number_of_active_scheduler--;
#ifndef NDEBUG
       syslogprintf(__FILE__,__LINE__,"watchdog", 5, "Counter decremented");
#endif
     if (safe_proc_mutex_unlock()==1) return 1;
   }
  return 0;
}

