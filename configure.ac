AC_INIT([poco], [0.1], [eu@felipetonello.com], [poco], [http://felipetonello.com/])
AC_PREREQ([2.63])
AM_INIT_AUTOMAKE([1.11 -Wall -Wno-portability no-define])

AC_ARG_WITH([x], AS_HELP_STRING([--with-x], [Build PoCo for X11]),
    [PKG_CHECK_MODULES(XCB, [ xcb xcb-aux xcb-util xcb-ewmh xcb-event ],
        AC_DEFINE([POCO_WS_X], [], [Build PoCo for X11]))])

AC_ARG_WITH([wayland], AS_HELP_STRING([--with-wayland], [Build PoCo for Wayland]),
    [AC_DEFINE([POCO_WS_WAYLAND], [], [Build PoCo for Wayland])])

AS_IF([test "x$with_x" = "xyes" && test "x$with_wayland" = "xyes"],
    [AC_MSG_ERROR([*** Cannot build PoCo for X11 and Wayland. You have to choose one window system.])])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AM_PROG_CC_C_O
AC_CONFIG_FILES([Makefile])
AC_OUTPUT