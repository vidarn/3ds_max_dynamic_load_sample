#include "max.h"

#define MYDLLEXPORT __declspec(dllexport)

HINSTANCE hInstance;
#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	// Hang on to this DLL's instance handle.
	hInstance = hinstDLL;
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return(TRUE);
}

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
MYDLLEXPORT const TCHAR* LibDescription(void) {
#pragma EXPORT
	return _T("3ds Max Dynamic load sample");
}

// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
MYDLLEXPORT int LibNumberClasses(void) {
#pragma EXPORT
	return 0;
}

// This function returns the number of plug-in classes this DLL
MYDLLEXPORT ClassDesc* LibClassDesc(int i) {
#pragma EXPORT
	return 0;
}

// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
MYDLLEXPORT ULONG LibVersion() {
#pragma EXPORT
	return VERSION_3DSMAX;
}
