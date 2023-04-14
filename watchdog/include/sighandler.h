
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


#ifndef __SIGHANDLER_H
#define __SIGHANDLER_H

/*
 * Todo: Tranlate comments into english
 *
   Standardsignalbehandlung
   Es muss nur die Funktion RegisterSignalHandler aufgerufen
   werden! Dies sollte zum Programmstart erfolgen!
   Danach werden die Signale 1-22 (ohne Signal 9) von einer
   Standardroutine behandelt! Diese gibt auf STDERR eine
   Meldung bei Eintreffen eines Signals aus. Die Meldung
   kann unterdrueckt werden, wenn die Environmentvariable
   DBG_ERRCLASS <2 gesetzt wird!
   export DBG_ERRCLASS

   Die Routine ReleaseSignalHandler setzt die Signalbehandlung wieder
   auf den Default zurueck!

   Die Routine RegisterSignalExitHandler registriert mit atexit die
   Funktion ReleaseSignalHandler, so daß die Freigabe des default
   Signalhaendler ordentlich erfolgen kann.    
   
*/

#include <signal.h>
 
void RegisterSignalHandler(void);        
void ReleaseSignalHandler(void);
void RegisterSignalExitHandler(void);

/* 
 *  volatile sig_atomic_t keep_going;     controls program termination  
 *  Moved this variable to struct globalArgs member runflags
 *
 */

void termination_handler (int signum);    /* clean up before termination  */
void reload_process_handler(int signum);  /* reloads the process handler  */
void reload_ini_handler(int signum);      /* reloads the ini file         */


#endif
