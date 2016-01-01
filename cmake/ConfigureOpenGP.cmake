#--- OpenGP (headeronly)
find_package(OpenGP REQUIRED)
include_directories(${OpenGP_INCLUDE_DIR})
#message(STATUS ${OpenGP_INCLUDE_DIR})

#--- Dummy target just to have sources/headers appear in IDE
file(GLOB_RECURSE SOURCES "${OpenGP_INCLUDE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${OpenGP_INCLUDE_DIR}/*.h")
add_custom_target(OPENGP_TARGET SOURCES ${HEADERS} ${SOURCES})
