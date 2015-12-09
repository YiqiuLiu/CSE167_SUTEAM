//
//  Tree.cpp
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#include "Tree.h"
#include <GL/glew.h>
#include <iostream>

Tree::Tree(glm::vec3 pos) : Drawable(){
    this->pos = pos;
    this->angle = glm::radians(30.0f);
    this->length = 0.05f;
    this->str = "X";
    this->trees = new vector<string>();
    this->active = glm::mat4(1.0f);
    this->active = glm::translate(active, this->pos);
    this->lineWidth = 10;
    this->depth = 6;
    for(int i = 0;i <= 6;++ i) {
        float num = (float) rand()/RAND_MAX;
        this->expand(num);
    }
    this->init();
    
}

void Tree::init()
{
    glGenVertexArrays(2, &this->VAO);
    glGenBuffers(2, &this->VBO);
    GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.2f, 0.0f, 0.3f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -0.2f, 0.0f, -0.3f,
        0.0f, 1.0f, 0.0f
    };
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0);
    
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);
//    
//    GLfloat g_vertex_buffer_data[] = {
//        0.0f,  2.0f, 0.0f,
//        0.0f,  0.0f, 0.0f,
//        0.0f,  2.0f, 0.01f
//    };
//    
//    glGenBuffers(1, &vertexbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glGenVertexArrays(2, &this->VAO1);
    glGenBuffers(2, &this->VBO1);
        GLfloat g_vertex_buffer_data[] = {
            0.0f,  length, 0.0f,
            0.0f,  0.0f, 0.0f,
            0.0f,  0.0f, 0.2f,
            0.0f,  length, 0.0f,
            0.0f,  length, 0.2f,
            0.0f,  0.0f, 0.2f
        };
    
    glBindVertexArray(VAO1);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0);
    
    
    
}

Tree::~Tree()
{
    delete trees;
}

void  Tree::expand(float num){
    string ch = "";
    
    for (int i = 0; i < str.length(); i++){
        ch = str.at(i);
        
        if (ch.compare("D") == 0){
            str.replace(i, 1, "DD");
            i = i + 1;
        } else if (ch.compare("X") == 0){
            
            if (num < 0.4){
                //LSystem.replace(i, 1, "D[LX]D[RX]LX");
                str.replace(i, 1, "D[LXV]D[RXV]LX");
                
            } else {
                //LSystem.replace(i, 1, "D[RX]D[LX]RX");
                str.replace(i, 1, "D[RXV]D[LXV]RX");
            }
            i = i + 13;	//11
        } 
        
    } 
    trees->push_back(str);
}

void Tree::push()
{
    modelStack.push_back(active);
    if (lineWidth > 0)
        lineWidth -= 1;
}

void Tree::pop()
{
    active = modelStack.back();
    modelStack.pop_back();
    lineWidth += 1;
}

void Tree::rotL()
{
//    modelStack.push_back(active);
    active = glm::rotate(active, angle, glm::vec3(1, 0, 0));
    active = glm::rotate(active, 5 * angle, glm::vec3(0, 1, 0));
    active = glm::rotate(active, angle, glm::vec3(0, 0, 1));
}

void Tree::rotR()
{
//    modelStack.push_back(active);
    active = glm::rotate(active, - angle, glm::vec3(1, 0, 0));
    active = glm::rotate(active, 5 * angle, glm::vec3(0, 1, 0));
    active = glm::rotate(active, - angle, glm::vec3(0, 0, 1));
}

void Tree::draw(Shader shader)
{
    modelStack.clear();
    this->active = glm::mat4(1.0f);
    this->active = glm::translate(active, this->pos);
    string ch = "";
    string LSystem = trees->at(6);
    for (int i = 0; i < LSystem.length(); i++){
        ch = LSystem.at(i);
        
        if (ch.compare("D") == 0 || ch.compare("X") == 0){
            drawLine(shader);
        } else if (ch.compare("[") == 0){
            push();
        } else if (ch.compare("]") == 0){
            pop();
        } else if (ch.compare("V") == 0){
            leaf(shader);
        } else if (ch.compare("R") == 0){
            rotR();
        } else if (ch.compare("L") == 0){
            rotL();
        }
    }
}

void Tree::drawLine(Shader shader)
{
    glm::mat4 model = active;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec4 color(0.55, 0.27, 0.07, 1.0);
    glUniform4fv(glGetUniformLocation(shader.ID, "color_calculate"), 1, glm::value_ptr(color));
    
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    
//    glVertexAttribPointer(
//                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//                          2,                  // size
//                          GL_FLOAT,           // type
//                          GL_FALSE,           // normalized?
//                          0,                  // stride
//                          (void*)0            // array buffer offset
//                          );
//    
//    // Draw the line !
//    glDrawArrays(GL_LINES, 0, 2); // 2 indices for the 2 end points of 1 line
//    
//    
//    glDisableVertexAttribArray(0);
    
//    modelStack.push_back(active);
    active = glm::translate(active, glm::vec3(0,length,0));
}

void Tree::leaf(Shader shader)
{
    glm::mat4 model = active;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec4 color(0.50, 1.0, 0.0, 1.0);
    glUniform4fv(glGetUniformLocation(shader.ID, "color_calculate"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
//    active = modelStack.back();
//    modelStack.pop_back();
}