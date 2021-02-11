#pragma once

struct Line {
  double x0;
  double y0;
  double x1;
  double y1;

  double GetLengthSquared() const;
};
