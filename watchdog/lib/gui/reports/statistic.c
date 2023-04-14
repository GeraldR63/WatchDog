
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




/*
 * HTML Status report creation of monitored processes
 *
 */



#include <statistic.h>
#include <httpcomm.h>
#include <htmlgen.h>
#include <watchlist.h>
#include <write_png.h>

/*
 * pinfo process info  Format watchdog?GRAPH-<process name>
 *
 */
void statisticreport(int port, char *pinfo)
{




HTMLPageHeader (port,"Watchdog: Process statistics report",pinfo,"icons/watchdog_bg.png",5);

HeadLine(port,"Statistics Report (under construction)(just a demo of watchdogs png library)");

char *dummy="<a href=/help/statistics.html><img\
 style=\"width: 800px; height: 600px;\" alt=\"Process statistics graph\"\
 title=\"Process statistics\"\
 src=\"/watchdog?DemoImage.png\"></a>\n";

htmlprintf(port,0,dummy);


HTMLPageFooter(port);
 
}



void DemoImage(int port)
{
 int i;
 struct pngwriter * p;

                        p=(struct pngwriter *) malloc(sizeof(struct pngwriter));

                        write_png_init(p, 800,600,32768,"/tmp/watchdog.png",port);
                        write_png_set_info(p, "Watchdog", "Gerald Roehrbein", "Watchdog auto created image", "Watchdog");

                        for (i=0; i<300; i++)
                         write_png_caleidoscope(p);

			write_png_plot_text( p, p->width/2 - (6*p->fontsize), p->height/2 - (p->fontsize) ,
                                             0, "OraForecast.com", 65535,0,0);
                        write_png_finish(p);
                        
                        free(p);

}
