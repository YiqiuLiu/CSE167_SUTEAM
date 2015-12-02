//
//  Tank.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/2/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "Tank.h"


Tank::Tank()
{
    top = Model("./obj/tank_top_no_texture.obj");
    bot = Model("./obj/tank_bottm_no_texture.obj");
    shader = ResourceManager::GetShader("model");
    position = glm::vec3(0.0, 0.0, 0.0);
    topAngle = 90.0;
}

Tank::~Tank()
{
    
}

void Tank::draw()
{
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    bot.Draw(shader);
    model = glm::mat4(1.0);
    model = glm::rotate(model, topAngle, glm::vec3(0.0, 1.0, 0.0));
    model = glm::translate(model, position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    top.Draw(shader);
}