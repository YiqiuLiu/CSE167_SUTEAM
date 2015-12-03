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