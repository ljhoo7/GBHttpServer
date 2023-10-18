file(GLOB_RECURSE CORE_SERVER_SRC server/*.cpp)
file(GLOB_RECURSE CORE_SERVER_HEADER server/*.h)

file(GLOB_RECURSE CORE_SHARED_SRC shared/*.cpp)
file(GLOB_RECURSE CORE_SHARED_HEADER shared/*.h)

file(GLOB_RECURSE CORE_CLIENT_SRC client/*.cpp)
file(GLOB_RECURSE CORE_CLIENT_HEADER client/*.h)

#set_source_files_properties(${CORE_SERVER_SRC} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

add_library(GBServerFrameworkCore STATIC 
	${CORE_SERVER_SRC} ${CORE_SERVER_HEADER}
	${CORE_SHARED_SRC} ${CORE_SHARED_HEADER}
)
add_library(GBClientFrameworkCore STATIC
	${CORE_CLIENT_SRC} ${CORE_CLIENT_HEADER}
	${CORE_SHARED_SRC} ${CORE_SHARED_HEADER}
)

set_property(TARGET GBServerFrameworkCore PROPERTY CXX_STANDARD 20)
set_property(TARGET GBServerFrameworkCore PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBServerFrameworkCore PRIVATE _UNICODE UNICODE)
target_include_directories(GBServerFrameworkCore PRIVATE $ENV{BOOST_ROOT})

set_property(TARGET GBClientFrameworkCore PROPERTY CXX_STANDARD 20)
set_property(TARGET GBClientFrameworkCore PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBClientFrameworkCore PRIVATE _UNICODE UNICODE)
target_include_directories(GBClientFrameworkCore PRIVATE $ENV{BOOST_ROOT})