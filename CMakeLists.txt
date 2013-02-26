
cmake_minimum_required(VERSION 2.8)

project(mytest)

set (My_VERSION_MAJOR 1)
set (My_VERSION_MINOR 0)

add_subdirectory(deps/lua)

set(app_SRCS src/main.c src/ll_utils.c)
set(app_LUASRCS lua_src/test.lua)

include_directories(deps/lua/src)

add_executable(mytest ${app_SRCS})
target_link_libraries(mytest lua)

 #FILE(MAKE_DIRECTORY logs) 
install (TARGETS mytest DESTINATION bin)
install (DIRECTORY lua_src/ DESTINATION bin/livelylua_lua)

add_test (AppRuns mytest)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${My_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${My_VERSION_MINOR}")
include (CPack)