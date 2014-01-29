# OpenGeometry
OpenGeometry is a collection of ... composed by:

- **Surface_mesh**: a simple and efficient halfedge-based mesh data structure for representing and processing 2-manifold polygonal surface meshes.

## Installing
See the INSTALL file for a quick guide on how to build and
install Surface_mesh.

## Changes
See the CHANGES file for a concise overview about recent changes
to Surface_mesh.


## Building Documentation
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
	user @ host ~/Developer/OpenGeometry.github.io: git push --force

 