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


##################

file(GLOB GAME_SERVER_SRC server/*.cpp)
file(GLOB GAME_SERVER_HEADER server/*.h)

file(GLOB GAME_SHARED_SRC shared/*.cpp)
file(GLOB GAME_SHARED_HEADER shared/*.h)

file(GLOB GAME_CLIENT_SRC client/*.cpp)
file(GLOB GAME_CLIENT_HEADER client/*.h)

#set_source_files_properties(${GAME_SRC} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

add_library(GBGameServerFramework STATIC 
    ${GAME_SERVER_SRC} ${GAME_SERVER_HEADER}
    ${GAME_SHARED_SRC} ${GAME_SHARED_HEADER}
)
add_library(GBGameClientFramework STATIC
    ${GAME_CLIENT_SRC} ${GAME_CLIENT_HEADER}
    ${GAME_SHARED_SRC} ${GAME_SHARED_HEADER}
)

target_include_directories (GBGameServerFramework PUBLIC $ENV{FLAT_BUFFER_INCLUDE} $ENV{BOOST_ROOT})

set_property(TARGET GBGameServerFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBGameServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBGameServerFramework PRIVATE _UNICODE UNICODE)

target_include_directories (GBGameClientFramework PUBLIC $ENV{FLAT_BUFFER_INCLUDE} $ENV{BOOST_ROOT})

set_property(TARGET GBGameClientFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBGameClientFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBGameClientFramework PRIVATE _UNICODE UNICODE)