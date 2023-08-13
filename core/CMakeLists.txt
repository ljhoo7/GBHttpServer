file(GLOB CORE_SRC *.cpp)
file(GLOB CORE_HEADER *.h)

set_source_files_properties(${CORE_SRC} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

add_library(GBServerFramework STATIC ${CORE_SRC} ${CORE_HEADER})

target_include_directories(GBServerFramework PRIVATE $ENV{BOOST_ROOT})

set_property(TARGET GBServerFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBServerFramework PRIVATE _UNICODE UNICODE)