file(GLOB GAME_SRC *.cpp)
file(GLOB GAME_HEADER *.h)

#set_source_files_properties(${GAME_SRC} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

add_library(GBGameServerFramework STATIC ${GAME_SRC} ${GAME_HEADER})

target_include_directories (GBGameServerFramework PRIVATE "../flatbuffers/include")
target_include_directories (GBGameServerFramework PRIVATE $ENV{BOOST_ROOT})

set_property(TARGET GBGameServerFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBGameServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBGameServerFramework PRIVATE _UNICODE UNICODE)