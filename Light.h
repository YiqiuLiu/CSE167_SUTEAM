#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	glm::vec3 position;
	glm::vec3 lightColor;

	Light(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f));
	~Light();

	glm::vec3 getPosition();
	glm::vec3 getColor();

	
};

