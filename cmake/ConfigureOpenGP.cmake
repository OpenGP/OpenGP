find_package(OpenGP REQUIRED)
include_directories(${OpenGP_INCLUDE_DIR})
#message(STATUS ${OpenGP_INCLUDE_DIR})

#--- avoid errors caused by duplicates
if(NOT TARGET OPENGP) 
    # Dummy target just to have sources/headers appear in IDE
    file(GLOB_RECURSE SOURCES "${OpenGP_INCLUDE_DIR}/*.cpp")
    file(GLOB_RECURSE HEADERS "${OpenGP_INCLUDE_DIR}/*.h")
    add_custom_target(OPENGP SOURCES ${HEADERS} ${SOURCES})
endif()

#--- Toggles the OpenGP configuration type
# 1) interactive ccmake exposes this option directly
# 2) command line can change this: "cmake -DOPENGP_HEADERONLY=False"
SET(OPENGP_HEADERONLY True CACHE BOOL "Setup OpenGP in header only?")
if(OPENGP_HEADERONLY)
    add_definitions(-DOPENGP_HEADERONLY)   
else()
    list(APPEND LIBRARIES OpenGP)    
endif()

