#version 330 core 
uniform mat4 MVP;
uniform mat4 MV;
in vec3 vpoint;
in vec3 vnormal;
out vec3 fpoint;
out vec3 fnormal;

void main() {
    gl_Position = MVP*vec4(vpoint, 1.0);
    fpoint = vec3(MV*vec4(vpoint, 1.0));
    fnormal = inverse(transpose(mat3(MV)))*vnormal;
}
