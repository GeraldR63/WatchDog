#ifndef W3SERVER_H
#define W3SERVER_H

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
 * \file w3server.h
 * \brief Main functions to set up a server.
 * To set up a server, simply call w3StartServer(int portnumber).
 * The rest will hopefully be done by the server itself.
 */

#include <httptypes.h>
#include <httpcomm.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

/*!
* \addtogroup server
* @{ 
*/
typedef struct {
  int	 socket;
  int	 port;
  char	 *hostname;
  struct sockaddr_in sock_addr;
  struct hostent *host;
} w3Server;

typedef struct {
  w3Server  *server;
  struct    sockaddr_in sock_addr;
  socklen_t addrlen;
  int	    socket;
} w3ServerConnection;

/*!
 * Accepts a connection for server server.
 */
w3ServerConnection* w3ServerAccept (w3Server *server);

/*!
 * Initialization for the server.
 */
boolean w3ServerInit (w3Server *server);

/*!
 * Closes the server.
 */
boolean w3ServerClose (w3Server *server);

/*!
 * Closes a connection to the server. con is deleted.
 */
boolean w3ServerDisconnect (w3ServerConnection* con);



/*!
 * Gets the first token out of str and returns it in a newly allocated char*.
 * idx is an index to the next character in the string str.
 */
char* w3ServerGetToken (char *str, int *idx);

/*!
 * Parses a "GET ..." request and returns a newly allocated char* containing the full path to the
 * requested file.
 * Replace this by a more general request parser.
 */
char* w3ServerGetPath (char *str);

/*!
 * Sends file pointed to by path to connection con.
 * If the file is in the HTDOC_BIN_BASE directory, it is executed and the 
 * standard output stored in a temporary file and sent to the client. The temporary files 
 * are stored in HTDOC_BIN_TEMPLATE and
 * should be removed from time to time, e.g. by a cron job (not done automatically).
 */
boolean w3ServerSendFile (char *path, w3ServerConnection *con);

/*!
 * Takes the appropriate action to notify client at connection con of error errorcode.
 * For errorcodes see file w3server.c (for now).
 */
boolean w3ServerSendError (int errorcode, w3ServerConnection *con);

/*!
 * After a connection has been established, serve the client.
 * This function does not disconnect.
 */
boolean w3ServerServeClient (w3Server *server, w3ServerConnection *con);

/*!
 * Starts a server on port port.
 */
/* boolean w3StartServer (void * port); */
void * w3StartServer (void * port);

/*!
 * Prints message msg to local console.
 */
void w3ServerMessage (const char* msg);

/*! @} */

#endif
