# @file examples/glfwviewer/CMakeLists.txt
# This file is to be included with a CMake "add_subdirectory", do not run cmake on it directly
message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

#--- OpenGP (headeronly)
find_package(OpenGP REQUIRED)
include_directories(${OpenGP_INCLUDE_DIR})

#--- Load the graphics libraries
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

#--- Context manager
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
add_definitions(${GLFW_DEFINITIONS})

#--- Multi-platform OpenGL support
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})

#--- Math
find_package(Eigen3 REQUIRED)
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

#--- compile
add_executable(glfwviewer main.cpp vshader.glsl fshader.glsl)
target_link_libraries(glfwviewer ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

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
target_stringify_shader(glfwviewer vshader)
target_stringify_shader(glfwviewer fshader)

#--- Deploy files to the build folder
target_deploy_file(glfwviewer ${CMAKE_CURRENT_SOURCE_DIR}/bunny.obj)
