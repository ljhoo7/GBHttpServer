file(GLOB_RECURSE SERVER_SRC server/*.cpp)
file(GLOB_RECURSE SERVER_HEADER server/*.h)

file(GLOB_RECURSE SHARED_SRC shared/*.cpp)
file(GLOB_RECURSE SHARED_HEADER shared/*.h)

file(GLOB_RECURSE CLIENT_SRC client/*.cpp)
file(GLOB_RECURSE CLIENT_HEADER client/*.h)

#set_source_files_properties(${SERVER_SRC} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

add_library(GBServerFramework STATIC 
	${SERVER_SRC} ${SERVER_HEADER}
	${SHARED_SRC} ${SHARED_HEADER}
)
add_library(GBClientFramework STATIC
	${CLIENT_SRC} ${CLIENT_HEADER}
	${SHARED_SRC} ${SHARED_HEADER}
)

set_property(TARGET GBServerFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBServerFramework PRIVATE _UNICODE UNICODE)
target_include_directories(GBServerFramework PRIVATE $ENV{FLAT_BUFFER_INCLUDE} $ENV{BOOST_ROOT})

set_property(TARGET GBClientFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBClientFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBClientFramework PRIVATE _UNICODE UNICODE)
target_include_directories(GBClientFramework PRIVATE $ENV{FLAT_BUFFER_INCLUDE}  $ENV{BOOST_ROOT})