#pragma once
#include <vector>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/SegmentsRenderer.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/Eigen.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderVertexNormals : public SegmentsRenderer{
protected:
    SurfaceMesh& mesh;
    
public:
    SurfaceMeshRenderVertexNormals(SurfaceMesh& mesh, Scalar offset=.1) : mesh(mesh){
        this->color = Vec3(0,0,1); ///< blue normals
        Mat3xN P1 = OpenGP::vertices_matrix(mesh);
        Mat3xN N = OpenGP::normals_matrix(mesh);
        Mat3xN P2 = P1 + offset*N;
        SegmentsRenderer::load(P1,P2);
    }
    Box3 bounding_box(){ return OpenGP::bounding_box(mesh); }
};
    
//=============================================================================
} // namespace OpenGP
//=============================================================================
