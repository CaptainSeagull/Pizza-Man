#version 330

precision highp float;

uniform vec3 colour;

layout (location = 0) out vec4 outColour;

void main(void) 
{   
	outColour = vec4(colour, 1.0);
}