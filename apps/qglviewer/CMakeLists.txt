#--- Tell where to find Qt
if(APPLE)
    set(CMAKE_PREFIX_PATH "/Developer/Qt/5.2.1/clang_64")
endif()

#--- Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-DUSE_EIGEN)

#--- OpenGP (headeronly)
find_package(OpenGP REQUIRED)
include_directories(${OpenGP_INCLUDE_DIR})

#--- Qt
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL) 
find_package(Qt5Xml)
set(CMAKE_AUTOMOC ON)

#--- OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
list(APPEND LIBRARIES ${OPENGL_LIBRARIES})

#--- QGLViewer
find_package(QGLViewer REQUIRED)
include_directories(${QGLVIEWER_INCLUDE_DIR})
add_definitions(-DQGLVIEWER)
list(APPEND LIBRARIES ${QGLVIEWER_LIBRARIES})

#--- Link application
qt5_add_resources(RESOURCES shaders.qrc)
add_executable(qglviewer main.cpp ${RESOURCES})
target_link_libraries(qglviewer ${LIBRARIES})
qt5_use_modules(qglviewer Widgets OpenGL Xml Script)
