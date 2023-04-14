
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

#ifndef __DEBUG_H
#define __DEBUG_H

#define CHECKENV(VAR)							       \
{									       \
    if (!getenv(VAR))							       \
    {									       \
	fprintf (stderr,"Environment: Variable %s ist nicht definiert!\n", VAR);       \
	abort();							       \
	/*NOTREACHED*/							       \
    }									       \
}


#define RNASSERT(VAR)							       \
{									       \
    if (!VAR)								       \
    {									       \
	fprintf (stderr,"ASSERTION\nFile : %s Line : %d\n", __FILE__, __LINE__);       \
	abort();							       \
	/*NOTREACHED*/							       \
    }									       \
}


#define CHECKSTRBUF(DEST,SRC)						       \
{									       \
    if (strlen(SRC) > (size_t)sizeof(DEST))				       \
    {									       \
	printf ("Puffer ist zu klein!\nFile : %s Line : %d\n",		       \
						__FILE__, __LINE__);	       \
	exit(120);							       \
	/*NOTREACHED*/							       \
    }									       \
}


#define DEL(VAR)							       \
{									       \
    if (VAR)								       \
    {									       \
	free(VAR);							       \
	VAR=0;								       \
    }									       \
}


#define CHARMALLOC(VAR , SIZE )						       \
{									       \
    VAR = (char*)malloc (sizeof(char)*SIZE);				       \
    assert (VAR != 0);							       \
}

#define NOPARAM()\
if (argc==1)\
{\
  printf("Programm erwartet mindestens einen Parameter!\n");\
  printf("Merschtenteils is das ein Filename!\n");\
  assert(argc!=1);\
  exit(-2);\
}

/*
  Das Makro MSG dient dem einfachen Debugging!
  In der Testversion generiert das Makro eine 
  Meldung auf stderr! Im Release kann es die
  Meldungen nach dev/null oder in eine Protokolldatei
  umleiten, oder gar nichts tun! Das Feature Release
  sollte dann noch entsprechend programmiert werden!
  Im Augenblick tut die Releasevariante nix!

  Fehlerklassen 1-5
  5 unwichtige Meldungen
  1 wichtige Meldungen
  0 keine Meldungen (die Null darf NICHT als Parameter uebergeben werden!)
 
  Ueber die Environmentvariable ERRCLASS koennen die 
  Fehlermeldungen entsprechend ihrer Fehlerklassen
  unterdrueckt werden! Ist ERRCLASS=0 wird keine
  Meldung ausgegeben!
*/

#ifndef NDEBUG 

/* NDEBUG wird auch von ASSERT verwendet um ein Release zu bilden! */

#define MSG(TEXTDATA,ERRCLASS)\
{\
  char * cpn=getenv("DBG_ERRCLASS")==0?"5":getenv("DBG_ERRCLASS");\
  if (atoi(cpn) >=ERRCLASS) {\
fprintf(stderr,"# DBGMSG %s SRC=%s LINE=%d MSG= %s\n",cpGetCurrentDate(), __FILE__,__LINE__,TEXTDATA);}\
}

#define EMSG(TEXTDATA,STRPAR,ERRCLASS)\
{\
  char * cpn=getenv("DBG_ERRCLASS")==0?"5":getenv("DBG_ERRCLASS");\
  if (atoi(cpn) >=ERRCLASS) {\
fprintf(stderr,"# DBGMSG %s SRC=%s LINE=%d MSG= %s %s\n",cpGetCurrentDate(), __FILE__,__LINE__,TEXTDATA, STRPAR);}\
}


#else
/* Im Release werden keine Ausgaben auf dem STDERR protokolliert! */
#define MSG(PAR1, PAR2) ((void)0)
#define EMSG(PAR1, PAR2, PAR3) ((void)0)
#endif

/*
 Das Makros gpsystem erweitert die Funktion system
 um eine Fehlerbehandlung! Wenn man diese Ferhlerbehandlung 
 NICHT mehr wuenscht und den Originalzustand wiederherstellen
 moechte, dann kann man ganz einfach hinter der 
 Definition des Makros gpsystem folgendes define aktivieren:
 define gpsystem system
 und dann ist wieder alles beim Alten!
*/

#define gpsystem(PGMCMD)\
{\
  char op[500];\
  if(system(PGMCMD)<0)\
  {\
     perror("Fehler bei system!");  \
     sprintf(op,"Fehler bei Start des Kindprozess %s",(char*)PGMCMD); \
     MSG(op,1);\
  }\
  else\
  {\
    sprintf(op,"Start des Kindprozess %s",(char*) PGMCMD); \
    MSG(op,4); \
  } \
}

/*
  Makro ARGPRINT dient der einfachen
  Ausgabe der Kommandozeile eines Programm.
  Es wird erwartet, das der ARGCOUNTER mit
  argc und die Argumentenliste mit argv
  bezeichnet wurden.
  int main(int argc, char *argv[]);

  Der Code des Makro wird ueber ein #define NDEBUG
  entfernt. Dies ist Standard wie ueber ASSERT
  vorgegeben.
*/

#ifndef NDEBUG
#define ARGPRINT()\
{\
   FILE *out=fopen("/tmp/arglog.log","w");\
   int argctr=0;\
   for(;argctr<argc;argctr++)\
      fprintf(out!=0?out:stderr,"%s ",argv[argctr]);\
   fprintf(out!=0?out:stderr,"\n");\
}
#else
#define ARGPRINT() ((void)0)
#endif

/*
#define SYSLOG(MSG,VALUE)\
{\
openlog("watchdog", LOG_CONS | LOG_PID, LOG_LOCAL3);\
syslog (LOG_DEBUG, MSG , VALUE);\
closelog();\
}
*/

/*
 * SYSLOG Makro allows to create 5 levels of debugmessages
 * in /etc/var/syslog
 * configured via environment variable DBG_ERRCLASS.
 * DBG_ERRCLASS=0 no logging
 * DBG_ERRCLASS=1 log errors
 * DBG_ERRCLASS=2 log warnings
 * DBG_ERRCLASS=3 log verbose 
 * DBG_ERRCLASS=4 log very verbose
 * DBG_ERRCLASS=5 log anything and write it also to stderr
 *
 * If DBG_ERRCLASS is not set than the makro will write anything into syslog.
 *
 * If you use this makro than you have to include syslog.h
 */

#define SYSLOG(MSG,ERRCLASS)\
{\
  char * cpn=(char*)getenv("DBG_ERRCLASS");\
  if (cpn==NULL) cpn=(char *)"5";\
  if ( atoi(cpn) >=ERRCLASS) {\
    openlog("watchdog", LOG_CONS | LOG_PID, LOG_LOCAL3);\
    syslog (LOG_DEBUG, MSG , ERRCLASS);\
    closelog();\
  }\
  if (atoi(cpn) == 5) {\
    fprintf(stderr,"# DBGMSG %s SRC=%s LINE=%d MSG= %s\n",cpGetCurrentDate(), __FILE__,__LINE__,MSG);}\
}



#endif
