# FILE: src/CMakeLists.txt
message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

# Where are the headers found? 
include_directories(.)

#--- Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-DUSE_EIGEN)

#--- Make a release library build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

#--- Add debug symbols if we are building in debug mode
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif()

#--- Add the common files to the build
file(GLOB COMMON_SOURCES ./OpenGP/*.cpp)
file(GLOB COMMON_HEADERS ./OpenGP/*.h)

#--- Add the surface_mesh files to the build
file(GLOB SURFACEMESH_SOURCES ./OpenGP/surface_mesh/*.cpp)
file(GLOB SURFACEMESH_HEADERS ./OpenGP/surface_mesh/*.h)

#--- TODO: Install rules (make install)
#install(FILES ${COMMON_HEADERS} DESTINATION include/OpenGP)
#install(FILES ${SURFACEMESH_HEADERS} DESTINATION include/OpenGP/surface_mesh)
#install(TARGETS OpenGP DESTINATION lib)

# Assemble sources of modules
set(SOURCES ${COMMON_SOURCES} ${SURFACEMESH_SOURCES})
set(HEADERS ${COMMON_HEADERS} ${SURFACEMESH_HEADERS})

if(BUILD_LIBRARY) 
    message(STATUS "Bulding libOpenGP.dylib")
    #--- If we are building the library then we are not headeronly
    add_definitions(-DDISABLE_HEADERONLY)
    #--- Compile the library
    if(UNIX)
        add_library(OpenGP SHARED ${SOURCES} ${HEADERS})
    elseif(WIN32)
        add_library(OpenGP STATIC ${SOURCES} ${HEADERS})
    endif()
    #--- Set the library version
    set_target_properties(OpenGP PROPERTIES VERSION 1.0)
else()
    #--- Dummy just to have sources/headers appear in IDE
    add_custom_target(dummy SOURCES ${HEADERS} ${SOURCES})
endif()



