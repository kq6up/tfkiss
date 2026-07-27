dnl Process this file with autoconf to produce a configure script.
AC_INIT([tfkiss],[1.3.2],[christopher.maness@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

dnl Require C99 or later
AC_PROG_CC_STDC

dnl The firmware source uses multiple translation units that intentionally
dnl share global definitions (device, speed, speedflag). GCC 10+ changed
dnl the default from -fcommon to -fno-common, breaking this pattern.
dnl Use -fcommon to restore the original build behavior.
CFLAGS="$CFLAGS -fcommon"

dnl Platform-specific settings
case "$host" in
    *-*-netbsd* | *-*-dragonfly*)
        AC_DEFINE([HAVE_CRTSCTS], [1], [Define if CRTSCTS is available])
        tfkiss_device="/dev/tty00"
        ;;
    *-*-linux*)
        AC_DEFINE([HAVE_CRTSCTS], [1], [Define if CRTSCTS is available])
        tfkiss_device="/dev/ttyS0"
        ;;
    *-*-solaris*)
        LIBS="$LIBS -lnsl -lsocket"
        tfkiss_device="/dev/ttya"
        ;;
    *-*-darwin*)
        tfkiss_device="/dev/tty.usbserial"
        ;;
    *)
        AC_MSG_WARN([$host is not a tested platform, proceeding anyway])
        tfkiss_device="/dev/ttyS0"
        ;;
esac

AC_SUBST([tfkiss_device])

dnl Installation directories
dnl Expand paths at configure time by substituting prefix directly.
dnl This ensures @tfkiss_conf_dir@ etc. in .in files get real paths.
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix=$prefix
eval tfkiss_conf_dir="${sysconfdir}/tfkiss"
eval tfkiss_doc_dir="${datadir}/doc/tfkiss"
eval tfkiss_log_dir="${localstatedir}/log"
eval tfkiss_run_dir="${localstatedir}/run"
eval tfkiss_spool_dir="${localstatedir}/spool/tfkiss"
eval tfkiss_examples_dir="${datadir}/examples/tfkiss"

AC_SUBST([tfkiss_conf_dir])
AC_SUBST([tfkiss_doc_dir])
AC_SUBST([tfkiss_log_dir])
AC_SUBST([tfkiss_run_dir])
AC_SUBST([tfkiss_spool_dir])
AC_SUBST([tfkiss_examples_dir])
AC_DEFINE_UNQUOTED([DEF_INI_DIR], ["$tfkiss_conf_dir"],
    [Default directory for tfkiss.ini])

dnl Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h errno.h fcntl.h netinet/in.h \
                  sys/ioctl.h sys/socket.h sys/time.h \
                  termios.h unistd.h stdint.h])

dnl Checks for typedefs and structures.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL
AC_CHECK_TYPES([socklen_t], [], [], [#include <sys/socket.h>])

dnl Checks for library functions.
AC_CHECK_FUNCS([getcwd gettimeofday memset mkdir mktime \
                select socket strdup strerror strstr strtoul \
                usleep srandom memmove snprintf])

dnl Feature: HIBAUD
AC_MSG_CHECKING([whether to enable HIBAUD])
AC_ARG_ENABLE([hibaud],
    [AS_HELP_STRING([--enable-hibaud],
        [use high baud rate (>38400), Linux only])],
    [AS_IF([test "x$enableval" = "xyes"],
        [AC_DEFINE([USE_HIBAUD], [1], [Enable high baud rate support])
         AC_MSG_RESULT([yes])],
        [AC_MSG_RESULT([no])])],
    [AC_MSG_RESULT([no])])

dnl Feature: XPID
AC_MSG_CHECKING([whether to enable XPID])
AC_ARG_ENABLE([xpid],
    [AS_HELP_STRING([--enable-xpid],
        [enable PID change code support (default: yes)])],
    [AS_IF([test "x$enableval" = "xyes"],
        [AC_DEFINE([USE_XPID], [1], [Enable XPID support])
         AC_MSG_RESULT([yes])],
        [AC_MSG_RESULT([no])])],
    [AC_DEFINE([USE_XPID], [1], [Enable XPID support])
     AC_MSG_RESULT([yes (default)])])

dnl Feature: FLEXNET
AC_MSG_CHECKING([whether to enable FLEXNET support])
AC_ARG_ENABLE([flexnet],
    [AS_HELP_STRING([--enable-flexnet],
        [enable FLEXNET support (default: yes)])],
    [AS_IF([test "x$enableval" = "xyes"],
        [AC_DEFINE([USE_FLEXDEC], [1], [Enable FLEXNET support])
         AC_MSG_RESULT([yes])],
        [AC_MSG_RESULT([no])])],
    [AC_DEFINE([USE_FLEXDEC], [1], [Enable FLEXNET support])
     AC_MSG_RESULT([yes (default)])])

dnl Feature: AXIP
AC_MSG_CHECKING([whether to enable AXIP support])
AC_ARG_ENABLE([axip],
    [AS_HELP_STRING([--enable-axip],
        [enable AXIP support (default: yes)])],
    [AS_IF([test "x$enableval" = "xyes"],
        [AC_DEFINE([USE_AXIP], [1], [Enable AXIP support])
         AC_MSG_RESULT([yes])],
        [AC_MSG_RESULT([no])])],
    [AC_DEFINE([USE_AXIP], [1], [Enable AXIP support])
     AC_MSG_RESULT([yes (default)])])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    doc/Makefile
    examples/Makefile
    examples/tfkiss.ini
    examples/tfkiss.cfg
])

AC_OUTPUT
AC_MSG_RESULT([])
AC_MSG_RESULT([---- Configuration complete.])
AC_MSG_RESULT([     Type 'make' to compile.])
AC_MSG_RESULT([     Type 'make install' to install.])
AC_MSG_RESULT([     NOTE: Edit ${sysconfdir}/tfkiss/tfkiss.ini])
AC_MSG_RESULT([     and set your callsign and serial device.])
AC_MSG_RESULT([])
