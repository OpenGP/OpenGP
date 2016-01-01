get_filename_component(PROJNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJNAME})

project(isoremesh)

include(ConfigureCompiler)
include(ConfigureEigen)
include(ConfigureOpenGP)
include(ConfigureCGAL)

add_executable(isoremesh main.cpp)
target_link_libraries(isoremesh ${LIBRARIES})
