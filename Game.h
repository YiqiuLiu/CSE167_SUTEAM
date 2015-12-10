#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Bullet.h"
#include "Tank.h"
#include "Resource_manager.h"
#include "HeightMap.h"
#include "Skybox.h"
#include "Particle.h"
#include "ShadowMap.h"
#include "ParticleManager.h"
#include "Tree.h"

class Game
{
public:
	// Game state
	//GameState  State;
	Model* botModel;
	Model* topModel;
	Model* bulletModel;

	Bullet* bullet;
	Tank* tank;
	std::vector<Drawable*> sceneList;

	Camera camera;
	Light light;
    HeightMap SanDiego;
    Skybox* skybox;
    Tree* tree;
	ShadowMap shadowMap;
    
	GLboolean  Keys[1024];
	GLuint	   Width, Height;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	
	void Render();
    bool keys[1024];
    
//    Particle* testParticle;
    ParticleManager* pm;


    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch);
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

private:
	void setLight(GLuint);
	void setPVmatrix(GLuint);
	void buildShadowMap();
	void RenderScene();
	void RenderTank(Shader);
	void RenderTerrain(Shader);
	void shadowRender(Shader);
	void RenderSky(Shader);
	void RenderTest();
	void computeLightView(glm::mat4 &view,glm::mat4 &projection,glm::mat4 trans);
};
#endif