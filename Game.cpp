#include "Game.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>



Game::Game(GLuint width, GLuint height){
	this->Width = width;
	this->Height = height;
}

void Game::Init()
{
    camera = Camera(glm::vec3(0.0f, 0.0f, -30.0f));
	light = Light(glm::vec3(1.2f, 1.0f, 2.0f));
    ResourceManager::LoadShader("./shader/model_loading.vs", "./shader/model_loading.frag", nullptr, "model");
    ResourceManager::LoadShader("./shader/sky.vs", "./shader/sky.frag", nullptr, "sky");
    ResourceManager::LoadShader("./shader/do_nothing.vert", "./shader/do_nothing.frag", nullptr, "do_nothing");
    ResourceManager::LoadShader("./shader/particle.vs", "./shader/particle.frag", nullptr, "part");
    
	topModel = new Model("./obj/tank_top_no_texture.obj");
	botModel = new Model("./obj/tank_bottm_no_texture.obj");
	bulletModel = new Model("./obj/missel.obj");
    
    SanDiego = HeightMap("./PPM/SanDiegoTerrain.ppm");
    tank = new Tank(topModel,botModel,bulletModel);
//    camera.updateCamera(tank->position);
    skybox = new Skybox;
//    testParticle = new Particle(glm::vec3(0,0,0), glm::vec3(0.1, 0.1, 0.1), 1);
    tree = new Tree();
    
	//bullet = new Bullet(tank->position,tank->topAngle,bulletModel);
}


Game::~Game()
{
}

void Game::ProcessInput(GLfloat dt)
{
    if(keys[GLFW_KEY_W]) {
        camera.ProcessKeyboard(FORWARD, dt);
    }
    if(keys[GLFW_KEY_S]) {
        camera.ProcessKeyboard(BACKWARD, dt);
    }
    if(keys[GLFW_KEY_A]) {
        camera.ProcessKeyboard(LEFT, dt);
    }
    if(keys[GLFW_KEY_D]) {
        camera.ProcessKeyboard(RIGHT, dt);
    }
	if (keys[GLFW_KEY_UP]){
		tank->move(dt);
        camera.updateCamera(tank->position);
	}
	if (keys[GLFW_KEY_DOWN]){
		tank->move(-dt);
        camera.updateCamera(tank->position);
	}
	if (keys[GLFW_KEY_LEFT]){
		tank->spinBot(dt);
        camera.updateCamera(tank->position);
//        camera.RotateCamera(0.5 * dt, tank->position);
	}
	if (keys[GLFW_KEY_RIGHT]){
		tank->spinBot(-dt);
        camera.updateCamera(tank->position);
	}
	if (keys[GLFW_KEY_SPACE]){
		sceneList.push_back(tank->fire());
	}
//    std::cout<<glm::to_string(tank->position)<<std::endl;
}

void Game::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
{
	if (!keys[GLFW_KEY_C]){
		tank->spinTop(-xoffset);
	}
	else	camera.processMouseMovement(xoffset, yoffset, constrainPitch);
}

void Game::ProcessMouseScroll(GLfloat yoffset)
{
//    camera.ProcessMouseScroll(yoffset);
}


void Game::Render(){
	Shader shader = ResourceManager::GetShader("model");
    Shader skyshader = ResourceManager::GetShader("sky");
    Shader terrainshader = ResourceManager::GetShader("do_nothing");
    Shader part_Shader = ResourceManager::GetShader("part");

    glDisable(GL_LIGHTING);
    
	// Projection and view matrix
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
    //render the skybox using skyshader
    skyshader.Use();
    glUniformMatrix4fv(glGetUniformLocation(skyshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(skyshader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

//    skybox->draw(skyshader);
    
    //render objects using shader
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
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    
    
    
//    SanDiego.Draw(terrainshader);

//    tank->draw(shader);
//    std::cout<<tree->trees->size()<<std::endl;

//    for (auto it : sceneList){
//        it->draw(shader);
//    }
    
    Shader partShader = ResourceManager::GetShader("part");
    partShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(partShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(partShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
//    tree->drawLine(part_Shader);
//    tree->leaf(part_Shader);
    tree->draw(part_Shader);
    //testParticle->draw(partShader);
    

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

void Game::Update(float dt){
//    testParticle->update(dt);
	Drawable* del;
	for (auto it = sceneList.begin(); it != sceneList.end();++it){
		if ((*it) == 0){
			continue;
		}
		if ((*it)->state == Drawable::ALIVE)
			(*it)->update(dt);
		else if ((*it)->state == Drawable::DEAD){
			del = *it;
			sceneList.erase(it);
            it --;
			delete (del);
		}
	}
}