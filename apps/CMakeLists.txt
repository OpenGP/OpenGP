if(NOT BUILD_APPLICATIONS)
    return()
else()
    message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")
endif()

add_subdirectory(glfwviewer)
add_subdirectory(isoremesh)
