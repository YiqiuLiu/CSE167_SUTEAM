//
//  HeightMap.hpp
//  TankBattle
//
//  Created by clyde on 15/12/2.
//  Copyright © 2015年 SUTeam. All rights reserved.
//

#ifndef HeightMap_hpp
#define HeightMap_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <assimp/types.h>
#include "Mesh.h"
#include "Shader.h"



class HeightMap {
public:
    struct Vertex {
        // Position
        glm::vec3 Position;
        // Normal
        glm::vec3 Normal;
        // TexCoords
        glm::vec2 TexCoords;
    };
    
    
    /*  HeightMap Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    int width,height;
    float scaleHeight;
    float fTextureU,fTextureV,mapScale;
    /*  Functions  */
    // Constructor
    HeightMap(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    
    unsigned char* LoadPPM(char* sImagePath, int &width,int &height);
    HeightMap(void);
    HeightMap(char* filename);
    float getHeight(float, float);
    glm::vec3 getNormal(float x, float z);

    void buildMap();
    // Render the HeightMap
    void Draw(Shader shader);
    
private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;
    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupHeightMap();
    unsigned char* tdata;
    
};


#endif /* HeightMap_hpp */
