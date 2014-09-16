#pragma once
#include <OpenGP/headeronly.h>
#include <OpenGP/types.h>
#include <OpenGP/NullStream.h>
#include <OpenGP/Surface_mesh.h>

#ifdef WITH_CGAL
    #include <OpenGP/surface_mesh/eigen.h>
    #include <OpenGP/CGAL/AABBSearcher.h>
#endif

//=============================================================================
namespace opengp{
//=============================================================================

/// @{ @todo centralize these definitions elsewhere
    const std::string VPOINT = "v:point";           ///< vertex coordinates
    const std::string VNORMAL = "v:normal";         ///< vertex normals
    const std::string VCOLOR = "v:color";           ///< vertex color
    const std::string VAREA = "v:area";             ///< vertex areas
    const std::string VQUALITY = "v:quality";       ///< vertex quality
    const std::string FNORMAL = "f:normal";         ///< face normals
    const std::string FAREA = "f:area";             ///< face area
    const std::string ELENGTH = "e:length";         ///< edge length
    const std::string FSELECTED = "f:selected";     ///< is face selected?    
/// @}

/// @{ @todo get rid of these defines!
typedef Vec3 Vector3;
typedef Surface_mesh::Vertex_property<Vector3>  Vector3VertexProperty; 
typedef Surface_mesh::Edge_property<bool>       BoolEdgeProperty; 
typedef Surface_mesh::Face_property<Vector3>    Vector3FaceProperty;
/// @}

class IsotropicRemesher{
private:
    Vector3VertexProperty points;
    BoolEdgeProperty efeature;
    Surface_mesh* mesh = NULL;
    Surface_mesh copy;
public:
    IsotropicRemesher(Surface_mesh& _mesh){
        this->mesh = &_mesh;
        efeature = mesh->edge_property<bool>("e:feature", false);
        points = mesh->vertex_property<Vector3>(VPOINT);
         
        if(reproject_to_surface)
            copy = *mesh; ///< deep copy
        
#ifdef WITH_CGAL
        VerticesMatrixMap vertices = vertices_matrix(*mesh);
        FacesMatrix faces = faces_matrix(*mesh);
        std::cout << vertices.cols() << " " << vertices.rows() << std::endl;
        std::cout << faces.cols() << " " << faces.rows() << std::endl;
        searcher.build(vertices, faces);
#endif
    }
    ~IsotropicRemesher(){
        mesh->remove_edge_property(efeature);
    }
   
/// @{ core methods
public:    
    void execute();
protected:
    void phase_analyze();
    void phase_remesh();
/// @} 

/// @{ algorithm parameters
private:
    NullStream nullstream;    
public:
    /// Where should I send the algorithm output to?
    std::ostream* myout = &(nullstream);
    /// What's the dihedral angle (degrees) of a feature?
    Scalar sharp_feature_deg = 44.0; ///< degrees
    /// How many iterations of merge/split/flip/relax should I run?
    Scalar num_iterations = 10;
    /// What's the largest admissible edge?
    Scalar longest_edge_length = nan();
    /// Should I mark short edges as features?
    bool keep_short_edges = false;
    /// After tangentially relaxing vertices, should I reproject vertices on the tangent space
    /// defined by vertex + vertex normal?
    bool reproject_on_tanget = true;
    /// After tangentially relaxing vertices, should I project on the original surface (slow, query an AABB search tree)
    bool reproject_to_surface = false;     
/// @}
    
#ifdef WITH_CGAL
private:
    AABBSearcher<VerticesMatrixMap, FacesMatrix> searcher;
#endif
    
/// @{ utilities
private:
    void splitLongEdges(Scalar maxEdgeLength);
    void collapseShortEdges(const Scalar _minEdgeLength, const Scalar _maxEdgeLength, bool keep_short_edges);
    void equalizeValences();
    void tangentialRelaxation();
    void projectToSurface();
    int targetValence(const Surface_mesh::Vertex &_vh);
    bool isBoundary(const Surface_mesh::Vertex &_vh);
    bool isFeature(const Surface_mesh::Vertex &_vh);
    Vector3 findNearestPoint(Surface_mesh& orginal_mesh, const Vector3& _point, Surface_mesh::Face& _fh, Scalar & _dbest);
/// @} utilities
};

//=============================================================================
} // opengp::
//=============================================================================

// Header only support
#ifdef HEADERONLY
    #include "remesh.cpp"
#endif
