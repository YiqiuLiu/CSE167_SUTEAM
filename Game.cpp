#include "Game.h"

Game::Game(GLuint width, GLuint height){
	this->Width = width;
	this->Height = height;
}

void Game::Init()
{
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    ResourceManager::LoadShader("./shader/model_loading.vs", "./shader/model_loading.frag", nullptr, "model");
    tank = Model("./obj/tank_whole_no_texture.obj");
}


Game::~Game()
{
}

void Game::Render(){
	Shader shader = ResourceManager::GetShader("model");
	shader.Use();
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
	tank.Draw(shader);
}