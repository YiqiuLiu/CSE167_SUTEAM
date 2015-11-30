#include <GL/glew.h>

#include "Model.h"
#include "Camera.h"
#include "resource_manager.h"

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
};
