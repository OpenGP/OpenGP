# FILE: OpenGP/CMakeLists.txt

# Where are the headers found? 
include_directories(.)

# Add the common files to the build
file(GLOB COMMON_SOURCES ./OpenGP/*.cpp)
file(GLOB COMMON_HEADERS ./OpenGP/.h)
install(FILES ${COMMON_HEADERS} DESTINATION include/OpenGP)

# Add the surface_mesh files to the build
file(GLOB SURFACEMESH_SOURCES ./OpenGP/surface_mesh/*.cpp)
file(GLOB SURFACEMESH_HEADERS ./OpenGP/surface_mesh/*.h)
install(FILES ${SURFACEMESH_HEADERS} DESTINATION include/OpenGP/surface_mesh)

# Assemble sources of modules
set(SOURCES ${COMMON_SOURCES} ${SURFACEMESH_SOURCES})
set(HEADERS ${COMMON_HEADERS} ${SURFACEMESH_HEADERS})
# Compile the library
if(UNIX)
    add_library(OpenGP SHARED ${SOURCES} ${HEADERS})
elseif(WIN32)
    add_library(OpenGP STATIC ${SOURCES} ${HEADERS})
endif()

# Set the library version
set_target_properties(OpenGP PROPERTIES VERSION 1.0)

# Install rules (for make install)
install(TARGETS OpenGP DESTINATION lib)
