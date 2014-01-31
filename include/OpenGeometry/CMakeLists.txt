# FILE: OpenGeometry/CMakeLists.txt

# Where are the headers found? 
include_directories(./)

# Add the common files to the build
file(GLOB COMMON_SOURCES ./*.cpp)
file(GLOB COMMON_HEADERS ./*.h)
install(FILES ${COMMON_HEADERS} DESTINATION include/OpenGeometry)

# Add the surface_mesh files to the build
file(GLOB SURFACEMESH_SOURCES ./surface_mesh/*.cpp)
file(GLOB SURFACEMESH_HEADERS ./surface_mesh/*.h)
install(FILES ${SURFACEMESH_HEADERS} DESTINATION include/OpenGeometry/surface_mesh)

# Assemble sources of modules
set(SOURCES ${COMMON_SOURCES} ${SURFACEMESH_SOURCES})
set(HEADERS ${COMMON_HEADERS} ${SURFACEMESH_HEADERS})
# Compile the library
if(UNIX)
	add_library(OpenGeometry SHARED ${SOURCES} ${HEADERS})
elseif(WIN32)
	add_library(OpenGeometry STATIC ${SOURCES} ${HEADERS})
endif()

# Set the library version
set_target_properties(OpenGeometry PROPERTIES VERSION 1.0)

# Install rules (for make install)
install(TARGETS OpenGeometry DESTINATION lib)
