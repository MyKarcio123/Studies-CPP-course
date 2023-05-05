#pragma once

#include <unordered_map>
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <iostream>

enum Input_Axis {
    VERTICAL,
    HORIZONTAL
};

class Input
{
public:
    static void SetWindow(GLFWwindow* window)
    {
        Input::window = window;
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
    }

    static bool GetKeyDown(int key)
    {
        return (lastKeyStates[key] == GLFW_RELEASE) && (keyStates[key] == GLFW_PRESS);
    }

    static bool GetKeyUp(int key)
    {
        return (lastKeyStates[key] == GLFW_PRESS) && (keyStates[key] == GLFW_RELEASE);
    }

    static bool GetKey(int key)
    {
        return (keyStates[key] == GLFW_PRESS);
    }

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        lastKeyStates[key] = keyStates[key];
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            keyStates[key] = GLFW_PRESS;
        }
        else if (action == GLFW_RELEASE)
        {
            keyStates[key] = GLFW_RELEASE;
        }
        if (lastKeyStates[key] != keyStates[key]) {
            keyToCheck.insert(key);
        }
    }
    static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
    {
        xPos = static_cast<float>(xposIn);
        yPos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }
    }
    static void Update() {
        xOffSet = xPos - lastX;
        yOffSet = lastY - yPos;
        lastX = xPos;
        lastY = yPos;
        for (auto it = keyToCheck.begin(); it != keyToCheck.end(); ++it) {
            lastKeyStates[*it] = keyStates[*it];
        }
        keyToCheck.clear();
    }

    static float GetAxis(Input_Axis axis) {
        if (axis == VERTICAL) {
            return xOffSet;
        }
        return yOffSet;
    }

    static void setPosition(float x, float y) {
        lastX = x / 2.0;
        lastY = y / 2.0;
    }
private:
    static std::unordered_set<int> keyToCheck;
    static std::unordered_map<int, int> keyStates;
    static std::unordered_map<int, int> lastKeyStates;
    static float xOffSet;
    static float yOffSet;
    static GLFWwindow* window;
    static bool firstMouse;
    static float lastX;
    static float lastY;  
    static float xPos;
    static float yPos;
};