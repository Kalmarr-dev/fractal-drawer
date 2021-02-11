#pragma once

#include "Shapes/Line.h"

struct ScaleRotationMatrix {
public:
  double scaledCos;
  double scaledSin;

  ScaleRotationMatrix(const Line& a, const Line& b);
  ScaleRotationMatrix();
  Line MultiplyByVector(const Line& vector) const;
};
