// Copyright (C) 2014 - LGG EPFL
#version 330 core 

/// Map data specified in glVertexAttribPointer(...) to shader variables
/// - location maps to the first argument glVertexAttribPointer(position,...)
/// - vec3 because we specified glVertexAttribPointer(..., 3, GL_FLOAT, ...)
/// - position is the name of the variable that you can use in the shader
in vec3 position;

/// The following variables are implicitly defined!!!
/// If you toggle the preprocessor directive the program will still compile/run
/// @see http://www.opengl.org/wiki/Built-in_Variable_(GLSL)
/// @note You could comment this out (built-in)
out gl_PerVertex{ vec4 gl_Position; };

void main() {
    /// gl_Position is a built-in variable that you *must* define
    /// The point location must be converted in homogeneous coordinates
    /// @note Homogenous Coordinates: 
    ///     the vector (x,y,z,1) is a position.
    ///     the vector (x,y,z,0) is a direction.
    gl_Position = vec4(position, 1.0);
    
    /// Alternative way of accessing the vector
    // gl_Position.xyz = position;
    // gl_Position.w = 1.0;
}


