cmake_minimum_required (VERSION 3.18.1)

project(GBHttpServer)

file(GLOB GBHttpServer_SRC src/*.cpp)

file(GLOB GBHttpServer_HEADER include/*.h)

add_executable(GBHttpServer ${GBHttpServer_SRC} ${GBHttpServer_HEADER})

set_property(TARGET GBHttpServer PROPERTY CXX_STANDARD 20)

add_subdirectory(lib/GBString)

add_dependencies(GBHttpServer GBString)