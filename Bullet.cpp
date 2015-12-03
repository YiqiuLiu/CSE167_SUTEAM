#include "Bullet.h"


Bullet::Bullet(glm::vec3 position,float angle, Model* model) : Drawable()
{
	this->position = position;
	this->angle = angle;
	this->bulletModel = model;
	this->speed = 1.0;
	//shader = &ResourceManager::GetShader("model");
}


Bullet::~Bullet()
{
}

void Bullet::draw(Shader shader){
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 rotate = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 trans = glm::translate(model, position);
	model = trans*rotate;
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	bulletModel->Draw(shader);
}

void Bullet::update(float dt){
	float pi = glm::pi<float>();
	position += glm::vec3(dt*speed*sinf(angle / 180.0*pi), 0, dt*speed*cosf(angle / 180.0*pi));
}
