# OpenGP
OpenGP is a *C++11* open source library for geometry processing. It has been designed and implemented with a focus on ease of use and performance while maintaining high flexibility. It follows the design principles of <a href="https://opensource.cit-ec.de/projects/surface_mesh">Surface_mesh</a>, a mesh data structure developed by the <a href="http://graphics.uni-bielefeld.de/">Bielefeld Graphics & Geometry Group</a> as a simpler to use version of <a href="http://www.openmesh.org/">OpenMesh</a>. The Reference Manual provides detailed information on the classes and functions provided by OpenGP. Code examples can be found in the respective tutorials. Installation instructions are reported below.

# System requirements
OpenGP uses <a href="http://www.cmake.org">CMake</a> as its build system. Version 2.8 or greater is required. OpenGP has been tested to build with the following compilers:

<table>
  <tr><th> Operating System </th> <th> Compiler </th></tr>
  <tr><td> Linux </td> <td> gcc </td> </tr>
  <tr><td> Mac OS-X </td> <td> gcc, clang </td> </tr>
  <tr><td> Windows </td> <td> Visual Studio 2008 </td> </tr>
</table>

Continuous integration with Travis-CI!
https://travis-ci.org/OpenGP/OpenGP

![https://travis-ci.org/OpenGP/OpenGP.svg?branch=master](https://travis-ci.org/OpenGP/OpenGP.svg?branch=master)

# Installing OpenGP - Header Only Option
To install the header-only version of the library you can just copy&paste the OpenGP folder in the "src" subdirectory in your favorite include folder. In MaxOSX you can also just install it using <a href="http://brew.sh/">homebrew</a> as:

    brew install https://raw.githubusercontent.com/OpenGP/OpenGP/master/opengp.rb

In your favorite build system simply define the macro ```OPENGP_HEADERONLY``` to use this method.

# Installing OpenGP - Compiled Library Option 
It is also possible to compile the library into a DLL for larger scale projects. You can checkout a snapshot of the sources as:

	$ ~/Developer: git clone https://github.com/OpenGP/OpenGP.git
	$ ~/Developer: cd OpenGP

To compile the library simply open ```cmake/ConfigureOpenGP.cmake``` and toggle the following line:

    # set(OPENGP_HEADERONLY TRUE)
    set(OPENGP_HEADERONLY FALSE)

After this change simply compile/install with <a href="http://www.cmake.org/">CMake</a>:

	$ ~/Developer/OpenGP: mkdir build
	$ ~/Developer/OpenGP: cd build
	$ ~/Developer/OpenGP/build: cmake .. 
	$ ~/Developer/OpenGP/build: make
	$ ~/Developer/OpenGP/build: sudo make install

To uninstall OpenGP (CURRENTLY BROKEN), simply type:

	$ ~/Developer/OpenGP/build: sudo make uninstall


Compiling the dynamic library, building the examples or building the applications (more involved examples), can be done by commenting/uncommenting items in the main CMakeLists.txt file.

# Documentation 

The documentation for this library is available at <a href="http://opengp.github.io/">http://opengp.github.io/</a>, but you can also generate a local copy by executing the following command (requires doxygen) and opening the file "doc/index.html":

	$ ~/Developer/OpenGP/build: make doxygen
    $ ~/Developer/OpenGP/build: open doc/index.html

To publish the documentation on <a href="http://opengp.github.io/">http://opengp.github.io/</a> (only if you have push access rights):

	$ ~/Developer/OpenGP/build: make publish_doc

## Configuring the library

The configuration procedure can be fine-tuned by specifying flags using the -D option of cmake, for example:

	$ ~/Developer/OpenGP/build: cmake -DCMAKE_BUILD_TYPE=Release ..

The command above would configure CMake to use release mode as its build type and /usr/bin/g++ as its C++ compiler. For additional information on using <code>cmake</code> and
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
  <td>{clang, g++, ...} Specify the compiler to be used (default: g++-5, required: c++11 compliant)</td>
</tr>
<tr>
  <td>CMAKE_CXX_FLAGS</td>
  <td>Specify additional compiler flags, e.g. "-DNDEBUG" disables standard-C assertions</td>
</tr>
</table>

## Changes
- Jan'16: Major refactor for adoption in DGP course OpenGP 1.1
- Feb'14: Initial release of the OpenGP 1.0 library
