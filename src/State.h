#pragma once

#include <GLFW/glfw3.h>
// #include "glfw/glfw-3.3.2/include/GLFW/glfw3.h"

#include <utility>

struct State {
  static bool windowWasResized;
  static std::pair<int, int> windowSize;
  static bool windowIsFullscreen;
};
