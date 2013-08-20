#include "platformdata.h"
#include "window-system.h"
#include "event-loop.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct poco_ws *ws = __poco_ws_init();

	__poco_load_platformdata(ws);

	__poco_event_loop(ws);

	__poco_ws_cleanup(ws);

	return 0;
}
