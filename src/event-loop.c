#include "event-loop.h"
#include "window-system.h"

void __poco_event_loop(struct poco_ws *ws)
{
	ws->event_loop(ws);
}
