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


#ifndef SURFACE_MESH_H
#define SURFACE_MESH_H


//== INCLUDES =================================================================


#include <surface_mesh/types.h>
#include <surface_mesh/properties.h>
#include <surface_mesh/IO.h>

//== NAMESPACE ================================================================


namespace surface_mesh {


//== CLASS DEFINITION =========================================================


/// A halfedge data structure for polygonal meshes.
class Surface_mesh
{

public: //------------------------------------------------------ topology types


    /// Base class for all topology types (internally it is basically an index)
    /// \sa Vertex, Halfedge, Edge, Face
    class Base_handle
    {
    public:

        /// constructor
        explicit Base_handle(int _idx=-1) : idx_(_idx) {}

        /// Get the underlying index of this handle
        int idx() const { return idx_; }

        /// reset handle to be invalid (index=-1)
        void reset() { idx_=-1; }

        /// return whether the handle is valid, i.e., the index is not equal to -1.
        bool is_valid() const { return idx_ != -1; }

        /// are two handles equal?
        bool operator==(const Base_handle& _rhs) const {
            return idx_ == _rhs.idx_;
        }

        /// are two handles different?
        bool operator!=(const Base_handle& _rhs) const {
            return idx_ != _rhs.idx_;
        }

        /// compare operator useful for sorting handles
        bool operator<(const Base_handle& _rhs) const {
            return idx_ < _rhs.idx_;
        }

    private:
        friend class Vertex_iterator;
        friend class Halfedge_iterator;
        friend class Edge_iterator;
        friend class Face_iterator;
        friend class Surface_mesh;
        int idx_;
    };


    /// this type represents a vertex (internally it is basically an index)
    ///  \sa Halfedge, Edge, Face
    struct Vertex : public Base_handle
    {
        /// default constructor (with invalid index)
        explicit Vertex(int _idx=-1) : Base_handle(_idx) {}
        std::ostream& operator<<(std::ostream& os) const { return os << 'v' << idx(); }
    };


    /// this type represents a halfedge (internally it is basically an index)
    /// \sa Vertex, Edge, Face
    struct Halfedge : public Base_handle
    {
        /// default constructor (with invalid index)
        explicit Halfedge(int _idx=-1) : Base_handle(_idx) {}
    };


    /// this type represents an edge (internally it is basically an index)
    /// \sa Vertex, Halfedge, Face
    struct Edge : public Base_handle
    {
        /// default constructor (with invalid index)
        explicit Edge(int _idx=-1) : Base_handle(_idx) {}
    };


    /// this type represents a face (internally it is basically an index)
    /// \sa Vertex, Halfedge, Edge
    struct Face : public Base_handle
    {
        /// default constructor (with invalid index)
        explicit Face(int _idx=-1) : Base_handle(_idx) {}
    };




public: //-------------------------------------------------- connectivity types

    /// This type stores the vertex connectivity
    /// \sa Halfedge_connectivity, Face_connectivity
    struct Vertex_connectivity
    {
        /// an outgoing halfedge per vertex (it will be a boundary halfedge for boundary vertices)
        Halfedge  halfedge_;
    };


    /// This type stores the halfedge connectivity
    /// \sa Vertex_connectivity, Face_connectivity
    struct Halfedge_connectivity
    {
        /// face incident to halfedge
        Face      face_;
        /// vertex the halfedge points to
        Vertex    vertex_;
        /// next halfedge within a face (or along a boundary)
        Halfedge  next_halfedge_;
        /// previous halfedge within a face (or along a boundary)
        Halfedge  prev_halfedge_;
    };


    /// This type stores the face connectivity
    /// \sa Vertex_connectivity, Halfedge_connectivity
    struct Face_connectivity
    {
        /// a halfedge that is part of the face
        Halfedge  halfedge_;
    };




public: //------------------------------------------------------ property types

    /// Vertex property of type T
    /// \sa Halfedge_property, Edge_property, Face_property
    template <class T> class Vertex_property : public Property<T>
    {
    public:

        /// default constructor
        explicit Vertex_property() {}
        explicit Vertex_property(Property<T> p) : Property<T>(p) {}

        /// access the data stored for vertex \c v
        typename Property<T>::reference operator[](Vertex v)
        {
            return Property<T>::operator[](v.idx());
        }

        /// access the data stored for vertex \c v
        typename Property<T>::const_reference operator[](Vertex v) const
        {
            return Property<T>::operator[](v.idx());
        }
    };


    /// Halfedge property of type T
    /// \sa Vertex_property, Edge_property, Face_property
    template <class T> class Halfedge_property : public Property<T>
    {
    public:

        /// default constructor
        explicit Halfedge_property() {}
        explicit Halfedge_property(Property<T> p) : Property<T>(p) {}

        /// access the data stored for halfedge \c h
        typename Property<T>::reference operator[](Halfedge h)
        {
            return Property<T>::operator[](h.idx());
        }

        /// access the data stored for halfedge \c h
        typename Property<T>::const_reference operator[](Halfedge h) const
        {
            return Property<T>::operator[](h.idx());
        }
    };


    /// Edge property of type T
    /// \sa Vertex_property, Halfedge_property, Face_property
    template <class T> class Edge_property : public Property<T>
    {
    public:

        /// default constructor
        explicit Edge_property() {}
        explicit Edge_property(Property<T> p) : Property<T>(p) {}

        /// access the data stored for edge \c e
        typename Property<T>::reference operator[](Edge e)
        {
            return Property<T>::operator[](e.idx());
        }

        /// access the data stored for edge \c e
        typename Property<T>::const_reference operator[](Edge e) const
        {
            return Property<T>::operator[](e.idx());
        }
    };


    /// Face property of type T
    /// \sa Vertex_property, Halfedge_property, Edge_property
    template <class T> class Face_property : public Property<T>
    {
    public:

        /// default constructor
        explicit Face_property() {}
        explicit Face_property(Property<T> p) : Property<T>(p) {}

        /// access the data stored for face \c f
        typename Property<T>::reference operator[](Face f)
        {
            return Property<T>::operator[](f.idx());
        }

        /// access the data stored for face \c f
        typename Property<T>::const_reference operator[](Face f) const
        {
            return Property<T>::operator[](f.idx());
        }
    };




public: //------------------------------------------------------ iterator types

    /// this class iterates linearly over all vertices
    /// \sa vertices_begin(), vertices_end()
    /// \sa Halfedge_iterator, Edge_iterator, Face_iterator
    class Vertex_iterator
    {
    public:

        /// Default constructor
        Vertex_iterator(Vertex v=Vertex(), const Surface_mesh* m=NULL) : hnd_(v), mesh_(m)
        {
            if (mesh_ && mesh_->garbage()) while (mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
        }

        /// get the vertex the iterator refers to
        Vertex operator*()  const { return  hnd_; }

        /// are two iterators equal?
        bool operator==(const Vertex_iterator& rhs) const
        {
            return (hnd_==rhs.hnd_);
        }

        /// are two iterators different?
        bool operator!=(const Vertex_iterator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment iterator
        Vertex_iterator& operator++()
        {
            ++hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
            return *this;
        }

        /// pre-decrement iterator
        Vertex_iterator& operator--()
        {
            --hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) --hnd_.idx_;
            return *this;
        }

    private:
        Vertex  hnd_;
        const Surface_mesh* mesh_;
    };


    /// this class iterates linearly over all halfedges
    /// \sa halfedges_begin(), halfedges_end()
    /// \sa Vertex_iterator, Edge_iterator, Face_iterator
    class Halfedge_iterator
    {
    public:

        /// Default constructor
        Halfedge_iterator(Halfedge h=Halfedge(), const Surface_mesh* m=NULL) : hnd_(h), mesh_(m)
        {
            if (mesh_ && mesh_->garbage()) while (mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
        }

        /// get the halfedge the iterator refers to
        Halfedge operator*()  const { return  hnd_; }

        /// are two iterators equal?
        bool operator==(const Halfedge_iterator& rhs) const
        {
            return (hnd_==rhs.hnd_);
        }

        /// are two iterators different?
        bool operator!=(const Halfedge_iterator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment iterator
        Halfedge_iterator& operator++()
        {
            ++hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
            return *this;
        }

        /// pre-decrement iterator
        Halfedge_iterator& operator--()
        {
            --hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) --hnd_.idx_;
            return *this;
        }

    private:
        Halfedge  hnd_;
        const Surface_mesh* mesh_;
    };


    /// this class iterates linearly over all edges
    /// \sa edges_begin(), edges_end()
    /// \sa Vertex_iterator, Halfedge_iterator, Face_iterator
    class Edge_iterator
    {
    public:

        /// Default constructor
        Edge_iterator(Edge e=Edge(), const Surface_mesh* m=NULL) : hnd_(e), mesh_(m)
        {
            if (mesh_ && mesh_->garbage()) while (mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
        }

        /// get the edge the iterator refers to
        Edge operator*()  const { return  hnd_; }

        /// are two iterators equal?
        bool operator==(const Edge_iterator& rhs) const
        {
            return (hnd_==rhs.hnd_);
        }

        /// are two iterators different?
        bool operator!=(const Edge_iterator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment iterator
        Edge_iterator& operator++()
        {
            ++hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
            return *this;
        }

        /// pre-decrement iterator
        Edge_iterator& operator--()
        {
            --hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) --hnd_.idx_;
            return *this;
        }

    private:
        Edge  hnd_;
        const Surface_mesh* mesh_;
    };


    /// this class iterates linearly over all faces
    /// \sa faces_begin(), faces_end()
    /// \sa Vertex_iterator, Halfedge_iterator, Edge_iterator
    class Face_iterator
    {
    public:

        /// Default constructor
        Face_iterator(Face f=Face(), const Surface_mesh* m=NULL) : hnd_(f), mesh_(m)
        {
            if (mesh_ && mesh_->garbage()) while (mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
        }

        /// get the face the iterator refers to
        Face operator*()  const { return  hnd_; }

        /// are two iterators equal?
        bool operator==(const Face_iterator& rhs) const
        {
            return (hnd_==rhs.hnd_);
        }

        /// are two iterators different?
        bool operator!=(const Face_iterator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment iterator
        Face_iterator& operator++()
        {
            ++hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) ++hnd_.idx_;
            return *this;
        }

        /// pre-decrement iterator
        Face_iterator& operator--()
        {
            --hnd_.idx_;
            assert(mesh_);
            while (mesh_->garbage() && mesh_->is_valid(hnd_) && mesh_->is_deleted(hnd_)) --hnd_.idx_;
            return *this;
        }

    private:
        Face  hnd_;
        const Surface_mesh* mesh_;
    };



public: //-------------------------- containers for C++11 range-based for loops

    /// this helper class is a container for iterating through all
    /// vertices using C++11 range-based for-loops.
    /// \sa vertices()
    class Vertex_container
    {
    public:
        Vertex_container(Vertex_iterator _begin, Vertex_iterator _end) : begin_(_begin), end_(_end) {}
        Vertex_iterator begin() const { return begin_; }
        Vertex_iterator end()   const { return end_;   }
    private:
        Vertex_iterator begin_, end_;
    };



