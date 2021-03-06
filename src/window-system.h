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
	void *platform_data;
	struct poco_geometry screen;
	void (*event_loop)(struct poco_ws *);
};

struct poco_window {
	struct poco_geometry geo;
	void *private;
	bool (*minimize)();
	bool (*unminimize)();
};

void __poco_ws_init(struct poco_ws **ws);
void __poco_ws_cleanup(struct poco_ws *ws);

/* the following are platform implementation specific */
void __poco_ws_platform_init(struct poco_ws *ws);
void __poco_ws_platform_cleanup(struct poco_ws *ws);

#endif /* _WINDOW_SYSTEM_H_ */
