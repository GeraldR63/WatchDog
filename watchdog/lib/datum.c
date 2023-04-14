
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


#include <datum.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/* Der Bezeichner i wird vorangestellt um interne Deklaration zu markieren! */

char * cpGetCurrentDate(void)
/*
  Datum im europaeischem Format zurueckliefern.
  dd.mm.yyyy
*/
{
  /* Liefert das aktuelle Datum nach i_datum im Format dd.mm.yyyy */
  static char i_datum[EUROPEAN_DATE_SIZE]="01.01.1888";
  long clock = time(0);
  strftime(i_datum, EUROPEAN_DATE_SIZE , EUROPEAN_DATE_FORMAT , localtime(&clock));
return i_datum;
}


char * cpGetCurrentTime(void)
/*
  Datum im europaeischem Format zurueckliefern.
  dd.mm.yyyy
*/
{
  /* Liefert das aktuelle Datum nach i_datum im Format dd.mm.yyyy */
  static char i_time[EUROPEAN_TIME_SIZE]="00:00:00";
  long clock = time(0);
  strftime(i_time, EUROPEAN_TIME_SIZE , EUROPEAN_TIME_FORMAT , localtime(&clock));
return i_time;
}

