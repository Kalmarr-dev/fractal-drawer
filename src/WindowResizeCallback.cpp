#include "WindowResizeCallback.h"
#include "State.h"

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
  State::windowWasResized = true;
  State::windowSize = {width, height};
}
