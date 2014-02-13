# @file examples/CMakeLists.txt
# This file is to be included with a CMake "add_subdirectory", do not run cmake on it directly

# Use the header only version of the library
add_definitions( -DHEADERONLY )

# @TODO: decide where to root the headers, as technically the first is technically a violation
# The second allows include sytanx of the type #include "OpenGP/Surface_mesh/..."
include_directories(../src)

#--- Compile examples one by one
add_executable(surface_mesh_basics      surface_mesh_basics.cpp)
add_executable(surface_mesh_iterators   surface_mesh_iterators.cpp)
add_executable(surface_mesh_io          surface_mesh_io.cpp)
add_executable(surface_mesh_barycenter  surface_mesh_barycenter.cpp)
add_executable(surface_mesh_c++11       surface_mesh_c++11.cpp)

#--- Compile exercises one by one
add_subdirectory(surface_mesh_viewer_glfw)
