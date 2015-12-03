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
#include <string>
#include <iostream>
#include <fstream>
#define BUFFER_OFFSET(i) ((void*)(i))

using namespace std;

class HeightMap
{
public:
    unsigned char* LoadPPM(char* sImagePath, int &width,int &height);
    HeightMap(void);
    HeightMap(char* filename);
    void buildMap();
private:
    unsigned char* tdata;
    GLuint VertexVBOID;
    int width, height;
    glm::vec3 vRenderScale;
    
    void render();
    
};
#endif /* HeightMap_hpp */
