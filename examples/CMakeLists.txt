cmake_minimum_required(VERSION 2.8)
project(examples)

#--- Tell Cmake where the FindOpenGeometry.cmake file is 
set(CMAKE_MODULE_PATH ../cmake)
#--- Tell where the source directory is w.r.t. examples folder
SET(ENV{OpenGeometryDIR} ../)

#--- Load OpenGeometry
find_package(OpenGeometry REQUIRED)
include_directories(${OpenGeometry_INCLUDE_DIRS})
if(NOT OpenGeometry_FOUND) 
    message(ERROR " OpenGeometry not found!")
endif(NOT OpenGeometry_FOUND)

message(STATUS "TEST" ${OpenGeometry_INCLUDE_DIRS})
  
# BEGIN(REMOVE ME, REMOVE ME, REMOVE ME)
set(TMP ${OpenGeometry_INCLUDE_DIRS}/surface_mesh)
ADD_LIBRARY( surfacemesh_temp STATIC
${TMP}/IO_obj.cpp
${TMP}/IO_off.cpp
${TMP}/IO_poly.cpp
${TMP}/IO_stl.cpp
${TMP}/IO.cpp)
# END(REMOVE ME, REMOVE ME, REMOVE ME)

add_executable(surface_mesh_basics surface_mesh_basics.cpp)
target_link_libraries(surface_mesh_basics surfacemesh_temp)
