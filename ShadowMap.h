#pragma once
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap
{
public:
	GLuint width, height;
	GLuint depthMapFBO;
	GLuint depthMap;

	ShadowMap(GLuint width = 1024,GLuint height =1024);
	~ShadowMap();

	void init();
	GLuint getFBO();
	GLuint getShadowMap();
};