    /// this helper class is a container for iterating through all
    /// halfedge using C++11 range-based for-loops.
    /// \sa halfedges()
    class Halfedge_container
    {
    public:
        Halfedge_container(Halfedge_iterator _begin, Halfedge_iterator _end) : begin_(_begin), end_(_end) {}
        Halfedge_iterator begin() const { return begin_; }
        Halfedge_iterator end()   const { return end_;   }
    private:
        Halfedge_iterator begin_, end_;
    };



    /// this helper class is a container for iterating through all
    /// edges using C++11 range-based for-loops.
    /// \sa edges()
    class Edge_container
    {
    public:
        Edge_container(Edge_iterator _begin, Edge_iterator _end) : begin_(_begin), end_(_end) {}
        Edge_iterator begin() const { return begin_; }
        Edge_iterator end()   const { return end_;   }
    private:
        Edge_iterator begin_, end_;
    };



    /// this helper class is a container for iterating through all
    /// faces using C++11 range-based for-loops.
    /// \sa faces()
    class Face_container
    {
    public:
        Face_container(Face_iterator _begin, Face_iterator _end) : begin_(_begin), end_(_end) {}
        Face_iterator begin() const { return begin_; }
        Face_iterator end()   const { return end_;   }
    private:
        Face_iterator begin_, end_;
    };





public: //---------------------------------------------------- circulator types

    /// this class circulates through all one-ring neighbors of a vertex.
    /// it also acts as a container-concept for C++11 range-based for loops.
    /// \sa Halfedge_around_vertex_circulator, Face_around_vertex_circulator, vertices(Vertex)
    class Vertex_around_vertex_circulator
    {
    public:

        /// default constructor
        Vertex_around_vertex_circulator(const Surface_mesh* m=NULL, Vertex v=Vertex())
        : mesh_(m), active_(true)
        {
            if (mesh_) halfedge_ = mesh_->halfedge(v);
        }

        /// are two circulators equal?
        bool operator==(const Vertex_around_vertex_circulator& rhs) const
        {
            assert(mesh_);
            return (active_ && (mesh_==rhs.mesh_) && (halfedge_==rhs.halfedge_));
        }

        /// are two circulators different?
        bool operator!=(const Vertex_around_vertex_circulator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment (rotate couter-clockwise)
        Vertex_around_vertex_circulator& operator++()
        {
            assert(mesh_);
            halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
            active_ = true;
            return *this;
        }

        /// pre-decrement (rotate clockwise)
        Vertex_around_vertex_circulator& operator--()
        {
            assert(mesh_);
            halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
            return *this;
        }

        /// get the vertex the circulator refers to
        Vertex operator*()  const
        {
            assert(mesh_);
            return mesh_->to_vertex(halfedge_);
        }

        /// cast to bool: true if vertex is not isolated
        operator bool() const { return halfedge_.is_valid(); }

        /// return current halfedge
        Halfedge halfedge() const { return halfedge_; }

        // helper for C++11 range-based for-loops
        Vertex_around_vertex_circulator& begin() { active_=!halfedge_.is_valid(); return *this; }
        // helper for C++11 range-based for-loops
        Vertex_around_vertex_circulator& end()   { active_=true;  return *this; }

    private:
        const Surface_mesh*  mesh_;
        Halfedge         halfedge_;
        // helper for C++11 range-based for-loops
        bool active_;
    };


    /// this class circulates through all outgoing halfedges of a vertex.
    /// it also acts as a container-concept for C++11 range-based for loops.
    /// \sa Vertex_around_vertex_circulator, Face_around_vertex_circulator, halfedges(Vertex)
    class Halfedge_around_vertex_circulator
    {
    public:

        /// default constructor
        Halfedge_around_vertex_circulator(const Surface_mesh* m=NULL, Vertex v=Vertex())
        : mesh_(m), active_(true)
        {
            if (mesh_) halfedge_ = mesh_->halfedge(v);
        }

        /// are two circulators equal?
        bool operator==(const Halfedge_around_vertex_circulator& rhs) const
        {
            assert(mesh_);
            return (active_ && (mesh_==rhs.mesh_) && (halfedge_==rhs.halfedge_));
        }

        /// are two circulators different?
        bool operator!=(const Halfedge_around_vertex_circulator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment (rotate couter-clockwise)
        Halfedge_around_vertex_circulator& operator++()
        {
            assert(mesh_);
            halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
            active_ = true;
            return *this;
        }

        /// pre-decrement (rotate clockwise)
        Halfedge_around_vertex_circulator& operator--()
        {
            assert(mesh_);
            halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
            return *this;
        }

        /// get the halfedge the circulator refers to
        Halfedge operator*() const { return halfedge_; }

        /// cast to bool: true if vertex is not isolated
        operator bool() const { return halfedge_.is_valid(); }

        // helper for C++11 range-based for-loops
        Halfedge_around_vertex_circulator& begin() { active_=!halfedge_.is_valid(); return *this; }
        // helper for C++11 range-based for-loops
        Halfedge_around_vertex_circulator& end()   { active_=true;  return *this; }

    private:
        const Surface_mesh*  mesh_;
        Halfedge         halfedge_;
        // helper for C++11 range-based for-loops
        bool active_;
    };


    /// this class circulates through all incident faces of a vertex.
    /// it also acts as a container-concept for C++11 range-based for loops.
    /// \sa Vertex_around_vertex_circulator, Halfedge_around_vertex_circulator, faces(Vertex)
    class Face_around_vertex_circulator
    {
    public:

        /// construct with mesh and vertex (vertex should not be isolated!)
        Face_around_vertex_circulator(const Surface_mesh* m=NULL, Vertex v=Vertex())
        : mesh_(m), active_(true)
        {
            if (mesh_)
            {
                halfedge_ = mesh_->halfedge(v);
                if (halfedge_.is_valid() && mesh_->is_boundary(halfedge_))
                    operator++();
            }
        }

        /// are two circulators equal?
        bool operator==(const Face_around_vertex_circulator& rhs) const
        {
            assert(mesh_);
            return (active_ && (mesh_==rhs.mesh_) && (halfedge_==rhs.halfedge_));
        }

        /// are two circulators different?
        bool operator!=(const Face_around_vertex_circulator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment (rotates counter-clockwise)
        Face_around_vertex_circulator& operator++()
        {
            assert(mesh_ && halfedge_.is_valid());
            do {
                halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
            } while (mesh_->is_boundary(halfedge_));
            active_ = true;
            return *this;
        }

        /// pre-decrement (rotate clockwise)
        Face_around_vertex_circulator& operator--()
        {
            assert(mesh_ && halfedge_.is_valid());
            do
                halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
            while (mesh_->is_boundary(halfedge_));
            return *this;
        }

        /// get the face the circulator refers to
        Face operator*() const
        {
            assert(mesh_ && halfedge_.is_valid());
            return mesh_->face(halfedge_);
        }

        /// cast to bool: true if vertex is not isolated
        operator bool() const { return halfedge_.is_valid(); }

        // helper for C++11 range-based for-loops
        Face_around_vertex_circulator& begin() { active_=!halfedge_.is_valid(); return *this; }
        // helper for C++11 range-based for-loops
        Face_around_vertex_circulator& end()   { active_=true;  return *this; }

    private:
        const Surface_mesh*  mesh_;
        Halfedge         halfedge_;
        // helper for C++11 range-based for-loops
        bool active_;
    };


    /// this class circulates through the vertices of a face.
    /// it also acts as a container-concept for C++11 range-based for loops.
    /// \sa Halfedge_around_face_circulator, vertices(Face)
    class Vertex_around_face_circulator
    {
    public:

        /// default constructor
        Vertex_around_face_circulator(const Surface_mesh* m=NULL, Face f=Face())
        : mesh_(m), active_(true)
        {
            if (mesh_) halfedge_ = mesh_->halfedge(f);
        }

        /// are two circulators equal?
        bool operator==(const Vertex_around_face_circulator& rhs) const
        {
            assert(mesh_);
            return (active_ && (mesh_==rhs.mesh_) && (halfedge_==rhs.halfedge_));
        }

        /// are two circulators different?
        bool operator!=(const Vertex_around_face_circulator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment (rotates counter-clockwise)
        Vertex_around_face_circulator& operator++()
        {
            assert(mesh_ && halfedge_.is_valid());
            halfedge_ = mesh_->next_halfedge(halfedge_);
            active_ = true;
            return *this;
        }

        /// pre-decrement (rotates clockwise)
        Vertex_around_face_circulator& operator--()
        {
            assert(mesh_ && halfedge_.is_valid());
            halfedge_ = mesh_->prev_halfedge(halfedge_);
            return *this;
        }

        /// get the vertex the circulator refers to
        Vertex operator*() const
        {
            assert(mesh_ && halfedge_.is_valid());
            return mesh_->to_vertex(halfedge_);
        }

        // helper for C++11 range-based for-loops
        Vertex_around_face_circulator& begin() { active_=false; return *this; }
        // helper for C++11 range-based for-loops
        Vertex_around_face_circulator& end()   { active_=true;  return *this; }

    private:
        const Surface_mesh*  mesh_;
        Halfedge         halfedge_;
        // helper for C++11 range-based for-loops
        bool active_;
    };


    /// this class circulates through all halfedges of a face.
    /// it also acts as a container-concept for C++11 range-based for loops.
    /// \sa Vertex_around_face_circulator, halfedges(Face)
    class Halfedge_around_face_circulator
    {
    public:

        /// default constructur
        Halfedge_around_face_circulator(const Surface_mesh* m=NULL, Face f=Face())
        : mesh_(m), active_(true)
        {
            if (mesh_) halfedge_ = mesh_->halfedge(f);
        }

        /// are two circulators equal?
        bool operator==(const Halfedge_around_face_circulator& rhs) const
        {
            assert(mesh_);
            return (active_ && (mesh_==rhs.mesh_) && (halfedge_==rhs.halfedge_));
        }

        /// are two circulators different?
        bool operator!=(const Halfedge_around_face_circulator& rhs) const
        {
            return !operator==(rhs);
        }

        /// pre-increment (rotates counter-clockwise)
        Halfedge_around_face_circulator& operator++()
        {
            assert(mesh_ && halfedge_.is_valid());
            halfedge_ = mesh_->next_halfedge(halfedge_);
            active_ = true;
            return *this;
        }

        /// pre-decrement (rotates clockwise)
        Halfedge_around_face_circulator& operator--()
        {
            assert(mesh_ && halfedge_.is_valid());
            halfedge_ = mesh_->prev_halfedge(halfedge_);
            return *this;
        }

        /// get the halfedge the circulator refers to
        Halfedge operator*() const { return halfedge_; }

        // helper for C++11 range-based for-loops
        Halfedge_around_face_circulator& begin() { active_=false; return *this; }
        // helper for C++11 range-based for-loops
        Halfedge_around_face_circulator& end()   { active_=true;  return *this; }

