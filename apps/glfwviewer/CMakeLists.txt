project(glfwviewer)

# set(CMAKE_MODULE_PATH ../../cmake)
include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureOpenGL)
include(ConfigureGLFW)
include(ConfigureGLEW)

#--- compile
add_executable(glfwviewer main.cpp vshader.glsl fshader.glsl)
target_link_libraries(glfwviewer ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

#--- convert shaders into header files
include(target_stringify_shader)
target_stringify_shader(glfwviewer vshader)
target_stringify_shader(glfwviewer fshader)
