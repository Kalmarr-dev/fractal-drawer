#include "Line.h"

double Line::GetLengthSquared() const {
  double x = (x1 - x0);
  double y = (y1 - y0);
  return (x * x) + (y * y);
}
