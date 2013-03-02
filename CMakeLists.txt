
cmake_minimum_required(VERSION 2.8.8)
include(/usr/local/share/cmake/Modules/ExternalProject.cmake)

project(mytest)

set (My_VERSION_MAJOR 1)
set (My_VERSION_MINOR 0)

ExternalProject_Add(lua-src
  URL https://github.com/mirkok/liblua/archive/lua-5.2.1.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

add_subdirectory(deps/mongoose)

set(app_SRCS src/main.c src/ll_utils.c)
set(app_LUASRCS lua_src/test.lua)

ExternalProject_Get_Property(lua-src install_dir)
include_directories(${install_dir}/include deps/mongoose)
add_executable(mytest ${app_SRCS})
add_dependencies(mytest lua-src lua-src)
target_link_libraries(mytest mongoose ${install_dir}/lib/liblua.a)

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