
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


#include <string.h>
#include <math.h>

#include <ps.h>
#include <httpcomm.h>
#include <htmlgen.h>
#include <watchlist.h>
#include <dirdo.h>
#include <datum.h>

#include <watchdog.h>
#include <watchlist.h>

#include <machine.h> 
#include <username.h>
#include <putils.h>
#include <w2top.h>
#include <cntusr.h>


#include <pwd.h>
#include <sys/types.h>
#include <sys/param.h>   /* for HZ */
#include <sys/time.h>
#include <time.h>

#include <version.h>


char *green="<a href=/help/processes.html><img\
 style=\"width: 12px; height: 12px;\" alt=\"Monitored and healthy\"\
 title=\"Monitored and healthy\"\
 src=\"/icons/green.png\"></a>\n";
char *yellow="<a href=/help/processes.html><img\
 style=\"width: 12px; height: 12px;\" alt=\"Not monitored but running\"\
 title=\"Not monitored but running\"\
 src=\"/icons/yellow.png\"></a>\n";
char *red="<a href=/help/processes.html><img\
 style=\"width: 12px; height: 12px;\" alt=\"Monitored and not running\"\
 title=\"Monitored and not running\"\
 src=\"/icons/red.png\"></a>\n";
char *magenta="<a href=/help/processes.html><img\
 style=\"width: 12px; height: 12px;\" alt=\"Unknown state\"\
 title=\"Unknown state\"\
 src=\"/icons/magenta.png\"></a>\n";


char *psinfo[]={
"<table style=\"text-align: left; width: 100%; background-color: rgb(0, 0,0);\" border=\"1\" class=\"table\">",
"  <tbody>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\">Legend</td><td></td><td></td><td></td><td></td><td align\"right\" valign=\"middle\">Version %s</td>\n",
"    </tr>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\">%s</td>\n", /* 6*/
"      <td align=\"right\" valign=\"middle\">%s</td>\n",
"      <td align=\"right\" valign=\"middle\">%s</td>\n",
"      <td align=\"right\" valign=\"middle\">%s</td>\n", /* 9*/
"      <td align=\"right\" valign=\"middle\"><a href=/help/gen_proc_help.html>Help</a></td>\n", 
"    </tr>",
"  </tbody>",
"</table>",
NULL
};


