#include "Input.h"

int* Input::keysPressed = new int[0];
int Input::keysPressedCount = 0;
int* Input::mouseKeysPressed = new int[0];
int Input::mouseKeysPressedCount = 0;
std::unique_ptr<FractalSkeleton> Input::currentFractal = nullptr;
bool Input::fractalIsReady = false;
bool Input::fractalsAreReadyToBeDeleted = false;
bool Input::cameraIsReadyToBeReset = false;
char Input::predrawnFractalIsReady = 0;
double Input::cameraZoom = 1;

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);

  if (action == GLFW_PRESS) {
    Input::mouseKeysPressedCount++;
    int* _mouseKeysPressed = new int[Input::mouseKeysPressedCount];
    std::copy(Input::mouseKeysPressed, mouseKeysPressed + mouseKeysPressedCount + 1, _mouseKeysPressed);
    delete[] Input::mouseKeysPressed;
    Input::mouseKeysPressed = _mouseKeysPressed;
    Input::mouseKeysPressed[Input::mouseKeysPressedCount - 1] = button;
  } else if (action == GLFW_RELEASE) {
    Input::mouseKeysPressedCount--;
    int* _mouseKeysPressed = new int[Input::mouseKeysPressedCount];
    int i = 0, j = 0;
    for ( ; i < Input::mouseKeysPressedCount + 1; i++) {
      if (Input::mouseKeysPressed[i] == button) continue;
      _mouseKeysPressed[j] = Input::mouseKeysPressed[i];
      j++;
    }
    delete[] Input::mouseKeysPressed;
    Input::mouseKeysPressed = _mouseKeysPressed;
  }

  if (action == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      Input::currentFractal->mainLineIsStarted = false;
      Input::currentFractal->baseLineIsStarted = false;
      if (Input::currentFractal->directionLineIsStarted) {
        Input::currentFractal->directionLineIsStarted = false;
        double x0, y0, x1, y1;
        x0 = Input::currentFractal->directionLineStart[0];
        y0 = Input::currentFractal->directionLineStart[1];
        x1 = (  mouseX / windowW - 0.5f) * 2;
        y1 = (- mouseY / windowW + 0.5f) * 2;
        Line line = {x0, y0, x1, y1};
        Input::currentFractal->directionLines.push_back(line);
      } else {
        Input::currentFractal->directionLineIsStarted = true;
        Input::currentFractal->directionLineStart[0] = (  mouseX / windowW - 0.5f) * 2;
        Input::currentFractal->directionLineStart[1] = (- mouseY / windowW + 0.5f) * 2;
      }
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      Input::currentFractal->directionLineIsStarted = false;
      Input::currentFractal->baseLineIsStarted = false;
      if (Input::currentFractal->mainLineIsStarted) {
        Input::currentFractal->mainLineIsStarted = false;
        double x0, y0, x1, y1;
        x0 = Input::currentFractal->mainLineStart[0];
        y0 = Input::currentFractal->mainLineStart[1];
        x1 = (  mouseX / windowW - 0.5f) * 2;
        y1 = (- mouseY / windowW + 0.5f) * 2;
        Line line = {x0, y0, x1, y1};
        Input::currentFractal->mainLine = line;
      } else {
        Input::currentFractal->mainLineIsStarted = true;
        Input::currentFractal->mainLineStart[0] = (  mouseX / windowW - 0.5f) * 2;
        Input::currentFractal->mainLineStart[1] = (- mouseY / windowW + 0.5f) * 2;
      }
    } else {
      Input::currentFractal->directionLineIsStarted = false;
      Input::currentFractal->mainLineIsStarted = false;
      if (Input::currentFractal->baseLineIsStarted) {
        Input::currentFractal->baseLineIsStarted = false;
        double x0, y0, x1, y1;
        x0 = Input::currentFractal->baseLineStart[0];
        y0 = Input::currentFractal->baseLineStart[1];
        x1 = (  mouseX / windowW - 0.5f) * 2;
        y1 = (- mouseY / windowW + 0.5f) * 2;
        Line line = {x0, y0, x1, y1};
        Input::currentFractal->baseLines.push_back(line);
      } else {
        Input::currentFractal->baseLineIsStarted = true;
        Input::currentFractal->baseLineStart[0] = (  mouseX / windowW - 0.5f) * 2;
        Input::currentFractal->baseLineStart[1] = (- mouseY / windowW + 0.5f) * 2;
      }
    }
  }
}

void Input::KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    Input::keysPressedCount++;
    int* _keysPressed = new int[Input::keysPressedCount];
    std::copy(Input::keysPressed, keysPressed + keysPressedCount + 1, _keysPressed);
    delete[] Input::keysPressed;
    Input::keysPressed = _keysPressed;
    Input::keysPressed[Input::keysPressedCount - 1] = key;
  } else if (action == GLFW_RELEASE) {
    Input::keysPressedCount--;
    int* _keysPressed = new int[Input::keysPressedCount];
    int i = 0, j = 0;
    for ( ; i < Input::keysPressedCount + 1; i++) {
      if (Input::keysPressed[i] == key) continue;
      _keysPressed[j] = Input::keysPressed[i];
      j++;
    }
    delete[] Input::keysPressed;
    Input::keysPressed = _keysPressed;
  }

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
}



bool Input::CheckIfMouseKeyIsPressed (int key) {
  bool result = false;
  for (size_t i = 0; i < Input::mouseKeysPressedCount; i++) {
    if (key == Input::mouseKeysPressed[i]) {
      result = true;
    }
  }
  return result;
}
