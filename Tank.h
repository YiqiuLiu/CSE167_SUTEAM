//
//  Tank.h
//  TankBattle
//
//  Created by Yiqiu Liu on 12/2/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//


#ifndef Tank_h
#define Tank_h

#include "Drawable.h"
#include "Model.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "Bullet.h"
//#include "Resource_manager.h"

class Tank: public Drawable{
public:
	Tank(Model* top, Model*bot,Model* bullet);
    ~Tank();
    glm::mat4 world;
	glm::mat4 selfRotate;
    glm::vec3 position;
	float topAngle;
	float botAngle;

    void draw(Shader);
    void move(float dt);
    void spinTop(GLfloat angle);
	void spinBot(GLfloat dt);
	void setPosition(glm::vec3);
	Bullet* fire();
private:
    Model* bot;
    Model* top;
	Model* bulletModel;
    //Shader shader;
    glm::mat4 topWorld;
	float moveSpeed;
	float spinSpeed;
    glm::mat4 botWorld;
    
};


#endif /* Tank_h */
