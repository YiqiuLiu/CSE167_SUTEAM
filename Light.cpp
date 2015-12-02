#include "Light.h"


Light::Light(glm::vec3 position,glm::vec3 lightColor)
{
	this->position = position;
	this->lightColor = lightColor;
}


Light::~Light()
{
}

glm::vec3 Light::getPosition(){
	return position;
}

glm::vec3 Light::getColor(){
	return lightColor;
}
