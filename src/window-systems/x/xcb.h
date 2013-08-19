#ifndef _XCB_H_
#define _XCB_H_

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

struct xcb_ewmh_connection_t;

struct poco_xcb {
	xcb_connection_t *conn;
	xcb_ewmh_connection_t *ewmh;
	int screen;
	xcb_window_t root;
	int default_screen;
	uint8_t root_depth;
	xcb_window_t us; /* Window Manager window id */
};

struct poco_xcb_window {
	xcb_window_t window;
	/* to be continued ... */
};

#endif /* _XCB_H_ */
