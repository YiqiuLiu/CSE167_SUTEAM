#include "Game.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <string>
#include <vector>



Game::Game(GLuint width, GLuint height){
	this->Width = width;
	this->Height = height;
}

void Game::Init()
{

    camera = Camera(glm::vec3(0.0f, 6.0f, -6.0f));
	light = Light(glm::vec3(100.0f, 100.0f, 0.0f));
	shadowMap = ShadowMap(1024,1024);

    ResourceManager::LoadShader("./shader/model_loading.vs", "./shader/model_loading.frag", nullptr, "model");
    ResourceManager::LoadShader("./shader/sky.vs", "./shader/sky.frag", nullptr, "sky");
    ResourceManager::LoadShader("./shader/do_nothing.vert", "./shader/do_nothing.frag", nullptr, "do_nothing");
    ResourceManager::LoadShader("./shader/particle.vs", "./shader/particle.frag", nullptr, "part");
	ResourceManager::LoadShader("./shader/shadowMapShader.vs","./shader/shadowMapShader.frag",nullptr,"shadowMap");
	ResourceManager::LoadShader("./shader/test.vs", "./shader/test.frag", nullptr, "test");

//	topModel = new Model("./obj/tank_top_no_texture.obj");
//	botModel = new Model("./obj/tank_bottm_no_texture.obj");
//	bulletModel = new Model("./obj/missel.obj");
    
    topModel = new Model("./obj/with_texture/tank_top_texture.obj");
    botModel = new Model("./obj/with_texture/tank_bottm_texture.obj");
    bulletModel = new Model("./obj/with_texture/missel_texture.obj");
	
//    camera.updateCamera(tank->position);
    skybox = new Skybox;

//    testParticle = new Particle(glm::vec3(0,0,0), glm::vec3(0.1, 0.1, 0.1), 1);
    tree = new Tree(glm::vec3(0,10,0));
//    cout<<"====debug===="<<endl;
//    cout<<tree->trees->at(0)<<endl;
//    cout<<tree->trees->at(1)<<endl;
//    cout<<tree->trees->at(2)<<endl;
//    cout<<"====debug===="<<endl;
    // terrain texture init
    SanDiego = HeightMap("./PPM/SanDiegoTerrain.ppm");
    Texture2D Text = ResourceManager::LoadTexture("./texture/sea.jpg", false, "sea");
    Texture inputText;
    inputText.id = Text.ID;
    inputText.type ="gSampler0";
    SanDiego.textures.push_back(inputText);
//    cout<<"sea text ID = "<<Text.ID<<endl;
    Text = ResourceManager::LoadTexture("./texture/sand.jpg", false, "sand");
    inputText.id = Text.ID;
    inputText.type ="gSampler1";
//    cout<<"sand text ID = "<<Text.ID<<endl;
    SanDiego.textures.push_back(inputText);
    Text = ResourceManager::LoadTexture("./texture/sand_grass.jpg", false, "sand_grass");
    inputText.id = Text.ID;
    inputText.type ="gSampler2";
    SanDiego.textures.push_back(inputText);
//    cout<<"sand_grass text ID = "<<Text.ID<<endl;
    Text = ResourceManager::LoadTexture("./texture/rock.jpg", false, "rock");
    inputText.id = Text.ID;
    inputText.type ="gSampler3";
//    cout<<"rock text ID = "<<Text.ID<<endl;
    SanDiego.textures.push_back(inputText);
    Text = ResourceManager::LoadTexture("./texture/snow.jpg", false, "snow");
    inputText.id = Text.ID;
    inputText.type ="gSampler4";
//    cout<<"snow text ID = "<<Text.ID<<endl;
    SanDiego.textures.push_back(inputText);
    shadowMap.init();
    inputText.id = shadowMap.getShadowMap();
    inputText.type ="shadowMap";
//    cout<<"shadowMap text ID = "<<shadowMap.getShadowMap()<<endl;
    SanDiego.textures.push_back(inputText);
    
    // texture init done
    
    //tankmov.init("./wav/offer_x.wav");

	//tank init
    tank = new Tank(topModel, botModel, bulletModel);
	glm::vec3 pos = tank->position;
	glm::vec3 normal = SanDiego.getNormal(pos.x, pos.z);
	float h = SanDiego.getHeight(pos.x,pos.z);
	tank->setPosition(glm::vec3(pos.x, h + 1, pos.z));
    float angle = -glm::angle(normal, glm::vec3(0, 1, 0));
    glm::vec3 axis = glm::cross(normal, glm::vec3(0, 1, 0));
    glm::mat4 temp = glm::mat4();
    tank->selfRotate = glm::rotate(temp, angle, axis);
    
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
	//RenderTest();

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

void Game::RenderTank(Shader shader){
	GLint objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(shader.ID, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(shader.ID, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(shader.ID, "viewPos");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.getColor()));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.getDirection()));
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
	tank->draw(shader);
}

