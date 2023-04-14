/*    Watchdog , a simple to use Linux  High Availability management software
 *    Copyright (C) 1989-2007  Gerald Roehrbein
 *  
 *    This software contains libraries I've alread implemented between 1989
 *    and 2007.
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *  
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *  
 *  
 *    Contact information:
 *  
 *    e-mail: Gerald.Roehrbein@OraForecast.com
 *  
 *    Gerald Roehrbein
 *    Boskamp 19
 *    24214 Gettorf
 *    Germany
 *  
 * 
 *    Watchdog version 0.9, Copyright (C) 1989-2007 by Gerald Roehrbein
 *    Watchdog comes with ABSOLUTELY NO WARRANTY
 *    This is free software, and you are welcome to redistribute it
 *    under certain conditions; Please read additional LICENSE file.
 *   
 */


#include <httpcfg.h>
#include <profile.h>
#include <string.h>
#include <stdlib.h>
#include <options.h>
#include <logwriter.h>

/*
 *   Reads section http from ini file as described in header
 *  
 */

void getIniHTTP(char *ini)
{
  char *section="http";

  memset(&iniHTTP,0,sizeof(iniHTTP));


  /*
   *
   * Read ini params from ini file and change configuration
   * of HTTP server settings if they are not set by 
   * command line
   *
   */  

  getPrivateProfileString(section,"root",iniHTTP.root,ini);
  if (globalArgs.flags.httpbase == 0)  /* Not overriden by param use INI setting! */
  {
     globalArgs.httpbase=iniHTTP.root;
#ifndef NDEBUG
     syslogprintf(__FILE__,__LINE__,"watchdog",5,"HTTPBASE='%s'",globalArgs.httpbase);
#endif
  }

  getPrivateProfileString(section,"cgi-bin",iniHTTP.cgibin,ini);
  if (globalArgs.flags.cgibase == 0)  /* Not overriden by param use INI setting! */
  {
     globalArgs.cgibase=iniHTTP.cgibin;
#ifndef NDEBUG
     syslogprintf(__FILE__,__LINE__,"watchdog",5,"CGI-BIN='%s'",globalArgs.cgibase);
#endif
  }  
  getPrivateProfileString(section,"port",iniHTTP.port,ini);
  iniHTTP.p=atoi(iniHTTP.port);

  if (globalArgs.flags.http_port == 0)  /* Not overriden by param use INI setting! */
     globalArgs.http_port=atoi(iniHTTP.port);
  
  getPrivateProfileString(section,"index_file",iniHTTP.index_file,ini);
  
  if (globalArgs.flags.index_file == 0)  /* Not overriden by param use INI setting! */
  {
     globalArgs.index_file=iniHTTP.index_file;
#ifndef NDEBUG
     syslogprintf(__FILE__,__LINE__,"watchdog",5,"INDEX_FILE='%s'",iniHTTP.index_file);
#endif
  }
  
 
}




