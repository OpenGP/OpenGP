#!/usr/bin/python
import numpy;
from opengp import Surface_mesh;

#-------------------------------------------------------------------------------
print "Read mesh vertices in a numpy matrix"
mesh = Surface_mesh();
mesh.read("../data/tet.obj");
print "Number of vertices", mesh.n_vertices();
mesh.property_stats();
print"\n"

#-------------------------------------------------------------------------------
print "Read mesh vertices in a numpy matrix"
matrix = mesh.get_vertices();
print "print few vertices of matrix"
print "v1:" , matrix[:,0]
print "v2:" , matrix[:,1]
print"\n"

#-------------------------------------------------------------------------------
print "Test a simple write operation"
for vidx in xrange(matrix.shape[1]):
    matrix[:,vidx] = vidx;
mesh.set_vertices(matrix);
print mesh.get_vertices();