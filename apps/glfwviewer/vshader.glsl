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
    /// TODO: inverse transpose
    vec4 n = M * vec4(vnormal, 0);
    
    ///--- Normalizations
    n.xyz = normalize(n.xyz);
    vec3 ldir = normalize(LDIR);
    
    ///--- (Gouraud) per-vertex shading 
    vec3 basecolor = vec3(1,0,0);
    float albedo = max( dot( n.xyz, ldir ), 0 );   
    vColor = vec4(basecolor * albedo,1);
    vColor.a = 1;
}
