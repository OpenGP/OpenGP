#pragma once
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <string>

//=============================================================================
namespace OpenGP {
//=============================================================================

HEADERONLY_INLINE bool read_mesh(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_off(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_obj(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_stl(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_mesh(const SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_off(const SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_obj(const SurfaceMesh& mesh, const std::string& filename);

/// Private helper function
template <typename T> void read(FILE* in, T& t)
{
    size_t n_items(0);
    n_items = fread((char*)&t, 1, sizeof(t), in);
    assert(n_items > 0);
}

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "IO.cpp"
    #include "IO_obj.cpp"
    #include "IO_off.cpp"
    #include "IO_poly.cpp"
    #include "IO_stl.cpp"
#endif