char *psbeg[]={
"<table style=\"text-align: left; width: 100%; background-color: rgb(0, 0,0);\" border=\"1\" class=\"table\">",
"  <tbody>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\"></td>\n",
"      <td align=\"right\" valign=\"middle\">%s %s</td>\n", /* 4 */
"      <td align=\"right\" valign=\"middle\">%s up</td>\n",  /* 5 */
"      <td align=\"right\" valign=\"middle\">%d users</td>\n", /* 6 */
"      <td align=\"right\" valign=\"middle\">load average</td>",
"      <td align=\"right\" valign=\"middle\">%3.2f, %3.2f, %3.2f</td>\n", /* 8 */
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\"></td>",
"     <td align=\"right\" valign=\"middle\">Tasks:</td>",
"      <td align=\"right\" valign=\"middle\">%5d total</td>",   /* 14 */
"      <td align=\"right\" valign=\"middle\">%5d running</td>", /* 15 */
"     <td align=\"right\" valign=\"middle\">%5d sleeping</td>", /* 16 */
"      <td align=\"right\" valign=\"middle\">%5d stopped</td>", /* 17 */
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\">%5d zombie</td>",  /* 18 */
"    </tr>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\"></td>",
"     <td align=\"right\" valign=\"middle\">Cpu(s):</td>",
"      <td align=\"right\" valign=\"middle\"> %3.2f%% us</td>",  /* 23 */
"      <td align=\"right\" valign=\"middle\"> %3.2f%% sy</td>",  /* 24 */
"     <td align=\"right\" valign=\"middle\">  %3.2f%% ni</td>",   /* 25 */
"      <td align=\"right\" valign=\"middle\"> %3.2f%% id</td>",  /* 26 */
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\"> %3.2f%% wa</td>",  /* 27 */
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\"> %3.2f%% hi</td>",  /* 28 */
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\"> % si</td>", /* 29 */
"    </tr>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\"></td>",
"      <td align=\"right\" valign=\"middle\">Mem:</td>",
"      <td align=\"right\" valign=\"middle\">%d k total</td>\n", /*23+9+2*/
"      <td align=\"right\" valign=\"middle\">%d k used</td>\n",
"      <td align=\"right\" valign=\"middle\">%d k free</td>\n",
"      <td align=\"right\" valign=\"middle\">%d k buffers</td>\n", /*26+9+2*/
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\"></td>",
"      <td align=\"right\" valign=\"middle\">Swap:</td>",
"      <td align=\"right\" valign=\"middle\">%d k total</td>\n", /* 32+9+2 */
"      <td align=\"right\" valign=\"middle\">%d k used</td>\n",
"      <td align=\"right\" valign=\"middle\">%d k free</td>\n",
"      <td align=\"right\" valign=\"middle\">%d k cached</td>\n", /* 35+9+2 */
"      <td colspan=\"7\" rowspan=\"1\" align=\"right\" valign=\"middle\"></td>",
"    </tr>",
"    <tr>",
"      <td colspan=\"13\" rowspan=\"1\" align=\"right\" valign=\"middle\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"right\" valign=\"middle\">WD</td>",
"      <td align=\"right\" valign=\"middle\">PID</td>",
"      <td align=\"right\" valign=\"middle\">USER</td>",
"      <td align=\"right\" valign=\"middle\">PR</td>",
"      <td align=\"right\" valign=\"middle\">NI</td>",
"      <td align=\"right\" valign=\"middle\">VIRT</td>",
"      <td align=\"right\" valign=\"middle\">RES</td>",
"      <td align=\"right\" valign=\"middle\">SHR</td>",
"      <td align=\"right\" valign=\"middle\">S</td>",
"      <td align=\"right\" valign=\"middle\">%CPU</td>",
"      <td align=\"right\" valign=\"middle\">%MEM</td>",
"      <td align=\"right\" valign=\"middle\">TIME+</td>",
"      <td align=\"right\" valign=\"middle\">COMMAND</td>",
"    </tr>",
NULL};

char * psred[] = {
"    <tr>\n",
/* "      <td align=\"right\" valign=\"middle\"></td>\n",     1 Prozess Graphs  */
"\n",
"      <td align=\"right\" valign=\"middle\">%s %s</td>\n",   /*  2 Graphics and WD Status */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  3 PID */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  4 User */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  5 Prio */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  6 Nice */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  7 Virt */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  8 RES  */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  9 SHR  */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  10 State */
"      <td align=\"right\" valign=\"middle\">%s%%</td>\n",   /*  11 % CPU */
"      <td align=\"right\" valign=\"middle\">%s%%</td>\n",   /*  12 % MEM */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  13 Time */
"      <td align=\"left\" valign=\"middle\">%s</td>\n",    /*  14 Command */
"      </tr>\n",
NULL};


char * pslist[] = {
"    <tr>\n",
/* "      <td align=\"right\" valign=\"middle\"></td>\n",     1 Prozess Graphs  */
"\n",
"      <td align=\"right\" valign=\"middle\">%s %s</td>\n",   /*  2 Graphics and WD Status */
"      <td align=\"right\" valign=\"middle\">%5d</td>\n",   /*  3 PID */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  4 User */
"      <td align=\"right\" valign=\"middle\">%3d</td>\n",   /*  5 Prio */
"      <td align=\"right\" valign=\"middle\">%4d</td>\n",   /*  6 Nice */
"      <td align=\"right\" valign=\"middle\">%5s</td>\n",   /*  7 Virt */
"      <td align=\"right\" valign=\"middle\">%5s</td>\n",   /*  8 RES  */
"      <td align=\"right\" valign=\"middle\">%-5s</td>\n",   /*  9 SHR  */
"      <td align=\"right\" valign=\"middle\">%6s</td>\n",   /*  10 State */
"      <td align=\"right\" valign=\"middle\">%5.2f%%</td>\n",   /*  11 % CPU */
"      <td align=\"right\" valign=\"middle\">%5.2f%%</td>\n",   /*  12 % MEM */
"      <td align=\"right\" valign=\"middle\">%s</td>\n",   /*  13 Time */
"      <td align=\"left\" valign=\"middle\">%s</td>\n",    /*  14 Command */
"      </tr>\n",
NULL};

