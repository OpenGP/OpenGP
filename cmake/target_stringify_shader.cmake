macro(target_stringify_shader MYTARGET SHADER)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR})
    # Make a unique name for target
    set(TARGET_NAME stringify_${MYTARGET}_${SHADER})
    add_custom_target( ${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} 
        # Parameters of the script
        -DSHADERNAME=${SHADER}
        -DINPUT_DIR=${SOURCE_DIR}
        -DOUTPUT_DIR=${TARGET_DIR}
        # Executes the script
        -P ${CMAKE_SOURCE_DIR}/cmake/stringify_shader.cmake
        # And when it does spit out this command
        DEPENDS ${SOURCE_DIR}/${SHADER}.glsl
        COMMENT "Converting ${SHADER}.glsl to a char* in ${SHADER}.h")
    # make header visible to the compile time (NEEDS MACRO!!)
    include_directories(${TARGET_DIR})
    # MYTARGET depends on the just created stringify target
    add_dependencies(${MYTARGET} ${TARGET_NAME})
endmacro()