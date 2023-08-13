file(GLOB HTTP_SRC *.cpp)
file(GLOB HTTP_HEADER *.h)

add_library(GBHttpServerFramework STATIC ${HTTP_SRC} ${HTTP_HEADER})

target_include_directories(GBHttpServerFramework PRIVATE $ENV{BOOST_ROOT})

set_property(TARGET GBHttpServerFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBHttpServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBHttpServerFramework PRIVATE _UNICODE UNICODE)


