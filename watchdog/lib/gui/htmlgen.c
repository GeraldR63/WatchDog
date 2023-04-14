
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
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <httpcomm.h>
#include <string.h>


void char2Dprint(const int sock, const char* l[])
/*
 * Writes an array of strings to given socket.
 * Planned for internal use of htmlgen.h only
 *
 */

{
int i=0;
while (l[i]!=NULL)  htmlprintf(sock,0,"%s\n",l[i++]);
}

void Header(const int sock)
{
char *l[]={"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n",
	   "<HTML>",
           "<!DOCTYPE HTML PUBLIC \"-//W3O//DTD W3 HTML 2.0//EN\">",
           "<!Author: G.Roehrbein, Boskamp 19 24214 Gettorf Germany>",
	   "<!        e-mail Gerald.Roehrbein@OraForecast.com>"
           "<link rel=\"stylesheet\" type=\"text/css\" href=\"styles/formstyle.css\">",
           NULL};

char2Dprint(sock,l);

}

void Footer(const int sock)
{
char *l[]={"<!Copyright OraForecast.com, Germany>",
           NULL};
char2Dprint(sock,l);
}

void Variable(const int sock , const char *name, const char * value)
/* Creates an invisible variable in a WebPage */
{
	htmlprintf(sock,0,"<input type=hidden name=%s value=%s>\n",name,value);
}

void beginComboBox(const int sock, const char *name)
/* Constructor of ComboBox class */
{
	htmlprintf(sock,0,"<SELECT NAME=%s>\n",name);
}

void ComboBoxAddItem(const int sock,const char *value)
/* Add a value to a combobox */
{
	htmlprintf(sock,0,"<OPTION>%s\n",value);
}

void endComboBox(const int sock)
/* Destructor of ComboBox class */
{
	htmlprintf(sock,0,"</SELECT>\n");
}

void startTable(const int sock, const char *fmt, ...)
/* Start a HTML table */
{
                   va_list ap;
                   int d;
                   char c, *s;

                   va_start(ap, fmt);
                   while (*fmt)
                        switch(*fmt++) {
                        case 's':           /* Only headlines of type string allowed */
                             s = va_arg(ap, char *);
                             printf("string %s\n", s);
                        case 'd':           /* Width of field */
                             d = va_arg(ap, int);
                             printf("int %d\n", d);
                             break;
                        case 'c':           /* Color of field */
                             s =  va_arg(ap, char *);
                             printf("char %c\n", c);
                             break;
                        }
                   va_end(ap);

}

void TableAddRow(const int sock, const char *fmt, ...)
/*
 * Add one row to the HTML table
 *
 */
{
                   va_list ap;
                   int d;
                   char c, *s;

                   va_start(ap, fmt);
                   while (*fmt)
                        switch(*fmt++) {
                        case 's':           /* Strings allowed  */
                             s = va_arg(ap, char *);
                             printf("string %s\n", s);
                             break;
                        case 'd':           /* Width of field */
                             d = va_arg(ap, int);
                             printf("int %d\n", d);
                             break;
                        case 'c':           /* Color of field */
                             s =  va_arg(ap, char *);
                             printf("char %c\n", c);
                             break;
                        }
                   va_end(ap);
}

void endTable(const int sock, const char *fmt, ...)
/* End HTML table */
{
                   va_list ap;
                   int d;
                   char c, *s;

                   va_start(ap, fmt);
                   while (*fmt)
                        switch(*fmt++) {
                        case 's':           /* string */
                             s = va_arg(ap, char *);
                             printf("string %s\n", s);
                             break;
                        case 'd':           /* int */
                             d = va_arg(ap, int);
                             printf("int %d\n", d);
                             break;
                        case 'c':           /* char */
                             /* need a cast here since va_arg only
                                takes fully promoted types */
                             c = (char) va_arg(ap, char *);
                             printf("char %c\n", c);
                             break;
                        }
                   va_end(ap);
}

void EditField(const int sock,const char *fielddesc, const char * varname, const int len)
/* Creates an EditField */
{
	htmlprintf(sock,0,"<strong>%s:</strong> <input ALIGN=right NAME=%s size=%d>\n",fielddesc,varname,len);
}

void CheckBox(const int sock,const char * varname, const char *value, const char *desc)
/* Creates a CheckBox. Details see RadioButton */
{
	htmlprintf(sock,0,"<INPUT TYPE=CHECKBOX NAME=%s VALUE=%s>  %50s  -  \n",varname,value,desc);
}

