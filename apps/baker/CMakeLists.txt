get_filename_component(PROJNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJNAME})

include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureCGAL)

add_executable(baker baker.cpp)
target_link_libraries(baker ${LIBRARIES})
