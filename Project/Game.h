#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Camera.h"
#include "Object.h"
#include "Vector"
class Game
{
	GLFWwindow* window;
	std::shared_ptr<Camera> cameraObject;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::shared_ptr<Object>> renderObjects;
public:
	Game(GLFWwindow* window_);
	void start();
	void update();
	void render();
};

