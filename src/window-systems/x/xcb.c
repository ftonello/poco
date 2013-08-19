#include "window-systems/x/xcb.h"
#include "window-system.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xcb/xcb_aux.h>
#include <xcb/xcb_event.h>

#define	LENGTH(x)	(sizeof(x) / sizeof(*x))

inline struct poco_xcb * get_platformdata(struct poco_ws *ws)
{
	return (struct poco_xcb *)ws->platformdata;
}

inline void set_platformdata(struct poco_ws *ws, struct poco_xcb *xcb)
{
	ws->platformdata = (void *)xcb;
}

void create_notify(xcb_create_notify_event_t *evt)
{
	printf("CreateNotify handled.\n");
}

void destroy_notify(xcb_destroy_notify_event_t *evt)
{
	printf("DestroyNotify handled.\n");
}

void map_request(xcb_map_request_event_t *evt)
{
	printf("MapRequest handled.\n");
}

void map_notify(xcb_map_notify_event_t *evt)
{
	printf("MapNotify handled.\n");
}

void unmap_notify(xcb_unmap_notify_event_t *evt)
{
	printf("UnmapNotify handled.\n");
}

void event_loop(struct poco_ws *ws)
{
	struct poco_xcb *xcb = get_platformdata(ws);
	xcb_generic_event_t *event;

	while (event = xcb_poll_for_event(xcb->conn)) {
		uint8_t resp_type = XCB_EVENT_RESPONSE_TYPE(event);
		switch (resp_type) {
		case XCB_CREATE_NOTIFY:
			create_notify((xcb_create_notify_event_t *)event);
			break;
		case XCB_DESTROY_NOTIFY:
			destroy_notify((xcb_destroy_notify_event_t *)event);
			break;
		case XCB_MAP_REQUEST:
			map_request((xcb_map_request_event_t *)event);
			break;
		case XCB_MAP_NOTIFY:
			map_notify((xcb_map_notify_event_t *)event);
			break;
		case XCB_UNMAP_NOTIFY:
			unmap_notify((xcb_unmap_notify_event_t *)event);
			break;
		/* case XCB_CLIENT_MESSAGE: */
		/* 	client_message(event); */
		/* 	break; */
		/* case XCB_CONFIGURE_REQUEST: */
		/* 	configure_request(event); */
		/* 	break; */
		/* case XCB_PROPERTY_NOTIFY: */
		/* 	property_notify(event); */
		/* 	break; */
		/* case XCB_ENTER_NOTIFY: */
		/* 	enter_notify(event); */
		/* 	break; */
		/* case XCB_MOTION_NOTIFY: */
		/* 	motion_notify(event); */
		/* 	break; */
		/* case XCB_FOCUS_IN: */
		/* 	focus_in(event); */
		/* 	break; */

		default:
			/* Unknown event type, ignore it */
			fprintf(stderr, "Unknown event: %d\n",
			        event->response_type);
		}
		free(event);
		xcb_flush(xcb->conn);
	}
}

