cmake_minimum_required(VERSION 2.8)
project(qglviewer)

#--- OpenGP shared modules
# set(CMAKE_MODULE_PATH ../../cmake)
include(ConfigureCompiler)
include(ConfigureQt5)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureOpenGL)
include(ConfigureQGLViewer)

#--- Link application
qt5_add_resources(RESOURCES shaders.qrc)
add_executable(qglviewer main.cpp ${RESOURCES})
target_link_libraries(qglviewer ${LIBRARIES})
qt5_use_modules(qglviewer Widgets OpenGL Xml Script)
