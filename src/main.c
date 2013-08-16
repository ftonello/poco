#include "platformdata.h"
#include "window-system.h"
#include "event-loop.h"

int main(int argc, char *argv[])
{
	__poco_load_platformdata();

	__poco_ws_init();

	__poco_event_loop();

	__poco_ws_cleanup();

	return 0;
}
