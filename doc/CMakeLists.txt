find_package(Doxygen)

if(DOXYGEN_FOUND STREQUAL "NO")
  message(STATUS "You have added the documentation build system, but Doxygen not found.")
  message(STATUS "Please get a copy through your package manager or at http://www.doxygen.org")
endif()

if("${DOXYGEN_VERSION}" VERSION_LESS 1.8.3.1)
  message(STATUS "Doxygen version too old. Please get a copy http://www.doxygen.org")
endif()

# prepare doxygen configuration file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# create rule for "make doxygen"
add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# copy all the examples sources to the doc folder
add_custom_command(TARGET doxygen PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/doc/examples/)
file(GLOB EXAMPLESCPP ${CMAKE_SOURCE_DIR}/examples/*.cpp)
foreach(example_cpp ${EXAMPLESCPP})
    add_custom_command(TARGET doxygen PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${example_cpp} ${CMAKE_BINARY_DIR}/doc/examples/)
endforeach()

# create rule for "make publish_doc"
add_custom_target(publish_doc 
    COMMAND ${CMAKE_COMMAND} -E remove_directory ./.git
	COMMAND git init
	COMMAND git add .
	COMMAND git commit -m "updated doc"
	COMMAND git remote add origin https://github.com/OpenGP/OpenGP.github.io
    COMMAND git push origin master --force
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    
# Publishing the doc needs for it to be built first
add_dependencies(publish_doc doxygen)

# cleanup $build/html on "make clean"
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES html)


