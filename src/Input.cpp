#include "Input.h"

std::unique_ptr<FractalSkeleton> Input::currentFractal = nullptr;
Camera* Input::currentCamera = nullptr;
bool Input::fractalIsReady = false;
bool Input::fractalsAreReadyToBeDeleted = false;
bool Input::cameraIsReadyToBeReset = false;
char Input::predrawnFractalIsReady = 0;
bool Input::fullscreenIsReadyToBeToggled = false;

double Input::cameraZoom = 1;

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);


  if (action == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      Input::currentFractal->mainLineIsStarted = false;
      Input::currentFractal->baseLineIsStarted = false;
      if (Input::currentFractal->directionLineIsStarted) {
        Input::currentFractal->directionLineIsStarted = false;
        double x0, y0, x1, y1;
        x0 = Input::currentFractal->directionLineStart[0];
        y0 = Input::currentFractal->directionLineStart[1];
        x1 = currentCamera->x + currentCamera->w * (  mouseX / windowW);
        y1 = currentCamera->y + currentCamera->h * (- mouseY / windowH + 1.0f);
        Line line = {x0, y0, x1, y1};
        Input::currentFractal->directionLines.push_back(line);
      } else {
        Input::currentFractal->directionLineIsStarted = true;
        Input::currentFractal->directionLineStart[0] = currentCamera->x + currentCamera->w * (  mouseX / windowW);
        Input::currentFractal->directionLineStart[1] = currentCamera->y + currentCamera->h * (- mouseY / windowH + 1.0f);
      }
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      Input::currentFractal->directionLineIsStarted = false;
      Input::currentFractal->baseLineIsStarted = false;
      if (Input::currentFractal->mainLineIsStarted) {
        Input::currentFractal->mainLineIsStarted = false;
        double x0, y0, x1, y1;
        x0 = Input::currentFractal->mainLineStart[0];
        y0 = Input::currentFractal->mainLineStart[1];
        x1 = currentCamera->x + currentCamera->w * (  mouseX / windowW);
        y1 = currentCamera->y + currentCamera->h * (- mouseY / windowH + 1.0f);
        Line line = {x0, y0, x1, y1};
        Input::currentFractal->mainLine = line;
      } else {
        Input::currentFractal->mainLineIsStarted = true;
        Input::currentFractal->mainLineStart[0] = currentCamera->x + currentCamera->w * (  mouseX / windowW);
        Input::currentFractal->mainLineStart[1] = currentCamera->y + currentCamera->h * (- mouseY / windowH + 1.0f);
      }
    } else {
      Input::currentFractal->directionLineIsStarted = false;
      Input::currentFractal->mainLineIsStarted = false;
      if (Input::currentFractal->baseLineIsStarted) {
        Input::currentFractal->baseLineIsStarted = false;
        double x0, y0, x1, y1;
        x0 = Input::currentFractal->baseLineStart[0];
        y0 = Input::currentFractal->baseLineStart[1];
        x1 = currentCamera->x + currentCamera->w * (  mouseX / windowW);
        y1 = currentCamera->y + currentCamera->h * (- mouseY / windowH + 1.0f);
        Line line = {x0, y0, x1, y1};
        Input::currentFractal->baseLines.push_back(line);
      } else {
        Input::currentFractal->baseLineIsStarted = true;
        Input::currentFractal->baseLineStart[0] = currentCamera->x + currentCamera->w * (  mouseX / windowW);
        Input::currentFractal->baseLineStart[1] = currentCamera->y + currentCamera->h * (- mouseY / windowH + 1.0f);
      }
    }
  }
}

void Input::KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    Input::fractalIsReady = true;
  }
  if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
    Input::fractalsAreReadyToBeDeleted = true;
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    Input::cameraIsReadyToBeReset = true;
  }
  if (key == GLFW_KEY_Z && action != GLFW_RELEASE) {
    Input::cameraZoom = 0.995;
    // Input::cameraZoom = 0.95;
  }
  if (key == GLFW_KEY_X && action != GLFW_RELEASE) {
    Input::cameraZoom = 1.005555;
    // Input::cameraZoom = 1.05555;
  }
  if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    predrawnFractalIsReady = 1;
  }
  if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
    predrawnFractalIsReady = 2;
  }
  if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
    predrawnFractalIsReady = 3;
  }
  if (key == GLFW_KEY_F && action == GLFW_PRESS) {
    fullscreenIsReadyToBeToggled = true;
  }
}
