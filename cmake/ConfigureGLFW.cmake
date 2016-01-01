#--- OpenGL Context Manager
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
add_definitions(${GLFW_DEFINITIONS})
list(APPEND LIBRARIES ${GLFW_LIBRARIES})

if(NOT GLFW_FOUND)
    message(WARNING "GLFW not found")
endif()
