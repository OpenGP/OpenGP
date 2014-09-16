#include "remesh.h"
#include "OpenGP/Surface_mesh.h"

//=============================================================================
namespace opengp {
//=============================================================================

/// @brief dot product between two eigen vectors
template <typename Scalar, int N>
inline Scalar dot(const Eigen::Matrix<Scalar, N, 1>& v0, const Eigen::Matrix<Scalar, N, 1>& v1){ return v0.dot(v1); }

namespace {
typedef Vec3 Point;
}

template <class T> static inline T deg_to_rad(const T& _angle) {
    return M_PI*(_angle/180);
}
template <class T> static inline T rad_to_deg(const T& _angle) {
    return 180*(_angle/M_PI);
}

static inline Scalar sane_aarg(Scalar _aarg) {
    if (_aarg < -1)
        _aarg = -1;
    else if (_aarg >  1)
        _aarg = 1;
    return _aarg;
}

static inline Scalar angle(Scalar _cos_angle, Scalar _sin_angle) {
    //sanity checks - otherwise acos will return NAN
    _cos_angle = sane_aarg(_cos_angle);
    return (Scalar) _sin_angle >= 0 ? acos(_cos_angle) : -acos(_cos_angle);
}


// Helper function
static Scalar calc_dihedral_angle(Surface_mesh & mesh, Surface_mesh::Halfedge _heh) {
    if (mesh.is_boundary(mesh.edge(_heh))) {
        //the dihedral angle at a boundary edge is 0
        return 0;
    }

    Vector3FaceProperty normal = mesh.face_property<Vector3>( FNORMAL );
    Vector3VertexProperty points = mesh.vertex_property<Vector3>( VPOINT );

    const Vector3& n0 = normal[mesh.face(_heh)];
    const Vector3& n1 = normal[mesh.face(mesh.opposite_halfedge(_heh))];

    Vector3 he = points[mesh.to_vertex(_heh)] - points[mesh.from_vertex(_heh)];

    Scalar da_cos = dot(n0, n1);

    //should be normalized, but we need only the sign
    Scalar da_sin_sign = dot(n0.cross(n1), he);
    return angle(da_cos, da_sin_sign);
}

static Scalar distPointTriangleSquared( const Vector3& _p,const Vector3& _v0,const Vector3& _v1,const Vector3& _v2,Vector3& _nearestPoint ) {
    Vector3 v0v1 = _v1 - _v0;
    Vector3 v0v2 = _v2 - _v0;
    Vector3 n = v0v1.cross(v0v2); // not normalized !
    double d = n.squaredNorm();


    // Check if the triangle is degenerated
    if (d < std::numeric_limits<float>::min() && d > -std::numeric_limits<float>::min()) {
        std::cerr << "distPointTriangleSquared: Degenerated triangle !\n";
        return std::numeric_limits<float>::max();
    }
    double invD = 1.0 / d;


    // these are not needed for every point, should still perform
    // better with many points against one triangle
    Vector3 v1v2 = _v2 - _v1;
    double inv_v0v2_2 = 1.0 / v0v2.squaredNorm();
    double inv_v0v1_2 = 1.0 / v0v1.squaredNorm();
    double inv_v1v2_2 = 1.0 / v1v2.squaredNorm();


    Vector3 v0p = _p - _v0;
    Vector3 t = v0p.cross(n);
    double  s01, s02, s12;
    double a = dot(t, v0v2) * -invD;
    double b = dot(t, v0v1) * invD;


    if (a < 0) {
        // Calculate the distance to an edge or a corner vertex
        s02 = dot( v0v2, v0p ) * inv_v0v2_2;
        if (s02 < 0.0) {
            s01 = dot( v0v1, v0p ) * inv_v0v1_2;
            if (s01 <= 0.0) {
                v0p = _v0;
            } else if (s01 >= 1.0) {
                v0p = _v1;
            } else {
                v0p = _v0 + v0v1 * s01;
            }
        } else if (s02 > 1.0) {
            s12 = dot( v1v2, Vector3(_p - _v1 )) * inv_v1v2_2;
            if (s12 >= 1.0) {
                v0p = _v2;
            } else if (s12 <= 0.0) {
                v0p = _v1;
            } else {
                v0p = _v1 + v1v2 * s12;
            }
        } else {
            v0p = _v0 + v0v2 * s02;
        }
    } else if (b < 0.0) {
        // Calculate the distance to an edge or a corner vertex
        s01 = dot( v0v1, v0p ) * inv_v0v1_2;
        if (s01 < 0.0) {
            s02 = dot( v0v2,  v0p ) * inv_v0v2_2;
            if (s02 <= 0.0) {
                v0p = _v0;
            } else if (s02 >= 1.0) {
                v0p = _v2;
            } else {
                v0p = _v0 + v0v2 * s02;
            }
        } else if (s01 > 1.0) {
            s12 = dot( v1v2, Vector3(_p - _v1 )) * inv_v1v2_2;
            if (s12 >= 1.0) {
                v0p = _v2;
            } else if (s12 <= 0.0) {
                v0p = _v1;
            } else {
                v0p = _v1 + v1v2 * s12;
            }
        } else {
            v0p = _v0 + v0v1 * s01;
        }
    } else if (a+b > 1.0) {
        // Calculate the distance to an edge or a corner vertex
        s12 = dot( v1v2, Vector3(_p - _v1 )) * inv_v1v2_2;
        if (s12 >= 1.0) {
            s02 = dot( v0v2, v0p ) * inv_v0v2_2;
            if (s02 <= 0.0) {
                v0p = _v0;
            } else if (s02 >= 1.0) {
                v0p = _v2;
            } else {
                v0p = _v0 + v0v2*s02;
            }
        } else if (s12 <= 0.0) {
            s01 = dot( v0v1,  v0p ) * inv_v0v1_2;
            if (s01 <= 0.0) {
                v0p = _v0;
            } else if (s01 >= 1.0) {
                v0p = _v1;
            } else {
                v0p = _v0 + v0v1 * s01;
            }
        } else {
            v0p = _v1 + v1v2 * s12;
        }
    } else {
        // Calculate the distance to an interior point of the triangle
        _nearestPoint = _p - n*(dot(n,v0p) * invD);
        return (_nearestPoint - _p).squaredNorm();
    }

    _nearestPoint = v0p;

    return (_nearestPoint - _p).squaredNorm();
}

inline Scalar ClosestPointTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c, Vector3 & closest) {
    // Check if P in vertex region outside A
    Vec3 ab = b - a;
    Vec3 ac = c - a;
    Vec3 ap = p - a;
    Scalar d1 = dot(ab, ap);
    Scalar d2 = dot(ac, ap);
    if (d1 <= 0 && d2 <= 0) {
        closest = a;
        return (p-closest).squaredNorm(); // barycentric coordinates (1,0,0)
    }
    // Check if P in vertex region outside B
    Vec3 bp = p - b;
    Scalar d3 = dot(ab, bp);
    Scalar d4 = dot(ac, bp);
    if (d3 >= 0 && d4 <= d3) {
        closest = b;
        return (p-closest).squaredNorm(); // barycentric coordinates (0,1,0)
    }
    // Check if P in edge region of AB, if so return projection of P onto AB
    Scalar vc = d1*d4 - d3*d2;
    if (vc <= 0 && d1 >= 0 && d3 <= 0) {
        Scalar v = d1 / (d1 - d3);
        closest = a + v * ab;
        return (p - closest).squaredNorm(); // barycentric coordinates (1-v,v,0)
    }
    // Check if P in vertex region outside C
    Vec3 cp = p - c;
    Scalar d5 = dot(ab, cp);
    Scalar d6 = dot(ac, cp);
    if (d6 >= 0 && d5 <= d6) {
        closest = c;
        return (p-closest).squaredNorm(); // barycentric coordinates (0,0,1)
    }
    // Check if P in edge region of AC, if so return projection of P onto AC
    Scalar vb = d5*d2 - d1*d6;
    if (vb <= 0 && d2 >= 0 && d6 <= 0) {
        Scalar w = d2 / (d2 - d6);
        closest = a + w * ac;
        return (p - closest).squaredNorm(); // barycentric coordinates (1-w,0,w)
    }
    // Check if P in edge region of BC, if so return projection of P onto BC
    Scalar va = d3*d6 - d5*d4;
    if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {
        Scalar w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        closest = b + w * (c - b);
        return (p - closest).squaredNorm(); // barycentric coordinates (0,1-w,w)
    }
    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    Scalar denom = 1.0 / (va + vb + vc);
    Scalar v = vb * denom;
    Scalar w = vc * denom;
    closest = a + ab * v + ac * w;
    return (p - closest).squaredNorm(); // = u*a + v*b + w*c, u = va * denom = 1 - v - w
}

