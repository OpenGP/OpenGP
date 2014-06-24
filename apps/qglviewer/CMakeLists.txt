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
message(STATUS OpenGP_INCLUDE_DIR ${OpenGP_INCLUDE_DIR})

#--- Qt
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL) 
find_package(Qt5Xml)
set(CMAKE_AUTOMOC ON)

#--- QGLViewer
find_package(QGLViewer REQUIRED)
include_directories(${QGLVIEWER_INCLUDE_DIR})
add_definitions(-DQGLVIEWER)
list(APPEND LIBRARIES ${QGLVIEWER_LIBRARIES})

#--- Link application
add_executable(qglviewer main.cpp QGLMeshLabViewer.h)
target_link_libraries(qglviewer ${LIBRARIES})

#--- Qt link steps
qt5_use_modules(qglviewer Widgets OpenGL Xml Script)
qt5_add_resources(RCC_SRCS shaders.qrc)

#--- Deploy demo files
configure_file(${CMAKE_SOURCE_DIR}/data/bunny.obj ${CMAKE_BINARY_DIR}/ COPYONLY)
