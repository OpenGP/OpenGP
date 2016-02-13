#pragma once
#include <OpenGP/headeronly.h>
#include <OpenGP/SurfaceMesh/Algorithm.h>

class SurfaceMeshSubdivideLoop : public OpenGP::SurfaceMeshAlgorithm{
public:
    static HEADERONLY_INLINE void exec(SurfaceMesh& mesh);
};

#ifdef HEADERONLY
    #include "Loop.cpp"
#endif
