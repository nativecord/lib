# ---------------------
# libwebsockets
# ---------------------
set(LWS_WITH_MINIMAL_EXAMPLES OFF)
set(LWS_WITHOUT_SERVER ON)
set(LWS_WITHOUT_TESTAPPS ON)
set(LWS_WITHOUT_TEST_SERVER ON)
set(LWS_WITHOUT_TEST_SERVER_EXTPOLL ON)
set(LWS_WITHOUT_TEST_PING ON)
set(LWS_WITHOUT_TEST_CLIENT ON)
set(LWS_UNIX_SOCK OFF)
set(LWS_WITH_HTTP_DIGEST_AUTH OFF)
set(LWS_CTEST_INTERNET_AVAILABLE OFF)
add_subdirectory(libwebsockets)

# ---------------------
# json
# ---------------------
add_subdirectory(json)