void Game::RenderTerrain(Shader terrainshader){
	terrainshader.SetVector3f("viewPos", camera.Position, true);
	terrainshader.SetVector3f("lightPos", light.direction);
	terrainshader.SetVector3f("objectColor", 1.0f, 0.5f, 0.31f, false);
	terrainshader.SetVector3f("lightColor", light.getColor(), false);
	terrainshader.SetFloat("fRenderHeight", SanDiego.scaleHeight);
	terrainshader.SetFloat("fMaxTextureU", float(SanDiego.height)*0.1f);
	terrainshader.SetFloat("fMaxTextureV", float(SanDiego.width)*0.1f);

	SanDiego.Draw(terrainshader);
}

void Game::RenderTest(){
	glViewport(0, 0, Width, Height);
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();

	Shader test = ResourceManager::GetShader("test");
	test.Use();
	glm::mat4 model = glm::mat4(1.0);
	test.SetMatrix4("model",model,true);
	test.SetMatrix4("projection", projection, true);
	test.SetMatrix4("view", view, true);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,shadowMap.getShadowMap());
	RenderQuad();
}

void Game::RenderScene(){
	glViewport(0, 0, Width, Height);

	// Projection and view matrix
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 lightProjection = glm::ortho(-20.0f, 30.0f, -10.0f, 30.0f, 0.1f, 200.0f);
	glm::mat4 lightView = glm::lookAt(light.getDirection() + tank->position, tank->position, glm::vec3(1.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	Shader shader = ResourceManager::GetShader("model");
	shader.Use();
	shader.SetMatrix4("view",view,false);
	shader.SetMatrix4("projection", projection, false);
	RenderTank(shader);

	for (auto it : sceneList){
		it->draw(shader);
	}

    Shader skyshader = ResourceManager::GetShader("sky");
	skyshader.Use();
	skyshader.SetMatrix4("projection", projection, true);
	skyshader.SetMatrix4("view", view, true);
	skybox->draw(skyshader);

    Shader terrainshader = ResourceManager::GetShader("do_nothing");
	terrainshader.SetMatrix4("projection", projection, true);
	terrainshader.SetMatrix4("view", view, true);
	terrainshader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix, true);
	RenderTerrain(terrainshader);


    Shader part_Shader = ResourceManager::GetShader("part");

//
//    
//    
    Shader partShader = ResourceManager::GetShader("part");
//
    partShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(partShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(partShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    tree->draw(part_Shader);
    

}

void Game::setLight(GLuint sID){
	GLint lightColorLoc = glGetUniformLocation(sID, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(sID, "lightPos");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.getColor()));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.getDirection()));
}

void Game::setPVmatrix(GLuint sID){
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(sID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(sID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void Game::Update(float dt){
    //testParticle->update(dt);
    
    //tank position
    glm::vec3 pos = tank->position;
    glm::vec3 normal = SanDiego.getNormal(pos.x, pos.z);
    float h = SanDiego.getHeight(pos.x,pos.z);
    tank->setPosition(glm::vec3(pos.x, h + 0.1, pos.z));
    float angle = -glm::angle(normal, glm::vec3(0, 1, 0));
    glm::vec3 axis = glm::cross(normal, glm::vec3(0, 1, 0));
    glm::mat4 temp = glm::mat4();
    tank->selfRotate = glm::rotate(temp, angle, axis);
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
	GLfloat near_plane = 0.1f;
	GLfloat far_plane = 200.0f;
	lightProjection = glm::ortho(-20.0f,30.0f,-10.0f,30.0f,near_plane,far_plane);
	lightView = glm::lookAt(light.getDirection()+tank->position, tank->position, glm::vec3(1.0));
	lightSpaceMatrix = lightProjection * lightView;
	
	shader.SetMatrix4("lightSpaceMatrix",lightSpaceMatrix,true);
	glViewport(0, 0,  1024, 1024);
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

void Game::computeLightView(glm::mat4 &view, glm::mat4 &projection, glm::mat4 trans){
	glm::vec4 position = glm::vec4(tank->position + light.direction,1.0);
	std::vector<glm::vec4> points;
	points.push_back(trans*glm::vec4(1.0, 1.0, 1.0, 1.0));
	points.push_back(trans*glm::vec4(1.0, 1.0, -1.0, 1.0));
	points.push_back(trans*glm::vec4(1.0, -1.0, 1.0, 1.0));
	points.push_back(trans*glm::vec4(1.0, -1.0, -1.0, 1.0));
	points.push_back(trans*glm::vec4(-1.0, 1.0, 1.0, 1.0));
	points.push_back(trans*glm::vec4(-1.0, 1.0, -1.0, 1.0));
	points.push_back(trans*glm::vec4(-1.0, -1.0, 1.0, 1.0));
	points.push_back(trans*glm::vec4(-1.0, -1.0, -1.0, 1.0));

	glm::vec4 dir = glm::vec4(light.direction,0.0);


}