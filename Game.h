#ifndef GAME_H
#define GAME_H
#include "Model.h"
#include "Camera.h"
#include "Resource_manager.h"

class Game
{
public:
	// Game state
	//GameState  State;
	Model tank;
	Camera camera;

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
    
    glm::vec3 lightPos;
    GLuint lightVAO;
    
    bool keys[1024];
    
    //debug
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch);
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);
};
#endif