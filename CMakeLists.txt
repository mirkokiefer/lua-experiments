
cmake_minimum_required(VERSION 2.8)

project(mytest)

set (My_VERSION_MAJOR 1)
set (My_VERSION_MINOR 0)

add_subdirectory(deps/luajit)
include_directories(deps/luajit/src deps/mongoose)

add_subdirectory(deps/mongoose)

set(app_SRCS src/main.c src/ll_utils.c)
set(app_LUASRCS lua_src/test.lua)
add_executable(mytest ${app_SRCS})

add_subdirectory(lua_src)

# this is just a hotfix as only since cmake 2.8.8 you can use imported libraries from subdirectories:
add_library(mylua STATIC IMPORTED)
set_property(TARGET mylua PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/deps/luajit/lua/lib/libluajit-5.1.a)
add_dependencies(mylua compile_lua)

target_link_libraries(mytest
  ${M_LIB} ${CMAKE_DL_LIBS} mylua
  mongoose
  -pagezero_size10000 -image_base100000000
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