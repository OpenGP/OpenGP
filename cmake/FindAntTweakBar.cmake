# Locate the AntTweakBar library
# This module defines the following variables:
# ATB_LIBRARIES, the name of the library;
# ATB_INCLUDE_DIRS, where to find ATB include files.
# ATB_FOUND, true if library path was resolved
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (ATB REQUIRED)
# INCLUDE_DIRECTORIES (${ATB_INCLUDE_DIRS})
# ADD_EXECUTABLE (executable ${YOUR_EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${ATB_LIBRARIES})
#
# TODO:
# Lookup for windows
# Allow the user to select to link to a shared library or to a static library.
#
# SEE:
# - https://raw.github.com/progschj/OpenGL-Examples/master/cmake_modules/FindATB.cmake
# 

FIND_PATH( ATB_INCLUDE_DIRS AntTweakBar.h
    $ENV{ATBDIR}/include
    /usr/local/include
    /usr/local/X11R6/include
    /usr/X11R6/include
    /usr/X11/include
    /usr/include/X11
    /usr/include
    /opt/X11/include
    /opt/include )

FIND_LIBRARY( ATB_LIBRARIES NAMES AntTweakBar PATHS
    $ENV{ATBDIR}/lib
    $ENV{ATBDIR}/support/msvc80/Debug
    $ENV{ATBDIR}/support/msvc80/Release
    /usr/local/lib
    /usr/local/X11R6/lib
    /usr/X11R6/lib
    /usr/X11/lib
    /usr/lib/X11
    /usr/lib
    /opt/X11/lib
    /opt/lib )

SET(ATB_FOUND "NO")
IF(ATB_LIBRARIES AND ATB_INCLUDE_DIRS)
    SET(ATB_FOUND "YES")
ENDIF(ATB_LIBRARIES AND ATB_INCLUDE_DIRS)
