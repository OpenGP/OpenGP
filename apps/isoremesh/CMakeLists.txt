# @todo description
message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

# OpenGP can be found at ../src
include_directories(../../src)

#--- Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-DUSE_EIGEN)

#--- CGAL (OPTIONAL)
find_package(CGAL)
if(CGAL_FOUND)
    add_definitions(-DUSE_CGAL)
    list(APPEND LIBS ${CGAL_LIBRARIES})
endif()

#--- Remesh executable
add_executable(isoremesh main.cpp)
target_link_libraries(isoremesh ${LIBS})

#--- Simple test dataset
configure_file("${CMAKE_SOURCE_DIR}/examples/data/bunny.obj" ${CMAKE_BINARY_DIR}/ COPYONLY)