void RadioButton(const int sock, const char * varname, const char *value, const char *desc)
/*
* Creates a RadioButton
* First param declares the group where button belong to
* Second defines selected value
* Third param shows a info message for the user
*/
{
	htmlprintf(sock,0,"<INPUT TYPE=RADIO NAME=%s VALUE=%s> %50s  -  \n",varname,value,desc);
}


void LineFeed(const int sock)
/* Creates a LineFeed */
{
	htmlprintf(sock,0,"<BR>\n");
}

void SEP(const int sock)
/* Creates a separator */
{
    htmlprintf(sock,0,"<HR>\n");
}

void SubmitButton(const int sock)
/* Creates a SubmitButton */
{
htmlprintf(sock,0,"<input TYPE=\"submit\" VALUE=\"Ok\">\n");
}

void ResetButton(const int sock)
/* Creates a ResetButton */
{
htmlprintf(sock,0,"<input TYPE=\"reset\" VALUE=\"Formular loeschen\">\n");
}

void Caption(const int sock, const char *s)
/* Creates a new caption */
{
	char *l="<head><title>%s</title></head>\n";
    htmlprintf(sock,0,l,s);
}


void BodyStart(const int sock, const char *job, const char *bgpic, const unsigned int timeout)
/*
 * Creates the body of a HTML page
 * First param defines URL to call when submit is pressed
 * second is the URL of the background image
 *
 * If timeout is != 0 than automated forwarding after a period of time is generated into the page.
 *
 */
{
char * delayer[]={  /* Required JavaScript functions */
		"<script language=\"JavaScript\">\n",
			"<!--\n"
			"function delayer(){\n",
			"window.location=\"%s\";\n",
			"}\n",
			"// --- Stop Hiding --->\n",
		"</script>\n", NULL
};

char tmp[64];
timeout==0?sprintf(tmp," ") : sprintf(tmp,"onload=\"setTimeout('delayer()',%d)\"",timeout*1000);

	 htmlprintf(sock,0,"<body %s BACKGROUND=\"%s\" style=\"color: rgb(255, 255, 255); background-color: rgb(0, 0, 0);\" alink=\"#ffcccc\" link=\"#ff0000\" vlink=\"#ffff66\"><form METHOD=POST ACTION=%s>\n",
           tmp,
           bgpic,
           job);

if (timeout != 0)
{
   int i=0;
   while (delayer[i]!=NULL)
   {
      htmlprintf(sock,0,delayer[i++],job);
   }
}
}

void HTMLPageHeader(const int sock,
                    const char *caption,        /* Caption line of a generated page       */
                    const char *submiturl,      /* Where to move if submit button pressed */
                    const char *urlBackGndPic,  /* URL of a background image to display   */
                    const unsigned int timeout) /* Timeout (automaticly start submiturl)  */
{
  Header(sock);
  Caption(sock,caption);
  BodyStart(sock,submiturl,urlBackGndPic, timeout);

}


void BodyEnd(const int sock)
{
	 htmlprintf(sock,0,"</body>");
}

void HTMLPageFooter(const int sock)
{
   BodyEnd(sock);
   Footer(sock);
}
  
void HeadLine(const int sock,const char *h)
/* Creates a headline */
{
 htmlprintf(sock,0,"<FONT COLOR=\"FF0000\"><H3>%s</H3></FONT>\n",h);
}

void HyperLink(const int sock,const char *disp, const char *go)
/* 
 * Creates a hyperlink
 * First param is the text to display and second where to go
 *
 */
{
	htmlprintf(sock,0,"<A HREF=%s>%s</A>\n",go,disp);
}

void HLCB(        const int sock,
		  const char * linkname, 
		  const char * link, 
		  const char * vargrp,
		  const char * value,
		  const char * desc)
/*  
 *  HyperLink Checkbox
 *  A combination of Hyperlink and CheckBox is displayed in one line
 *
 */
{
	CheckBox(sock,vargrp,value,desc);
	HyperLink(sock,linkname,link);
	htmlprintf(sock,0,"<BR>");
}

void HLRB(        const int sock,
		  const char * linkname, 
		  const char * link, 
		  const char * vargrp,
		  const char * value,
		  const char * desc)
/*  
 *  HyperLink RadioButton
 *  A combination of Hyperlink and CheckBox is displayed in one line
 *
 */
{
	RadioButton(sock,vargrp,value,desc);
	HyperLink(sock,linkname,link);
	htmlprintf(sock,0,"<BR>");
	
}