char *psend[]={
"  </tbody>",
"</table>",
NULL
};

char * uptime()
{
       static char rv[64];
       double min;
       int m, hours, days;

       min=statics.boottime/60;
       hours=min/60;
       days= floor(hours/24);
       hours=floor(hours - (days * 24));
       m=floor(min - (days * 60 * 24) - (hours * 60));
       sprintf(rv,"%d days, %02d:%02d",days,hours,m);

return rv;
}


char * process_image (const char *pname, const int force_red)
/*
 * Return image of process state dependent on real state of process with PID 
 * green.png   - Process is Watchdog monitored and everything is fine
 * yellow.png  - Process is not Watchdog monitored but running
 * red.png     - Process is Watchdog monitored and expected to run but it does not run
 * magenta     - Process have an unknown state
 *
 *
 * Via this Icon a user can access much more detail process information
 * If a process is green a user can access all configuration data of the process
 * If a process is yellow it will be allowed to add Watchdog monitoring
 * If a process is red it will be allowed to manual restart the process and access the logfiles of the process
 * If a process is magenta than it seem that there is an error
 *
 * ToDo: Add this behaviour to this function
 *
 */
{
	static char img[256];

	if (search_in_ini_proc(pname) == 1)
	{
		sprintf(img,"<a href=/watchdog?INI-%s><img\
		 style=\"width: 12px; height: 12px;\" alt=\"Monitored and running\"\
		 title=\"Monitored and running\"\
		 src=\"/icons/green.png\"></a>\n",pname);
	}
	else
	{
		sprintf(img, "<a href=/watchdog?INI-%s><img\
		 style=\"width: 12px; height: 12px;\" alt=\"Not monitored but running\"\
		 title=\"Not monitored but running\"\
		 src=\"/icons/yellow.png\"></a>\n",pname);
	}

        if (force_red == 1)
	{
		sprintf(img, "<a href=/watchdog?INI-%s><img\
		 style=\"width: 12px; height: 12px;\" alt=\"Monitored but not running\"\
		 title=\"Monitored but not running\"\
		 src=\"/icons/red.png\"></a>\n",pname);
	}


return img;
}

/*
 * Create graph for processes runtime
 *
 */

char *graphics_image(const char *pname)
{
static char img[256];

sprintf(img,"<a href=/watchdog?GRAPH-%s><img\
 style=\"width: 24px; height: 12px;\" alt=\"Process statistics\"\
 title=\"Create an image with process statistics\"\
 src=\"/icons/graphics.png\"></a>\n",pname);

return img;
}

void list_failed_processes(sock)
{
caddr_t processes;

struct check_processes *t=cpRoot;  /* This is the list of all processes to be monitored from ini file !*/

while ( t!= NULL)                  /* Check each process which have to run */
{
	int found,i,n;
	resetnextproc();           /* Let the list point to first element of process list */
        found=0;                   /* Assume that process does not exist */

                /* Search for the process to be monitored in the list of running processes */
		for (i = 0 ; i < system_info.P_ACTIVE; i++)
		{
		  struct top_proc *p;
		  p = getnextproc();
		  if (p==NULL) continue;

		  if (strstr(t->check,p->name))
                  {
                      found=1;
                      break;
                  }
		}

         if ((found == 0)  & (strlen(t->check)>0))
         {
         n^=n;
  	 while (psred[n]!= NULL)  
  	 {
            switch(n)
            {
  		case 2: htmlprintf(sock,0,psred[n++],graphics_image(t->check), process_image(t->check,1));  
                 /* Display Watchdog status image and process statistics generator */
		break;
  		case 14: 
                    htmlprintf(sock,0,psred[n++],t->check);
		break;
  		default: htmlprintf(sock,0,psred[n++],"-");
            }
         }
         }
	 t=t->next;
}
}

