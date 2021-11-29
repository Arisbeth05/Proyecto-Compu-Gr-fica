#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevez() { return muevez; }
	GLfloat getmuevexChica() { return muevexChica; }
	GLfloat getmueveyChica() { return mueveyChica; }
	GLfloat getmuevezChica() { return muevezChica; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat gethel_x() { return hel_x; }
	GLfloat gethel_y() { return hel_y; }
	GLfloat getheli() { return heli; }

	GLfloat getllanta_1() { return llanta_1; }
	GLfloat getllanta_2() { return llanta_2; }
	GLfloat getllanta_3() { return llanta_3; }
	GLfloat getllanta_4() { return llanta_4; }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevez;
	GLfloat muevexChica;
	GLfloat muevezChica;
	GLfloat mueveyChica;

	GLfloat hel_x;
	GLfloat hel_y;
	GLfloat heli;

	GLfloat llanta_1;
	GLfloat llanta_2;
	GLfloat llanta_3;
	GLfloat llanta_4;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

