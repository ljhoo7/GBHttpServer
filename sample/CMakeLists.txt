file(GLOB SAMPLE_SRC *.cpp *.h)

add_executable(Sample ${SAMPLE_SRC})

target_include_directories(Sample PRIVATE $ENV{BOOST_ROOT})
target_link_directories(Sample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

## using boost through vcpkg + cmake not working yet
#find_package(Boost REQUIRED)

target_link_libraries(Sample PRIVATE GBHttpServerFramework ws2_32.lib)

set_property(TARGET Sample PROPERTY CXX_STANDARD 20)