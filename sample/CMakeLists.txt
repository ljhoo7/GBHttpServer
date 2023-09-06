#file(GLOB HTTP_SAMPLE_SRC http/*.cpp http/*.h)
file(GLOB GAME_SAMPLE_SERVER_SRC game/server/*.cpp game/server/*.h)
file(GLOB GAME_SAMPLE_CLIENT_SRC game/client/*.cpp game/client/*.h)

#add_executable(HttpSample ${HTTP_SAMPLE_SRC})
add_executable(GameServerSample ${GAME_SAMPLE_SERVER_SRC})
add_executable(GameClientSample ${GAME_SAMPLE_CLIENT_SRC})

#target_include_directories(HttpSample PRIVATE $ENV{BOOST_ROOT})
#target_link_directories(HttpSample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

target_include_directories(GameServerSample PRIVATE $ENV{FLAT_BUFFER_INCLUDE} $ENV{BOOST_ROOT})
target_link_directories(GameServerSample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

## using boost through vcpkg + cmake not working yet
#find_package(Boost REQUIRED)

#target_link_libraries(HttpSample PRIVATE GBServerFrameworkCore GBHttpServerFramework ws2_32.lib)
#set_property(TARGET HttpSample PROPERTY CXX_STANDARD 20)

target_link_libraries(GameServerSample PRIVATE GBServerFrameworkCore GBGameServerFramework ws2_32.lib)
target_link_libraries(GameClientSample PRIVATE GBServerFrameworkCore GBGameServerFramework ws2_32.lib)

set_property(TARGET GameServerSample PROPERTY CXX_STANDARD 20)
set_property(TARGET GameClientSample PROPERTY CXX_STANDARD 20)