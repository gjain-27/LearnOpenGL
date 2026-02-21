#version 330 core

uniform vec3 uColour;
out vec4 FragColour;

void main() {
	FragColour = vec4(uColour, 1.0f);
}