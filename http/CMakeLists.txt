file(GLOB HTTP_SRC src/*.cpp)

file(GLOB ROUTER_FILES src/*Router.cpp include/*Router.h src/GBPathSegment.cpp include/GBPathSegment.h)
file(GLOB READER_FILES src/*Reader.cpp include/*Reader.h)
file(GLOB WRITER_FILES src/*Writer.cpp include/*Writer.h)

file(GLOB HTTP_HEADER include/*.h)

source_group("Router" FILES ${ROUTER_FILES})
source_group("Reader" FILES ${READER_FILES})
source_group("Writer" FILES ${WRITER_FILES})

add_library(GBHttpServerFramework STATIC ${HTTP_SRC} ${HTTP_HEADER})

target_include_directories(GBHttpServerFramework PRIVATE $ENV{BOOST_ROOT})
target_link_directories(GBHttpServerFramework PRIVATE $ENV{BOOST_ROOT}/stage/lib)

set_property(TARGET GBHttpServerFramework PROPERTY CXX_STANDARD 20)
set_property(TARGET GBHttpServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)

target_compile_definitions(GBHttpServerFramework PRIVATE _UNICODE UNICODE)


