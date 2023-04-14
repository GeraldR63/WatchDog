
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



#ifndef __UTIL_H
#define __UTIL_H


/*
 * search or instring function
 * if b exists in a than return 0 else -1
 *
 */
int search(char *a, char*b);

/*
 * not case sensitive string compare
 *
 */

int strcmpi(char *a, char *b);

/*
 * Remove any whitespace at the end of a string
 */

void strcut(char*);
/*
 *   Function isText
 *   Input String
 *   Return       1 if String contains an alphanumerical character
 *                0 if the character contains whitespace
 *   
 */

int isText(char *);


char *hostname(void);

/* Concat of two strings */
char* StringCat (char *str1, char *str2);

/* Duplicates two strings */
char* StringDup (char *str);



#endif

