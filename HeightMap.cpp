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
}

void HeightMap::buildMap()
{
    
    struct MyVertex
    {
        float x, y, z;        //Vertex
        float nx, ny, nz;     //Normal
        float s0, t0;         //Texcoord0
    };
    
    MyVertex pvertex[3];
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*3, &pvertex[0].x, GL_STATIC_DRAW);
    
    
    //Define this somewhere in your header file
    #define BUFFER_OFFSET(i) ((void*)(i))
    
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(12));   //The starting point of normals, 12 bytes away
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexVBOID);
    //To render, we can either use glDrawElements or glDrawRangeElements
    //The is the number of indices. 3 indices needed to make a single triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
    //0 and 3 are the first and last vertices
    //glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
    //glDrawRangeElements may or may not give a performance advantage over glDrawElements
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


void HeightMap::render()
{
    //TODO
}