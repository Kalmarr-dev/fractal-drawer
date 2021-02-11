#include "DrawnLines.h"


std::vector<std::unique_ptr<VertexBuffer>> DrawnLines::up_VertexBuffers;

bool DrawnLines::newBufferShouldBeAdded = false;
bool DrawnLines::newBufferCouldBeAdded  = false;

int DrawnLines::LastLineCoordinatesSize = 0;
float* DrawnLines::LastLineCoordinates = new float[DrawnLines::LastLineCoordinatesSize];

void DrawnLines::AddMouseCurrentPosition(GLFWwindow* window, const int windowW, const int windowH)
{
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  int newVBSize = LastLineCoordinatesSize + 2;

  float* _mousePositionsArr = new float[newVBSize];
  std::copy(LastLineCoordinates, LastLineCoordinates + newVBSize, _mousePositionsArr);
  delete[] LastLineCoordinates;
  LastLineCoordinates = _mousePositionsArr;
  LastLineCoordinates[newVBSize - 2] = (  mouseX / windowW - 0.5f) * 2;
  LastLineCoordinates[newVBSize - 1] = (- mouseY / windowH + 0.5f) * 2;
  LastLineCoordinatesSize += 2;
  // std::cout << LastLineCoordinatesSize << '\n';
}

void DrawnLines::StoreLastLine() {
  AddBuffer(LastLineCoordinates, (unsigned int) (LastLineCoordinatesSize * sizeof(float)));
  delete[] LastLineCoordinates;
  LastLineCoordinatesSize = 0;
  LastLineCoordinates = new float[0];
}

void DrawnLines::AddBuffer(const void* data, unsigned int size) {
  std::unique_ptr<VertexBuffer> up_currentLineVB( new VertexBuffer(data, size));
  DrawnLines::up_VertexBuffers.push_back(std::move(up_currentLineVB));
}
