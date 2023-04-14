
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


#include <options.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <logwriter.h>
#include <string.h>

#ifndef NDEBUG
#include <write_png.h>
#endif

enum boolean {true, false}; 
static const char *optString = "i:p:d:hst:o:nc:e:b:T?";
void display_usage( void );


void initoptions()
/*
 * Set defaults for command line options
 *
 */
{
    /* 
     * Initialize globalArgs before we start  work. 
     * flags is a bitfield which represents changes of params via commandline
     * if a parameter was set by commandline than the corresponding 
     * flag is toggled to 1.
     *
     */

    globalArgs.iniFile = "watchdog.ini";
    globalArgs.flags.iniFile=0;
    globalArgs.oneshot = "generic";               /* Not used */
    globalArgs.flags.oneshot=0;
    globalArgs.http_port = 81;
    globalArgs.flags.http_port=0;
    globalArgs.debug_level = 1;
    globalArgs.flags.debug_level=0;

    /* Default is NO HTTP server */
    globalArgs.run_http = 0;
    globalArgs.flags.run_http=0;
    globalArgs.runflags.keep_HTTP_going=0;

    /* Default is NO DAEMON */
    globalArgs.run_daemon = 0;
    globalArgs.flags.run_daemon=0;
    globalArgs.runflags.keep_going=0;
    globalArgs.runflags.keep_monitoring_going=0;

    globalArgs.run_scheduler = 0;                  /* Stupid option. If run as daemon scheduler have to run! */
    globalArgs.flags.run_scheduler=0;
    globalArgs.schedule = 120;
    globalArgs.flags.schedule=0;
    globalArgs.httpbase = "/etc/watchdog/htdocs";  /* Only set in ini file !*/
    globalArgs.flags.httpbase=0;
    globalArgs.cgibase = "/cgi-bin";               /* Only set in ini file !*/
    globalArgs.flags.cgibase=0;
    globalArgs.index_file = "index.html";          /* Only set in ini file !*/
    globalArgs.flags.index_file=0;
    globalArgs.error_file = "/error.html";         /* Not realy used today !*/
    globalArgs.flags.error_file=0;

    globalArgs.runflags.run_mysql_stats=1;        /* 
                                                      This flags allows or disallows writing
                                                      global or process statistics to 
                                                      a MySQL connection
                                                  */
     memset(globalArgs.hostname,0,1024);
     gethostname (globalArgs.hostname, 1024 -1 ); 
}


