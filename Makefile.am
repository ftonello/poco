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
	src/window-system.h \
	src/window-stack.c \
	src/window-stack.h \
	src/rb-tree.c \
	src/rb-tree.h

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

if HAVE_TESTS
TESTS = check_rbtree
check_PROGRAMS = check_rbtree

check_rbtree_SOURCES = src/tests/rb-tree-test.c src/rb-tree.c
check_rbtree_CFLAGS = $(LIBCHECK_CFLAGS) $(AM_CFLAGS)
check_rbtree_LDADD = $(LIBCHECK_LIBS)
endif

poco_LDADD = $(AM_LDADD)
poco_CFLAGS = $(AM_CFLAGS)

dist_noinst_SCRIPTS = autogen.sh