    private:
        const Surface_mesh*  mesh_;
        Halfedge         halfedge_;
        // helper for C++11 range-based for-loops
        bool active_;
    };



public: //-------------------------------------------- constructor / destructor

    /// \name Construct, destruct, assignment
    //@{

    /// default constructor
    Surface_mesh()
    {
        // allocate standard properties
        // same list is used in operator=() and assign()
        vconn_    = add_vertex_property<Vertex_connectivity>("v:connectivity");
        hconn_    = add_halfedge_property<Halfedge_connectivity>("h:connectivity");
        fconn_    = add_face_property<Face_connectivity>("f:connectivity");
        vpoint_   = add_vertex_property<Point>("v:point");
        vdeleted_ = add_vertex_property<bool>("v:deleted", false);
        edeleted_ = add_edge_property<bool>("e:deleted", false);
        fdeleted_ = add_face_property<bool>("f:deleted", false);
    
        deleted_vertices_ = deleted_edges_ = deleted_faces_ = 0;
        garbage_ = false;
    }

    // destructor (is virtual, since we inherit from Geometry_representation)
    virtual ~Surface_mesh(){}

    /// copy constructor: copies \c rhs to \c *this. performs a deep copy of all properties.
    Surface_mesh(const Surface_mesh& rhs) { operator=(rhs); }

    /// assign \c rhs to \c *this. performs a deep copy of all properties.
    Surface_mesh& operator=(const Surface_mesh& rhs)
    {
        if (this != &rhs)
        {
            // deep copy of property containers
            vprops_ = rhs.vprops_;
            hprops_ = rhs.hprops_;
            eprops_ = rhs.eprops_;
            fprops_ = rhs.fprops_;
    
            // property handles contain pointers, have to be reassigned
            vconn_    = vertex_property<Vertex_connectivity>("v:connectivity");
            hconn_    = halfedge_property<Halfedge_connectivity>("h:connectivity");
            fconn_    = face_property<Face_connectivity>("f:connectivity");
            vdeleted_ = vertex_property<bool>("v:deleted");
            edeleted_ = edge_property<bool>("e:deleted");
            fdeleted_ = face_property<bool>("f:deleted");
            vpoint_   = vertex_property<Point>("v:point");
    
            // normals might be there, therefore use get_property
            vnormal_  = get_vertex_property<Point>("v:normal");
            fnormal_  = get_face_property<Point>("f:normal");
    
            // how many elements are deleted?
            deleted_vertices_ = rhs.deleted_vertices_;
            deleted_edges_    = rhs.deleted_edges_;
            deleted_faces_    = rhs.deleted_faces_;
            garbage_          = rhs.garbage_;
        }
    
        return *this;
    }

    /// assign \c rhs to \c *this. does not copy custom properties.
    Surface_mesh& assign(const Surface_mesh& rhs)
    {
        if (this != &rhs)
        {
            // clear properties
            vprops_.clear();
            hprops_.clear();
            eprops_.clear();
            fprops_.clear();
    
            // allocate standard properties
            vconn_    = add_vertex_property<Vertex_connectivity>("v:connectivity");
            hconn_    = add_halfedge_property<Halfedge_connectivity>("h:connectivity");
            fconn_    = add_face_property<Face_connectivity>("f:connectivity");
            vpoint_   = add_vertex_property<Point>("v:point");
            vdeleted_ = add_vertex_property<bool>("v:deleted", false);
            edeleted_ = add_edge_property<bool>("e:deleted", false);
            fdeleted_ = add_face_property<bool>("f:deleted", false);
    
            // normals might be there, therefore use get_property
            vnormal_  = get_vertex_property<Point>("v:normal");
            fnormal_  = get_face_property<Point>("f:normal");
    
            // copy properties from other mesh
            vconn_.array()     = rhs.vconn_.array();
            hconn_.array()     = rhs.hconn_.array();
            fconn_.array()     = rhs.fconn_.array();
            vpoint_.array()    = rhs.vpoint_.array();
            vdeleted_.array()  = rhs.vdeleted_.array();
            edeleted_.array()  = rhs.edeleted_.array();
            fdeleted_.array()  = rhs.fdeleted_.array();
    
            // resize (needed by property containers)
            vprops_.resize(rhs.vertices_size());
            hprops_.resize(rhs.halfedges_size());
            eprops_.resize(rhs.edges_size());
            fprops_.resize(rhs.faces_size());
    
            // how many elements are deleted?
            deleted_vertices_ = rhs.deleted_vertices_;
            deleted_edges_    = rhs.deleted_edges_;
            deleted_faces_    = rhs.deleted_faces_;
            garbage_          = rhs.garbage_;
        }
    
        return *this;
    }

    //@}




public: //------------------------------------------------------------- file IO

    /// \name File IO
    //@{

    /// read mesh from file \c filename. file extension determines file type.
    /// \sa write(const std::string& filename)
    bool read(const std::string& filename){
        return read_mesh(*this, filename);
    }

    /// write mesh to file \c filename. file extensions determines file type.
    /// \sa read(const std::string& filename)
    bool write(const std::string& filename) const{
        return write_mesh(*this, filename);
    }

    //@}




public: //----------------------------------------------- add new vertex / face

    /// \name Add new elements by hand
    //@{

    /// add a new vertex with position \c p
    Vertex add_vertex(const Point& p)
    {
        Vertex v = new_vertex();
        vpoint_[v] = p;
        return v;        
    }
    
    /// add a new face with vertex list \c vertices
    /// \sa add_triangle, add_quad
    Face add_face(const std::vector<Vertex>& vertices)
    {
        const unsigned int n(vertices.size());
        assert (n > 2);
    
        Vertex        v;
        unsigned int  i, ii, id;
        Halfedge      inner_next, inner_prev, outer_next, outer_prev, boundary_next, boundary_prev, patch_start, patch_end;
    
    
        // use global arrays to avoid new/delete of local arrays!!!
        std::vector<Halfedge>&  halfedges    = add_face_halfedges_;
        std::vector<bool>&      is_new       = add_face_is_new_;
        std::vector<bool>&      needs_adjust = add_face_needs_adjust_;
        NextCache&              next_cache   = add_face_next_cache_;
        halfedges.clear();
        halfedges.resize(n);
        is_new.clear();
        is_new.resize(n);
        needs_adjust.clear();
        needs_adjust.resize(n, false);
        next_cache.clear();
        next_cache.reserve(3*n);
    
    
        // test for topological errors
        for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
        {
            if ( !is_boundary(vertices[i]) )
            {
                // std::cerr << "Surface_meshT::add_face: complex vertex\n";
                return Face();
            }
    
            halfedges[i] = find_halfedge(vertices[i], vertices[ii]);
            is_new[i]    = !halfedges[i].is_valid();
    
            if (!is_new[i] && !is_boundary(halfedges[i]))
            {
                // std::cerr << "Surface_meshT::add_face: complex edge\n";
                return Face();
            }
        }
    
    
        // re-link patches if necessary
        for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
        {
            if (!is_new[i] && !is_new[ii])
            {
                inner_prev = halfedges[i];
                inner_next = halfedges[ii];
    
                if (next_halfedge(inner_prev) != inner_next)
                {
                    // here comes the ugly part... we have to relink a whole patch
    
                    // search a free gap
                    // free gap will be between boundary_prev and boundary_next
                    outer_prev = opposite_halfedge(inner_next);
                    outer_next = opposite_halfedge(inner_prev);
                    boundary_prev = outer_prev;
                    do
                    {
                        boundary_prev = opposite_halfedge(next_halfedge(boundary_prev));
                    }
                    while (!is_boundary(boundary_prev) || boundary_prev==inner_prev);
                    boundary_next = next_halfedge(boundary_prev);
                    assert(is_boundary(boundary_prev));
                    assert(is_boundary(boundary_next));
    
    
                    // ok ?
                    if (boundary_next == inner_next)
                    {
                        // std::cerr << "Surface_meshT::add_face: patch re-linking failed\n";
                        return Face();
                    }
    
                    // other halfedges' handles
                    patch_start = next_halfedge(inner_prev);
                    patch_end   = prev_halfedge(inner_next);
    
                    // relink
                    next_cache.push_back(NextCacheEntry(boundary_prev, patch_start));
                    next_cache.push_back(NextCacheEntry(patch_end, boundary_next));
                    next_cache.push_back(NextCacheEntry(inner_prev, inner_next));
                }
            }
        }
    
    
    
        // create missing edges
        for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
        {
            if (is_new[i])
            {
                halfedges[i] = new_edge(vertices[i], vertices[ii]);
            }
        }
    
    
    
        // create the face
        Face f(new_face());
        set_halfedge(f, halfedges[n-1]);
    
    
    
        // setup halfedges
        for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
        {
            v          = vertices[ii];
            inner_prev = halfedges[i];
            inner_next = halfedges[ii];
    
            id = 0;
            if (is_new[i])  id |= 1;
            if (is_new[ii]) id |= 2;
    
            if (id)
            {
                outer_prev = opposite_halfedge(inner_next);
                outer_next = opposite_halfedge(inner_prev);
    
                // set outer links
                switch (id)
                {
                    case 1: // prev is new, next is old
                        boundary_prev = prev_halfedge(inner_next);
                        next_cache.push_back(NextCacheEntry(boundary_prev, outer_next));
                        set_halfedge(v, outer_next);
                        break;
    
                    case 2: // next is new, prev is old
                        boundary_next = next_halfedge(inner_prev);
                        next_cache.push_back(NextCacheEntry(outer_prev, boundary_next));
                        set_halfedge(v, boundary_next);
                        break;
    
                    case 3: // both are new
                        if (!halfedge(v).is_valid())
                        {
                            set_halfedge(v, outer_next);
                            next_cache.push_back(NextCacheEntry(outer_prev, outer_next));
                        }
                        else
                        {
                            boundary_next = halfedge(v);
                            boundary_prev = prev_halfedge(boundary_next);
                            next_cache.push_back(NextCacheEntry(boundary_prev, outer_next));
                            next_cache.push_back(NextCacheEntry(outer_prev, boundary_next));
                        }
                        break;
                }
    
                // set inner link
                next_cache.push_back(NextCacheEntry(inner_prev, inner_next));
            }
            else needs_adjust[ii] = (halfedge(v) == inner_next);
    
    
            // set face handle
            set_face(halfedges[i], f);
        }
    
    
    
        // process next halfedge cache
        NextCache::const_iterator ncIt(next_cache.begin()), ncEnd(next_cache.end());
        for (; ncIt != ncEnd; ++ncIt)
        {
            set_next_halfedge(ncIt->first, ncIt->second);
        }
    
    
    
        // adjust vertices' halfedge handle
        for (i=0; i<n; ++i)
        {
            if (needs_adjust[i])
            {
                adjust_outgoing_halfedge(vertices[i]);
            }
        }
    
    
        return f;
    }

    /// add a new triangle connecting vertices \c v0, \c v1, \c v2
    /// \sa add_face, add_quad
    Face add_triangle(Vertex v0, Vertex v1, Vertex v2)
    {
        add_face_vertices_.resize(3);
        add_face_vertices_[0] = v0;
        add_face_vertices_[1] = v1;
        add_face_vertices_[2] = v2;
        return add_face(add_face_vertices_);
    }

