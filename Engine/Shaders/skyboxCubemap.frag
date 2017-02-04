// cubeMap fragment shader
#version 330

precision highp float;

layout (location = 0) out vec4 outColour;
in vec3 cubeTexCoord;

uniform samplerCube cubeMap;

uniform struct FogParameters
{
	vec4 colour;
	float density, startHeight;
}fogParams;

in vec4 eyePos, worldPos;

float getFogFactor();
 
void main(void) 
{   
	outColour = texture(cubeMap, cubeTexCoord);

	//fog stuffs
	float fogCoord = abs(eyePos.z / eyePos.w);
	outColour = mix(outColour, fogParams.colour, getFogFactor());
}

float getFogFactor()
{
	float result = 1.0;

	//if(worldPos.y < fogParams.startHeight)
		result = exp(-fogParams.density*((-worldPos.y - fogParams.startHeight)/worldPos.w));

	return 1.0 - clamp(result, 0.0, 1.0);
}