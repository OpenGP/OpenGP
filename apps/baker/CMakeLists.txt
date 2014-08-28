# @todo description
message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

#--- OpenGP (headeronly)
find_package(OpenGP REQUIRED)
include_directories(${OpenGP_INCLUDE_DIR})

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
add_executable(baker baker.cpp)
target_link_libraries(baker ${LIBS})
