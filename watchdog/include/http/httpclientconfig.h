/*
    Copyright (C) by Christian Gosch.
    Copyright (C) 2007 by Gerald Roehrbein

    This file is part of w3server and Watchdog a High Availability suite
    configurable by, a very small HTTP(-subset) server.

    w3server and Watchdog is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Watchdog is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef __W3CLIENT_CONFIG_H
#define __W3CLIENT_CONFIG_H

#include "config.h"

/* #define __W3CLIENT_DEBUG 1 */
#undef __W3CLIENT_DEBUG

#undef  W3CLIENT_CREATE_CACHE

#define W3_CLIENT_CACHE_DIR_DEF "cache/"
#define W3_CLIENT_INDEX_HTML_DEF "/index.html"


#endif
