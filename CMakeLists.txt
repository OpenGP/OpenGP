project(OpenGP) #< do not use "project" anywhere else!
cmake_minimum_required(VERSION 2.8)

#--- where should I look for cmake config files?
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
 
#--- import libraries
include(cmake/ConfigureCompiler.cmake)
include(cmake/ConfigureEigen.cmake)
include(cmake/ConfigureOpenGP.cmake)

#--- basic components (only depend from Eigen & OpenGP)
add_subdirectory(src)      # builds the dynamic library
add_subdirectory(data)     # deploys data to build folder
add_subdirectory(doc)      # builds the documentation
add_subdirectory(examples) # builds the examples

#--- full-blown applications (can be compiled independently)
add_subdirectory(apps/glfwviewer)
add_subdirectory(apps/glfwviewer_raw)
add_subdirectory(apps/glfwviewer_multiwin)
add_subdirectory(apps/glfwviewer_debug)
add_subdirectory(apps/glfwviewer_vquality)
add_subdirectory(apps/imshow)
add_subdirectory(apps/baker)
add_subdirectory(apps/subdivision)
add_subdirectory(apps/isoremesh)
#add_subdirectory(apps/qglviewer) # UNSTABLE / OBSOLETE
