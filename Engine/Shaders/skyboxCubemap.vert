#version 330

uniform mat4 modelViewMat;
uniform mat4 projectionMat;
uniform mat4 modelMat;

layout (location = 0) in  vec3 in_Position;
out vec3 cubeTexCoord;
out vec4 eyePos, worldPos;

void main(void) 
{
	vec4 vertexPosition = modelViewMat * vec4(in_Position,1.0);

	gl_Position = projectionMat * vertexPosition;

	cubeTexCoord = normalize(in_Position);
	eyePos = vertexPosition;
	worldPos = modelMat * vec4(in_Position,1.0);
}
