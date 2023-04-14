#ifndef __OPTIONS_H
#define __OPTIONS_H

#define ONE_SHOT 0
#define EXIT_FAILURE 1

/*enum boolean {true, false}; */

struct globalArgsFlags { 
   /* 
    * 
    * A bit field which represents all options set by command line 
    * If a parameter in globaArg_t was set by commandline than
    * the corresponding variable of this bitfield at toggled to 1!
    * 
    */

   unsigned iniFile:1;            
   unsigned http_port:1;         
   unsigned debug_level:1;      
   unsigned run_http:1;        
   unsigned run_daemon:1;
   unsigned run_scheduler:1;
   unsigned schedule:1;
   unsigned oneshot:1;
   unsigned httpbase:1;
   unsigned cgibase:1;
   unsigned index_file:1;
   unsigned error_file:1;

} globalArgsFlags;

struct globalRunFlags {
   unsigned keep_going:1;             /* Set to 0 if you want to stop watchdog normal */
   unsigned keep_monitoring_going:1;   /* Set to 0 if you want to stop all process monitors */

   unsigned keep_HTTP_going:1;        /* Set to 0 if you want to stop Watchdogs HTTP server */

   unsigned run_mysql_stats:1;  /* Special flag is set to 0 if there was an MySQL error
                                   This flag is reseted to 1  by SIGUSR2 only or if you restart
                                   Watchdog. This flag is meant to handle errors with
                                   the MySQL database gracefull.
                                   The flag is set to 0 only by function check_db_error in
                                   lib/statistics/w_mysql.c
                                 */
};


volatile struct globalArgs_t {
        /* Shows if a parameter was set by a command line option! */
        struct globalArgsFlags flags;

        /* The runflags are used to control Watchdog's threads */
        struct globalRunFlags  runflags;

	char * iniFile;    /* Default watchdog.ini */
	int http_port;     /* Default 81 */
	int debug_level;   /* Default 5 - show all errors */
	int run_http;	   /* Default 0 - do not run HTTP server */
	int run_daemon;    /* Default 0 - do not run as daemon */
	int run_scheduler; /* Default 1 - run scheduler threads. Do not change this parameter! */
	int schedule;	   /* Default 60 - random between 120/2 and 120 seconds */
	char * oneshot;    /* Default generic - run check for  only one section  */
        char * httpbase;   /* Default /etc/watchdog/htdocs  */
        char * cgibase;    /* Default /cgi-bin */
        char * index_file; /* Default index.html */
        char * error_file; /* Default error.html */
        char  hostname[1024];   /* Points to hostname !*/
} globalArgs;

/* volatile struct globalArgs globalArgs; */

void getoptions(int argc, char *argv[]);


#endif
