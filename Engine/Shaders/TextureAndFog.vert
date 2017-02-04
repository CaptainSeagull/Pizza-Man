#version 330

layout (shared) uniform MatrixBlock
{
	mat4 modelMat;
	mat4 viewMat;
	mat4 modelViewMat;
	mat4 projectionMat;
	mat4 mvpMat;
	mat3 normalMat;
	vec3 eyePos;
};

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;

out VS_OUT
{
	vec4 eyePos; 
	vec4 worldPos;
	vec3 vertexPosition;
	vec3 vertexToEye;
	vec3 normalVec;
	vec2 texCoord;
} vs_out;

void main(void) 
{
	vec4 vertexPosition = modelViewMat * vec4(in_Position, 1.0);

	gl_Position = projectionMat * vertexPosition;

	vs_out.eyePos = vertexPosition;
	vs_out.worldPos = modelMat * vec4(in_Position, 1.0);
	vs_out.vertexPosition = (modelMat * vec4(in_Position, 1.0)).xyz;
	vs_out.vertexToEye = normalize(eyePos - vs_out.vertexPosition);
	vs_out.normalVec = normalize(normalMat * in_Normal);
	vs_out.texCoord = in_TexCoord;
}
