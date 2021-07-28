cmake_minimum_required (VERSION 3.17.0)

project(GBHttpServerFramework)

# 이 CMake 함수는 curl/CMakeLists.txt에서 curl/lib/CMakeLists.txt를 위해서 복사해온 것이다.
# "Makefile.inc"를 include 시키기 위한 다소 ugly한 방법이다.
function(transform_makefile_inc INPUT_FILE OUTPUT_FILE)
  file(READ ${INPUT_FILE} MAKEFILE_INC_TEXT)
  string(REPLACE "$(top_srcdir)"   "\${CURL_SOURCE_DIR}" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})
  string(REPLACE "$(top_builddir)" "\${CURL_BINARY_DIR}" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})

  string(REGEX REPLACE "\\\\\n" "!π!α!" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})
  string(REGEX REPLACE "([a-zA-Z_][a-zA-Z0-9_]*)[\t ]*=[\t ]*([^\n]*)" "SET(\\1 \\2)" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})
  string(REPLACE "!π!α!" "\n" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})

  string(REGEX REPLACE "\\$\\(([a-zA-Z_][a-zA-Z0-9_]*)\\)" "\${\\1}" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})    # Replace $() with ${}
  string(REGEX REPLACE "@([a-zA-Z_][a-zA-Z0-9_]*)@" "\${\\1}" MAKEFILE_INC_TEXT ${MAKEFILE_INC_TEXT})    # Replace @@ with ${}, even if that may not be read by CMake scripts.
  file(WRITE ${OUTPUT_FILE} ${MAKEFILE_INC_TEXT})
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${INPUT_FILE}")
endfunction()

file(GLOB GBHttpServerFramework_SRC src/*.cpp)

file(GLOB ROUTER_FILES src/*Router.cpp include/*Router.h src/GBPathSegment.cpp include/GBPathSegment.h)
file(GLOB METHOD_FILES src/*Method.cpp include/*Method.h)
file(GLOB READER_FILES src/*Reader.cpp include/*Reader.h)
file(GLOB WRITER_FILES src/*Writer.cpp include/*Writer.h)

file(GLOB GBHttpServerFramework_HEADER include/*.h)

source_group("Router" FILES ${ROUTER_FILES})
source_group("Method" FILES ${METHOD_FILES})
source_group("Reader" FILES ${READER_FILES})
source_group("Writer" FILES ${WRITER_FILES})

add_library(GBHttpServerFramework STATIC ${GBHttpServerFramework_SRC} ${GBHttpServerFramework_HEADER})

set_property(TARGET GBHttpServerFramework PROPERTY CXX_STANDARD 20)

target_compile_definitions(GBHttpServerFramework PRIVATE _UNICODE UNICODE)

#####################################

file(GLOB SAMPLE_SRC sample/*.cpp sample/*.h)

add_executable(Sample ${SAMPLE_SRC})

set_property(TARGET GBHttpServerFramework PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_link_libraries(Sample PRIVATE GBHttpServerFramework ws2_32.lib)

set_property(TARGET Sample PROPERTY CXX_STANDARD 20)

####################################

add_subdirectory(lib/curl/lib)
add_subdirectory(TestHttpClient)

include_external_msproject(UnitTester UnitTester/UnitTester.csproj PLATFORM "Any CPU")

add_dependencies(UnitTester GBHttpServerFramework Sample)

add_dependencies(TestHttpClient libcurl)

set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT UnitTester)

