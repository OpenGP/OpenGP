#--- default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}") 
endif()

#--- Add debug symbols if we are building in debug mode
if(UNIX AND ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif()

#--- UBUNTU
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Using gcc/g++ compiler (Linux)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wno-long-long")
endif()

#--- APPLE 
if(APPLE AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Using Clang compiler on (Apple)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wextra -Wno-long-long")
endif()

#--- WINDOWS
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
endif()

#--- OVERRIDE WITH ANYTHING YOU WANT
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wno-long-long")
