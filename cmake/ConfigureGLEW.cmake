#--- Multi-platform OpenGL support
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
list(APPEND LIBRARIES ${GLEW_LIBRARIES})
