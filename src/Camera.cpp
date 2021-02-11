#include "Camera.h"


Camera::Camera() {
  x = -1.0;
  y = -1.0;
  w = 2.0;
  h = 2.0;
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
