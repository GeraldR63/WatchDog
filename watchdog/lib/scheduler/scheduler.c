
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
   Scheduler
*/


#include <stdio.h>
#include <time.h>
#include <syslog.h>
#include <stdarg.h>
#include <scheduler.h>
#include <sighandler.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <logwriter.h>
#include <pthread.h>
#include <watchlist.h>
#include <options.h>
#include <w2top.h>

volatile int thread_ctr=0;                /*
                                           *
                                           * Do not change this variable by
                                           * direct access this
                                           * You may ask it's content directly!
                                           *
                                           */

void thread_inc_latch(void);              /* INC thread_ctr */
void thread_dec_latch(void);              /* DEC thread_ctr */




struct threads{
                pthread_t *l;
              } threads;

struct threads  l[256];                   /* Number of processes is actual limited. */

void * watchdog (void * process);

#define DELAY 60

void schedule(char * arg)
{
    int  iret1;
    int i=0;

	/* Create independent threads to restart not running programs */
	l[i].l=(pthread_t *) malloc (sizeof(pthread_t));
	/* syslogprintf(__FILE__,__LINE__,"watchdog",5,"Allocated some memory"); */
        iret1 = pthread_create( l[i].l, NULL, watchdog, (void *) "Nix" );


	while ((globalArgs.runflags.keep_monitoring_going==1) & (globalArgs.runflags.keep_going==1))
	{
        printf("Scheduler: Called #%d times. Sleeping %d seconds.\n",++i, DELAY);
        fflush(stdout);
	    sleep(DELAY);		/* Delay 60 seconds */
	}
	printf("Scheduler: Terminated normal!\n");
}

/*

int main (int argc, char *argv[])
{

  schedule("* * * * *");

return 0;
}

*/



struct event {
	 /* The current time as given by strftime */

	 char *m; /* Minute */
	 char *H; /* Hour */
	 char *d; /* Day */
	 char *M; /* Month */
	 char *Y; /* Year */

	 /* Copy of schedule string from Watchdog.ini */
	 char schedule[256];
     /* Substring containing Minutes in schedule */
	 char *minutes;
	 /* Substring containing Hours in schedule */
	 char *hours;
	 /* Substring containing Days in schedule */
	 char *days;
	 /* Substring containing Month in schedule */
	 char *months;
	 /* Substring containing Year in schedule */
	 char *years;
 } event;

void * watchdog (void * process)
/*
 * Function watchdog
 * 
 * This function is a scheduler thread which monitors a process
 * as given by watchdog.ini
 *
 * Parameter: Pointer to check_process construct of the process to monitor
 *
 */

