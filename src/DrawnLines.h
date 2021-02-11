#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "VertexBuffer.h"
// #include <GLFW/glfw3.h>
#include "src/glfw/glfw-3.3.2/include/GLFW/glfw3.h"



struct DrawnLines {
  static std::vector<std::unique_ptr<VertexBuffer>> up_VertexBuffers;
  static bool newBufferShouldBeAdded;
  static bool newBufferCouldBeAdded;
  static float* LastLineCoordinates;
  static int LastLineCoordinatesSize;

  static void AddMouseCurrentPosition(GLFWwindow* window, const int windowW, const int windowH);
  static void StoreLastLine();
  static void AddBuffer(const void* data, unsigned int size);
};
