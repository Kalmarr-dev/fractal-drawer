#pragma once

#include "Shapes/Fractal.h"
#include "Camera.h"
#include "ScaleRotationMatrix.h"

#include <vector>
#include <bits/stdc++.h>


struct ExpandingFractal {
  Line mainLine;
  // int parentIndex;
  int reflectionLinesCount;
  Line* reflectionLines;
  Line translationVector;
  ScaleRotationMatrix srm;
  int layer;

  ExpandingFractal(Fractal* f, Line mainLine, int layer = 1);
  // ExpandingFractal(ExpandingFractal& f, Line mainLine);
  ~ExpandingFractal();
  ExpandingFractal(ExpandingFractal&& f);
};


class FractalRegion {
public:
  Fractal* fractal;
  double fractalWidth;
  double fractalHeight;
  bool FractalIsStrictlyDecreasing;
  // unsigned int* drawnIndices;
  std::vector<ExpandingFractal> expandingFractals;
  // std::vector<int> expandingFractalsOnTheLevel;
  std::vector<int> processibleReflectionLinesIndices;
  std::vector<int> expandingFractalsParentIndices;
  int maxExpandingFractalLayer;

  FractalRegion(Fractal* fractal_ptr);

  void UpdateProcessibleReflectionLinesIndices(const Camera& camera, double acceptableScreenSize = 0.1); // 0.004
  bool CreateNesessaryExpandingFractals();
};
