#include "OpenGP/Surface_mesh.h"

/// Same name as C++ element (but no namespace)
class Surface_mesh{
private:
    /// Hide too complex structure by encapsulation
    opengp::Surface_mesh mesh;
public:
    void read(const char* path){ mesh.read(path); }
    void property_stats(){ mesh.property_stats(); }
    int n_vertices(){ return mesh.n_vertices(); }
};