#pragma once
#include <vector>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/PointsRenderer.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/Eigen.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderCloud : public PointsRenderer{
private:
    SurfaceMesh& mesh;
public:
    SurfaceMeshRenderCloud(SurfaceMesh& mesh) : mesh(mesh){
        Mat3xN mat = vertices_matrix(mesh);
        PointsRenderer::load(mat);   
    }
    Box3 bounding_box(){ return OpenGP::bounding_box(mesh); }
};
    
//=============================================================================
} // namespace OpenGP
//=============================================================================
