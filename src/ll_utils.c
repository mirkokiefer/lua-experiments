
#include "ll_utils.h"

void ll_error(lua_State *L, const char *print_string, const char *message) {
  printf(print_string, message);
}

void ll_stackdump(lua_State *L) {
  int i;
  int top = lua_gettop(L);
  for(i=1;i<=top;i++){ /*repeatforeachlevel*/
    int t = lua_type(L, i);
    switch (t) {
      case LUA_TSTRING: {  /* strings */
        printf("’%s’", lua_tostring(L, i));
        break;
      }
      case LUA_TBOOLEAN: {  /* booleans */
        printf(lua_toboolean(L, i) ? "true" : "false");
        break; }
      case LUA_TNUMBER: {  /* numbers */
        printf("%g", lua_tonumber(L, i));
        break;
      }
      default: {  /* other values */
        printf("%s", lua_typename(L, t));
        break; }
    }
    printf("  ");  /* put a separator */
  }
  printf("\n");  /* end the listing */
}

void ll_call(lua_State *L, const char *func, const char *sig, ...) {
  va_list vl;
  int narg, nres;  /* number of arguments and results */
  va_start(vl, sig);
  lua_getglobal(L, func);  /* push function */
  
  for (narg = 0; *sig; narg++) {  /* repeat for each argument */
    /* check stack space */
    luaL_checkstack(L, 1, "too many arguments");
    switch (*sig++) {
      case 'd':  /* double argument */
        lua_pushnumber(L, va_arg(vl, double));
        break;
      case 'i':  /* int argument */
        lua_pushinteger(L, va_arg(vl, int));
        break;
      case 's':  /* string argument */
        lua_pushstring(L, va_arg(vl, char *));
        break;
      case '>':  /* end of arguments */
        goto endargs;
      default:
        ll_error(L, "invalid signature", sig);
    }
  }
  endargs:
  
  nres = strlen(sig); /* number of expected results */
  /* do the call */
  if (lua_pcall(L, narg, nres, 0) != 0)  /* do the call */
    ll_error(L, "error calling function: %s", lua_tostring(L, -1));
  
  nres = -nres;  /* stack index of first result */
  while (*sig) {  /* repeat for each result */
    switch (*sig++) {
      case 'd':  /* double result */
        if (!lua_isnumber(L, nres))
          ll_error(L, "wrong result type", NULL);
        *va_arg(vl, double*) = lua_tonumber(L, nres);
        break;
      case 'i':  /* int result */
        if (!lua_isnumber(L, nres))
          ll_error(L, "wrong result type", NULL);
        *va_arg(vl, int *) = lua_tointeger(L, nres);
        break;
      case 's':  /* string result */
        if (!lua_isstring(L, nres))
          ll_error(L, "wrong result type", NULL);
        *va_arg(vl, const char **) = lua_tostring(L, nres);
        break;
      default:
        ll_error(L, "invalid signature", sig);
    }
    nres++;
  }
  
  va_end(vl);
}