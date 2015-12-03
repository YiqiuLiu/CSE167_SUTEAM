#pragma once
#include "Drawable.h"
#include "Model.h"
#include <glm/glm.hpp>
#include "Shader.h"
//#include "Resource_manager.h"
class Bullet :
	public Drawable
{
public:
	glm::vec3 position;
	float angle;
	Model* bulletModel;

	int deathCount;
	float speed;

	//Shader* shader;
	Bullet(glm::vec3, float, Model*);
	~Bullet();
	void draw(Shader);
	void update(float dt);
};

