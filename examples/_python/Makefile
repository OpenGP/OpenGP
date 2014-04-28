#--- OSX system python (used by /usr/bin/python)
PYTHON=/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
NUMPY=/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy/core/include

#--- Other libraries
OPENGP=../../src
OPENGP_FLAGS=-DUSE_EIGEN -DHEADERONLY
EIGEN=/usr/local/include/eigen3

#--- Sum it up
INCLUDES=-I$(PYTHON) -I$(NUMPY) -I$(OPENGP) -I$(EIGEN)
FLAGS=$(OPENGP_FLAGS)

all: compile demo
	
demo:
	./runme.py
	
compile:
	swig -python -c++ -o generated.cpp opengp.i
	g++ -std=c++11 $(INCLUDES) $(FLAGS) -c generated.cpp
	g++ -std=c++11 -lpython -dynamiclib generated.o -o _opengp.so
	rm -f generated*	
	
clean:
	rm -f mymodule.py mymodule.cpp mymodule.pyc _mymodule.so