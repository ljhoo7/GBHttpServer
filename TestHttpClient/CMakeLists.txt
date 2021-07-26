files(GLOB TEST_CLIENT_SRC *.cpp *.h)

add_executable(TestHttpClient ${TEST_CLIENT_SRC})