inline static bool TestSphereTriangle(Point sphereCenter, Scalar sphereRadius, Point a, Point b, Point c, Point &p) {
    // Find point P on triangle ABC closest to sphere center
    ClosestPointTriangle(sphereCenter, a, b, c, p);

    // Sphere and triangle intersect if the (squared) distance from sphere
    // center to point p is less than the (squared) sphere radius
    Vec3 v = p - sphereCenter;
    return dot(v, v) <= sphereRadius * sphereRadius;
}

inline static bool TestSphereTriangle(Point sphereCenter, Scalar sphereRadius, Point a, Point b, Point c) {
    Vector3 p(0,0,0);
    return TestSphereTriangle(sphereCenter, sphereRadius, a, b, c);
}

/// performs edge splits until all edges are shorter than the threshold
void IsotropicRemesher::splitLongEdges(Scalar maxEdgeLength ) {
    *myout << __FUNCTION__ << std::endl;
    
    const Scalar maxEdgeLengthSqr = maxEdgeLength * maxEdgeLength;

    Surface_mesh::Edge_iterator e_it;
    Surface_mesh::Edge_iterator e_end = mesh->edges_end();

    // iterate over all edges
    int n_splits = 0;
    for (e_it = mesh->edges_begin(); e_it != e_end; ++e_it) {
        const Surface_mesh::Halfedge & hh = mesh->halfedge( *e_it, 0 );

        const Surface_mesh::Vertex & v0 = mesh->from_vertex(hh);
        const Surface_mesh::Vertex & v1 = mesh->to_vertex(hh);

        Vector3 vec = points[v1] - points[v0];

        // edge too long?
        if ( vec.squaredNorm() > maxEdgeLengthSqr ) {

            const Vector3 midPoint = points[v0] + ( 0.5 * vec );

            // split at midpoint
            Surface_mesh::Vertex vh = mesh->add_vertex( midPoint );

            bool hadFeature = efeature[*e_it];

            mesh->split(*e_it, vh);
            n_splits++;
            
            if ( hadFeature ) {
                for(Surface_mesh::Halfedge e: mesh->halfedges(vh)) {
                    if ( mesh->to_vertex(e) == v0 || mesh->to_vertex(e) == v1 ) {
                        efeature[mesh->edge(e)] = true;
                    }
                }
            }
        }
    }
    
    *myout << "    split " << n_splits << " edges" << std::endl;
}

