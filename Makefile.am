AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = poco
poco_SOURCES = \
	src/main.c \
	src/utils.h \
	src/event-loop.c \
	src/event-loop.h \
	src/platformdata.c \
	src/platformdata.h \
	src/window-system.c \
	src/window-system.h

AM_CFLAGS = \
	-I$(top_srcdir)/src
AM_LDADD = 

if HAVE_X
poco_SOURCES += \
	src/window-systems/x/xcb.c \
	src/window-systems/x/xcb.h

AM_CFLAGS += $(XCB_CFLAGS)
AM_LDADD += $(XCB_LIBS)
endif

if HAVE_WAYLAND
poco_SOURCES += \
	src/window-systems/wayland/wayland.c \
	src/window-systems/wayland/wayland.h
endif

poco_LDADD = $(AM_LDADD)
poco_CFLAGS = $(AM_CFLAGS)

dist_noinst_SCRIPTS = autogen.sh
