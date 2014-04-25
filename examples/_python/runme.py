#!/usr/bin/python

from opengp import Surface_mesh;
import opengp;

s = Surface_mesh();
s.read("../data/bunny.obj");
s.property_stats();
print "Number of vertices", s.n_vertices();

# to convert a Eigen::matrix to something like [[1,2,3],[4,5,6]];
# see this stuff: http://www.swig.org/Doc1.1/HTML/Typemaps.html

# to convert a Eigen::matrix into a numpy.matrix
# possibly see this stuff: https://github.com/jorisv/Eigen3ToPython