void genprocreport(int sock)
{
caddr_t processes;

int n,i;
n=0;
resetnextproc();
for (i = 0 ; i < system_info.P_ACTIVE; i++)
{
  struct top_proc *p;
  
  /* 
  htmlprintf(sock,0,"<tr><td>%s.</td></tr>",format_next_process(processes, username)); 
  continue;
  */


  p = getnextproc();
  if (p==NULL) continue;

  n^=n;

  /* if (p != NULL) */
  while (pslist[n]!= NULL)  
  {
   switch (n)
   {
  	case 0: htmlprintf(sock,0,pslist[n++],"-");
		break;
/*
  	case 1: htmlprintf(sock,0,pslist[n++],graphics_image(p->name));  Display a graphics image and link 
		break;
*/
  	case 2: htmlprintf(sock,0,pslist[n++],graphics_image(p->name), process_image(p->name,0));  
                 /* Display Watchdog status image and process statistics generator */
		break;
  	case 3: htmlprintf(sock,0,pslist[n++],p->pid);
		break;
  	case 4: htmlprintf(sock,0,pslist[n++],username(p->uid)); 
		break;
  	case 5: htmlprintf(sock,0,pslist[n++],p->pri);
		break;
  	case 6: htmlprintf(sock,0,pslist[n++],p->nice);
		break;
  	case 7: htmlprintf(sock,0,pslist[n++],format_k(p->size));
		break;
  	case 8: htmlprintf(sock,0,pslist[n++],format_k(p->rss));
		break;
  	case 9: htmlprintf(sock,0,pslist[n++],"-");
		break;
  	case 10: htmlprintf(sock,0,pslist[n++],getStateAbbrev(p->state));
		break;
  	case 11: htmlprintf(sock,0,pslist[n++],p->wcpu * 100.0);
		break;
  	case 12: htmlprintf(sock,0,pslist[n++],p->pcpu * 100.0);
		break;
  	case 13: htmlprintf(sock,0,pslist[n++],format_time(p->time / HZ ));
		break;
  	case 14: 
                    htmlprintf(sock,0,pslist[n++],p->name);
		break;
  	default: htmlprintf(sock,0,"%s\n",pslist[n++]);
        
   } // end of switch case construct 
  } // end of while for each column of a line 
 } /* end of for interation to display all processes*/
}

int rn=0;

