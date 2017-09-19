#pragma once
#include <OpenGP/headeronly.h>
#include <OpenGP/SurfaceMesh/Algorithm.h>

class SurfaceMeshSubdivideLoop : public OpenGP::SurfaceMeshAlgorithm{
public:
    static HEADERONLY_INLINE void exec(OpenGP::SurfaceMesh& mesh);
};

#ifdef HEADERONLY
    #include "Loop.cpp"
#endif
