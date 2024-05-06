# ---------------------
# json
# ---------------------
add_subdirectory(json)

# ---------------------
# libhv
# ---------------------
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WITH_HTTP_SERVER OFF CACHE BOOL "" FORCE)
set(WITH_MQTT OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED ON CACHE BOOL "" FORCE)
set(WITH_OPENSSL ON CACHE BOOL "" FORCE)
add_subdirectory(libhv)
# target_compile_options(hv PRIVATE -w)
get_target_property(hv_include_dirs hv INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(hv PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${hv_include_dirs}")
