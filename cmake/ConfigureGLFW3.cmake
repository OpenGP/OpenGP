#--- Context manager
find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})
add_definitions(${GLFW3_DEFINITIONS})
list(APPEND LIBRARIES ${GLFW3_LIBRARIES})

if(UNIX AND NOT APPLE)
    list(APPEND LIBRARIES rt m dl Xrandr Xinerama Xext Xcursor Xrender Xfixes X11 pthread xcb Xau Xdmcp)
endif()

if(NOT GLFW3_FOUND)
    message(WARNING "GLFW3 not found")
endif()
