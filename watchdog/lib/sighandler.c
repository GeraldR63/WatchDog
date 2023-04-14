
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


#include <sighandler.h>
#include <stdio.h>
#include <stdlib.h>
#include <debug.h>
#include <logwriter.h>
#include <scheduler.h>
#include <unistd.h>
#include <options.h>
#include <w_mysql.h>       /* For MySQL database connection reset ! */

/*
 *
 * volatile sig_atomic_t keep_going = 1;      controls program termination 
 * Moved to globalArgs member runflags;
 *
 * Variable globalArgs.runflags.keep_going 
 * controls Watchdog wide thread termination
 * If you want to force all threads to terminate set keep_going to 0
 *
 * This variable is toggled by termination_handler only
 *
 * Don't toggle keep_going manually call termination_handler instead!
 *
 */


void RegisterSignalHandler(void)
{
/* Establish signal handler to clean up before termination: */
          if (signal (SIGTERM, termination_handler) == SIG_IGN)
             signal (SIGTERM, SIG_IGN);

           signal (SIGINT, SIG_IGN);
           signal (SIGHUP, SIG_IGN);
           signal (SIGUSR1, reload_process_handler);
           signal (SIGUSR2, reload_ini_handler);

           RegisterSignalExitHandler();
}

void ReleaseSignalHandler(void)
/*
 * Called if Watchdog stops
 */
{
#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Called at exit...");
#endif
}

void RegisterSignalExitHandler(void)
/*
 * Register onExit functions
 */
{
  atexit(ReleaseSignalHandler);
}

void termination_handler (int signum)
/*
 * Processed at SIGTERM
 */

{
   globalArgs.runflags.keep_going = 0;
   globalArgs.runflags.keep_HTTP_going = 0;
   globalArgs.runflags.keep_monitoring_going = 0;

#ifndef NDEBUG
   syslogprintf(__FILE__, __LINE__, "watchdog", 5, "Watchdog Signal: Asked for termination");
#endif

   /*
    * Wait for running threads to terminate clean
    */

   while (thread_ctr != 0)
   {
#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__, "watchdog", 5, "Watchdog Signal: Waiting for threads to terminate");
#endif
   sleep(1);
   }

#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog",5, "Watchdog Signal: Terminating");
#endif

   signal (signum, termination_handler);
}


void reload_process_handler(int signum)  /* reloads the process handler  */
/*
 * Forces a reload of running processes if called via SIGUSR1
 * This function is called each 60 seconds to refresh number of processes
 *
 */
{
#ifndef NDEBUG
        syslogprintf(__FILE__,__LINE__,"watchdog",5,"Watchdog Signal: Reload list of running processes");
#endif
        signal (signum, reload_process_handler);
}
void reload_ini_handler(int signum)      /* reloads the ini file         */
/*
 * Forces a reload of ini file
 * This is called during startup or signal SIGUSR2
 */
{
#ifndef NDEBUG
        syslogprintf(__FILE__,__LINE__,"watchdog",5,"Watchdog Signal: Reload ini file");
#endif

    globalArgs.runflags.run_mysql_stats=1;        /* 
                                                      This flags allows or disallows writing
                                                      global or process statistics to 
                                                      a MySQL connection.
                                                      This is set to 1 without respect to mysql_port.
                                                      If mysql_port is set to -1 in ini file this
                                                      will disable logging MySQL database logging!
                                                    */
    closeDB();  /* If mysql_port >= 0 then try to close database */ 
    openDB();   /* Than try to reopen connection ! If there is an error than
                 * global_args.runflags.run_mysql_stats is set to 0 
                 */
    signal (signum, reload_ini_handler);
}

