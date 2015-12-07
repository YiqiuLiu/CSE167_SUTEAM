//
//  Tree.h
//  TankBattle
//
//  Created by Yiqiu Liu on 12/6/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#ifndef Tree_h
#define Tree_h
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Drawable.h"

using namespace std;

class Tree : public Drawable{
public:
    float angle;
    float length;
    int depth;
    vector<string> *trees;
    string str;
    glm::mat4 active;
    glm::vec3 pos;
    vector<glm::mat4> modelStack;
    float lineWidth;
    
    void expand(float num);
    void pop();
    void push();
    void rotL();
    void rotR();
    void draw(Shader);
    void drawLine(Shader);
    void init();
    void leaf(Shader);
    
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    
    GLuint VAO;
    GLuint VBO;
    
    GLuint VAO1;
    GLuint VBO1;
    
    
    Tree(glm::vec3);
    ~Tree();
};

#endif /* Tree_h */
