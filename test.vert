#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in int aState;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 normal;
flat out int state;

void main() {
    gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
    normal = aNormal;
    state = aState;
}
