#include <max.h>
#include <impapi.h>
#include <maxscript/maxscript.h>
#include <maxscript/util/listener.h>

#include "dynamic_load.h"

#ifdef MAX_DYNAMIC_LOAD
#pragma optimize("", off)
#endif

DYNAMIC_FUNC_PREFIX
int test_func(int a, float b) {
	the_listener->edit_stream->printf(L"Hello from dynamic function: %d (%f)\n", a, b);
	return 55;
}
