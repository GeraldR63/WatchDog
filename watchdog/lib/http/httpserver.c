/*
    Copyright (C) by Christian Gosch.
    Copyright (C) 2007 by Gerald Roehrbein

    Added some functions, removed some memory leaks and made it really work. Roehrbein.

    This file contains only a few lines written by Christian Gosch.
    I've rewritten most of the stuff Christian had written.

    But nevertheless he original started the w3server project.


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
 * \file w3server.h
 * \brief Main functions to set up a server.
 */

#include <httpserver.h>
#include <httptypes.h>
#include <httpcomm.h>
#include <httppost.h>


#include <logwriter.h>
#include <options.h>
#include <util.h>

#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sighandler.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <htmlgen.h>

/*
 * Reporting and internal Watchdog managemen
 *
 */
#include <ps.h>
#include <schedulerreport.h>
#include <http.h>
#include <status.h>
#include <ini.h>
#include <signals.h>
#include <statistic.h>


#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif

#define W3_HTTP_VERSION 1.0


/* File and directory settings. */
/* html document base directory */
/* This has to be an ABSOLUTE PATH !!! */

/* 
 * Binary base directory. Relative to HTDOC_BASE and has to start with a '/'.
 * All requested files in this directory are handled as executables and their output is sent 
 * on their behalf.
 */

/* Template filename for Unix mktemp() function */
char HTDOC_BIN_TEMPLATE[] = "/temp/temp.XXXXXX";
char HTDOC_DOTHTML[] = "";

/* Response strings */
char RESPONSE_OK[] = "HTTP/1.0 200 \n";
char RESPONSE_NOT_FOUND[] = "HTTP/1.0 404 \n";
char RESPONSE_SERVICE_UNAVAILABLE[] = "HTTP/1.0 503 \n";
char RESPONSE_BAD_REQUEST[] = "HTTP/1.0 400 \n";

/* Error codes */
#define W3_ERROR_NOT_FOUND 1
#define W3_ERROR_SERVICE_UNAVAILABLE 2
#define W3_ERROR_BAD_REQUEST 3

/* Commands */
#define W3_CMD_GET 1
#define W3_CMD_POST 2
#define W3_CMD_HEAD 3
#define W3_CMD_UNKNOWN 4

int reinit=0;

/*!
 * 
 */
w3ServerConnection* w3ServerAccept (w3Server *server) {
  w3ServerConnection *con=NULL;

  con = (w3ServerConnection*) malloc (sizeof (w3ServerConnection));
  if (con)
  {
#ifndef NDEBUG
  syslogprintf(__FILE__,__LINE__,"watchdog",5,"Listening");
#endif
  
  if ( listen (server->socket, 128) == -1 ) {
#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Server: can't do listen()\n");
#endif
    reinit=1;
    free(con);
    con=NULL;
  }
  else
  {
  con->server = server;
  if ( (con->socket = accept (server->socket, (struct sockaddr*)&con->sock_addr, &con->addrlen)) == -1 ) {
    free (con);
    con = NULL;
#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Listened");
#endif
  } else {

#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Connect");
#endif
  }
  }
  } /* if con=calloc */

  return con;
}


/*!
 * 
 */
boolean w3ServerInit (w3Server *server) {
  socklen_t namelen;

#ifndef NDEBUG
  syslogprintf(__FILE__,__LINE__,"watchdog",5,"Initializing Watchdog's HTTP server");
#endif

  server->hostname = (char*)malloc (sizeof (char) * MAXHOSTNAMELEN);
  if ( gethostname (server->hostname, MAXHOSTNAMELEN) != 0 ) {
    syslogprintf(__FILE__,__LINE__,"watchdog",1,"Server: Could not get hostname");
    abort();
  }

#ifndef NDEBUG
  syslogprintf(__FILE__,__LINE__,"watchdog",5,"Hello, I am running at %s", server->hostname);
  syslogprintf(__FILE__,__LINE__,"watchdog",5,"Starting at port %d",server->port);
#endif
  
  server->host = gethostbyname (server->hostname);
  /* convert host name to IP address and copy to sock_addr */
  bcopy( ((struct hostent*)server->host)->h_addr, &server->sock_addr.sin_addr, sizeof (server->host->h_addr));
  /* initialise protocol prot number */
  server->sock_addr.sin_port = htons(server->port);
  server->sock_addr.sin_family = AF_INET;
  /* create a socket */
  server->socket = socket (AF_INET, SOCK_STREAM, 0);
  /* connect the socket to the server */
  namelen = sizeof (server->sock_addr);

  {
  /*
   * Try to bind 100 times if not possible than
   * produce some other errors until
   * arriving here
   *
   */

  int n;
  n=0;
  while ( bind(server->socket, (struct sockaddr*)&server->sock_addr, namelen) != 0 ) {
    syslogprintf(__FILE__,__LINE__,"watchdog",1,"HTTP Server: can't bind socket !");
    if (n++ >100) break;
    sleep(10);
    /* abort(); */
  }
  }
  reinit=0;
  return true;
}


