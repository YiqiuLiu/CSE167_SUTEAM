//
//  Skybox.h
//  TankBattle
//
//  Created by Yiqiu Liu on 12/2/15.
//  Copyright © 2015 SUTeam. All rights reserved.
//

#ifndef Skybox_h
#define Skybox_h
#include "Drawable.h"
#include "Shader.h"
// Std. Includes
#include <string>
#include <algorithm>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Skybox : public Drawable{
public:
    Skybox();
    ~Skybox();
    
    GLuint loadCubemap(vector<const GLchar*> faces);
    GLuint cubemapTexture;
    GLuint skyboxVAO, skyboxVBO;
    vector<const GLchar*> faces;
    void draw(Shader);
};

#endif /* Skybox_h */
