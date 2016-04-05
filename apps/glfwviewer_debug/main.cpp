#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/MATLAB/random.h>
#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/GL/PointsRenderer.h>
#include <OpenGP/GL/SegmentsRenderer.h>

using namespace OpenGP;
int main(int, char**) {
    TrackballWindow window(__FILE__,640,480);

    int num_points = 500;

    ///--- Colored points rendering
    MatMxN P = .3f*OpenGP::randn(3,num_points);
    PointsRenderer cloud(P);
    window.scene.add(cloud);
    cloud.set_colors( OpenGP::rand(3, P.cols()) );
    cloud.set_vsize(10);

    ///--- Line segments renderering
    MatMxN Z = MatMxN::Zero(3,num_points);
    SegmentsRenderer segments(Z,P);
    segments.color = Vec3(1,0,0);
    window.scene.add(segments);

    return window.run();
}


