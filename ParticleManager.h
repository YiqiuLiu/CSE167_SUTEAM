//
//  ParticalManager.h
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#ifndef ParticleManager_h
#define ParticleManager_h

#include "Drawable.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "Particle.h"

class ParticleManager : public Drawable
{
public:
    glm::vec3 position;
    glm::vec3 velocity;
    GLuint amount;
    ParticleManager(GLuint, glm::vec3, glm::vec3);
    ~ParticleManager();
    
    void draw(Shader);
    void update(GLfloat, GLuint, glm::vec3);
    void init();
    
    std::vector<Particle*> particles;
    
    GLuint firstUnusedParticle();
    void respawnParticle(Particle *particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};



#endif /* ParticleManager_h */
