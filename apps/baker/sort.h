#include <OpenGP/Surface_mesh.h>

//=============================================================================
namespace opengp{
//=============================================================================

namespace {
    typedef Surface_mesh::Vertex Vertex;
} /// ::anonymous

/// @par mesh, not const as property is added (and then deleted)
Surface_mesh sort(Surface_mesh& mesh, int dimension=0){
    Surface_mesh sorted;
    
    ///--- Sort vertices accordin to a criteria
    {
        class Sort_Functor{
        private:
            Surface_mesh::Vertex_property<Vec3> vpoints;
            int sort_dimension;
        public:
            Sort_Functor(const Surface_mesh& mesh, int dimension=0){
                assert(dimension>=0 && dimension<3); ///< x,y,z
                sort_dimension = dimension;
                vpoints = mesh.get_vertex_property<Vec3>("v:point");            
            }
            bool operator() (const Vertex& a, const Vertex& b){
                return vpoints[a][sort_dimension] < vpoints[b][sort_dimension];
            }
        };
               
        std::vector<Vertex> sorted_vertices;
        for(auto vid: mesh.vertices())
            sorted_vertices.push_back(vid);
        std::sort(sorted_vertices.begin(), sorted_vertices.end(), Sort_Functor(mesh,dimension));
       
#define SHOW_DEBUG_OUTPUT
#ifdef SHOW_DEBUG_OUTPUT
        {
            auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
            std::cout << std::setprecision(2) << std::fixed << std::showpos;
            for(auto v: sorted_vertices){
                fprintf(stdout, "%2d", v.idx());
                std::cout << " " << vpoints[v].transpose() << std::endl;
            }
            std::cout << std::noshowpos;
        }
#endif
        
        ///--- Re-assemble sorted mesh
        auto vpoints = mesh.get_vertex_property<Vec3>("v:point");       
        auto vidxs = mesh.add_vertex_property<Vertex>("v:idxs"); ///< remapped idx

        /// Insert vertices (and store remapped positions)
        for(auto v: sorted_vertices)
            vidxs[v] = sorted.add_vertex( vpoints[v] );
        
        /// Insert faces (w.r.t. remapped positions)
        for(auto f: mesh.faces()){
            // extract remapped indexes from faces
            std::vector<Vertex> v_on_f;
            for(auto v: mesh.vertices(f))
                v_on_f.push_back( vidxs[v] ); 
            // Add face to the new mesh
            sorted.add_face(v_on_f);
        }
    }
    
    return sorted; 
}
   
//=============================================================================
} // namespace opengp
//=============================================================================
