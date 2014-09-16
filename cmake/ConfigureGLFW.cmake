#--- Context manager
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
add_definitions(${GLFW_DEFINITIONS})

if(NOT GLFW_FOUND)
    message(WARNING "GLFW not found")
endif()
