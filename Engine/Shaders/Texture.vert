#version 330

uniform mat4 modelViewMat;
uniform mat4 modelMat;
uniform mat4 projectionMat;

// layout (location = 0) in  vec3 in_Position;
layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 in_TexCoord;

out vec2 out_TexCoord;

void main(void) 
{
	out_TexCoord = in_TexCoord;

	vec4 vertexPosition = modelViewMat * vec4(in_Position, 1.0);

	gl_Position = projectionMat * vertexPosition;
}
