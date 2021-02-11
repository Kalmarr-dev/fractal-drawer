#pragma once

#include "Line.h"
#include <vector>
#include <iostream>

class Fractal {
private:
public:
  unsigned int depth;
  // int scaleBase10;
  unsigned int baseLinesCount;
  unsigned int directionLinesCount;
  unsigned int linesCount;
  unsigned int lastReflectionLinesCount;
  Line mainLine;
  Line* baseLines;
  Line* directionLines;
  Line* lines;
  Line* lastReflectionLines;
  std::vector<int> linesOnTheLayer;
  float* vertices;
  unsigned int* indices;

  Fractal(const Line mainLine, const std::vector<Line>& baseLines, const std::vector<Line>& directionLines, const int MAXLINES = (5 + 9 ), const float MINLINESIZE = 0.001);
  // Fractal(const Fractal& fractal, const Line mainLine);
  Fractal(Fractal&& f);
  ~Fractal();
  static int GetLinesCount(const int baseLinesSize, const int directionLinesSize, const int depth);
};
