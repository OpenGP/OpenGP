# OpenGeometry
OpenGeometry is a *C++11* open source library for geometry processing. It has been designed and implemented with a focus on ease of use and performance while maintaining high flexibility. It follows the design principles of <a href="https://opensource.cit-ec.de/projects/surface_mesh">Surface_mesh</a>, a mesh data structure developed by the <a href="http://graphics.uni-bielefeld.de/">Bielefeld Graphics & Geometry Group</a> as a simpler to use version of <a href="http://www.openmesh.org/">OpenMesh</a>. The Reference Manual provides detailed information on the classes and functions provided by OpenGeometry. Code examples can be found in the respective tutorials. Installation instructions are reported below.

# System requirements
OpenGeometry uses <a href="http://www.cmake.org">CMake</a> as its build system. Version 2.8 or greater is required. OpenGeometry has been tested to build with the following compilers:

<table>
  <tr><th> Operating System </th> <th> Compiler </th></tr>
  <tr><td> Linux </td> <td> gcc </td> </tr>
  <tr><td> Mac OS-X </td> <td> gcc, clang </td> </tr>
  <tr><td> Windows </td> <td> Visual Studio 2008 </td> </tr>
</table>

# Installing OpenGeometry
The latest release is available from the GitHub at https://github.com/OpenGeometry. You can checkout a snapshot of the sources, compile them with <a href="http://www.cmake.org/">CMake</a>, and install them as follows:

	$ ~/Developer: git clone https://github.com/OpenGeometry/OpenGeometry.git
	$ ~/Developer: cd OpenGeometry && mkdir build && cd build
	$ ~/Developer/OpenGeometry/build: cmake ..
	$ ~/Developer/OpenGeometry/build: make
	$ ~/Developer/OpenGeometry/build: sudo make install

The documentation for this library is available at <a href="http://opengeometry.github.io/">http://opengeometry.github.io/</a>, but you can also generate a local copy by executing the following command and opening the file "build/index.html":

	$ ~/Developer/OpenGeometry/build: make doxygen

## Configuring the library

The configuration procedure can be fine-tuned by specifying flags using the -D option of cmake, for example:

	$ ~/Developer/OpenGeometry/build: cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/g++

The command above would configure CMake to use release mode as its build type and /usr/bin/g++ as its C++ compiler. To build the examples, configure CMake with:

	$ ~/Developer/OpenGeometry/build: cmake -DWITH_EXAMPLES=true ..

These are a few useful flags, but for additional information on using <code>cmake</code> and
customizing its configuration see the <a href="http://cmake.org/cmake/help/documentation.html">CMake
documentation</a>.

<table>
<tr><th>Flag</th><th>Description</th></tr>
<tr>
  <td>CMAKE_BUILD_TYPE</td>
  <td>{DEBUG | RELEASE} Specify the build type</td>
</tr>
<tr>
  <td>CMAKE_CXX_COMPILER</td>
  <td>{clang, g++, ...} Specify the compiler to be used</td>
</tr>
<tr>
  <td>CMAKE_CXX_FLAGS</td>
  <td>Specify additional compiler flags, e.g. "-DNDEBUG" disables standard-C assertions</td>
</tr>
</table>

## Changes
* Release 1.0:
Release date: February 2014
Initial release of the OpenGeometry library.
