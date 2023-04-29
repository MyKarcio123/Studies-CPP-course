#include "Input.h"
GLFWwindow* Input::window = nullptr;
std::unordered_map<int, int> Input::keyStates;
std::unordered_map<int, int> Input::lastKeyStates;
std::unordered_set<int> Input::keyToCheck;
float Input::xOffSet = 0.0;
float Input::yOffSet = 0.0;
float Input::lastX = 0.0;
float Input::lastY = 0.0;
float Input::xPos = 0.0;
float Input::yPos = 0.0;
bool Input::firstMouse = false;