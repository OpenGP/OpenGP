message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)

add_executable(surface_mesh_basics      surface_mesh_basics.cpp)
add_executable(surface_mesh_iterators   surface_mesh_iterators.cpp)
add_executable(surface_mesh_io          surface_mesh_io.cpp)
add_executable(surface_mesh_barycenter  surface_mesh_barycenter.cpp)
add_executable(surface_mesh_c++11       surface_mesh_c++11.cpp)
add_executable(global_properties        global_properties.cpp)
