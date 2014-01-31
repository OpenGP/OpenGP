# @file examples/CMakeLists.txt
# This file is to be included with a CMake "add_subdirectory", do not run cmake on it directly

# Use the header only version of the library
add_definitions( -DHEADERONLY )

# That can be found at ../src
include_directories(../src/OpenGeometry)

#--- Compile exercises one by one
add_executable(surface_mesh_basics      surface_mesh_basics.cpp)
add_executable(surface_mesh_iterators   surface_mesh_iterators.cpp)
add_executable(surface_mesh_io          surface_mesh_io.cpp)
add_executable(surface_mesh_barycenter  surface_mesh_barycenter.cpp)
add_executable(surface_mesh_c++11       surface_mesh_c++11.cpp)

