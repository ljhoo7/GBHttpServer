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