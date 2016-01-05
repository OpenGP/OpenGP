#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/IO/IO.h>
#include <cstdio>

//=============================================================================
namespace OpenGP {
//=============================================================================

inline bool read_off_ascii(SurfaceMesh& mesh,
                    FILE* in,
                    const bool has_normals,
                    const bool has_texcoords,
                    const bool has_colors)
{
    typedef Vec3 Normal;
    typedef Vec3 TextureCoordinate;
    typedef Vec3 Color;
    
    char                 line[200], *lp;
    int                  nc;
    unsigned int         i, j, items, idx;
    unsigned int         nV, nF, nE;
    Vec3                 p, n, c;
    Vec2                 t;
    SurfaceMesh::Vertex v;
    
    // properties
    SurfaceMesh::Vertex_property<Normal>              normals;
    SurfaceMesh::Vertex_property<TextureCoordinate>  texcoords;
    SurfaceMesh::Vertex_property<Color>               colors;
    if (has_normals)   normals   = mesh.vertex_property<Normal>("v:normal");
    if (has_texcoords) texcoords = mesh.vertex_property<TextureCoordinate>("v:texcoord");
    if (has_colors)    colors    = mesh.vertex_property<Color>("v:color");

    // #Vertice, #Faces, #Edges
    items = fscanf(in, "%d %d %d\n", (int*)&nV, (int*)&nF, (int*)&nE);
    mesh.clear();
    mesh.reserve(nV, std::max(3*nV, nE), nF);


    // read vertices: pos [normal] [color] [texcoord]
    for (i=0; i<nV && !feof(in); ++i)
    {
        // read line
        lp = fgets(line, 200, in);
        lp = line;

        // position
        items = sscanf(lp, "%f %f %f%n", &p[0], &p[1], &p[2], &nc);
        assert(items==3);
        v = mesh.add_vertex((Vec3)p);
        lp += nc;

        // normal
        if (has_normals)
        {
            if (sscanf(lp, "%f %f %f%n", &n[0], &n[1], &n[2], &nc) == 3)
            {
                normals[v] = n;
            }
            lp += nc;
        }

        // color
        if (has_colors)
        {
            if (sscanf(lp, "%f %f %f%n", &c[0], &c[1], &c[2], &nc) == 3)
            {
                if (c[0]>1.0f || c[1]>1.0f || c[2]>1.0f) c *= (1.0/255.0);
                colors[v] = c;
            }
            lp += nc;
        }

        // tex coord
        if (has_texcoords)
        {
            items = sscanf(lp, "%f %f%n", &t[0], &t[1], &nc);
            assert(items == 2);
            texcoords[v][0] = t[0];
            texcoords[v][1] = t[1];
            lp += nc;
        }
    }



    // read faces: #N v[1] v[2] ... v[n-1]
    std::vector<SurfaceMesh::Vertex> vertices;
    for (i=0; i<nF; ++i)
    {
        // read line
        lp = fgets(line, 200, in);
        lp = line;

        // #vertices
        items = sscanf(lp, "%d%n", (int*)&nV, &nc);
        assert(items == 1);
        vertices.resize(nV);
        lp += nc;

        // indices
        for (j=0; j<nV; ++j)
        {
            items = sscanf(lp, "%d%n", (int*)&idx, &nc);
            assert(items == 1);
            vertices[j] = SurfaceMesh::Vertex(idx);
            lp += nc;
        }
        mesh.add_face(vertices);
    }


    return true;
}


//-----------------------------------------------------------------------------


inline bool read_off_binary(SurfaceMesh& mesh,
                     FILE* in,
                     const bool has_normals,
                     const bool has_texcoords,
                     const bool has_colors)
{
    typedef Vec3 Normal;
    typedef Vec3 TextureCoordinate;
    
    unsigned int       i, j, idx;
    unsigned int       nV, nF, nE;
    Vec3               p, n, c;
    Vec2               t;
    SurfaceMesh::Vertex  v;


    // binary cannot (yet) read colors
    if (has_colors) return false;


    // properties
    SurfaceMesh::Vertex_property<Normal>              normals;
    SurfaceMesh::Vertex_property<TextureCoordinate>  texcoords;
    if (has_normals)   normals   = mesh.vertex_property<Normal>("v:normal");
    if (has_texcoords) texcoords = mesh.vertex_property<TextureCoordinate>("v:texcoord");


    // #Vertice, #Faces, #Edges
    read(in, nV);
    read(in, nF);
    read(in, nE);
    mesh.clear();
    mesh.reserve(nV, std::max(3*nV, nE), nF);


    // read vertices: pos [normal] [color] [texcoord]
    for (i=0; i<nV && !feof(in); ++i)
    {
        // position
        read(in, p);
        v = mesh.add_vertex((Vec3)p);

        // normal
        if (has_normals)
        {
            read(in, n);
            normals[v] = n;
        }

        // tex coord
        if (has_texcoords)
        {
            read(in, t);
            texcoords[v][0] = t[0];
            texcoords[v][1] = t[1];
        }
    }


    // read faces: #N v[1] v[2] ... v[n-1]
    std::vector<SurfaceMesh::Vertex> vertices;
    for (i=0; i<nF; ++i)
    {
        read(in, nV);
        vertices.resize(nV);
        for (j=0; j<nV; ++j)
        {
            read(in, idx);
            vertices[j] = SurfaceMesh::Vertex(idx);
        }
        mesh.add_face(vertices);
    }


    return true;
}


//-----------------------------------------------------------------------------


bool read_off(SurfaceMesh& mesh, const std::string& filename)
{
    char  line[200];
    bool  has_texcoords = false;
    bool  has_normals   = false;
    bool  has_colors    = false;
    bool  has_hcoords   = false;
    bool  has_dim       = false;
    bool  is_binary     = false;


    // open file (in ASCII mode)
    FILE* in = fopen(filename.c_str(), "r");
    if (!in) return false;


    // Ignore comments (#) until I find header
    char* c = NULL;
    while(!feof(in)){
        c = fgets(line, 200, in);
        assert(c != NULL);
        // printf("%c\n",c[0]);
        if(c[0]!='#') break;
    }
    
    // read header: [ST][C][N][4][n]OFF BINARY
    if (c[0] == 'S' && c[1] == 'T') { has_texcoords = true; c += 2; }
    if (c[0] == 'C') { has_colors  = true; ++c; }
    if (c[0] == 'N') { has_normals = true; ++c; }
    if (c[0] == '4') { has_hcoords = true; ++c; }
    if (c[0] == 'n') { has_dim     = true; ++c; }
    if (strncmp(c, "OFF", 3) != 0) { fclose(in); return false; } // no OFF
    if (strncmp(c+4, "BINARY", 6) == 0) is_binary = true;


    if (has_hcoords || has_dim)
    {
        std::cerr << "homogeneous coords, and vertex dimension != 3 are not supported" << std::endl;
        fclose(in);
        return false;
    }


    // if binary: reopen file in binary mode
    if (is_binary)
    {
        fclose(in);
        in = fopen(filename.c_str(), "rb");
        c = fgets(line, 200, in);
        assert(c != NULL);
    }


    // read as ASCII or binary
    bool ok = (is_binary ?
               read_off_binary(mesh, in, has_normals, has_texcoords, has_colors) :
               read_off_ascii(mesh, in, has_normals, has_texcoords, has_colors));


    fclose(in);
    return ok;
}


//-----------------------------------------------------------------------------


bool write_off(const SurfaceMesh& mesh, const std::string& filename)
{
    typedef Vec3 Normal;
    typedef Vec3 Color;
    typedef Vec3 TextureCoordinate;  
    typedef Vec3 Point;
    
    FILE* out = fopen(filename.c_str(), "w");
    if (!out)
        return false;


    bool  has_normals   = false;
    bool  has_texcoords = false;

    SurfaceMesh::Vertex_property<Normal> normals = mesh.get_vertex_property<Normal>("v:normal");
    SurfaceMesh::Vertex_property<TextureCoordinate> texcoords = mesh.get_vertex_property<TextureCoordinate>("v:texcoord");
    SurfaceMesh::Vertex_property<Color> vcolor = mesh.get_vertex_property<Color>("v:color");
    SurfaceMesh::Face_property<Color> fcolor = mesh.get_face_property<Color>("f:color");

    if (normals)   has_normals = true;
    if (texcoords) has_texcoords = true;


    // header
    if(has_texcoords)
        fprintf(out, "ST");
    if(has_normals)
        fprintf(out, "N");
    if(vcolor)
        fprintf(out, "C");
    fprintf(out, "OFF\n%d %d 0\n", mesh.n_vertices(), mesh.n_faces());

    // vertices, and optionally normals and texture coordinates
    SurfaceMesh::Vertex_property<Vec3> points = mesh.get_vertex_property<Vec3>("v:point");
    for (SurfaceMesh::Vertex_iterator vit=mesh.vertices_begin(); vit!=mesh.vertices_end(); ++vit)
    {
        const Point& p = points[*vit];
        if( !vcolor ){
            fprintf(out, "%.10f %.10f %.10f", p[0], p[1], p[2]);
        } else {
            const Color& c = vcolor[*vit] * 255;
            int r = c[0], g = c[1], b=c[2];
            fprintf(out, "%.10f %.10f %.10f %d %d %d %d", p[0], p[1], p[2], r, g, b, 255);            
        }
            
        if (has_normals)
        {
            const Normal& n = normals[*vit];
            fprintf(out, " %.10f %.10f %.10f", n[0], n[1], n[2]);
        }

        if (has_texcoords)
        {
            const TextureCoordinate& t = texcoords[*vit];
            fprintf(out, "% .10f %.10f", t[0], t[1]);
        }

        fprintf(out, "\n");
    }


    // faces
    for (SurfaceMesh::Face_iterator fit=mesh.faces_begin(); fit!=mesh.faces_end(); ++fit)
    {
        int nV = mesh.valence(*fit);
        fprintf(out, "%d", nV);
        SurfaceMesh::Vertex_around_face_circulator fvit=mesh.vertices(*fit), fvend=fvit;
        do
        {
            fprintf(out, " %d", (*fvit).idx());
        }
        while (++fvit != fvend);

        if (fcolor)
        {
             const Color& c = fcolor[*fit] * 255;
             int r = c[0], g = c[1], b=c[2];
             fprintf(out, " %d %d %d", r, g, b);
        }

        fprintf(out, "\n");
    }

    fclose(out);
    return true;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
