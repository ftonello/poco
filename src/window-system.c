#include "window-system.h"

#include <assert.h>

struct poco_ws * __poco_ws_new()
{
	poco_ws *ws;
	if (!(ws = malloc(sizeof(struct poco_ws))))
		return NULL;
	return ws;
}

void __poco_ws_free(struct poco_ws *ws)
{
	assert(ws);

	free(ws);
}
