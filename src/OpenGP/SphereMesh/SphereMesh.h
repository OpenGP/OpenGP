#pragma once

#include <vector>
#include <regex>
#include <iostream>
#include <fstream>

#include <OpenGP/types.h>
#include <OpenGP/SurfaceMesh/internal/Global_properties.h>
#include <OpenGP/SurfaceMesh/internal/properties.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SphereMesh : public Global_properties {
public:

    template <typename H>
    class HandleIterator;

    class BaseHandle {
        template <typename>
        friend class HandleIterator;
    private:

        int _idx;

    public:

        explicit BaseHandle(int idx=-1) : _idx(idx) {}

        int idx() const { return _idx; }

        bool is_valid() const { return _idx > -1; }

        bool operator==(const BaseHandle &rhs) const {
            return _idx == rhs._idx;
        }

        bool operator!=(const BaseHandle &rhs) const {
            return _idx != rhs._idx;
        }

        bool operator<(const BaseHandle &rhs) const {
            return _idx < rhs._idx;
        }

    };

    template <typename H>
    class HandleIterator {
    private:

        const SphereMesh *mesh;
        H handle;

    public:

        HandleIterator(H h=H(0), const SphereMesh *m=nullptr) : handle(h), mesh(m) {}

        H operator*() const { return handle; }

        bool operator==(const HandleIterator &rhs) const {
            return handle == rhs.handle;
        }

        bool operator!=(const HandleIterator &rhs) const {
            return handle != rhs.handle;
        }

        HandleIterator &operator++() {
            assert(mesh);
            ++handle._idx;
            return *this;
        }

        HandleIterator &operator--() {
            assert(mesh);
            --handle._idx;
            return *this;
        }


    };

    template <typename H>
    class HandleContainer {
    public:

        using HIterator = HandleIterator<H>;

    private:

        HIterator _begin, _end;

    public:

        HandleContainer(HIterator _begin, HIterator _end) : _begin(_begin), _end(_end) {}
        HIterator begin() const { return _begin; }
        HIterator end()   const { return _end; }

    };

    template <typename H, typename T>
    class HandleProperty : public Property<T> {
    public:

        /// default constructor
        explicit HandleProperty() {}
        explicit HandleProperty(Property<T> p) : Property<T>(p) {}

        /// access the data stored for vertex \c v
        typename Property<T>::reference operator[](H h)
        {
            return Property<T>::operator[](h.idx());
        }

        /// access the data stored for vertex \c v
        typename Property<T>::const_reference operator[](H h) const
        {
            return Property<T>::operator[](h.idx());
        }
    };

    struct Vertex : public BaseHandle {
        explicit Vertex(int idx=-1) : BaseHandle(idx) {}
    };

    struct Sphere : public BaseHandle {
        explicit Sphere(int idx=-1) : BaseHandle(idx) {}
    };

    struct Edge : public BaseHandle {
        explicit Edge(int idx=-1) : BaseHandle(idx) {}
    };

    struct Face : public BaseHandle {
        explicit Face(int idx=-1) : BaseHandle(idx) {}
    };

    using VertexIterator = HandleIterator<Vertex>;
    using SphereIterator = HandleIterator<Sphere>;
    using EdgeIterator = HandleIterator<Edge>;
    using FaceIterator = HandleIterator<Face>;

    using VertexContainer = HandleContainer<Vertex>;
    using SphereContainer = HandleContainer<Sphere>;
    using EdgeContainer = HandleContainer<Edge>;
    using FaceContainer = HandleContainer<Face>;

    template <typename T>
    using VertexProperty = HandleProperty<Vertex, T>;
    template <typename T>
    using SphereProperty = HandleProperty<Sphere, T>;
    template <typename T>
    using EdgeProperty = HandleProperty<Edge, T>;
    template <typename T>
    using FaceProperty = HandleProperty<Face, T>;

    typedef Vec4 Point;

