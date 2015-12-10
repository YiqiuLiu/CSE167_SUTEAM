//
//  Tank.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/2/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "Tank.h"


Tank::Tank(Model* top, Model*bot, Model* bullet) : Drawable()
{
    this->top = top;
    this->bot = bot;
	this->bulletModel = bullet;
    this->up = glm::vec3(0.0, 1.0, 0.0);
    //shader = ResourceManager::GetShader("model");
    position = glm::vec3(0.0, 10.0, 0.0);
    topAngle = 0.0;
	botAngle = 0.0;
	moveSpeed = 3.0;
	spinSpeed = 0.5;
}

Tank::~Tank()
{
    
}

void Tank::draw(Shader shader)
{
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 scale;
    scale = glm::scale(scale, glm::vec3(0.2,0.2,0.2));
	glm::mat4 rotate = glm::rotate(model, botAngle, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 trans = glm::translate(model, position);
	model = trans*rotate*scale;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    bot->Draw(shader);
    model = glm::mat4(1.0);
    rotate = glm::rotate(model, topAngle, glm::vec3(0.0, 1.0, 0.0));
	trans = glm::translate(model, position);
	model = trans*rotate*scale;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    top->Draw(shader);
}

void Tank::move(GLfloat dt)
{
	float scale = dt*moveSpeed;
	this->position += glm::vec3(scale*sinf(botAngle), 0, scale*cosf(botAngle));
}
void Tank::spinTop(GLfloat angle)
{
    this->topAngle += angle*spinSpeed;
}

void Tank::spinBot(GLfloat dt){
	this->botAngle += dt*spinSpeed;
}

Bullet* Tank::fire(){
	Bullet* bullet = new Bullet(this->position,this->topAngle,this->bulletModel);
	return bullet;
}

void Tank::setPosition(glm::vec3 newPos){
	this->position = newPos;
}