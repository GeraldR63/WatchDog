# WatchDog
WatchDog is a software to visualize and manage processes in LINUX environments
It's works like the well known top but it is accessible via a WEB browser. The software is also able to check for processes that have to run on a server
and restart them if required.


Remark:
I just compiled it now.
All you have to do is to install the source in /etc/watchdog.

Than you habe to build the PNG library from source (directory watchdog/source/third.party/libpng).

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


This code works. You get a full working HTTP server, PNG libraries. You get some primitives (linw/ circle) to write statistical data into dynamicaly generated PNG images. There are also some primitives to read from initialization files.

Screenshot to show that it runs, more ore less, using LYNX as HTTP client instead of FireFox/ IE.


<img src="https://github.com/GeraldR63/AstroNavi/blob/main/Sight%20Reduction%20Form.jpg" width="900" height="1024">

