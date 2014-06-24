#pragma once
#include <QGLViewer/qglviewer.h>
#include <QMouseEvent>
#include <QGLViewer/manipulatedCameraFrame.h>

class QGLMeshLabViewer : public QGLViewer {
protected:
    QGLMeshLabViewer(){
        init();
    }
    
    void init(){
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
        /// MeshLAB like double click action
        {
            /// Modified version of "RAP_FROM_PIXEL"
            if(!camera()->setPivotPointFromPixel(e->pos()))
                return; // do nothing
            camera()->setSceneCenter( camera()->pivotPoint() );
            /// Stolen from "centerScene"
            camera()->frame()->projectOnLine(sceneCenter(), camera()->viewDirection());
            setVisualHintsMask(1);
            update();
        }
    }
};
