#include "application.h"

#include <glm/gtc/type_ptr.hpp>

Application::Application() :
	SCREEN_WIDTH{1280}, SCREEN_HEIGHT{720}, window{initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Rubik's Cube", 4, 6)},
	keys{window},
	MOUSE_SENSITIVITY{6.0f}, camera{glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f},
	rubiksCube{}, ROTATION_SPEED_RADS{glm::radians(90.0f)},
	shaderProgram{createShaderProgram("../res/vertex_shader.shader", "../res/fragment_shader.shader")},
	u_modelMatrix{(unsigned int)glGetUniformLocation(shaderProgram, "u_modelMatrix")},
	u_viewMatrix{(unsigned int)glGetUniformLocation(shaderProgram, "u_viewMatrix")},
	dragging{false},
	prevTime{0}
{
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mousePositionCallbackProxy);
	glfwSetMouseButtonCallback(window, mouseButtonCallbackProxy);

	keys.setKeybind("WHITE", GLFW_KEY_W);
	keys.setKeybind("YELLOW", GLFW_KEY_Y);
	keys.setKeybind("RED", GLFW_KEY_R);
	keys.setKeybind("ORANGE", GLFW_KEY_O);
	keys.setKeybind("BLUE", GLFW_KEY_B);
	keys.setKeybind("GREEN", GLFW_KEY_G);
	keys.setKeybind("CLOCKWISE", GLFW_KEY_RIGHT);
	keys.setKeybind("COUNTER_CLOCKWISE", GLFW_KEY_LEFT);

	glDebugMessageCallback(glDebugCallback, nullptr);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	camera.position = {5.0f, 5.0f, 5.0f};
	camera.rotateGlobal(camera.up(), glm::radians(45.0f));
	camera.rotateGlobal(camera.right(), -atanf(1.0f / sqrtf(2.0f)));

	glGenBuffers(1, &positionVbo);
	glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
	glBufferData(GL_ARRAY_BUFFER, 
				 RubiksCube::VOX_NUM_VERTS * sizeof(glm::vec3), rubiksCube.getVoxelVertexPositions(),
				 GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, 
				 RubiksCube::VOX_NUM_VERTS * RubiksCube::CUBE_NUM_VOXES * sizeof(glm::vec4), rubiksCube.getColors(), 
				 GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindVertexBuffer(0, positionVbo, 0, sizeof(glm::vec3));

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				 RubiksCube::VOX_NUM_INDICES * sizeof(unsigned int), rubiksCube.getVoxelIndices(), 
				 GL_STATIC_DRAW);

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"),
					   1, GL_FALSE, glm::value_ptr(camera.projectionMatrix()));

	glm::mat4 viewMatrix = camera.viewMatrix();
	glUniformMatrix4fv(u_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

Application::~Application()
{
	glDeleteBuffers(1, &positionVbo);
	glDeleteBuffers(1, &colorVbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void Application::mouseButtonCallback(int button, int action)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
		{
			dragging = true;
			glfwGetCursorPos(window, &mouseLastX, &mouseLastY);
		}
		else
		{
			dragging = false;
		}
	}
}

void Application::mousePositionCallback(double xpos, double ypos)
{
	if(dragging)
	{
		float mouseDX = (float)(xpos - mouseLastX);
		float mouseDY = -(float)(ypos - mouseLastY);

		glm::vec3 mouseMove{mouseDX, mouseDY, 0.0f};
		mouseMove = glm::vec3{camera.inverseRotationMatrix() * glm::vec4{mouseMove, 1.0f}};

		glm::vec3 rotationAxis = glm::normalize(glm::cross(camera.behind(), mouseMove));
		float angle = glm::length(mouseMove) * MOUSE_SENSITIVITY * deltaTime;

		rubiksCube.globalTransform.rotation = glm::angleAxis(angle, rotationAxis) * rubiksCube.globalTransform.rotation;

		mouseLastX = xpos;
		mouseLastY = ypos;
	}
}

void Application::run()
{
	while(!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		glfwPollEvents();

		keys.update();

		if(keys.keyJustPressed("WHITE"))
		{
			rubiksCube.selectedFace = RubiksCube::FACE_POS_Y;
		}
		else if(keys.keyJustPressed("YELLOW"))
		{
			rubiksCube.selectedFace = RubiksCube::FACE_NEG_Y;
		}
		else if(keys.keyJustPressed("RED"))
		{
			rubiksCube.selectedFace = RubiksCube::FACE_POS_Z;
		}
		else if(keys.keyJustPressed("ORANGE"))
		{
			rubiksCube.selectedFace = RubiksCube::FACE_NEG_Z;
		}
		else if(keys.keyJustPressed("BLUE"))
		{
			rubiksCube.selectedFace = RubiksCube::FACE_POS_X;
		}
		else if(keys.keyJustPressed("GREEN"))
		{
			rubiksCube.selectedFace = RubiksCube::FACE_NEG_X;
		}

		if(keys.keyPressed("CLOCKWISE"))
		{
			rubiksCube.rotateFace(-ROTATION_SPEED_RADS * deltaTime);
		}
		else if(keys.keyPressed("COUNTER_CLOCKWISE"))
		{
			rubiksCube.rotateFace(ROTATION_SPEED_RADS * deltaTime);
		}

		if(keys.anyKeyJustReleased())
		{
			rubiksCube.snapFace();
		}

		glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(size_t voxel = 0; voxel < RubiksCube::CUBE_NUM_VOXES; voxel++)
		{
			glBindVertexBuffer(1, colorVbo, voxel * RubiksCube::VOX_NUM_VERTS * sizeof(glm::vec4), sizeof(glm::vec4));

			glm::mat4 modelMatrix = rubiksCube.globalTransform.matrix() * rubiksCube.getTransforms()[voxel].matrix();
			glUniformMatrix4fv(u_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			glDrawElements(GL_TRIANGLES, RubiksCube::VOX_NUM_INDICES, GL_UNSIGNED_INT, (const void*)0);
		}

		glfwSwapBuffers(window);
	}
}

void Application::mousePositionCallbackProxy(GLFWwindow* window, double xpos, double ypos)
{
	((Application*)glfwGetWindowUserPointer(window))->mousePositionCallback(xpos, ypos);
}

void Application::mouseButtonCallbackProxy(GLFWwindow* window, int button, int action, int mods)
{
	((Application*)glfwGetWindowUserPointer(window))->mouseButtonCallback(button, action);
}