#define BUFSIZE 1024*10


#define n 19 
int cgi_settings(const int sock)
/* 
 * Prints a complete HTML page to socket containing settings of
 * all of the environmentvariables and probably posted form values above!
 *
 */
{

char *l[]={"Content-type: text/html\n",
		   "<HTML>",
           "<!DOCTYPE HTML PUBLIC \"-//W3O//DTD W3 HTML 2.0//EN\">",
	       "<head>",
		   "<title>CGI</title>",
		   "</head>",
           "<body>",
		   "<h1>CGI- Variables</h1>",
		   "<pre>Settings</pre>",
		   "<form METHOD=\"POST\" ACTION=\"index.html\">",
		   "<input type=\"hidden\" name=\"subject\" value=\"Feedback\">",
		   "<input type=\"hidden\" name=\"RCPT\" value=\"Gerald.Roehrbein@OraForecast.com\">",
		   "<p><strong>Section:</strong><input NAME=\"section\" size=\"50\"> <br>",
		   "<p><strong>Key:</strong><input NAME=\"key\" size=\"50\"> <br>",
		   "<strong>Value:</strong> <input NAME=\"value\" size=\"70\"> </p>",
		   "<p><input TYPE=\"submit\" VALUE=\"Ok\">",
		   "<input TYPE=\"reset\" VALUE=\"Clear Form\">. </p>",
		   "</form>",
		   "</body>"};

#define MAXVARS 27
char *cgiVars[]={	"DOCUMENT_ROOT",
					"GATEWAY_INTERFACE",
					"HTTP_ACCEPT",
					"HTTP_ACCEPT_ENCODING",
					"HTTP_ACCEPT_LANGUAGE",
					"HTTP_HOST",
					"HTTP_PRAGMA",
					"HTTP_REFERER",
					"HTTP_USER_AGENT",
					"HTTP_VIA",
					"PATH",
					"PATH_TRANSLATED",
					"QUERY_STRING",
					"REMOTE_ADDR",
					"REMOTE_HOST",
					"REMOTE_PORT",
					"REQUEST_METHOD",
					"REQUEST_URI",
					"SCRIPT_FILENAME",
					"SCRIPT_NAME",
					"SERVER_ADMIN",
					"SERVER_NAME",
					"SERVER_PORT",
					"SERVER_PROTOCOL",
					"SERVER_SOFTWARE",
					"subject",
					"RCPT"
				};

	int i=0;
	
	for(;i<n;i++) 
	{ 
		htmlprintf(sock,0,"%s\n",l[i]);
		if (i==n-1)  /* Insert before last line of static text the environment variables! */
		{
			int i=0;
			for(;i<MAXVARS;i++)
			{ 
				char *e=getenv(cgiVars[i]);

				if (!e) e="<NIX>";

				htmlprintf(sock,0,"<br>%s=[%s]\n",cgiVars[i], e);
			}
		}

    }
	

	{
        char buf[BUFSIZE];
	    char inchar;
		char * str_value;
		int buflen,i;
		memset(buf,0,BUFSIZE);


		htmlprintf(sock,0,"<H2>Form Values (from stdin):</H2>\n");
		str_value = getenv("CONTENT_LENGTH");
		if (str_value != (char *) 0)	
		{
		   buflen = atoi(str_value);	
		   i = 0;	
		   while (i < buflen)		
		   {		
		      inchar = fgetc(stdin);
		      if (inchar == EOF) break;		
		      buf[i++] = (char) inchar;		

		   }	
		   buf[i] = '\0';	
		}else
			buf[0] = '\0';
		htmlprintf(sock,0,buf);
	
	}
	
	return 0;
}


void DisplayErrorPage(const int sock,char *message)  /* Displays file not found error message ! */
{
htmlprintf(sock,0,"%s <h2>%s</h2> %s", 
"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n\
<html><head><title>\nWatchdog HTTP server's error message</title></head>\n\
<body bgcolor=black text=white>\n\
<address>Watchdog HTTP server by Gerald Roehrbein and Christian Gosch</address><hr>\n\
<br>\
<img src=/images/watchdog.png alt=\"Watchdog\">\n<hr>\n",
message
,
"<copyright>Watchdog copyright (C) 2007 by Gerald Roehrbein<br>\n\
Thanks to Christian Gosch for some lines of a  HTTP server!</copyright><br>\n\
</body></html>\n");

}
