#version 330 core
in vec3 fnormal; ///< in camera coordinates!
out vec3 fcolor; 

/// assumption: light at camera position (0,0,0)
void main(){
    /// Simple phong lighting model
    fcolor = (max(fnormal.z,0) * vec3(1.0,0.0,0.0));
    
    /// Normal-map like rendering
    // fcolor = fnormal;
}
