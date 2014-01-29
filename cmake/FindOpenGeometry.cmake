# Locate the OpenGeometry library (version 1.0)
# This module defines the following variables:
# OpenGeometry_INCLUDE_DIRS, where to find OpenGeometry include files.
# OpenGeometry_FOUND, true if library path was resolved
#
# Usage example to compile an "executable" target:
#
# FIND_PACKAGE (OpenGeometry REQUIRED)
# INCLUDE_DIRECTORIES (${OpenGeometry_INCLUDE_DIRS})
# ADD_EXECUTABLE (executable ${YOUR_EXECUTABLE_SRCS})
#
FIND_PATH( OpenGeometry_INCLUDE_DIRS OpenGeometry/Surface_mesh/Surface_mesh.h
    $ENV{OpenGeometryDIR}/include
    /usr/local/include
    /usr/local/X11R6/include
    /usr/X11R6/include
    /usr/X11/include
    /usr/include/X11
    /usr/include
    /opt/X11/include
    /opt/include)

# @todo this might be done in a better way in the FIND_PATH above
SET(OpenGeometry_INCLUDE_DIRS ${OpenGeometry_INCLUDE_DIRS}/OpenGeometry)

SET(OpenGeometry_FOUND "NO")
IF(OpenGeometry_INCLUDE_DIRS)
  SET(OpenGeometry_FOUND "YES")
ENDIF(OpenGeometry_INCLUDE_DIRS)
