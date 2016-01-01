#pragma once

#include <vector>
#include <Eigen/Core>
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>

#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>

//=============================================================================
namespace opengp {
//=============================================================================

class SceneObject{
public:
    ShaderProgram program;
    /// Model Transformation Matrix
    Mat4x4 model = Mat4x4::Identity();

    virtual ~SceneObject(){}
    virtual void init() = 0;
    virtual void display() = 0;
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
