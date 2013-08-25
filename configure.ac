AC_INIT([PoCo], [0.1], [eu@felipetonello.com], [poco], [http://felipetonello.com/])
AC_PREREQ([2.63])
AM_INIT_AUTOMAKE([1.11 -Wall -Wno-portability no-define])

#### with X11 support option ####
AC_ARG_WITH([x], AS_HELP_STRING([--with-x], [Build PoCo for X11]),
    [PKG_CHECK_MODULES(XCB, [ xcb xcb-aux xcb-util xcb-ewmh xcb-event ],
        AC_DEFINE([POCO_WS_X], [], [Build PoCo for X11]))], with_x=no)

AM_CONDITIONAL([HAVE_X], [test "x$with_x" = "xyes"])

#### with Wayland support option ####

AC_ARG_WITH([wayland], AS_HELP_STRING([--with-wayland], [Build PoCo for Wayland]),
    [AC_DEFINE([POCO_WS_WAYLAND], [], [Build PoCo for Wayland])], with_wayland=no)

AM_CONDITIONAL([HAVE_WAYLAND], [test "x$with_wayland" = "xyes"])

#### Check which window system was selected ####
AS_IF([test "x$with_x" = "xyes" && test "x$with_wayland" = "xyes"],
    [AC_MSG_ERROR([*** Cannot build PoCo for X11 and Wayland. You have to choose one window system.])])

AS_IF([test "x$with_x" = "xno" && test "x$with_wayland" = "xno"],
    [AC_MSG_ERROR([*** You should specify one window system. Options are X or Wayland.])])

#### check unit tests ####

AC_ARG_ENABLE([tests],
    AS_HELP_STRING([--disable-tests],[disable unit tests]))

AS_IF([test "x$enable_tests" != "xno"],
    [PKG_CHECK_MODULES(LIBCHECK, [ check ], HAVE_LIBCHECK=1, HAVE_LIBCHECK=0)],
    HAVE_LIBCHECK=0)

AS_IF([test "x$enable_tests" = "xyes" && test "x$HAVE_LIBCHECK" = "x0"],
    [AC_MSG_ERROR([*** check library not found])])

AM_CONDITIONAL([HAVE_TESTS], [test "x$HAVE_LIBCHECK" = x1])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AM_PROG_CC_C_O
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