boolean w3ServerClose (w3Server *server) {
  syslogprintf(__FILE__,__LINE__,"watchdog",2,"Shutting down server");
  close (server->socket);
  return true;
}

boolean w3ServerDisconnect (w3ServerConnection* con) {

#ifndef NDEBUG
  syslogprintf(__FILE__,__LINE__,"watchdog",3,"Disconnecting");
#endif

  close (con->socket);
  free (con);
  return true;
}

char* w3ServerGetToken (char *str, int *idx) {
  char *token = (char*)calloc (256, sizeof(char));
  int i = 0;
  int tokenSize = 256;
  char c = 'a';
  while ( (str[i] != ' ') && (str[i] != 0) && (str[i] != 13)) {
    c = str[i];
    token[i++] = c;
    if (i >= tokenSize) {
      tokenSize += 256;
      token = (char*)realloc ((void*)token, (tokenSize) * sizeof(char));
    }
  }
  token[i] = 0;

#ifndef NDEBUG
  syslogprintf(__FILE__,__LINE__,"watchdog",5,"Got token -%s-\n",token);
#endif

  *idx = i + 1;
  return token;
}

char* w3ServerGetPath (char *str) {
  enum {
    PATH_DONE,
    PATH_GO,
    GET_PATH,
    PATH_ERROR
  };
  char *path = NULL;
  char *token = NULL;
  char *command = NULL;
  int i = 0;
  int state = PATH_GO;
  while (state != PATH_DONE) {
    switch (state) {
    case PATH_GO:
      token = w3ServerGetToken (str, &i);
      if (strcmp (token, "GET") == 0) {
	state = GET_PATH;
	command = StringDup (token);

      } else {
	state = PATH_ERROR;
      }

      if (token) { free(token); token=NULL; }  /* Avoid memory leak. Roehrbein */

      break;
    case GET_PATH:
      path = w3ServerGetToken (&str[i], &i);
      state = PATH_DONE;
      break;
    case PATH_DONE:
      break;
    case PATH_ERROR:
#ifndef NDEBUG
      syslogprintf(__FILE__,__LINE__,"watchdog",5,"Error interpreting command ",command);
#endif
      state = PATH_DONE;
      break;
    }
  }
  return path;
}

char* w3ServerReceiveRequest (w3ServerConnection *con) {
  size_t s = 0;
  char* data;
  unsigned int size = 0;
  
  data = (char*)calloc (1024,sizeof(char));
  
  do {
    s = read ( con->socket, (void*)(&((char*)data)[size]), 1024 );
    if ( s == -1 ) {
#ifndef NDEBUG
      syslogprintf(__FILE__,__LINE__,"watchdog",5,"ERROR receiving data");
#endif
      return NULL;
    }

    size += (unsigned int)s;

#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Received data %s\n",&data[size - s]);
#endif

    data = (char*)realloc ( (void*)data, (size_t)((1024 + size) * sizeof(char)) );

  } while ( (s == 1024 ) && (data[size - 1] != '\n') ); 
   /* 
    * Change by Roehrbein
    * s>0 changed to s==1024. Read until number of bytes reads is not equal to number of bytes have to read!
    *
    */

#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"No more data to receive");
#endif
  return data;
}

int w3ServerIdentifyCommand (char *cmd) {
  if (strcmp (cmd,"GET") == 0) {
    return W3_CMD_GET;
  }
  if (strcmp (cmd,"POST") == 0) {
    return W3_CMD_POST;
  }
  if (strcmp (cmd,"HEAD") == 0) {
    return W3_CMD_HEAD;
  }
  return W3_CMD_UNKNOWN;
}

boolean watchdog_internal(char * path, w3ServerConnection *con)
{

  boolean retval = false;

    if (strncmp(path,"/watchdog",9)==0)
    /* 
     * Check for internal command
     * If it is an internal command than exec internal command if not
     * show file (if exist)
     *
     */
    {
#ifndef NDEBUG
        syslogprintf(__FILE__,__LINE__,"watchdog",5,"HTTP: Exec Watchdog internal command %s",path);
#endif
      
       if (strstr(path,"PROCESSES")!=NULL)
       {
           psreport(con->socket);
           retval= true;
       }
       else if (strstr(path,"INI")!=NULL)
       {
           inireport(con->socket);
           retval=true;
       }
       else if (strstr(path,"UPDATE")!=NULL)
       {
           DisplayErrorPage(con->socket,"POST and INIUPDATE under construction");
           retval=true;
       }
       else if (strstr(path,"SCHEDULER")!=NULL)
       {
          schedulerreport(con->socket);
          retval=true;
       }
       else if (strstr(path,"SIGNAL")!=NULL)
       {
          signalreport(con->socket);
          retval=true;
       }
       else if (strstr(path,"HTTP")!=NULL)
       {
          httpreport(con->socket);
          retval=true;
       }
       else if (strstr(path,"PROC")!=NULL)
       {
          statusreport(con->socket);
          retval=true;
       }
       else if (strstr(path,"GRAPH")!=NULL)
       {
          statisticreport(con->socket, path);
          retval=true;
       }
       else if (strstr(path,"DemoImage.png")!=NULL)
       {
          DemoImage(con->socket);
          retval=true;
       }
       else
       {
         DisplayErrorPage(con->socket,"No such internal function!");
         retval=false;
       }
     }

return retval;
}

