
ExternalProject_Add(project_lua
  URL http://luajit.org/download/LuaJIT-2.0.1.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make
  INSTALL_COMMAND make install PREFIX=${CMAKE_CURRENT_BINARY_DIR}/lua
  BUILD_IN_SOURCE 1
)

ExternalProject_Get_Property(project_lua install_dir)

include_directories(${install_dir}/include)

add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libluajit-5.1.a)
add_dependencies(lua project_lua)