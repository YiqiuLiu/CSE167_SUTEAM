//
//  HeightMap.cpp
//  TankBattle
//
//  Created by clyde on 15/12/2.
//  Copyright © 2015年 SUTeam. All rights reserved.
//

#include "HeightMap.h"

HeightMap::HeightMap()
{
    width = 0;
    height = 0;
    tdata = nullptr;
    //
}

HeightMap::HeightMap(char* filename)
{
    width = 0;
    height = 0;
    tdata = LoadPPM(filename, width, height);
    if(tdata == NULL || width == 0 || height == 0 )
        std::cout<< "error reading ppm file, no data retrieved. " << std::endl;
    mapScale = 1000.0;
    scaleHeight = 30.0;
    fTextureU = float(width)*0.1f;
    fTextureV = float(height)*0.1f;
    
    buildMap();
}

void HeightMap::buildMap()
{
    glDisable(GL_BLEND);
    
    
    

    float RED=0.3, GREEN=0.59, BLUE=0.11;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float fScaleC = float(j)/float(width-1);
            float fScaleR = float(i)/float(height-1);
            float fVertexHeight = float(int(tdata[(i*width+j)*3])*RED +
                                        int(tdata[(i*width+j)*3+1])*GREEN +
                                        int(tdata[(i*width+j)*3+2])*BLUE)/255.0f;
            Vertex vtx;
            vtx.Position = glm::vec3(mapScale*(-0.5f+fScaleC), scaleHeight * fVertexHeight, mapScale*(-0.5f+fScaleR));
            vtx.TexCoords = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
            vertices.push_back(vtx);
            glm::vec3 prt = vtx.Position;
           
            if (j==3 && i == 1 && prt.y < 1)
                std::cout << "{"
                << prt.x << " " << prt.y << " " << prt.z
                << "}"<<endl;
        };
    }
    // normal calculation
    vector< vector<glm::vec3> > vNormals[2];
    for(int i = 0; i < 2; i++) vNormals[i] = vector< vector<glm::vec3> >(height-1, vector<glm::vec3>(width-1));
    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            glm::vec3 vTriangle0[] =
            {
                vertices[i*width + j].Position,
                vertices[(i+1)*width + j].Position,
                vertices[(i+1)*width + j+1].Position
            };
            glm::vec3 vTriangle1[] =
            {
                vertices[(i+1)*width + j+1].Position,
                vertices[i*width + j+1].Position,
                vertices[i*width + j].Position
            };
            
            glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]);
            glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]);
            
            vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
            vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);
            
            // Look for upper-left triangles
            if(j != 0 && i != 0)
                for(int k = 0; k < 2; k++)vFinalNormal += vNormals[k][i-1][j-1];
            // Look for upper-right triangles
            if(i != 0 && j != width-1) vFinalNormal += vNormals[0][i-1][j];
            // Look for bottom-right triangles
            if(i != height-1 && j != height-1)
                for(int k = 0; k < 2; k++)vFinalNormal += vNormals[k][i][j];
            // Look for bottom-left triangles
            if(i != height-1 && j != 0)
                vFinalNormal += vNormals[1][i][j-1];
            vFinalNormal = glm::normalize(vFinalNormal);
            
            vertices[i*width + j].Normal = vFinalNormal; // Store final normal of j-th vertex in i-th row
        }
    }
    
    
    GLuint cS,     // Current square.
    sX, sZ, // Positon of current square.
    tl,     // Vertex index: Top left.
    tr,     // Vertex index: Top right.
    bl,     // Vertex index: Bottom left.
    br;     // Vertex index: Bottom right.
    
    // Build triangles for each square on the terrain plane.
    for (cS = 0; cS < width * height; cS += 1) {
        
        // Calculate current square position.
        sX = cS % (width -1);
        sZ = (cS - sX) / (width - 1) ;
        if (sZ == height || sZ == height -1 ) continue;
        // Calculate indices of each square corner.
        tl = (sZ * width) + sX;
        tr = (sZ * width) + (sX + 1);
        bl = ((sZ + 1) * width) + sX;
        br = ((sZ + 1) * width) + (sX + 1);
        this->indices.push_back(bl);
        this->indices.push_back(tr);
        this->indices.push_back(tl);
        this->indices.push_back(bl);
        this->indices.push_back(br);
        this->indices.push_back(tr);
    }
    
    
    setupHeightMap();
    
}