{

struct check_processes * cp;

#ifndef NDEBUG
	int n=0;
#endif
        cp=(struct check_processes *) process;
	thread_inc_latch();  /* Increment number of threads started */


	while( (globalArgs.runflags.keep_monitoring_going==1) && (globalArgs.runflags.keep_going))
	{
		struct event event; /* This structure holds scheduler interval */
		time_t start, end;  /* Required to calculate duration of one iteration */
		int d;
  	        long clock;
                char *lasts;        /* Used by strtok_r */
  	                       /* mi:hh [day of month] month year */
  	        char i_time[19]="00:00 Mon Jan 2007";

		/*
		 * Copy schedule interval to internal structure.
		 * Required because it's possible to change scheduling
		 * dynamically via HTML GUI or change in Watchdog.ini and signal.
		 *
		 */
            
            /* 
             * Process manager asked for permission to update internal process list from system!
             * Scheduler have to wait until the list is refreshed
             *
             */

            while (globalProcessSync.proc_refresh_scheduled == 1) sleep (1);

		strcpy(event.schedule,cp->schedule);

		d=(rand()%(DELAY/2)+1);
  	        clock = time(0);

        strftime(i_time, 19 , "%M %H %d %m %Y" , localtime(&clock));

        /*
         * Calculate the duration of execution
         * The next event will start exactly 60 seconds after current time
         * or immediatly if check of process took more than 60 seconds.
         */


        start=time(NULL);


        /* Thread save strtok_r */

        event.m=(char*)strtok_r(i_time," ", &lasts);
        event.H=(char*)strtok_r((char*)NULL," ", &lasts);
        event.d=(char*)strtok_r((char*)NULL," ", &lasts);
        event.M=(char*)strtok_r((char*)NULL," ", &lasts);
        event.Y=(char*)strtok_r((char*)NULL," ", &lasts);

        /* Create list with dates */
        event.minutes=(char*)strtok_r(event.schedule," ", &lasts);
        event.hours=(char*)strtok_r(NULL," ", &lasts);
        event.days=(char*)strtok_r(NULL," ", &lasts);
        event.months=(char*)strtok_r(NULL," ", &lasts);
        event.years=(char*)strtok_r(NULL," ", &lasts);

#ifndef NDEBUG
        syslogprintf(__FILE__, __LINE__, "watchdog", 5, "Watchdog: Time %s %s %s %s %s \n",event.m, event.H, event.d, event.M, event.Y);
#endif


        /* Check for valid minute */
        if ((strstr( event.minutes,event.m)!= NULL ) | (strstr(event.minutes,"*")!= NULL))
        {
			/* Check for valid minute */
			if ((strstr(event.hours, event.H)!= NULL ) | (strstr(event.hours,"*")!= NULL))
            {
				if ((strstr(event.days, event.d)!= NULL ) | (strstr(event.days,"*")!= NULL))
				{
					if ((strstr(event.months, event.M)!= NULL ) | (strstr(event.months,"*")!= NULL))
					{
						if ((strstr(event.years, event.Y)!= NULL ) | (strstr(event.years,"*")!=NULL))
						{
						    /* 
						     * Process manager asked for permission to update internal process list 
						     * from system!
						     * Scheduler have to wait until the list is refreshed
						     *
						     */

					    		while (globalProcessSync.proc_refresh_scheduled == 1) sleep (1);
#ifndef NDEBUG
				syslogprintf(__FILE__,__LINE__,"watchdog",5,"Watchdog: Execute check process\n");
#endif
							/*
 							 * Ensure that during access of the process information the
 							 * list is not refreshed by proc_refresh called at main loop!
 							 */

							 proc_thread_start();

							/*
							 * Execute check
							 * Well now this will do nothing! ;-) except sleeping!
							 *
							 */
							 sleep((rand()%(DELAY)+1));

							 /*
 							  * Tell process refresh thread that we do not need
 							  * the memory structures managed by proc_refresh anymore.
 							  * perhaps later.
 							  */
							 proc_thread_end();
						}
					}
			    }
			}
		}



		end=time(NULL);

		if ( (end-start) >= 60)
		  d=0;
		else
		  d=60-(end-start);

#ifndef NDEBUG
		syslogprintf(__FILE__,__LINE__,"watchdog",5,"Watchdog: Called #%d times. Executiontime was %d seconds. Sleeping duration %d seconds",++n,end-start,d);
#endif
		sleep(d);
	}
#ifndef NDEBUG
	syslogprintf(__FILE__, __LINE__,"watchdog",5,"Watchdog: Thread terminated normal");
#endif
	thread_dec_latch();  /* Decrement number of threads started */

return (void *) "Nix";
}


/*
 * Simple thread parallel management
 * Replace as soon as possible with MUTEX'es!
 */

enum boolean {true, false};
enum boolean running=false;

void thread_inc_latch(void)
{

	    while (running==true)
			sleep((rand()%(7)+1));

		running=true;
        thread_ctr++;
        running=false;

}

void thread_dec_latch(void)
{
	    while (running==true)
			sleep((rand()%(7)+1));

		running=true;
        thread_ctr--;
        running=false;

}
