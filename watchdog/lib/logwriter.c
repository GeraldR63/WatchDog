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

#include <logwriter.h>
#include <datum.h>
#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>


/*
 * (w) 2007 Gerald Roehrbein
 *
 * Function(s) to handle log writing
 *
 * Function 	syslogprintf
 * Parameters	cfile    - filename as given by makro __FILE__
 *              line     - linenumber as given by makro __LINE__
 * 		item	 - name in syslog
 * 		level    - loglevel 1-5
 * 		fmt      - printf format string with the message
 * 		...      - ellipse. a free list of args fitting to format string
 *
 * This function writes everything to syslog dependent on environmentvariable
 *
 * DBG_ERRCLASS
 *
 * If DBG_ERRCLASS is 5 (default if not set) than everything will also be written to stderr
 * 
 *
 *   Possible configuration with environment variable DBG_ERRCLASS.
 *   DBG_ERRCLASS=0 no logging
 *   DBG_ERRCLASS=1 log errors
 *   DBG_ERRCLASS=2 log warnings
 *   DBG_ERRCLASS=3 log verbose
 *   DBG_ERRCLASS=4 log very verbose
 *   DBG_ERRCLASS=5 log anything and write it also to stderr
 *  
 *
 */



void syslogprintf(char *cfile, int line, char *item, int level, char *fmt, ...)
{
 va_list arg;
 int done;
 char msg[1024]="Default syslogprintf message";
 char * cpn;
 cpn=(char*)getenv("DBG_ERRCLASS");

 if (cpn==NULL) cpn=(char *)"5";
 /* 
  *				
  * If DBG_ERRCLASS is 0 than this function is absolute silent! 
  * It will log nothing.
  *
  */

 if (atoi(cpn)==0) return;  
 
 /*
  * Ellipsis (variable args handling)
  * to build the message string
  */


  va_start (arg, fmt);
  done = vsprintf (msg,fmt, arg);
  va_end (arg);


  /*
   * End of Ellipsis handler
   *
   */


  if ( atoi(cpn) >= level) {
    openlog(item, LOG_CONS | LOG_PID, LOG_LOCAL3);
    syslog (LOG_DEBUG, msg , level);
    closelog();
  }

}
