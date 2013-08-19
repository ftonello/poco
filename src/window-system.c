#include "window-system.h"

#include <malloc.h>
#include <assert.h>

struct poco_ws * __poco_ws_new()
{
	struct poco_ws *ws;
	if (!(ws = calloc(1, sizeof(struct poco_ws))))
		return NULL;
	return ws;
}

void __poco_ws_free(struct poco_ws *ws)
{
	assert(ws);

	free(ws);
}
