#include "window-system.h"

#include <malloc.h>
#include <assert.h>

#include "utils.h"

void __poco_ws_init(struct poco_ws **ws)
{
	if (!(*ws = calloc(1, sizeof(struct poco_ws))))
		poco_log_fatal("Could not allocate poco_ws structure.");

	__poco_ws_platform_init(*ws);
}

void __poco_ws_cleanup(struct poco_ws *ws)
{
	assert(ws);

	__poco_ws_platform_cleanup(ws);

	free(ws);
}
