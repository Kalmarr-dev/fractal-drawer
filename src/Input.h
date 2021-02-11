#pragma once

#include "FractalSkeleton.h"

// #include <GLFW/glfw3.h>
#include "glfw/glfw-3.3.2/include/GLFW/glfw3.h"
#include <memory>

struct Input {
  static int* keysPressed;
  static int keysPressedCount;
  static int* mouseKeysPressed;
  static int mouseKeysPressedCount;

  static bool fractalIsReady;
  static bool fractalsAreReadyToBeDeleted;
  static bool cameraIsReadyToBeReset;
  static char predrawnFractalIsReady;

  static double cameraZoom;

  static std::unique_ptr<FractalSkeleton> currentFractal;

  static void MouseButtonCallback (GLFWwindow* window, int button, int action, int mods);
  static void KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods);
  static bool CheckIfMouseKeyIsPressed (int key);
};
