#pragma once

class Camera {
public:
  double x, y, h, w;

  Camera();

  void ZoomToCoordinates(double x, double y, double zoom);
};
