message(STATUS "==> ${CMAKE_CURRENT_LIST_DIR}")

include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)

#--- Find all headers/sources
file(GLOB_RECURSE SOURCES "${OpenGP_INCLUDE_DIR}/OpenGP/*.cpp")
file(GLOB_RECURSE HEADERS "${OpenGP_INCLUDE_DIR}/OpenGP/*.h")

if(NOT WITH_LIBRARY) 
    #--- Dummy target just to have sources/headers appear in IDE
    add_custom_target(opengplib SOURCES ${HEADERS} ${SOURCES})
else()
    message(STATUS "Building libOpenGP.dylib")
    
    #--- If we are building the library then we are not headeronly
    add_definitions(-DDISABLE_HEADERONLY)

    #--- Compile the library
    if(UNIX)
        add_library(OpenGP SHARED ${SOURCES} ${HEADERS})
    elseif(WIN32)
        add_library(OpenGP STATIC ${SOURCES} ${HEADERS})
    endif()

    #--- It's a C++ library
    set_target_properties(OpenGP PROPERTIES LINKER_LANGUAGE CXX)
    #--- Set the library version
    set_target_properties(OpenGP PROPERTIES VERSION 1.0)
endif()

#--- "make install" (always install source as well for headeronly support)
install(FILES ${HEADERS} DESTINATION include/OpenGP)
install(FILES ${SOURCES} DESTINATION include/OpenGP)
if(WITH_LIBRARY)
    install(TARGETS OpenGP DESTINATION lib/)
endif()

#--- "make uninstall" (not fully supported by cmake)
# http://www.cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND 
                  ${CMAKE_COMMAND} -P 
                  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
