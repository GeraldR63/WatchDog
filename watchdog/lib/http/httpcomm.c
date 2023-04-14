/*
    Copyright (C) by Christian Gosch and Gerald Roehrbein.
    Added syslogprintf by Gerald Roehrbein.

    This file is part of w3server, a very small HTTP(-subset) server.

    w3server is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <httpclient.h>
#include <httpcomm.h>
#include <logwriter.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <httppost.h>

#include <httpclientconfig.h>

#define LINE_SIZE 10240

int htmlprintf (int sock, const int size, const char* fmt, ...) 
/* 
 * Writes a string to the open socket and works like printf
 * (w) 2007 Gerald Roehrbein
 * If size==0 than fmt is handled like a printf format
 * If size!=0 than size byte of fmt is written RAW to socket
 */

{
  va_list arg;
  int len;
  char buf[LINE_SIZE];
  char *msg=buf;

  if (size == 0)  /* treat arguments like printf */
  {
      va_start (arg, fmt);
      len = vsnprintf (msg,LINE_SIZE,fmt, arg);
      va_end (arg);
  }
  else           /* write raw data to socket */
  {
      msg=fmt;
      len=size; 
  }


  if ( (len < 0) | (write (sock, (void *) msg, len) == -1) ) 
  {
#ifndef NDEBUG
    syslogprintf (__FILE__,__LINE__,"watchdog",5,"htmplprintf Error: sending data");
#endif
    return -1;
  }

  if (len >= LINE_SIZE)
    syslogprintf (__FILE__,__LINE__,"watchdog",2,"htmplprintf Warning: Input line possibly to long!");

  return len;
}


void* w3ReceiveData (unsigned int *size, int sock) {
  size_t s = 0;
  char* data;

  data  = (char*)malloc (1024 * sizeof(char));
  *size = 0;
  do {
    s = read ( sock, (void*)(&((char*)data)[*size]), 1024 );
    if ( s == -1 ) {
#ifndef NDEBUG
      syslogprintf(__FILE__,__LINE__,"watchdog",5,"receiving data"); 
#endif
      return NULL;
    }
    *size += (unsigned int)s;
    /* Remark by Roehrbein: Maybe 1024 + *size is false it should be 1024 * (*size)) */
    data = (char*)realloc ( (void*)data, (size_t)((1024 + *size) * sizeof(char)) );
  } while (s > 0);

  data = (char*)realloc ( (void*)data, (size_t)(*size) * sizeof(char) );

  return (void*)data;
}


#ifndef NDEBUG
/*
 * This function write data received into a file. This is just required to understand the structure
 * of posted data!
 *
 */

#define TMPFILE "/tmp/watchdog.post.data"

void POSTdebug( char * data)
{
  FILE *out=fopen(TMPFILE,"a+");

  if (out)
  {
      fwrite ((char *) data, 1024, 1, out);
      fclose(out);
  }
}

#endif


/*
 *
 * This function will get HTTP POST data and build an array with all data posted
 * by browser.
 *
 */


struct httppost * w3GetPostData (char *data) 
{
  struct httppost *post=NULL;  /* Struct to keep post data */
  char *d;                     /* Pointer to find the first post record */

#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"No more POST data to receive");
#endif


  return post;
}



