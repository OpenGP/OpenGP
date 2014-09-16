#--- default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}") 
endif()

#--- Add debug symbols if we are building in debug mode
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif()

#--- set default compiler flags
#if(UNIX AND CMAKE_COMPILER_IS_GNUCXX)
#    message(STATUS "Using gcc/g++ compiler")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wno-long-long")
#endif()
#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#    message(STATUS "Using Clang compiler")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wextra -Wno-long-long")
#endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wno-long-long")

#--- windows fixes
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
endif()

