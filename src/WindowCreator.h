#pragma once

#include <GLFW/glfw3.h>
// #include "glfw/glfw-3.3.2/include/GLFW/glfw3.h"

struct WindowCreator {
  static GLFWwindow* CreateWindowedWindow (int width, int height, GLFWwindow* prevWindow = NULL);
  static GLFWwindow* CreateFullscreenWindow (int width, int height, GLFWwindow* prevWindow = NULL);
};