void getoptions(int argc, char *argv[])
{
  enum boolean error=false;
  int opt = 0;

  initoptions();
  opt = getopt( argc, argv, optString );

    /*
     * Get and check all Arguments as required to get a running programm!
     *
     */

    while( opt != -1 ) {
        switch( opt ) {
            case 'b':
                       globalArgs.httpbase=optarg;
                       globalArgs.flags.httpbase=1;
#ifndef NDEBUG
		       syslogprintf(__FILE__,__LINE__,"watchdog",5,"HTTP directory is %s\n",optarg);
#endif
                       break;
            case 'c':
                       globalArgs.cgibase=optarg;
                       globalArgs.flags.cgibase=1;
#ifndef NDEBUG
		       syslogprintf(__FILE__,__LINE__,"watchdog",5,"CGI subdirectory is %s\n",optarg);
#endif
                       break;

            case 'i':
                {
                FILE * in=fopen(optarg,"r");
                if (in)
                {
                globalArgs.iniFile = optarg; /* true */
                globalArgs.flags.iniFile=1;
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Inififile is %s\n",optarg);
#endif
			    }
			    else
			    {
				   puts( "*******************************************************************");
				   puts( "*** -i violation. No such file.                                 ***");
				   puts( "*******************************************************************");
				   exit( EXIT_FAILURE );
			    }

			    }
                break;

            case 'o':  /* Do we really need this option ? */
                if (error == false )
                {
                globalArgs.oneshot = optarg; /* true */
                globalArgs.flags.oneshot=1;
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Check only section [%s] of %s\n",optarg, globalArgs.iniFile);
#endif
                exit (ONE_SHOT);
			    }
			    else
			    {
				   puts( "*******************************************************************");
				   puts( "*** -o violation. It's only allowed to use -o and -i together.  ***");
                                   puts( "*******************************************************************");
                                   exit( EXIT_FAILURE );
			    }
                break;
            case 'n':
                globalArgs.run_daemon=1;
                globalArgs.flags.run_daemon=1;
                globalArgs.runflags.keep_going=1;
                globalArgs.runflags.keep_monitoring_going=1;

#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Configured to run as a daemon");
#endif
                error=true;
                break;

            case 'p':
                globalArgs.http_port = atoi(optarg);
                globalArgs.flags.http_port=1;
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Port is %s\n",optarg);
#endif
                error=true;  /* Don't use this param with -o! */
                break;

            case 'd':
                globalArgs.debug_level = atoi(optarg);
                globalArgs.flags.debug_level=1;

                if (((globalArgs.debug_level >-1) & (globalArgs.debug_level <6)) & ! isalpha(optarg[0]) )
                {
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Debug level is %d\n",globalArgs.debug_level);
#endif
                error=true;  /* Don't use this param with -o! */
			    }
			    else
			    {
				   puts( "*******************************************************************");
				   puts( "*** -d number format violation . [0-5]                          ***");
                   		   puts( "*******************************************************************");
                   		   exit( EXIT_FAILURE );
			    }

                break;

            case 'h':
                globalArgs.run_http=1;
                globalArgs.flags.run_http=1;
                globalArgs.runflags.keep_HTTP_going=1;
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Starting HTTP server\n");
#endif
                error=true;  /* Don't use this param with -o! */
                break;

            case 's':  /* Do we really need this? */
                globalArgs.run_scheduler=1;
                globalArgs.flags.run_scheduler=1;
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Enabling scheduler\n");
#endif
                error=true;  /* Don't use this param with -o! */
                break;

            case 't':
                globalArgs.schedule= atoi(optarg);
                globalArgs.flags.schedule=1;
                if ((globalArgs.schedule >59) & (globalArgs.schedule <3601) )
                {
#ifndef NDEBUG
                syslogprintf(__FILE__,__LINE__,"watchdog",5,"Scheduler set to %d seconds\n", globalArgs.schedule);
#endif
                error=true;  /* Don't use this param with -o! */
			    }
			    else
			    {
				   puts( "*******************************************************************");
				   puts( "*** -t number format violation . [60-3600]                      ***");
                                   puts( "*******************************************************************");
                                   exit( EXIT_FAILURE );
                }

                break;
#ifndef NDEBUG
            case 'T':   /* This is a special Test mode used during debugging of PNG image creation */
                     fprintf(stderr,"Creating PNG testimage in /tmp\n");
			int i;
			struct pngwriter * p;

			p=(struct pngwriter *) malloc(sizeof(struct pngwriter));

			write_png_init(p, 800,600,32768,"/tmp/watchdog.png",-1);

			write_png_set_info(p, "Watchdog", "Gerald Roehrbein", "Watchdog auto created image", "Watchdog");


			for (i=0; i<300; i++)
			 write_png_caleidoscope(p);

			write_png_finish(p);

                     fprintf(stderr,"End of testmode!\n");
                     exit(10);
                     break;
#endif

            case '?':
                display_usage();
                break;

            default:
                /* You won't actually get here. */
                display_usage();
                break;
        }

        opt = getopt( argc, argv, optString );
    }



}



void display_usage( void )
{
    puts( "*******************************************************************");
    puts( "*** Watchdog - High Availability processor                      ***" );
    puts( "*** (c) 1989-2007 OraForecast.com                               ***" );
    puts( "***                                                             ***" );
    puts( "*** Gerald.Roehrbein@OraForecast.com                            ***" );
    puts( "***                                                             ***" );
    puts( "*** Commandline options                                         ***" );
    puts( "***                                                             ***" );
    puts( "*** -i <inifile> path and name of Watchdog's ini file           ***" );
    puts( "*** -p <number> where Watchdog will offer HTTP protocol.        ***" );
    puts( "*** -d <number> Debuglevel between 0 and 5.                     ***" );
    puts( "*** -h enable HTTP server.                                      ***" );
    puts( "*** -s enable scheduler.                                        ***" );
    puts( "*** -t check interval. Check delay between 50% and 100% of this ***" );
    puts( "*** -n enable daemon mode. Run Watchdog as a background daemon  ***" );
    puts( "*** -o <section> check one section defined in ini file.         ***" );
    puts( "*** -? Help                                                     ***" );
    puts( "***                                                             ***" );
    puts( "*** Watchdog executes parameters by their order.                ***" );
    puts( "*** If you want to start a HTTP server at a different port than ***" );
    puts( "*** you have to set the port first.                             ***" );
    puts( "*** If not the server listens at the default port.              ***" );
    puts( "***                                                             ***" );
    puts( "*** If Watchdog is in daemon mode it's controlled by three      ***" );
    puts( "*** signals                                                     ***" );
    puts( "***                                                             ***" );
    puts( "*** kill -s SIGTERM <pid> Terminate                             ***" );
    puts( "*** kill -s SIGUSR1 <pid> Force reload of /proc                 ***" );
    puts( "*** kill -s SIGUSR2 <pid> Force reload of ini file              ***" );
    puts( "***                                                             ***" );
    puts( "*******************************************************************" );

    exit( EXIT_FAILURE );
}
