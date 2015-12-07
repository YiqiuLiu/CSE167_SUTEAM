#include "ShadowMap.h"


ShadowMap::ShadowMap(GLuint width, GLuint height)
{
	this->width = width;
	this->height = height;
}


ShadowMap::~ShadowMap()
{
}

void ShadowMap::init(){
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D,depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenFramebuffers(1, &depthMapFBO);
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint ShadowMap::getFBO(){
	return this->depthMapFBO;
}

GLuint ShadowMap::getShadowMap(){
	return this->depthMap;
}