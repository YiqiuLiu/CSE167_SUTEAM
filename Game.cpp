#include "Game.h"
#include <GLFW/glfw3.h>

Game::Game(GLuint width, GLuint height){
	this->Width = width;
	this->Height = height;
}

void Game::Init()
{
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    ResourceManager::LoadShader("./shader/model_loading.vs", "./shader/model_loading.frag", nullptr, "model");
    tank = Model("./obj/tank_whole_no_texture.obj");
    
    lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
}


Game::~Game()
{
}

void Game::ProcessInput(GLfloat dt)
{
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, dt);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, dt);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, dt);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, dt);
}

void Game::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
{
    camera.processMouseMovement(xoffset, yoffset, constrainPitch);
}

void Game::ProcessMouseScroll(GLfloat yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


void Game::Render(){
	Shader shader = ResourceManager::GetShader("model");
	shader.Use();
    //uniform
    GLint objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
    GLint lightColorLoc  = glGetUniformLocation(shader.ID, "lightColor");
    GLint lightPosLoc    = glGetUniformLocation(shader.ID, "lightPos");
    GLint viewPosLoc     = glGetUniformLocation(shader.ID, "viewPos");
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
    glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);
    
    
	// Transformation matrices
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Draw the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(lightVAO);
	tank.Draw(shader);
    glBindVertexArray(0);

}