
#ifndef lua_lib_lua_utils_h
#define lua_lib_lua_utils_h

#include "livelylua.h"

void ll_error(lua_State *L, const char *print_string, const char *message);
void ll_stackdump(lua_State *L);
void ll_call(lua_State *L, const char *func, const char *sig, ...);
#endif
