macro(add_example name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} ${LIBRARIES})
endmacro()

add_example(surface_mesh_basics)
add_example(surface_mesh_iterators)
add_example(surface_mesh_io)
add_example(surface_mesh_barycenter)
add_example(surface_mesh_c++11)
add_example(global_properties)

#--- data needs to be copied to run folder
file(COPY ${PROJECT_SOURCE_DIR}/data/bunny.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
