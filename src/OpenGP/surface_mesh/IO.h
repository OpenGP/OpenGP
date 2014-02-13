//=============================================================================
// Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen
// Copyright (C) 2011-2013 by Graphics & Geometry Group, Bielefeld University
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation, version 2.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================


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
