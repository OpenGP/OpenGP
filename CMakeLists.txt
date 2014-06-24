project(OpenGP)
cmake_minimum_required(VERSION 2.8)

#--- default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}") 
endif()

#--- where should I look for cmake extensions?
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#--- set output paths
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

#--- set default compiler flags
if(UNIX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Using gcc/g++ compiler")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wno-long-long")
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Using Clang compiler")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wextra -Wno-long-long")
endif()

#--- windows fixes
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
endif()

#--- Generate rules to build the documentation
option(BUILD_DOCS          "Generate rules to build documentation"    OFF)
option(BUILD_LIBRARY       "Build the OpenGP dynamic library"         OFF)
option(BUILD_EXAMPLES      "Build the OpenGP examples"                OFF)
option(BUILD_APPLICATIONS  "Build the applications/demos"             OFF)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(apps)
 
