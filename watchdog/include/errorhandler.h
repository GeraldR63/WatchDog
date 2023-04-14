
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

#ifndef __ERRORHANDLER_H
#define __ERRORHANDLER_H

/*
 * Function 	wdabort
 * Parameter	pointer to struct check_processes
 *
 * Watchdog abort
 *
 * This function implements an abort handler for watchdog
 * the behaviour depends on settings in watchdog ini.
 * if on_error_force_reboot is set for a process than
 * this function will reboot if called
 * if  on_errors_force_reboot is defined in section main of
 * watchdog.ini than watchdog will force reboot after 
 * wdabort was called the number of times as defined by
 * on_errors_force_reboot.
 *
 * If both parameters are not set, than wdabort will 
 * behave like normal abort.
 *
 * 
 */

void wdabort(void *);


#endif
