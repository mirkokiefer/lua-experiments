
cmake_minimum_required(VERSION 2.8)

set(lua_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test.lua)
set(lua_csrcs ${CMAKE_CURRENT_BINARY_DIR}/test.c)

add_custom_command(
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT ${lua_csrcs}
    COMMAND ../deps/luajit/src/luajit -b ${lua_srcs} ${lua_csrcs}
)

add_custom_target(compile_lua_strings DEPENDS ${lua_csrcs})