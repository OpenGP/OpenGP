#--- Find all headers/sources
file(GLOB_RECURSE SOURCES "OpenGP/*.cpp")
file(GLOB_RECURSE HEADERS "OpenGP/*.h")

#--- "make install" (always install source as well for headeronly support)
install(FILES ${HEADERS} DESTINATION include/OpenGP)
install(FILES ${SOURCES} DESTINATION include/OpenGP)

#--- STOP HERE: do not compile if in header only mode
if(OPENGP_HEADERONLY)
    message(STATUS "Not building libOpenGP.dylib (header-only)")
    return()
else()
    message(STATUS "Building libOpenGP.dylib")    
endif()

#--- Gets rid of annoying CMake 3 warnings
if(NOT (CMAKE_MAJOR_VERSION LESS 3))
    cmake_policy(SET CMP0042 NEW)
endif()

#--- OpenGP headers
include_directories(${CMAKE_CURRENT_LIST_DIR})

#--- Compiling library needs dependencies
include(ConfigureOpenGL)
include(ConfigureGLFW3)
include(ConfigureGLEW)

#--- Prevents OpenGP from self-linking!!
#message(STATUS "LIBRARIES: ${LIBRARIES}")
set(LIBRARIES_ ${LIBRARIES} )
list(REMOVE_ITEM LIBRARIES_ OpenGP)

#--- Compile the library
if(UNIX)
    add_library(OpenGP SHARED ${SOURCES} ${HEADERS})
    target_link_libraries(OpenGP ${LIBRARIES_})
elseif(WIN32)
    add_library(OpenGP STATIC ${SOURCES} ${HEADERS})
    target_link_libraries(OpenGP ${LIBRARIES_})
endif()

#--- "make install" (always install source as well for headeronly support)
install(TARGETS OpenGP DESTINATION lib/)

#--- It's a C++ library
set_target_properties(OpenGP PROPERTIES LINKER_LANGUAGE CXX)
#--- Set the library version
set_target_properties(OpenGP PROPERTIES VERSION 1.0)

# TODO fix this
#--- "make uninstall" (not fully supported by cmake)
# http://www.cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#               IMMEDIATE @ONLY)
#add_custom_target(uninstall COMMAND 
#                  ${CMAKE_COMMAND} -P 
#                  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
