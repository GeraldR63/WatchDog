
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

#ifndef __HTMLGEN_H
#define __HTMLGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/*
 * (c) 1998-2007 Gerald Roehrbein
 * Boskamp 19
 * 24214 Gettorf
 *
 * This code is licensed under GPL 2.0.
 *
 */


/*
 * Definition of two functions to generate a error free HTML page header and page footer
 *
 */

/* This is the first function you need to call if you want to create a HTML page 
 *
 * Page "constructor"!
 *
 */
void HTMLPageHeader(const int sock,             /* Socket */
                    const char *caption,        /* Caption line of a generated page */
		    const char *submiturl,      /* Where to move if submit button pressed */
		    const char *urlBackGndPic,	/* URL of a background image */
                    const unsigned int timeout);/* Automated forward to page submiturl if != 0 */

/* This is the last function you need to call if you want to create a HTML page 
 *
 * Page "destructor"!
 * 
 */
void HTMLPageFooter(const int sock);


void Header(const int sock);
void Footer(const int sock);

/* 
 * ComboBox creation
 *
 */
void	beginComboBox(const int sock,const char *name);       /* Name of ComboBox */
void	endComboBox(const int sock);
void    ComboBoxAddItem(const int sock,const char *value);    /* Default Values in the combobox */

/*
 * Table creation
 *
 */
void startTable(const int sock, const char *fmt, ...);
void TableAddRow(const int sock, const char *fmt, ...);
void endTable(const int sock, const char *fmt, ...);

/*
 * Other (simple) HTML elements
 * Not all HTML elements already defined
 *
 */

void Variable(const int sock,const char *name, const char * value);
void EditField(const int sock,const char *fielddesc, const char * varname, const int len);
void CheckBox(const int sock,const char * varname, const char *value, const char *desc);
void RadioButton(const int sock,const char * varname, const char *value, const char *desc);

void LineFeed(const int sock);  /* LineFeed */
void SEP(const int sock); /* Separator */
void SubmitButton(const int sock);
void ResetButton(const int sock,const char *t);
void Caption(const int sock,const char *);
void BodyStart(const int sock,const char *submitjob, const char *bgpic, const unsigned int timeout);
void BodyEnd(const int sock);
void HeadLine(const int sock,const char *h);
void HyperLink(const int sock,const char *disp, const char *go);

/* A hyperlink check box  or group of hyperlink check boxes */
void HLCB(        const int sock,
		  const char * linkname,  /* Visible part of the name */
		  const char * link,      /* Where to go if clicked   */
		  const char * vargrp,    /* Name of the group        */
		  const char * value,     /* POST value               */
		  const char * desc);     /* A visible description of the checkbox */

/* A hyperlink radio button  or group of hyperlink radio buttons */
void HLRB(        const int sock,
		  const char * linkname,  /* Visible part of the name */
		  const char * link,      /* Where to go if clicked   */
		  const char * vargrp,    /* Name of the radio button group which is POSTED */
		  const char * value,     /* POST value               */
		  const char * desc);     /* A visible description of the radiobutton */

/* Generate Java Script which redirect after 5 seconds to a given page */
void AutomatedExec(const int sock, const char *url);

void PassWordMessageBox(const int sock, char *url);
void DisplayPasswordHyperLink(const int sock, const char *);

void DisplayErrorPage(const int sock, const char *message);  /* HTML File not found !*/

int cgi_settings(const int sock);       /* Prints all of the CGI relevant environment variables */

#endif
