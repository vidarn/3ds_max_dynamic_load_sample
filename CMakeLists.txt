cmake_minimum_required(VERSION 3.12)

project(DynamicLoad3dsMax)

# Variables
set(3DSMAX_VERSIONS "2019;2020" CACHE STRING "3dsMax versions (year)")
set(3DSMAX_DYNAMIC_LOAD true CACHE STRING "Compile to dynamic dll (for development)")


set (DYNAMIC_LOAD_SOURCES
	"sample/Sample_dynamic.cpp"
)

set (SRC
	"sample/DllEntry.cpp"
	"sample/Sample.cpp"
	"dynamic_load.c"
)


foreach(3DSMAX_VERSION ${3DSMAX_VERSIONS})

	set(3DSMAXSDK_ROOT${3DSMAX_VERSION} $ENV{ADSK_3DSMAX_SDK_${3DSMAX_VERSION}} )
	set(3DSMAXSDK_INCLUDE${3DSMAX_VERSION} "${3DSMAXSDK_ROOT${3DSMAX_VERSION}}/include/" )
	set(3DSMAXSDK_LIB${3DSMAX_VERSION} "${3DSMAXSDK_ROOT${3DSMAX_VERSION}}/lib/x64/Release/" )

	ADD_DEFINITIONS(-DUNICODE)
	ADD_DEFINITIONS(-D_UNICODE)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS) # supress MSVC warn C4996

	set(INCLUDES 
		${CMAKE_CURRENT_SOURCE_DIR}
		${3DSMAXSDK_INCLUDE${3DSMAX_VERSION}}
	)

	set(LIBS 
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}Paramblk2.lib"
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}core.lib"
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}geom.lib"
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}maxutil.lib"
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}Maxscrpt.lib"
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}bmm.lib"
		"${3DSMAXSDK_LIB${3DSMAX_VERSION}}gup.lib"
		"comctl32"
		"odbc32"
		"odbccp32"
	)
	set(DEST DynamicLoad3dsMax)

add_compile_definitions(DYNAMIC_DLL_NAME="DynamicLoad3dsMax${3DSMAX_VERSION}_dynamic.dll")
if(3DSMAX_DYNAMIC_LOAD)
	add_compile_definitions(MAX_DYNAMIC_LOAD)

	add_library(DynamicLoad3dsMax${3DSMAX_VERSION}_dynamic MODULE ${DYNAMIC_LOAD_SOURCES})
	target_link_libraries(DynamicLoad3dsMax${3DSMAX_VERSION}_dynamic ${LIBS})
	target_include_directories(DynamicLoad3dsMax${3DSMAX_VERSION}_dynamic PUBLIC ${INCLUDES})

	string(RANDOM PDB_ID)
	set_target_properties(DynamicLoad3dsMax${3DSMAX_VERSION}_dynamic PROPERTIES PDB_NAME "dynamic_${3DSMAX_VERSION}_${PDB_ID}")

	install(TARGETS DynamicLoad3dsMax${3DSMAX_VERSION}_dynamic DESTINATION DynamicLoad3dsMax)
else()
	list(APPEND SRC ${DYNAMIC_LOAD_SOURCES})
endif()

	add_library(DynamicLoad3dsMax${3DSMAX_VERSION} MODULE ${SRC})
	target_link_libraries (DynamicLoad3dsMax${3DSMAX_VERSION} ${LIBS})
	target_include_directories ( DynamicLoad3dsMax${3DSMAX_VERSION} PUBLIC ${INCLUDES})

	set_target_properties(DynamicLoad3dsMax${3DSMAX_VERSION} PROPERTIES SUFFIX ".gup")
	target_compile_definitions(DynamicLoad3dsMax${3DSMAX_VERSION} PUBLIC DL_MAX_VERSION="${3DSMAX_VERSION}")
	install(TARGETS DynamicLoad3dsMax${3DSMAX_VERSION} DESTINATION DynamicLoad3dsMax)

endforeach(3DSMAX_VERSION)

install(FILES "sample/PackageContents.xml" DESTINATION DynamicLoad3dsMax)
