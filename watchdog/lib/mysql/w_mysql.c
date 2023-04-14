
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


#include <stdio.h>
#include <unistd.h>

#include <logwriter.h>
#include <mysql.h>
#include <inimain.h>
#include <watchlist.h>
#include <options.h>

#include <cntusr.h>
#include <machine.h>

/* #include <httpserver.h>  for */

#include <username.h>
#include <string.h>


/*
 * Prototypes
 */

void check_db_error(); 
MYSQL  *my=NULL;

int openDB()
{
   /* Initialize database handle */

   /* Turn off MySQL statistics if port is set to -1 in ini file ! */
   if ( (globalArgs.runflags.run_mysql_stats==0) | (inimain.mysql_porti == -1) ) return 0 ;

   my = mysql_init(NULL);
   if(my == NULL)
      {
         syslogprintf(__FILE__,__LINE__,"watchdog",1,"No MySQL database object");
         globalArgs.runflags.run_mysql_stats=0;
         return 0;
      }

   if( mysql_real_connect (
        my,   /* Pointer to MySQL*/
        inimain.mysql_host, /* Host-Name*/
        inimain.mysql_user, /* User-Name*/
        inimain.mysql_pass, /* Password */
        inimain.mysql_db,   /* DB name*/
        inimain.mysql_porti,/* Port (default=0) */
        NULL, /* "/var/run/mysqld/mysqld.sock",  Socket (default=NULL)*/
        0      /* No flags */  )  == NULL)
      {
               check_db_error();
         return 0;
      } 
   else
      {
#ifndef NDEBUG
      syslogprintf(__FILE__,__LINE__,"watchdog",5,"Succesfull connected to MySQL database");
#endif
      }

return 1;
}
void closeDB(void)
{

   /* Turn off MySQL statistics if port is set to -1 in ini file ! */
   if ( inimain.mysql_porti == -1 ) return ;
   if (my != NULL )  /* If connection exists than close database connection ! */
   {
         mysql_close (my);
         my=NULL;
   }
}


/*
 * Checks for database error and writes them to syslog
 * If one error occurs than writing to database is stopped
 * until SIGUSR2 is sended to software!
 *
 *
 */

void check_db_error() 
{
   if (mysql_errno(my) != 0) 
      {
         globalArgs.runflags.run_mysql_stats=0;
         syslogprintf(__FILE__,__LINE__,"watchdog",1, "MySQL errno: %u error: (%s)", mysql_errno(my), mysql_error(my));
         
      }
}

int isMonitored(struct top_proc *p)
/*
 * Checks that a given process is monitored
 * If not returns 0
 * If so it returns 1
 */

{
 /* Process is monitored and configured to have statistics in the database ! */
 /* if ((search_in_ini_proc(p->name) == 1) & (strncmp("yes",search_in_ini_proc_struct(p->name)->stat,3) ==0)) return 1; */
 struct check_processes *ps;
  ps= search_in_ini_proc_struct(p->name);
 
 if (ps == NULL) return 0;
 if (strncmp("yes",ps->stat,3) ==0) return 1; 


return 0;  
}

void write_process_stats_2_db()
{
  char query[1024];
  int i;
   /* Turn off MySQL statistics if port is set to -1 in ini file ! */
   if ( (globalArgs.runflags.run_mysql_stats==0) | (inimain.mysql_porti == -1) | (inimain.stat_i==0) ) return ;

  resetnextproc();

  for ( i = 0 ; i< system_info.P_ACTIVE; i++)
  {
    struct top_proc *p;
    p = getnextproc();
    if (p==NULL) continue;

   switch(inimain.stat_i)
   {
      case 1:  /* Write all process data to MySQL database 
                * Just go on..
                *
                */
               break;
      case 2:  if (isMonitored(p)==0) continue;  /* Not monitored but have to be monitored! */
               break;
      case 3:  if (isMonitored(p)==1) continue;  /* Monitored but have to be not monitored! */
               break;
   }
  sprintf(query, "insert into process_stats (system_id, timestamp,pid, user, pr, ni, virt, res, shr, state, p_cpu,p_mem,time,command) values (\"%s\",sysdate(),%d,\"%s\",%d,%d,%ld,%ld,%d,%d,%f,%f,%ld,\"%s\" )",
   globalArgs.hostname,
   p->pid,
   username(p->uid),
   p->pri,
   p->nice,
   p->size,
   p->rss,
   0,
   p->state,
   p->wcpu,
   p->pcpu,
   p->time,
   p->name
  );

   
     mysql_query(my, query);
     check_db_error();
     if (globalArgs.runflags.run_mysql_stats==0) break; /* If there is an error than goodbye */
 }
}


void write_global_stats_2_db()
{
  
  char  query[1024];

   /* Turn off MySQL statistics if port is set to -1 in ini file ! */
   if ( (globalArgs.runflags.run_mysql_stats==0) | (inimain.mysql_porti == -1) | (inimain.stat_i == 0) ) return ;

 sprintf(query,
"insert into global_stats values (\"%s\",sysdate(),%d,%ld,%ld,%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)" ,
   globalArgs.hostname,                              /* system_id */
    count_active_users(utmpfile),         /* users */
   (long)(system_info.load_avg[0]*100.0), /* load_1 */
   (long)(system_info.load_avg[1]*100.0), /* load_2 */
   (long)(system_info.load_avg[2]*100.0), /* load_3 */
   system_info.p_total,                   /* t_total */
   *(system_info.procstates+1),           /* t_running */
   *(system_info.procstates+2),           /* t_sleeping */
   *(system_info.procstates+5),           /* t_stopped */
   *(system_info.procstates+4),           /* t_zombie */
   *(system_info.cpustates),              /* c_usr   */
   *(system_info.cpustates+1),            /* c_sys */
   *(system_info.cpustates+2),            /* c_ni */
   *(system_info.cpustates+3),            /* c_idle */
   *(system_info.cpustates+4),            /* c_wait */
   *(system_info.cpustates+5),            /* c_hi */
   0,   /* Placeholder for %SI */         /* c_si */
   *(system_info.memory) + * (system_info.memory+1), /* m_total */
   *(system_info.memory),                 /* m_used */
   *(system_info.memory+1),               /* m_free */
   *(system_info.memory+3),               /* m_buffers */
   *(system_info.swap) + *(system_info.swap +1), /* s_total */
   *(system_info.swap),                   /* s_used */
   *(system_info.swap+1),                 /* s_free */
   *(system_info.memory+4)                /* s_cached */
   );


   mysql_query(my, query);
  
   check_db_error();

}



