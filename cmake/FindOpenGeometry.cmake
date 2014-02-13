# Locate the OpenGP library (version 1.0)
# This module defines the following variables:
# OpenGP_INCLUDE_DIRS, where to find OpenGP include files.
# OpenGP_FOUND, true if library path was resolved
#
# Usage example to compile an "executable" target:
#
# FIND_PACKAGE (OpenGP REQUIRED)
# INCLUDE_DIRECTORIES (${OpenGP_INCLUDE_DIRS})
# ADD_EXECUTABLE (executable ${YOUR_EXECUTABLE_SRCS})
#

# DEBUG: outputs given environment variable
#message(STATUS OpenGPDIR $ENV{OpenGPDIR})

FIND_PATH( OpenGP_INCLUDE_DIRS OpenGP/Surface_mesh/Surface_mesh.h
    $ENV{OpenGPDIR}
    /usr/local/include
    /usr/local/X11R6/include
    /usr/X11R6/include
    /usr/X11/include
    /usr/include/X11
    /usr/include
    /opt/X11/include
    /opt/include)

# @todo this might be done in a better way in the FIND_PATH above
SET(OpenGP_INCLUDE_DIRS ${OpenGP_INCLUDE_DIRS}/OpenGP)

SET(OpenGP_FOUND "NO")
IF(OpenGP_INCLUDE_DIRS)
  SET(OpenGP_FOUND "YES")
ENDIF(OpenGP_INCLUDE_DIRS)
