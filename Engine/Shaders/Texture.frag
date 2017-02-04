#version 330

precision highp float;

layout (location = 0) out vec4 outColour;

uniform sampler2D u_textureUnit;

in vec2 out_TexCoord;


void main(void) 
{   
	outColour = texture2D(u_textureUnit, out_TexCoord);
}