# @file examples/glfwviewer/CMakeLists.txt
# This file is to be included with a CMake "add_subdirectory", do not run cmake on it directly

# OpenGP can be found at ../src
include_directories(../../src/OpenGP)
# Use the header only version of the library
add_definitions( -DHEADERONLY )

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

#--- stringify shader macro
macro(target_stringify_shader MYTARGET SHADER)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR})
    # Make a unique name for target
    set(TARGET_NAME stringify_${MYTARGET}_${SHADER})
    add_custom_target( ${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} 
        # Parameters of the script
        -DSHADERNAME=${SHADER}
        -DINPUT_DIR=${SOURCE_DIR}
        -DOUTPUT_DIR=${TARGET_DIR}
        # Executes the script
        -P ${CMAKE_SOURCE_DIR}/cmake/stringify_shader.cmake
        # And when it does spit out this command
        DEPENDS ${SOURCE_DIR}/${SHADER}.glsl
        COMMENT "Converting ${SHADER}.glsl to a char* in ${SHADER}.h")
    # make header visible to the compile time (NEEDS MACRO!!)
    include_directories(${TARGET_DIR})
    # MYTARGET depends on the just created stringify target
    add_dependencies(${MYTARGET} ${TARGET_NAME})
endmacro(target_stringify_shader)

#--- Wraps file deployments
macro(target_deploy_file MYTARGET FILEPATH)
    get_filename_component(FILENAME ${FILEPATH} NAME)
    set(TARGETNAME deployfile_${FILENAME})
    add_custom_target(${TARGETNAME}
        COMMAND ${CMAKE_COMMAND} -E copy ${FILEPATH} ${CMAKE_BINARY_DIR}
        DEPENDS ${FILEPATH})
    add_dependencies(${MYTARGET} ${TARGETNAME})
endmacro()

#--- Convert shaders to a header file
target_stringify_shader(surface_mesh_viewer_glfw vshader)
target_stringify_shader(surface_mesh_viewer_glfw fshader)

#--- Deploy files to the build folder
target_deploy_file(surface_mesh_viewer_glfw ${CMAKE_CURRENT_SOURCE_DIR}/bunny.obj)
