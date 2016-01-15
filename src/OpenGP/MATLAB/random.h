#pragma once
#include <OpenGP/types.h>

//== NAMESPACE ================================================================
namespace OpenGP {
//=============================================================================

/// @see MATLAB randn(1,1)
inline Scalar _randn( const Scalar mean, const Scalar stddev ){
    Scalar u1 = ( static_cast<Scalar> ( std::rand() ) + 1 ) / ( (Scalar)RAND_MAX + 1 );
    Scalar u2 = ( static_cast<Scalar> ( std::rand() ) + 1 ) / ( (Scalar)RAND_MAX + 1 );
    assert( -2 * std::log( u1 ) >= 0 );
    Scalar t1 = std::sqrt( -2 * std::log( u1 ) ) * cos( 2 * M_PI * u2 );
    return mean + stddev * t1;
}

/// Generates matrix whose elements have normal/gaussian distribution
/// @see MATLAB randn function
inline MatMxN randn(int rows, int cols){
    MatMxN zeros(rows, cols);
    for(int i=0; i<rows*cols; i++)
        zeros.data()[i] = _randn(0.0,1.0);
    return zeros;   
}


/// @see MATLAB rand(1,1)
inline Scalar _rand(){
    /// TODO check this one actually gives [0..1]
    return ( static_cast<Scalar> ( std::rand() ) ) / ( (Scalar)RAND_MAX );
}

/// Generates matrix whose elements have uniform distribution in [0,1]
/// @see MATLAB rand function
inline MatMxN rand(int rows, int cols){
    MatMxN zeros(rows, cols);
    for(int i=0; i<rows*cols; i++)
        zeros.data()[i] = _rand();
    return zeros;   
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
