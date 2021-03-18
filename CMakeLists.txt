cmake_minimum_required(3.8.0)

file(GLOB CORE_SRC ./src/*.cpp)

file(GLOB CORE_HEADER ./include/*.h)

add_library(CORE STATIC CORE_SRC CORE_HEADER)