boolean w3ServerRespond (char *request, w3ServerConnection *con) {
  /* For now, do only GET stuff ... */
  char *path;
  char *temp;
  char *token=NULL;
  int  idx = 0;
  char *tmp_request=NULL;  /* Required for POST requests to get the URL of POST destination ! */

  tmp_request=strdup(request);

    
  
  /* path = w3ServerGetPath (request); */
  token = w3ServerGetToken (request, &idx);
  switch (w3ServerIdentifyCommand (token)) {
  case W3_CMD_GET:
    path = w3ServerGetToken (&request[idx], &idx);
    /* If client sent "GET / ", send standard index file. */ 
    if ( (strlen(path) == 1) || (path[strlen(path) - 1] == '/') ) {
      temp = StringCat (path, globalArgs.index_file);
      free(path);
      path = temp;
    }
    

    /* http://<domain>/watchdog?<command> handler */
    if (strncmp(path,"/watchdog",9)==0)
    /* 
     * Check for internal command
     * If it is an internal command than exec internal command if not
     * show file (if exist)
     *
     */
    {
#ifndef NDEBUG
        syslogprintf(__FILE__,__LINE__,"watchdog",5,"HTTP: Exec Watchdog internal command %s",path);
#endif
        watchdog_internal(path, con);  /* If known execute internal Watchdog command ! */
    }
    else
    {
    /* Not an internal command to execute a file is requested! */
    /* w3SendData ((void*)RESPONSE_OK, strlen (RESPONSE_OK) * sizeof(char), con->socket); */
#ifndef NDEBUG
        syslogprintf(__FILE__,__LINE__,"watchdog",5,"HTTP: Exec external command %s",path);
#endif
    w3ServerSendFile (path, con);
    }
    free (path);
    break;
  case W3_CMD_HEAD:
#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Got HEAD request");
#endif
    DisplayErrorPage(con->socket,"Service not available");
    break;
  case W3_CMD_POST:    
    {
#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Got POST request");
#endif

      char *p= w3GetPostData(tmp_request);
      if (p != NULL )
      {
#ifndef NDEBUG
        /*
         * Write POST to file for debug assistance
         */
        FILE * out=fopen("/tmp/watchdog.post.data","w");
        if (out)
        {
            fprintf(out,"%s",p);
            fclose(out);
        }
#endif
        /* free(p); */
      }

       path = w3ServerGetToken (&request[idx], &idx);
       if (strncmp(path,"/watchdog",9)==0)
       {  /* Seems to be an internal watchdog handled POST command try to execute it ! */
#ifndef NDEBUG
        syslogprintf(__FILE__,__LINE__,"watchdog",5,"HTTP: Exec Watchdog internal POST command %s",path);
#endif
          watchdog_internal(path, con);  /* If known execute internal Watchdog command ! */
       }
       else
       {   /* Not an internal command. Maybe it's a file and if not generate an error ! */
           w3ServerSendFile(path,con); 
       }

    }
    break;
  default:
#ifndef NDEBUG
    syslogprintf(__FILE__,__LINE__,"watchdog",5,"Unknown request");
#endif
    DisplayErrorPage(con->socket,"Bad request");
    break;
  }

    if( tmp_request != NULL) { free(tmp_request); tmp_request=NULL; } 
    if( token != NULL) { free (token); token=NULL; }  /* Avoid memory leak ! Roehrbein */
  return true;
}

/*
 * Check pathes given by users above HTTPBASE
 * If a user tries to access any files above HTTPBASE than this function
 * will redirect to index.html
 */

char *nimdacheck(char *path)
{
  char *p="/index.html";    /* 
                            * If someone want to access a file above HTTPBASE
                            * server will show the index file.
                            *
                            */
  int ctr=0;
  char *t=path;

  /* 
   * Count slashes and double dots
   * If there are more double dots as slashes
   * given path is destroyed and Watchdog will show 
   * index.html without any additional comments!
   *
   */

  while (*t!= '0')
  {
     if (*t == '/') ctr++;
     if (strlen(t)>=2) { if ( strncmp("..",t,2)==0) ctr--; };
     t++;
  }

  if (ctr>=0) p=path;

return p;
}

