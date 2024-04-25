# ---------------------
# libwebsockets
# ---------------------
set(LWS_WITH_MINIMAL_EXAMPLES 0)
set(LWS_WITHOUT_SERVER 1)
set(LWS_WITHOUT_TESTAPPS 1)
set(LWS_UNIX_SOCK 0)
set(LWS_WITH_HTTP_DIGEST_AUTH 0)
set(LWS_CTEST_INTERNET_AVAILABLE 0)
add_subdirectory(libwebsockets)

# ---------------------
# json
# ---------------------
add_subdirectory(json)
