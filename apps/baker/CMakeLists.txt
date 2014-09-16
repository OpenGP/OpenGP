project(baker)

# set(CMAKE_MODULE_PATH ../../cmake)
include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureCGAL)

add_executable(baker baker.cpp)
target_link_libraries(baker ${LIBRARIES})
