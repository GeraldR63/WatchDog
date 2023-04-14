
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
#include <inimain.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <string.h>
#include <util.h>

int abort_counter=0;

void wdabort(void * ptr)
{
  struct check_processes * current=ptr;

  abort_counter++;

  if ((current!= NULL ? strcmpi(current->on_error_force_reboot,"yes")==0 : 0==1 ) 
         | (( (abort_counter>=inimain.efr) & (inimain.efr != 0) )
         & (getenv("DBG_ERRCLASS")== NULL ? 0 : atoi((char*)getenv("DBG_ERRCLASS")) <3)))
         reboot(RB_AUTOBOOT);

  if (inimain.efr == 0 )
    abort();
}
