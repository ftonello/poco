#include "event-loop.h"
#include "window-system.h"
#include "utils.h"

#include <assert.h>

void __poco_event_loop(struct poco_ws *ws)
{
	assert(ws);

	if (ws->event_loop)
		ws->event_loop(ws);
	else
		poco_log_fatal("Error: No event loop registered");
}
