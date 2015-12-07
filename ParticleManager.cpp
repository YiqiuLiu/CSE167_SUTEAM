//
//  ParticalManager.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "ParticleManager.h"

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
        glm::vec3 random(0, 0, ((rand() % 100) - 50) / 100.0f);
        GLfloat random1 = (rand() % 100) - 50 / 100.0f;
        this->particles.push_back(new Particle(this->position + random, random1 * this->velocity, 1.0f));
    }
}

ParticleManager::~ParticleManager()
{
    for(auto p : this->particles) {
        delete p;
    }
}

void ParticleManager::draw(Shader shader)
{
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
    for (GLuint i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], this->position);
    }
    for(auto p : this->particles)
    {
        if(p->life > 0.0f)
        {
            p->update(dt);
        }
        p->update(dt);
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
    glm::vec3 random(((rand() % 100) - 50) / 100.0f, ((rand() % 100) - 50) / 100.0f, ((rand() % 100) - 50) / 100.0f);
    particle->position = particle->position + offset + random;
    particle->life = 1.0f;
    particle->velocity *= 0.1f;
}
