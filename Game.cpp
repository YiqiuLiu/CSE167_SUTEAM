#include "Game.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>



Game::Game(GLuint width, GLuint height){
	this->Width = width;
	this->Height = height;
}

void Game::Init()
{
    camera = Camera(glm::vec3(0.0f, 10.0f, -10.0f));
	light = Light(glm::vec3(-10.0f, 50.0f, -12.0f));
	shadowMap = ShadowMap(Width,Height);

    ResourceManager::LoadShader("./shader/model_loading.vs", "./shader/model_loading.frag", nullptr, "model");
    ResourceManager::LoadShader("./shader/sky.vs", "./shader/sky.frag", nullptr, "sky");
    ResourceManager::LoadShader("./shader/do_nothing.vert", "./shader/do_nothing.frag", nullptr, "do_nothing");
    ResourceManager::LoadShader("./shader/particle.vs", "./shader/particle.frag", nullptr, "part");
	ResourceManager::LoadShader("./shader/shadowMapShader.vs","./shader/shadowMapShader.frag",nullptr,"shadowMap");
	ResourceManager::LoadShader("./shader/test.vs", "./shader/test.frag", nullptr, "test");

	topModel = new Model("./obj/tank_top_no_texture.obj");
	botModel = new Model("./obj/tank_bottm_no_texture.obj");
	bulletModel = new Model("./obj/missel.obj");
    
    SanDiego = HeightMap("./PPM/SanDiegoTerrain.ppm");
    tank = new Tank(topModel,botModel,bulletModel);
    camera.updateCamera(tank->position);
    skybox = new Skybox;
    
	shadowMap.init();
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
	buildShadowMap();
	RenderScene();
}

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void Game::RenderScene(){
	Shader shader = ResourceManager::GetShader("model");
    Shader skyshader = ResourceManager::GetShader("sky");
    Shader terrainshader = ResourceManager::GetShader("do_nothing");

	glViewport(0, 0, Width, Height);
    
	// Projection and view matrix
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();

    skyshader.Use();
	skyshader.SetMatrix4("projection", projection,false);
	skyshader.SetMatrix4("projection", view, false);

    shader.Use();
	shader.SetVector3f("objectColor",1.0f,0.5f,0.31f,false);
	shader.SetVector3f("lightColor", light.getColor(), false);
	shader.SetVector3f("lightPos", light.getPosition(), false);
	shader.SetVector3f("viewPos", camera.Position, false);
	shader.SetMatrix4("projection", projection, true);
	shader.SetMatrix4("view", view, true);
    tank->draw(shader);

	terrainshader.Use();
	terrainshader.SetVector3f("viewPos", camera.Position, true);
	terrainshader.SetMatrix4("projection", projection, true);
	terrainshader.SetMatrix4("view", view, true);
	terrainshader.SetVector3f("lightPos", light.position);
	terrainshader.SetVector3f("viewPos", camera.Position);
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 100.0f);
	glm::mat4 lightView = glm::lookAt(light.getPosition(), tank->position, glm::vec3(1.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	terrainshader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix, true);
	
	terrainshader.SetVector3f("objectColor", 1.0f, 0.5f, 0.31f, false);
	terrainshader.SetVector3f("lightColor", light.getColor(), false);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,shadowMap.getShadowMap());
	glUniform1i(glGetUniformLocation(terrainshader.ID, "shadowMap"), 0);
	SanDiego.Draw(terrainshader);
	/*
	Shader testShader = ResourceManager::GetShader("test");
	testShader.Use();
	glm::mat4 model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(10.0, 10.0, 10.0));
	testShader.SetMatrix4("projection", projection, true);
	testShader.SetMatrix4("view", view, true);
	testShader.SetMatrix4("model", model, true);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap.getShadowMap());
	RenderQuad();
	*/
	


//    for (auto it : sceneList){
//        it->draw(shader);
//    }
    
    Shader partShader = ResourceManager::GetShader("part");
	
    partShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(partShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(partShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    
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
    //testParticle->update(dt);
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

void Game::buildShadowMap(){
	Shader shader = ResourceManager::GetShader("shadowMap");
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = 1.0f;
	GLfloat far_plane = 100.0f;
	lightProjection = glm::ortho(-20.0f,20.0f,-20.0f,20.0f,near_plane,far_plane);
	lightView = glm::lookAt(light.getPosition(), tank->position, glm::vec3(1.0));
	lightSpaceMatrix = lightProjection * lightView;
	
	shader.SetMatrix4("lightSpaceMatrix",lightSpaceMatrix,true);
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.getFBO());
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowRender(shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::shadowRender(Shader shader){
	shader.Use();
	SanDiego.Draw(shader);
	tank->draw(shader);
}