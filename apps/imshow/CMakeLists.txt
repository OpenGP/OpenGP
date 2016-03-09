# Creates either a RGB or a Grayscale Image with eigen and shows it on screen
get_filename_component(PROJNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJNAME})

include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureOpenGL)
include(ConfigureGLFW3)
include(ConfigureGLEW)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")
add_executable(${PROJNAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${PROJNAME} ${LIBRARIES})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/uncompressed.tga DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
