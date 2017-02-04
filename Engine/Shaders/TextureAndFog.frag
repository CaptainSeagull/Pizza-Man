#version 330

precision highp float;

const int maxLights = 102;

layout (shared) uniform LightBlock
{
	int numLights;
	vec4 position[maxLights];
	vec4 direction[maxLights];
	vec4 ambient;
	vec4 diffuse[maxLights];
	vec4 specular[maxLights];
	vec3 attenuation[maxLights];
	float cosCutoffAngle[maxLights];
	float secondaryCutoffAngle[maxLights];
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

vec3 lightToVert;
vec3 reflection;
float attenuationValue;
float lightToVertDistance;

in VS_OUT
{
	vec4 eyePos; 
	vec4 worldPos;
	vec3 vertexPosition;
	vec3 vertexToEye;
	vec3 normalVec;
	vec2 texCoord;
} fs_in;

layout (location = 0) out vec4 outColour;

uniform sampler2D u_textureUnit;
uniform materialStruct material;


uniform struct FogParameters
{
	vec4 colour;
	float density, startHeight;
}fogParams;

float getFogFactor();
vec4 calculateLight(int i);
vec4 calculateLightColour(int i);

void main(void) 
{   
	vec4 lightColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	for(int i=0; i<numLights; i++)
	{
		lightColour += calculateLight(i);
	}
	outColour = lightColour * texture2D(u_textureUnit, fs_in.texCoord);

	outColour = mix(outColour, fogParams.colour, getFogFactor());
}

float getFogFactor()
{
	float result = 1.0;

	//if(worldPos.y < fogParams.startHeight)
		result = exp(-fogParams.density*((-fs_in.worldPos.y - fogParams.startHeight)/fs_in.worldPos.w));

	return 1.0 - clamp(result, 0.0, 1.0);
}

vec4 calculateLight(int i)
{
		//Calculate the vector from the vertex position to the light source in world coordinates. 
	lightToVert = fs_in.vertexPosition - position[i].xyz;
		//Calculate the distance of the above vector. We do this before normalising or the length will always be one. 
	lightToVertDistance = length(lightToVert);
		//Normalise lightVec to get the direction from the vertex to the light source.
	lightToVert = normalize(lightToVert);

		/*Calculate reflection light makes to the surface*/
	reflection = reflect(lightToVert, fs_in.normalVec);

	if(cosCutoffAngle[i] == 0 && direction[i] == vec4(0.0, 0.0, 0.0, 0.0))
	{
		return calculateLightColour(i);
	}
	else
	{
			//Get dot product of spotlight direction and vertex vector. 
		float dotProduct = dot(lightToVert, normalize(direction[i].xyz));

		if(dotProduct > cosCutoffAngle[i])
		{
			return calculateLightColour(i);
		}
		else if(dotProduct > secondaryCutoffAngle[i])
		{
			return vec4(calculateLightColour(i).xyz*((dotProduct - secondaryCutoffAngle[i])/(cosCutoffAngle[i] - secondaryCutoffAngle[i])), 1.0);
		}
		else
			return vec4(0.0, 0.0, 0.0, 1.0);
	}
}

vec4 calculateLightColour(int i)
{
	float attenuationValue = 1/(attenuation[i].x + attenuation[i].y*lightToVertDistance + attenuation[i].z*lightToVertDistance*lightToVertDistance);
	
		//Calculate colour of light and object material.
		//Ambient intensity
	vec4 ambientI = ambient * material.ambient;

		// Diffuse intensity
	vec4 diffuseI =  diffuse[i] * material.diffuse;
	diffuseI = diffuseI * max(dot(fs_in.normalVec, -lightToVert), 0.0) * attenuationValue;

		// Specular intensity
	vec4 specularI = specular[i] * material.specular;
		//Specular Intensity = Ks*(R.V)^(alpha)*Is
	specularI = specularI * pow( max( dot( reflection, fs_in.vertexToEye ), 0 ), material.shininess ) * attenuationValue;

	return ambientI + diffuseI + specularI;
}