# Locate the OpenGP library
# This module defines the following variables:
# OpenGP_INCLUDE_DIRS, where to find OpenGP include files.
# OpenGP_FOUND true if library path was resolved
#
# Usage example to compile an "executable" target:
#
# find_package(OpenGP REQUIRED)
# include_directories(${OpenGP_INCLUDE_DIR})
# add_executable(executable ${YOUR_EXECUTABLE_SRCS})
# 
# DEBUG: outputs given environment variable
# message(STATUS OpenGPDIR $ENV{OpenGP_DIR})

find_path(OpenGP_INCLUDE_DIR OpenGP/SurfaceMesh/SurfaceMesh.h
    # Check build tree
    ./src
    ../src
    ../../src
    ../../../src
    # Check environment
    $ENV{OpenGP_DIR}
    # check system folders
    /usr/local/include
    /usr/include
    /opt/include)

#--- Found?
if(OpenGP_INCLUDE_DIR)
    set(OpenGP_FOUND "YES")
else()
    set(OpenGP_FOUND "NO")
endif()

#--- Message
if(OpenGP_FOUND)
#    if(NOT CMAKE_FIND_QUIETLY)
#        message(STATUS "Found OpenGP: ${OpenGP_INCLUDE_DIR}")
#    endif()
else()
    if(OpenGP_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find OpenGP")
    endif()
endif()
