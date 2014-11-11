#--- Context manager
find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})
add_definitions(${GLFW3_DEFINITIONS})

if(NOT GLFW3_FOUND)
    message(WARNING "GLFW3 not found")
endif()
