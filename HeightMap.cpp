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
    buildMap_test();
    //buildMap();
}
struct MyVertex__1
{
    float x, y, z;        //Vertex
    float nx, ny, nz;     //Normal
    float s0, t0;         //Texcoord0
};

void HeightMap::buildMap_test()
{
    glDisable(GL_BLEND);
        
    MyVertex__1 pvertex[3];
    //VERTEX 0
    pvertex[0].x = 0.0;
    pvertex[0].y = 0.0;
    pvertex[0].z = 0.0;
    pvertex[0].nx = 0.0;
    pvertex[0].ny = 0.0;
    pvertex[0].nz = 1.0;
    pvertex[0].s0 = 0.0;
    pvertex[0].t0 = 0.0;
    //VERTEX 1
    pvertex[1].x = 1.0;
    pvertex[1].y = 0.0;
    pvertex[1].z = 0.0;
    pvertex[1].nx = 0.0;
    pvertex[1].ny = 0.0;
    pvertex[1].nz = 1.0;
    pvertex[1].s0 = 1.0;
    pvertex[1].t0 = 0.0;
    //VERTEX 2
    pvertex[2].x = 0.0;
    pvertex[2].y = 1.0;
    pvertex[2].z = 0.0;
    pvertex[2].nx = 0.0;
    pvertex[2].ny = 0.0;
    pvertex[2].nz = 1.0;
    pvertex[2].s0 = 0.0;
    pvertex[2].t0 = 1.0;
    
    glGenBuffers(1, &VertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex__1)*3, &pvertex[0].x, GL_STATIC_DRAW);
    
    ushort pindices[3];
    pindices[0] = 0;
    pindices[1] = 1;
    pindices[2] = 2;
    
    glGenBuffers(1, &IndexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort)*3, pindices, GL_STATIC_DRAW);
    
        
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(MyVertex__1), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(MyVertex__1), BUFFER_OFFSET(12));   //The starting point of normals, 12 bytes away
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex__1), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    //To render, we can either use glDrawElements or glDrawRangeElements
    //The is the number of indices. 3 indices needed to make a single triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    
    std::cout<<"sssss"<<std::endl;
    
}

void HeightMap::buildMap()
{
    glDisable(GL_BLEND);
    
    
    MyVertex *pvertex = new MyVertex[width*height];
    
    //test
    AttVertex = pvertex;
    float fTextureU = float(width)*0.1f;
    float fTextureV = float(height)*0.1f;
    
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
            if (!j) {
                std::cout <<int(tdata[(i*width+j)*3])<<":"<<int(tdata[(i*width+j)*3+1])<<":"<<int(tdata[(i*width+j)*3+2])<<std::endl;

            std::cout << -0.5f+fScaleC<<" "<<fVertexHeight<<" "<<-0.5f+fScaleR<<std::endl;
            }
            pvertex[i*width + j].vert = glm::vec3(-0.5f+fScaleC, fVertexHeight, -0.5f+fScaleR);
            pvertex[i*width + j].txtr = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
        }
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
                pvertex[i*width + j].vert,
                pvertex[(i+1)*width + j].vert,
                pvertex[(i+1)*width + j+1].vert
            };
            glm::vec3 vTriangle1[] =
            {
                pvertex[(i+1)*width + j+1].vert,
                pvertex[i*width + j+1].vert,
                pvertex[i*width + j].vert
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
        
            pvertex[i*width + j].norm = vFinalNormal; // Store final normal of j-th vertex in i-th row
        }
    }
    
    glGenBuffers(1, &VertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*width*height, pvertex, GL_STATIC_DRAW);
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glEnableClientState(GL_VERTEX_ARRAY);
    // type, stride size, offset
    glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), 0);   //The starting point of the VBO, for the vertices
    
    //test
    //glEnableClientState(GL_COLOR_ARRAY);
    //glColorPointer(3,GL_FLOAT, sizeof(MyVertex), (void*)sizeof(glm::vec3));
    
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(MyVertex), (void*)sizeof(glm::vec3));
    
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexVBOID);
    //To render, we can either use glDrawElements or glDrawRangeElements
    //The is the number of indices. 3 indices needed to make a single triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
    //0 and 3 are the first and last vertices
    //glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
    //glDrawRangeElements may or may not give a performance advantage over glDrawElements
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindVertexArray(0);

    //delete pvertex;
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

