
ExternalProject_Add(project_luajit
  URL https://github.com/mirkok/luajit.cmake/archive/v2.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lua
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/lua
)

ExternalProject_Get_Property(project_luajit install_dir)

include_directories(${install_dir}/include/luajit-2.0)

add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libluajit-5.1.a)
add_dependencies(lua project_luajit)