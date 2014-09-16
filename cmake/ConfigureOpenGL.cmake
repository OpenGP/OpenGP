find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
list(APPEND LIBRARIES ${OPENGL_LIBRARIES})

if(NOT OPENGL_FOUND)
    message(WARNING "OpenGL not found")
endif(NOT OPENGL_FOUND)

#--- Allows 
if(UNIX AND NOT APPLE)
    add_definitions(-DGL_GLEXT_PROTOTYPES=1)
endif()
