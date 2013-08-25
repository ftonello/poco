#include "platformdata.h"
#include "window-system.h"
#include "event-loop.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
	/* poco_ws shouldnt be the main struct */
	struct poco_ws *ws = NULL;

	__poco_ws_init(&ws);

	__poco_load_platformdata(ws);

	__poco_event_loop(ws);

	__poco_ws_cleanup(ws);

	return 0;
}
