/*
    Copyright (C) by Christian Gosch.
    Copyright (C) by Gerald Roehrbein

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
 * \file w3client.h
 * \brief Connection and higher level communication functions.
 */

#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <httptypes.h>

/*!
 * \addtogroup client
 * @{
 */

struct  clientGlobalStruct {
  int numOfThreads;
  char *hostname;
  char *lastpath;
  int port;
};

/*!
 * Initialization of global data structures. Call this before doing anything with the client functions !
 */
void w3ClientInit ();

/*!
 * Sets up a connection to <CODE>con</CODE>.
 * The following members of <CODE>con</CODE> have to be set correctly: <br>
 * <CODE>hostname</CODE>
 * <CODE>port</CODE>
 * \author Christian Gosch
 */
boolean w3ClientConnect (w3ClientConnection *con);

/*!
 * Gets a document described by <CODE>doc</CODE>.
 * All fields in <CODE>doc</CODE> except <CODE>data</CODE> and <CODE>dataLength</CODE> have to be set
 * correctly. The latter two are filled by this function, <CODE>data</CODE> contains the received
 * data of length <CODE>dataLength</CODE>.
 * \return <CODE>true</CODE> if successful, otherwise <CODE>false</CODE>.
 * \author Christian Gosch
 */
boolean w3ClientGetDocument (w3Document *doc);

/*!
 * Closes the connection <CODE>con</CODE>.
 * \return <CODE>true</CODE> if successful, otherwise <CODE>false</CODE>.
 * \author Christian Gosch
 */
boolean w3ClientDisconnect (w3ClientConnection *con);


boolean w3ClientGetImagePath (char *uriString, boolean withoutHostname);

/*!
 * Retrieves the URI described in uriString to file filename.
 * Also retrieves all images using the lexical analyzer for images (img.lex).
 * The html file is stored in <CODE>filenamey</CODE> and in the cache directory defined
 * by <CODE>W3_CLIENT_CACHE_DIR</CODE>.
 * Downloaded html files are stored flatly in the cache by using the whole URI and substituting
 * slashes by underscores ("/" --> "_"). So, <br>
 * www.goschs.de/hund/katze/maus.html would be cache/www.goschs.de_hund_katze_maus.html<br>
 * All images are stored in subdirectories, starting with the hostname. E.g., <br>
 * www.goschs.de/www/images/hund.gif would be <br>
 * cache/www.goschs.de/www/images/hund.gif
 */
boolean w3ClientDocToFile (const char* uriString, const char *filename, int port);

/*!
 * This is not supporting the cache structure yet. It is not maintained since 
 * it is not yet needed for the assignment in CNS.
 * Opens a thread and returns immediately, without joining the thread.
 * Use w3ClientJoin- od DestroyAllThreads () to join them. Be sure to always call one of the latter
 * two to free the memory used by the thread structures, as soon as you have no more threads pending.
 */
boolean w3ClientDocToFileMT (const char* uriString, const char *filename, int port);

/*!
 * Joins all pending threads and frees the taken memory.
 */
boolean w3ClientJoinAllThreads ();

/*!
 * Kills all pending threads and frees the taken memory. 
 */
boolean w3ClientDestroyAllThreads ();

/*!
 * @}
 */

#endif





