# configure.ac
AC_INIT(top, 3.6.1)

echo "Configuring $PACKAGE_STRING"

AC_CONFIG_HEADER([config.h])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# options processing
AC_ARG_WITH(module, AC_HELP_STRING([--with-module=NAME],
		    [use the platform module NAME]),
		    [if test ! -f machine/m_$withval.c;
		     then AC_MSG_ERROR([No such module $withval]); fi])

AC_ARG_WITH(ext, AC_HELP_STRING([--with-ext=EXT],
    [use the extension EXT]),
    [if test -f ext/$withval.c; then
	AC_DEFINE(WITH_EXT, 1, [Include code that utilizes extensions])
	SRC="$SRC ext/$withval.c"
	OBJ="$OBJ $withval.o"
     else
        AC_MSG_ERROR([No such extension $withval])
     fi])

AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],
    [enable support for debugging output]))
if test "x$enable_debug" = xyes; then
    AC_DEFINE(DEBUG, 1, [Support for debugging output])
fi

AC_ARG_ENABLE(kill, AC_HELP_STRING([--disable-kill],
    [disable kill and renice commands]), ,[enable_kill=yes])
if test x$enable_kill = xyes; then
    AC_DEFINE(ENABLE_KILL, 1, [Enable kill and renice])
fi

enable_color=yes
AC_ARG_ENABLE(color, AC_HELP_STRING([--disable-color],
    [disable the use of color]))

AC_ARG_ENABLE(colour, AC_HELP_STRING([--disable-colour],
    [synonym for --disable-color]), [enable_color=$enableval])
if test x$enable_color = xyes; then
    AC_DEFINE(ENABLE_COLOR, 1, [Enable color])
fi

# check for needed programs
AC_CHECK_PROGS(MAKE, make)
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL

# system checks

# we make the version number available as a C preprocessor definition
AC_MSG_CHECKING(OS revision number)
osrev=`uname -r | tr -cd ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`

if test "$osrev" != "unknown" ; then
   AC_DEFINE_UNQUOTED(OSREV, $osrev, [Define the OS revision.])
   osmajor=`uname -r | sed 's/^\([[0-9]]*\).*$/\1/'`
   if test -n "$osmajor"; then
      AC_DEFINE_UNQUOTED(OSMAJOR, $osmajor, [Define the major OS revision number.])
   fi
else
   AC_DEFINE(OSREV, "")
fi
AC_MSG_RESULT($osrev)

# checks for libraries
AC_CHECK_LIB(kstat, kstat_open)
AC_CHECK_LIB(kvm, kvm_open)
AC_CHECK_LIB(elf, elf32_getphdr)
# -lmld -lmach
AC_CHECK_LIB(mach, vm_statistics)
AC_SEARCH_LIBS(tgetent, termcap curses ncurses)
AC_CHECK_LIB(m, exp)

# check for libraries required by extension
extlibs=""
if test -n "$with_ext" -a -f "${srcdir}/ext/$with_ext.libs"; then
    AC_MSG_CHECKING(for libraries needed by extensions)
    for lib in `cat "${srcdir}/ext/$with_ext.libs"`
    do
	saveLIBS=$LIBS
	LIBS="$LIBS -l$lib"
	AC_TRY_LINK(, [exit(0);], [extlibs="$extlibs -l$lib"], )
	LIBS=$saveLIBS
    done
    AC_MSG_RESULT($extlibs)
    LIBS="$LIBS$extlibs"
fi

# checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([stdarg.h termcap.h sys/resource.h sys/time.h])
AC_HEADER_TIME
AC_MSG_CHECKING(for a good signal.h)
SIGNAL_H="no"
for f in /usr/include/signal.h /usr/include/sys/signal.h /usr/include/sys/iso/signal_iso.h /usr/include/bits/signum.h; do
    if grep SIGKILL $f >/dev/null 2>&1; then
       SIGNAL_H=$f
       break
    fi
done
AC_MSG_RESULT($SIGNAL_H)
if test "$SIGNAL_H" = "no"; then
    SIGNAL_H="/dev/null"
fi
AC_SUBST(SIGNAL_H)

# checks for typedefs, structures, and compiler characteristics.
AC_CHECK_DECLS([sys_errlist])
AC_TYPE_SIGNAL
AC_CHECK_TYPE(time_t, long)

# Checks for library functions.
AC_CHECK_FUNCS([getopt memcpy setpriority strchr strerror snprintf sighold sigrelse sigaction sigprocmask])

# determine correct user, group, and mode
# these can be overridden later if need be
AC_MSG_CHECKING(for correct ls options)
lslong="ls -l"
if test `$lslong -d . | wc -w` -lt 9; then
   lslong="ls -lg"
fi
AC_MSG_RESULT($lslong)


# determine correct module
AC_MSG_CHECKING(for a platform module)
if test "$with_module"; then
    MODULE=$with_module
