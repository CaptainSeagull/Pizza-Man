#version 330

uniform mat4 modelViewMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

layout (location = 0) in vec3 in_Position;

void main(void) 
{
	vec4 vertexPosition = modelViewMat * vec4(in_Position, 1.0);

	gl_Position = projectionMat * vertexPosition;
}