    /// add a new quad connecting vertices \c v1, \c v2, \c v3, \c v4
    /// \sa add_triangle, add_face
    Face add_quad(Vertex v1, Vertex v2, Vertex v3, Vertex v4)
    {
        add_face_vertices_.resize(4);
        add_face_vertices_[0] = v1;
        add_face_vertices_[1] = v2;
        add_face_vertices_[2] = v3;
        add_face_vertices_[3] = v4;
        return add_face(add_face_vertices_);
    }

    //@}




public: //--------------------------------------------------- memory management

    /// \name Memory Management
    //@{

    /// returns number of (deleted and valid) vertices in the mesh
    unsigned int vertices_size() const { return (unsigned int) vprops_.size(); }
    /// returns number of (deleted and valid)halfedge in the mesh
    unsigned int halfedges_size() const { return (unsigned int) hprops_.size(); }
    /// returns number of (deleted and valid)edges in the mesh
    unsigned int edges_size() const { return (unsigned int) eprops_.size(); }
    /// returns number of (deleted and valid)faces in the mesh
    unsigned int faces_size() const { return (unsigned int) fprops_.size(); }


    /// returns number of vertices in the mesh
    unsigned int n_vertices() const { return vertices_size() - deleted_vertices_; }
    /// returns number of halfedge in the mesh
    unsigned int n_halfedges() const { return halfedges_size() - 2*deleted_edges_; }
    /// returns number of edges in the mesh
    unsigned int n_edges() const { return edges_size() - deleted_edges_; }
    /// returns number of faces in the mesh
    unsigned int n_faces() const { return faces_size() - deleted_faces_; }


    /// returns true iff the mesh is empty, i.e., has no vertices
    unsigned int empty() const { return n_vertices() == 0; }


    /// clear mesh: remove all vertices, edges, faces
    void clear(){
        vprops_.resize(0);
        hprops_.resize(0);
        eprops_.resize(0);
        fprops_.resize(0);
    
        vprops_.free_memory();
        hprops_.free_memory();
        eprops_.free_memory();
        fprops_.free_memory();
    
        deleted_vertices_ = deleted_edges_ = deleted_faces_ = 0;
        garbage_ = false;
    }

    /// remove unused memory from vectors
    void free_memory(){
        vprops_.free_memory();
        hprops_.free_memory();
        eprops_.free_memory();
        fprops_.free_memory();
    }

    /// reserve memory (mainly used in file readers)
    void reserve(unsigned int nvertices,
                 unsigned int nedges,
                 unsigned int nfaces ){
        vprops_.reserve(nvertices);
        hprops_.reserve(2*nedges);
        eprops_.reserve(nedges);
        fprops_.reserve(nfaces);
    }

