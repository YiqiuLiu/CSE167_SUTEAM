//
//  Tank.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/2/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "Tank.h"


Tank::Tank(Model* top, Model*bot)
{
    this->top = top;
    this->bot = bot;
    shader = ResourceManager::GetShader("model");
    position = glm::vec3(0.0, 0.0, 0.0);
    topAngle = 0.0;
	botAngle = 0.0;
	moveSpeed = 3.0;
	spinSpeed = 30.0;
}

Tank::~Tank()
{
    
}

void Tank::draw()
{
    glm::mat4 model = glm::mat4(1.0);
	glm::mat4 rotate = glm::rotate(model, botAngle, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 trans = glm::translate(model, position);
	model = trans*rotate;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    bot->Draw(shader);
    model = glm::mat4(1.0);
    rotate = glm::rotate(model, topAngle, glm::vec3(0.0, 1.0, 0.0));
	trans = glm::translate(model, position);
	model = trans*rotate;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    top->Draw(shader);
}

void Tank::move(GLfloat dt)
{
	float pi = glm::pi<float>();
	float scale = dt*moveSpeed;
	this->position += glm::vec3(scale*sinf(botAngle / 180.0*pi), 0, scale*cosf(botAngle / 180.0*pi));
}
void Tank::spinTop(GLfloat angle)
{
    this->topAngle += angle*spinSpeed;
}

void Tank::spinBot(GLfloat dt){
	this->botAngle += dt*spinSpeed;
}