boolean w3ServerSendFile (char *path, w3ServerConnection *con) {
  FILE		*f;
  char		*filename;
  char		*tmp;
  boolean	isBin;
  pid_t		PID; 
  int		i, l;
  char		*tempFilename;
  char		buffer[10240];
  size_t	bytes = 0;

  /* path=nimdacheck(path); */

  filename = StringCat (globalArgs.httpbase, path);

#ifndef NDEBUG
  syslogprintf(__FILE__,__LINE__,"watchdog",5,"%s%s",globalArgs.httpbase,path);
#endif

  isBin = false;

  /****************************************************************/
  /* Check for and handle requests for binaries in HTDOC_BIN_BASE */
  /* tmp = copyString (path); */
  l = strlen (globalArgs.cgibase);
  if ( strlen (path) >= l ) {
    isBin = true;
    for (i = 0; i < l; i++) {
      if (path[i] != globalArgs.cgibase[i]) {
	isBin = false;
	break;
      }
    }
    if (isBin == true) {
      if ( !((i < strlen (path)) && (path[i] == '/')) ) {
	isBin = false;
      }
    }
    if (isBin == true) {
      /* Try to execute the filename, since it seems to be in /bin: */
      tmp = StringDup (HTDOC_BIN_TEMPLATE);
      /* if ( mktemp (tmp) == NULL ) { */
      if ( mkstemp (tmp) != 0 ) {
	syslogprintf(__FILE__,__LINE__,"watchdog",1,"Could not create temporary filename\n");
	free (tmp);
	return false;
      }
      tempFilename = StringCat (tmp, HTDOC_DOTHTML);
      free (tmp);
      PID = fork();
      switch (PID) {
      case 0: 
	tmp = StringCat (globalArgs.httpbase, tempFilename);
	free (tempFilename);
	tempFilename = tmp;
	tmp = NULL;
	freopen (tempFilename, "w", stdout);
	execl (filename, filename, NULL);
	perror ("executing file");
	break;
      default: waitpid (PID, NULL, 0);
	break;
      }
      w3ServerSendFile (tempFilename, con);
      free (tempFilename);
      return true;
    }
  }
  /****************************************/
  
  f = fopen (filename, "r");

  /*
  for (i = 0; i < 10240; i++) {
    buffer[i] = 0;
  }
  */

  memset (buffer,0,10240);

  if (f == NULL) {
    free (filename);
    DisplayErrorPage(con->socket,"Not found");
    return false;
  }
  while (!feof(f)) {
    bytes = fread ( (void*)&buffer[0], sizeof(char), 10240, f);
    if (ferror(f)) {
      syslogprintf(__FILE__,__LINE__,"watchdog",1,"Error reading requested file, terminating connection");
      DisplayErrorPage(con->socket,"Not found");
      fclose(f);
      free (filename);
      return false;
    }
     /* w3SendData ((void*)&buffer[0], bytes, con->socket); */

     htmlprintf(con->socket,bytes,buffer); 

  }
  fclose(f);
  free (filename);
  return true;
}

boolean w3ServerServeClient (w3Server *server, w3ServerConnection *con) {
  char *str;

  str = w3ServerReceiveRequest (con);
  if (str == NULL) {
    syslogprintf(__FILE__,__LINE__,"watchdog",2,"No data received !\n");
  }
  else
  {
  /* w3ServerMessage (str); */
  w3ServerRespond (str, con);
  free (str);
  }

  return true;
}

/*!
 * 
 */

void * w3StartServer (void * port) {
  w3ServerConnection *con;
  w3Server server;

#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog",5,"w3StartServer");
#endif

  server.port = globalArgs.http_port;
  if ( w3ServerInit (&server) == true )
  {

#ifndef NDEBUG
   syslogprintf(__FILE__,__LINE__,"watchdog",5,"w3ServerInit: OK");
#endif

  /*
   *  This loop serves the HTTP server
   *
   *  Endless loop which run until someone ask for termination of the HTTP server
   *  or Watchdog 
   */

  while ((globalArgs.runflags.keep_going==1) & (globalArgs.runflags.keep_HTTP_going==1)) {

    if (reinit==1)            /* Workaround required if using MySQL database write */
     w3ServerInit(&server);

    con = w3ServerAccept (&server);
    if (con != NULL) {
      w3ServerServeClient (&server, con);
      w3ServerDisconnect (con);
    }
  }
  w3ServerClose(&server);
  }
  else
  {
   syslogprintf(__FILE__,__LINE__,"watchdog",1,"w3ServerInit: NOT OK");
   raise(SIGTERM);
  }

  /* return false; */
  return (void *) "Nix";
}

















