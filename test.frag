#version 330 core

layout(location = 0) out vec4 out_Color;

in vec3 normal;

uniform vec3 light;
uniform vec3 tint;




void main() {
    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = vec3(ambientStrength);

    // Diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = vec3(diff);

    // Simple color
    vec3 result = (ambient + diffuse) * tint;


	out_Color = vec4(result, 1.0);
}