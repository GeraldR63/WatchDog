# WatchDog for LINUX
WatchDog is a software to visualize and manage processes in LINUX environments
It's works like the well known top but it is accessible via a WEB browser. The software is also able to check for processes that have to run on a server
and restart them if required.


Remark:
I just compiled it now (20230415) to check that it more or less runs. I did not fix all issues the software faces because since development before 2007 and present time a lot of changes made to HTML and the LINUX core. Pls. fix this by yourself. I won't fix this because I can not spend time anymore to this project. 

All you have to do is to install the source in /etc/watchdog.

Than you have to build the PNG library from source (directory watchdog/source/third.party/libpng).

make test
make install.

Maybe you have to add -DPNG_NO_MMX_CODE to compiler options to make it compile.
Than copy  

cp libpng.so.3.1.2.16  /usr/lib/libpng.so.3

You also should remove everything related to MySQL from the code or just create a comment. Up2now it seems that 

apt install libmysqlclient-dev

is not available anymore. It also seems that it is required to have some changes to FireFox to show the images. Using LYNX it worked. Sorry, this code is from 2007 and it worked in 2007 in Red Hat Environments very well.

To compile the watchdog it's also required to add -lfreetype to the compiler options in the makefile.


It's also good to have in the watchdogs build directory commands below

cd /etc/wachtdog
chown root:root -R *
chmod -R a+x
Additional remark: Include instead of <asm/page.h> in file "m_linux.c" <sys/user.h> to avoid compile error with PAGE_SHIFT and other constants.



This code works. You get a full working HTTP server, PNG libraries. You get some primitives (line/ circle) to write statistical data into dynamicaly generated PNG images. There are also some primitives to read from initialization files.

Screenshot to show that it runs, more ore less, using LYNX as HTTP client instead of FireFox/ IE.
The server is at 127.0.1.1:81. Change it for your needs. I offered this at my own Web Server for several years as a service for users with no problems. However if it runs correct it looks very different. HTML table and status images green, yellow red and so on are missing in screenshot below becaus LNYX can not show them.


<img src="https://github.com/GeraldR63/WatchDog/blob/main/screenshot.png" width="1024" height="768">

This is a powerful tool or better it was a powerful tool in LINUX HA environments. Because it managed a lot of stuff by itself and it offers decentral monitoring in real time.

Maybe this source is not the latest release I wrote but it works and shows a lot of useful C stuff.
If everything is perfect than you should see a lot of nice images and stats. Image above show not the full working application because FireFox only shows the HTML source instead of the generated page. No clue what these guys changed last years. It's not fun to work in the IT anymore. Too many changes to basic stuff.

I publish this because I'm old today but maybe there are some younger fellows interested in socket programming and so on.
