#include "Game.h"
#include <GLFW/glfw3.h>


Game::Game(GLuint width, GLuint height){
	this->Width = width;
	this->Height = height;
}

void Game::Init()
{
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	light = Light(glm::vec3(1.2f, 1.0f, 2.0f));
    ResourceManager::LoadShader("./shader/model_loading.vs", "./shader/model_loading.frag", nullptr, "model");
	topModel = new Model("./obj/tank_top_no_texture.obj");
	botModel = new Model("./obj/tank_bottm_no_texture.obj");
	bulletModel = new Model("./obj/missel.obj");
	tank = new Tank(topModel,botModel);
	bullet = new Bullet(tank->position,tank->topAngle,bulletModel);
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
	if (keys[GLFW_KEY_UP]){
		tank->move(dt);
	}
	if (keys[GLFW_KEY_DOWN]){
		tank->move(-dt);
	}
	if (keys[GLFW_KEY_LEFT]){
		tank->spinBot(dt);
	}
	if (keys[GLFW_KEY_RIGHT]){
		tank->spinBot(-dt);
	}
}

void Game::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
{
	if (keys[GLFW_KEY_O]){
		tank->spinTop(yoffset);
	}
	else	camera.processMouseMovement(xoffset, yoffset, constrainPitch);
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
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.getColor()));
    glUniform3fv(lightPosLoc, 1,glm::value_ptr(light.getPosition()));
    glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);
    
	// Transformation matrices
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	tank->draw(shader);
	bullet->draw(shader);
}

void Game::setLight(GLuint sID){
	GLint lightColorLoc = glGetUniformLocation(sID, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(sID, "lightPos");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.getColor()));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.getPosition()));
}

void Game::setPVmatrix(GLuint sID){
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(sID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(sID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}