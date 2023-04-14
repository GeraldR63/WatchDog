
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

#include <run_watchdog.h>
#include <processes.h>
#include <watchlist.h>
#include <syslog.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util.h>
#include <pthread.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <logwriter.h>
#include <fcntl.h>
#include <futil.h>
#include <datum.h>
#include <errorhandler.h>

/*
 * Function prototypes
 * Thread onErrorHandler
 *
 */

void *onErrorHandler( void *ptr );
int runExternalModule(char*);
void sendmail(void * ptr);
void macro_replace(char* msg, void *ptr, int addEcho);
int find_process(struct running_processes *rp, char *process);




struct check_processes * iIsRunning(struct running_processes *rp, struct check_processes *cp)
{
   struct threads  l[256];
   struct check_processes * retval=NULL;
   struct check_processes * current=cp;
   int i=0;

   while (current != NULL )
   /*
    * Check that there is one running process for each process in watchdog.ini
    *
    */
   {
     /* Last element of double linked list contains NULL pointers */
     if ((current->check != NULL) & (isText(current->check)==1) ) 
     {
     /*
      * External Module Handler
      * If an external module is defined than run this check as an additional test
      * if the module is not avail or accessible the function 
      * run_external return 1 as success  and only the find_process check will be executed
      * In this case an error is logged in the syslog!
      *
      */

     if ((find_process(rp, current->check)!=1) | (runExternalModule(current->external)!=1))
     /*
      * No process found...
      *
      */
     {
       /*
        * Start a thread which tries to restart process
        *
        */

        int  iret1;
        if (strcmpi(current->debug,"yes")==0)
        	syslogprintf(__FILE__,__LINE__,"watchdog",1,"Process %s not found.",current->check);


        /* Create independent threads to restart not running programs */
        l[i].l=(pthread_t *) malloc (sizeof(pthread_t));
        iret1 = pthread_create( l[i++].l, NULL, onErrorHandler, (void*) current);
        /* pthread_join( *thread, NULL); */

        /* onErrorHandler(current); */
     }
     else
     /*
      * Process found try external check..
      *
      */
     {
        if (strcmpi(current->syslog,"yes")==0)
        {
        if (strcmpi(current->syslog,"yes")==0) 
            syslogprintf(__FILE__,__LINE__,"watchdog",2,"Process %s found.",current->check);
        } 
     }
     }
     current=current->next;  
   }

/*
 *  Wait for threads trying to restart faulty processes
 * 
 */
        {
        int c=0;
        for (;c<i;c++)
                pthread_join(*l[c].l, NULL);
        }

/*
 *  Continue after all threads succesfull endet
 *
 */


return  retval;
}

/*
 * Function onErrorHandler
 *
 * Run restart of dead process  n-times in an intervall
 *
 * If not successfull than it executes the onerror command
 * Attention: This is a thread!
 */


void *onErrorHandler( void *ptr )
{
     struct check_processes *current;
     char buf[1024];

     int delay=60;    /*Default delay */
     int retries=5;   /*Default retries */
     current = (struct check_processes *) ptr;

     if(current->d!=0) delay=current->d;
     if(current->r!=0) retries=current->r;

     for(;;)
     {
        /* Logging to syslog only if allowed */
        if (strcmpi(current->debug,"yes")==0) 
        {
     		syslogprintf(	__FILE__,
				__LINE__,
                    		"watchdog",2,"Tried %d times to restart %s for max. %d times with %d seconds delay\n", 
		current->r-retries+1,
		current->check,
		current->r,
		current->d);
        }

        /* Redirect any scriptoutput to /dev/nul */        
        sprintf(buf,"%s >/dev/null",current->restart);

        if (system(buf)==0)
        {
          if (strcmpi(current->debug,"yes")==0) 
          {
     	     syslogprintf(	__FILE__,
				__LINE__,
				"watchdog",2,"%s succesfull restarted with %s\n", current->check,current->restart);
          }
          /* If succcesfull restarted than exit this endless loop */
          break;
        }
        else
        {
          if (strcmpi(current->debug,"yes")==0) 
          {
     	     syslogprintf(	__FILE__,
				__LINE__,
				"watchdog",2,"%s not succesfull restarted using %s for %d times.\n", 
		current->check,
		current->restart,
		current->r-retries+1);
          }
        }

        sleep(delay);
        if (retries--<=1) 
        {
          /* OnExit Handler 
 	   * Watchdog could not restart the process now we try to execute the 
 	   * onerror command
 	   * */
        char *msg="";
        /* Redirect any scriptoutput to /dev/nul */        
        sprintf(buf,"%s >/dev/null",current->onerror);

        if (system(buf)!=0)
        {
          msg="not ";
          sendmail(current);
        }


          if (strcmpi(current->debug,"yes")==0) 
          {
     	        syslogprintf(	__FILE__,
				__LINE__,
				"watchdog",2,"OnError %s %ssuccesfull executed .\n", 
		current->onerror, msg);
          }

        break;
        }
        
     }

  return NULL;
}

