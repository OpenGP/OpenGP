find_package(Doxygen)

if(DOXYGEN_FOUND STREQUAL "NO")
  message(STATUS "Doxygen not found. Please get a copy http://www.doxygen.org")
endif()

if("${DOXYGEN_VERSION}" VERSION_LESS 1.8.3.1)
  message(STATUS "Doxygen version too old. Please get a copy http://www.doxygen.org")
endif()

# prepare doxygen configuration file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# create rule for "make doxygen"
add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# create rule for "make publish_doc"
add_custom_target(publish_doc 
	COMMAND doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
	COMMAND ${CMAKE_COMMAND} -E remove_directory ./.git
	COMMAND git init
	COMMAND git add .
	COMMAND git commit -m "updated doc"
	COMMAND git remote add origin https://github.com/OpenGeometry/OpenGeometry.github.io.git
	COMMAND git push origin master --force
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# cleanup $build/html on "make clean"
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES html)

