# ---------------------
# options
# ---------------------
option(NATIVECORD_LIB_SHARED "Build as shared library" OFF)
option(NATIVECORD_LIB_TEST "Build the test solution" OFF)

# ---------------------
# cmake settings
# ---------------------
cmake_minimum_required(VERSION 3.29.2) # Find_OpenSSL broke on Windows on prev
                                       # versions
set(CMAKE_CXX_STANDARD 20)

# ---------------------
# project
# ---------------------
project(nativecord-lib LANGUAGES CXX)
add_subdirectory(vendor)

# ---------------------
# warnings
# ---------------------
add_library(nativecord-warnings INTERFACE)
if(MSVC)
  target_compile_options(nativecord-warnings INTERFACE /W4 /WX)
else()
  target_compile_options(nativecord-warnings INTERFACE -Wall -Wextra -Werror)
endif()

# ---------------------
# library
# ---------------------
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS "src/lib/*.cpp" "src/lib/*.h")

if(NATIVECORD_LIB_SHARED)
  add_library(nativecord-lib SHARED ${LIB_SOURCES})
  target_compile_definitions(nativecord-lib PUBLIC ADDS NATIVECORD_LIB_SHARED)
  if(MSVC)
    target_compile_options(nativecord-lib PRIVATE /DLL)
  endif()
else()
  add_library(nativecord-lib STATIC ${LIB_SOURCES})
endif()

target_include_directories(nativecord-lib PUBLIC "src/lib")
target_link_libraries(
  nativecord-lib PRIVATE nativecord-warnings websockets
                         ${LIBWEBSOCKETS_DEP_LIBS} nlohmann_json)
set_target_properties(nativecord-lib PROPERTIES USE_FOLDERS ON)

# ---------------------
# test project
# ---------------------
if(NATIVECORD_LIB_TEST)
  file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "src/test/*.cpp"
       "src/test/*.h")
  add_executable(nativecord-test ${TEST_SOURCES})
  target_link_libraries(
    nativecord-test PRIVATE nativecord-lib nativecord-warnings nlohmann_json)
  set_target_properties(nativecord-test PROPERTIES USE_FOLDERS ON)

  if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                 PROPERTY VS_STARTUP_PROJECT nativecord-test)
    target_compile_definitions(nativecord-test PRIVATE ADDS WIN32_LEAN_AND_MEAN)

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86")
      target_compile_definitions(nativecord-test PRIVATE ADDS _X86_)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64" OR CMAKE_SYSTEM_PROCESSOR
                                                     MATCHES "x86_64")
      target_compile_definitions(nativecord-test PRIVATE ADDS _AMD64_)
    else()
      message(
        FATAL_ERROR "Unimplemented architecture (${CMAKE_SYSTEM_PROCESSOR})")
    endif()
  endif()
endif()

# ---------------------
# folder grouping
# ---------------------
function(group_files sources)
  foreach(FILE ${sources})
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" GROUP "${PARENT_DIR}"
    )# remove full dir
    string(REGEX MATCH "[(A-z)|(a-z)|(0-9)]+/[(A-z)|(a-z)|(0-9)]+" GROUPNAME
                 ${GROUP}) # get the first two folders from dir
    string(LENGTH ${GROUPNAME} GROUPLEN) # get project name len
    string(SUBSTRING ${GROUP} ${GROUPLEN} -1 GROUP) # remove project name from
                                                    # dir
    source_group("${GROUP}" FILES "${FILE}") # group file
  endforeach()
endfunction()

group_files("${LIB_SOURCES}")
if(NATIVECORD_LIB_TEST)
  group_files("${TEST_SOURCES}")
endif()
