#include "MouseButtonCallback.h"

#include <iostream>

bool mouseButtonRightIsPressed = false;

void MouseButtonPressCallback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    mouseButtonRightIsPressed = true;
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    mouseButtonRightIsPressed = false;
  }
}

void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
  if (GetMouseButtonRightState()) {
    DrawnLines::newBufferCouldBeAdded = true;
    int windowW, windowH;
    glfwGetWindowSize(window, &windowW, &windowH);
    DrawnLines::AddMouseCurrentPosition(window, windowW, windowH);
  } else if (DrawnLines::newBufferCouldBeAdded) {
    DrawnLines::StoreLastLine();
    DrawnLines::newBufferCouldBeAdded = false;
  }
}


void AddMouseCurrentPosition(float*& mousePositionsArr, unsigned int*& mousePositionsIndicesArr, int& mousePositionsArrSize, GLFWwindow* window, const int windowW, const int windowH)
{
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  int oldVASize, newVASize;
  oldVASize = newVASize = mousePositionsArrSize;

  newVASize += 2;

  float* _mousePositionsArr = new float[newVASize];
  std::copy(mousePositionsArr, mousePositionsArr + std::min(oldVASize, newVASize), _mousePositionsArr);
  delete[] mousePositionsArr;
  mousePositionsArr = _mousePositionsArr;
  mousePositionsArr[newVASize - 2] = (  mouseX / windowW - 0.5f) * 2;
  mousePositionsArr[newVASize - 1] = (- mouseY / windowH + 0.5f) * 2;

  unsigned int* _mousePositionsIndicesArr = new unsigned int[newVASize];
  std::copy(mousePositionsIndicesArr, mousePositionsIndicesArr + std::min(oldVASize, newVASize), _mousePositionsIndicesArr);
  delete[] mousePositionsIndicesArr;
  mousePositionsIndicesArr = _mousePositionsIndicesArr;
  mousePositionsIndicesArr[newVASize / 2 - 1] = newVASize / 2 - 1;

  mousePositionsArrSize = newVASize;
}

// void AddMouseCurrentPosition(GLFWwindow* window, const int windowW, const int windowH)
// {
//   double mouseX, mouseY;
//   glfwGetCursorPos(window, &mouseX, &mouseY);
//
//   int newVBSize = DrawnLines::LastLineCoordinatesSize + 2;
//
//   float* _mousePositionsArr = new float[newVBSize];
//   std::copy(DrawnLines::LastLineCoordinates, DrawnLines::LastLineCoordinates + newVBSize, _mousePositionsArr);
//   delete[] DrawnLines::LastLineCoordinates;
//   DrawnLines::LastLineCoordinates = _mousePositionsArr;
//   DrawnLines::LastLineCoordinates[newVBSize - 2] = (  mouseX / windowW - 0.5f) * 2;
//   DrawnLines::LastLineCoordinates[newVBSize - 1] = (- mouseY / windowH + 0.5f) * 2;
//   DrawnLines::LastLineCoordinatesSize += 2;
//   std::cout << DrawnLines::LastLineCoordinatesSize << '\n';
// }

void AddMouseReleaseIndex(std::vector<int>& MouseReleaseIndices, const int& mousePositionsArrSize) {
  if (!MouseReleaseIndices.empty()) {
    MouseReleaseIndices.push_back(mousePositionsArrSize / 2 - 1);
  } else {
    MouseReleaseIndices.push_back(-1);
  }
}

// void AddMouseReleaseIndex() {
//   if (!MouseReleaseIndices.empty()) {
//     MouseReleaseIndices.push_back(mousePositionsArrSize / 2 - 1);
//   } else {
//     MouseReleaseIndices.push_back(-1);
//   }
// }

bool GetMouseButtonRightState() {
  return mouseButtonRightIsPressed;
}
