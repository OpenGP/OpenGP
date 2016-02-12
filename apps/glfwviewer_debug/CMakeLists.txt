# Examples for the OpenGL rendering classes in OpenGP/GL
get_filename_component(FOLDERNAME ${CMAKE_CURRENT_LIST_DIR} NAME)

include(ConfigureOpenGL)
include(ConfigureGLFW3)
include(ConfigureGLEW)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")
add_executable(${FOLDERNAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${FOLDERNAME} ${LIBRARIES})
