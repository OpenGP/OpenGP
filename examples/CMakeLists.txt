cmake_minimum_required(VERSION 2.8)
project(examples)

#--- Tell Cmake where the FindOpenGeometry.cmake file is 
#--- Tell where the source directory is w.r.t. examples folder
set(CMAKE_MODULE_PATH ../cmake)
SET(ENV{OpenGeometryDIR} ../)

#--- Load OpenGeometry
find_package(OpenGeometry REQUIRED)
include_directories(${OpenGeometry_INCLUDE_DIRS})
if(NOT OpenGeometry_FOUND) 
    message(ERROR " OpenGeometry not found!")
endif(NOT OpenGeometry_FOUND)

#--- Define header only library
add_definitions( -DHEADERONLY )

#--- Compile exercises one by one
add_executable(surface_mesh_basics      surface_mesh_basics.cpp)
add_executable(surface_mesh_iterators   surface_mesh_iterators.cpp)
add_executable(surface_mesh_io          surface_mesh_io.cpp)
add_executable(surface_mesh_barycenter  surface_mesh_barycenter.cpp)
