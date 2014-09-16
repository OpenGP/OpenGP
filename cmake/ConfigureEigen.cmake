find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# TODO check how to do this
# if(NOT Eigen3_FOUND)
#     message(WARNING "Eigen3 not found")
# endif()