#version 330 core

in vec3 oColour;
out vec4 FragColour;

void main() {
	FragColour = vec4(oColour, 1.0f);
}