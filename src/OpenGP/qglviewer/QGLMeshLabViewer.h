#pragma once
#include <Eigen/Dense>
#include <QMouseEvent>
#include <QGLViewer/manipulatedCameraFrame.h>
#include <QGLShaderProgram>
#include <QGLViewer/qglviewer.h>
#include <QMatrix4x4>
#include <QVector3D>


/// @brief Specialization of QGLViewer for OpenGL4 with actions that have been modified to make its default behavior match the one of MeshLab (i.e. double click to center scene, no spinning)
class QGLMeshLabViewer : public QGLViewer {
protected:
    /// Format class to enable OpenGL4 core profile 
    class OpenGL4Format : public QGLFormat{
    public:
        OpenGL4Format(){
            setVersion(3,3);
            setProfile(QGLFormat::CoreProfile);
            setSampleBuffers(true);     
        }
    };
    
protected:
    QGLMeshLabViewer() : QGLViewer(OpenGL4Format()){
        /// Disable QGLViewer's default "spin"
        camera()->frame()->setSpinningSensitivity(100); 
        
        /// Bindings @see QGLViewer::setDefaultMouseBindings()
        /// Extra behavior in this->mouseDoubleClickEvent()
        {
            /// Disable double click to align scene
            setMouseBinding(Qt::NoModifier, Qt::LeftButton, NO_CLICK_ACTION, true);   /// ALIGN_CAMERA
            setMouseBinding(Qt::ShiftModifier, Qt::RightButton, NO_CLICK_ACTION);     /// RAP_FROM_PIXEL
            setMouseBinding(Qt::NoModifier, Qt::MiddleButton, NO_CLICK_ACTION, true); /// ZOOM_TO_FIT
        }
    }
    
    void mouseDoubleClickEvent(QMouseEvent* e){
        //std::cout << __FUNCTION__ << std::endl;
        
        /// MeshLAB like double click action
        {
            /// Modified version of "RAP_FROM_PIXEL"
            if(!camera()->setPivotPointFromPixel(e->pos())){
                std::cout << "failed" << std::endl;
                return; // do nothing
            }
            camera()->setSceneCenter( camera()->pivotPoint() );
            /// Stolen from "centerScene"
            camera()->frame()->projectOnLine(sceneCenter(), camera()->viewDirection());
            setVisualHintsMask(1);
            update();
        }
    }
    
    static void setup_modelview(qglviewer::Camera* camera, QGLShaderProgram& program_){
        ///--- Fetch matrixes from trackball
        Eigen::Matrix4f MVP; 
        camera->getModelViewProjectionMatrix(MVP.data());
        MVP.transposeInPlace();
        Eigen::Matrix4f MV; 
        camera->getModelViewMatrix(MV.data());
        MV.transposeInPlace();
        
        // std::cout << "MVP:\n" << MVP << std::endl;
        // std::cout << "MV:\n" << MV << std::endl;
        
        ///--- Set shader variables
        program_.setUniformValue (program_.uniformLocation ("MVP"), QMatrix4x4(MVP.data()));
        program_.setUniformValue (program_.uniformLocation ("MV"), QMatrix4x4(MV.data()));   
    }
};
