#include <OpenGP/Surface_mesh.h>
#include <Eigen/Geometry>

namespace Eigen{

template <typename Scalar,int _AmbientDim>
inline std::ostream& operator<<(std::ostream& os, const Eigen::AlignedBox<Scalar,_AmbientDim>& box)
{
    os << "[min[" << box.min().transpose() << "] max[" << box.max().transpose() << "]]";
    return os;
}

} // Eigen


namespace opengp{

/// @todo should we move this
typedef Eigen::AlignedBox<Scalar,3> Box3; 
Box3 bounding_box(const Surface_mesh& mesh)
{
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
    Box3 bbox;
    bbox.setNull();
    for(auto v: mesh.vertices())
        bbox.extend( vpoints[v] );
    return bbox;
}

   
} // opengp