GLint HeightMap::TextureFromFile(const char* path, string directory)

{
    //Generate texture ID and load texture data
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    TEXTURE_id = textureID;
    return textureID;
}

void HeightMap::render(Shader shader)
{
    //glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16.0f);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    
    // Step 2
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // Step 3
    glVertexPointer(3, GL_FLOAT, sizeof(MyVertex__1), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
    glNormalPointer(GL_FLOAT, sizeof(MyVertex__1), BUFFER_OFFSET(12));   //The starting point of normals, 12 bytes away
    glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex__1), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    //To render, we can either use glDrawElements or glDrawRangeElements
    //The is the number of indices. 3 indices needed to make a single triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    
    // Step 4
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    
    // Step 5
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //To render, we can either use glDrawElements or glDrawRangeElements
    //The is the number of indices. 3 indices needed to make a single triangle
    glBindVertexArray(0);

}

//test
typedef struct
{
    float location[4];
    float color[4];
} Vertex;
Vertex verts[6]; // triangle vertices
GLubyte tindices[6]; // triangle vertex indices
GLuint vboHandle[1]; // a VBO that contains interleaved positions and colors
GLuint indexVBO;

void HeightMap::InitGeometry()
{
    verts[0].location[0] = -0.5; verts[0].location[1] = -0.5; verts[0].location[2] = 0; verts[0].location[3] = 1;
    verts[1].location[0] = -0.5; verts[1].location[1] = 0.5; verts[1].location[2] = 0; verts[1].location[3] = 1;
    verts[2].location[0] = 0.5; verts[2].location[1] = 0.5; verts[2].location[2] = 0; verts[2].location[3] = 1;
    verts[3].location[0] = 0.5; verts[3].location[1] = 0.5; verts[3].location[2] = 0; verts[3].location[3] = 1;
    verts[4].location[0] = 0.5; verts[4].location[1] = -0.5; verts[4].location[2] = 0; verts[4].location[3] = 1;
    verts[5].location[0] = -0.5; verts[5].location[1] = -0.5; verts[5].location[2] = 0; verts[5].location[3] = 1;
    verts[0].color[0] = 1; verts[0].color[1] = 1; verts[0].color[2] = 0; verts[0].color[3] = 1;
    verts[1].color[0] = 1; verts[1].color[1] = 1; verts[1].color[2] = 0; verts[1].color[3] = 1;
    verts[2].color[0] = 1; verts[2].color[1] = 1; verts[2].color[2] = 0; verts[2].color[3] = 1;
    verts[3].color[0] = 1; verts[3].color[1] = 0; verts[3].color[2] = 0; verts[3].color[3] = 1;
    verts[4].color[0] = 1; verts[4].color[1] = 0; verts[4].color[2] = 0; verts[4].color[3] = 1;
    verts[5].color[0] = 1; verts[5].color[1] = 0; verts[5].color[2] = 0; verts[5].color[3] = 1;
    // create triangle vertex indices.
    tindices[0] = 0; tindices[1] = 1; tindices[2] = 2;
    tindices[3] = 3; tindices[4] = 4; tindices[5] = 5;
}

void  HeightMap::InitVBO()
{
    
    glGenBuffers(1, vboHandle); // create two VBO handles, one position, one color handle
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); // bind the first handle
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*6, verts, GL_STATIC_DRAW); // allocate space and copy the
    // position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    cout<<"VBO HANDLE:"<<vboHandle[0]<<"indexVBOL"<<indexVBO<<endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*6, tindices, GL_STATIC_DRAW); // load the
    // index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); // clean up
    // by now, we moved the position and color data over to the graphics card. There will be no redundant data
    // copy at drawing time
}

void  HeightMap::display(Shader shader)
{
    glClearColor(0,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16.0f);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
    glEnableClientState(GL_COLOR_ARRAY); // enable the color array on the client side
    // number of coordinates per vertex (4 here), type of the coordinates,
    // stride between consecutive vertices, and pointers to the first coordinate
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), (char*) NULL+ 16);
    glVertexPointer(4,GL_FLOAT, sizeof(Vertex), (char*) NULL+ 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (char*) NULL+0);
    glDisableClientState(GL_VERTEX_ARRAY); glDisableClientState(GL_COLOR_ARRAY);
    //glutSwapBuffers();
}
