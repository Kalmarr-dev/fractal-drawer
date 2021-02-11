#include "FractalSkeleton.h"


FractalSkeleton::FractalSkeleton() {
  FractalSkeleton::mainLine = {0.9, 0.9, -0.9, 0.9};
  // std::vector<Line> FractalSkeleton::baseLines;
  // std::vector<Line> FractalSkeleton::directionLines;
  FractalSkeleton::mainLineIsStarted = false;
  FractalSkeleton::baseLineIsStarted = false;
  FractalSkeleton::directionLineIsStarted = false;

  FractalSkeleton::depth = -1;
}


Fractal FractalSkeleton::GenerateFractal(const int& MAXLINES, const float& MINLINESIZE) {
  if (directionLines.size() > 0) {
    if (baseLines.size() == 0) {
      baseLines.push_back(mainLine);
    }
    Fractal fractal(mainLine, baseLines, directionLines, MAXLINES, MINLINESIZE);
    return fractal;
  } else {
    Line squareMainLine = {0.9, 0.9, -0.9, 0.9};
    std::vector<Line> squareBaseLines;
    Line baseLine1 = {0.9, 0.9, 0.0, 0.0};
    squareBaseLines.push_back(baseLine1);
    Line baseLine2 = {0.0, 0.0, -0.9, 0.9};
    squareBaseLines.push_back(baseLine2);
    std::vector<Line> squareDirectionLines;
    Line directionLine1 = {0.9, 0.9, 0.3, 0.9};
    Line directionLine2 = {0.3, 0.9, -0.3, 0.9};
    Line directionLine3 = {-0.9, 0.9, -0.9, 0.3};
    Line directionLine4 = {-0.9, 0.3, -0.9, -0.3};
    Line directionLine5 = {-0.3, -0.9, 0.3, -0.9};
    Line directionLine6 = {0.3, -0.9, 0.9, -0.9};
    Line directionLine7 = {0.9, -0.3, 0.9, 0.3};
    Line directionLine8 = {-0.9, -0.3, -0.9, -0.9};
    squareDirectionLines.push_back(directionLine1);
    squareDirectionLines.push_back(directionLine2);
    squareDirectionLines.push_back(directionLine3);
    squareDirectionLines.push_back(directionLine4);
    squareDirectionLines.push_back(directionLine5);
    squareDirectionLines.push_back(directionLine6);
    squareDirectionLines.push_back(directionLine7);
    squareDirectionLines.push_back(directionLine8);
    Fractal serpinskySquare(squareMainLine, squareBaseLines, squareDirectionLines);
    return serpinskySquare;
  }
}

Fractal FractalSkeleton::GenerateFractalPreview(const double& mouseX, const double& mouseY, const int depth) {
  Line _mainLine;
  if (mainLineIsStarted) {
    _mainLine = {mainLineStart[0], mainLineStart[1], mouseX, mouseY};
  } else {
    _mainLine = mainLine;
  }
  std::vector<Line> _baseLines;
  _baseLines = baseLines;
  if (baseLineIsStarted) {
    Line _baseLine = {baseLineStart[0], baseLineStart[1], mouseX, mouseY};
    _baseLines.push_back(_baseLine);
  }
  if (_baseLines.size() == 0) {
    _baseLines.push_back(_mainLine);
  }
  std::vector<Line> _directionLines;
  _directionLines = directionLines;
  if (directionLineIsStarted) {
    Line _directionLine = {directionLineStart[0], directionLineStart[1], mouseX, mouseY};
    _directionLines.push_back(_directionLine);
  }
  Fractal fractal(_mainLine, _baseLines, _directionLines, 100);
  return fractal;
}
