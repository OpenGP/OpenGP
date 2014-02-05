#include <Eigen/Geometry>
#include <surface_mesh/Surface_mesh.h>
namespace surface_mesh{

typedef Eigen::AlignedBox<Scalar,3> Box3;
Box3 bounding_box(const Surface_mesh& mesh){
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
    Box3 bbox;
    bbox.setNull();
    for(Surface_mesh::Vertex v: mesh.vertices()){
        Vec3f vec;   
        bbox.extend( vec /*vpoints[v]*/ );
    }
    return bbox;    
}
    
} // surface_mesh
