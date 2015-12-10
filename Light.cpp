#include "Light.h"


Light::Light(glm::vec3 direction,glm::vec3 lightColor)
{
	this->direction = direction;
	this->lightColor = lightColor;
}


Light::~Light()
{
}

glm::vec3 Light::getDirection(){
	return direction;
}

glm::vec3 Light::getColor(){
	return lightColor;
}