/*
 * Function 	runExternalModule
 * Parameter	module to execute
 *
 * Return 	1 on success and if module undefined
 * 		0 no success
 *
 * If module is defined but function will not find it or not be able to
 * execute it than this function stop watchdog with an assert.
 *
 */

int runExternalModule(char *mod)
{
   int retval=1;
   char buf[1024];
   sprintf(buf,"%s >/dev/null",mod);

   /* Redirect any scriptoutput to /dev/nul */        
   sprintf(buf,"%s >/dev/null",mod);

   /*
    * Check that module exists.
    * If not log this to syslog 
    *
    */

   if (mod != NULL ) /* There is a module defined */
   {
   /*
    * Check that we can execute the defined module.
    * If not log this and die.
    * 
    */

   if (strlen(mod)!=0) /* if no module defined than access check is unnecessary */
   {
   if (access (mod, R_OK | W_OK | X_OK) != 0) /* In case of an error */
   {
          char *msg="Fatal: Module %s not found, readable, writeable or executable! .\n";
          /* Log this error */ 
          syslogprintf(	__FILE__,
			__LINE__,
			"watchdog",1,msg, mod);

          /* Module not found and stop... */
          fprintf(stderr,"Assertion:\nFile:%s\nLine:%d\nFatal: Module %s not found, readable,writeable or executeable!\n",__FILE__,__LINE__,mod);
          wdabort(NULL);
   }
   }
   else
   {
       /* 
        * If no external module defined than return success! 
        */
       return 1;
   }


   if ((system(buf))!=0)
   {
          /* Log error*/
          syslogprintf(	__FILE__,
			__LINE__,
			"watchdog",1,"Module %s not succesfull executed .\n", mod);
          retval=0;
   }
   else
   {
          /* Log warning */
          syslogprintf(	__FILE__,
			__LINE__,
			"watchdog",2,"Module %s succesfull executed .\n", mod);
   }
   }
  

   return retval;
}

/*
 * Function find_process
 * Parameter 	root pointer to double linked running process list
 * 		string from watchdog.ini to identifiy a process
 * Return	1 if found
 * 		0 if not found
 *
 * 		If there is not enough memory this function and called 
 * 		subfunctions will stop watchdog with an assert.
 *
 *
 */

int find_process(struct running_processes *rp, char *process)
{
   struct running_processes * current=rp;

   while (current != NULL )
   {
     if (current->cmdline != NULL )
     if (search(current->cmdline,process)==0) /* Process found.  */
       return  1;
     current=current->next;
   }

return 0;
}

/*
 * Function 		sendmail
 * Parameters		pointer to a check_processes structure containing required informations
 * 			to send message succesfull
 *
 */

void sendmail(void * ptr)
{
 struct check_processes * current=ptr;
 int len=0;

 /* 
  * 
  * Allocate enough memory to read the file into memory and have enough room
  * to replace the makros with the real text.
  * This algorithm seems to waste some space but only for a few microseconds.
  *
  */

 char *msg=(char *) malloc(len=(filelength(current->message)*2)+strlen(current->owner)+2+strlen(current->service)+2+3);
 
 if (msg != NULL )
 {
   /*
    * File to memory..
    *
    */

   FILE * handle=fopen(current->message, "r" );
   if  (handle != NULL )
   {
     fread(msg,len,1,handle);
     fclose(handle);

     
     /*
      * Replace macros in message template
      */
     macro_replace(msg,current,1);
     
     /*
      * Replace macros in service template
      */

     macro_replace(current->service,current,0); 

     strcat(msg," | ");  /* Add pipe symbol...* */
     strcat(msg,current->service);


   if ((system(msg))!=0)
   {
          /* Log error*/
          syslogprintf(	__FILE__,
			__LINE__,
			"watchdog",1,"Message %s not succesfull send .\n", current->service);
   }
   else
   {
          /* Log warning */
          syslogprintf(	__FILE__,
			__LINE__,
			"watchdog",2,"Message %s succesfull send .\n", current->service);
   }

   }
   else
   {
      /* This is a fatal problem */
      syslogprintf(__FILE__,__LINE__,"watchdog",1,"File %s not found!\n",current->message);
      fprintf(stderr,"Assertion:\nFile:%s\nLine:%d\nFatal: File %s not found!\n",__FILE__,__LINE__,current->message);
      wdabort(NULL);
   }
   free(msg);
 }
 else
 {
  /* This is a fatal problem */
  syslogprintf(__FILE__,__LINE__,"watchdog",1,"Out of memory error");
  fprintf(stderr,"Assertion:\nFile:%s\nLine:%d\nFatal: Out of memory error\n",__FILE__,__LINE__);
  wdabort(NULL);
 }
} 

