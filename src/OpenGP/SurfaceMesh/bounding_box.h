#pragma once
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace Eigen{
//=============================================================================

template <typename Scalar,int _AmbientDim>
inline std::ostream& operator<<(std::ostream& os, const Eigen::AlignedBox<Scalar,_AmbientDim>& box){
    os << "[min[" << box.min().transpose() << "] max[" << box.max().transpose() << "]]";
    return os;
}

//=============================================================================
} // namespace eigen
//=============================================================================

//=============================================================================
namespace OpenGP{
//=============================================================================

inline Box3 bounding_box(const SurfaceMesh& mesh)
{
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
    Box3 bbox;
    bbox.setNull();
    for(auto v: mesh.vertices())
        bbox.extend( vpoints[v] );
    return bbox;
}

/// turn bounding box into a bounding cube (same edge lengths)
inline Box3 bbox_cubified(const Box3& box){
    // TODO: move this function to OpenGP/Types
    Vec3 centre = box.center();
    Vec3 d2 = .5 * box.diagonal();
    // find biggest sizes
    Scalar s = 0;
    s = std::max(s, d2.x());
    s = std::max(s, d2.y());
    s = std::max(s, d2.z());
    Vec3 d2new = Vec3(s,s,s);
    Vec3 bMin = centre-d2new;
    Vec3 bMax = centre+d2new;
    return Box3(bMin, bMax);
}

/// Slightly enlarges the bounding box by a given factor (about center)
inline Box3 bbox_scaled(const Box3& box, Scalar factor){
    // TODO: move this function to OpenGP/Types    
    Vec3 centre = box.center();
    Vec3 d2 = .5 * box.diagonal();
    Vec3 bMin = centre - factor * d2;
    Vec3 bMax = centre + factor * d2;
    return Box3(bMin, bMax);
}
   
//=============================================================================
} // namespace OpenGP
//=============================================================================
