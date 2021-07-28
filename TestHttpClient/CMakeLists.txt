file(GLOB TEST_CLIENT_SRC *.cpp *.h)

add_executable(TestHttpClient ${TEST_CLIENT_SRC})

target_include_directories(TestHttpClient PRIVATE ./../lib/curl/include)

target_link_libraries(TestHttpClient PRIVATE 
	./../lib/curl/lib/$<CONFIG>/libcurl
	ws2_32
	winmm
	wldap32
)

target_compile_definitions(TestHttpClient PRIVATE CURL_STATICLIB)