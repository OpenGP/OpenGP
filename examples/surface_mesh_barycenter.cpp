// Copyright (C) 2013 by Graphics & Geometry Group, Bielefeld University
#include <OpenGP/Surface_mesh.h>
using namespace opengp;

int main(int /*argc*/, char** argv)
{
    Surface_mesh mesh;

    mesh.read(argv[1]);

    // get (pre-defined) property storing vertex positions
    Surface_mesh::Vertex_property<Point> points = mesh.get_vertex_property<Point>("v:point");

    Surface_mesh::Vertex_iterator vit, vend = mesh.vertices_end();

    Point p(0,0,0);

    for (vit = mesh.vertices_begin(); vit != vend; ++vit)
    {
        // access point property like an array
        p += points[*vit];
    }

    p /= mesh.n_vertices();

    std::cout << "barycenter: " << p << std::endl;
}
