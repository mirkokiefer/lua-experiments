
ExternalProject_Add(project_lua
  URL https://github.com/mirkok/lua.cmake/archive/v5.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/lua
)

ExternalProject_Get_Property(project_lua install_dir)

include_directories(${install_dir}/include)

add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${install_dir}/lib/liblua.a)
add_dependencies(lua project_lua)