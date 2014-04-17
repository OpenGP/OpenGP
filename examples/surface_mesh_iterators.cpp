#include <OpenGP/Surface_mesh.h>
using namespace opengp;

int main(int /*argc*/, char** argv)
{
    Surface_mesh mesh;

    mesh.read(argv[1]);

    float mean_valence = 0.0f;
    unsigned int vertex_valence;

    // instantiate iterator and circulators
    Surface_mesh::Vertex_iterator vit;
    Surface_mesh::Vertex_around_vertex_circulator vc, vc_end;

    // loop over all vertices
    for (vit = mesh.vertices_begin(); vit != mesh.vertices_end(); ++vit)
    {
        // initialize circulators
        vc = mesh.vertices(*vit);
        vc_end = vc;

        // reset counter
        vertex_valence = 0;

        // loop over all incident vertices
        do
        {
            ++vertex_valence;
        } while (++vc != vc_end);

        // sum up vertex valences
        mean_valence += vertex_valence;
    }

    mean_valence /= mesh.n_vertices();

    std::cout << "mean valence: " << mean_valence << std::endl;
}
