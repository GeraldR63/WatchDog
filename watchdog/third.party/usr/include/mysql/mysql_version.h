/* Copyright Abandoned 1996, 1999, 2001 MySQL AB
   This file is public domain and comes with NO WARRANTY of any kind */

/* Version numbers for protocol & mysqld */

#ifndef _mysql_version_h
#define _mysql_version_h
#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION		10
#define MYSQL_SERVER_VERSION		"4.0.24"
#define MYSQL_BASE_VERSION		"mysqld-4.0"
#define MYSQL_SERVER_SUFFIX_DEF		"_Debian-10ubuntu2"
#define FRM_VER				6
#define MYSQL_VERSION_ID		40024
#define MYSQL_PORT			3306
#define MYSQL_UNIX_ADDR			"/var/run/mysqld/mysqld.sock"
#define MYSQL_CONFIG_NAME		"my"
#define MYSQL_COMPILATION_COMMENT	"Source distribution"

/* mysqld compile time options */
#ifndef MYSQL_CHARSET
#define MYSQL_CHARSET			"latin1"
#endif /* MYSQL_CHARSET */
#endif /* _CUSTOMCONFIG_ */

#ifndef LICENSE
#define LICENSE				GPL
#endif /* LICENSE */

#endif /* _mysql_version_h */
