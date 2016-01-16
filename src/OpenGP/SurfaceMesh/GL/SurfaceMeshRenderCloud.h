#pragma once
#include <vector>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/PointsRenderer.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/Eigen.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderCloud : public PointsRenderer{
public:
    SurfaceMeshRenderCloud(SurfaceMesh& mesh){
        Mat3xN mat = vertices_matrix(mesh);
        PointsRenderer::load(mat);   
    }
};
    
//=============================================================================
} // namespace OpenGP
//=============================================================================
