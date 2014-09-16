#ifndef SURFACE_MESH_IO_H
#define SURFACE_MESH_IO_H


//== INCLUDES =================================================================


#include <OpenGP/Surface_mesh.h>
#include <string>


//== NAMESPACE ================================================================


namespace opengp {


//=============================================================================


HEADERONLY_INLINE bool read_mesh(Surface_mesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_off(Surface_mesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_obj(Surface_mesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_stl(Surface_mesh& mesh, const std::string& filename);

HEADERONLY_INLINE bool write_mesh(const Surface_mesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_off(const Surface_mesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_obj(const Surface_mesh& mesh, const std::string& filename);

/// Private helper function
template <typename T> void read(FILE* in, T& t)
{
    size_t n_items(0);
    n_items = fread((char*)&t, 1, sizeof(t), in);
    assert(n_items > 0);
}

//=============================================================================
} // namespace opengp

#ifdef HEADERONLY
    #include "IO.cpp"
    #include "IO_obj.cpp"
    #include "IO_off.cpp"
    #include "IO_poly.cpp"
    #include "IO_stl.cpp"
#endif

//=============================================================================
#endif // SURFACE_MESH_IO_H
//=============================================================================
