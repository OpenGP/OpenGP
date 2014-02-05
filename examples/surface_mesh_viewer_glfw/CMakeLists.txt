# @file examples/surface_mesh_viewer_glfw/CMakeLists.txt
# This file is to be included with a CMake "add_subdirectory", do not run cmake on it directly

# Use the header only version of the library
add_definitions( -DHEADERONLY )

# OpenGeometry can be found at ../src
include_directories(../../src/OpenGeometry)

#--- Load the graphics libraries
find_package(OpenGL) #OPTIONAL
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLFW) # OPTIONAL
include_directories(${GLFW_INCLUDE_DIRS})
add_definitions(${GLFW_DEFINITIONS})

find_package(GLEW) # OPTIONAL
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})

find_package(Eigen3) # OPTIONAL
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-DUSE_EIGEN)

set(ALL_FOUND ${GLFW_FOUND} AND ${OPENGL_FOUND} AND ${EIGEN_FOUND} AND ${GLEW_FOUND})
if(NOT ALL_FOUND)
    message(WARNING "Cannot compile GLFW viewer")
    if(NOT GLFW_FOUND)
        message(WARNING "GLFW not found")
    endif(NOT GLFW_FOUND)
    if(NOT OPENGL_FOUND)
        message(WARNING "OpenGL not found")
    endif(NOT OPENGL_FOUND)
    if(NOT EIGEN_FOUND)
        message(WARNING "EIGEN not found")
    endif(NOT EIGEN_FOUND)
    return()
endif(NOT ALL_FOUND)

message(STATUS "Found GLFW at:   " ${GLFW_INCLUDE_DIRS})
message(STATUS "Found Eigen3 at: " ${Eigen3_INCLUDE_DIRS})
add_executable(surface_mesh_viewer_glfw surface_mesh_viewer_glfw.cpp vshader.glsl fshader.glsl)
target_link_libraries(surface_mesh_viewer_glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

#--- Wraps file deployments
macro(deploy_file MYTARGET FILEPATH)
    get_filename_component(FILENAME ${FILEPATH} NAME)
    set(TARGETNAME deployfile_${FILENAME})
    add_custom_target(${TARGETNAME}
        COMMAND ${CMAKE_COMMAND} -E copy ${FILEPATH} ${CMAKE_BINARY_DIR}
        DEPENDS ${FILEPATH})
    add_dependencies(${MYTARGET} ${TARGETNAME})
endmacro()

deploy_file(surface_mesh_viewer_glfw ${CMAKE_CURRENT_SOURCE_DIR}/vshader.glsl)
deploy_file(surface_mesh_viewer_glfw ${CMAKE_CURRENT_SOURCE_DIR}/fshader.glsl)
#deploy_file(surface_mesh_viewer_glfw ${CMAKE_SOURCE_DIR}/examples/data/triangle.obj)
#deploy_file(surface_mesh_viewer_glfw ${CMAKE_SOURCE_DIR}/examples/data/square.off)
#deploy_file(surface_mesh_viewer_glfw ${CMAKE_SOURCE_DIR}/examples/data/tet.obj)
deploy_file(surface_mesh_viewer_glfw ${CMAKE_SOURCE_DIR}/examples/data/sphere.obj)
#deploy_file(surface_mesh_viewer_glfw ${CMAKE_SOURCE_DIR}/examples/data/bunny.obj)
