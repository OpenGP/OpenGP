# OpenGeometry
TODO

- **Surface_mesh**: a simple and efficient halfedge-based mesh data structure for representing and processing 2-manifold polygonal surface meshes.

## 1) Compiling / Installing
OpenGeometry uses CMake as its build configuration system. The library can be used both as header-only (slower compile on larger systems, but simpler setup) or as a compiled library (faster to compile, but more complex compile/link setup).

### 1.1) Installing "header-only" Library
The header only library can be either installed locally (within your project) or globally (shared across your projects). The global install is suitable only for UNIX systems.

**Local Install.** Add the "include" subfolder in this project to your project's include path: 

	g++ -I/path/to/include/OpenGeometry main.cpp 

**Global Install.** Simply copy the OpenGeometry folder found in "include" subfolder into your system include subfolder (e.g. /usr/local/include, /opt/local/include, etc...):

	mv ./include/OpenGeometry /usr/local/include 

### 1.2) Installing "compiled" Library
For unix-like system follow these instructions to configure and the library:

	user @ host ~/Developer/OpenGeometry: mkdir build
	user @ host ~/Developer/OpenGeometry: cd build
	user @ host ~/Developer/OpenGeometry/build: cmake ..
	user @ host ~/Developer/OpenGeometry/build: make
	user @ host ~/Developer/OpenGeometry/build: sudo make install

Upon installation, both the library and headers will be installed to the directory given via *CMAKE_INSTALL_PREFIX*, which defaults to /usr/local/ on Unix-like systems. If you need to install to a
custom location set the install prefix during build configuration:

	user @ host ~/Developer/OpenGeometry/build: cmake -DCMAKE_INSTALL_PREFIX=your/custom/path ..


## 2) Examples
TODO 


## 3) Building Documentation
Assume that you have cloned the repositories from GitHub with the structure:

	~/Developer
	  --> OpenGeometry
	  --> OpenGeometry.github.io

To build the documentation, execute the following 

	user @ host ~/Developer/OpenGeometry.github.io: doxygen ../OpenGeometry/doc/Doxyfile 
	Searching for include files...
	Searching for example files...
	Searching for images...
	Searching for dot files...
	... 
 
Then after adding the files to the commit list, (force) push the changes to the github server:

	user @ host ~/Developer/OpenGeometry.github.io: git add --all .
	user @ host ~/Developer/OpenGeometry.github.io: git commit -m "recompiled doc"
	user @ host ~/Developer/OpenGeometry.github.io: git push --force


## 4) Changes
* Release 1.1:
Release date: ????
Initial release of the OpenGeometry library.

* Release 1.0
Release date: November 2013
Initial release of the Surface_mesh library.
