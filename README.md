Dynamic loading of dlls in 3ds Max Plugins
===

This repo demonstrates a way of loading certain functions in a dll dynamically while developing 3ds Max plugins. This avoids having to restart 3ds Max all the time.

# Limitations
The dynamically loaded functions are restricted to simple functions with C linkage.
You cannot, for instance, pass references into the functions.

# Usage
There are two files that are needed in order to enable dynamic reloading of function, as well as some changes 
to the structure of a project.
The two files are `dynamic_load.c` and `dynamic_load.h`. The former should be added among your source files 
and the latter should be included in files that want to load dynamic functions.

The dynamic functions should be defined in a separate file like so>
```
DYNAMIC_FUNC_PREFIX
int test_dynamic(int a, float b) {
	the_listener->edit_stream->printf(L"Hello %d %f\n", a, b);
	return 36;
}
```
`DYNAMIC_FUNC_PREFIX` will make sure that the function is properly exported from the dll when compiled 
dynamically.

At the top of this file, I usually also write 
```
#ifdef MAX_DYNAMIC_LOAD
#pragma optimize("", off)
#endif
```
This disables optimization when running dynamically, which makes debugging easier since 3ds Max requires you to
always compile in Release mode.

To call a dynamic function, a couple of macros are used, like so:
```
#define DYNAMIC_FUNC_ARG_TYPES int, float
#define DYNAMIC_FUNC_ARG_NAMES 654, 0.32f
	CALL_DYNAMIC_FUNC(test_dynamic,int,result)
#undef DYNAMIC_FUNC_ARG_TYPES
#undef DYNAMIC_FUNC_ARG_NAMES
//result is now an int holding the return value of the call
```
The first line defines the types of the arguments, which needs to be known when loading the function from
the DLL, the second line defines the values of the arguments, and the third line calls the actual function.
The second and third arguments to CALL_DYNAMIC_FUNC are the return type and the name of a variable that should contain the
result of the call. The variable will be declared by the macro, you should not do so yourself.

If MAX_DYNAMIC_LOAD is defined, the above macro will load the function from a dll in the same folder
as the plugin, whose name is found in DYNAMIC_DLL_NAME. Otherwise it will assume the function can be found in the
plugin dll and call it directly.

# Compilation with CMake
This is how I usually set up my CMakeFile to allow switching between dynamic loading and regular calls:
```
set (DYNAMIC_LOAD_SOURCES
	"SampleGUP_dynamic.cpp"
)

set (SRC
	"DllEntry.cpp"
	"SampleGUP.cpp"
	"dynamic_load.c"
)
```
The files listed under DYNAMIC_LOAD_SOURCES contain functions that will be included in the dynamic dll
I then add a CMake option to enable or disable dynamic loading>
```
set(3DSMAX_DYNAMIC_LOAD false CACHE STRING "Compile to dynamic dll (for development)")
```
And, finally, add the target to build the dynamic DLL if the flag is set
```
add_compile_definitions(DYNAMIC_DLL_NAME="dynamic.dll")
if(3DSMAX_DYNAMIC_LOAD)
	add_compile_definitions(MAX_DYNAMIC_LOAD)

	add_library(dynamic MODULE ${DYNAMIC_LOAD_SOURCES})
	#... Link with 3ds Max libraries etc ...
else()
	list(APPEND SRC ${DYNAMIC_LOAD_SOURCES})
endif()
```



# Sample
The sample folder Contains a sample 3ds max plugin demonstrating the dynamic loading which can be compiled with CMake. 
It is recommended to set CMAKE_INSTALL_PREFIX to `C:\Users\[USERNAME]\AppData\Roaming\Autodesk\ApplicationPlugins` and runnig cmake install with the Release build type
(Note that using the Debug build type in CMake is not supported when developing 3ds Max plugins an will crash).

The sample plugin is very simple and exposes the function `dynamic_sample_funcs.test` to maxscript.
You can call it like this:
```
dynamic_sample_funcs.test 4
```
and the plugin will print a short message to the maxscript listener window.
This can be used to test the dynamic reloading 
- change the body of the `test_func` function in Sample_dynamic.cpp and recompile, while keeping 3ds Max running.
Call the function from maxscript again and you should see the new message printed to the listener window.

# PDB file locking
If you run 3ds Max under the visual studio debugger, it will sometimes lock the .pdb for the dynamic dll and prevent it from being
rebuilt. I find that when using CMake visual studio 2019, I can just build the project again and it will work. Otherwise, one
could use a program like LockHunter to unlock the file.

Similarly, you might run into the issue that CMake fails to install the 3ds Max plugin since it's loaded in 3ds Max.
I usually get around this by defining my dynamic dll target before the main plugin target, so that is is properly installed
even if the plugin is locked.
