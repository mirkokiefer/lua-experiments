
cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

project(mytest)

set (My_VERSION_MAJOR 1)
set (My_VERSION_MINOR 0)

include(deps/lua.cmake)

ExternalProject_Add(project_mongoose
  URL https://github.com/mirkok/mongoose/archive/3.7-cmake.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/mongoose
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(project_mongoose install_dir)
set (mongoose_dir ${install_dir})


set(app_SRCS src/main.c src/ll_utils.c)
set(app_LUASRCS lua_src/test.lua)

include_directories(${mongoose_dir}/include)
add_executable(mytest ${app_SRCS})
add_dependencies(mytest project_mongoose)
find_package (Threads)
target_link_libraries(mytest
  ${mongoose_dir}/lib/libmongoose.a lua
  ${CMAKE_THREAD_LIBS_INIT} m ${CMAKE_DL_LIBS}
)

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
