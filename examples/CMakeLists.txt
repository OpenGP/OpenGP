# @file examples/CMakeLists.txt
# @note this file is to be included with a CMake "add_subdirectory", do not run cmake on it directly
message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

#--- Make a release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

#--- examples use eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-DUSE_EIGEN)

#--- examples know where to find the OpenGP header only source
add_definitions( -DHEADERONLY )
include_directories(../src)

#--- Compile examples one by one
add_executable(surface_mesh_basics      surface_mesh_basics.cpp)
add_executable(surface_mesh_iterators   surface_mesh_iterators.cpp)
add_executable(surface_mesh_io          surface_mesh_io.cpp)
add_executable(surface_mesh_barycenter  surface_mesh_barycenter.cpp)
add_executable(surface_mesh_c++11       surface_mesh_c++11.cpp)
add_executable(global_properties        global_properties.cpp)
