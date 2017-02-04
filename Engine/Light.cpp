#include "Light.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;
using namespace std;

Light::Light() : Light(vec4(0.0f))
{

}

Light::Light(glm::vec4 position) : position(position)
{
}

Light::~Light()
{
}
