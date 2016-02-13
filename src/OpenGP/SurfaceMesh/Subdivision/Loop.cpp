#include "Loop.h"
#include <OpenGP/MLogger.h>

void SurfaceMeshSubdivideLoop::exec(SurfaceMesh& mesh){
    /// TODO: other pre-conditions?
    CHECK(mesh.is_triangle_mesh());
    
    // reserve memory
    int nv = mesh.n_vertices();
    int ne = mesh.n_edges();
    int nf = mesh.n_faces();
    mesh.reserve(nv+ne, 2*ne+3*nf, 4*nf);

    // get properties
    VertexProperty<Point> points = mesh.vertex_property<Point>("v:point");
    VertexProperty<Point> vpoint = mesh.add_vertex_property<Point>("loop:vpoint");
    EdgeProperty<Point>   epoint = mesh.add_edge_property<Point>("loop:epoint");
    VertexProperty<bool>  vfeature = mesh.get_vertex_property<bool>("v:feature");
    EdgeProperty<bool>    efeature = mesh.get_edge_property<bool>("e:feature");

    // compute vertex positions
    for(Vertex v: mesh.vertices()){        
        if ( /*isolated vertex?*/ mesh.is_isolated(v)){
            vpoint[v] = points[v];
        }
        else if (/*boundary vertex?*/ mesh.is_boundary(v) ) {
            Halfedge h1 = mesh.halfedge(v);
            Halfedge h0 = mesh.prev_halfedge(h1);
            Point p = points[v];
            p *= 6.0;
            p += points[mesh.to_vertex(h1)];
            p += points[mesh.from_vertex(h0)];
            p *= 0.125;
            vpoint[v] = p;
        }

        // interior feature vertex?
        else if (vfeature && vfeature[v]) {
            Point p = points[v];
            p *= 6.0;
            int count = 0;

            for(Halfedge vh: mesh.halfedges(v)){
                if (efeature[mesh.edge(vh)]) {
                    p += points[mesh.to_vertex(vh)];
                    ++count;
                }
            }

            if (count == 2) { // vertex is on feature edge
                p *= 0.125;
                vpoint[v] = p;
            } else { // keep fixed
                vpoint[v] = points[v];
            }
        }

        // interior vertex
        else {
            Point p = Point::Zero();
            Scalar  inv_k = 1.0 / mesh.valence(v);
            for(Vertex vvit: mesh.vertices(v))
                p += inv_k * points[vvit];
            Scalar beta = (0.625 - pow(0.375 + 0.25*cos(2.0*M_PI*inv_k), 2.0));

            vpoint[v] = points[v]*(Scalar)(1.0-beta) + beta*p;
        }
    }

    // compute edge positions
    for(Edge e: mesh.edges()){
        if ( /*boundary or feature edge?*/ mesh.is_boundary(e) || (efeature && efeature[e])) {
            epoint[e] = (points[mesh.vertex(e,0)] + points[mesh.vertex(e,1)]) * Scalar(0.5);
        }
        else /*interior edge*/ {
            Halfedge h0 = mesh.halfedge(e, 0);
            Halfedge h1 = mesh.halfedge(e, 1);
            Point p = points[mesh.to_vertex(h0)];
            p += points[mesh.to_vertex(h1)];
            p *= 3.0;
            p += points[mesh.to_vertex(mesh.next_halfedge(h0))];
            p += points[mesh.to_vertex(mesh.next_halfedge(h1))];
            p *= 0.125;
            epoint[e] = p;
        }
    }

    // set new vertex positions
    for(Vertex v: mesh.vertices())
        points[v] = vpoint[v];

    // inserts new vertices on edges
    for(Edge e: mesh.edges()){
        // feature edge?
        if (efeature && efeature[e]) {
            Halfedge e_v = mesh.insert_vertex(e, epoint[e]);
            Vertex v = mesh.to_vertex(e_v);
            Edge   e = *(--mesh.edges_end());
            vfeature[v] = true;
            efeature[e] = true;
        }

        // normal edge
        else {
            mesh.insert_vertex(e, epoint[e]);
        }
    }

    // split faces
    for(Face f: mesh.faces()){
        Halfedge h = mesh.halfedge(f);
        mesh.insert_edge(h, mesh.next_halfedge(mesh.next_halfedge(h)));
        h = mesh.next_halfedge(h);
        mesh.insert_edge(h, mesh.next_halfedge(mesh.next_halfedge(h)));
        h = mesh.next_halfedge(h);
        mesh.insert_edge(h, mesh.next_halfedge(mesh.next_halfedge(h)));
    }

    // clean-up properties
    mesh.remove_vertex_property(vpoint);
    mesh.remove_edge_property(epoint);
}