private:

    Property_container vprops;
    Property_container sprops;
    Property_container eprops;
    Property_container fprops;

    typedef int SphereConnectivity;
    typedef Eigen::Matrix<int, 2, 1> EdgeConnectivity;
    typedef Eigen::Matrix<int, 3, 1> FaceConnectivity;

    VertexProperty<Point> vpoint;

    SphereProperty<SphereConnectivity> sconn;
    EdgeProperty<EdgeConnectivity> econn;
    FaceProperty<FaceConnectivity> fconn;

    VertexProperty<bool> vdeleted;
    SphereProperty<bool> sdeleted;
    EdgeProperty<bool>   edeleted;
    FaceProperty<bool>   fdeleted;

    unsigned int deleted_vertices;
    unsigned int deleted_spheres;
    unsigned int deleted_edges;
    unsigned int deleted_faces;

    bool has_garbage;

public:

    SphereMesh() {
        sconn  = add_sphere_property<SphereConnectivity>("s:connectivity");
        econn  = add_edge_property<EdgeConnectivity>("e:connectivity");
        fconn  = add_face_property<FaceConnectivity>("f:connectivity");
        vpoint = add_vertex_property<Point>("v:point");
    }

    virtual ~SphereMesh() {}

    void clear() {

        vprops.resize(0);
        sprops.resize(0);
        eprops.resize(0);
        fprops.resize(0);

        vprops.free_memory();
        sprops.free_memory();
        eprops.free_memory();
        fprops.free_memory();

        deleted_vertices = deleted_spheres = deleted_edges = deleted_faces = 0;
        has_garbage = false;

    }

    void free_memory() {
        vprops.free_memory();
        sprops.free_memory();
        eprops.free_memory();
        fprops.free_memory();
    }

    void reserve(int nv, int ns, int ne, int nf) {
        vprops.reserve(nv);
        sprops.reserve(ns);
        eprops.reserve(ne);
        fprops.reserve(nf);
    }

    void add_vertex(Point vertex) {
        vprops.push_back();
        vpoint[*(--(vertices_end()))] = vertex;
    }

    void add_sphere(Vertex vertex) {
        sprops.push_back();
        sconn[*(--(spheres_end()))] = vertex.idx();
    }

    void add_edge(Vertex v0, Vertex v1) {
        eprops.push_back();
        econn[*(--(edges_end()))] = EdgeConnectivity(v0.idx(), v1.idx());
    }

    void add_face(Vertex v0, Vertex v1, Vertex v2) {
        fprops.push_back();
        fconn[*(--(faces_end()))] = FaceConnectivity(v0.idx(), v1.idx(), v2.idx());
    }

    void delete_vertex(Vertex v) {

        if (vdeleted[v]) return;

        std::vector<Sphere> spheres_to_delete;
        std::vector<Edge> edges_to_delete;
        std::vector<Face> faces_to_delete;

        for (auto s : spheres()) {
            if (Vertex(sconn[s]) == v) {
                spheres_to_delete.push_back(s);
            }
        }

        for (auto e : edges()) {
            if (Vertex(econn[e](0)) == v || Vertex(econn[e](1)) == v) {
                edges_to_delete.push_back(e);
            }
        }

        for (auto f : faces()) {
            if (Vertex(fconn[f](0)) == v || Vertex(fconn[f](1)) == v || Vertex(fconn[f](2)) == v) {
                faces_to_delete.push_back(f);
            }
        }

        for (auto s : spheres_to_delete) {
            delete_sphere(s);
        }

        for (auto e : edges_to_delete) {
            delete_edge(e);
        }

        for (auto f : faces_to_delete) {
            delete_face(f);
        }

        vdeleted[v] = true;
        deleted_vertices++;
        has_garbage = true;

    }

    void delete_sphere(Sphere s) {

        if (sdeleted[s]) return;

        sdeleted[s] = true;
        deleted_spheres++;
        has_garbage = true;

    }

    void delete_edge(Edge e) {

        if (edeleted[e]) return;

        edeleted[e] = true;
        deleted_edges++;
        has_garbage = true;

    }

    void delete_face(Face f) {

        if (fdeleted[f]) return;

        fdeleted[f] = true;
        deleted_faces++;
        has_garbage = true;

    }

    Vertex vertex(Sphere s) const { return Vertex(sconn[s]); }
    Vertex vertex(Edge e, int i) const { assert(i < 2 && i > -1); return Vertex(econn[e](i)); }
    Vertex vertex(Face f, int i) const { assert(i < 3 && i > -1); return Vertex(fconn[f](i)); }

    VertexIterator vertices_begin() const { return VertexIterator(Vertex(0), this); }
    SphereIterator spheres_begin() const { return SphereIterator(Sphere(0), this); }
    EdgeIterator edges_begin() const { return EdgeIterator(Edge(0), this); }
    FaceIterator faces_begin() const { return FaceIterator(Face(0), this); }

    VertexIterator vertices_end() const { return VertexIterator(Vertex(vprops.size()), this); }
    SphereIterator spheres_end() const { return SphereIterator(Sphere(sprops.size()), this); }
    EdgeIterator edges_end() const { return EdgeIterator(Edge(eprops.size()), this); }
    FaceIterator faces_end() const { return FaceIterator(Face(fprops.size()), this); }

    VertexContainer vertices() const { return VertexContainer(vertices_begin(), vertices_end()); }
    SphereContainer spheres() const { return SphereContainer(spheres_begin(), spheres_end()); }
    EdgeContainer edges() const { return EdgeContainer(edges_begin(), edges_end()); }
    FaceContainer faces() const { return FaceContainer(faces_begin(), faces_end()); }

    unsigned int vertices_size() const { return (unsigned int) vprops.size(); }
    unsigned int spheres_size() const { return (unsigned int) sprops.size(); }
    unsigned int edges_size() const { return (unsigned int) eprops.size(); }
    unsigned int faces_size() const { return (unsigned int) fprops.size(); }

    unsigned int n_vertices() const { return vertices_size() - deleted_vertices; }
    unsigned int n_spheres() const { return spheres_size() - deleted_spheres; }
    unsigned int n_edges() const { return edges_size() - deleted_edges; }
    unsigned int n_faces() const { return faces_size() - deleted_faces; }

    bool is_valid(Vertex v) const { return v.idx() != 0 && v.idx() < vertices_size(); }
    bool is_valid(Sphere s) const { return s.idx() != 0 && s.idx() < spheres_size(); }
    bool is_valid(Edge e) const { return e.idx() != 0 && e.idx() < edges_size(); }
    bool is_valid(Face f) const { return f.idx() != 0 && f.idx() < faces_size(); }

    template <class T>
    VertexProperty<T> add_vertex_property(const std::string& name, const T t=T()) {
        return VertexProperty<T>(vprops.add<T>(name, t));
    }

    template <class T>
    VertexProperty<T> get_vertex_property(const std::string& name) const {
        return VertexProperty<T>(vprops.get<T>(name));
    }

    template <class T>
    VertexProperty<T> vertex_property(const std::string& name, const T t=T()) {
        return VertexProperty<T>(vprops.get_or_add<T>(name, t));
    }

    template <class T>
    void remove_vertex_property(VertexProperty<T>& p) {
        vprops.remove(p);
    }

    const std::type_info& get_vertex_property_type(const std::string& name) {
        return vprops.get_type(name);
    }


    template <class T>
    SphereProperty<T> add_sphere_property(const std::string& name, const T t=T()) {
        return SphereProperty<T>(sprops.add<T>(name, t));
    }

    template <class T>
    SphereProperty<T> get_sphere_property(const std::string& name) const {
        return SphereProperty<T>(sprops.get<T>(name));
    }

    template <class T>
    SphereProperty<T> sphere_property(const std::string& name, const T t=T()) {
        return SphereProperty<T>(sprops.get_or_add<T>(name, t));
    }

    template <class T>
    void remove_sphere_property(SphereProperty<T>& p) {
        sprops.remove(p);
    }

    const std::type_info& get_sphere_property_type(const std::string& name) {
        return sprops.get_type(name);
    }


    template <class T>
    EdgeProperty<T> add_edge_property(const std::string& name, const T t=T()) {
        return EdgeProperty<T>(eprops.add<T>(name, t));
    }

    template <class T>
    EdgeProperty<T> get_edge_property(const std::string& name) const {
        return EdgeProperty<T>(eprops.get<T>(name));
    }

    template <class T>
    EdgeProperty<T> edge_property(const std::string& name, const T t=T()) {
        return EdgeProperty<T>(eprops.get_or_add<T>(name, t));
    }

    template <class T>
    void remove_edge_property(EdgeProperty<T>& p) {
        eprops.remove(p);
    }

    const std::type_info& get_edge_property_type(const std::string& name) {
        return eprops.get_type(name);
    }


    template <class T>
    FaceProperty<T> add_face_property(const std::string& name, const T t=T()) {
        return FaceProperty<T>(fprops.add<T>(name, t));
    }

    template <class T>
    FaceProperty<T> get_face_property(const std::string& name) const {
        return FaceProperty<T>(fprops.get<T>(name));
    }

    template <class T>
    FaceProperty<T> face_property(const std::string& name, const T t=T()) {
        return FaceProperty<T>(fprops.get_or_add<T>(name, t));
    }

    template <class T>
    void remove_face_property(FaceProperty<T>& p) {
        fprops.remove(p);
    }

    const std::type_info& get_face_property_type(const std::string& name) {
        return fprops.get_type(name);
    }

    bool read(const std::string& filename) {

        std::ifstream file_stream(filename, std::ios_base::in);

        if (!file_stream.is_open()) {
            return false;
        }

        std::string text((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());

        return read_text(text);

    }

    bool read_text(const std::string& text) {

        std::regex vert_regex(R"regex(v\s+([\d\-\.]+)\s+([\d\-\.]+)\s+([\d\-\.]+)\s+([\d\-\.]+))regex");

        std::sregex_iterator verts_begin(text.begin(), text.end(), vert_regex);
        std::sregex_iterator verts_end;

        for (auto i = verts_begin;i != verts_end;++i) {

            auto match = *i;

            vprops.push_back();
            auto it = vertices_end();
            vpoint[*(--it)] = Point(std::stof(match[1]), std::stof(match[2]), std::stof(match[3]), std::stof(match[4]));

        }

        std::regex sphere_regex(R"regex(s\s+([\d]+))regex");
        std::sregex_iterator spheres_begin(text.begin(), text.end(), sphere_regex);
        std::sregex_iterator spheres_end;

        for (auto i = spheres_begin;i != spheres_end;++i) {

            auto match = *i;

            sprops.push_back();
            auto it = this->spheres_end();
            sconn[*(--it)] = std::stoi(match[1]);

        }

        std::regex edge_regex(R"regex(p\s+([\d]+)\s+([\d]+))regex");
        std::sregex_iterator edges_begin(text.begin(), text.end(), edge_regex);
        std::sregex_iterator edges_end;

        for (auto i = edges_begin;i != edges_end;++i) {

            auto match = *i;

            eprops.push_back();
            auto it = this->edges_end();
            econn[*(--it)] = EdgeConnectivity(std::stoi(match[1]), std::stoi(match[2]));

        }

        std::regex face_regex(R"regex(w\s+([\d]+)\s+([\d]+)\s+([\d]+))regex");
        std::sregex_iterator faces_begin(text.begin(), text.end(), face_regex);
        std::sregex_iterator faces_end;

        for (auto i = faces_begin;i != faces_end;++i) {

            auto match = *i;

            fprops.push_back();
            auto it = this->faces_end();
            fconn[*(--it)] = FaceConnectivity(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]));

        }

        return true;

    }

    bool write(const std::string& filename) const {

        std::ofstream file_stream(filename);

        if (!file_stream.is_open()) {
            return false;
        }

        auto text = write_text();

        file_stream << text;

        return true;

    }

    std::string write_text() const {

        std::string text;

        for (auto v : vertices()) {
            auto vert = vpoint[v];
            text += "v ";
            text += std::to_string(vert(0)) + " ";
            text += std::to_string(vert(1)) + " ";
            text += std::to_string(vert(2)) + " ";
            text += std::to_string(vert(3)) + "\n";
        }

        for (auto s : spheres()) {
            auto sphere = sconn[s];
            text += "s ";
            text += std::to_string(sphere) + "\n";
        }

        for (auto e : edges()) {
            auto edge = econn[e];
            text += "p ";
            text += std::to_string(edge(0)) + " ";
            text += std::to_string(edge(1)) + "\n";
        }

        for (auto f : faces()) {
            auto face = fconn[f];
            text += "w ";
            text += std::to_string(face(0)) + " ";
            text += std::to_string(face(1)) + " ";
            text += std::to_string(face(2)) + "\n";
        }

        return text;

    }

    bool garbage() const { return has_garbage; }

    void garbage_collection() {

        int  i, i0, i1,
        nV(vertices_size()),
        nS(spheres_size()),
        nE(edges_size()),
        nF(faces_size());

        VertexProperty<Vertex> vmap = add_vertex_property<Vertex>("v:garbage-collection");

        for (i = 0;i < nV;i++) {
            vmap[Vertex(i)] = Vertex(i);
        }

        // remove deleted vertices
        if (nV > 0) {
            i0=0;  i1=nV-1;

            while (1) {
                // find first deleted and last un-deleted
                while (!vdeleted[Vertex(i0)] && i0 < i1)  ++i0;
                while ( vdeleted[Vertex(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;

                // swap
                vprops.swap(i0, i1);
            };

            // remember new size
            nV = vdeleted[Vertex(i0)] ? i0 : i0+1;
        }

        VertexProperty<Vertex> vmap_reverse = add_vertex_property<Vertex>("v:garbage-collection-2");

        for (i = 0;i < nV;i++) {

            Vertex vnew(i);
            Vertex vold = vmap[vnew];

            vmap_reverse[vold] = vnew;

        }

        // remove deleted spheres
        if (nS > 0) {
            i0=0;  i1=nS-1;

            while (1) {
                // find first deleted and last un-deleted
                while (!sdeleted[Sphere(i0)] && i0 < i1)  ++i0;
                while ( sdeleted[Sphere(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;

                // swap
                sprops.swap(i0, i1);
            };

            // remember new size
            nS = sdeleted[Sphere(i0)] ? i0 : i0+1;
        }

        // remove deleted edges
        if (nE > 0) {
            i0=0;  i1=nE-1;

            while (1) {
                // find first deleted and last un-deleted
                while (!edeleted[Edge(i0)] && i0 < i1)  ++i0;
                while ( edeleted[Edge(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;

                // swap
                eprops.swap(i0, i1);
            };

            // remember new size
            nE = edeleted[Edge(i0)] ? i0 : i0+1;
        }

        // remove deleted faces
        if (nF > 0) {
            i0=0;  i1=nF-1;

            while (1) {
                // find first deleted and last un-deleted
                while (!fdeleted[Face(i0)] && i0 < i1)  ++i0;
                while ( fdeleted[Face(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;

                // swap
                fprops.swap(i0, i1);
            };

            // remember new size
            nF = fdeleted[Face(i0)] ? i0 : i0+1;
        }

        for (i = 0;i < nV;i++) {

            Sphere s(i);

            sconn[s] = vmap_reverse[Vertex(sconn[s])].idx();

        }

        for (i = 0;i < nE;i++) {

            Edge e(i);

            econn[e](0) = vmap_reverse[Vertex(econn[e](0))].idx();
            econn[e](1) = vmap_reverse[Vertex(econn[e](1))].idx();

        }

        for (i = 0;i < nF;i++) {

            Face f(i);

            fconn[f](0) = vmap_reverse[Vertex(fconn[f](0))].idx();
            fconn[f](1) = vmap_reverse[Vertex(fconn[f](1))].idx();
            fconn[f](2) = vmap_reverse[Vertex(fconn[f](2))].idx();

        }

        remove_vertex_property(vmap);
        remove_vertex_property(vmap_reverse);

        vprops.resize(nV); vprops.free_memory();
        sprops.resize(nS); sprops.free_memory();
        eprops.resize(nE); eprops.free_memory();
        fprops.resize(nF); fprops.free_memory();

        deleted_vertices = deleted_spheres = deleted_edges = deleted_faces = 0;
        has_garbage = false;

    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
