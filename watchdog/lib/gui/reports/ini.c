
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
 * HTML configuration of ini file
 *
 */

#include <ini.h>
#include <httpcomm.h>
#include <htmlgen.h>
#include <watchlist.h>


char * cfg [] ={
"<table style=\"text-align: left; width: 100%;  background-color: rgb(0, 0,0); \" border=\"1\" class=\"table\" >",
"  <tbody>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/active_process.html\">Active process</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"1024\" size=\"100\" readonly=\"readonly\"",
" name=\"df_process_name\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/active.html\">Activate monitoring</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"checkBox\" ",
" checked=\"checked\" name=\"cb_enable\" value=\"check\"",
" type=\"checkbox\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\">Monitor",
"settings</td>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/check.html\">check</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\"",
" maxlength=\"1024\" size=\"100\" name=\"df_check\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/count.html\">count</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"3\" size=\"3\" name=\"df_count\" value=\"1\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/external.html\">external</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" value=\"/etc/init.d/&lt;script&gt;  check\" maxlength=\"1024\"",
" size=\"100\" name=\"df_external\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/restart.html\">restart</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" value=\"/etc/init.d/&lt;script&gt; restart\" maxlength=\"1024\"",
" size=\"100\" name=\"df_restart\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/onerror.html\">onerror</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"1024\" size=\"100\" name=\"df_onerror\"",
" value=\"shutdown -r now\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/runlevel.html\">runlevel</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"14\" size=\"14\" name=\"df_runlevel\"",
" value=\"1+,2+,3+,4-,5-\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/mail.html\">mail</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"checkBox\" ",
" name=\"df_mail\" value=\"yes\" type=\"checkbox\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/owner.html\">owner</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"1024\" size=\"100\" name=\"df_owner\"",
" value=\"user@domain\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/service.html\">service</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"1024\" size=\"100\" name=\"df_service\"",
" value=\"mailx -s &quot;Watchdog @$host&quot; $owner\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/message.html\">message</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"1024\" size=\"100\" name=\"df_message_file\"",
" value=\"/etc/watchdog/modules/default/message.txt\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/retries.html\">retries</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"3\" size=\"3\" name=\"df_retries\" value=\"5\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/delay.html\">delay</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"10\" size=\"10\" name=\"df_delay\"",
" value=\"60\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/debug.html\">debug</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"checkBox\" ",
" name=\"cb_debug\" value=\"yes\" type=\"checkbox\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/syslog.html\">syslog</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"checkBox\" ",
" checked=\"checked\" name=\"cb_syslog\" value=\"yes\"",
" type=\"checkbox\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/on_error_force_reboot.html\">on_error_force_reboot</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"checkBox\" ",
" name=\"cb_onerrorforcereboot\" value=\"yes\" type=\"checkbox\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/logging.html\">data log</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"checkBox\" ",
" name=\"cb_logging\" value=\"yes\" type=\"checkbox\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"><a",
" href=\"/help/watchdog.ini/schedule.html\">schedule</a></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"inputField\" ",
" maxlength=\"1024\" size=\"100\" name=\"df_schedule\"",
" value=\"* * * * *\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"      <td align=\"undefined\" valign=\"undefined\"><input class=\"pushButton\" ",
" name=\"Submit\" type=\"submit\"><input class=\"pushButton\"  name=\"Reset\"",
" type=\"reset\"></td>",
"    </tr>",
"    <tr>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"      <td align=\"undefined\" valign=\"undefined\"></td>",
"    </tr>",
"  </tbody>",
"</table>",
NULL
};



void inireport(int port)
{
int n;
HTMLPageHeader (port,"Watchdog: Process monitor settings","watchdog?UPDATE","icons/watchdog_bg.png",0);
HeadLine(port,"INI file configuration (under construction)");

n^=n;

while (cfg[n] != NULL)
{
	switch (n)
	{	

	  default: 
        	     htmlprintf(port,0,"%s\n",cfg[n++]);
	}
}

HTMLPageFooter(port);
 
}
