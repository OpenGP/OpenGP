#pragma once

#define _USE_MATH_DEFINS
#include <math.h>

#include <OpenGP/types.h>
#include <Eigen/Geometry>

//=============================================================================
namespace OpenGP {
//=============================================================================

class ArcballCamera{
private:
    const Scalar dollyStep = 0.5f;
    Scalar dolly;
    Vec2   lastPos;
    Vec3   tumbleVector; ///< Euler rotation angles XYZ
    Mat4x4 dollyMatrix;
    Mat4x4 tumbleMatrix;
   
public:
    ArcballCamera(){
        resetCamera();
        resetMatrices();
    }

    void mouse_down_tumble(const Vec2& point){
        lastPos = point;
    }

    void mouse_drag_tumble(const Vec2& point){
        using namespace Eigen;

        float deltaX = point[0] - lastPos[0];
        float deltaY = point[1] - lastPos[1];

        tumbleVector[0] += 0.005f * deltaY;
        tumbleVector[1] += 0.005f * deltaX;
        tumbleMatrix =
            Affine3f(AngleAxis<Scalar>(radians(tumbleVector[0]), Vec3(1, 0, 0))).matrix() *
            Affine3f(AngleAxis<Scalar>(radians(tumbleVector[1]), Vec3(0, 1, 0))).matrix();
    }

    void mouse_scroll(float deltaY){
        using namespace Eigen;
        dolly -= dollyStep * deltaY;
        dollyMatrix = Affine3f(Translation<Scalar, 3>(0, 0, -1.0f * dolly)).matrix();
    }

    void resetCamera(){
        dolly = 2.0f;
        tumbleVector = Vec3(0.0f, 0.0f, 0.0f);
        resetMatrices();
    }

    Mat4x4 getCameraMatrix(){
        return dollyMatrix  * tumbleMatrix;
    }

private:
    void resetMatrices(){
        using namespace Eigen;
        dollyMatrix = Affine3f(Translation<float, 3>(0, 0, -1.0f * dolly)).matrix();
        tumbleMatrix =
            Affine3f(AngleAxis<float>(radians(tumbleVector[0]), Vec3(1, 0, 0))).matrix() *
            Affine3f(AngleAxis<float>(radians(tumbleVector[1]), Vec3(0, 1, 0))).matrix();
    }

    float radians(float degrees)
    {
        return (float)(M_PI / 180) * degrees;
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
