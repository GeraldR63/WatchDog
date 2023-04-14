
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


#ifndef __WATCHDOG_H
#define __WATCHDOG_H

/*
 *  Top - a top users display for Berkeley Unix
 *
 *  General (global) definitions
 */

/* Maximum number of columns allowed for display */
#define MAX_COLS	255

/* Log base 2 of 1024 is 10 (2^10 == 1024) */
#define LOG1024		10

/* Special atoi routine returns either a non-negative number or one of: */
#define Infinity	-1
#define Invalid		-2

/* maximum number we can have */
#define Largest		0x7fffffff

/*
 * The entire display is based on these next numbers being defined as is.
 */

#define NUM_AVERAGES    3

struct ext_decl {
    int (*f_minibar)(char *, int);
    int (*f_display)(char *, int);
};

/*
 *  Definitions for things that might vary between installations.
 */

/*
 *  "Table_size" defines the size of the hash tables used to map uid to
 *  username.  Things will work best if the number is a prime number.
 *  We use a number that should be suitable for most installations.
 */
#ifndef Table_size
#define Table_size	8191
#endif

/*
 *  "Nominal_TOPN" is used as the default TOPN when Default_TOPN is Infinity
 *  and the output is a dumb terminal.  If we didn't do this, then
 *  installations who use a default TOPN of Infinity will get every
 *  process in the system when running top on a dumb terminal (or redirected
 *  to a file).  Note that Nominal_TOPN is a default:  it can still be
 *  overridden on the command line, even with the value "infinity".
 */
#ifndef Nominal_TOPN
#define Nominal_TOPN	40
#endif

#ifndef Default_TOPN
#define Default_TOPN	15
#endif

#ifndef Default_DELAY
#define Default_DELAY	5
#endif

/*
 *  If the local system's getpwnam interface uses random access to retrieve
 *  a record (i.e.: 4.3 systems, Sun "yellow pages"), then defining
 *  RANDOM_PW will take advantage of that fact.  If RANDOM_PW is defined,
 *  then getpwnam is used and the result is cached.  If not, then getpwent
 *  is used to read and cache the password entries sequentially until the
 *  desired one is found.
 *
 *  We initially set RANDOM_PW to something which is controllable by the
 *  Configure script.  Then if its value is 0, we undef it.
 */

#define RANDOM_PW	1
#if RANDOM_PW == 0
#undef RANDOM_PW
#endif

#endif
