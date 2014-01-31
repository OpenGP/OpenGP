// Copyright (C) 2014 by Andrea Tagliasacchi
// Barycenter example re-written with C++11 syntax

#include <surface_mesh/Surface_mesh.h>
using namespace surface_mesh;
int main(int /*argc*/, char** argv)
{
    Surface_mesh mesh;
    mesh.read(argv[1]);
    auto points = mesh.get_vertex_property<Point>("v:point");
    Point p(0,0,0);
    for(auto vit: mesh.vertices())
        p += points[vit];
    p /= mesh.n_vertices();
    
    std::cout << "barycenter: " << p << std::endl;
}
