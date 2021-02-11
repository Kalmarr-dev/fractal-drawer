#pragma once

#include "DrawnLines.h"

// #include <GLFW/glfw3.h>
#include "src/glfw/glfw-3.3.2/include/GLFW/glfw3.h"
#include <vector>

void MouseButtonPressCallback(GLFWwindow* window, int button, int action, int mods);

void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);

void AddMouseCurrentPosition(float*& mousePositionsArr, unsigned int*& mousePositionsIndicesArr, int& mousePositionsArrSize, GLFWwindow* window, const int windowW, const int windowH);
// void AddMouseCurrentPosition(GLFWwindow* window, const int windowW, const int windowH);

void AddMouseReleaseIndex(std::vector<int>& MouseReleaseIndices, const int& mousePositionsArrSize);
// void AddMouseReleaseIndex();

bool GetMouseButtonRightState();
