#include "Camera.h"
#include <algorithm>

Camera::Camera() {
  x = -8.0 / 4.5;
  y = -4.5 / 4.5;
  w = 16.0 / 4.5;
  h = 9.0 / 4.5;
}


void Camera::ZoomToCoordinates(double x, double y, double zoom) {
  double dX, dY;
  dX = (x ) * Camera::w / 2;
  dY = (y ) * Camera::h / 2;
  // dX = x - Camera::x;
  // dY = y - Camera::y;
  Camera::x = Camera::x + Camera::w / 2;
  Camera::y = Camera::y + Camera::h / 2;
  Camera::x = Camera::x + dX - dX / zoom;
  Camera::y = Camera::y + dY - dY / zoom;
  Camera::w /= zoom;
  Camera::h /= zoom;
  Camera::x = Camera::x - Camera::w / 2;
  Camera::y = Camera::y - Camera::h / 2;
}

void Camera::ScaleToMatchScreen(int windowW, int windowH) {
  int smaller = std::min(windowW, windowH);
  double horizontalScaleF = (double)windowW / (double)smaller;
  double verticalScaleF = (double)windowH / (double)smaller;
  double smallerSide = std::min(w, h);
  x = x + w * .5 - smallerSide * horizontalScaleF * .5;
  y = y + h * .5 - smallerSide * verticalScaleF * .5;
  w = smallerSide * horizontalScaleF;
  h = smallerSide * verticalScaleF;
}
