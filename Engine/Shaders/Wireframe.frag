#version 330

precision highp float;

layout (location = 0) out vec4 outColour;

uniform vec3 colour;

void main(void)
{
	outColour = vec4(colour, 1.0f);
}