unsigned char* HeightMap::LoadPPM(char* filename , int &width,int &height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    size_t read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    
    if((fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }
    
    //Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    
    //Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while(buf[0][0] == '#');
    
    //Set the width and height
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    //Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while(buf[0][0] == '#');
    
    //Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    
    //If the read was a failure, error
    if(read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    
    return rawData;
}

void HeightMap::Draw(Shader shader)
{
    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    
    glm::mat4 model = glm::mat4(1.0);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        // Now set the sampler to the correct texture unit
        //cout<<name<<endl;
        glUniform1i(glGetUniformLocation(shader.ID, (name).c_str()), i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    
    // Also set each HeightMap's shininess property to a default value (if you want you could extend this to another HeightMap property and possibly change this value)
    glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16.0f);
    
    // Draw HeightMap
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, (GLuint)this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Always good practice to set everything back to defaults once configured.
    for (GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void HeightMap::setupHeightMap()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    
    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
    
    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
}

float HeightMap::getHeight(float x, float z)
{
    //cout<<"x:"<<x<<"z:"<<z<<endl;
    int j = (x / mapScale + 0.5f ) * (width - 1);
    int i = (z / mapScale + 0.5f ) * (height - 1);
    float fj = (x / mapScale + 0.5f ) * (width - 1);
    float fi = (z / mapScale + 0.5f ) * (height - 1);
    //cout<< "i:" << i<< "fi:"<<fi << "j:"<<j<<"fj:"<<fj<<endl;
    int i_p = i + 1;
    int j_p = j + 1;
    if (i_p >= width || j_p>=height)
        return vertices[i*width + j].Position.y;
    float tl,tr,bl,br;
    bl = vertices[i*width + j].Position.y;
    br = vertices[i_p*width + j].Position.y;
    tl = vertices[i*width + j_p].Position.y;
    tr = vertices[i_p*width + j_p].Position.y;
    //cout<< "bl:" << bl<< "br:"<<br << "tl:"<<tl<<"tr:"<<tr<<endl;

    float t_iter = tl+(fi - i)*(tr - tl);
    float b_iter = bl+(fi - i)*(br - bl);
    float result = b_iter + (fj - j)*(t_iter - b_iter);
    //return vertices[i*width + j].Position.y;
    //cout<<"result"<<result<<endl;
    return result;
}

glm::vec3 HeightMap::getNormal(float x, float z)
{
    int j = (x / mapScale + 0.5f ) * (width - 1);
    int i = (z / mapScale + 0.5f ) * (height - 1);
    float fj = (x / mapScale + 0.5f ) * (width - 1);
    float fi = (z / mapScale + 0.5f ) * (height - 1);
    //cout<< "i:" << i<< "fi:"<<fi << "j:"<<j<<"fj:"<<fj<<endl;
    int i_p = i + 1;
    int j_p = j + 1;
    if (i_p >= width || j_p>=height)
        return vertices[i*width + j].Normal;
    glm::vec3 tl,tr,bl,br;
    bl = vertices[i*width + j].Normal;
    br = vertices[i_p*width + j].Normal;
    tl = vertices[i*width + j_p].Normal;
    tr = vertices[i_p*width + j_p].Normal;
    //cout<< "bl:" << bl<< "br:"<<br << "tl:"<<tl<<"tr:"<<tr<<endl;
    
    glm::vec3 t_iter = tl+(fi - i)*(tr - tl);
    glm::vec3 b_iter = bl+(fi - i)*(br - bl);
    glm::vec3 result = b_iter + (fj - j)*(t_iter - b_iter);
    //return vertices[i*width + j].Position.y;
    //cout<<"result"<<result<<endl;
    return result;
}