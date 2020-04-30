#include <stdlib.h>
#include <Windows.h>
#include <max.h>
#include <gup.h>
#include <impapi.h>
#include <maxscript/maxscript.h>
#include <maxscript/util/listener.h>

#include "Sample.h"
#include "dynamic_load.h"

class FuncInterface : public  FPInterfaceDesc
{
public:
	FuncInterface() : FPInterfaceDesc(
		FUNC_INTERFACE,
		_T("dynamic_sample_funcs"),
		0,
		nullptr,
		FP_CORE,
		p_end) {
		AppendFunction(
			func_test, _T("test"), 0, TYPE_VOID, 0, 1,
			_T("blaj"), 0, TYPE_INT,
			p_end
		);
	}

    BEGIN_FUNCTION_MAP
        VFN_1(func_test, Test, TYPE_INT);
    END_FUNCTION_MAP

	void Test(int blaj) {
#define DYNAMIC_FUNC_ARG_TYPES int, float
#define DYNAMIC_FUNC_ARG_NAMES blaj, 0.32f
	CALL_DYNAMIC_FUNC(test_func,int,result)
#undef DYNAMIC_FUNC_ARG_TYPES
#undef DYNAMIC_FUNC_ARG_NAMES
	the_listener->edit_stream->printf(L"Result: %d\n", result);
	}

};

static FuncInterface g_func_interface;
