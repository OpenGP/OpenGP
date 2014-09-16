#pragma once
#include <QGLWidget>
#include <OpenGP/GL/QGLFormat32.h>

//== NAMESPACE ================================================================
namespace opengp {
//=============================================================================

class QGLWidget32 : public QGLWidget{ 
public:
    QGLWidget32(QWidget* parent=0) : 
        QGLWidget(QGLFormat32(), parent){}
};

//=============================================================================
} // namespace opengp
//=============================================================================
