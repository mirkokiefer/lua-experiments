
cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

project(mytest)

set (My_VERSION_MAJOR 1)
set (My_VERSION_MINOR 0)

include(deps/luajit.cmake)
include(deps/mongoose.cmake)

set(app_SRCS src/simplemain.c)
set(app_LUASRCS lua_src/test.lua)

add_executable(mytest ${app_SRCS})

find_package (Threads)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(build_opts -pagezero_size10000 -image_base100000000)
endif()

target_link_libraries(mytest
  mongoose lua
  ${CMAKE_THREAD_LIBS_INIT} m ${CMAKE_DL_LIBS}
  ${build_opts}
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
