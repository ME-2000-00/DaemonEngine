#version 330 core

layout(location = 0) out vec4 out_Color;

uniform vec4 in_color;

void main() {
	out_Color = in_color;
}