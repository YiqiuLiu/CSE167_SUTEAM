#include "Bullet.h"


Bullet::Bullet(glm::vec3 position,float angle, Model* model) : Drawable()
{
	this->position = position;
	this->angle = angle;
	this->bulletModel = model;
	this->speed = 1.0;
	this->deathCount = 100;
	//shader = &ResourceManager::GetShader("model");
}


Bullet::~Bullet()
{
}

void Bullet::draw(Shader shader){
	glm::mat4 model = glm::mat4(1.0);
    glm::mat4 scale;
    scale = glm::scale(scale, glm::vec3(0.5,0.5,0.5));
	glm::mat4 rotate = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 trans = glm::translate(model, position);
	model = trans*rotate*scale;
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	bulletModel->Draw(shader);
}

void Bullet::update(float dt){
#ifdef __APPLE__
	position += glm::vec3(dt*speed*sinf(angle), 0, dt*speed*cosf(angle));
#else
	position += glm::vec3(dt*speed*sinf(glm::radians(angle)), 0, dt*speed*cosf(glm::radians(angle)));
#endif
//    std::cout<<"angle"<<angle<<std::endl;
	deathCount--;
	if (deathCount <= 0){
		state = DEAD;
	}
}