void __poco_ws_init(struct poco_ws *ws /* OUT */)
{
	ws = __poco_ws_new();

	struct poco_xcb *xcb = xcb = malloc(sizeof(struct poco_xcb));

	set_platformdata(ws, xcb);

	xcb->conn = xcb_connect(NULL, &xcb->screen);

	if (xcb_connection_has_error(xcb->conn))
		fprintf(stderr, "Can't open the default X display.\n");

	/* initialize EWMH atoms */
	xcb->ewmh = malloc(sizeof(xcb_ewmh_connection_t));
	xcb_intern_atom_cookie_t * cookie = xcb_ewmh_init_atoms(xcb->conn, xcb->ewmh);
	if (xcb_ewmh_init_atoms_replies(xcb->ewmh, cookie, NULL) == 0)
		fprintf(stderr, "Can't initialize EWMH atoms.\n");

	/* get screen information */
	/* xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(xcb->conn)).data; */
	xcb_screen_t *screen = xcb_aux_get_screen(xcb->conn, xcb->default_screen);
	if (!xcb->screen)
		fprintf(stderr, "Can't acquire the default screen.\n");
	ws->screen.width = screen->width_in_pixels;
	ws->screen.height = screen->height_in_pixels;
	xcb->root = screen->root;
	xcb->root_depth = screen->root_depth;

	/* create window for WM */
	xcb->us = xcb_generate_id(xcb->conn);
	xcb_create_window(xcb->conn, XCB_COPY_FROM_PARENT,
	                  xcb->us, xcb->root, 0, 0,
	                  ws->screen.width, ws->screen.height,
	                  0, XCB_WINDOW_CLASS_INPUT_ONLY, XCB_COPY_FROM_PARENT,
	                  0, 0);

	const char *title = "PoCo Window Manager";
	xcb_change_property(xcb->conn,
	                    XCB_PROP_MODE_REPLACE,
	                    xcb->us,
	                    XCB_ATOM_WM_NAME,
	                    XCB_ATOM_STRING,
	                    8,
	                    strlen(title),
	                    title);

	/* set supported EWMH atoms */
	xcb_atom_t net_atoms[] = {xcb->ewmh->_NET_SUPPORTED,
	                          xcb->ewmh->_NET_WORKAREA,
	                          xcb->ewmh->_NET_NUMBER_OF_DESKTOPS,
	                          xcb->ewmh->_NET_CURRENT_DESKTOP,
	                          xcb->ewmh->_NET_DESKTOP_VIEWPORT,
	                          xcb->ewmh->_NET_DESKTOP_GEOMETRY,
	                          xcb->ewmh->_NET_CLIENT_LIST,
	                          xcb->ewmh->_NET_ACTIVE_WINDOW,
	                          xcb->ewmh->_NET_WM_DESKTOP,
	                          xcb->ewmh->_NET_WM_STATE,
	                          xcb->ewmh->_NET_WM_STATE_FULLSCREEN,
	                          xcb->ewmh->_NET_WM_STATE_DEMANDS_ATTENTION,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_DESKTOP,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_NORMAL,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_DOCK,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_DIALOG,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_UTILITY,
	                          xcb->ewmh->_NET_WM_WINDOW_TYPE_TOOLBAR};

	xcb_ewmh_set_supported(xcb->ewmh, xcb->screen, LENGTH(net_atoms), net_atoms);

	/* Send hints */
	xcb_ewmh_set_active_window(xcb->ewmh, xcb->screen, XCB_WINDOW_NONE);

	xcb_ewmh_set_supporting_wm_check(xcb->ewmh, xcb->root, xcb->us);

	xcb_ewmh_set_desktop_geometry(xcb->ewmh, xcb->screen,
	                              ws->screen.width,
	                              ws->screen.height);

	xcb_ewmh_coordinates_t coord = { 0, 0 };
	xcb_ewmh_set_desktop_viewport(xcb->ewmh, xcb->screen, 1, &coord);

	xcb_ewmh_geometry_t geo = { 0, 0,
	                            ws->screen.width,
	                            ws->screen.height };
	xcb_ewmh_set_workarea(xcb->ewmh, xcb->screen, 1, &geo);

	xcb_ewmh_set_number_of_desktops(xcb->ewmh, xcb->screen, 1);

	xcb_ewmh_set_current_desktop(xcb->ewmh, xcb->screen, 0);

	xcb_ewmh_request_change_showing_desktop(xcb->ewmh, xcb->screen, 0);

	/* Register for some root window events */
	/* This is the type of value that should be nice to have from the platformdata code */
	const static uint32_t values[] = { XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
	                                   XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT };
	xcb_change_window_attributes(xcb->conn, xcb->root, XCB_CW_EVENT_MASK, values);

	/* Register event loop */
	ws->event_loop = event_loop;
}

void __poco_ws_cleanup(struct poco_ws *ws)
{
	struct poco_xcb *xcb = get_platformdata(ws);

	/* clean ewmh stuff */
	xcb_ewmh_connection_wipe(xcb->ewmh);
	free(xcb->ewmh);

	/* destroy our window */
	xcb_destroy_window(xcb->conn, xcb->us);

	/* disconnect from X */
	xcb_flush(xcb->conn);
	xcb_disconnect(xcb->conn);

	__poco_ws_free(ws);
}