/// collapse edges shorter than minEdgeLength if collapsing doesn't result in new edge longer than maxEdgeLength
void IsotropicRemesher::collapseShortEdges(const Scalar _minEdgeLength, const Scalar _maxEdgeLength, bool isKeepShortEdges ) {
    *myout << __FUNCTION__ << std::endl;
    
    const Scalar _minEdgeLengthSqr = _minEdgeLength * _minEdgeLength;
    const Scalar _maxEdgeLengthSqr = _maxEdgeLength * _maxEdgeLength;

    //add checked property
    BoolEdgeProperty checked = mesh->edge_property< bool >("e:checked", false);

    Surface_mesh::Edge_iterator e_it;

    bool finished = false;

    int n_collapsed = 0;
    while( !finished ) {
        finished = true;
        for (e_it = mesh->edges_begin(); e_it != mesh->edges_end() ; ++e_it) {

            if ( checked[*e_it] )
                continue;

            checked[*e_it] = true;

            const Surface_mesh::Halfedge & hh = mesh->halfedge(*e_it,0);

            const Surface_mesh::Vertex & v0 = mesh->from_vertex(hh);
            const Surface_mesh::Vertex & v1 = mesh->to_vertex(hh);

            const Vector3 vec = points[v1] - points[v0];

            const Scalar edgeLength = vec.squaredNorm();

            // Keep originally short edges, if requested
            bool hadFeature = efeature[*e_it];
            if ( isKeepShortEdges && hadFeature ) continue;

            // edge too short but don't try to collapse edges that have length 0
            if ( (edgeLength < _minEdgeLengthSqr) && (edgeLength > std::numeric_limits<Scalar>::epsilon()) ) {

                //check if the collapse is ok
                const Vector3 & B = points[v1];

                bool collapse_ok = true;

            for( Surface_mesh::Halfedge hvit: mesh->halfedges(v0) ) {
                    Scalar d = (B - points[ mesh->to_vertex(hvit) ]).squaredNorm();

                    if ( d > _maxEdgeLengthSqr || mesh->is_boundary( mesh->edge( hvit ) ) || efeature[mesh->edge(hvit)] ) {
                        collapse_ok = false;
                        break;
                    }
                }

                if( collapse_ok && mesh->is_collapse_ok(hh) ) {
                    mesh->collapse( hh );
                    n_collapsed++;
                    finished = false;
                }
            }
        }
    }

    *myout << "    collapsed " << n_collapsed << " edges" << std::endl;
    
    mesh->remove_edge_property(checked);
    mesh->garbage_collection();
}

