AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = poco
poco_SOURCES = \
	src/main.c \
	src/event-loop.c \
	src/event-loop.h \
	src/platformdata.c \
	src/platformdata.h \
	src/window-system.c \
	src/window-system.h \
	src/window-systems/x/xcb.c \
	src/window-systems/x/xcb.h
poco_CFLAGS = -I$(abs_top_srcdir)/src $(XCB_CFLAGS)
poco_LDADD = $(XCB_LIBS)

dist_noinst_SCRIPTS = autogen.sh
