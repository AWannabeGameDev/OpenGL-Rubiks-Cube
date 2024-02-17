#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
					 GLsizei length, const GLchar* message, const void* userParam);

GLFWwindow* initialize(int windowWidth, int windowHeight, const char* title, int majorVersion, int minorVersion);

unsigned int compileShader(unsigned int type, const char* path);
unsigned int createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);

#endif