#include "ScaleRotationMatrix.h"

ScaleRotationMatrix::ScaleRotationMatrix(const Line& a, const Line& b) {
  double aX = a.x1 - a.x0;
  double aY = a.y1 - a.y0;
  double bX = b.x1 - b.x0;
  double bY = b.y1 - b.y0;
  scaledCos = ( (aX * bX) + (aY * bY) ) / a.GetLengthSquared();
  scaledSin = ( (aX * bY) - (aY * bX) ) / a.GetLengthSquared();
}

ScaleRotationMatrix::ScaleRotationMatrix() {}


Line ScaleRotationMatrix::MultiplyByVector(const Line& vector) const {
  double vecX = vector.x1 - vector.x0;
  double vecY = vector.y1 - vector.y0;
  double outVecX = vecX * scaledCos - vecY * scaledSin;
  double outVecY = vecX * scaledSin + vecY * scaledCos;

  Line outVec = {0.0, 0.0, outVecX, outVecY};
  return outVec;
}