else
    case $target_os in
	aix4.2*)	MODULE=aix43;;
	aix4.3*)	MODULE=aix43;;
	aix5*)		MODULE=aix5;;
	dec-osf*)	MODULE=decosf1;;
	osf1*)		MODULE=decosf1;;
	osf4*)		MODULE=decosf1;;
	osf5*)		MODULE=decosf1;;
        freebsd*)	MODULE=freebsd; USE_KMEM=1;;
	hpux7*)		MODULE=hpux7;;
	hpux8*)		MODULE=hpux8;;
	hpux9*)		MODULE=hpux9;;
	hpux10*)	MODULE=hpux10;;
	hpux11*)	MODULE=hpux10;;
	irix5*)		MODULE=irix5;;
	irix6*)		MODULE=irixsgi;;
	linux*)		MODULE=linux; SET_MODE=755;;
        netbsd*)	MODULE=netbsd; SET_MODE=755;;
	solaris2*)	MODULE=sunos5; SET_MODE=755;;
	sunos4*)	MODULE=sunos4;;
	sysv4*)		MODULE=svr4;;
	sysv5*)		MODULE=svr5;;
	darwin*)
	    echo "macosx"
	    echo "The macosx module is untested.  Use at your own risk."
	    echo "If you really want to use this module, please run configure as follows:"
	    echo "    ./configure --with-module=macosx"
	    AC_MSG_ERROR([macosx module unsupported]);;
	*)  echo "none"
	    echo "Configure doesn't recognize this system and doesn't know"
	    echo "what module to assign to it.  Help the cause and run the"
	    echo "following command to let the maintainers know about this"
	    echo "deficiency!  Thanks.  Just cut and paste the following:"
echo "uname -a | mail -s $target_os bill@lefebvre.org"
	    echo ""
	    AC_MSG_ERROR([System type $target_os unrecognized])
    esac
fi
AC_MSG_RESULT($MODULE)
SRC="$SRC machine/m_$MODULE.c"
OBJ="$OBJ m_$MODULE.o"
AC_SUBST(SRC)
AC_SUBST(OBJ)

# extra things that need to be done for certain systems
# also handle setup for 64-bit detection
bits="default"
case $MODULE in
    aix5)
	AC_CHECK_LIB(perfstat, perfstat_cpu_total)
	if test -f /usr/sbin/bootinfo; then
	    bits="`/usr/sbin/bootinfo -K`"
	    extra_flag="-q64"
	fi
    ;;
    svr5)
        # -lmas
        AC_CHECK_LIB(mas, mas_open)
    ;;
    sunos5)
	if test -f /usr/bin/isainfo; then
	    bits="`/usr/bin/isainfo -b`"
	    if test -d /kernel/amd64; then
		extra_flag="-xarch=amd64"
	    else
		extra_flag="-xarch=v9"
	    fi
	fi
    ;;
esac

# if we are 64-bit, try to turn on the appropriate flags
AC_MSG_CHECKING(address space size)
if test "$bits" = "64"; then
    AC_MSG_RESULT(64 bit)
    if test `basename $CC` = "gcc"; then
        extra_flag="-m64"
    fi
    AC_MSG_CHECKING(whether $CC accepts $extra_flag)
    echo "main() { exit(0); }" >conftest.c
    if $CC -c $extra_flag conftest.c >/dev/null 2>&1; then
        AC_MSG_RESULT(yes)
	CFLAGS="$CFLAGS $extra_flag"
    else
	AC_MSG_RESULT(no)
    fi
    rm -f conftest.c conftest.o
else
    AC_MSG_RESULT($bits)
fi

AC_MSG_CHECKING(for installation settings)
# calculate appropriate settings
OWNER=""
GROUP=""
MODE=""
if test ! -n "$USE_KMEM" -a -d /proc; then
#   make sure we are installed so that we can read /proc
    rm -f conftest.txt
    if test -r /proc/0/psinfo; then
#       system uses solaris-style /proc
	$lslong /proc/0/psinfo >conftest.txt
    elif test -r /proc/1/stat; then
#       linux-style /proc?
	$lslong /proc/1/stat >conftest.txt
    else
	echo "-r--r--r-- 1 bin bin 32 Jan 1 12:00 /foo" >conftest.txt
    fi

#   set permissions so that we can read stuff in /proc
    if grep '^.......r..' conftest.txt >/dev/null; then
#	world readable
	MODE=755
    elif grep '^....r.....' conftest.txt >/dev/null; then
#	group readable
	MODE=2711
	GROUP=`awk ' { print $4 }'`
    else
#	probably only readable by root
	MODE=4711
	OWNER=`awk ' { print $3 }'`
    fi

elif test -c /dev/kmem; then
    $lslong -L /dev/kmem >conftest.txt
    if grep '^....r..r..' conftest.txt >/dev/null; then
        MODE=755
    elif grep '^....r..-..' conftest.txt >/dev/null; then
	MODE=2755
	GROUP=`$AWK ' { print $4 }' conftest.txt`
    fi
else
    MODE=755
fi
rm -f conftest.txt
# let module settings override what we calculated
OWNER=${SET_OWNER:-$OWNER}
GROUP=${SET_GROUP:-$GROUP}
MODE=${SET_MODE:-$MODE}

# set only those things that require it
result=""
INSTALL_OPTS_PROG=""
if test x$OWNER != x; then
    result="${result}owner=$OWNER, "
    INSTALL_OPTS_PROG="$INSTALL_OPTS_PROG -o $OWNER"
fi
if test x$GROUP != x; then
    result="${result}group=$GROUP, "
    INSTALL_OPTS_PROG="$INSTALL_OPTS_PROG -g $GROUP"
fi
result="${result}mode=$MODE"
INSTALL_OPTS_PROG="$INSTALL_OPTS_PROG -m $MODE"

AC_MSG_RESULT($result)

# Define man page supplement
MAN_SUPPLEMENT=machine/m_$MODULE.man
AC_SUBST_FILE(MAN_SUPPLEMENT)

AC_SUBST(MODULE)
AC_SUBST(INSTALL_OPTS_PROG)

# wrapup

AC_CONFIG_FILES(Makefile top.1)
AC_OUTPUT
