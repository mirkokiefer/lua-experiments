
#include "livelylua.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>

#include "../deps/mongoose/mongoose.h"

static int begin_request_handler(struct mg_connection *conn) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  char content[100];

  // Prepare the message we're going to send
  int content_length = snprintf(content, sizeof(content),
                                "Hello from mongoose! URI: %s",
                                request_info->uri);

  // Send HTTP reply to the client
  mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "\r\n"
            "%s",
            content_length, content);

  // Returning non-zero tells mongoose that our function has replied to
  // the client, and mongoose should not send client any more data.
  return 1;
}

void test_mongoose() {
  struct mg_context *ctx;
  struct mg_callbacks callbacks;

  // List of options. Last element must be NULL.
  const char *options[] = {"listening_ports", "8080", NULL};

  // Prepare callbacks structure. We have only one callback, the rest are NULL.
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;

  // Start the web server.
  ctx = mg_start(&callbacks, NULL, options);

  // Wait until user hits "enter". Server is running in separate thread.
  // Navigating to http://localhost:8080 will invoke begin_request_handler().
  getchar();

  // Stop the server.
  mg_stop(ctx);
}

void test_read_person_table(lua_State *L, char *var) {
  const char *type;
  int age;
  lua_getglobal(L, var);
  if (!lua_istable(L, -1))
    ll_error(L, "’%s’ is not a table", var);
  
  lua_pushstring(L, "type");
  lua_gettable(L, -2);
  if (lua_isstring(L, -1)) {
    type = lua_tostring(L, -1);
  }
  printf("table %s:\n", var);
  printf("type = %s\n", type);
  lua_pop(L, 1);
  
  lua_pushstring(L, "age");
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1)) {
    age = lua_tointeger(L, -1);
  }
  printf("age = %i\n", age);
  lua_pop(L, 2);
}

void test_read_double(lua_State *L) {
  lua_getglobal(L, "test");
  double a;
  if (lua_isstring(L, -1)) {
    a = lua_tonumber(L, -1);
  }
  printf("test = %f\n", a);
  lua_pop(L, 1);
}

void test_globals_write(lua_State *L) {
  lua_newtable(L);
  
  lua_pushstring(L, "type");
  lua_pushstring(L, "person");
  lua_settable(L, -3);
  
  lua_pushinteger(L, 20);
  lua_setfield(L, -2, "age"); // same as settable except it doesnt require key on stack
  
  lua_setglobal(L, "jim");
}

void test_arrays(lua_State *L) {
  lua_newtable(L);
  lua_pushstring(L, "string1");
  lua_rawseti(L, -2, 1);
  lua_pushstring(L, "string2");
  lua_rawseti(L, -2, 2);
  lua_setglobal(L, "someArray");
}

double test_call_func(lua_State *L, char *function, double a, double b) {
  lua_getglobal(L, function);
  lua_pushnumber(L, a);
  lua_pushnumber(L, b);
  if (lua_pcall(L, 2, 1, 0) != 0)
    ll_error(L, "error running function ’mult’: %s",
          lua_tostring(L, -1));
  if (!lua_isnumber(L, -1))
    ll_error(L, "function ’mult’ must return a number", NULL);
  double result = lua_tonumber(L, -1);
  lua_pop(L, 1);
  return result;
}

static int test_cmult(lua_State *L) {
  double a = lua_tonumber(L, 1);
  double b = lua_tonumber(L, 2);
  lua_pushnumber(L, a*b);
  return 1;
}

static int test_register_modules(lua_State *L) {
  static const struct luaL_Reg mylib [] = {
    {"mymult", test_cmult},
    {NULL, NULL}  /* sentinel */
  };
  luaL_newlib(L, mylib);
  return 1;
}

/*static void test_require_modules(lua_State *L) {
  luaL_requiref(L, "mylib", test_register_modules, 1);
  lua_pop(L, 1);
}*/

static void registerlib(lua_State *L, const char *name, lua_CFunction f) {
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");
  lua_pushcfunction(L, f);
  lua_setfield(L, -2, name);  /* package.preload[name] = f */
  lua_pop(L, 2);  /* pop ’package’ and ’preload’ tables */
}

int main(int argc, const char * argv[]) {
  char *execDir = dirname((char*)argv[0]);
  char *relScriptPath = "/livelylua_lua/test.lua";
  char absScriptPath[strlen(execDir)+strlen(relScriptPath)];
  strcpy(absScriptPath, execDir);
  strcat(absScriptPath, relScriptPath);

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  registerlib(L, "mylib", test_register_modules);

  if (luaL_loadfile(L, absScriptPath) || lua_pcall(L, 0, 0, 0))
    ll_error(L, "cannot run config. file: %s", lua_tostring(L, -1));
  
  test_read_double(L);
  test_read_person_table(L, "bob");
  test_globals_write(L);
  test_read_person_table(L, "jim");
  double z = test_call_func(L, "mult", 3, 4);
  printf("lua mult 3*4=%f\n", z);
  
  double z2=0;
  ll_call(L, "mult", "dd>d", 3.0, 4.0, &z2);
  printf("again lua mult 3*4=%f\n", z2);
  lua_pop(L, 1);

  lua_pushcfunction(L, test_cmult);
  lua_setglobal(L, "mymult");
  
  double z1 = test_call_func(L, "mymult", 3, 4);
  printf("c mult 3*4=%f\n", z1);

  ll_stackdump(L);
  lua_close(L);

  test_mongoose();
  return 0;
}

