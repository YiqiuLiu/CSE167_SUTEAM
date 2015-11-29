#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Game.h"
#include "../resource_manager.h"

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main(int argc, const char* argv[]) {
	if (!glfwInit()) {
		return 1;
	}

	int monitorCount;

	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

	GLFWmonitor* largestMonitor = monitors[0];

	const GLFWvidmode* largestVidmode = glfwGetVideoMode(largestMonitor);

	for (int i = 1; i < monitorCount; i += 1) {
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitors[i]);

		if (vidmode->width * vidmode->height > largestVidmode->width * largestVidmode->height) {
			largestVidmode = vidmode;

			largestMonitor = monitors[i];
		}
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(largestVidmode->width, largestVidmode->height, "OpenGLTest", largestMonitor, nullptr);

	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window." << std::endl;

		glfwTerminate();

		return 1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	glewInit();

	const GLubyte* openGLRenderer = glGetString(GL_RENDERER);

	const GLubyte* openGLVersion = glGetString(GL_VERSION);

	std::cout << "Renderer: " << openGLRenderer << std::endl;

	std::cout << "OpenGL version supported: " << openGLVersion << std::endl;

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLuint triangleVBO = 0;

	glGenBuffers(1, &triangleVBO);

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint triangleVAO = 0;

	glGenVertexArrays(1, &triangleVAO);

	glBindVertexArray(triangleVAO);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	const char* vertexShaderSource =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		"  gl_Position = vec4 (vp, 1.0);"
		"}";

		const char* fragmentShaderSource =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main() {"
		"  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
		"}";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, fragmentShader);

	glAttachShader(shaderProgram, vertexShader);

	glLinkProgram(shaderProgram);

	glfwSetKeyCallback(window, keyCallback);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(triangleVAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}