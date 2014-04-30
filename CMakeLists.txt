project(OpenGP)
cmake_minimum_required(VERSION 2.8)

#--- where should I look for cmake extensions?
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#--- set output paths
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

#--- set default compiler flags
if((UNIX OR APPLE) AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wno-long-long")
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wextra -Wno-long-long")
endif()

#--- TODO: what was this for?
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
endif()

#--- Generate rules to build the documentation
if(WITH_DOCS)
    add_subdirectory(doc)
endif()

#--- Generate rules to compile the library
if(WITH_LIBRARY and DISABLE_HEADERONLY)
    add_subdirectory(src)
endif()

#--- Generate rules to build the examples
if(WITH_EXAMPLES)
    add_subdirectory(examples)
endif()
