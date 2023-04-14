
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

#include <profile.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>
#include <util.h>
#include <syslog.h>
#include <stdlib.h>
#include <logwriter.h>

int getPrivateProfileString(char *s, char *k, char *value, char *file)
{
  char section[123];
  int retval=0;
  FILE * in ;
  char key[123];

  sprintf(section,"[%s]",s);

  sprintf(key,"%s=",k);


  in = fopen(file,"rt");
  if(in)
  {
    static char line[1024];
    int s=0;
    while(!feof(in))
      {
        fgets(line,1024,in);
        if (line[0]==';') continue;

        if ((line[0]=='[') & (s==1) ) break; /* next section begins */

        if (search(line,section)==0) 
           s=1;

        if((s==1) & (search(line,key)==0))
        {
           strtok(line,"=");
           strcpy(value,strtok(NULL,"="));
           /* Last character is linefeed. Replace this with string end! */
           value[strlen(value)-1]=0;
           syslogprintf(__FILE__,__LINE__,"watchdog",5,"Found in section %s %s%s",section,key,value);
           break;
        }
      } 
    fclose(in);
  }
  else
  {
   syslogprintf(__FILE__,__LINE__,"watchdog",5,"File %s not found.",file);
   RNASSERT("watchdog.ini not found. Increase debuglevel to 5 for more info");
  }

return retval;
}

