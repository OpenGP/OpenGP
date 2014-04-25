all:
	swig -python -c++ -I../../src -o generated.cpp opengp.i
	g++ -DHEADERONLY -DUSE_EIGEN -I/usr/local/Cellar/eigen/3.2.1/include/eigen3 -I/usr/include/python2.7 -I../../src -c generated.cpp
	g++ -lpython -dynamiclib generated.o -o _opengp.so
	rm -f generated*
	# ./runme.py
	
clean:
	rm -f opengp.py opengp.cpp opengp.pyc _opengp.so