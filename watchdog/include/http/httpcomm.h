/*
    Copyright (C) by Christian Gosch.
    Copyright (C) 2007 by Gerald Roehrbein

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
 * \file w3comm.h
 * \brief Convenient communication functions (to get rid of thr messy read/write stuff).
 */

#ifndef __HTTPCOMM_H__
#define __HTTPCOMM_H__

#include <httptypes.h>
#include <httpclient.h>
#include <httppost.h>

#include <sys/types.h>

/*
 *
 * htmlprintf
 *
 * A printf like function which writes some text  to a given socket
 * (w) 2007 by Gerald Roehrbein as a replacement for w3SendData
 *
 * If size==0 than it works like printf
 * If size!=0 than size bytes of fmt will be written to socket
 *
 */

int htmlprintf (const int sock, const int size, const char* fmt, ...);


/*!
 * Receives data from <CODE>con</CODE> into <CODE>void *data</CODE>. The size of the received block is 
 * stored in <CODE>size</CODE>.
 * \brief Receive data through a connection.
 * \author Christian Gosch
 */

void* w3ReceiveData (unsigned int *size, int sock);
/*!
 * @}
 */

/* This function creates an array containing for each posted variable on entry!
 * It returns a pointer to the posted data.
 * First elements member n contains the number of variables found and returned by
 * w3GetPostData
 *
 * If no POST data found a NULL pointer is returned!
 *
 * Author: Gerald Roehrbein
 */
struct httppost  *  w3GetPostData ( char *);


#endif







