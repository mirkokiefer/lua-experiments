
ExternalProject_Add(project_lua
  URL https://github.com/mirkok/liblua/archive/lua-5.2.1.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(project_lua install_dir)

include_directories(${install_dir}/include)

add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${install_dir}/lib/liblua.a)
add_dependencies(lua project_lua)