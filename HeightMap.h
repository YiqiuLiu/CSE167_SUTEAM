//
//  HeightMap.hpp
//  TankBattle
//
//  Created by clyde on 15/12/2.
//  Copyright © 2015年 SUTeam. All rights reserved.
//

#ifndef HeightMap_hpp
#define HeightMap_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <string>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include <vector>
#define BUFFER_OFFSET(i) ((void*)(i))

using namespace std;
struct MyVertex
{
    glm::vec3 vert;        //Vertex
    glm::vec3 norm;     //Normal
    glm::vec2 txtr;         //Texcoord0
};
class HeightMap
{
public:
    unsigned char* LoadPPM(char* sImagePath, int &width,int &height);
    HeightMap(void);
    HeightMap(char* filename);
    void buildMap();
    void buildMap_test();
    void render(Shader);
    GLint TextureFromFile(const char* path, string directory);
    //test
    void InitGeometry();
    void InitVBO();
    void display(Shader shader);

private:
    unsigned char* tdata;
    GLuint VertexVBOID,IndexVBOID,TEXTURE_id;
    int width, height;
    glm::vec3 vRenderScale;
    MyVertex *AttVertex;

};
#endif /* HeightMap_hpp */
