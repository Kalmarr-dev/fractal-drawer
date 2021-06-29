#include "WindowCreator.h"

#include <GLFW/glfw3.h>
// #include "glfw/glfw-3.3.2/include/GLFW/glfw3.h"

#include "Input.h"
#include "WindowResizeCallback.h"


GLFWwindow* WindowCreator::CreateWindowedWindow(int width, int height, GLFWwindow* prevWindow) {
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  GLFWwindow* window = glfwCreateWindow(width, height, "Fractal Drawer", NULL, prevWindow);
  glfwMakeContextCurrent(window);
  glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
  glfwSetKeyCallback(window, Input::KeyCallback);
  glfwSetWindowSizeCallback(window, WindowResizeCallback);
  return window;
}


GLFWwindow* WindowCreator::CreateFullscreenWindow(int width, int height, GLFWwindow* prevWindow) {
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(width, height, "Fractal Drawer", glfwGetPrimaryMonitor(), prevWindow);
  glfwMakeContextCurrent(window);
  glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
  glfwSetKeyCallback(window, Input::KeyCallback);
  glfwSetWindowSizeCallback(window, WindowResizeCallback);
  return window;
}
