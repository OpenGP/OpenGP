//== INCLUDES =================================================================

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/IO/IO.h>
#include <cstdio>

//=============================================================================
namespace OpenGP {
//=============================================================================

bool read_obj(SurfaceMesh& mesh, const std::string& filename) {
    char s[200];
    float  x, y, z;
    std::vector<SurfaceMesh::Vertex>  vertices;
    std::vector<Vec3> all_tex_coords;   //individual texture coordinates
    std::vector<int> halfedge_tex_idx; //texture coordinates sorted for halfedges
    SurfaceMesh::Halfedge_property <Vec3> tex_coords = mesh.halfedge_property<Vec3>("h:texcoord");
    bool with_tex_coord=false;

    // clear mesh
    mesh.clear();

    // open file (in ASCII mode)
    FILE* in = fopen(filename.c_str(), "r");
    if (!in) return false;

    // clear line once
    memset(&s, 0, 200);

    // pre-parse to find out the number of vertices
    {
        uint vnormal_counter = 0; // number of vertex normals parsed
        while (in && !feof(in) && fgets(s, 200, in)) {
            if (s[0] == '#' || isspace(s[0])) continue; // comment
            else if (strncmp(s, "v ", 2) == 0) { if (sscanf(s, "v %f %f %f", &x, &y, &z)) mesh.add_vertex(Vec3(0,0,0)); }
            else if (strncmp(s, "vn ", 3) == 0) { if (sscanf(s, "vn %f %f %f", &x, &y, &z)) vnormal_counter++; }
            else continue;
        }

        // If we have read any vertex normals, it must match the number of vertices
        // and in this case allocate memory for normals
        if (vnormal_counter!=0){
            assert(vnormal_counter==mesh.n_vertices());
            mesh.add_vertex_property<Vec3>("v:normal");
        }
        
        // Start from the beginning again
        in = freopen(filename.c_str(),"r",in);
    }

    // parse line by line (currently only supports vertex positions & faces
    uint vpoint_counter = 0;  //< number of vertex positions parsed
    uint vnormal_counter = 0; //< number of vertex normals parsed
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
    auto vnormals = mesh.get_vertex_property<Vec3>("v:normal");
    while (in && !feof(in) && fgets(s, 200, in)) {
        // comment
        if (s[0] == '#' || isspace(s[0])) continue;

        // vertex
        else if (strncmp(s, "v ", 2) == 0) {
            if (sscanf(s, "v %f %f %f", &x, &y, &z)) {
                vpoints[ SurfaceMesh::Vertex(vpoint_counter++) ] = Vec3(x,y,z);
                // mesh.add_vertex(Vec3(x,y,z));
            }
        }
        // normal
        else if (strncmp(s, "vn ", 3) == 0) {
            int n_read = sscanf(s, "vn %f %f %f", &x, &y, &z);
            assert((n_read==0) || (n_read==3));
            if (n_read) {
                // note: problematic as it can be either a vertex property when interpolated or a halfedge property for hard edges
                assert(vnormal_counter<mesh.n_vertices());
                vnormals[ SurfaceMesh::Vertex(vnormal_counter) ] = Vec3(x,y,z);
                vnormal_counter++;
            }
        }

        // texture coordinate
        else if (strncmp(s, "vt ", 3) == 0) {
            if (sscanf(s, "vt %f %f", &x, &y)) {
                z=1;
                all_tex_coords.push_back(Vec3(x,y,z));
            }
        }

        // face
        else if (strncmp(s, "f ", 2) == 0) {
            int component(0), nV(0);
            bool endOfVertex(false);
            char* p0, *p1(s+1);

            vertices.clear();
            halfedge_tex_idx.clear();

            // skip white-spaces
            while (*p1==' ') ++p1;

            while (p1) {
                p0 = p1;

                // overwrite next separator

                // skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
                while (*p1!='/' && *p1!='\r' && *p1!='\n' && *p1!=' ' && *p1!='\0') ++p1;

                // detect end of vertex
                if (*p1 != '/') {
                    endOfVertex = true;
                }

                // replace separator by '\0'
                if (*p1 != '\0') {
                    *p1 = '\0';
                    p1++; // point to next token
                }

                // detect end of line and break
                if (*p1 == '\0' || *p1 == '\n') {
                    p1 = 0;
                }

                // read next vertex component
                if (*p0 != '\0') {
                    switch (component) {
                        case 0: { // vertex
                            vertices.push_back( SurfaceMesh::Vertex(atoi(p0) - 1) );
                            break;
                        }
                        case 1: { // texture coord
                            int idx = atoi(p0)-1;
                            halfedge_tex_idx.push_back(idx);
                            with_tex_coord=true;
                            break;
                        }
                        case 2: // normal
                            break;
                    }
                }

                ++component;

                if (endOfVertex) {
                    component = 0;
                    nV++;
                    endOfVertex = false;
                }
            }

            SurfaceMesh::Face f=mesh.add_face(vertices);


            // add texture coordinates
            if (with_tex_coord) {
                SurfaceMesh::Halfedge_around_face_circulator h_fit = mesh.halfedges(f);
                SurfaceMesh::Halfedge_around_face_circulator h_end = h_fit;
                unsigned v_idx =0;
                do {
                    tex_coords[*h_fit]=all_tex_coords.at(halfedge_tex_idx.at(v_idx));
                    ++v_idx;
                    ++h_fit;
                } while (h_fit!=h_end);
            }
        }
        // clear line
        memset(&s, 0, 200);
    }

    fclose(in);
    return true;
}


//-----------------------------------------------------------------------------


bool write_obj(const SurfaceMesh& mesh, const std::string& filename) {
    typedef Vec3 TextureCoordinate;

    FILE* out = fopen(filename.c_str(), "w");
    if (!out)
        return false;

    // comment
    fprintf(out, "# OBJ export from SurfaceMesh\n");

    //vertices
    SurfaceMesh::Vertex_property<Vec3> points = mesh.get_vertex_property<Vec3>("v:point");
    for (SurfaceMesh::Vertex_iterator vit=mesh.vertices_begin(); vit!=mesh.vertices_end(); ++vit) {
        const Vec3& p = points[*vit];
        fprintf(out, "v %.10f %.10f %.10f\n", p[0], p[1], p[2]);
    }

    //normals
    SurfaceMesh::Vertex_property<Vec3> normals = mesh.get_vertex_property<Vec3>("v:normal");
    if (normals) {
        for (SurfaceMesh::Vertex_iterator vit=mesh.vertices_begin(); vit!=mesh.vertices_end(); ++vit) {
            const Vec3& p = normals[*vit];
            fprintf(out, "vn %.10f %.10f %.10f\n", p[0], p[1], p[2]);
        }
    }

    //optionally texture coordinates
    // do we have them?
    std::vector<std::string> h_props= mesh.halfedge_properties();
    bool with_tex_coord = false;
    std::vector<std::string>::iterator h_prop_end = h_props.end();
    std::vector<std::string>::iterator h_prop_start= h_props.begin();
    while (h_prop_start!=h_prop_end) {
        if (0==(*h_prop_start).compare("h:texcoord")) {
            with_tex_coord=true;
        }
        ++h_prop_start;
    }

    //if so then add
    if (with_tex_coord) {
        SurfaceMesh::Halfedge_property<TextureCoordinate> tex_coord = mesh.get_halfedge_property<TextureCoordinate>("h:texcoord");
        for (SurfaceMesh::Halfedge_iterator hit=mesh.halfedges_begin(); hit!=mesh.halfedges_end(); ++hit) {
            const TextureCoordinate& pt = tex_coord[*hit];
            fprintf(out, "vt %.10f %.10f %.10f\n", pt[0], pt[1], pt[2]);
        }
    }

    //faces
    for (SurfaceMesh::Face_iterator fit=mesh.faces_begin(); fit!=mesh.faces_end(); ++fit) {
        fprintf(out, "f");
        SurfaceMesh::Vertex_around_face_circulator fvit=mesh.vertices(*fit), fvend=fvit;
        SurfaceMesh::Halfedge_around_face_circulator fhit=mesh.halfedges(*fit);
        do {
            if (with_tex_coord) {
                // write vertex index, tex_coord index and normal index
                fprintf(out, " %d/%d/%d", (*fvit).idx()+1, (*fhit).idx()+1, (*fvit).idx()+1);
                ++fhit;
            } else {
                // write vertex index and normal index
                fprintf(out, " %d//%d", (*fvit).idx()+1, (*fvit).idx()+1);
            }
        } while (++fvit != fvend);
        fprintf(out, "\n");
    }

    fclose(out);
    return true;
}


//=============================================================================
} // namespace OpenGP
//=============================================================================
