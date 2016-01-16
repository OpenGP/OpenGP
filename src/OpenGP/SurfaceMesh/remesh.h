#pragma once
#include <OpenGP/headeronly.h>
#include <OpenGP/types.h>
#include <OpenGP/NullStream.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

#ifdef WITH_CGAL
    #include <OpenGP/SurfaceMesh/Eigen.h>
    #include <OpenGP/CGAL/AABBSearcher.h>
#endif

//=============================================================================
namespace OpenGP{
//=============================================================================



class IsotropicRemesher{
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
    
private:
    SurfaceMesh::Vertex_property<Vec3> points;
    SurfaceMesh::Edge_property<bool> efeature;
    SurfaceMesh* mesh = NULL;
    SurfaceMesh copy;
public:
    IsotropicRemesher(SurfaceMesh& _mesh){
        this->mesh = &_mesh;
        efeature = mesh->edge_property<bool>("e:feature", false);
        points = mesh->vertex_property<Vec3>(VPOINT);
         
        if(reproject_to_surface)
            copy = *mesh; ///< deep copy
        
#ifdef WITH_CGAL
        VerticesMatrixMap vertices = vertices_matrix(*mesh);
        TrianglesMatrix faces = faces_matrix(*mesh);
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
    AABBSearcher<VerticesMatrixMap, TrianglesMatrix> searcher;
#endif
    
/// @{ utilities
private:
    void splitLongEdges(Scalar maxEdgeLength);
    void collapseShortEdges(const Scalar _minEdgeLength, const Scalar _maxEdgeLength, bool keep_short_edges);
    void equalizeValences();
    void tangentialRelaxation();
    void projectToSurface();
    int targetValence(const SurfaceMesh::Vertex &_vh);
    bool isBoundary(const SurfaceMesh::Vertex &_vh);
    bool isFeature(const SurfaceMesh::Vertex &_vh);
    Vec3 findNearestPoint(SurfaceMesh& orginal_mesh, const Vec3& _point, SurfaceMesh::Face& _fh, Scalar & _dbest);
/// @} utilities
};

//=============================================================================
} // OpenGP::
//=============================================================================

// Header only support
#ifdef HEADERONLY
    #include "remesh.cpp"
#endif
