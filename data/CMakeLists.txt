#--- Files in the current (data) folder
file(GLOB_RECURSE DATAFILES "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(datafile ${DATAFILES})
    # message(STATUS ${datafile})
    #--- copy file to the build folder
    configure_file(${datafile} ${CMAKE_BINARY_DIR}/data/ COPYONLY)
endforeach()