void IsotropicRemesher::equalizeValences(){
    *myout << __FUNCTION__ << std::endl;
    
    Surface_mesh::Edge_iterator e_it;
    Surface_mesh::Edge_iterator e_end = mesh->edges_end();

    for (e_it = mesh->edges_begin(); e_it != e_end; ++e_it) {

        if ( !mesh->is_flip_ok(*e_it) ) continue;
        if ( efeature[*e_it] ) continue;

        const Surface_mesh::Halfedge & h0 = mesh->halfedge( *e_it, 0 );
        const Surface_mesh::Halfedge & h1 = mesh->halfedge( *e_it, 1 );

        if (h0.is_valid() && h1.is_valid()) {
            if (mesh->face(h0).is_valid() && mesh->face(h1).is_valid()) {
                //get vertices of corresponding faces
                const Surface_mesh::Vertex & a = mesh->to_vertex(h0);
                const Surface_mesh::Vertex & b = mesh->to_vertex(h1);
                const Surface_mesh::Vertex & c = mesh->to_vertex(mesh->next_halfedge(h0));
                const Surface_mesh::Vertex & d = mesh->to_vertex(mesh->next_halfedge(h1));

                const int deviation_pre =  abs((int)(mesh->valence(a) - targetValence(a)))
                                           +abs((int)(mesh->valence(b) - targetValence(b)))
                                           +abs((int)(mesh->valence(c) - targetValence(c)))
                                           +abs((int)(mesh->valence(d) - targetValence(d)));
                mesh->flip(*e_it);

                const int deviation_post = abs((int)(mesh->valence(a) - targetValence(a)))
                                           +abs((int)(mesh->valence(b) - targetValence(b)))
                                           +abs((int)(mesh->valence(c) - targetValence(c)))
                                           +abs((int)(mesh->valence(d) - targetValence(d)));

                if (deviation_pre <= deviation_post)
                    mesh->flip(*e_it);
            }
        }
    }
}

///returns 4 for boundary vertices and 6 otherwise
inline int IsotropicRemesher::targetValence(const Surface_mesh::Vertex& _vh ) {
    if (isBoundary(_vh))
        return 4;
    else
        return 6;
}

inline bool IsotropicRemesher::isBoundary(const Surface_mesh::Vertex& _vh ) {
    for(Surface_mesh::Halfedge hvit: mesh->halfedges(_vh) ) {
        if ( mesh->is_boundary( mesh->edge( hvit ) ) )
            return true;
    }
    return false;
}

inline bool IsotropicRemesher::isFeature(const Surface_mesh::Vertex& _vh ) {
    for(Surface_mesh::Halfedge hvit: mesh->halfedges(_vh) ) {
        if(efeature[mesh->edge(hvit)])
            return true;
    }

    return false;
}

void IsotropicRemesher::tangentialRelaxation() {
    *myout << __FUNCTION__ << std::endl;
    
    ///--- Tangential relaxation needs vertex normals
    mesh->update_face_normals();
    mesh->update_vertex_normals();

    Vector3VertexProperty q = mesh->vertex_property<Vector3>("v:q");
    Vector3VertexProperty normal = mesh->vertex_property<Vector3>(VNORMAL);

    Surface_mesh::Vertex_iterator v_it;
    Surface_mesh::Vertex_iterator v_end = mesh->vertices_end();

    //first compute barycenters
    for (v_it = mesh->vertices_begin(); v_it != v_end; ++v_it) {

        Vector3 tmp(0,0,0);
        unsigned int N = 0;

        for( Surface_mesh::Halfedge hvit: mesh->halfedges(*v_it) ) {
            tmp += points[ mesh->to_vertex(hvit) ];
            N++;
        }

        if (N > 0)
            tmp /= (Scalar) N;

        q[*v_it] = tmp;
    }

    //move to new position
    for (v_it = mesh->vertices_begin(); v_it != v_end; ++v_it) {
        if ( !isBoundary(*v_it) && !isFeature(*v_it) ) {
            if(reproject_on_tanget)
                points[*v_it] = q[*v_it] + (dot(normal[*v_it], Vector3(points[*v_it] - q[*v_it]) ) * normal[*v_it]);
            else
                points[*v_it] = q[*v_it];
        }
    }

    mesh->remove_vertex_property(q);
}