    /// remove deleted vertices/edges/faces
    void garbage_collection()
    {
        int  i, i0, i1,
        nV(vertices_size()),
        nE(edges_size()),
        nH(halfedges_size()),
        nF(faces_size());
    
        Vertex    v;
        Halfedge  h;
        Face      f;
    
    
        // setup handle mapping
        Vertex_property<Vertex>      vmap = add_vertex_property<Vertex>("v:garbage-collection");
        Halfedge_property<Halfedge>  hmap = add_halfedge_property<Halfedge>("h:garbage-collection");
        Face_property<Face>          fmap = add_face_property<Face>("f:garbage-collection");
        for (i=0; i<nV; ++i)
            vmap[Vertex(i)] = Vertex(i);
        for (i=0; i<nH; ++i)
            hmap[Halfedge(i)] = Halfedge(i);
        for (i=0; i<nF; ++i)
            fmap[Face(i)] = Face(i);
    
    
    
        // remove deleted vertices
        if (nV > 0)
        {
            i0=0;  i1=nV-1;
    
            while (1)
            {
                // find first deleted and last un-deleted
                while (!vdeleted_[Vertex(i0)] && i0 < i1)  ++i0;
                while ( vdeleted_[Vertex(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;
    
                // swap
                vprops_.swap(i0, i1);
            };
    
            // remember new size
            nV = vdeleted_[Vertex(i0)] ? i0 : i0+1;
        }
    
    
        // remove deleted edges
        if (nE > 0)
        {
            i0=0;  i1=nE-1;
    
            while (1)
            {
                // find first deleted and last un-deleted
                while (!edeleted_[Edge(i0)] && i0 < i1)  ++i0;
                while ( edeleted_[Edge(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;
    
                // swap
                eprops_.swap(i0, i1);
                hprops_.swap(2*i0,   2*i1);
                hprops_.swap(2*i0+1, 2*i1+1);
            };
    
            // remember new size
            nE = edeleted_[Edge(i0)] ? i0 : i0+1;
            nH = 2*nE;
        }
    
    
        // remove deleted faces
        if (nF > 0)
        {
            i0=0;  i1=nF-1;
    
            while (1)
            {
                // find 1st deleted and last un-deleted
                while (!fdeleted_[Face(i0)] && i0 < i1)  ++i0;
                while ( fdeleted_[Face(i1)] && i0 < i1)  --i1;
                if (i0 >= i1) break;
    
                // swap
                fprops_.swap(i0, i1);
            };
    
            // remember new size
            nF = fdeleted_[Face(i0)] ? i0 : i0+1;
        }
    
    
        // update vertex connectivity
        for (i=0; i<nV; ++i)
        {
            v = Vertex(i);
            if (!is_isolated(v))
                set_halfedge(v, hmap[halfedge(v)]);
        }
    
    
        // update halfedge connectivity
        for (i=0; i<nH; ++i)
        {
            h = Halfedge(i);
            set_vertex(h, vmap[to_vertex(h)]);
            set_next_halfedge(h, hmap[next_halfedge(h)]);
            if (!is_boundary(h))
                set_face(h, fmap[face(h)]);
        }
    
    
        // update handles of faces
        for (i=0; i<nF; ++i)
        {
            f = Face(i);
            set_halfedge(f, hmap[halfedge(f)]);
        }
    
    
        // remove handle maps
        remove_vertex_property(vmap);
        remove_halfedge_property(hmap);
        remove_face_property(fmap);
    
    
        // finally resize arrays
        vprops_.resize(nV); vprops_.free_memory();
        hprops_.resize(nH); hprops_.free_memory();
        eprops_.resize(nE); eprops_.free_memory();
        fprops_.resize(nF); fprops_.free_memory();
    
        deleted_vertices_ = deleted_edges_ = deleted_faces_ = 0;
        garbage_ = false;
    }


    /// returns whether vertex \c v is deleted
    /// \sa garbage_collection()
    bool is_deleted(Vertex v) const
    {
        return vdeleted_[v];
    }
    /// returns whether halfedge \c h is deleted
    /// \sa garbage_collection()
    bool is_deleted(Halfedge h) const
    {
        return edeleted_[edge(h)];
    }
    /// returns whether edge \c e is deleted
    /// \sa garbage_collection()
    bool is_deleted(Edge e) const
    {
        return edeleted_[e];
    }
    /// returns whether face \c f is deleted
    /// \sa garbage_collection()
    bool is_deleted(Face f) const
    {
        return fdeleted_[f];
    }


    /// return whether vertex \c v is valid, i.e. the index is stores it within the array bounds.
    bool is_valid(Vertex v) const
    {
        return (0 <= v.idx()) && (v.idx() < (int)vertices_size());
    }
    /// return whether halfedge \c h is valid, i.e. the index is stores it within the array bounds.
    bool is_valid(Halfedge h) const
    {
        return (0 <= h.idx()) && (h.idx() < (int)halfedges_size());
    }
    /// return whether edge \c e is valid, i.e. the index is stores it within the array bounds.
    bool is_valid(Edge e) const
    {
        return (0 <= e.idx()) && (e.idx() < (int)edges_size());
    }
    /// return whether face \c f is valid, i.e. the index is stores it within the array bounds.
    bool is_valid(Face f) const
    {
        return (0 <= f.idx()) && (f.idx() < (int)faces_size());
    }

    //@}




public: //---------------------------------------------- low-level connectivity

    /// \name Low-level connectivity
    //@{

    /// returns an outgoing halfedge of vertex \c v.
    /// if \c v is a boundary vertex this will be a boundary halfedge.
    Halfedge halfedge(Vertex v) const
    {
        return vconn_[v].halfedge_;
    }

    /// set the outgoing halfedge of vertex \c v to \c h
    void set_halfedge(Vertex v, Halfedge h)
    {
        vconn_[v].halfedge_ = h;
    }

    /// returns whether \c v is a boundary vertex
    bool is_boundary(Vertex v) const
    {
        Halfedge h(halfedge(v));
        return (!(h.is_valid() && face(h).is_valid()));
    }

    /// returns whether \c v is isolated, i.e., not incident to any face
    bool is_isolated(Vertex v) const
    {
        return !halfedge(v).is_valid();
    }

    /// returns whether \c v is a manifold vertex (not incident to several patches)
    bool is_manifold(Vertex v) const
    {
        // The vertex is non-manifold if more than one gap exists, i.e.
        // more than one outgoing boundary halfedge.
        int n(0);
        Halfedge_around_vertex_circulator hit = halfedges(v), hend=hit;
        if (hit) do
        {
            if (is_boundary(*hit))
                ++n;
        }
        while (++hit!=hend);
        return n<2;
    }

    /// returns the vertex the halfedge \c h points to
    Vertex to_vertex(Halfedge h) const
    {
        return hconn_[h].vertex_;
    }

    /// returns the vertex the halfedge \c h emanates from
    Vertex from_vertex(Halfedge h) const
    {
        return to_vertex(opposite_halfedge(h));
    }

    /// sets the vertex the halfedge \c h points to to \c v
    void set_vertex(Halfedge h, Vertex v)
    {
        hconn_[h].vertex_ = v;
    }

    /// returns the face incident to halfedge \c h
    Face face(Halfedge h) const
    {
        return hconn_[h].face_;
    }

    /// sets the incident face to halfedge \c h to \c f
    void set_face(Halfedge h, Face f)
    {
        hconn_[h].face_ = f;
    }

    /// returns the next halfedge within the incident face
    Halfedge next_halfedge(Halfedge h) const
    {
        return hconn_[h].next_halfedge_;
    }

    /// sets the next halfedge of \c h within the face to \c nh
    void set_next_halfedge(Halfedge h, Halfedge nh)
    {
        hconn_[h].next_halfedge_ = nh;
        hconn_[nh].prev_halfedge_ = h;
    }

    /// returns the previous halfedge within the incident face
    Halfedge prev_halfedge(Halfedge h) const
    {
        return hconn_[h].prev_halfedge_;
    }

    /// returns the opposite halfedge of \c h
    Halfedge opposite_halfedge(Halfedge h) const
    {
        return Halfedge((h.idx() & 1) ? h.idx()-1 : h.idx()+1);
    }

    /// returns the halfedge that is rotated counter-clockwise around the
    /// start vertex of \c h. it is the opposite halfedge of the previous halfedge of \c h.
    Halfedge ccw_rotated_halfedge(Halfedge h) const
    {
        return opposite_halfedge(prev_halfedge(h));
    }

    /// returns the halfedge that is rotated clockwise around the
    /// start vertex of \c h. it is the next halfedge of the opposite halfedge of \c h.
    Halfedge cw_rotated_halfedge(Halfedge h) const
    {
        return next_halfedge(opposite_halfedge(h));
    }

    /// return the edge that contains halfedge \c h as one of its two halfedges.
    Edge edge(Halfedge h) const
    {
        return Edge(h.idx() >> 1);
    }

    /// returns whether h is a boundary halfege, i.e., if its face does not exist.
    bool is_boundary(Halfedge h) const
    {
        return !face(h).is_valid();
    }


    /// returns the \c i'th halfedge of edge \c e. \c i has to be 0 or 1.
    Halfedge halfedge(Edge e, unsigned int i) const
    {
        assert(i<=1);
        return Halfedge((e.idx() << 1) + i);
    }

    /// returns the \c i'th vertex of edge \c e. \c i has to be 0 or 1.
    Vertex vertex(Edge e, unsigned int i) const
    {
        assert(i<=1);
        return to_vertex(halfedge(e, i));
    }

    /// returns the face incident to the \c i'th halfedge of edge \c e. \c i has to be 0 or 1.
    Face face(Edge e, unsigned int i) const
    {
        assert(i<=1);
        return face(halfedge(e, i));
    }

    /// returns whether \c e is a boundary edge, i.e., if one of its
    /// halfedges is a boundary halfedge.
    bool is_boundary(Edge e) const
    {
        return (is_boundary(halfedge(e, 0)) || is_boundary(halfedge(e, 1)));
    }

    /// returns a halfedge of face \c f
    Halfedge halfedge(Face f) const
    {
        return fconn_[f].halfedge_;
    }

    /// sets the halfedge of face \c f to \c h
    void set_halfedge(Face f, Halfedge h)
    {
        fconn_[f].halfedge_ = h;
    }

    /// returns whether \c f is a boundary face, i.e., it one of its edges is a boundary edge.
    bool is_boundary(Face f) const
    {
        Halfedge h  = halfedge(f);
        Halfedge hh = h;
        do
        {
            if (is_boundary(opposite_halfedge(h)))
                return true;
            h = next_halfedge(h);
        }
        while (h != hh);
        return false;
    }

    //@}




public: //--------------------------------------------------- property handling

    /// \name Property handling
    //@{

    /** add a vertex property of type \c T with name \c name and default value \c t.
     fails if a property named \c name exists already, since the name has to be unique.
     in this case it returns an invalid property */
    template <class T> Vertex_property<T> add_vertex_property(const std::string& name, const T t=T())
    {
        return Vertex_property<T>(vprops_.add<T>(name, t));
    }
    /** add a halfedge property of type \c T with name \c name and default value \c t.
     fails if a property named \c name exists already, since the name has to be unique.
     in this case it returns an invalid property */
    template <class T> Halfedge_property<T> add_halfedge_property(const std::string& name, const T t=T())
    {
        return Halfedge_property<T>(hprops_.add<T>(name, t));
    }
    /** add a edge property of type \c T with name \c name and default value \c t.
     fails if a property named \c name exists already, since the name has to be unique.
     in this case it returns an invalid property */
    template <class T> Edge_property<T> add_edge_property(const std::string& name, const T t=T())
    {
        return Edge_property<T>(eprops_.add<T>(name, t));
    }
    /** add a face property of type \c T with name \c name and default value \c t.
     fails if a property named \c name exists already, since the name has to be unique.
     in this case it returns an invalid property */
    template <class T> Face_property<T> add_face_property(const std::string& name, const T t=T())
    {
        return Face_property<T>(fprops_.add<T>(name, t));
    }


    /** get the vertex property named \c name of type \c T. returns an invalid
     Vertex_property if the property does not exist or if the type does not match. */
    template <class T> Vertex_property<T> get_vertex_property(const std::string& name) const
    {
        return Vertex_property<T>(vprops_.get<T>(name));
    }
    /** get the halfedge property named \c name of type \c T. returns an invalid
     Vertex_property if the property does not exist or if the type does not match. */
    template <class T> Halfedge_property<T> get_halfedge_property(const std::string& name) const
    {
        return Halfedge_property<T>(hprops_.get<T>(name));
    }
    /** get the edge property named \c name of type \c T. returns an invalid
     Vertex_property if the property does not exist or if the type does not match. */
    template <class T> Edge_property<T> get_edge_property(const std::string& name) const
    {
        return Edge_property<T>(eprops_.get<T>(name));
    }
    /** get the face property named \c name of type \c T. returns an invalid
     Vertex_property if the property does not exist or if the type does not match. */
    template <class T> Face_property<T> get_face_property(const std::string& name) const
    {
        return Face_property<T>(fprops_.get<T>(name));
    }


    /** if a vertex property of type \c T with name \c name exists, it is returned.
     otherwise this property is added (with default value \c t) */
    template <class T> Vertex_property<T> vertex_property(const std::string& name, const T t=T())
    {
        return Vertex_property<T>(vprops_.get_or_add<T>(name, t));
    }
    /** if a halfedge property of type \c T with name \c name exists, it is returned.
     otherwise this property is added (with default value \c t) */
    template <class T> Halfedge_property<T> halfedge_property(const std::string& name, const T t=T())
    {
        return Halfedge_property<T>(hprops_.get_or_add<T>(name, t));
    }
    /** if an edge property of type \c T with name \c name exists, it is returned.
     otherwise this property is added (with default value \c t) */
    template <class T> Edge_property<T> edge_property(const std::string& name, const T t=T())
    {
        return Edge_property<T>(eprops_.get_or_add<T>(name, t));
    }
    /** if a face property of type \c T with name \c name exists, it is returned.
     otherwise this property is added (with default value \c t) */
    template <class T> Face_property<T> face_property(const std::string& name, const T t=T())
    {
        return Face_property<T>(fprops_.get_or_add<T>(name, t));
    }


    /// remove the vertex property \c p
    template <class T> void remove_vertex_property(Vertex_property<T>& p)
    {
        vprops_.remove(p);
    }
    /// remove the halfedge property \c p
    template <class T> void remove_halfedge_property(Halfedge_property<T>& p)
    {
        hprops_.remove(p);
    }
    /// remove the edge property \c p
    template <class T> void remove_edge_property(Edge_property<T>& p)
    {
        eprops_.remove(p);
    }
    /// remove the face property \c p
    template <class T> void remove_face_property(Face_property<T>& p)
    {
        fprops_.remove(p);
    }


    /** get the type_info \c T of vertex property named \c. returns an typeid(void)
     if the property does not exist or if the type does not match. */
    const std::type_info& get_vertex_property_type(const std::string& name)
    {
        return vprops_.get_type(name);
    }
    /** get the type_info \c T of halfedge property named \c. returns an typeid(void)
     if the property does not exist or if the type does not match. */
    const std::type_info& get_halfedge_property_type(const std::string& name)
    {
        return hprops_.get_type(name);
    }
    /** get the type_info \c T of edge property named \c. returns an typeid(void)
     if the property does not exist or if the type does not match. */
    const std::type_info& get_edge_property_type(const std::string& name)
    {
        return eprops_.get_type(name);
    }
    /** get the type_info \c T of face property named \c. returns an typeid(void)
     if the property does not exist or if the type does not match. */
    const std::type_info& get_face_property_type(const std::string& name)
    {
        return fprops_.get_type(name);
    }


    /// returns the names of all vertex properties
    std::vector<std::string> vertex_properties() const
    {
        return vprops_.properties();
    }
    /// returns the names of all halfedge properties
    std::vector<std::string> halfedge_properties() const
    {
        return hprops_.properties();
    }
    /// returns the names of all edge properties
    std::vector<std::string> edge_properties() const
    {
        return eprops_.properties();
    }
    /// returns the names of all face properties
    std::vector<std::string> face_properties() const
    {
        return fprops_.properties();
    }
    /// prints the names of all properties
    void property_stats() const
    {
        std::vector<std::string> props;
    
        std::cout << "vertex properties:\n";
        props = vertex_properties();
        for (unsigned int i=0; i<props.size(); ++i)
            std::cout << "\t" << props[i] << std::endl;
    
        std::cout << "halfedge properties:\n";
        props = halfedge_properties();
        for (unsigned int i=0; i<props.size(); ++i)
            std::cout << "\t" << props[i] << std::endl;
    
        std::cout << "edge properties:\n";
        props = edge_properties();
        for (unsigned int i=0; i<props.size(); ++i)
            std::cout << "\t" << props[i] << std::endl;
    
        std::cout << "face properties:\n";
        props = face_properties();
        for (unsigned int i=0; i<props.size(); ++i)
            std::cout << "\t" << props[i] << std::endl;
    }

    //@}




public: //--------------------------------------------- iterators & circulators

    /// \name Iterators & Circulators
    //@{

    /// returns start iterator for vertices
    Vertex_iterator vertices_begin() const
    {
        return Vertex_iterator(Vertex(0), this);
    }

    /// returns end iterator for vertices
    Vertex_iterator vertices_end() const
    {
        return Vertex_iterator(Vertex(vertices_size()), this);
    }

    /// returns vertex container for C++11 range-based for-loops
    Vertex_container vertices() const
    {
        return Vertex_container(vertices_begin(), vertices_end());
    }

    /// returns start iterator for halfedges
    Halfedge_iterator halfedges_begin() const
    {
        return Halfedge_iterator(Halfedge(0), this);
    }

    /// returns end iterator for halfedges
    Halfedge_iterator halfedges_end() const
    {
        return Halfedge_iterator(Halfedge(halfedges_size()), this);
    }

    /// returns halfedge container for C++11 range-based for-loops
    Halfedge_container halfedges() const
    {
        return Halfedge_container(halfedges_begin(), halfedges_end());
    }

    /// returns start iterator for edges
    Edge_iterator edges_begin() const
    {
        return Edge_iterator(Edge(0), this);
    }

    /// returns end iterator for edges
    Edge_iterator edges_end() const
    {
        return Edge_iterator(Edge(edges_size()), this);
    }

    /// returns edge container for C++11 range-based for-loops
    Edge_container edges() const
    {
        return Edge_container(edges_begin(), edges_end());
    }

    /// returns start iterator for faces
    Face_iterator faces_begin() const
    {
        return Face_iterator(Face(0), this);
    }

    /// returns end iterator for faces
    Face_iterator faces_end() const
    {
        return Face_iterator(Face(faces_size()), this);
    }

    /// returns face container for C++11 range-based for-loops
    Face_container faces() const
    {
        return Face_container(faces_begin(), faces_end());
    }

    /// returns circulator for vertices around vertex \c v
    Vertex_around_vertex_circulator vertices(Vertex v) const
    {
        return Vertex_around_vertex_circulator(this, v);
    }

    /// returns circulator for outgoing halfedges around vertex \c v
    Halfedge_around_vertex_circulator halfedges(Vertex v) const
    {
        return Halfedge_around_vertex_circulator(this, v);
    }

    /// returns circulator for faces around vertex \c v
    Face_around_vertex_circulator faces(Vertex v) const
    {
        return Face_around_vertex_circulator(this, v);
    }

    /// returns circulator for vertices of face \c f
    Vertex_around_face_circulator vertices(Face f) const
    {
        return Vertex_around_face_circulator(this, f);
    }

    /// returns circulator for halfedges of face \c f
    Halfedge_around_face_circulator halfedges(Face f) const
    {
        return Halfedge_around_face_circulator(this, f);
    }

    //@}





public: //--------------------------------------------- higher-level operations

    /// \name Higher-level Topological Operations
    //@{

    /// returns whether the mesh a triangle mesh. this function simply tests
    /// each face, and therefore is not very efficient.
    /// \sa trianglate(), triangulate(Face)
    bool is_triangle_mesh() const
    {
        Face_iterator fit=faces_begin(), fend=faces_end();
        for (; fit!=fend; ++fit)
            if (valence(*fit) != 3)
                return false;
    
        return true;        
    }

    /// returns whether the mesh a quad mesh. this function simply tests
    /// each face, and therefore is not very efficient.
    bool is_quad_mesh() const
    {
        Face_iterator fit=faces_begin(), fend=faces_end();
        for (; fit!=fend; ++fit)
            if (valence(*fit) != 4)
                return false;
    
        return true;
    }

    /// triangulate the entire mesh, by calling triangulate(Face) for each face.
    /// \sa trianglate(Face)
    void triangulate()
    {
        /* The iterators will stay valid, even though new faces are added,
         because they are now implemented index-based instead of
         pointer-based.
         */
        Face_iterator fit=faces_begin(), fend=faces_end();
        for (; fit!=fend; ++fit)
            triangulate(*fit);
    }

    /// triangulate the face \c f
    /// \sa trianglate()
    void triangulate(Face f)
    {
        /*
         Split an arbitrary face into triangles by connecting
         each vertex of fh after its second to vh.
    
         - fh will remain valid (it will become one of the
         triangles)
         - the halfedge handles of the new triangles will
         point to the old halfedges
         */
    
        Halfedge base_h  = halfedge(f);
        Vertex   start_v = from_vertex(base_h);
        Halfedge next_h  = next_halfedge(base_h);
    
        while (to_vertex(next_halfedge(next_h)) != start_v)
        {
            Halfedge next_next_h(next_halfedge(next_h));
    
            Face new_f = new_face();
            set_halfedge(new_f, base_h);
    
            Halfedge new_h = new_edge(to_vertex(next_h), start_v);
    
            set_next_halfedge(base_h, next_h);
            set_next_halfedge(next_h, new_h);
            set_next_halfedge(new_h,  base_h);
    
            set_face(base_h, new_f);
            set_face(next_h, new_f);
            set_face(new_h,  new_f);
    
            base_h = opposite_halfedge(new_h);
            next_h = next_next_h;
        }
        set_halfedge(f, base_h);  //the last face takes the handle _fh
    
        set_next_halfedge(base_h, next_h);
        set_next_halfedge(next_halfedge(next_h), base_h);
    
        set_face(base_h, f);
    }


    /// returns whether collapsing the halfedge \c h is topologically legal.
    /// \attention This function is only valid for triangle meshes.
    bool is_collapse_ok(Halfedge v0v1)
    {
        Halfedge  v1v0(opposite_halfedge(v0v1));
        Vertex    v0(to_vertex(v1v0));
        Vertex    v1(to_vertex(v0v1));
        Vertex    vv, vl, vr;
        Halfedge  h1, h2;
    
    
        // the edges v1-vl and vl-v0 must not be both boundary edges
        if (!is_boundary(v0v1))
        {
            vl = to_vertex(next_halfedge(v0v1));
            h1 = next_halfedge(v0v1);
            h2 = next_halfedge(h1);
            if (is_boundary(opposite_halfedge(h1)) && is_boundary(opposite_halfedge(h2)))
                return false;
        }
    
    
        // the edges v0-vr and vr-v1 must not be both boundary edges
        if (!is_boundary(v1v0))
        {
            vr = to_vertex(next_halfedge(v1v0));
            h1 = next_halfedge(v1v0);
            h2 = next_halfedge(h1);
            if (is_boundary(opposite_halfedge(h1)) && is_boundary(opposite_halfedge(h2)))
                return false;
        }
    
    
        // if vl and vr are equal or both invalid -> fail
        if (vl == vr) return false;
    
    
        // edge between two boundary vertices should be a boundary edge
        if ( is_boundary(v0) && is_boundary(v1) &&
            !is_boundary(v0v1) && !is_boundary(v1v0))
            return false;
    
    
        // test intersection of the one-rings of v0 and v1
        Vertex_around_vertex_circulator vv_it, vv_end;
        vv_it = vv_end = vertices(v0);
        do
        {
            vv = *vv_it;
            if (vv != v1 && vv != vl && vv != vr)
                if (find_halfedge(vv, v1).is_valid())
                    return false;
        }
        while (++vv_it != vv_end);
    
    
        // passed all tests
        return true;
    }

    /** Collapse the halfedge \c h by moving its start vertex into its target
     vertex. For non-boundary halfedges this function removes one vertex, three
     edges, and two faces. For boundary halfedges it removes one vertex, two
     edges and one face.
     \attention This function is only valid for triangle meshes.
     \attention Halfedge collapses might lead to invalid faces. Call
     is_collapse_ok(Halfedge) to be sure the collapse is legal.
     \attention The removed items are only marked as deleted. You have
     to call garbage_collection() to finally remove them.
     */
    void collapse(Halfedge h)
    {
        Halfedge h0 = h;
        Halfedge h1 = prev_halfedge(h0);
        Halfedge o0 = opposite_halfedge(h0);
        Halfedge o1 = next_halfedge(o0);
    
        // remove edge
        remove_edge(h0);
    
        // remove loops
        if (next_halfedge(next_halfedge(h1)) == h1)
            remove_loop(h1);
        if (next_halfedge(next_halfedge(o1)) == o1)
            remove_loop(o1);
    }


    /** Split the face \c f by first adding point \c p to the mesh and then
     inserting edges between \c p and the vertices of \c f. For a triangle
     this is a standard one-to-three split.
     \sa split(Face, Vertex)
     */
    Vertex split(Face f, const Point& p) { Vertex v=add_vertex(p); split(f,v); return v; }

    /** Split the face \c f by inserting edges between \c p and the vertices
     of \c f. For a triangle this is a standard one-to-three split.
     \sa split(Face, const Point&)
     */
    void split(Face f, Vertex v)
    {
        /*
         Split an arbitrary face into triangles by connecting each vertex of fh to vh.
         - fh will remain valid (it will become one of the triangles)
         - the halfedge handles of the new triangles will point to the old halfeges
         */
    
        Halfedge hend = halfedge(f);
        Halfedge h    = next_halfedge(hend);
    
        Halfedge hold = new_edge(to_vertex(hend), v);
    
        set_next_halfedge(hend, hold);
        set_face(hold, f);
    
        hold = opposite_halfedge(hold);
    
        while (h != hend)
        {
            Halfedge hnext = next_halfedge(h);
    
            Face fnew = new_face();
            set_halfedge(fnew, h);
    
            Halfedge hnew = new_edge(to_vertex(h), v);
    
            set_next_halfedge(hnew, hold);
            set_next_halfedge(hold, h);
            set_next_halfedge(h,    hnew);
    
            set_face(hnew, fnew);
            set_face(hold, fnew);
            set_face(h,    fnew);
    
            hold = opposite_halfedge(hnew);
    
            h = hnext;
        }
    
        set_next_halfedge(hold, hend);
        set_next_halfedge(next_halfedge(hend), hold);
    
        set_face(hold, f);
    
        set_halfedge(v, hold);
    }


    /** Split the edge \c e by first adding point \c p to the mesh and then
     connecting it to the two vertices of the adjacent triangles that are
     opposite to edge \c e.
     \attention This function is only valid for triangle meshes.
     \sa split(Edge, Vertex)
     */
    Vertex split(Edge e, const Point& p) { Vertex v=add_vertex(p); split(e,v); return v; }

    /** Split the edge \c e by connecting vertex \c v it to the two vertices
     of the adjacent triangles that are opposite to edge \c e.
     \attention This function is only valid for triangle meshes.
     \sa split(Edge, Point)
     */
    void split(Edge e, Vertex v)
    {
        Halfedge h0 = halfedge(e, 0);
        Halfedge o0 = halfedge(e, 1);
    
        Vertex   v2 = to_vertex(o0);
    
        Halfedge e1 = new_edge(v, v2);
        Halfedge t1 = opposite_halfedge(e1);
    
        Face     f0 = face(h0);
        Face     f3 = face(o0);
    
        set_halfedge(v, h0);
        set_vertex(o0, v);
    
        if (!is_boundary(h0))
        {
            Halfedge h1 = next_halfedge(h0);
            Halfedge h2 = next_halfedge(h1);
    
            Vertex   v1 = to_vertex(h1);
    
            Halfedge e0 = new_edge(v, v1);
            Halfedge t0 = opposite_halfedge(e0);
    
            Face f1 = new_face();
            set_halfedge(f0, h0);
            set_halfedge(f1, h2);
    
            set_face(h1, f0);
            set_face(t0, f0);
            set_face(h0, f0);
    
            set_face(h2, f1);
            set_face(t1, f1);
            set_face(e0, f1);
    
            set_next_halfedge(h0, h1);
            set_next_halfedge(h1, t0);
            set_next_halfedge(t0, h0);
    
            set_next_halfedge(e0, h2);
            set_next_halfedge(h2, t1);
            set_next_halfedge(t1, e0);
        }
        else
        {
            set_next_halfedge(prev_halfedge(h0), t1);
            set_next_halfedge(t1, h0);
            // halfedge handle of _vh already is h0
        }
    
    
        if (!is_boundary(o0))
        {
            Halfedge o1 = next_halfedge(o0);
            Halfedge o2 = next_halfedge(o1);
    
            Vertex v3 = to_vertex(o1);
    
            Halfedge e2 = new_edge(v, v3);
            Halfedge t2 = opposite_halfedge(e2);
    
            Face f2 = new_face();
            set_halfedge(f2, o1);
            set_halfedge(f3, o0);
    
            set_face(o1, f2);
            set_face(t2, f2);
            set_face(e1, f2);
    
            set_face(o2, f3);
            set_face(o0, f3);
            set_face(e2, f3);
    
            set_next_halfedge(e1, o1);
            set_next_halfedge(o1, t2);
            set_next_halfedge(t2, e1);
    
            set_next_halfedge(o0, e2);
            set_next_halfedge(e2, o2);
            set_next_halfedge(o2, o0);
        }
        else
        {
            set_next_halfedge(e1, next_halfedge(o0));
            set_next_halfedge(o0, e1);
            set_halfedge(v, e1);
        }
    
        if (halfedge(v2) == h0)
            set_halfedge(v2, t1);
    }


    /** Subdivide the edge \c e = (v0,v1) by splitting it into the two edge
     (v0,p) and (p,v1). Note that this function does not introduce any other
     edge or faces. It simply splits the edge. Returns halfedge that points to \c p.
     \sa insert_vertex(Edge, Vertex)
     \sa insert_vertex(Halfedge, Vertex)
     */
    Halfedge insert_vertex(Edge e, const Point& p)
    {
        return insert_vertex(halfedge(e,0), add_vertex(p));
    }

    /** Subdivide the edge \c e = (v0,v1) by splitting it into the two edge
     (v0,v) and (v,v1). Note that this function does not introduce any other
     edge or faces. It simply splits the edge. Returns halfedge that points to \c p.
     \sa insert_vertex(Edge, Point)
     \sa insert_vertex(Halfedge, Vertex)
     */
    Halfedge insert_vertex(Edge e, Vertex v)
    {
        return insert_vertex(halfedge(e,0), v);
    }

    /** Subdivide the edge \c e = (v0,v1) by splitting it into the two edge
     (v0,v) and (v,v1). Note that this function does not introduce any other
     edge or faces. It simply splits the edge. Returns halfedge that points to \c p.
     \sa insert_vertex(Edge, Point)
     \sa insert_vertex(Edge, Vertex)
     */
    Halfedge insert_vertex(Halfedge h0, Vertex v)
    {
        // before:
        //
        // v0      h0       v2
        //  o--------------->o
        //   <---------------
        //         o0
        //
        // after:
        //
        // v0  h0   v   h1   v2
        //  o------>o------->o
        //   <------ <-------
        //     o0       o1
    
        Halfedge h2 = next_halfedge(h0);
        Halfedge o0 = opposite_halfedge(h0);
        Halfedge o2 = prev_halfedge(o0);
        Vertex   v2 = to_vertex(h0);
        Face     fh = face(h0);
        Face     fo = face(o0);
    
        Halfedge h1 = new_edge(v, v2);
        Halfedge o1 = opposite_halfedge(h1);
    
        // adjust halfedge connectivity
        set_next_halfedge(h1, h2);
        set_next_halfedge(h0, h1);
        set_vertex(h0, v);
        set_vertex(h1, v2);
        set_face(h1, fh);
    
        set_next_halfedge(o1, o0);
        set_next_halfedge(o2, o1);
        set_vertex(o1, v);
        set_face(o1, fo);
    
        // adjust vertex connectivity
        set_halfedge(v2, o1);
        adjust_outgoing_halfedge(v2);
        set_halfedge(v, h1);
        adjust_outgoing_halfedge(v);
    
        // adjust face connectivity
        if (fh.is_valid()) set_halfedge(fh, h0);
        if (fo.is_valid()) set_halfedge(fo, o1);
    
        return h0;
    }


    /// insert edge between the to-vertices v0 of h0 and v1 of h1.
    /// returns the new halfedge from v0 to v1.
    /// \attention h0 and h1 have to belong to the same face
    Halfedge insert_edge(Halfedge h0, Halfedge h1)
    {
        assert(face(h0) == face(h1));
        assert(face(h0).is_valid());
    
        Vertex   v0 = to_vertex(h0);
        Vertex   v1 = to_vertex(h1);
    
        Halfedge h2 = next_halfedge(h0);
        Halfedge h3 = next_halfedge(h1);
    
        Halfedge h4 = new_edge(v0, v1);
        Halfedge h5 = opposite_halfedge(h4);
    
        Face     f0 = face(h0);
        Face     f1 = new_face();
    
        set_halfedge(f0, h0);
        set_halfedge(f1, h1);
    
        set_next_halfedge(h0, h4);
        set_next_halfedge(h4, h3);
        set_face(h4, f0);
    
        set_next_halfedge(h1, h5);
        set_next_halfedge(h5, h2);
        Halfedge h = h2;
        do
        {
            set_face(h, f1);
            h = next_halfedge(h);
        }
        while (h != h2);
    
        return h4;
    }


    /** Check whether flipping edge \c e is topologically
     \attention This function is only valid for triangle meshes.
     \sa flip(Edge)
     */
    bool is_flip_ok(Edge e) const
    {
        // boundary edges cannot be flipped
        if (is_boundary(e)) return false;
    
        // check if the flipped edge is already present in the mesh
    
        Halfedge h0 = halfedge(e, 0);
        Halfedge h1 = halfedge(e, 1);
    
        Vertex v0 = to_vertex(next_halfedge(h0));
        Vertex v1 = to_vertex(next_halfedge(h1));
    
        if (v0 == v1)   // this is generally a bad sign !!!
            return false;
    
        if (find_halfedge(v0, v1).is_valid())
            return false;
    
        return true;
    }

    /** Flip edge \c e: Remove edge \c e and add an edge between the two vertices
     opposite to edge \c e of the two incident triangles.
     \attention This function is only valid for triangle meshes.
     \sa is_flip_ok(Edge)
     */
    void flip(Edge e)
    {
        // CAUTION : Flipping a halfedge may result in
        // a non-manifold mesh, hence check for yourself
        // whether this operation is allowed or not!
    
        //let's make it sure it is actually checked
        assert(is_flip_ok(e));
    
        Halfedge a0 = halfedge(e, 0);
        Halfedge b0 = halfedge(e, 1);
    
        Halfedge a1 = next_halfedge(a0);
        Halfedge a2 = next_halfedge(a1);
    
        Halfedge b1 = next_halfedge(b0);
        Halfedge b2 = next_halfedge(b1);
    
        Vertex   va0 = to_vertex(a0);
        Vertex   va1 = to_vertex(a1);
    
        Vertex   vb0 = to_vertex(b0);
        Vertex   vb1 = to_vertex(b1);
    
        Face     fa  = face(a0);
        Face     fb  = face(b0);
    
        set_vertex(a0, va1);
        set_vertex(b0, vb1);
    
        set_next_halfedge(a0, a2);
        set_next_halfedge(a2, b1);
        set_next_halfedge(b1, a0);
    
        set_next_halfedge(b0, b2);
        set_next_halfedge(b2, a1);
        set_next_halfedge(a1, b0);
    
        set_face(a1, fb);
        set_face(b1, fa);
    
        set_halfedge(fa, a0);
        set_halfedge(fb, b0);
    
        if (halfedge(va0) == b0)
            set_halfedge(va0, a1);
        if (halfedge(vb0) == a0)
            set_halfedge(vb0, b1);
    }


    /** returns the valence (number of incident edges or neighboring vertices)
     of vertex \c v. */
    unsigned int valence(Vertex v) const
    {
        unsigned int count(0);
    
        Vertex_around_vertex_circulator vvit = vertices(v);
        Vertex_around_vertex_circulator vvend = vvit;
        if (vvit) do
        {
            ++count;
        } while (++vvit != vvend);
    
        return count;
    }

    /// returns the valence of face \c f (its number of vertices)
    unsigned int valence(Face f) const
    {
        unsigned int count(0);
    
        Vertex_around_face_circulator fvit = vertices(f);
        Vertex_around_face_circulator fvend = fvit;
        do {
            ++count;
        } while (++fvit != fvend);
    
        return count;
    }

    /// find the halfedge from start to end
    Halfedge find_halfedge(Vertex start, Vertex end) const
    {
        assert(is_valid(start) && is_valid(end));
    
        Halfedge h  = halfedge(start);
        const Halfedge hh = h;
    
        if (h.is_valid())
        {
            do
            {
                if (to_vertex(h) == end)
                    return h;
                h = cw_rotated_halfedge(h);
            }
            while (h != hh);
        }
    
        return Halfedge();   
    }

    /// find the edge (a,b)
    Edge find_edge(Vertex a, Vertex b) const
    {
        Halfedge h = find_halfedge(a,b);
        return h.is_valid() ? edge(h) : Edge();
    }

    /// deletes the vertex \c v from the mesh
    void delete_vertex(Vertex v)
    {
        if (vdeleted_[v])  return;
    
        // collect incident faces
        std::vector<Face> incident_faces;
        incident_faces.reserve(6);
    
        Face_around_vertex_circulator fc, fc_end;
        fc = fc_end = faces(v);
    
        if (fc)
        do
        {
            incident_faces.push_back(*fc);
        } while (++fc != fc_end);
    
        // delete incident faces
        std::vector<Face>::iterator fit(incident_faces.begin()),
                                    fend(incident_faces.end());
    
        for (; fit != fend; ++fit)
            delete_face(*fit);
    
        vdeleted_[v] = true;
        deleted_vertices_++;
        garbage_ = true;
    }

    /// deletes the edge \c e from the mesh
    void delete_edge(Edge e)
    {
        if (edeleted_[e])  return;
    
        Face f0 = face(halfedge(e, 0));
        Face f1 = face(halfedge(e, 1));
    
        if (f0.is_valid()) delete_face(f0);
        if (f1.is_valid()) delete_face(f1);
    }

    /// deletes the face \c f from the mesh
    void delete_face(Face f)
    {
        if (fdeleted_[f])  return;
        
        // mark face deleted
        if (!fdeleted_[f])
        {
            fdeleted_[f] = true;
            deleted_faces_++;
        }
    
        // boundary edges of face f to be deleted
        std::vector<Edge> deleted_edges;
        deleted_edges.reserve(3);
    
    
        // vertices of face f for updating their outgoing halfedge
        std::vector<Vertex> vertices;
        vertices.reserve(3);
    
    
        // for all halfedges of face f do:
        //   1) invalidate face handle.
        //   2) collect all boundary halfedges, set them deleted
        //   3) store vertex handles
        Halfedge_around_face_circulator hc, hc_end;
        hc = hc_end = halfedges(f);
    
        do
        {
            set_face(*hc, Face());
    
            if (is_boundary(opposite_halfedge(*hc)))
                deleted_edges.push_back(edge(*hc));
    
            vertices.push_back(to_vertex(*hc));
    
        } while (++hc != hc_end);
    
    
        // delete all collected (half)edges
        // delete isolated vertices
        if (!deleted_edges.empty())
        {
            std::vector<Edge>::iterator del_it(deleted_edges.begin()),
                                        del_end(deleted_edges.end());
    
            Halfedge h0, h1, next0, next1, prev0, prev1;
            Vertex   v0, v1;
    
            for (; del_it!=del_end; ++del_it)
            {
                h0    = halfedge(*del_it, 0);
                v0    = to_vertex(h0);
                next0 = next_halfedge(h0);
                prev0 = prev_halfedge(h0);
    
                h1    = halfedge(*del_it, 1);
                v1    = to_vertex(h1);
                next1 = next_halfedge(h1);
                prev1 = prev_halfedge(h1);
    
                // adjust next and prev handles
                set_next_halfedge(prev0, next1);
                set_next_halfedge(prev1, next0);
    
                // mark edge deleted
                if (!edeleted_[*del_it])
                {
                    edeleted_[*del_it] = true;
                    deleted_edges_++;
                }
    
                // update v0
                if (halfedge(v0) == h1)
                {
                    if (next0 == h1)
                    {
                        if (!vdeleted_[v0])
                        {
                            vdeleted_[v0] = true;
                            deleted_vertices_++;
                        }
                    }
                    else set_halfedge(v0, next0);
                }
    
                // update v1
                if (halfedge(v1) == h0)
                {
                    if (next1 == h0)
                    {
                        if (!vdeleted_[v1])
                        {
                            vdeleted_[v1] = true;
                            deleted_vertices_++;
                        }
                    }
                    else  set_halfedge(v1, next1);
                }
            }
        }
    
    
        // update outgoing halfedge handles of remaining vertices
        std::vector<Vertex>::iterator v_it(vertices.begin()),
                                      v_end(vertices.end());
        for (; v_it!=v_end; ++v_it)
            adjust_outgoing_halfedge(*v_it);
    
        garbage_ = true;
    }

    //@}


public: //------------------------------------------ geometry-related functions

    /// \name Geometry-related Functions
    //@{

    /// position of a vertex (read only)
    const Point& position(Vertex v) const { return vpoint_[v]; }

    /// position of a vertex
    Point& position(Vertex v) { return vpoint_[v]; }

    /// vector of vertex positions
    std::vector<Point>& points() { return vpoint_.vector(); }

    /// compute face normals by calling compute_face_normal(Face) for each face.
    void update_face_normals()
    {
        if (!fnormal_)
            fnormal_ = face_property<Point>("f:normal");
    
        Face_iterator fit, fend=faces_end();
    
        for (fit=faces_begin(); fit!=fend; ++fit)
            fnormal_[*fit] = compute_face_normal(*fit);
    }

    /// compute normal vector of face \c f.
    Normal compute_face_normal(Face f) const
    {
        Halfedge h = halfedge(f);
        Halfedge hend = h;
    
        Point p0 = vpoint_[to_vertex(h)];
        h = next_halfedge(h);
        Point p1 = vpoint_[to_vertex(h)];
        h = next_halfedge(h);
        Point p2 = vpoint_[to_vertex(h)];
    
        if (next_halfedge(h) == hend) // face is a triangle
        {
            return cross(p2-=p1, p0-=p1).normalize();
        }
    
        else // face is a general polygon
        {
            Normal n(0,0,0);
    
            hend = h;
            do
            {
                n += cross(p2-p1, p0-p1);
                h  = next_halfedge(h);
                p0 = p1;
                p1 = p2;
                p2 = vpoint_[to_vertex(h)];
            }
            while (h != hend);
    
            return n.normalize();
        }
    }

    /// compute vertex normals by calling compute_vertex_normal(Vertex) for each vertex.
    void update_vertex_normals()
    {
        if (!vnormal_)
            vnormal_ = vertex_property<Point>("v:normal");
    
        Vertex_iterator vit, vend=vertices_end();
    
        for (vit=vertices_begin(); vit!=vend; ++vit)
            vnormal_[*vit] = compute_vertex_normal(*vit);
    }

    /// compute normal vector of vertex \c v.
    Normal compute_vertex_normal(Vertex v) const
    {
        Point     nn(0,0,0);
        Halfedge  h = halfedge(v);
    
        if (h.is_valid())
        {
            const Halfedge hend = h;
            const Point p0 = vpoint_[v];
    
            Point   n, p1, p2;
            Scalar  cosine, angle, denom;
    
            do
            {
                if (!is_boundary(h))
                {
                    p1 = vpoint_[to_vertex(h)];
                    p1 -= p0;
    
                    p2 = vpoint_[from_vertex(prev_halfedge(h))];
                    p2 -= p0;
    
                    // check whether we can robustly compute angle
                    denom = sqrt(dot(p1,p1)*dot(p2,p2));
                    if (denom > std::numeric_limits<Scalar>::min())
                    {
                        cosine = dot(p1,p2) / denom;
                        if      (cosine < -1.0) cosine = -1.0;
                        else if (cosine >  1.0) cosine =  1.0;
                        angle = acos(cosine);
    
                        n   = cross(p1,p2);
    
                        // check whether normal is != 0
                        denom = norm(n);
                        if (denom > std::numeric_limits<Scalar>::min())
                        {
                            n  *= angle/denom;
                            nn += n;
                        }
                    }
                }
    
                h  = cw_rotated_halfedge(h);
            }
            while (h != hend);
    
            nn.normalize();
        }
    
        return nn;
    }

    /// compute the length of edge \c e.
    Scalar edge_length(Edge e) const
    {
        return norm(vpoint_[vertex(e,0)] - vpoint_[vertex(e,1)]);
    }

    //@}




private: //---------------------------------------------- allocate new elements

    /// allocate a new vertex, resize vertex properties accordingly.
    Vertex new_vertex()
    {
        vprops_.push_back();
        return Vertex(vertices_size()-1);
    }

    /// allocate a new edge, resize edge and halfedge properties accordingly.
    Halfedge new_edge(Vertex start, Vertex end)
    {
        assert(start != end);

        eprops_.push_back();
        hprops_.push_back();
        hprops_.push_back();

        Halfedge h0(halfedges_size()-2);
        Halfedge h1(halfedges_size()-1);

        set_vertex(h0, end);
        set_vertex(h1, start);

        return h0;
    }

    /// allocate a new face, resize face properties accordingly.
    Face new_face()
    {
        fprops_.push_back();
        return Face(faces_size()-1);
    }




private: //--------------------------------------------------- helper functions

    /** make sure that the outgoing halfedge of vertex v is a boundary halfedge
     if v is a boundary vertex. */
    void adjust_outgoing_halfedge(Vertex v)
    {
        Halfedge h  = halfedge(v);
        const Halfedge hh = h;
    
        if (h.is_valid())
        {
            do
            {
                if (is_boundary(h))
                {
                    set_halfedge(v, h);
                    return;
                }
                h = cw_rotated_halfedge(h);
            }
            while (h != hh);
        }
    }

    /// Helper for halfedge collapse
    void remove_edge(Halfedge h)
    {
        Halfedge  hn = next_halfedge(h);
        Halfedge  hp = prev_halfedge(h);
    
        Halfedge  o  = opposite_halfedge(h);
        Halfedge  on = next_halfedge(o);
        Halfedge  op = prev_halfedge(o);
    
        Face      fh = face(h);
        Face      fo = face(o);
    
        Vertex    vh = to_vertex(h);
        Vertex    vo = to_vertex(o);
    
    
    
        // halfedge -> vertex
        Halfedge_around_vertex_circulator vh_it, vh_end;
        vh_it = vh_end = halfedges(vo);
        do
        {
            set_vertex(opposite_halfedge(*vh_it), vh);
        }
        while (++vh_it != vh_end);
    
    
        // halfedge -> halfedge
        set_next_halfedge(hp, hn);
        set_next_halfedge(op, on);
    
    
        // face -> halfedge
        if (fh.is_valid())  set_halfedge(fh, hn);
        if (fo.is_valid())  set_halfedge(fo, on);
    
    
        // vertex -> halfedge
        if (halfedge(vh) == o)  set_halfedge(vh, hn);
        adjust_outgoing_halfedge(vh);
        set_halfedge(vo, Halfedge());
    
    
        // delete stuff
        if (!vdeleted_) vdeleted_ = vertex_property<bool>("v:deleted", false);
        if (!edeleted_) edeleted_ = edge_property<bool>("e:deleted", false);
        vdeleted_[vo]      = true; ++deleted_vertices_;
        edeleted_[edge(h)] = true; ++deleted_edges_;
        garbage_ = true;
    }

    /// Helper for halfedge collapse
    void remove_loop(Halfedge h)
    {
        Halfedge  h0 = h;
        Halfedge  h1 = next_halfedge(h0);
    
        Halfedge  o0 = opposite_halfedge(h0);
        Halfedge  o1 = opposite_halfedge(h1);
    
        Vertex    v0 = to_vertex(h0);
        Vertex    v1 = to_vertex(h1);
    
        Face      fh = face(h0);
        Face      fo = face(o0);
    
    
    
        // is it a loop ?
        assert ((next_halfedge(h1) == h0) && (h1 != o0));
    
    
        // halfedge -> halfedge
        set_next_halfedge(h1, next_halfedge(o0));
        set_next_halfedge(prev_halfedge(o0), h1);
    
    
        // halfedge -> face
        set_face(h1, fo);
    
    
        // vertex -> halfedge
        set_halfedge(v0, h1);  adjust_outgoing_halfedge(v0);
        set_halfedge(v1, o1);  adjust_outgoing_halfedge(v1);
    
    
        // face -> halfedge
        if (fo.is_valid() && halfedge(fo) == o0)
            set_halfedge(fo, h1);
    
    
        // delete stuff
        if (!edeleted_) edeleted_ = edge_property<bool>("e:deleted", false);
        if (!fdeleted_) fdeleted_ = face_property<bool>("f:deleted", false);
        if (fh.is_valid()) { fdeleted_[fh] = true; ++deleted_faces_; }
        edeleted_[edge(h0)] = true; ++deleted_edges_;
        garbage_ = true;
    }

    /// are there deleted vertices, edges or faces?
    bool garbage() const { return garbage_; }



private: //------------------------------------------------------- private data

    friend bool read_poly(Surface_mesh& mesh, const std::string& filename);

    Property_container vprops_;
    Property_container hprops_;
    Property_container eprops_;
    Property_container fprops_;

    Vertex_property<Vertex_connectivity>      vconn_;
    Halfedge_property<Halfedge_connectivity>  hconn_;
    Face_property<Face_connectivity>          fconn_;

    Vertex_property<bool>  vdeleted_;
    Edge_property<bool>    edeleted_;
    Face_property<bool>    fdeleted_;

    Vertex_property<Point>   vpoint_;
    Vertex_property<Normal>  vnormal_;
    Face_property<Normal>    fnormal_;

    unsigned int deleted_vertices_;
    unsigned int deleted_edges_;
    unsigned int deleted_faces_;
    bool garbage_;

    // helper data for add_face()
    typedef std::pair<Halfedge, Halfedge>  NextCacheEntry;
    typedef std::vector<NextCacheEntry>    NextCache;
    std::vector<Vertex>      add_face_vertices_;
    std::vector<Halfedge>    add_face_halfedges_;
    std::vector<bool>        add_face_is_new_;
    std::vector<bool>        add_face_needs_adjust_;
    NextCache                add_face_next_cache_;
};


//------------------------------------------------------------ output operators


inline std::ostream& operator<<(std::ostream& os, Surface_mesh::Vertex v)
{
    return (os << 'v' << v.idx());
}

inline std::ostream& operator<<(std::ostream& os, Surface_mesh::Halfedge h)
{
    return (os << 'h' << h.idx());
}

inline std::ostream& operator<<(std::ostream& os, Surface_mesh::Edge e)
{
    return (os << 'e' << e.idx());
}

inline std::ostream& operator<<(std::ostream& os, Surface_mesh::Face f)
{
    return (os << 'f' << f.idx());
}


//=============================================================================
} // namespace surface_mesh
//=============================================================================
#endif // SURFACE_MESH_H
//=============================================================================
