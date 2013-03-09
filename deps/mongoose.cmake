
ExternalProject_Add(project_mongoose
  URL https://github.com/mirkok/mongoose.cmake/archive/v3.7.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/mongoose
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/mongoose
)

ExternalProject_Get_Property(project_mongoose install_dir)

include_directories(${install_dir}/include)

add_library(mongoose STATIC IMPORTED)
set_property(TARGET mongoose PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libmongoose.a)
add_dependencies(mongoose project_mongoose)