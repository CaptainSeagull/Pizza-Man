#include "Material.h"
#include <glm\gtc\type_ptr.hpp>
using namespace glm;

Material::Material() : Material(vec4(0.0f), vec4(0.0f), vec4(0.0f), 0.0f)
{
}

Material::Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

void Material::setUniforms(GLuint shader)
{
	glUseProgram(shader);

	glUniform4fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(ambient));
	glUniform4fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform4fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), shininess);
}

Material::~Material()
{
}
