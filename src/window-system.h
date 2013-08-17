#ifndef _WINDOW_SYSTEM_H_
#define _WINDOW_SYSTEM_H_

#ifdef POCO_WS_X
#include "window-systems/x/xcb.h"
#elif POCO_WS_WAYLAND
#include "window-systems/wayland/wayland.h"
#endif

#include <stdint.h>
#include <stdbool.h>

struct poco_screen {
	uint32_t width; /* in pixels */
	uint32_t height; /* in pixels */
};

struct poco_ws {
	struct void *platformdata;
	struct poco_screen screen;
	bool (*event_loop)(struct poco_ws *);
};

struct poco_ws * __poco_ws_new();

void __poco_ws_free(struct poco_ws *ws);

/* the following are implementation specific */

void __poco_ws_init(struct poco_ws *ws /* OUT */);

void __poco_ws_cleanup(struct poco_ws *ws);

#endif /* _WINDOW_SYSTEM_H_ */
