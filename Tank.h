//
//  Tank.h
//  TankBattle
//
//  Created by Yiqiu Liu on 12/2/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//


#ifndef Tank_h
#define Tank_h

#include "Model.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "Resource_manager.h"

class Tank{
public:
    Tank();
    ~Tank();
    glm::mat4 world;
    glm::vec3 position;

    void draw();
    void move(float dt);
    void spinTop(GLfloat angle);
	void spinBot(GLfloat dt);
private:
    Model bot;
    Model top;
    Shader shader;
    glm::mat4 topWorld;
    float topAngle;
	float botAngle;
	float moveSpeed;
	float spinSpeed;
    glm::mat4 botWorld;
    
};


#endif /* Tank_h */
