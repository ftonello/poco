#ifndef _WINDOW_SYSTEM_H_
#define _WINDOW_SYSTEM_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#error "You should run ./autogen script before."
#endif

#include <stdint.h>
#include <stdbool.h>

struct poco_geometry {
	uint32_t x; /* in pixels */
	uint32_t y; /* in pixels */
	uint32_t width; /* in pixels */
	uint32_t height; /* in pixels */
};

struct poco_ws {
	/* xcb or wayland */
	void *platformdata;
	struct poco_geometry screen;
	void (*event_loop)(struct poco_ws *);
};

struct poco_window {
	struct poco_geometry geo;
	void *platformdata;
	bool (*minimize)();
	bool (*unminimize)();
};

struct poco_ws * __poco_ws_new();

void __poco_ws_free(struct poco_ws *ws);

/* the following are implementation specific */

struct poco_ws * __poco_ws_init();

void __poco_ws_cleanup(struct poco_ws *ws);

#endif /* _WINDOW_SYSTEM_H_ */
