#pragma once
#include <OpenGP/types.h>
#include <QGLViewer/vec.h>

namespace qglviewer{
    Vec tr(opengp::Vec3 vec){ return Vec(vec.x(), vec.y(), vec.z()); }
} // qglviewer::
