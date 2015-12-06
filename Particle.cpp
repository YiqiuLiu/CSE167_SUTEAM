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
    this->dieSpeed = 0.05f;
}

Particle::~Particle()
{
    
}

void Particle::draw(Shader shader)
{
    glm::mat4 model;
    model = glm::translate(model, this->position);
    model = glm::scale(model, glm::vec3(3, 3, 3));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec4 color_yellow(1.0, 1.0, 0.0, 1.0);
    glm::vec4 color_red(1.0, 0, 0, 1.0);
    glm::vec4 color = life * color_red + (1 - life) * color_yellow;
    
    glUniform4fv(glGetUniformLocation(shader.ID, "color_calculate"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Particle::update(GLfloat dt)
{
    this->position += dt * moveSpeed * this->velocity;
    this->life -= dt * dieSpeed;
}

void Particle::setup()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    GLfloat vertices[] = {
        0.05f,  0.05f, 0.0f,
        0.0f, 0.05f, 0.05f,
        0.0f,  0.05f, 0.00f
    };
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0);
}