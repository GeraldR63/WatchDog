
/*   Watchdog , a simple to use Linux  High Availability management software
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

#include <util.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int search(char *a, char*b)
{
 int l=strlen(a);
 int n=strlen(b);
 int i;
    for (i=0;i<=l;i++)
      if (strncmp(a++,b,n)==0) return 0; 
 return -1;
}


int strcmpi(char *a, char *b)
{
for (; *a && *b && (tolower(*a) == tolower(*b)); ++a, ++b);

return *a - *b;
}


void strcut(char *str)
{
  char *strend=str+strlen(str)-1;
  while(isspace(*strend) && str <= strend)
   *strend-- ='\0';
}


/*
 *   Function isText
 *   Input String
 *   Return       1 if String contains an alphanumerical character
 *                0 if the character contains whitespace
 *  
 */

int isText(char *a)
{
   int n=strlen(a);

   for (;n>=0;n--)
     if (isalnum(a[n])) return 1;

return 0;
}


/* Concat of two strings */
char*
StringCat (char *str1, char *str2) {
  char *str;
  if ( (str2 != NULL) && (str1 != NULL) ) {
    str = (char*)calloc ((strlen(str1) + strlen(str2) + 2), sizeof (char));
    str = strcpy (str, str1);
    return (strcat (str, str2));
  } else {
    if (str1 != NULL) {
      str = StringDup(str1);
    } 
    if (str2 != NULL) {
      str = StringDup(str2);
    } 
    return str;
  }
  return NULL;
}

/* Duplicates two strings */
char*
StringDup (char *str) {
  char *newstr;
  newstr = (char*)calloc ( (strlen(str) + 1), sizeof(char));
  newstr = strcpy (newstr, str);
  return newstr;
}
