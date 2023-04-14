
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

#ifndef __INIMAIN_H
#define __INIMAIN_H

struct inimain
{
     char processes[1024];    /* Processes to monitor */
     char admin[512];         /* Admin adresse for example an e-mail address */
     char service[512];       /* Definition of a service which sends admin a message */
     char on_errors_force_reboot[6];  /* Number between 0 and 9999 */
     char http[5];            /* HTTP server on/off. See options.h for usage!*/
     char daemon[5];          /* daemon mode on/off. See options.h for usage! */
     int  efr;                /* Integer value of on_errors_force_reboot */

                              /* MySQL database connect for statistics module */
     char mysql_host[128];    /* hostname (localhost) */
     char mysql_user[32];     /* username (watchdog)  */
     char mysql_pass[32];     /* password (watchdog)  */
     char mysql_db[32];       /* database (watchdog)  */
     char mysql_port[7];      /* port     (0=default) */
     int  mysql_porti;        /* port converted to an integer */
     char stat[32];           /* global statistics gathering level all, monitored, not_monitored */
     int  stat_i;             /* 0 = no statistics 1=all 2=monitored 3=not_monitored */
} inimain;


/* struct inimain inimain; */

/*
 * Function which moves corresponding  keys from section main of watchdog.ini
 * into structure inimain
 *
 */

void getIniMain(char *);

#endif

