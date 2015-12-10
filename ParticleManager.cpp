//
//  ParticalManager.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "ParticleManager.h"
#include <vector>

ParticleManager::ParticleManager(GLuint amount, glm::vec3 position, glm::vec3 velocity) :Drawable()
{
    this->amount = amount;
    this->position = position;
    this->velocity = velocity;
    this->init();
}

void ParticleManager::init()
{
    for(int i = 0;i < this->amount;++ i)
    {
        int level = i % 100;
        GLfloat a = (rand() % 100) / 100.0f;
        GLfloat b = sqrt(1 - a * a);
        GLfloat x = level / 50.0f * (rand() % 100 - 50) / 50.0f * a;
        GLfloat z = level / 50.0f * (rand() % 100 - 50) / 50.0f * b;
        glm::vec3 random(x, level / 10.0f +  a, z);

        this->particles.push_back(new Particle(this->position + random, this->velocity, 1.0f - (level + 1) / 100.0f));
    }
    
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    GLfloat vertices[] = {
        10.0f,  10.0f, 0.0f,
        0.0f, 10.0f, 10.0f,
        0.00f,  10.0f, 0.00f
    };
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0);
}

ParticleManager::~ParticleManager()
{
    for(auto p : this->particles) {
        delete p;
    }
}

void ParticleManager::draw(Shader shader)
{
    glm::mat4 model;
    model = glm::translate(model, this->position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec4 color(1.0, 0.0, 1.0, 1.0);
    
    glUniform4fv(glGetUniformLocation(shader.ID, "color_calculate"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(auto p : this->particles)
    {
        if(p->life > 0.0f)
        {
            p->draw(shader);
        }
    }
}

void ParticleManager::update(GLfloat dt, GLuint newParticles, glm::vec3 offset)
{
    this->position += dt * this->velocity;
//    for (GLuint i = 0; i < newParticles; ++i)
//    {
//        int unusedParticle = this->firstUnusedParticle();
//        this->respawnParticle(this->particles[unusedParticle], this->position);
//    }
    for(auto p : this->particles)
    {
        if(p->life > 0.0f)
        {
            p->update(dt);
        }
    }
}

GLuint lastUsedParticle = 0;
GLuint ParticleManager::firstUnusedParticle(){
    for (GLuint i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i]->life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i]->life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleManager::respawnParticle(Particle *particle, glm::vec3 offset)
{
    particle->position =  offset + glm::vec3(0, 5, 0);
    particle->life = 1.0f;
    particle->velocity = this->velocity;
}
