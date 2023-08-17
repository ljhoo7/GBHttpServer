file(GLOB CORE_SRC *.cpp)
file(GLOB CORE_HEADER *.h)

#set_source_files_properties(${CORE_SRC} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

add_library(GBServerFrameworkCore STATIC ${CORE_SRC} ${CORE_HEADER})

set_property(TARGET GBServerFrameworkCore PROPERTY CXX_STANDARD 20)
set_property(TARGET GBServerFrameworkCore PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBServerFrameworkCore PRIVATE _UNICODE UNICODE)