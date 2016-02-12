get_filename_component(FOLDERNAME ${CMAKE_CURRENT_LIST_DIR} NAME)

include(ConfigureCGAL) # OPTIONAL

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")
add_executable(${FOLDERNAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(baker ${LIBRARIES})
