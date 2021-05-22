cmake_minimum_required (VERSION 3.18.1)

project(GBHttpServer)

file(GLOB GBHttpServerFramework_SRC src/*.cpp)

file(GLOB ROUTER_FILES src/*Router.cpp include/*Router.h src/GBPathSegment.cpp include/GBPathSegment.h)
file(GLOB METHOD_FILES src/*Method.cpp include/*Method.h)

file(GLOB GBHttpServerFramework_HEADER include/*.h)

source_group("Router" FILES ${ROUTER_FILES})
source_group("Method" FILES ${METHOD_FILES})

add_executable(GBHttpServerFramework ${GBHttpServerFramework_SRC} ${GBHttpServerFramework_HEADER})

set_property(TARGET GBHttpServerFramework PROPERTY CXX_STANDARD 20)

target_compile_definitions(GBHttpServerFramework PRIVATE _UNICODE UNICODE)

target_link_libraries(GBHttpServerFramework PRIVATE ws2_32.lib)