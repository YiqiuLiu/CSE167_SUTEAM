//
//  Particle.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "Particle.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Particle::Particle(glm::vec3 pos, glm::vec3 vel, GLfloat li) : Drawable()
{
    this->position = pos;
    this->velocity = vel;
    this->life = li;
    this->setup();
    this->moveSpeed = 0.1f;
    this->dieSpeed = 0.1f;
}

Particle::~Particle()
{
    
}

void Particle::draw(Shader shader)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glm::mat4 model;
    glm::mat4 model1;
    model = glm::translate(model, this->position);
    model1 = glm::scale(model1, glm::vec3(3, 3, 3));
    model = model * model1;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec3 color_yellow(1.0, 1.0, 0.0);
    glm::vec3 color_red(1.0, 0, 0);
    glm::vec4 color = glm::vec4(life * color_red + (1 - life) * color_yellow, life);
    
    glUniform4fv(glGetUniformLocation(shader.ID, "color_calculate"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Particle::update(GLfloat dt)
{
    this->position += dt *  this->velocity;
    this->life -= dt * dieSpeed;
}

void Particle::setup()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    GLfloat vertices[] = {
        0.02f,  0.02f, 0.0f,
        0.0f, 0.02f, 0.02f,
        0.00f,  0.02f, 0.00f
    };
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0);
}