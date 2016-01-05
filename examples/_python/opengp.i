%module opengp

// Everything in the %{ ... %} block is simply copied 
// verbatim to the resulting wrapper file created by SWIG
%{
    /// Needed for numpy support
    #define SWIG_FILE_WITH_INIT
    #include "SurfaceMesh.h"
%}

/// numpy typemap support
%include "numpy.i"

/// When code is included in the %init section, it is copied directly into the module initialization function.
%init %{
    import_array(); ///< needed for numpy support
%}

/// INPLACE_ARRAY2 essentially means that we are passing a matrix by reference
%apply (float* INPLACE_ARRAY2, int DIM1, int DIM2) {(float* matrix, int rows, int cols)}
/// With ARGOUTVIEWM_ARRAY2 the C++ new memory will be automatically managed 
%apply (float** ARGOUTVIEWM_ARRAY2, int *DIM1, int *DIM2) {(float** matrix, int* rows, int* cols)}

/// And a second include needed here
%include "SurfaceMesh.h"

