#pragma once
#include <vector>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/SegmentsRenderer.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/Eigen.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderVertexNormals : public SegmentsRenderer{
public:
    SurfaceMeshRenderVertexNormals(SurfaceMesh& mesh, Scalar offset=.1){
        this->color = Vec3(0,0,1); ///< blue normals
        Mat3xN P1 = OpenGP::vertices_matrix(mesh);
        Mat3xN N = OpenGP::normals_matrix(mesh);
        Mat3xN P2 = P1 + offset*N;
        SegmentsRenderer::load(P1,P2);
    }
};
    
//=============================================================================
} // namespace OpenGP
//=============================================================================
