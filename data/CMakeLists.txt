if(NOT WITH_DATA)
    return()
else()
    message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")
endif()

#--- Deploy demo files
file(GLOB_RECURSE DATAFILES "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(datafile ${DATAFILES})
    # message(STATUS ${datafile})
    configure_file(${datafile} ${CMAKE_BINARY_DIR}/data/ COPYONLY)
endforeach()
