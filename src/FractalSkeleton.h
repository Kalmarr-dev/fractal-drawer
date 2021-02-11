#pragma once

#include "Shapes/Line.h"
#include "Shapes/Fractal.h"

#include <vector>

struct FractalSkeleton {
  Line mainLine;
  double mainLineStart[2];
  bool mainLineIsStarted;
  std::vector<Line> baseLines;
  double baseLineStart[2];
  bool baseLineIsStarted;
  std::vector<Line> directionLines;
  double directionLineStart[2];
  bool directionLineIsStarted;

  int depth;

  FractalSkeleton();

  Fractal GenerateFractal(const int& MAXLINES = 100, const float& MINLINESIZE = 0.001);
  Fractal GenerateFractalPreview(const double& mouseX, const double& mouseY, const int depth = 6);
};