Vector3 IsotropicRemesher::findNearestPoint(Surface_mesh& original_mesh, const Vector3& _point, Surface_mesh::Face& _fh, Scalar& /*=*/ _dbest) {
    Vector3VertexProperty orig_points = original_mesh.vertex_property<Vector3>( VPOINT );

    Vector3 p_best(nan(), nan(), nan());
    Scalar d_best = inf();
    Surface_mesh::Face fh_best;

    // exhaustive search
    for(Surface_mesh::Face f: original_mesh.faces()) {
        Surface_mesh::Vertex_around_face_circulator cfv_it = original_mesh.vertices(f);

        // Assume triangular
        const Vector3& pt0 = orig_points[     *cfv_it];
        const Vector3& pt1 = orig_points[ *(++cfv_it)];
        const Vector3& pt2 = orig_points[ *(++cfv_it)];

        Vector3 ptn = _point;

        //Surface_mesh::Scalar d = distPointTriangleSquared( _point, pt0, pt1, pt2, ptn );
        Scalar d = ClosestPointTriangle( _point, pt0, pt1, pt2, ptn );

        if( d < d_best) {
            d_best = d;
            p_best = ptn;

            fh_best = f;
        }
    }

    // return face
    _fh = fh_best;

    // return distance
    _dbest = sqrt(d_best);

    return p_best;
}

void IsotropicRemesher::projectToSurface() {
    *myout << __FUNCTION__ << std::endl;
    
#ifdef WITH_CGAL
    for(Surface_mesh::Vertex v: mesh->vertices())
        points[v] = searcher.closest_point(points[v]);
#else
    Surface_mesh::Vertex_iterator v_it;
    Surface_mesh::Vertex_iterator v_end = mesh->vertices_end();
    for (v_it = mesh->vertices_begin(); v_it != v_end; ++v_it) {
        if (isBoundary(*v_it)) continue;
        if ( isFeature(*v_it)) continue;

        Vector3 p = points[*v_it];

        Surface_mesh::Face fhNear;
        Scalar distance;

        Vector3 pNear = findNearestPoint(copy, p, fhNear, distance);

        points[*v_it] = pNear;
    }
#endif
}

void IsotropicRemesher::execute(){
    *myout << __FUNCTION__ << std::endl;
    phase_analyze();
    phase_remesh();
}

void IsotropicRemesher::phase_analyze(){
    // Conver to radians
    Scalar TH = deg_to_rad(sharp_feature_deg);

    ///--- Identify feature edges
    for(Surface_mesh::Edge e: mesh->edges()) {
        Scalar dihedral = calc_dihedral_angle(*mesh, mesh->halfedge(e,0));
        if(abs(dihedral)>TH)
            efeature[e] = true;
    }
    
    ///--- Mark short edges as features
    if(keep_short_edges){
        for(Surface_mesh::Edge e: mesh->edges()) {
            const Surface_mesh::Halfedge & hh = mesh->halfedge( e, 0 );
            const Surface_mesh::Vertex & v0 = mesh->from_vertex(hh);
            const Surface_mesh::Vertex & v1 = mesh->to_vertex(hh);
            const Vector3 vec = points[v1] - points[v0];

            if (vec.norm() <= longest_edge_length)
                efeature[e] = true;
        }
    }

    ///--- Count num features
    int n_efeature = 0;
    for(Surface_mesh::Edge e: mesh->edges())
        if(efeature[e])
            n_efeature++;
    *myout << "#edges: " << mesh->n_edges() << " #features: " << n_efeature << std::endl;

}

void IsotropicRemesher::phase_remesh(){
    const Scalar low  = (4.0 / 5.0) * longest_edge_length;
    const Scalar high = (4.0 / 3.0) * longest_edge_length;
    
    for(int i = 0; i < num_iterations; i++) {
        *myout << "---------------------------------------------" << std::endl;
        *myout << "Iteration: " << (i+1) << "/" << num_iterations <<
                  " on mesh with #vertices: " << mesh->n_vertices() << std::endl;
        splitLongEdges(high);
        collapseShortEdges(low, high, keep_short_edges);
        equalizeValences();
        tangentialRelaxation();
        if(reproject_to_surface)
            projectToSurface();
    }
}

//=============================================================================
} // opengp::
//=============================================================================
