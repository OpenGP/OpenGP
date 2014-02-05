#version 330 core 
/// @todo see Pg 238 Angel for simpler version than layout

/// These are per-vertex properties
layout(location = 0) in vec3 VPOS; 
layout(location = 1) in vec3 VNOR;

/// These are fixed for the whole mesh
uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP; 
uniform vec3 LDIR;

/// These are passed to the fragment shader
out vec4 vColor;

void main(){ 
    gl_Position = MVP * vec4(VPOS, 1.0); /// Clip coordinates
    
    ///--- Normal in world coordinates
    // @see https://code.google.com/p/opengl-tutorial-org/source/browse/tutorial09_vbo_indexing/StandardShading.vertexshader
    // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    vec4 n = M * vec4(VNOR, 0);
    
    ///--- Normalization
    n.xyz = normalize(n.xyz);
    vec3 ldir = normalize(LDIR);
    
    ///--- per vertex shading
    vec3 basecolor = vec3(1,0,0);
    float albedo = max( dot( n.xyz, ldir ), 0 );   
    vColor = vec4(basecolor * albedo,1);
    vColor.a = 1;
}
