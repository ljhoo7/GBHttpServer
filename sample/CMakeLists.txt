file(GLOB GAME_SAMPLE_SERVER_SRC server/*.cpp game/server/*.h)
file(GLOB GAME_SAMPLE_CLIENT_SRC mock_client/*.cpp game/mock_client/*.h)

add_executable(GameServerSample ${GAME_SAMPLE_SERVER_SRC})
add_executable(GameClientSample ${GAME_SAMPLE_CLIENT_SRC})
target_include_directories(GameClientSample PRIVATE $ENV{FLAT_BUFFER_INCLUDE} $ENV{BOOST_ROOT})
target_link_directories(GameClientSample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

target_include_directories(GameServerSample PRIVATE $ENV{FLAT_BUFFER_INCLUDE} $ENV{BOOST_ROOT})
target_link_directories(GameServerSample PRIVATE $ENV{BOOST_ROOT}/stage/lib)

target_link_libraries(GameServerSample PRIVATE GBServerFramework ws2_32.lib)
target_link_libraries(GameClientSample PRIVATE GBClientFramework ws2_32.lib)

set_property(TARGET GameServerSample PROPERTY CXX_STANDARD 20)
set_property(TARGET GameClientSample PROPERTY CXX_STANDARD 20)