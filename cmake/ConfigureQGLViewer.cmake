find_package(QGLViewer)
if(QGLVIEWER_FOUND)
    include_directories(${QGLVIEWER_INCLUDE_DIR})
    list(APPEND LIBRARIES ${QGLVIEWER_LIBRARIES})
    add_definitions(-DWITH_QGLVIEWER)
endif()