//
//  Particle.h
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#ifndef Particle_h
#define Particle_h

#include "Drawable.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Particle : public Drawable
{
public:
    glm::vec3 position;
    glm::vec3 velocity;
    GLfloat life;
    GLuint VAO;
    GLuint VBO;
    
    GLfloat moveSpeed;
    GLfloat dieSpeed;
    
    Particle(glm::vec3, glm::vec3, GLfloat);
    ~Particle();
    
    void setup();
    void draw(Shader);
    void update(GLfloat);
};


#endif /* Particle_h */
