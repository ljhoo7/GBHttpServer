file(GLOB HTTP_SAMPLE_SRC http/*.cpp http/*.h)
file(GLOB GAME_SAMPLE_SRC game/*.cpp game/*.h)

add_executable(HttpSample http/${SAMPLE_SRC})
add_executable(GameServerSample game/${SAMPLE_SRC})

target_include_directories(HttpSample PRIVATE $ENV{BOOST_ROOT})
target_link_directories(HttpSample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

target_include_directories(GameServerSample PRIVATE $ENV{BOOST_ROOT})
target_link_directories(GameServerSample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

## using boost through vcpkg + cmake not working yet
#find_package(Boost REQUIRED)

target_link_libraries(HttpSample PRIVATE GBHttpServerFramework ws2_32.lib)

set_property(TARGET HttpSample PROPERTY CXX_STANDARD 20)

target_link_libraries(GameServerSample PRIVATE GBHttpServerFramework ws2_32.lib)

set_property(TARGET GameServerSample PROPERTY CXX_STANDARD 20)