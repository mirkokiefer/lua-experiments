
cmake_minimum_required(VERSION 2.8)
include(/usr/local/share/cmake/Modules/ExternalProject.cmake)

project(mytest)

set (My_VERSION_MAJOR 1)
set (My_VERSION_MINOR 0)

ExternalProject_Add(project_lua
  URL https://github.com/mirkok/liblua/archive/lua-5.2.1.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Add(project_mongoose
  URL https://github.com/mirkok/mongoose/archive/3.7-cmake.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/mongoose
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(project_lua install_dir)
set (lua_dir ${install_dir})
ExternalProject_Get_Property(project_mongoose install_dir)
set (mongoose_dir ${install_dir})


set(app_SRCS src/main.c src/ll_utils.c)
set(app_LUASRCS project_lua/test.lua)

include_directories(${lua_dir}/include ${mongoose_dir}/include)
add_executable(mytest ${app_SRCS})
add_dependencies(mytest project_lua project_mongoose)
target_link_libraries(mytest ${mongoose_dir}/lib/libmongoose.a ${lua_dir}/lib/liblua.a)

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
