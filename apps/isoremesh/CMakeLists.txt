project(isoremesh)

# set(CMAKE_MODULE_PATH ../../cmake)
include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureCGAL)

add_executable(isoremesh main.cpp)
target_link_libraries(isoremesh ${LIBRARIES})
