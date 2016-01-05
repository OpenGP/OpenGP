#include "OpenGP/SurfaceMesh/SurfaceMesh.h"

/// Same name as C++ element (but no namespace)
class Surface_mesh{
private:
    /// Hide too complex structure by encapsulation
    OpenGP::Surface_mesh mesh;

public:
    void read(const char* path){ mesh.read(path); }
    void property_stats(){ mesh.property_stats(); }
    int n_vertices(){ return mesh.n_vertices(); }

    void set_vertices(float* matrix, int rows, int cols){
        auto points = mesh.get_vertex_property<OpenGP::Vec3>("v:point");
        assert(cols==mesh.n_vertices());
        assert(rows==3);
        int XOFF=0*cols, YOFF=1*cols, ZOFF=2*cols;
        for(auto vit: mesh.vertices()){
            int x = matrix[XOFF+vit.idx()];
            int y = matrix[YOFF+vit.idx()];
            int z = matrix[ZOFF+vit.idx()];
            points[vit] = OpenGP::Vec3(x,y,z);
        }
    }
    void get_vertices(float** matrix, int *rows, int* cols){
        // Allocate the necessary memory
        *rows = 3;
        *cols = mesh.n_vertices();
        int nels = (*rows)*(*cols);
        *matrix = new float[nels];
        assert((*matrix)!=NULL);

        // Initialization with vertex coordinates
        auto points = mesh.get_vertex_property<OpenGP::Vec3>("v:point");
        int XOFF=0*(*cols), YOFF=1*(*cols), ZOFF=2*(*cols);
        for(auto vit: mesh.vertices()){
            // std::cout << vit.idx() << " " << points[vit].transpose() << std::endl;
            (*matrix)[XOFF+vit.idx()] = points[vit].x();
            (*matrix)[YOFF+vit.idx()] = points[vit].y();
            (*matrix)[ZOFF+vit.idx()] = points[vit].z();
        }
    }
};

