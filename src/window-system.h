#ifndef _WINDOW_SYSTEM_H_
#define _WINDOW_SYSTEM_H_

#ifdef POCO_WS_X
#include "window-systems/xcb.h"
#elif POCO_WS_WAYLAND
#include "window-systems/wayland.h"
#endif

void __poco_ws_init();

void __poco_ws_cleanup();

#endif /* _WINDOW_SYSTEM_H_ */
