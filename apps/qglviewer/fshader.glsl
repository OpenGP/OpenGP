#version 330 core
in vec3 fpoint; 
in vec3 fnormal;
out vec3 fcolor;

/// Shade red front facing, solid blue back
void main(){
    if(fnormal.z>0)
        fcolor = fnormal.z * vec3(1.0,0.0,0.0);
    else
        fcolor = vec3(0,0,1);
}
