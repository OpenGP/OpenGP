project(OpenGP)
cmake_minimum_required(VERSION 2.8)

#--- where should I look for cmake configuration files?
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#--- everything will be built in the project build folder
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
 
#--- basic components
add_subdirectory(examples) # builds the examples
add_subdirectory(library)  # builds dynamic library
add_subdirectory(data)     # deploys data to the build folder
add_subdirectory(doc)      # builds the documentation
 
#--- full-blown applications
if(WITH_APPLICATIONS)
    add_subdirectory(apps/qglviewer)
    add_subdirectory(apps/baker)
    add_subdirectory(apps/isoremesh)
    add_subdirectory(apps/glfwviewer)
endif()
