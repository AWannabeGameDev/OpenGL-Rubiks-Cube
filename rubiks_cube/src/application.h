#ifndef APPLICATION_H
#define APPLICATION_H

#include "util.h"
#include "camera.h"
#include "rubiks_cube.h"
#include "keyboard_input.h"

class Application 
{
private :

	const int SCREEN_WIDTH, SCREEN_HEIGHT;
	GLFWwindow* window;
	KeyboardInput keys;

	const float MOUSE_SENSITIVITY;
	Camera camera;

	RubiksCube rubiksCube;
	const float ROTATION_SPEED_RADS;

	unsigned int positionVbo, colorVbo, ebo, vao;
	unsigned int shaderProgram;
	unsigned int u_modelMatrix;
	unsigned int u_viewMatrix;

	bool dragging;
	double mouseLastX, mouseLastY;

	float deltaTime, prevTime;

	static void mousePositionCallbackProxy(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallbackProxy(GLFWwindow* window, int button, int action, int mods);

	void mouseButtonCallback(int button, int action);
	void mousePositionCallback(double xpos, double ypos);

public :
	Application();
	~Application();

	void run();
};

#endif