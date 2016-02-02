#version 330 core 
/// These are per-vertex properties
in vec3 vposition; 
in vec3 vnormal;

/// These are fixed for the whole mesh
uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP; 
uniform vec3 LDIR;

/// These are passed to the fragment shader
out vec4 vColor;

void main(){ 
    gl_Position = MVP * vec4(vposition, 1.0); /// Clip coordinates
    
    ///--- Normal in world coordinates
    // @see https://code.google.com/p/opengl-tutorial-org/source/browse/tutorial09_vbo_indexing/StandardShading.vertexshader
    // TODO: Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    vec4 n = M * vec4(vnormal, 0);
    
    ///--- Normalization
    n.xyz = normalize(n.xyz);
    vec3 ldir = normalize(LDIR);
    
    ///--- per vertex shading
    vec3 basecolor = vec3(1,0,0);
    float albedo = max( dot( n.xyz, ldir ), 0 );   
    vColor = vec4(basecolor * albedo,1);
    vColor.a = 1;
}
