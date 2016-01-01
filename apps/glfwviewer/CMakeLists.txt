# Simple mesh viewer based on GLFW
get_filename_component(PROJNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJNAME})

# set(CMAKE_MODULE_PATH ../../cmake)
include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureOpenGL)
include(ConfigureGLFW)
include(ConfigureGLEW)

#--- compile
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE SHADERS "*.glsl")
add_executable(${PROJNAME} ${HEADERS} ${SOURCES} ${SHADERS})
target_link_libraries(${PROJNAME} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

#--- shaders & data need to be copied to run folder
file(COPY vshader.glsl DESTINATION ${PROJECT_BINARY_DIR})
file(COPY fshader.glsl DESTINATION ${PROJECT_BINARY_DIR})
file(COPY bunny.obj    DESTINATION ${PROJECT_BINARY_DIR})