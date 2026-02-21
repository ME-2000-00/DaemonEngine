#version 330 core

layout(location = 0) out vec4 out_Color;

in vec3 normal;
flat in int state;

uniform vec3 light;
uniform vec3 colors[3]; // array of 3 colors





vec3 result = vec3(1.0);
void main() {
    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = vec3(ambientStrength);

    // Diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = vec3(diff);

    // Simple color
    result = (ambient + diffuse) * colors[state - 1];

	out_Color = vec4(result, 1.0);
}