/*
    Copyright (C) by Christian Gosch.

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
/*!
 * \file w3types.h
 * \brief Type definitions.
 * \author Christian Gosch
 */

#ifndef __HTTPTYPES_H__
#define __HTTPTYPES_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*!
 * \addtogroup client
 * @{
 */
/**
 * Describes a URI. Use w3ParseURI to retrieve this structure out of a given const char* string.
 */
typedef struct {
  /** Name of the host running the server. */
  char      *hostname;
  /** Complete path to the file including all necessary slashes and so on. */
  char      *path;
  /** Complete filename of the file to be retrieved. THIS IS NOT USED YET ! */
  char	    *filename;
  /** Port number of the server process. HTTP servers normally run on port 80. */
  short int port;

} w3URI;

/**
 * Connection data structure for the client.
 */
typedef struct {
  /** Name of the host running the server. */
  char	    *hostname;
  /** Contains a valid socket descriptor after a connection has successfully been set up. */
  int       socket;
  /** Contains the sockaddr data used by lower level functions. Normally the user should not need to know about this. */
  struct    sockaddr_in sockaddr;
  /** Port number of the server process. HTTP servers normally run on port 80. */
  short int port;

} w3ClientConnection;

/**
 * Data structure for a document.
 */
typedef struct {
  /** URI structure describing the file containing the document. */
  w3URI        uri;
  /** The actual document data. Normally filled in by <CODE>w3ClientGetDocument()</CODE>. */
  char         *data;
  /** Length of the document data. */
  unsigned int dataLength;
} w3Document;


typedef int boolean;

#define true 1
#define false 0



#ifndef CR
#define CR 13
#endif

#ifndef LF
#define LF 10
#endif

/*!
 * @}
 */

#endif










