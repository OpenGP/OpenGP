#pragma once

#include <OpenGP/SphereMesh/SphereMesh.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

inline Vec2 pill_tangent(const Vec4 &s0_3D, const Vec4 &s1_3D) {

    /// Adapted from pill_dist.m

    Scalar dist_3D = (s1_3D.block<3, 1>(0, 0) - s0_3D.block<3, 1>(0, 0)).norm();

    Vec3 s0(0, 0, s0_3D(3));
    Vec3 s1(dist_3D, 0, s1_3D(3));

    Vec3 a = s1 - s0;
    Scalar a2 = (a(0)*a(0)) + (a(1)*a(1));
    Scalar ad = std::sqrt(a2);
    Vec2 ab = a.block<2, 1>(0, 0);
    Vec2 an = ab / ad;

    Scalar beta = ab.dot(Vec2(std::sqrt((ad*ad)-(a(2)*a(2))), a(2))) / (ad * ad);
    Scalar alpha = std::sqrt(1 - (beta * beta));

    return Vec2(beta, alpha);

}

//=============================================================================
} // namespace OpenGP
//=============================================================================