void psreport(int sock)
{
int n;

/* Remove this after scheduler is active! */

/* if (rn++==0) */
proc_refresh(); 
/*
else
resetnextproc();
*/

proc_thread_start(); /* Tell the /proc reader that he have to wait for us until we completely created the report! */


n^=n;

HTMLPageHeader (sock,"Watchdog: Process Status Report","watchdog?PROCESSES","icons/watchdog_bg.png",15);
HeadLine(sock,"Process monitor (comparable with top) Under construction!");


while ( psinfo[n]!=NULL)
{
  switch(n)
  {
    case 3:
                htmlprintf(sock,0,psinfo[n++],version());
                break;
     case 6:
  		htmlprintf(sock,0,psinfo[n++],green);
                break;
     case 7:
  		htmlprintf(sock,0,psinfo[n++],yellow);
                break;
     case 8:
  		htmlprintf(sock,0,psinfo[n++],red);
                break;
     case 9:
  		htmlprintf(sock,0,psinfo[n++],magenta);
                break;

     default:
  		htmlprintf(sock,0,psinfo[n++]);
  }
}

n^=n;
while ( psbeg[n]!=NULL )
{
  switch(n)
  {
    case 4:
  		htmlprintf(sock,0,psbeg[n++],cpGetCurrentDate(), cpGetCurrentTime());
                break;
    case 5:
  		htmlprintf(sock,0,psbeg[n++],uptime());
                break;
    case 6:
  		htmlprintf(sock,0,psbeg[n++],count_active_users(utmpfile));
                break;
    case 8:
  		/* htmlprintf(sock,0,psbeg[n++],getProcLine("/proc/%s","loadavg")); */
  		htmlprintf(sock,0,psbeg[n++], system_info.load_avg[0], system_info.load_avg[1], system_info.load_avg[2]);
                break;
    case 14:
  		htmlprintf(sock,0,psbeg[n++], system_info.p_total);
                break;
    case 15:
  		htmlprintf(sock,0,psbeg[n++], *(system_info.procstates+1));
                break;
    case 16:
  		htmlprintf(sock,0,psbeg[n++], *(system_info.procstates+2));
                break;
    case 17:
  		htmlprintf(sock,0,psbeg[n++], *(system_info.procstates+5));
                break;
    case 18:
  		htmlprintf(sock,0,psbeg[n++], *(system_info.procstates+4));
                break;
    
    case 23:
  		htmlprintf(sock,0,psbeg[n++], (double)*(system_info.cpustates) / (double)10.0 );
                break;
    case 24:
  		htmlprintf(sock,0,psbeg[n++], (double)*(system_info.cpustates+1) /(double)10.0 );
                break;
    case 25:
  		htmlprintf(sock,0,psbeg[n++], (double)*(system_info.cpustates+2) / (double)10.0 );
                break;
    case 26:
  		htmlprintf(sock,0,psbeg[n++], (double)*(system_info.cpustates+3) / (double)10.0);
                break;
    case 27:
  		htmlprintf(sock,0,psbeg[n++], (double)*(system_info.cpustates+4) / (double)10.0 );
                break;
    case 28:
  		htmlprintf(sock,0,psbeg[n++], (double)*(system_info.cpustates+5) / (double)10.0 );
                break;
    /*
    case 29:
  		htmlprintf(sock,0,psbeg[n++], (float)*(system_info.cpustates+6) / (float)10.0 );
                break;
    */

    case 23+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.m_total); */
  		htmlprintf(sock,0,psbeg[n++],*(system_info.memory) + * (system_info.memory+1));
                break;
    case 24+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.m_used); */
  		htmlprintf(sock,0,psbeg[n++],*system_info.memory);
                break;
    case 25+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.m_free); */
  		htmlprintf(sock,0,psbeg[n++],*(system_info.memory+1));
                break;
    case 26+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.m_buffers); */
  		htmlprintf(sock,0,psbeg[n++],*(system_info.memory+3));
                break;
    case 32+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.s_total); */
  		htmlprintf(sock,0,psbeg[n++], *(system_info.swap) + *(system_info.swap +1));
                break;
    case 33+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.s_used); */
  		htmlprintf(sock,0,psbeg[n++], *(system_info.swap));
                break;
    case 34+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.s_free); */
  		htmlprintf(sock,0,psbeg[n++],*(system_info.swap+1));
                break;
    case 35+9+2:
  		/* htmlprintf(sock,0,psbeg[n++],mem.s_cached); */
  		htmlprintf(sock,0,psbeg[n++],*(system_info.memory+4));
                break;
    default:
  		htmlprintf(sock,0,"%s\n",psbeg[n++]);
  }
}

 /* 
  * 
  * This searches for processes which should run but do not run ! 
  * This processes will have the red flag!
  *
  */

 list_failed_processes(sock);

 /* 
  * This generates a list of the running processes with a yellow sign
  * and a list of running processes which are configured in the ini
  * file and correct running using a green sign.
  *
  */
 genprocreport(sock); 

/* dir_do("/proc", ppl, sock); */


n^=n;
while ( psend[n]!=NULL )
{
  htmlprintf(sock,0,"%s\n",psend[n++]);
}


HTMLPageFooter(sock);

/* 
 * HTML page with process and system information generated. Asynchonous refresh of process information
 * is now allowed!
 *
 */

proc_thread_end();
 
}