/*
 * Function 		macro_replace
 * Parameters		watchdog message to send
 * 			pointer to a check_processes structure containing required informations
 * 			to replace in the message text
 *
 * ====================Example message.txt file===================
 *
 * 			Hello $owner,
 *
 * 			watchdog tried on $date until $time without success $retries times to restart  process $check
 * 			at  $host.
 *
 * 			You should investigate and find the reasons for this.
 *
 * 			Please change restart script in a way to enable watchdog to be succesful.
 *
 *
 * 			kind regards
 * 			Watchdog
 *
 * ====================Example message.txt file===================
 *
 * Supported macros:  	$owner		- owner of the server as defined in watchdog.ini
 * 			$date		- date of event
 * 			$time		- time of event
 * 			$retries	- number of retries watchdog tried to restart process
 * 			$check		- check string (process name)
 * 			$host		- hostname where the mail was generated
 *
 */

void macro_replace(char* msg, void *ptr, int addEcho)
{
 struct check_processes * current=ptr;
 int len;
 char *tmp=(char *) malloc(len=(filelength(current->message)*2));

#define TOKENS 6
 char *tokens[]={"owner",  /* List of tokens this function is able to deal with */
                 "date",   /* cpGetCurrentDate */
                 "time",   /* cpGetCurrentTime */
                 "retries",
                 "check",
                 "host"    /* gethostname(char*,int size) */
                };

 if (tmp != NULL)
 {
   char *curtok;   /* current token */
   char hostname[128];
   gethostname(hostname,128);
   memset(tmp,0,len);
   /*
    * ToDo: parser for makro replacement
    */

   syslogprintf(__FILE__,__LINE__,"watchdog",5,"check:%s owner:%s Host:%s Date:%s Time:%s",
	current->check,current->owner,hostname,cpGetCurrentDate(), cpGetCurrentTime());
   

   if ((curtok=strtok(msg,"$")) != NULL)
   {
   /* If $ sign found than it is possible a token */
   /* do */

   if (addEcho==1)
   strcat(tmp,"echo \"");
   strcat(tmp,curtok);
   while ((curtok=strtok(NULL,"$")) != NULL )
   {
   char *t=curtok;
   int i;
   for (i=0;i<TOKENS;i++)
   {
               /*"owner",    List of tokens this function is able to deal with 
                * "date",    cpGetCurrentDate 
                * "time",    cpGetCurrentTime 
                * "retries",
                * "check",
                * "host"     gethostname(char*,int size) 
                */
      if (strncmp(curtok,tokens[i],strlen(tokens[i]))==0)
      {
                t=curtok+strlen(tokens[i]);  
      switch (i)
      {
       case  0:     /* Owner */
                strcat(tmp, current->owner);
		break;
       case  1:	/* Date */
                strcat(tmp,(char *) cpGetCurrentDate());
		break;
       case  2:     /* Time */
                strcat(tmp,(char *) cpGetCurrentTime());
		break;
       case 3:     /* Retries */
                strcat(tmp,current->retries);
		break;
       case 4:	/* Check */
                strcat(tmp,current->check);
		break;
       case 5:     /* Host */
                strcat(tmp,hostname);
		break;
         default:   /* No token, musst be $ sign */
                strcat(tmp,"$");
      }
      }
   }      
   strcat(tmp,t);
   } 
   }

   if (addEcho==1)
   strcat(tmp,"\"");
   /* Replace template with real message text */
   strcpy(msg,tmp);

   free(tmp); 
 }
 else
 {
  /* This is a fatal problem */
  syslogprintf(__FILE__,__LINE__,"watchdog",1,"Out of memory error");
  fprintf(stderr,"Assertion:\nFile:%s\nLine:%d\nFatal: Out of memory error\n",__FILE__,__LINE__);
  wdabort(NULL);
 }

}


