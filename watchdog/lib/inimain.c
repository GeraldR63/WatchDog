
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


#include <inimain.h>
#include <profile.h>
#include <string.h>
#include <stdlib.h>
#include <options.h>

/*
 * Reads section main from ini file as described in header
 *
 */
struct inimain inimain;

void getIniMain(char *ini)
{
  char *section="main";

  memset(&inimain,0,sizeof(inimain));

  getPrivateProfileString(section,"processes",inimain.processes,ini);
  getPrivateProfileString(section,"admin",inimain.admin,ini);
  getPrivateProfileString(section,"service",inimain.service,ini);
  getPrivateProfileString(section,"on_errors_force_reboot",inimain.on_errors_force_reboot,ini);
  if (globalArgs.flags.run_http == 0)  /* Do not override if set by commandline */
  {
     /*
      * If not set by commandline than read inifile 
      * and set global flag for this option
      */
     getPrivateProfileString(section,"http",inimain.http,ini);
     if (strncmp("yes",inimain.http,3)==0) 
     { 
        globalArgs.run_http=1;
        globalArgs.runflags.keep_HTTP_going=1;
     }
  }
  if (globalArgs.flags.run_daemon == 0) /* Do not override if set by commandline */
  {
     /*
      * If not set by commandline than read inifile 
      * and set global flag for this option
      */
     getPrivateProfileString(section,"daemon",inimain.daemon,ini);
     if (strncmp("yes",inimain.daemon,3)==0) 
     { 
        globalArgs.run_daemon=1;
        globalArgs.runflags.keep_going=1;
        globalArgs.runflags.keep_monitoring_going=1;
     }
  }

  if (strlen(inimain.on_errors_force_reboot)>=1)
    inimain.efr=atoi(inimain.on_errors_force_reboot);

     getPrivateProfileString(section,"mysql_host",inimain.mysql_host,ini);
     getPrivateProfileString(section,"mysql_user",inimain.mysql_user,ini);
     getPrivateProfileString(section,"mysql_pass",inimain.mysql_pass,ini);
     getPrivateProfileString(section,"mysql_db",inimain.mysql_db,ini);
     getPrivateProfileString(section,"mysql_port",inimain.mysql_port,ini);

     inimain.mysql_porti=atoi(inimain.mysql_port);

     getPrivateProfileString(section,"stat",inimain.stat,ini);
     if (strncmp("none",inimain.stat,4)==0) inimain.stat_i=0;
     if (strncmp("all",inimain.stat,3)==0) inimain.stat_i=1;
     if (strncmp("monitored",inimain.stat,9)==0) inimain.stat_i=2;
     if (strncmp("not_monitored",inimain.stat,13)==0) inimain.stat_i=3;

#ifndef NDEBUG
     syslogprintf(__FILE__,__LINE__,"watchdog",5,"Statistic level %s, %d",inimain.stat, inimain.stat_i);
#endif

}


